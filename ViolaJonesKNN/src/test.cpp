#include "FeaturesExtraction.h"
#include "Operations.h"
#define NUM_CLASS 4
#define NUM_SAMPLES 100
#define NUM_FEATURES 81*2


int main(int argc, char* argv[]) {

	Operations op;
	string video_path = "Video/";
	string video_name = "1.h264";
	cout<<"Ciao"<<endl;
	op.laneDetect(video_path,video_name);
/*
	FeaturesExtraction extractor;

	char* fileXML = "res/GuariniI.xml";
	string classifier = "Domcascade.xml"; //Detecting//
	string video_path = "Video/";
	string test_video_name = "test.mpg";
	string result_file_name = "/res/results.txt";
	string path_photo[] ={ "Signals/left/frame (","Signals/right/frame (","Signals/stop/frame (","Signals/forward/frame ("};


	int row=NUM_CLASS; // numero di campioni
	int col=NUM_FEATURES; // dimensione vettore features
	int numberOfNeirest=2;

	Mat trainData = Mat(row*NUM_SAMPLES, col, CV_32FC1); // matrice che per ogni riga contiene le features dell'immagine del trainig set
	Mat trainClasses = Mat(row*NUM_SAMPLES, 1, CV_32FC1 ); // vettore colonna che identifica la classe di appartenenza

	vector<Mat> vectVideoFrame;
	vector<float>  features, features2;

	////////////////////////////// TRAIN //////////////////////////////
	for (int i = 0 ; i < NUM_CLASS; i++) {
		for (int j = 1; j < NUM_SAMPLES; j++){
			stringstream name;
			name << j << ").jpg";
			Mat img = imread(path_photo[i]+name.str(), 1);
			features = extractor.computeGradientHogStop(img, features);
			features = extractor.computeGradientHogArrows(img, features);
			for(int z=0; z<NUM_FEATURES;z++){
				trainData.at<float>(i*NUM_SAMPLES+(j-1),z)=features.at(z);
			}

			trainClasses.row(i*NUM_SAMPLES+(j-1)).col(0)=i;
			features.clear();
		}
		cout << "Fine estrazione features dalla cartella: " << i+1 <<endl;
	}

	CvKNearest KNN = CvKNearest();
	KNN.train(trainData, trainClasses);
	////////////////////////////// DECISION //////////////////////////////
	op.detectVideoGenerateXML(video_path, test_video_name, classifier, fileXML, false, KNN, numberOfNeirest);
*/
	printf("\nFine");
	return 0;
}
