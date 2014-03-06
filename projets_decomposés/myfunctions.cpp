#include "myfunctions.hpp"


Point3f generate3DFrom2DPoints(Point2f imagePoint,Mat rotationMatrix, Mat cameraMatrix, Mat tvec,int hauteurDuPlan)
{
	//permet de faire les transformations pour passer de la 2D à la 3D en contraignant Z, la hauteur dans le repère objet
	//A optimiser sur les inversions de matrices
	//cf http://stackoverflow.com/questions/12299870/computing-x-y-coordinate-3d-from-image-point
	
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

void GenerateExtrinsecMatrix(String intrinsecFileName,std::vector<Point2f> imagePoints, std::vector<Point3f> objectPoints,Mat tvec, Mat rvec, Mat rotationMatrix, Mat cameraMatrix)
{
	
  //std::vector<Point2f> imagePoints = Generate2DPoints();
  //std::vector<Point3f> objectPoints = Generate3DPoints();
  //Mat rvec(3,1,DataType<double>::type);
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

string intToString(int number)
{
	std::stringstream ss;
	ss << number;
	return ss.str();
}

void drawTarget(int u, int v,int X,int Y,Mat &frame){

	//use some of the openCV drawing functions to draw crosshairs
	//on your tracked image!
	circle(frame,Point(u,v),20,Scalar(0,255,0),2);
	line(frame,Point(u,v-5),Point(u,v-25),Scalar(0,255,0),2);
	line(frame,Point(u,v+5),Point(u,v+25),Scalar(0,255,0),2);
	line(frame,Point(u-5,v),Point(u-25,v),Scalar(0,255,0),2);
	line(frame,Point(u+5,v),Point(u+25,v),Scalar(0,255,0),2);

	putText(frame,"x="+intToString(X)+" ; y="+intToString(Y),Point(u,v+30),1,1,Scalar(0,255,0),2);

}

void erodeAndDilate(Mat &thresh){

	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 3px by 3px rectangle

	Mat erodeElement = getStructuringElement( MORPH_ELLIPSE,Size(5,5)); //MORPH_RECT MORPH_ELLIPSE
    //dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement( MORPH_ELLIPSE,Size(5,5));

	erode(thresh,thresh,erodeElement);
	erode(thresh,thresh,erodeElement);

	dilate(thresh,thresh,dilateElement);
	dilate(thresh,thresh,dilateElement);
}

bool trackFilteredObject(int &u, int &v, Mat threshold, Mat &cameraFeed){
	Mat temp;
	threshold.copyTo(temp);
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//find contours of filtered image using openCV findContours function
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) 
	{
		int numObjects = hierarchy.size();
        //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
        if(numObjects<MAX_NUM_OBJECTS){
			for (int index = 0; index >= 0; index = hierarchy[index][0]) 
			{
				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;

				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				//we only want the object with the largest area so we safe a reference area each
				//iteration and compare it to the area in the next iteration.
                if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea)
                {
					u = moment.m10/area;
					v = moment.m01/area;
					objectFound = true;
				} else objectFound = false;


			}

		}//else putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
	}
	return objectFound ;
}

void createHSVTrackbars(String trackbarWindowName, int *h1, int *h2, int *s1,int *s2,int *v1,int *v2){
	//create window for trackbars
    namedWindow(trackbarWindowName,0);

	//create trackbars and insert them into window  
    createTrackbar( "H MIN", trackbarWindowName, h1, 255, on_trackbar );
    createTrackbar( "H MAX", trackbarWindowName, h2, 255, on_trackbar );
    createTrackbar( "S MIN", trackbarWindowName, s1, 255, on_trackbar );
    createTrackbar( "S MAX", trackbarWindowName, s2, 255, on_trackbar );
    createTrackbar( "V MIN", trackbarWindowName, v1, 255, on_trackbar );
    createTrackbar( "V MAX", trackbarWindowName, v2, 255, on_trackbar );

}

//Function called by the trackbar
void on_trackbar( int, void* )
{
	//cout << "coucou" ;//This function gets called whenever a
	// trackbar position is changed
    //frames_stream << 10 << " " << 15 << " " << "\n";
}

bool generate2DPointsFromCheesboard(Mat undistord_image, Size chess_height_width, std::vector<Point2f> &ptvec)
{

	//cvtColor(undistord_image,undistord_image,CV_RGB2GRAY);
	bool found = findChessboardCorners( undistord_image, chess_height_width, ptvec, CV_CALIB_CB_ADAPTIVE_THRESH );
	//cout << ptvec ;
	if (found)
		drawChessboardCorners(undistord_image,chess_height_width, ptvec, found) ;
	else cout<< "ERROR ; chessboard not found" << endl;
	//imshow("Cheesboard found", undistord_image);
	//waitKey(0); 
	return found ;	
}


void generate3DPointsFromCheesboard(Size chess_height_width, float squareSize, vector<Point3f>& corners ) 
{
	corners.clear();
	for( int i = 0; i < chess_height_width.height; ++i )
	{
		for( int j = 0; j < chess_height_width.width; ++j )
        corners.push_back(Point3f(float( j*squareSize ), float( i*squareSize ), 0));       
	}
}

void drawSimpleText(Mat img,String text)
{
//extract from http://docs.opencv.org/modules/core/doc/drawing_functions.html#gettextsize	
int fontFace = FONT_HERSHEY_SCRIPT_SIMPLEX;
double fontScale = 2;
int thickness = 3;
int baseline = 0;
Size textSize = getTextSize(text, fontFace,
                            fontScale, thickness, &baseline);
baseline += thickness;

// center the text
Point textOrg((img.cols - textSize.width)/2,
              (img.rows + textSize.height)/2);

// draw the box
/*rectangle(img, textOrg + Point(0, baseline),
          textOrg + Point(textSize.width, -textSize.height),
          Scalar(0,0,255));*/
// ... and the baseline first
/*line(img, textOrg + Point(0, thickness),
     textOrg + Point(textSize.width, thickness),
     Scalar(0, 0, 255));*/

// then put the text itself
putText(img, text, textOrg, fontFace, 2,
        Scalar::all(255), 1, CV_AA);
	
}
