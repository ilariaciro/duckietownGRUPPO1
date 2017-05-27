/*
 * Image.cpp
 *
 *  Created on: 18/mar/2017
 *      Author: ilaria
 */

#include "Image.h"

Image :: Image(){

}

Mat Image :: thresholdImage(Mat image, int value, int threshold_type, string pathDest, string name, bool write){

	Mat imageThresh;

	if(!image.data){
		cout<<"No data here"<<endl;
	}
	cvtColor(image, imageGrey, CV_BGR2GRAY);
	threshold(imageGrey, imageThresh, value, MAX_VALUE, threshold_type);

	if(write){
		imwrite(pathDest+name, imageThresh);
	}
	namedWindow("Display_"+name, CV_WINDOW_AUTOSIZE);
	imshow("Display_"+name, imageThresh);
	return imageThresh;
}

Mat Image :: adaptiveThresholdImage(Mat image, int adaptiveThresh, int threshold_type, int blocksize, int C, string pathDest, string name, bool write){
	Mat adaptiveImageThresh;

	if(!image.data){
		cout<<"No data here"<<endl;
	}
	cvtColor(image, imageGrey, CV_BGR2GRAY);
	adaptiveThreshold(imageGrey, adaptiveImageThresh, MAX_VALUE, adaptiveThresh, threshold_type, blocksize, C);

	if(write){
		imwrite(pathDest+name, adaptiveImageThresh);
	}
	namedWindow("Display_"+name, CV_WINDOW_AUTOSIZE);
	imshow("Display_"+name, adaptiveImageThresh);
	return adaptiveImageThresh;
}


Mat Image :: filter2DLowPass(Mat image, int kernel_size, string pathDest, string name, bool write){
	Mat filter2DLowPassImage;
	Mat kernel;

	kernel = Mat::ones( kernel_size, kernel_size, CV_32F )/ (float)(kernel_size*kernel_size);

	    filter2D(image, filter2DLowPassImage, -1 , kernel, Point( -1, -1 ), 0, BORDER_DEFAULT );


	if(write){
			imwrite(pathDest+name, filter2DLowPassImage);
		}
		namedWindow("Display_"+name, CV_WINDOW_AUTOSIZE);
		imshow("Display_"+name, filter2DLowPassImage);

		return filter2DLowPassImage;

}

Mat Image :: filter2DHighPass(Mat image, int i, string pathDest, string name, bool write){
	Mat filter2DHighPassImage;
	Mat kernel = (Mat_<char>(3, 3) << -1, -1, -1,
									  -1,  i, -1,
									  -1, -1, -1);

	filter2D(image, filter2DHighPassImage, image.depth(), kernel);

	if(write){
			imwrite(pathDest+name, filter2DHighPassImage);
		}
		namedWindow("Display_"+name, CV_WINDOW_AUTOSIZE);
		imshow("Display_"+name, filter2DHighPassImage);

		return filter2DHighPassImage;

}

Mat Image :: gaussianFilter(Mat image, int sigmax, int sigmay, string pathDest, string name, bool write){
	Mat gaussianImage;
	for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
	{
		GaussianBlur( image, gaussianImage, Size( i, i ), sigmax, sigmay );

	}
	if(write){
		imwrite(pathDest+name, gaussianImage);
	}
	namedWindow("Display_"+name, CV_WINDOW_AUTOSIZE);
	imshow("Display_"+name,gaussianImage);
	return gaussianImage;

}

Mat Image :: bilateralFilterImage(Mat image, string pathDest, string name, bool write){
	Mat bilateralImage;
	for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
	{
		bilateralFilter(image, bilateralImage, i, i*2, i/2);
	}
	if(write){
		imwrite(pathDest+name, bilateralImage);
	}
	namedWindow("Display_"+name, CV_WINDOW_AUTOSIZE);
	imshow("Display_"+name,bilateralImage);
	return bilateralImage;
}
