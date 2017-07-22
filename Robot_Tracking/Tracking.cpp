#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/features2d.hpp"
//#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <ctime>
#include <unistd.h>
#include <string>
using namespace cv;
using namespace std;

const int robot_types = 2;
bool initialized = false;
VideoCapture camera(0);
int robot_minimum_hue[robot_types];
int robot_maximum_hue[robot_types];


void initialize() {
	initialized = true;
	
	robot_minimum_hue[0] = 38;
	robot_maximum_hue[0] = 75;
	robot_minimum_hue[1] = 160;
	robot_maximum_hue[1] = 179;
	//Hue Values
	//Orange  0-22
	//Yellow 22- 38
	//Green 38-75
	//Blue 75-130
	//Violet 130-160
	//Red 160-179
	
	camera.set(CV_CAP_PROP_EXPOSURE, 0.5);
	camera.set(CV_CAP_PROP_BRIGHTNESS, 0.5);
	camera.set(CV_CAP_PROP_CONTRAST, 0.5);
	if (!camera.isOpened()) {
        cerr << "Couldn't open video capture device" << endl;
    }
    
}

vector<KeyPoint> blob(Mat original_image, Mat color_filtered){
	//Ptr<FeatureDetector> blobsDetector = FeatureDetector::create("SimpleBlob");
	SimpleBlobDetector::Params params;
	
	params.filterByColor = false;

	// Filter by Area.
	params.filterByArea = true;
	params.minArea = 1500;

	// Filter by Circularity
	params.filterByCircularity = true;
	params.minCircularity = 0.1;

	// Filter by Convexity
	params.filterByConvexity = true;
	params.minConvexity = 0.70;

	// Filter by Inertia
	params.filterByInertia = true;
	params.minInertiaRatio = 0.01;

	Ptr<SimpleBlobDetector> blobsDetector = SimpleBlobDetector::create(params);
	vector<KeyPoint> keypoints;
	blobsDetector->detect(color_filtered, keypoints);
	return keypoints;
}


struct detected_robot{
	int robot_type;
	float x_position;
	float y_position;
	
	detected_robot(int r, float x, float y){
		robot_type = r;
		x_position = x;
		y_position = y;
	}
};

vector<detected_robot> get_robot_positions(){
	if(initialized == false){
		initialize();
	}
	vector<detected_robot> detections;
	Mat original_image, img_HSV;
	//Retrieve Image
	camera >> original_image;
    medianBlur(original_image, img_HSV, 3);
    cvtColor(img_HSV, img_HSV, COLOR_BGR2HSV);
    
    for(int i = 0;i<robot_types;i++){
	    
	    int minimum_hue = robot_minimum_hue[i];
		int maximum_hue = robot_maximum_hue[i];
	    Mat color_filtered;
	    //color filter
	    inRange(img_HSV, Scalar(minimum_hue, 50, 50), Scalar(maximum_hue, 255, 255), color_filtered);
	    
	    //morphological opening (removes small objects from the foreground)
		erode(color_filtered, color_filtered, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
		dilate( color_filtered, color_filtered, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
	    //morphological closing (removes small holes from the foreground)
		dilate( color_filtered, color_filtered, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
		erode(color_filtered, color_filtered, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
		//GaussianBlur(color_filtered, color_filtered, cv::Size(9, 9), 2, 2);
		
		vector<KeyPoint> keypoints = blob(original_image, color_filtered);
		for (size_t t = 0; t < keypoints.size(); ++t){
			circle(original_image, keypoints[t].pt, 4, Scalar(255, 0, 255), -1);
			detected_robot robot(i, keypoints[t].pt.x, keypoints[t].pt.y);
			detections.push_back(robot);
		}
		imshow("filtered", color_filtered);
		
	}
	
	imshow("detections", original_image);
	waitKey();
	return detections;
}

int main(int argc, char** argv)
{
    initialize();
    //Camera_Test();
    for(int x = 0;x<10;x++){
		get_robot_positions();
	}
    return 0;
}
//CV_CAP_PROP_POS_MSEC Current position of the video file in milliseconds.
//CV_CAP_PROP_POS_FRAMES 0-based index of the frame to be decoded/captured next.
//CV_CAP_PROP_POS_AVI_RATIO Relative position of the video file: 0 - start of the film, 1 - end of the film.
//CV_CAP_PROP_FRAME_WIDTH Width of the frames in the video stream.
//CV_CAP_PROP_FRAME_HEIGHT Height of the frames in the video stream.
//CV_CAP_PROP_FPS Frame rate.
//CV_CAP_PROP_FOURCC 4-character code of codec.
//CV_CAP_PROP_FRAME_COUNT Number of frames in the video file.
//CV_CAP_PROP_FORMAT Format of the Mat objects returned by retrieve() .
//CV_CAP_PROP_MODE Backend-specific value indicating the current capture mode.
//CV_CAP_PROP_BRIGHTNESS Brightness of the image (only for cameras).
//CV_CAP_PROP_CONTRAST Contrast of the image (only for cameras).
//CV_CAP_PROP_SATURATION Saturation of the image (only for cameras).
//CV_CAP_PROP_HUE Hue of the image (only for cameras).
//CV_CAP_PROP_GAIN Gain of the image (only for cameras).
//CV_CAP_PROP_EXPOSURE Exposure (only for cameras).
//CV_CAP_PROP_CONVERT_RGB Boolean flags indicating whether images should be converted to RGB.
//CV_CAP_PROP_WHITE_BALANCE_U The U value of the whitebalance setting (note: only supported by DC1394 v 2.x backend currently)
//CV_CAP_PROP_WHITE_BALANCE_V The V value of the whitebalance setting (note: only supported by DC1394 v 2.x backend currently)
//CV_CAP_PROP_RECTIFICATION Rectification flag for stereo cameras (note: only supported by DC1394 v 2.x backend currently)
//CV_CAP_PROP_ISO_SPEED The ISO speed of the camera (note: only supported by DC1394 v 2.x backend currently)
//CV_CAP_PROP_BUFFERSIZE Amount of frames stored in internal buffer memory (note: only supported by DC1394 v 2.x backend currently)
