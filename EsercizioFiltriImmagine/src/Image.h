/*
 * Image.h
 *
 *  Created on: 18/mar/2017
 *      Author: ilaria
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <cv.h>
#include <highgui.h>
#include <string>
#include <stdbool.h>

using namespace cv;
using namespace std;

class Image {
private:
    int MAX_VALUE = 255;
    int MAX_KERNEL_LENGTH = 31;
	Mat imageGrey;

public:
	Image();
	Mat thresholdImage(Mat image, int value, int threshold_type, string pathDest, string name, bool write);
	Mat adaptiveThresholdImage(Mat image, int adaptiveThresh, int threshold_type, int blocksize, int C, string pathDest, string name,  bool write);
	Mat filter2DLowPass(Mat image, int kernel_size, string pathDest, string name, bool write);
	Mat filter2DHighPass(Mat image, int i, string pathDest, string name, bool write);
	Mat gaussianFilter(Mat image, int sigmax, int sigmay, string pathDest, string name, bool write);
	Mat bilateralFilterImage(Mat image, string pathDest, string name, bool write);

};

#endif /* IMAGE_H_ */
