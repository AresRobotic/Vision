
#include <iostream>
#include <time.h>
#include "opencv2/opencv.hpp"


# define FRAME_WIDTH 640
# define FRAME_HEIGHT 480
# define MAX_NUM_OBJECTS 50
//minimum and maximum object area
# define MIN_OBJECT_AREA 20*20
# define MAX_OBJECT_AREA FRAME_HEIGHT*FRAME_WIDTH/1.5

# define TRACKBAR_WINDOW_NAME "Trackbars"


using namespace cv;
using namespace std;


Point3f generate3DFrom2DPoints(Point2f imagePoint,Mat rotationMatrix, Mat cameraMatrix, Mat tvec,int hauteurDuPlan) ;
vector<Point3f> Generate3DPoints() ;
vector<Point2f> Generate2DPoints() ;
void GenerateExtrinsecMatrix(String intrinsecFileName,std::vector<Point2f> imagePoints, std::vector<Point3f> objectPoints,Mat tvec,Mat rotationMatrix, Mat cameraMatrix) ;

string intToString(int number) ;

void drawTarget(int u, int v,int X,int Y,Mat &frame) ;

void erodeAndDilate(Mat &thresh) ;

void trackFilteredObject(int &u, int &v, Mat threshold, Mat &cameraFeed, bool &objectFound) ;

void createHSVTrackbars(String trackbarWindowName, int *h1, int *h2, int *s1,int *s2,int *v1,int *v2) ;

void on_trackbar( int, void* );

bool generate2DPointsFromCheesboard(Mat undistord_image, Size chess_height_width, std::vector<Point2f> &ptvec) ;

void generate3DPointsFromCheesboard(Size chess_height_width, float squareSize, vector<Point3f>& corners ) ;
