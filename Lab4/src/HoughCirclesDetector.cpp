#include "HoughCirclesDetector.h"

//Constructor
HoughCirclesDetector::HoughCirclesDetector(cv::Mat input_img, int dp, int minDist, int param1, 
	                                       int param2, int minRadius, int maxRadius):
	_input_img{ input_img.clone() }, 
	_dp{ dp },
	_minDist{ minDist },
	_param1{ param1 },
	_param2{ param2 },
	_minRadius{ minRadius },
	_maxRadius{ maxRadius },
	_result_img{ input_img.clone() }
{}

//detect lines through Hough transform
void HoughCirclesDetector::detectCircles()
{
	cv::namedWindow(window, cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO | cv::WINDOW_GUI_EXPANDED);
	cv::createTrackbar("DP", window, &(_dp), 50, houghCirclesTrackbar, (void*)this);
	cv::createTrackbar("Min dist", window, &(_minDist), 50, houghCirclesTrackbar, (void*)this);
	cv::createTrackbar("Param1", window, &(_param1), 50, houghCirclesTrackbar, (void*)this);
	cv::createTrackbar("Param2", window, &(_param2), 50, houghCirclesTrackbar, (void*)this);
	cv::createTrackbar("Min radius", window, &(_minRadius), 50, houghCirclesTrackbar, (void*)this);
	cv::createTrackbar("Max radius", window, &(_maxRadius), 50, houghCirclesTrackbar, (void*)this);
	cv::imshow(window, _result_img);
}

//set the input image
void HoughCirclesDetector::setInput(cv::Mat img)
{
	_input_img = img;
}

//get the input image
cv::Mat HoughCirclesDetector::getInput()
{
	return _input_img;
}

// get the output
cv::Mat HoughCirclesDetector::getResult()
{
	return _result_img;
}

//set the distance resolution
void HoughCirclesDetector::setdp(int dp)
{
	_dp = dp;
}

//get the distance resolution
double HoughCirclesDetector::getdp()
{
	return _dp * RATIO_DP;
}

//set the angle resolution
void HoughCirclesDetector::setMinDist(int dist)
{
	_minDist = dist;
}

//get the angle resolution
double HoughCirclesDetector::getMinDist()
{
	return _minDist * RATIO_MIN_DIST;
}

//get the distance resolution
double HoughCirclesDetector::getParam1()
{
	return _param1 * RATIO_PARAM1;
}

//set the distance resolution
void HoughCirclesDetector::setParam2(int param)
{
	_param1 = param;
}

//get the distance resolution
double HoughCirclesDetector::getParam2()
{
	return _param2 * RATIO_PARAM2;
}

//set the angle resolution
void HoughCirclesDetector::setMinRadius(int radius)
{
	_minRadius = radius;
}

//get the angle resolution
int HoughCirclesDetector::getMinRadius()
{
	return _minRadius;
}

//set the angle resolution
void HoughCirclesDetector::setMaxRadius(int radius)
{
	_maxRadius = radius;
}

//get the angle resolution
int HoughCirclesDetector::getMaxRadius()
{
	return _maxRadius;
}

void HoughCirclesDetector::houghCirclesTrackbar(int value, void* params)
{
	HoughCirclesDetector* hcd = (HoughCirclesDetector*) params;
	hcd->houghCircles();
}

void HoughCirclesDetector::houghCircles()
{}

