#include "opencv2/opencv.hpp"
#include <iostream>


using namespace cv;
using namespace std;
/*
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
 */
#include <iostream>
#include <string>
 
std::vector<Point2f> Generate2DPoints();
std::vector<Point3f> Generate3DPoints();
 
int main( int argc, char* argv[])
{
  // Read points
  std::vector<Point2f> imagePoints = Generate2DPoints();
  std::vector<Point3f> objectPoints = Generate3DPoints();
 
  std::cout << "There are " << imagePoints.size() << " imagePoints and " << objectPoints.size() << " objectPoints." << std::endl;
  
  
  //On utilise la matrice intrinseque
	FileStorage fs("intrinsec.yml", FileStorage::READ);
	Mat cameraMatrix, distCoeffs;
	fs["camera_matrix"] >> cameraMatrix;
	fs["distortion_coefficients"] >> distCoeffs;
  
  /*setIdentity(cameraMatrix);
 
  std::cout << "Initial cameraMatrix: " << cameraMatrix << std::endl;
 
  Mat distCoeffs(4,1,DataType<double>::type);
  distCoeffs.at<double>(0) = 0;
  distCoeffs.at<double>(1) = 0;
  distCoeffs.at<double>(2) = 0;
  distCoeffs.at<double>(3) = 0;*/
  
  
 std::cout << "Initial cameraMatrix: " << cameraMatrix << std::endl;
  Mat rvec(3,1,DataType<double>::type);
  Mat tvec(3,1,DataType<double>::type);
 
  solvePnP(objectPoints, imagePoints, cameraMatrix, distCoeffs, rvec, tvec);
 
  std::cout << "rvec: " << rvec << std::endl;
  std::cout << "tvec: " << tvec << std::endl;
 
  //Verification de la projection
  std::vector<Point2f> projectedPoints;
  projectPoints(objectPoints, rvec, tvec, cameraMatrix, distCoeffs, projectedPoints);
 
  for(unsigned int i = 0; i < projectedPoints.size(); ++i)
    {
    std::cout << "Image point: " << imagePoints[i] << " Projected to " << projectedPoints[i] << std::endl;
    }
 
  return 0;
}
 
 
std::vector<Point2f> Generate2DPoints()
{
  std::vector<Point2f> points;
 
  float x,y;
 
  x=229;y=267 ;
  points.push_back(Point2f(x,y));
 
  x=601;y=89 ;
  points.push_back(Point2f(x,y));
 
  x=1065;y=301 ;
  points.push_back(Point2f(x,y));
 
  x=915;y=635 ;
  points.push_back(Point2f(x,y));
 
 
  for(unsigned int i = 0; i < points.size(); ++i)
    {
    std::cout << points[i] << std::endl;
    }
 
  return points;
}
 
 
std::vector<Point3f> Generate3DPoints()
{
  std::vector<Point3f> points;
 
 
  float x,y,z;
  z=0;
 
  x=0;y=0;
  points.push_back(Point3f(x,y,z));
 
  x=1;y=0;
  points.push_back(Point3f(x,y,z));
 
  x=1;y=1;
  points.push_back(Point3f(x,y,z));
 
  x=0;y=1;
  points.push_back(Point3f(x,y,z));
 
 
  for(unsigned int i = 0; i < points.size(); ++i)
    {
    std::cout << points[i] << std::endl;
    }
 
  return points;
}
