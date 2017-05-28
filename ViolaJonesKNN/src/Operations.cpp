#include "Operations.h"
#include "FeaturesExtraction.h"

using namespace std;

Operations :: Operations(){

}

vector<Mat> Operations :: detectImage(string path_image, string name_img, string name_classifier){
	vector<Mat> vectImg;
	/*Caricamento classificatore dall'attributo privato cascade*/
	if (!cascade.load(name_classifier)) {
		cout << "Errore caricamento classificatore" << endl;
	}
	img = imread(path_image + name_img, 1 );
	if(!img.data)
		cout<<"Errore caricamento immagine"<<endl;

	cvtColor(this->img, this->gray_img, CV_BGR2GRAY);
	//Applicazione VJ
	cascade.detectMultiScale(this->gray_img, images, 1.1, 5, 0, cvSize(30, 30), cvSize(800, 800));
	//Disegno bbox
	for (int i = 0; i < images.size(); i++)
	{
		Point pt1(images[i].x + images[i].width,
				images[i].y + images[i].height);
		Point pt2(images[i].x, images[i].y);
		subImg=cropImage(images[i]);
		/*Salvataggio frame tagliati*/
		vectImg.push_back(this->subImg);
		rectangle(img, pt1, pt2, cvScalar(0, 0, 255), 2, 8, 0);
	}
	//	namedWindow( "Result Image", CV_WINDOW_AUTOSIZE );
	//	imshow("Result Image", img);
	//	waitKey(0);

	return vectImg;
}

void Operations :: detectVideoGenerateXML(string path_video, string name_video, string name_classifier, char* fileXML, bool genXML, CvKNearest KNN, int numberOfNearestKNN){
	ofstream stream;
	FeaturesExtraction extractor;
	Operations op;

	/* Caricamento Classificatore*/
	if (!cascade.load(name_classifier))
		cout << "Errore caricamento classificatore" << endl;

	VideoCapture cap (path_video+name_video);
	if (!cap.isOpened()) {
		cout << "Loading Error!" << endl;
		exit(-1);
	}

	if (genXML) {
		if( remove(fileXML) == 0 )
			cout << "File sovrascritto" << endl;

		stream.open(fileXML, ofstream::app);
		stream << "<xml ver=\"1.0\" encode=\"UTF-8\">\n";
		stream.close();
	}

	int id = 1;
	int index = 0;

	while (true) {
		/*@grab = lettura frame dopo frame*/
		if (cap.grab()) {
			/*@retrive= ritorna il frame grabbato del video*/
			cap.retrieve(this->img);
			cvtColor(this->img, this->gray_img, CV_BGR2GRAY);

			//Applicazione VJ
			cascade.detectMultiScale(this->gray_img, this->images, 1.1, 5, 0, cvSize(90, 90), cvSize(800, 800));

			//Disegno bbox sul frame grabbato del video
			for (int i = 0; i < this->images.size(); i++) {
				Point pt1(this->images[i].x + this->images[i].width,
						this->images[i].y + this->images[i].height);
				Point pt2(this->images[i].x, this->images[i].y);
				subImg=cropImage(images[i]);

				/*
				 * Inserimneto features interessate
				 */
				vector<float> features;
				features = extractor.computeGradientHogStop(subImg, features);
				features = extractor.computeGradientHogArrows(subImg, features);

				float res;
				Mat vetFeatures(1, features.size(), CV_32FC1);
				for(int i=0; i<features.size();i++)
					vetFeatures.at<float>(0,i)=features.at(i);

				res=KNN.find_nearest(vetFeatures,numberOfNearestKNN);

				/*Salvataggio frame tagliati*/
				this->vectImg.push_back(this->subImg);
				string sign_name;
				float sign_num;


				if((int)res == 0){
					sign_name = "Left";
					sign_num = 3;
					rectangle(this->img, pt1, pt2, cvScalar(0, 255, 0), 2, 8, 0);
				}
				else if ((int)res == 1){
					sign_name = "Right";
					sign_num = 2;
					rectangle(this->img, pt1, pt2, cvScalar(255, 0, 0), 2, 8, 0);
				}else if ((int)res == 2) {
					sign_name = "Stop";
					sign_num = 0;
					rectangle(this->img, pt1, pt2, cvScalar(0, 0, 255), 2, 8, 0);
				} else if ((int)res == 3){
					sign_name = "Forward";
					sign_num = 1;
					rectangle(this->img, pt1, pt2, cvScalar(255, 255, 0), 2, 8, 0);
				}
				else
					rectangle(this->img, pt1, pt2, cvScalar(255, 255, 255), 2, 8, 0);

				if (genXML) {
					stream.open(fileXML, ofstream::app);
					stream << "<object framespan=\"" << id << ":" << id << "\" id=\"" << sign_num << "\" name=\"" << sign_name << "\">\n"
							"\t<attribute name=\"Name\">\n"
							"\t<data-svalue value=\"" << sign_name << "\"/>\n"
							"\t</attribute>\n"
							"\t<attribute name=\"Location\">\n"
							"\t<data-bbox framespan=\"" << id << ":" << id << "\" height=\"" << this->images[i].height << "\" width=\"" << this->images[i].width << "\""
							" x=\"" << this->images[i].x << "\" y=\"" << this->images[i].y << "\"/>\n"
							"\t</attribute>\n"
							"</object>\n";
					stream.close();
				}
				index++;
			}
			/*Visualizzazioen risultato VJ*/
			namedWindow( "Result", CV_WINDOW_AUTOSIZE );
			imshow("Result", this->img);
			waitKey(25);

		} else {
			/*chiusura forzata del video senza attendere il comnado dell'utente*/
			destroyWindow("Result");
			cap.release();

			break;
		}
		id++;
	}

	if (genXML) {
		stream.open(fileXML, ofstream::app);
		stream << "</xml>";
		stream.close();
	}

}

