#include "HoughTransform.h"

//Constructor
HoughTransform::HoughTransform(cv::Mat input_img, double rho, double theta, int threshold):
	_input_img{ input_img },
	_rho{ rho },
	_theta { theta },
	_threshold { threshold }
{}

//detect lines through Hough transform
void HoughTransform::detectLines()
{

}

//detect circles through Hough transform
void HoughTransform::detectCircles()
{

}

//set the input image
void HoughTransform::setInput(cv::Mat img)
{
	_input_img = img;
}

//get the input image
cv::Mat HoughTransform::getInput()
{
	return _input_img;
}

// get the output
cv::Mat HoughTransform::getResult()
{
	return _result_img;
}

//set the distance resolution
void HoughTransform::setRho(double rho)
{
	_rho = rho;
}

//get the distance resolution
double HoughTransform::getRho()
{
	return _rho;
}

//set the angle resolution
void HoughTransform::setTheta(double theta)
{
	_theta = theta;
}

//get the angle resolution
double HoughTransform::getTheta()
{
	return _theta;
}

//set the accumulator threshold
void HoughTransform::setThresh(int threshold)
{
	_threshold = threshold;
}

//get the accumulator threshold
int HoughTransform::getThresh()
{
	return _threshold;
}