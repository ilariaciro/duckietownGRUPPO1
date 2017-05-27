/*
 * FeaturesExtraction.cpp
 *
 *  Created on: 07/mag/2017
 *      Author: ilaria
 */

#include "FeaturesExtraction.h"

FeaturesExtraction::FeaturesExtraction() {
	// TODO Auto-generated constructor stub

}

FeaturesExtraction::~FeaturesExtraction() {
	// TODO Auto-generated destructor stub
}

/*
 * Estrazione features di colore usando la banda H della rappresentazione HSV (indipendente dalla luminosità).
 * input:
 * 	- Mat m: rappresenda il ritaglio della porzione di immagine dal quale si vogliono estrare le features.
 * 	- vector<float> features: è il vettore di float che viene riempito dei valori numerici
 *  - output:
 *  -vector<float> features: è il vettore di float che viene riempito dei valori numerici
 */
vector<float> FeaturesExtraction::computeStdMeanHSV(Mat m, vector<float> features){
	Mat imgMean, imgStd, mHSV, HBand;
	cvtColor(m,mHSV,COLOR_BGR2HSV);

	extractChannel(mHSV,HBand,0);
	meanStdDev(HBand,imgMean,imgStd);
	//	namedWindow("HBand",WINDOW_AUTOSIZE);
	//	imshow("HBand",HBand);
	//	waitKey(0);
	features.push_back(imgMean.at<double>(0,0));
	features.push_back(imgStd.at<double>(0,0));
	return features;
}


vector<float>  FeaturesExtraction :: computeGradientHogStop(Mat img, vector<float> features){
	Mat HSV;
	vector<float> tmp;
	//the dst image size,e.g.100x100
	Size size(20,20);
	resize(img,img,size);//resize image
	// convertire RGB image in HSV
	cvtColor(img, HSV, COLOR_BGR2HSV);
	Mat1b lower_red_hue_range;
	Mat1b upper_red_hue_range;
	/**
	 * Range delle bande del rosso isolate per riconoscere gli Stop
	 * Tali bande sono due, essendo in HSV presenti due bande per il colore rosso
	 * spostando i bound si determinano tonalità di rosso differenti
	 */
	inRange(HSV, Scalar(0, 90, 90), Scalar(10, 255, 255), lower_red_hue_range);
	inRange(HSV, Scalar(160, 100, 100), Scalar(179, 255, 255), upper_red_hue_range);
	Mat1b bw = lower_red_hue_range | upper_red_hue_range;
	Scalar hsv_l(0, 100, 100);
	Scalar hsv_h(10, 255, 255);
	/*
	 * erosione dell'immagine per ridurre la presenza dei bianchi non appartenenti al segnale interessato.
	 */
	erode(bw, bw, Mat(), Point(-1, -1), 2, 1, 1);
	/*
	 * dilatazione aumento dei bianchi all'interno dell'immagine
	 */
	dilate(bw, bw, Mat(), Point(-1, -1), 1, 1, 1);
	HOGDescriptor hog(Size(20,20), Size(10,10), Size(5,5), Size(10,10),9, 1, 4, 0, 0.2, true,64);
	hog.compute(bw, tmp);
	//		namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
	//		imshow("Contours", bw );
	//		waitKey(0);
	for (int i=0; i<tmp.size();i++)
		features.push_back(tmp.at(i));
	return features;
}

vector<float> FeaturesExtraction :: computeHog(Mat img, vector <float> features){
	vector<float> tmp;
	/*
	 * il primo parametro è un'immagine di dimensione 20x20 per questo si effettua il resize
	 * l'otput è un vettore di 81 elementi componenti l'istogramma dei gradienti
	 */
	//the dst image size,e.g.100x100
	Size size(20,20);
	resize(img,img,size);//resize image
	HOGDescriptor hog(Size(20,20), Size(10,10), Size(5,5), Size(10,10),9, 1, 4, 0, 0.2, true,64);
	cvtColor(img, img, CV_BGR2GRAY);
	hog.compute(img, tmp);
	for (int i=0; i<tmp.size();i++)
		features.push_back(tmp.at(i));
	return features;
}

