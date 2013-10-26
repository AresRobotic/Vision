#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat edges;
    namedWindow("edges",1);
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        //Change la couleur
        cvtColor(frame, edges, CV_BGR2GRAY);

        //Filtre Gaussien pour adoucir l'image et supprimer le bruit

        GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);

        //Filtre de Canny pour détecter les contours
        Canny(edges, edges, 0, 30, 3);
        imshow("edges", edges);
        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
