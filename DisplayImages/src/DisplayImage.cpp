/*
 * DisplayImage.cpp
 *
 *  Created on: 16/mar/2017
 *      Author: Ciro Domenico e Ilaria
 */

#include <cv.h>
#include <highgui.h>
#include <string>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
  Mat image;
  string path="images/";
  string imageName="HappyLittleFish.png";

  image = imread( path+imageName, 1 );

  if(!image.data )
    {
      printf( "No image data \n" );
      return -1;
    }

  namedWindow( "Display Image", CV_WINDOW_AUTOSIZE );
  imshow( "Display Image", image );

  waitKey(0);

  return 0;
}
