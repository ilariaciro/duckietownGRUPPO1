/*
 * FeaturesExtraction.h
 *
 *  Created on: 07/mag/2017
 *      Author: ilaria
 */

#ifndef FEATURESEXTRACTION_H_
#define FEATURESEXTRACTION_H_

#include <cv.h>
#include <highgui.h>
#include <string>
#include <stdbool.h>
#include <stdio.h>
#include "opencv2/opencv.hpp"
#include <fstream>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

class FeaturesExtraction {
public:
	FeaturesExtraction();
	virtual ~FeaturesExtraction();
	/*
	 * Calcolo features di colore sulla banda H. Media e deviazione standard.
	 */
	vector<float> computeStdMeanHSV(Mat m, vector<float> features);
	vector<float> computeColorHist(Mat img, vector<float> features);
	vector<float> computeHuMoment(Mat img, vector<float> features);
	vector<float> computeThreeMoment(Mat img, vector<float> features);
	vector<float> computeOtherFeatures(Mat img, vector<float>  features);
	vector<float> computeGradientHogArrows(Mat img, vector<float> features);
	vector<float> computeGradientHogStop(Mat img, vector<float> features);
	vector<float> computeWhiteArrows(Mat img, vector<float> features);
	vector<vector<Point> > filterBlobsByArea(vector<vector<Point> > contours);
	vector<float> computeHog(Mat img, vector <float> features);
};

#endif /* FEATURESEXTRACTION_H_ */
