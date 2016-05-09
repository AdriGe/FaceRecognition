#include "_LoadImages.h"

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2\contrib\contrib.hpp"

#include <vector>
#include "constants.h"

/* Global variables */
std::string face_cascade_name = "C:\\FacesDatabase\\haarcascade_frontalface_alt.xml"; //Path to the cascade file
cv::CascadeClassifier face_cascade;
std::string window_name = "Capture - Face detection";
cv::RNG rng(12345);

const cv::Size sizeOfImage(200, 200);

using namespace std;



/////////////////////////////////////////////////////////////////
///                  Default Constructor	                  ///
/////////////////////////////////////////////////////////////////
_LoadImages::_LoadImages()
{
	set_numberOfFaces(0);

	//Ask the path to the file that contains the list of files that will be used for the training phase (not used anymore, should be deleted)
	cout << "Please enter the path to the list of images : " << endl; 
}


void _LoadImages::train()
{
	//Create a FaceRecognizer API instance that uses the Fisherfaces algorithm
	cv::Ptr<cv::FaceRecognizer> fisherfaces = cv::createFisherFaceRecognizer();
	
	//Set the path to the list of images that will be used for the training phase
	set_pathToList(PATH_TO_LIST);
	ifstream list(pathToList.c_str());

	//Check if the stream to the file was successfully created
	if(list)
	{
		string readLine;
		
		//Read each line of the file
		while(getline(list, readLine))
		{
			//Split the image path and the id number used for the identification process
			int pos = readLine.find(";");
			cout << endl << readLine << " ";
			int sizeOfPathToImage = pos+1;
			int sizeOfLabel = readLine.size()-pos;

			//Insert the splited data into 2 separate tables
			char* pathToImageChar = new char[sizeOfPathToImage];
			char* labelChar = new char[sizeOfLabel];

			readLine.copy(pathToImageChar, sizeOfPathToImage, 0);
			readLine.copy(labelChar, sizeOfLabel, pos+1);

			pathToImageChar[sizeOfPathToImage-1] = '\0';
			labelChar[sizeOfLabel-1] = '\0';

			string pathToImage(pathToImageChar);

			int label = atoi(labelChar);

			//Load the image into the vector
			loadImage(pathToImage, label);
		}

		//Perform the training using the vectors that were created previously
		fisherfaces->train(listImages, listLabels);

		//Store the database created into an XML file
		cv::FileStorage fs_fisher(PATH_TO_XML_FISHERFACES, cv::FileStorage::WRITE);
		fisherfaces->save(fs_fisher);
		fs_fisher.release();
	}
	else
	{
		//If a problem occured while opening PATH_TO_LIST, it tells it to the user
		cout << "ERROR : The program could not open the requested file." << endl;
	}
}

int _LoadImages::loadImage(string path, int label)
{
	//Check that the cascade file was successfully loaded
	if(!face_cascade.load(face_cascade_name))
	{
		cout << "Error while loading cascade files" << endl;                            
	} 

	//Multiply antislashes
	replaceChar(path, "\\", "\\\\");

	cv::Mat image = cv::imread(path, CV_LOAD_IMAGE_COLOR);

	//On vérifie que des images ont été détectées
	//Check that images were successfully detected
	if(image.size().width!=0)
	{
		//Create intermediate variables that will be used to normalize the images
		std::vector<cv::Rect> faces; //Contains the face(s) coordinates inside the image (rectangle in which the face will be included)
		cv::Mat frame_gray; //Shades of grey empty image
		cvtColor( image, frame_gray, CV_BGR2GRAY ); //Convert the image to shades of grey
		equalizeHist( frame_gray, frame_gray ); //Histogram equalization
	
		//Facial recognition
		face_cascade.detectMultiScale( frame_gray, faces, 1.5, 2, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30, 30) );
		cv::Mat croppedFace; //Only contains the face
		cout << faces.size() << " face(s) detected" << endl;

		//Normalization done only if one and only on face was detected
		//If the image does not contain any face then it's useless to process it
		//If it contains more than one face we can't tell who should be added to the database
		if(faces.size()==1) 
		{
			//Crop the image to only get the face
			croppedFace = frame_gray(cv::Rect(faces[0].x, faces[0].y, faces[0].width, faces[0].height));
					
			//Resize the image
			cv::resize(croppedFace, croppedFace, sizeOfImage);

			//Insert the normalized image and its id number into 2 different vectors
			listImages.push_back(croppedFace);
			listLabels.push_back(label);

			//Set how many faces are contained in the DB
			set_numberOfFaces(get_numberOfFaces()+1);
		
			//Print the normalized images
			cv::imshow("test", croppedFace);
			cv::waitKey(5);
		}
	}
	else
	{
		cout << " No image found at this location." << image.size().width <<endl;
	}
	return 0;
}


void _LoadImages::replaceChar(string& path, string charToFind, string charToReplace)
{
	//The path to the image starts at this position
	size_t pos = 0;
	
	//While the end was not reached
	while((pos = path.find(charToFind, pos)) != std::string::npos)
	{
		//Replace the string
		path.replace(pos, charToFind.length(), charToReplace);
		pos += charToReplace.length();
	}
}