void Operations :: frameVideoCapture(char* path, char* path_dest,int dim){

	cvNamedWindow( "DisplayVideo", CV_WINDOW_AUTOSIZE );
	CvCapture* capture = cvCreateFileCapture(path);
	IplImage* frame;
	int num=0;
	/*salvataggio frame come caratteri*/
	char s [dim];

	while(1) {
		num++;
		frame = cvQueryFrame( capture );
		if( !frame ) break;
		cvShowImage( "DisplayVideo", frame );
		cvWaitKey(33);//lettura frame ogni 33 millisecondi
		/*salvataggio frame*/
		sprintf(s,"%sframe%d.jpg",path_dest,num);
		cvSaveImage(s,frame);
	}
	cvReleaseCapture( &capture );
	cvDestroyWindow("DisplayVideo");
}

Mat Operations:: cropImage(Rect bbox){
	Mat subImg;

	/*Rettangolo bbox*/
	Rect roi(bbox.x, bbox.y, bbox.width, bbox.height);
	/*Ritaglio immagine all'interno dell'immagine*/
	subImg = img(roi);
	return subImg;
}

void Operations:: laneDetect(string path_video, string name_video){
	Mat mask,HSV,greyImg;

	//////////////APRO LO STREAM VIDEO//////////////////
	VideoCapture cap (path_video+name_video);
	//cap.set(CV_CAP_PROP_FOURCC,CV_FOURCC('H','2','6','4'));
	if (!cap.isOpened()) {
		cout << "Loading Error!" << endl;
		exit(-1);
	}

	while(true){
		if (cap.grab()) {
			////// INIZIO IL FILTRAGGIO SULLA BANDA HSV////////////////////
			cap.retrieve(this->img);
			//mask=preprocessingLineDetection(mask);
			cvtColor(this->img, HSV, COLOR_BGR2HSV);
			cvtColor(this->img,greyImg, COLOR_BGR2GRAY);
			Mat1b yellow_mask;
			Mat1b white_mask;
			inRange(HSV, Scalar(20, 100, 100), Scalar(30, 255, 255), yellow_mask);
			inRange(greyImg, 200, 255,white_mask);
			bitwise_or(yellow_mask,white_mask,mask);
			bitwise_and(greyImg,mask,mask);

			GaussianBlur(mask,mask,Size(5,5),10,10,2);
			Canny(mask,mask,50,150,3);

			/* MANCA LA PARTE RELATIVA AL ROI (region of interest)
			 * PERCHÈ NEL VIDEO FATTO NON ABBIAMO L'ORIZZONTE!
			 * NEL CASO IN CUI IL VIDEO HA L'ORIZZONTE BISOGNA CONSIDERARE
			 * UN RETTANGOLO CHE PRENDE IN CONSIDERAZIONE SOLO LA PARTE IN BASSO
			 * PRIMA DEL ROI DEFINIRE I POINTS DELL'IMMAGINE IN MODO DA CONSIDERARE
			 * SOLO LA PARTE BASSA
			 */

			/*HoughLinesP PARAMETRI:
			 il 5o parametro e' la sensibilità: piu e' basso piu facilemte trova linee (parametro standard 50)
			 il 6o parametro è il numerominimo di punti che devono costituore una linea, scarta i falsi positivi(?) (parametro standard 50)
			 il 7o parametro e' il gap: distanza max tra due punti che possono ancora considerati appartenenti linea (evita linee 'spezzate') (parametro standard 10)
			 */
			vector<Vec4i> lines;
			HoughLinesP(mask, lines,1,CV_PI/180,50,50,10);

			for( size_t i = 0; i < lines.size(); i++ ){
				Vec4i l = lines[i];
				line( img, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
			}

			namedWindow( "Result", CV_WINDOW_AUTOSIZE );
			imshow("Result", mask);
			namedWindow( "Hough", CV_WINDOW_AUTOSIZE );
			imshow("Hough", img);
			waitKey(25);
		}
	}
}
