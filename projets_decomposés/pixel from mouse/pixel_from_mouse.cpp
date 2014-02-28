#include "opencv2/opencv.hpp"
#include <iostream>


using namespace cv;
using namespace std;

 void onMouse(int evt, int x, int y, int flags, void* param) {
    if(evt == CV_EVENT_LBUTTONDOWN) {
        /*Point* ptPtr = (Point*)param;
        ptPtr->x = x;
        ptPtr->y = y;
        cout << x <<y ;*/
        cout<<""<<x<<" "<<y<<endl;

    }
}

int main() {
	 Mat frame, undistord;
	String window_name = "1 " ;
	Mat cameraMatrix, distCoeffs;
	
    Point2i pt(-1,-1);
    namedWindow( window_name, CV_WINDOW_AUTOSIZE );
    frame = imread("1.png");
    
    
   //On utilise la matrice intrinseque calculée précédement
	FileStorage fs("intrinsec.yml", FileStorage::READ);
	fs["camera_matrix"] >> cameraMatrix;
	fs["distortion_coefficients"] >> distCoeffs;
	//On redresse l'image produite en fonction des défaults du capteur
	undistort(frame, undistord, cameraMatrix, distCoeffs);
	
    
    
    
    imshow(window_name, undistord);

    setMouseCallback(window_name, onMouse, (void*)&pt);
    
  waitKey(0);
  return 0;
    
    // Note that we passed '&pt' (a pointer
    // to `pt`) to the mouse callback function.
    // Therefore `pt` will update its [x,y] coordinates
    // whenever user left-clicks on the image in "Output Window".
}
 
/*
static void onMouse( int event, int x, int y, int f, void* ){

 Vec3b pix=image.at<Vec3b>(y,x);  // row,col !!
 int B=pix.val[0];
 int G=pix.val[1];
 int R=pix.val[2];
 cout<<R<<endl<<G<<endl<<B<<endl;

}


int main( int argc, char** argv )
{
  namedWindow( window_name, CV_WINDOW_AUTOSIZE );
  image = imread( "1.png");
  imshow( window_name, image );    
  setMouseCallback( window_name, onMouse, 0 );
  waitKey(0);
  return 0;
}*/
