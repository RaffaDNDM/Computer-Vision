#include "StreetDetector.h"

//Constructor
StreetDetector::StreetDetector(cv::Mat input_img):
	_input_img{ input_img },
	_cd{ CannyDetector::CannyDetector(_input_img, 0, 0, 3) },
	_ht{ HoughTransform::HoughTransform(_input_img, 0.0, 0.0, 0) }
{}

// perform detection (Canny + Hough)
void StreetDetector::detect()
{

}

// get the output of the filter
cv::Mat StreetDetector::getResult()
{
	return _result_img;
}

//set input image
void StreetDetector::setInput(cv::Mat img)
{
	_input_img = img;
}

//get input image
cv::Mat StreetDetector::getInput()
{
	return _input_img;
}