vector<float>  FeaturesExtraction :: computeGradientHogArrows(Mat img, vector<float> features){
	Mat HSV, kernel,kernelDil, imgGrey, bw;
	vector<float> tmp;
	Size size(20,20);//the dst image size,e.g.100x100

	resize(img,img,size); //resize image
	// convert RGB image to HSV
	cvtColor(img, HSV, CV_RGB2HSV);
	GaussianBlur(HSV,HSV,Size(3,3),3);
	cvtColor(img,imgGrey,CV_RGB2GRAY);
	threshold(imgGrey,bw,0,255,THRESH_OTSU);
	//default kernel ellisse size(5,5) per 100 100 d i resize

	kernel = getStructuringElement(MORPH_ELLIPSE ,Size(3,2),Point(-1,-1) );
	erode(bw, bw, kernel);
	//winSize rilevazione dimensione finestra
	//blocksize dimensione blocco in pixel
	//blockStride, deve essere un multiplo della dimensione della cella (quella di sotto)
	//cellSize, dimensione cella
	//nbins,
	//derivAper,
	//winSigma,
	//histogramNormType,
	//L2HysThresh,
	//gammal correction,
	//nlevels=64
	//Use signed gradients

	HOGDescriptor hog(Size(20,20), Size(10,10), Size(5,5), Size(10,10),9, 1, 4, 0, 0.2, true,64);
	hog.compute(bw, tmp);
	for (int i=0; i<tmp.size();i++)
		features.push_back(tmp.at(i));
	//	namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
	//	imshow( "Contours", bw );
	//	waitKey(0);
	return features;
}

vector<float> FeaturesExtraction :: computeColorHist(Mat img, vector<float> features){
	Size newresize(100,100);//the dst image size,e.g.100x100
	resize(img,img,newresize);//resize image
	Mat hsv_img;
	vector<float> result;
	cvtColor(img, hsv_img, CV_BGR2HSV);
	Mat histogram[9];
	int size = (hsv_img.rows-2)/3;

	int hbins = 30, sbins = 32;
	int histSize[] = {hbins, sbins};
	// hue varies from 0 to 179, see cvtColor
	float hranges[] = { 0, 180 };
	// saturation varies from 0 (black-gray-white) to
	// 255 (pure spectrum color)
	float sranges[] = { 0, 256 };
	const float* ranges[] = { hranges, sranges };
	int channels[] = {0, 1};

	for (int k = 0; k < 3; ++k)
		for (int l = 0; l < 3; ++l)
		{
			Rect tile(size*k, size*l, size, size);
			Mat img_tile = hsv_img(tile);
			calcHist(&img_tile, 1, channels, Mat(), histogram[3*k + l], 2, histSize, ranges, true, false);
			normalize( histogram[3*k + l], histogram[3*k + l], 1, 0, NORM_L2, -1, Mat());
		}

	for (int i = 0; i < 9; ++i)
		for(int k = 0; k< histogram[i].rows; ++k)
			for (int j = 0; j < histogram[i].cols; ++j)
				features.push_back(histogram[i].at<float>(k,j));
	return features;
}

vector<vector<Point> > FeaturesExtraction::filterBlobsByArea(vector<vector<Point> > contours){
	int minArea = 500;
	vector<vector<Point> > filtContours;
	int currArea = -1;
	for(int i=0;i<contours.size();i++){
		currArea = contourArea(contours[i]);
		if(currArea>minArea)
			filtContours.push_back(contours[i]);
	}
	return filtContours;
}

