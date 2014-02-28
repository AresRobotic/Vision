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
  // Read points
  std::vector<Point2f> imagePoints = Generate2DPoints();
  std::vector<Point3f> objectPoints = Generate3DPoints();
 


  Mat tvec(3,1,DataType<double>::type);
  Mat cameraMatrix(3,3,cv::DataType<double>::type) ;
  Mat rotationMatrix(3,3,cv::DataType<double>::type);
  GenerateExtrinsecMatrix("intrinsec.yml",imagePoints,objectPoints,tvec,rotationMatrix, cameraMatrix) ;
  
  /*cout << "Cam_main" <<  cameraMatrix ;
  cout << "Rot_main" <<  cameraMatrix ;*/
    
   for(unsigned int i = 0; i < imagePoints.size(); ++i)
     {

	Point3f projectedObjectPoints =	generate3DFrom2DPoints(imagePoints[i], rotationMatrix,cameraMatrix,tvec,0) ;
std::cout <<"Object point =" << objectPoints[i]<< " projected to " << projectedObjectPoints <<  std::endl ;
   }
 
  return 0;
}
 

