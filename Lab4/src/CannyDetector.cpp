#include "CannyDetector.h"

/*
cv::Mat BWimg;
cv::cvtColor(img, BWimg, cv::COLOR_BGR2GRAY);
*/

//Constructor
CannyDetector::CannyDetector(cv::Mat input_img, double threshold1, double threshold2, int aperture_size):
	_input_img{ input_img },
	_threshold1{ threshold1},
	_threshold2{ threshold2 },
	_aperture_size{ aperture_size}
{}

//perform edge detection
void CannyDetector::detect()
{

}

//set the input image
void CannyDetector::setInput(cv::Mat img)
{
	_input_img = img;
}

//get the input image
cv::Mat CannyDetector::getInput()
{
	return _input_img;
}

// get the output of the filter
cv::Mat CannyDetector::getResult()
{
	return _result_img;
}

//set the aperture size
void CannyDetector::setApertureSize(int size)
{
	_aperture_size = size;
}

//get the aperture Size
int CannyDetector::getApertureSize()
{
	return _aperture_size;
}

//set the first threshold
void CannyDetector::setThresh1(double threshold)
{
	_threshold1 = threshold;
}

//get the first threshold
double CannyDetector::getThresh1()
{
	return _threshold1;
}

//set the second threshold
void CannyDetector::setThresh2(double threshold)
{
	_threshold2 = threshold;
}

//get the second threshold
double CannyDetector::getThresh2()
{
	return _threshold2;
}