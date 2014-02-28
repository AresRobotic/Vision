
#include <iostream>
#include <time.h>
#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;

Point3f generate3DFrom2DPoints(Point2f imagePoint,Mat rotationMatrix, Mat cameraMatrix, Mat tvec,int hauteurDuPlan) ;
vector<Point3f> Generate3DPoints() ;
vector<Point2f> Generate2DPoints() ;
void GenerateExtrinsecMatrix(String intrinsecFileName,std::vector<Point2f> imagePoints, std::vector<Point3f> objectPoints,Mat tvec,Mat rotationMatrix, Mat cameraMatrix) ;
