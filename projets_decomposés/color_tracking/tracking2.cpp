//objectTrackingTutorial.cpp

//Written by  Kyle Hounslow 2013

//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software")
//, to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
//and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//IN THE SOFTWARE.

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


////////////////////
/*
  std::vector<Point2f> imagePoints ;
  std::vector<Point3f> objectPoints ;
  Mat tvec(3,1,DataType<double>::type);
  Mat cameraMatrix(3,3,cv::DataType<double>::type) ;
  Mat rotationMatrix(3,3,cv::DataType<double>::type);
  Mat rvec(3,1,DataType<double>::type);
*/

/////////////////


int main(int argc, char* argv[])
{

    int h1 = 0; int s1 = 0; int v1 = 0;
    int h2 = 255; int s2 = 255; int v2 = 250;
    //x and y values for the location of the object image
	int u = 0, v = 0;
	//Real point value
	int X,Y = 0;
	// If object if found
	bool objectFound ;
	bool trackObjects = true;
    bool useErodeAndDilate = true;
	
	//matrix storage for HSV image, input and filtered
	Mat HSV, cameraFeed, threshold ;

	const string windowName = "Original Image";
	const string windowName2 = "Thresholded Image";
	const string windowName3 = "After Morphological Operations";

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
	




	//video capture object to acquire webcam feed
	VideoCapture capture(0);
	
	if (!capture.isOpened())  // if not success, exit program
    {		
		cout << "Cannot open the video cam" << endl;
        return -1;
    }

	//set height and width of capture frame
	capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);
	//start an infinite loop where webcam feed is copied to cameraFeed matrix
	//all of our operations will be performed within this loop
	
	//capture.read(cameraFeed);
	//chessboard calibration
	  std::vector<Point3f> objectPoints ;
	  std::vector<Point2f> ptvec ;
	  Mat tvec(3,1,DataType<double>::type);
	  Mat rvec(3,1,DataType<double>::type);
	  Mat cameraMatrix(3,3,cv::DataType<double>::type) ;
	  Mat rotationMatrix(3,3,cv::DataType<double>::type);
	  Size chessSize = cvSize(9,6) ;
	  generate3DPointsFromCheesboard(chessSize,3.0,objectPoints);
	
	
	
	//Mat img = imread("chessboard.png");//, CV_LOAD_IMAGE_GRAYSCALE);
	
	
	//namedWindow( "Image View", WINDOW_AUTOSIZE );
	

	
   
    
    
		//create slider bars for HSV filtering
	createHSVTrackbars("Trackbars",&h1,&h2,&s1,&s2,&v1,&v2);
	
	while(1){
		capture.read(cameraFeed);
		bool found = generate2DPointsFromCheesboard(cameraFeed,chessSize,ptvec) ;
		imshow("Image View", cameraFeed);
		if (found)
		{
			cout << "chessboard found" << endl;
			break; 
		}
		
		// 0.5 s delay
		waitKey(100) ;
		//wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		if (waitKey(30) == 27) 
		{
			cout << "esc key is pressed by user" << endl;
			break; 
		}
	}
	//Generate extrinsec parameter
	 GenerateExtrinsecMatrix("intrinsec.yml",ptvec,objectPoints,tvec,rvec,rotationMatrix, cameraMatrix) ;
	
	
	
	while(1){
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
						
			trackFilteredObject(u,v,threshold,cameraFeed,objectFound);
				if (objectFound)
				{
					Point2f uv(u,v) ;	
					Point3f projectedObjectPoints = generate3DFrom2DPoints(uv, rotationMatrix,cameraMatrix,tvec,0) ;
					X = projectedObjectPoints.x ;
					Y =  projectedObjectPoints.y ;
					
					putText(cameraFeed,"Tracking Object",Point(0,50),2,1,Scalar(0,255,0),2);
					//draw object location on screen
					drawTarget(u,v,X,Y,cameraFeed);
					
				}
			
		}
		//show frames 
		imshow(windowName2,threshold);
		imshow(windowName,cameraFeed);


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
