#ifndef LOADIMAGES_H_INCLUDED
#define LOADIMAGES_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <opencv\cv.h>
#include <opencv\highgui.h>


class _LoadImages
{
  public:
	_LoadImages();


	int loadImage(std::string path, int label);
	void train();
	void replaceChar(std::string& path, std::string charToFind, std::string charToReplace);

	//_____________________________Getters and Setters_____________________________
	bool set_pathToList(std::string path) { pathToList=path; return true; }
	std::string get_pathToList() { return pathToList; }
	//_____________________________________________________________________________________
	std::vector<cv::Mat> get_listImages() { return listImages; }
	std::vector<int> get_listLabels() { return listLabels; }
	//_____________________________________________________________________________________
	bool _LoadImages::set_numberOfFaces(unsigned int nbOfFaces){ if(nbOfFaces>=0) { numberOfFaces=nbOfFaces; return true; } else { numberOfFaces=0; return false; }}
	unsigned int get_numberOfFaces() { return numberOfFaces; }
	

  protected:
	std::string pathToList;
	std::vector<cv::Mat> listImages;
	std::vector<int> listLabels;
	int numberOfFaces;
};


#endif LOADIMAGES_H_INCLUDED