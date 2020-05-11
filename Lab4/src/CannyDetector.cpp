#include "CannyDetector.h"

CannyDetector::CannyDetector(cv::Mat input_img, int threshold1, int threshold2, int aperture_size):
	_input_img{ input_img.clone() },
	_threshold1{ threshold1},
	_threshold2{ threshold2 },
	_aperture_size{ aperture_size},
	_result_img{input_img.clone()}
{}

void CannyDetector::detect()
{
	blur(_input_img, _input_img, cv::Size(9,9));
	cv::namedWindow(window, cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO | cv::WINDOW_GUI_EXPANDED);
	cv::createTrackbar("Threshold 1", window, &(_threshold1), 200, cannyTrackbar, (void*) this);
	cv::createTrackbar("Threshold 2", window, &(_threshold2), 200, cannyTrackbar, (void*) this);
	cv::imshow(window, _result_img);
}

void CannyDetector::cannyTrackbar(int value, void* params)
{
	CannyDetector* cd = (CannyDetector*)params;
	cd->canny();
}

void CannyDetector::canny()
{
	cv::Canny(getInput(), _result_img, getThresh1(), getThresh2(), getApertureSize());
	cv::imshow(window, _result_img);
	_modified = true;
}

void CannyDetector::setInput(cv::Mat img)
{
	_input_img = img;
}

cv::Mat CannyDetector::getInput()
{
	return _input_img;
}

cv::Mat CannyDetector::getResult()
{
	return _result_img;
}

void CannyDetector::setModified(bool value)
{
	_modified = value;
}

bool CannyDetector::getModified()
{
	return _modified;
}

void CannyDetector::setApertureSize(int size)
{
	_aperture_size = size;
}

int CannyDetector::getApertureSize()
{
	return _aperture_size;
}

void CannyDetector::setThresh1(int threshold)
{
	_threshold1 = threshold;
}

double CannyDetector::getThresh1()
{
	return _threshold1*RATIO1;
}

void CannyDetector::setThresh2(int threshold)
{
	_threshold2 = threshold;
}

double CannyDetector::getThresh2()
{
	return _threshold2*RATIO2;
}
