
#include "opencv2/opencv.hpp"
#include "../myfunctions.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>


#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>
 

using namespace std;
using namespace cv;


int main(int argc, char* argv[])
{
	cout << "OpenCV version : " << CV_VERSION << endl;

    int h1 = 0; int s1 = 16; int v1 = 160;
    int h2 = 17 ; int s2 = 78; int v2 = 204;
    //x and y values for the location of the object image
	int u = 0, v = 0;
	//Real point value
	//int X,Y = 0;
	// If object if found
	bool objectFound ;
	bool trackObjects = true;
    bool useErodeAndDilate = true;
	
	//matrix storage for HSV image, input and filtered
	Mat HSV, cameraFeed, threshold ;

	/*imagePoints = Generate2DPoints();
    objectPoints = Generate3DPoints();
	
    GenerateExtrinsecMatrix("intrinsec.yml",imagePoints,objectPoints,tvec,rvec,rotationMatrix, cameraMatrix) ; 
    */

	
	
	//some boolean variables for different functionality within this
	//program
	/*if( argc < 2 ){
		printf( "\n Error! No video data!!! \n" );
		return -1;
	}*/




	//video capture object to acquire webcam feed camera 1
	VideoCapture capture;
	capture.open(1);
	
	if (!capture.isOpened())  // if not success, try default cam
    {		
		cout << "Camera 1 is not here ; try camera 0" << endl;
		capture.open(0);
		if (!capture.isOpened())
		{
			cout << "Cannot open the video cam" << endl;
			return -1;
		}
    }

	//set height and width of capture frame
	capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);


	//initialisation for chessboard calibration
	  std::vector<Point3f> objectPoints ;
	  std::vector<Point2f> ptvec ;
	  Mat tvec(3,1,DataType<double>::type);
	  Mat rvec(3,1,DataType<double>::type);
	  Mat cameraMatrix(3,3,cv::DataType<double>::type) ;
	  Mat rotationMatrix(3,3,cv::DataType<double>::type);
	  //Chessboard size
	  Size chessSize = cvSize(9,6) ;
	  
	  //Generate 3D points that correspond to the chessboard
	  generate3DPointsFromCheesboard(chessSize,3.0,objectPoints);
	
	//Loop while not finding a chessboard of size "chessSize" and not pressed "echap"
	/*while(waitKey(30) != 27){
		capture.read(cameraFeed);
		bool found = generate2DPointsFromCheesboard(cameraFeed,chessSize,ptvec) ;
		imshow("Image View", cameraFeed);
		if (found)
		{
			cout << "chessboard found" << endl;
			break; 
		}
		
		// 0.5 s delay
		//waitKey(100) ;
		//wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop

	}
	
	//Generate extrinsec parameter
	 GenerateExtrinsecMatrix("intrinsec.yml",ptvec,objectPoints,tvec,rvec,rotationMatrix, cameraMatrix) ;
	*/
	
	// Loop for the tracking
	while(1){
		
		    
		//create slider bars for HSV filtering
		createHSVTrackbars("Trackbars",&h1,&h2,&s1,&s2,&v1,&v2);
		//store image to matrix
		capture.read(cameraFeed);
		//convert frame from BGR to HSV colorspace
		cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
		
		//filter HSV image between values and store filtered image to threshold matrix
		inRange(HSV,Scalar(h1,s1,v1),Scalar(h2,s2,v2),threshold);
		
		//perform morphological operations on thresholded image to eliminate noise and emphasize the filtered object(s)
		if(useErodeAndDilate)
			erodeAndDilate(threshold);
		//pass in thresholded frame to our object tracking function
		//this function will return the x and y coordinates of the
		//filtered object
		if(trackObjects)
		{
			objectFound = trackFilteredObject(u,v,threshold,cameraFeed);
				if (objectFound)
				{
					Point2f uv(u,v) ;	
					Point3f projectedObjectPoints = generate3DFrom2DPoints(Point2f(u,v), rotationMatrix,cameraMatrix,tvec,0) ;
					
					putText(cameraFeed,"Tracking Object",Point(0,50),2,1,Scalar(0,125,125),1);
					//draw object location on screen
					drawTarget(u,v,projectedObjectPoints.x,projectedObjectPoints.y,cameraFeed);
					
				}
			
		}
		//show frames 
		imshow("Thresholded Image",threshold);
		imshow("Original Image",cameraFeed);
		//"After Morphological Operations"

		//delay 30ms so that screen can refresh.
		//image will not appear without this waitKey() command
		waitKey(10);
		int c = cvWaitKey(15);    
		//If 'ESC' is pressed, break the loop
		if((char)c==27 ){
			break;
		}
	}

	return 0;
}
