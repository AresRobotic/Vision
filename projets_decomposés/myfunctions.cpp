#include "myfunctions.hpp"


Point3f generate3DFrom2DPoints(Point2f imagePoint,Mat rotationMatrix, Mat cameraMatrix, Mat tvec,int hauteurDuPlan)
{

	Mat tempMat, tempMat2;
	double s;
	Mat uvPoint = cv::Mat::ones(3,1,DataType<double>::type); //u,v,1
	Mat projectedObjectPoints = cv::Mat::ones(3,1,cv::DataType<double>::type); //u,v,1
	uvPoint.at<double>(0,0) = imagePoint.x; //got this point using mouse callback
	uvPoint.at<double>(1,0) = imagePoint.y;
	//cout << "Rotation :"<<rotationMatrix <<"camera"<<cameraMatrix ; //DEBUG
	tempMat = rotationMatrix.inv() * cameraMatrix.inv() * uvPoint;
	tempMat2 = rotationMatrix.inv() * tvec;
	s = hauteurDuPlan + tempMat2.at<double>(2,0); //285 represents the height Zconst
	s /= tempMat.at<double>(2,0);
	projectedObjectPoints = rotationMatrix.inv() * (s * cameraMatrix.inv() * uvPoint - tvec);
		
	Point3f out(projectedObjectPoints.at<double>(0,0),projectedObjectPoints.at<double>(1,0),hauteurDuPlan) ;
	return out ;
		
	}


std::vector<Point2f> Generate2DPoints()
{
  std::vector<Point2f> points;
 
  float x,y;
  
 
  x=99;y=202 ;
  points.push_back(Point2f(x,y));
 
  x=541;y=245 ;
  points.push_back(Point2f(x,y));
 
  x=636;y=336 ;
  points.push_back(Point2f(x,y));
 
  x=2;y=279 ;
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
 
  x=100;y=0;
  points.push_back(Point3f(x,y,z));
 
  x=100;y=58;
  points.push_back(Point3f(x,y,z));
 
  x=0;y=58;
  points.push_back(Point3f(x,y,z));
 
 
  for(unsigned int i = 0; i < points.size(); ++i)
    {
    std::cout << points[i] << std::endl;
    }
 
  return points;
}

void GenerateExtrinsecMatrix(String intrinsecFileName,std::vector<Point2f> imagePoints, std::vector<Point3f> objectPoints,Mat tvec, Mat rotationMatrix, Mat cameraMatrix)
{
	
  //std::vector<Point2f> imagePoints = Generate2DPoints();
  //std::vector<Point3f> objectPoints = Generate3DPoints();
  Mat rvec(3,1,DataType<double>::type);
  //Mat rotationMatrix(3,3, DataType<double>::type) ;
 
  std::cout << "There are " << imagePoints.size() << " imagePoints and " << objectPoints.size() << " objectPoints." << std::endl;
  
  
  //On utilise la matrice intrinseque calculée précédement
	FileStorage fs(intrinsecFileName, FileStorage::READ);
	Mat  distCoeffs;
	fs["camera_matrix"] >> cameraMatrix;
	fs["distortion_coefficients"] >> distCoeffs;
	//cout << "Cam" << cameraMatrix ;

  
  //std::cout << "Initial cameraMatrix: " << cameraMatrix << std::endl;
  //Mat rvec(3,1,DataType<double>::type);

  solvePnP(objectPoints, imagePoints, cameraMatrix, distCoeffs, rvec, tvec);
  cv::Rodrigues(rvec,rotationMatrix);
  
   //Verification de la projection
  std::vector<Point2f> projectedImagePoints;
  cv::projectPoints(objectPoints, rvec, tvec, cameraMatrix, distCoeffs, projectedImagePoints);
  //TODO :
  //Calculate the retroprojection error
 
  for(unsigned int i = 0; i < projectedImagePoints.size(); ++i)
    {
		std::cout << "Image point: " << imagePoints[i] << " projected to " << projectedImagePoints[i] << std::endl;
    }
	
	
	}
