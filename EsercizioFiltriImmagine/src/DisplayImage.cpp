/*
 * DisplayImage.cpp
 *
 *  Created on: 16/mar/2017
 *      Author: Ciro Domenico e Ilaria
 */

#include "Image.h"


int main(int argc, char** argv) {
	string path="images/";
	string imageName="1.png";

	Mat image;
	Mat imageThresh;
	Mat adaptiveImageThresh;
	Mat filter2DLowPassImage;
	Mat filter2DHighPassImage_9;
	Mat filter2DHighPassImage_8;
	Mat gaussianImage;
	Mat bilateralImage;

	int threshold_type = 3;
	int value=100;
	int blocksize = 3;
	double C = 1;

	int kernel_size=10;
	int sigmax = 50;
	int sigmay = 50;

	Image img;


	image = imread(path+imageName, CV_LOAD_IMAGE_COLOR);
	if(!image.data)
		{
			cout<<"AH! No data here"<<endl;
			return -1;
		}

	namedWindow("Display_"+imageName, CV_WINDOW_AUTOSIZE);
	imshow("Display_"+imageName, image);

	/*THRESHOLD*/
	imageThresh=img.thresholdImage(image,value,threshold_type,path,"Thresh.png",true);
	adaptiveImageThresh=img.adaptiveThresholdImage(image,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY, blocksize, C,path,"Adaptive_Thresh.png", true);

	/*FILTRI*/
	filter2DHighPassImage_9=img.filter2DHighPass(image, 9, path, "Filter2D_High_Pass_9.png", true);
	filter2DHighPassImage_9=img.filter2DHighPass(image, 8, path, "Filter2D_High_Pass_8.png", true);
	filter2DLowPassImage=img.filter2DLowPass(image, kernel_size, path, "Filter2D_Low_Pass.png", true);
	gaussianImage=img.gaussianFilter(image, sigmax, sigmay, path, "Gaussian_Blur.png", true);
	bilateralImage=img.bilateralFilterImage(image, path, "Bilateral_Blur.png", true);

	waitKey(0);
	return 0;
}

