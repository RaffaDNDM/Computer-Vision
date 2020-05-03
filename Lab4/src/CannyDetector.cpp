#include "CannyDetector.h"

/*
cv::Mat BWimg;
cv::cvtColor(img, BWimg, cv::COLOR_BGR2GRAY);
*/

//Constructor
CannyDetector::CannyDetector(cv::Mat input_img, int threshold1, int threshold2, int aperture_size):
	_input_img{ input_img.clone() },
	_threshold1{ threshold1},
	_threshold2{ threshold2 },
	_aperture_size{ aperture_size},
	_result_img{input_img.clone()}
{}

//perform edge detection
void CannyDetector::detect()
{
	blur(_input_img, _input_img, cv::Size(5, 5));
	cv::namedWindow(window, cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO | cv::WINDOW_GUI_EXPANDED);
	cv::createTrackbar("Threshold 1", window, &(_threshold1), 200, cannyTrackbar, (void*) this);
	cv::createTrackbar("Threshold 2", window, &(_threshold2), 200, cannyTrackbar, (void*) this);
	cv::imshow(window, _result_img);
}

void CannyDetector::cannyTrackbar(int value, void* params)
{
	CannyDetector* cd = (CannyDetector*)params;
	cv::Canny(cd->getInput(), cd->_result_img, cd->getThresh1(), cd->getThresh2(), cd->getApertureSize());
	imshow(cd->window, cd->_result_img);

	cd->_modified = true;
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

//set the modified state
void CannyDetector::setModified(bool value)
{
	_modified = value;
}

//get the modified state
bool CannyDetector::getModified()
{
	return _modified;
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
void CannyDetector::setThresh1(int threshold)
{
	_threshold1 = threshold;
}

//get the first threshold
double CannyDetector::getThresh1()
{
	return _threshold1*RATIO1;
}

//set the second threshold
void CannyDetector::setThresh2(int threshold)
{
	_threshold2 = threshold;
}

//get the second threshold
double CannyDetector::getThresh2()
{
	return _threshold2*RATIO2;
}