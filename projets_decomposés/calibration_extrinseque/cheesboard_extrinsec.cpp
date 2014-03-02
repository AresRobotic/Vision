#include "opencv2/opencv.hpp"
#include <iostream>
#include "../myfunctions.hpp"
#include <string>
 

using namespace cv;
using namespace std;
/*
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
 */



int main( int argc, char* argv[])
{
 // std::vector<Point2f> imagePoints ;
  std::vector<Point3f> objectPoints ;
  Mat tvec(3,1,DataType<double>::type);
  Mat cameraMatrix(3,3,cv::DataType<double>::type) ;
  Mat rotationMatrix(3,3,cv::DataType<double>::type);
  Size chessSize = cvSize(9,6) ;
  
  generate3DPointsFromCheesboard(chessSize,3.0,objectPoints);
	
	
	std::vector<Point2f> ptvec ;
	Mat img = imread("chessboard.png");//, CV_LOAD_IMAGE_GRAYSCALE);
	
	generate2DPointsFromCheesboard(img,chessSize,ptvec) ;
	//namedWindow( "Image View", WINDOW_AUTOSIZE );
	imshow("Image View", img);

	
    GenerateExtrinsecMatrix("intrinsec.yml",ptvec,objectPoints,tvec,rotationMatrix, cameraMatrix) ;
    
    
	
	
	cout << objectPoints << ptvec ;
/*

  Mat tvec(3,1,DataType<double>::type);
  Mat cameraMatrix(3,3,cv::DataType<double>::type) ;
  Mat rotationMatrix(3,3,cv::DataType<double>::type);
  GenerateExtrinsecMatrix("intrinsec.yml",imagePoints,objectPoints,tvec,rotationMatrix, cameraMatrix) ;
  

    
   for(unsigned int i = 0; i < imagePoints.size(); ++i)
     {

	Point3f projectedObjectPoints =	generate3DFrom2DPoints(imagePoints[i], rotationMatrix,cameraMatrix,tvec,0) ;
std::cout <<"Object point =" << objectPoints[i]<< " projected to " << projectedObjectPoints <<  std::endl ;
   }
 */
  waitKey(0); 
  return 0;
}



 

