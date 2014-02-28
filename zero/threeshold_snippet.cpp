#include <opencv2/opencv.hpp>
#include "opencv/highgui.h"
#include "opencv/cv.h"
 
#include <iostream>
#include <stdlib.h>
#include <stdio.h>


using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	VideoCapture capture;
	//open capture object at location zero (default location for webcam)
	capture.open(0);
	
    Mat src;
    capture.read(src); 
	

    int rh = 255, rl = 100, gh = 255, gl = 0, bh = 70, bl = 0;

    string windowName = "background";
    namedWindow(windowName);

    createTrackbar("rh", windowName, &rh, 255);
    createTrackbar("rl", windowName, &rl, 255);
    createTrackbar("gh", windowName, &gh, 255);
    createTrackbar("gl", windowName, &gl, 255);
    createTrackbar("bh", windowName, &bh, 255);
    createTrackbar("bl", windowName, &bl, 255);

    // for dilation
    Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));

    Mat bgIsolation;
    int key = 0;
    do
    {
        inRange(src, Scalar(bl, gl, rl), Scalar(bh, gh, rh), bgIsolation);

        bitwise_not(bgIsolation, bgIsolation);

        erode(bgIsolation, bgIsolation, Mat());
        dilate(bgIsolation, bgIsolation, element);

        imshow(windowName, bgIsolation);
        key = waitKey(33);
    } while((char)key != 27);

    waitKey();

    return 0;
}
