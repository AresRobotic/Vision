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
  std::vector<Point2f> imagePoints ;
  std::vector<Point3f> objectPoints ;
  Mat tvec(3,1,DataType<double>::type);
  Mat cameraMatrix(3,3,cv::DataType<double>::type) ;
  Mat rotationMatrix(3,3,cv::DataType<double>::type);
  Mat rvec(3,1,DataType<double>::type);


/////////////////


int main(int argc, char* argv[])
{
	imagePoints = Generate2DPoints();
    objectPoints = Generate3DPoints();
    int X,Y = 0;
    int h1 = 0; int s1 = 0; int v1 = 0;
    int h2 = 255; int s2 = 255; int v2 = 250;
    //x and y values for the location of the object image
	int u = 0, v = 0;
	//Equivalent with the object Point2f
	
	// If object if found
	bool objectFound ;
	
	//Matrix to store each frame of the webcam feed
	Mat cameraFeed;
	//matrix storage for HSV image
	Mat HSV;
	//matrix storage for binary threshold image
	Mat threshold;
	//names that will appear at the top of each window
	const string windowName = "Original Image";
	const string windowName1 = "HSV Image";
	const string windowName2 = "Thresholded Image";
	const string windowName3 = "After Morphological Operations";

	
  
    GenerateExtrinsecMatrix("intrinsec.yml",imagePoints,objectPoints,tvec,rvec,rotationMatrix, cameraMatrix) ; 


	
	
	//some boolean variables for different functionality within this
	//program
	/*if( argc < 2 ){
		printf( "\n Error! No video data!!! \n" );
		return -1;
	}*/
	
    bool trackObjects = true;
    bool useErodeAndDilate = true;


	//create slider bars for HSV filtering
	createHSVTrackbars("Trackbars",&h1,&h2,&s1,&s2,&v1,&v2);
	//video capture object to acquire webcam feed
	VideoCapture capture(0);
	//capture.open(0);
	
	/*if(!capture.open(argv[1])){
		exit(1);         // Exit if fail
	}*/
	

	//set height and width of capture frame
	capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);
	//start an infinite loop where webcam feed is copied to cameraFeed matrix
	//all of our operations will be performed within this loop
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
		//imshow(windowName1,HSV);
		

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
