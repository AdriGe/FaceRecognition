#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

#include <iostream>
#include <string>

using namespace std;



const std::string PATH_TO_LIST("C:\\FacesDatabase\\demo.txt"); //Path to the file that contains the list of images (and their label) to perform the training
const char PATH_TO_CAM[] = "http://192.168.43.1:8080/video?lol.mjpeg"; //IP Camera URL (you can use the "IP Webcam" app on Android)
const std::string PATH_TO_XML_EIGENFACES("C:\\FacesDatabase\\eigenfaces3.xml"); //Path to the XML file that contains the database computed during the training phase (Eigenfaces algorithm)
const std::string PATH_TO_XML_FISHERFACES("C:\\FacesDatabase\\fisherfacesdemo.xml"); //Path to the XML file that contains the database computed during the training phase (Fisherfaces algorithm)
const std::string PATH_TO_XML_LBPH("C:\\FacesDatabase\\LBPH.xml"); //Path to the XML file that contains the database computed during the training phase (LBPH algorithm)

#endif CONSTANTS_H_INCLUDED