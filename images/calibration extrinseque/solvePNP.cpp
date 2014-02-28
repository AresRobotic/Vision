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
  
  
  //On utilise la matrice intrinseque calculée précédement
	FileStorage fs("intrinsec.yml", FileStorage::READ);
	Mat cameraMatrix, distCoeffs;
	fs["camera_matrix"] >> cameraMatrix;
	fs["distortion_coefficients"] >> distCoeffs;
  

  
 std::cout << "Initial cameraMatrix: " << cameraMatrix << std::endl;
  Mat rvec(3,1,DataType<double>::type);
  Mat tvec(3,1,DataType<double>::type);
  cv::Mat rotationMatrix(3,3,cv::DataType<double>::type);
 
  solvePnP(objectPoints, imagePoints, cameraMatrix, distCoeffs, rvec, tvec);
  cv::Rodrigues(rvec,rotationMatrix);
 
  std::cout << "rvec: " << rvec << std::endl;
  std::cout << "tvec: " << tvec << std::endl;
  std::cout << "rotationMatrix: " << rotationMatrix << std::endl;
 
  //Verification de la projection
  std::vector<Point2f> projectedPoints;
  cv::projectPoints(objectPoints, rvec, tvec, cameraMatrix, distCoeffs, projectedPoints);
 
  for(unsigned int i = 0; i < projectedPoints.size(); ++i)
    {
    std::cout << "Image point: " << imagePoints[i] << " Projected to " << projectedPoints[i] << std::endl;
    }
    
   for(unsigned int i = 0; i < imagePoints.size(); ++i)
     {

   cv::Mat uvPoint = cv::Mat::ones(3,1,cv::DataType<double>::type); //u,v,1
	uvPoint.at<double>(0,0) = imagePoints[i].x; //got this point using mouse callback
	uvPoint.at<double>(1,0) = imagePoints[i].y;
	
	cv::Mat tempMat, tempMat2;
double s;
tempMat = rotationMatrix.inv() * cameraMatrix.inv() * uvPoint;
tempMat2 = rotationMatrix.inv() * tvec;
s = 0 + tempMat2.at<double>(2,0); //285 represents the height Zconst
s /= tempMat.at<double>(2,0);
std::cout << "P = " << rotationMatrix.inv() * (s * cameraMatrix.inv() * uvPoint - tvec) <<"P real =" << objectPoints[i]<< std::endl ;

   }

    
  //  rvec: [-0.6516997555859243; -0.6977915209735557; -0.9088726124391173]
//tvec: [-0.5458128309114365; -0.2893036301628076; 1.910207684906602]
/*finalm = [0.4332427076264066, 0.8638966371873064, -0.2568722572505709, -0.5458128309114365;
  -0.4712986590380123, 0.4600923336570399, 0.7524577187450405, -0.2893036301628076;
  0.7682306491396391, -0.2049332690572291, 0.6064849750455537, 1.910207684906602]
*/
 
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
 
  x=150;y=0;
  points.push_back(Point3f(x,y,z));
 
  x=150;y=150;
  points.push_back(Point3f(x,y,z));
 
  x=0;y=150;
  points.push_back(Point3f(x,y,z));
 
 
  for(unsigned int i = 0; i < points.size(); ++i)
    {
    std::cout << points[i] << std::endl;
    }
 
  return points;
}
