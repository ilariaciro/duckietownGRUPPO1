
#ifndef OPERATIONS_H_
#define OPERATIONS_H_

#include <cv.h>
#include <highgui.h>
#include <string>
#include <stdbool.h>
#include <stdio.h>
#include "opencv2/opencv.hpp"
#include <fstream>
#include <iostream>


using namespace cv;
using namespace std;

class Operations {
private:
	/*	Attributi Privati Classe */
	Mat subImg;
	Mat img, gray_img;
	vector<Rect> images; //bbox vettore
	vector<Mat> vectImg;
	CascadeClassifier cascade; //classificatore


public:
	Operations();
	vector<Mat> detectImage(string path_image, string img, string name_classifier);
	//vector<Mat> detectVideoGenerateXML(string path_video, string name_video, string name_classifier, char* fileXML);
	void detectVideoGenerateXML(string path_video, string name_video, string name_classifier, char* fileXML, bool genXML, CvKNearest KNN, int numberOfNearestKNN);
	void frameVideoCapture(char* path, char* path_dest,int dim);
	Mat cropImage(Rect bbox);
	vector<float> predict_svm_classifier(const char* filename, vector<vector<float> > features);
	void train_svm_classifier(const char* filename_to_save, vector<vector<float> > features);
	void laneDetect(string path_video, string name_video);
};


#endif /* OPERATIONS_H_ */
