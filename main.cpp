#include <iostream>
#include <vector>
#include <string>

#include <opencv\cv.h> //OpenCV 2.4
#include <opencv\highgui.h> //OpenCV 2.4
#include "opencv2\contrib\contrib.hpp" //OpenCV 2.4

#include "_LoadImages.h"
#include "_Eigenfaces.h"
#include "_FaceDetection.h"
#include "tools.h"
#include "constants.h"
std::string face_cascade_name1 = "C:\\FacesDatabase\\haarcascade_frontalface_alt.xml"; //Path to the cascade file
cv::CascadeClassifier face_cascade1;

const cv::Size sizeOfImage(200, 200); //Size of the normalized images
void scanWebcam();

int main()
{
	while(true)
	{
		int choice=0;

		//Menu
		cout << "1 : Learn - 2 : Process - 3 : Exit" << endl;
		cin >> choice;

		//Perform the choosen action
		if(choice==1)
		{
			cout << "Train : " << endl;
			_LoadImages load;
			load.train();
		}
		else if(choice==2)
		{
			scanWebcam();
		}
		else if(choice==3)
		{
			break;
		}
	}

	system("pause");
	return 0;
}




void scanWebcam()
{
	//Check that the cascade file was loaded
	if(!face_cascade1.load( face_cascade_name1 ))
	{
		cout << "Error while loading cascade files" << endl;                            
	} 

	CvCapture* capture;
	cv::Mat frame;

	//Connect to the video stream
	capture = cvCaptureFromFile(PATH_TO_CAM);

	//If the connection was successful 
	if(capture)
	{
		//Create a FaceRecognizer object that uses the Fisherfaces algorithm (also works with the eigenfaces and LBPH algorithms)
		cv::Ptr<cv::FaceRecognizer> fisherfaces = cv::createFisherFaceRecognizer();

		//Load the database that was previously created during the training phase
		cv::FileStorage fs_fisher(PATH_TO_XML_FISHERFACES, cv::FileStorage::READ);
		fisherfaces->load(fs_fisher);

		//Infinite loop to detect the faces continuously
		while(true)
		{
			//Get one picture from the videostream (The facial recognition is done on images from the video and not directly from the videostream)
			frame = cvQueryFrame( capture );
			cv::namedWindow("test");

			//Check that one image was successfully extracted from the video
			if(!frame.empty())
			{
				//Variables used for the id process
				int predictedLabel = -1;
				double predictedConfidence = 0.0;

				std::vector<cv::Rect> faces; //Contains the rectangle coordinates in which the face will be included
				cv::Mat frame_gray; //Grey image
				cvtColor( frame, frame_gray, CV_RGB2GRAY ); //Converts the image from RGB to shades of grey
				equalizeHist( frame_gray, frame_gray ); //Histogram equalization
				
				//We perform a face detection
				face_cascade1.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30, 30) );
				

				//If at least one face was detected then we can perform an identification
				for(int i=0; i<faces.size();i++)
				{
					//Get only (crop) the face (shades of grey)
					cv::Mat croppedFace = frame_gray(cv::Rect(faces[i].x, faces[i].y, faces[i].width, faces[i].height));
					//Resize the image
					cv::resize(croppedFace, croppedFace, sizeOfImage);
					
					//Start the identification
					fisherfaces->predict(croppedFace, predictedLabel, predictedConfidence);
					
					//Print the result in the console
					cout << "##### ID " << predictedLabel << "    confidence : " << predictedConfidence;

					int id=predictedLabel;
					const int THRESHOLD = 1000; //Threshold for the facial recognition. Used to make sure that the face was properly recognized.

					string printedName;

					cv::Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );

					//Print the ID result on the video (it's really bad to do it this way !! A funtion should be created !)
					if(id==1 && predictedConfidence>THRESHOLD)
					{
						printedName="Adrien";
						//Print the circle around the face
						ellipse( frame, center, cv::Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, cv::Scalar(0,255,0), 4, 8, 0);
						//Print the person's name
						cv::putText(frame,printedName, center, cv::FONT_HERSHEY_SIMPLEX, 1.0f, cv::Scalar(0,255,0), 2, 8, false );
					}
					else if(id==2 && predictedConfidence>THRESHOLD)
					{
						printedName="Ofir";
						ellipse( frame, center, cv::Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, cv::Scalar(0,255,0), 4, 8, 0);
						cv::putText(frame,printedName, center, cv::FONT_HERSHEY_SIMPLEX, 1.0f, cv::Scalar(0,255,0), 2, 8, false );
					}
					else if(id==3 && predictedConfidence>THRESHOLD)
					{
						printedName="Jeremie";
						ellipse( frame, center, cv::Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, cv::Scalar(0,255,0), 4, 8, 0);
						cv::putText(frame,printedName, center, cv::FONT_HERSHEY_SIMPLEX, 1.0f, cv::Scalar(0,255,0), 2, 8, false );
					}
					else
					{
						printedName="UNKNOWN";
						ellipse( frame, center, cv::Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, cv::Scalar(0,0,255), 4, 8, 0);
						cv::putText(frame,printedName, center, cv::FONT_HERSHEY_SIMPLEX, 1.0f, cv::Scalar(0,0,255), 2, 8, false );
					}
					
				}
				cout << endl;

				//Print each images to recreate a video
				cv::imshow("test", frame);
			}	
			else
			{
				cout << " --(!) No captured frame -- Break!" << endl;
				break;
			}

			int c = cv::waitKey(10);
		}

	}
}