//----------------------------------
//  author = Danu andrean
//  create = july 2021
//  title  = object tracking | KRSRI

//-----------------------------------
//
//-----------------------------------

#include <sstream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <sys/time.h>

using namespace std;

char serialPortFilename[] = "/dev/ttyACM0";
string arduino;

using namespace cv;
using namespace std;
//initial min and max HSV filter values.
//these will be changed using trackbars
int H_MIN = 0;
int H_MAX = 65;
int S_MIN = 75;
int S_MAX = 256;
int V_MIN = 197;
int V_MAX = 256;

int H_MIN2 = 0;
int H_MAX2 = 256;
int S_MIN2 = 0;
int S_MAX2 = 168;
int V_MIN2 = 0;
int V_MAX2 = 42;

RNG rng(12345);
//default capture width and height
const int FRAME_WIDTH = 960;
const int FRAME_HEIGHT = 650;
const int center_y = FRAME_HEIGHT/2;
const int center_x = FRAME_WIDTH/2.5;
float value_x = 0;
float value_y = 0;


//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS = 50;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 10 * 10;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH / 1.5;
//names that will appear at the top of each window
const string windowName = "Original Image";
const string windowName1 = "HSV Image";
const string windowName2 = "Thresholded Image";
const string windowName3 = "After Morphological Operations";
const string trackbarWindowName = "Trackbars";


string intToString(int number) {


	std::stringstream ss;
	ss << number;
	return ss.str();
}
void createTrackbars() {
	//create window for trackbars


	namedWindow(trackbarWindowName, 3);
	
	//create memory to store trackbar name on window
	char TrackbarName[50];
                             // ---->    ---->     ---->      
	createTrackbar("H_MIN", trackbarWindowName, &H_MIN, 256);
	createTrackbar("H_MAX", trackbarWindowName, &H_MAX, 256);
	createTrackbar("S_MIN", trackbarWindowName, &S_MIN, 256);
	createTrackbar("S_MAX", trackbarWindowName, &S_MAX, 256);
	createTrackbar("V_MIN", trackbarWindowName, &V_MIN, 256);
	createTrackbar("V_MAX", trackbarWindowName, &V_MAX, 256);

	/*createTrackbar("H_MIN2", trackbarWindowName, &H_MIN2, 256);
	createTrackbar("H_MAX2", trackbarWindowName, &H_MAX2, 256);
	createTrackbar("S_MIN2", trackbarWindowName, &S_MIN2, 256);
	createTrackbar("S_MAX2", trackbarWindowName, &S_MAX2, 256);
	createTrackbar("V_MIN2", trackbarWindowName, &V_MIN2, 256);
	createTrackbar("V_MAX2", trackbarWindowName, &V_MAX2, 256);*/


}

void morphOps(Mat &thresh, Mat &thresh2) {

	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 3px by 3px recstangle

	Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));
	//dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8, 8));

	erode(thresh, thresh, erodeElement);
	//erode(thresh, thresh, erodeElement);

	dilate(thresh, thresh, dilateElement);
	//dilate(thresh, thresh, dilateElement);

	/*erode(thresh2, thresh2, erodeElement);
	//erode(thresh2, thresh2, erodeElement);

	dilate(thresh2, thresh2, dilateElement);
	//dilate(thresh2, thresh2, dilateElement);*/

}
void trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed) {

	Mat temp;
	Mat temp2;
	threshold.copyTo(temp);
	//threshold2.copyTo(temp2);
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	
	

	//find contours of filtered image using openCV findContours function
	findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	//findContours(temp2, contours2, hierarchy2, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	//findContours(temp2, contours2, hierarchy2, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	//use moments method to find our filtered object
	double refArea = 0;
	double refArea2 = 0;
	bool objectFound1 = false;
	bool objectFound2 = false;
	arduino="0,0,0";


	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();
		//int numObjects2 = hierarchy.size();

		//if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
		if (numObjects < MAX_NUM_OBJECTS) {
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {

				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;
				//===================================================identifikasi object================================================================//
				if (area > MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea) {
					x = moment.m10 / area;
					y = moment.m01 / area;
					objectFound1 = true;
					refArea = area;
				}
				else {
					objectFound1 = false;
					//putText(cameraFeed, "filter lagi dong", Point(0, 50), 2, 1, Scalar(0, 0, 255), 2);
				}
			}

			//jika object di temukan menggambar garis tengah
			if (objectFound1 == true) {
				
				arduino="1,"+intToString(value_x)+","+intToString(value_y);
				//center calculation
				if(y > center_y) value_y = ((y-center_y)*-1)/2;
				else value_y = (center_y-y) /2;
				
				if(x > center_x) value_x = (x-center_x)/2;
				else value_x = ((center_x-x)*-1) /2;
				//cout<<center_y<< " -- " <<center_x<<endl;
				
					
				
				putText(cameraFeed, "Tracking Object", Point(x + 60, y - 120), 2, 1, Scalar(0, 255, 0), 2);
				putText(cameraFeed, "o", Point(center_x, center_y), 2, 1, Scalar(0,0, 255), 2);

				//menggambar garis tengah

				/*if (y - 25 > 0)
					line(cameraFeed, Point(x, y), Point(x, y - 25), Scalar(0, 255, 0), 2);
				else line(cameraFeed, Point(x, y), Point(x, 0), Scalar(0, 255, 0), 2);
				if (y + 25 < FRAME_HEIGHT)
					line(cameraFeed, Point(x, y), Point(x, y + 25), Scalar(0, 255, 0), 2);
				else line(cameraFeed, Point(x, y), Point(x, FRAME_HEIGHT), Scalar(0, 255, 0), 2);
				if (x - 25 > 0)
					line(cameraFeed, Point(x, y), Point(x - 25, y), Scalar(0, 255, 0), 2);
				else line(cameraFeed, Point(x, y), Point(0, y), Scalar(0, 255, 0), 2);
				if (x + 25 < FRAME_WIDTH)
					line(cameraFeed, Point(x, y), Point(x + 25, y), Scalar(0, 255, 0), 2);
				else line(cameraFeed, Point(x, y), Point(FRAME_WIDTH, y), Scalar(0, 255, 0), 2);*/

				putText(cameraFeed, intToString(value_x) + "," + intToString(value_y), Point(x + 90, y + 30), 1, 1, Scalar(0, 255, 0), 2);
			}

		}
		else putText(cameraFeed, "TOO MUCH NOISE! ADJUST FILTER", Point(0, 50), 1, 2, Scalar(0, 0, 250), 2);
	}
	/*if (hierarchy2.size() > 0) {
		int numObjects2 = hierarchy2.size();
		//int numObjects2 = hierarchy.size();

		//if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
		if (numObjects2 < MAX_NUM_OBJECTS) {
			for (int index = 0; index >= 0; index = hierarchy2[index][0]) {

				Moments moment2 = moments((cv::Mat)contours2[index]);
				double area2= moment2.m00;
				//===================================================identifikasi object================================================================//

				if (area2 > MIN_OBJECT_AREA && area2<MAX_OBJECT_AREA && area2>refArea) {
					x = moment2.m10 / area2;
					y = moment2.m01 / area2;
					objectFound2 = true;
					refArea2 = area2;
				}
				else {
					objectFound2 = false;
					//putText(cameraFeed, "filter lagi dong", Point(0, 50), 2, 1, Scalar(0, 0, 255), 2);
				}
			} //akhir for
			if (objectFound2 == true) {
				putText(cameraFeed, "orang", Point(x + 60, y - 120), 2, 1, Scalar(0, 255, 0), 2);
			}
		}
	}*/
	//================================================== menggambar object ===================================================//
	vector<vector<Point> > contours_poly3(contours.size());
	vector<Rect> boundRect3(contours.size());
	vector<Point2f>center3(contours.size());
	vector<float>radius3(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly3[i], 3, true);
		boundRect3[i] = boundingRect(Mat(contours_poly3[i]));
		minEnclosingCircle((Mat)contours_poly3[i], center3[i], radius3[i]);


		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		/////persegii////
		rectangle(cameraFeed, boundRect3[i].tl(), boundRect3[i].br(), (0, 200, 300), 5);
		circle(cameraFeed, center3[i], 80, Scalar(0, 255, 0), 2);
	}




}