vector<float> FeaturesExtraction :: computeHuMoment(Mat img, vector<float>  features){
	Mat HSV, kernel,kernelDil, imgGrey, bw;
	Size size(200,200);//the dst image size,e.g.100x100
	resize(img,img,size);//resize image
	// convert RGB image to HSV
	cvtColor(img, HSV, CV_RGB2HSV);
	GaussianBlur(HSV,HSV,Size(3,3),3);
	cvtColor(img,imgGrey,CV_RGB2GRAY);
	threshold(imgGrey,bw,0,255,THRESH_OTSU);

	//default kernel ellisse size(5,5) per 100 100 d i resize
	kernel = getStructuringElement(MORPH_ELLIPSE ,Size(14,14),Point(-1,-1) );
	erode(bw, bw, kernel);
	kernel = getStructuringElement(MORPH_RECT ,Size(2,20),Point(-1,-1) );
	erode(bw, bw, kernel);

	/*Estrazione contorni e filtro in base all'area*/
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// Find contours
	findContours( bw, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	contours = filterBlobsByArea(contours);

	Mat drawing = Mat::zeros( bw.size(), CV_8UC3 );
	for( int i = 0; i< contours.size(); i++ )
	{
		Scalar color = Scalar( 255, 255, 255 );
		drawContours( drawing, contours, i, color, CV_FILLED, 8, hierarchy, 0, Point() );

	}
	/// Show in a window
	//	namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
	//	imshow( "Contours", drawing );
	//	waitKey(0);
	/*
	 * Calcolo baricentro
	 */
	Mat imBw= Mat::zeros( bw.size(), CV_8UC1 );
	cvtColor(drawing, imBw, CV_BGR2GRAY);

	Moments mom1 = moments(imBw);
	Point centroid = Point(mom1.m10/mom1.m00 , mom1.m01/mom1.m00 );
	circle(drawing, centroid,  1, Scalar(0,0,255),3);
	//	namedWindow( "Point on view", CV_WINDOW_AUTOSIZE );
	//	imshow( "Point on view", drawing );
	//	waitKey(0);

	/* Classificazione freccia */
	Point centerOfImage = Point(bw.size().height/2, bw.size().width/2);
	features.push_back(centroid.x);
	features.push_back(centroid.y);
	return features;
}

vector<float> FeaturesExtraction :: computeThreeMoment(Mat img, vector<float> features){
	Size size(100,100);//the dst image size,e.g.100x100
	resize(img,img,size);//resize image
	Mat HSV, bw;
	// convert RGB image to HSV
	cvtColor(img, HSV, CV_RGB2HSV);
	Scalar hsv_l(0, 0, 200);
	Scalar hsv_h(180, 255, 255);
	inRange(HSV, hsv_l, hsv_h, bw);
	erode(bw, bw, Mat(), Point(-1, -1), 2, 1, 1);
	//	imshow("Specific Colour",bw);
	//	waitKey(0);

	Moments mom =  moments(bw);
	// central normalized moments
	features.push_back(mom.nu20);
	return features;
}

vector<float> FeaturesExtraction :: computeOtherFeatures(Mat img, vector<float> features){
	Size size(100,100);//the dst image size,e.g.100x100
	resize(img,img,size);//resize image
	//Lunghezza della curva
	//features.push_back((float)arcLength(contour[0], true));
	//Area del contorno
	//features.push_back((float)contourArea(contour[0], false));
	//Raggio del minimo cerchio
	//Point2f center;
	//float radius;
	//minEnclosingCircle(contour[0], center, radius);
	//features.push_back(radius);
	//Pixel bianchi
	//vector<Point> white_pixels;
	Mat HSV;
	// convert RGB image to HSV
	cvtColor(img, HSV, COLOR_BGR2HSV);
	Mat1b lower_red_hue_range;
	Mat1b upper_red_hue_range;

	inRange(HSV, Scalar(0, 100, 100), Scalar(10, 255, 255), lower_red_hue_range);
	inRange(HSV, Scalar(160, 100, 100), Scalar(179, 255, 255), upper_red_hue_range);
	Mat1b bw = lower_red_hue_range | upper_red_hue_range;
	Scalar hsv_l(0, 100, 100);
	Scalar hsv_h(10, 255, 255);
	erode(bw, bw, Mat(), Point(-1, -1), 2, 1, 1);
	dilate(bw, bw, Mat(), Point(-1, -1), 2, 1, 1);;
	float res = (float) countNonZero(bw);  //
	//	imshow("Specific Colour",bw);
	//	waitKey(0);
	features.push_back(res);
	return features;
}

vector<float> FeaturesExtraction :: computeWhiteArrows(Mat img, vector<float> features){

	Mat HSV, bw;
	vector<Mat> hsv_planes;
	// convert RGB image to HSV
	cvtColor(img, HSV, CV_RGB2HSV);


	Scalar hsv_l(0, 0, 200);
	Scalar hsv_h(180, 255, 255);
	inRange(HSV, hsv_l, hsv_h, bw);
	erode(bw, bw, Mat(), Point(-1, -1), 2, 1, 1);
	dilate(bw, bw, Mat(), Point(-1, -1), 2, 1, 1);
	//	imshow("Specific Colour", bw);
	//	waitKey(0);

	//Estrai features 4 quadranti
	Rect top_left(Point(0, 0), Size(bw.size().height/2, bw.size().width/2));
	Rect bottom_left(Point(0, bw.size().width/2), Size(bw.size().height/2, bw.size().width/2));
	Rect top_right(Point(bw.size().height/2, 0), Size(bw.size().height/2, bw.size().width/2));
	Rect bottom_right(Point(bw.size().height/2, bw.size().width/2), Size(bw.size().height/2, bw.size().width/2));

	Mat top_right_quadrant;
	Mat top_left_quadrant;
	Mat bottom_right_quadrant;
	Mat bottom_left_quadrant;

	top_right_quadrant = bw(top_right);
	top_left_quadrant = bw(top_left);
	bottom_right_quadrant =bw(bottom_right);
	bottom_left_quadrant = bw(bottom_left);
	imshow("top_right_quadrant ", top_right_quadrant );
	waitKey(0);
	imshow("top_left_quadrant", top_left_quadrant);
	waitKey(0);
	imshow("bottom_right_quadrant", bottom_right_quadrant);
	waitKey(0);
	imshow("bottom_left_quadrant", bottom_left_quadrant);
	waitKey(0);

	float res1 = (float) countNonZero(top_right_quadrant);
	float res2 = (float) countNonZero(top_left_quadrant);
	float res3 = (float) countNonZero(bottom_right_quadrant);
	float res4 = (float) countNonZero(bottom_left_quadrant);
	features.push_back(res1);
	features.push_back(res2);
	features.push_back(res3);
	features.push_back(res4);
	return features;
}