int main(int argc, char* argv[])
{
	struct timeval tv;
   long s_prev = tv.tv_sec; 
   long s_prev_close  = tv.tv_sec; 
   FILE *serPort;
	
	//some boolean variables for different functionality within this
	//program
	bool trackObjects = true;
	bool useMorphOps = true;
	
	Mat cameraFeed;	//Matrix to store each frame of the webcam feed
	
	Mat HSV1;			//matrix storage for HSV image
	Mat threshold1;			//matrix storage for binary threshold image
	Mat HSV2;			//matrix storage for HSV image
	Mat threshold2;
	int x = 0, y = 0;		//x and y values for the location of the object
	createTrackbars();		//create slider bars for HSV filtering
	VideoCapture capture;
	
	capture.open(0);		//open capture object at location zero (default location for webcam)
	
	capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);	//set height and width of capture frame
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);

	//start an infinite loop where webcam feed is copied to cameraFeed matrix
	//all of our operations will be performed within this loop
	while (1) {
		//store image to matrix
		capture.read(cameraFeed);
		//convert frame from BGR to HSV colorspace
		cvtColor(cameraFeed, HSV1, COLOR_BGR2HSV);
		cvtColor(cameraFeed, HSV2, COLOR_BGR2HSV);
		//filter HSV image between values and store filtered image to
		//threshold matrix
		inRange(HSV1, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), threshold1);
		inRange(HSV2, Scalar(H_MIN2, S_MIN2, V_MIN2), Scalar(H_MAX2, S_MAX2, V_MAX2), threshold2);
		
		if (useMorphOps) {			//perform morphological operations on thresholded image to eliminate noise
			morphOps(threshold1, threshold2);	//and emphasize the filtered object(s)
			
		}						
		
		//pass in thresholded frame to our object tracking function
		//this function will return the x and y coordinates of the
		//filtered object
		if (trackObjects) {
			trackFilteredObject(x, y, threshold1, cameraFeed);
			
		}

		//show frames 
		imshow(windowName2, threshold1);
		imshow(windowName, cameraFeed);
		imshow(windowName1, HSV2);


		//delay 30ms so that screen can refresh.
		//image will not appear without this waitKey() command
		waitKey(30);

		gettimeofday(&tv,NULL);
      long s = tv.tv_sec; 
      
      if( s %2==0 && s != s_prev ){
         s_prev = s;

         serPort = fopen(serialPortFilename, "w");
         if (serPort == NULL)
         {
            printf("ERROR");	
            return 0;
         }
         // string arduino="2,90,11";

         for(int i = 0; i<arduino.length();i++){
            char send =arduino[i];
            fwrite(&send, sizeof(send),1, serPort);
         }
         cout<<"send"<<endl;
      }
      if(s == s_prev+1 &&  s_prev_close != s){
         s_prev_close = s;

         fclose(serPort);
         cout<<"close"<<endl; 
      }
	}



	return 0;
}
