#include "CannyDetector.hpp"

CannyDetector::CannyDetector(cv::Mat img, Dataset::Type type) :
	_input_img{img.clone()},
	_result_img{img.clone()},
	_dataset_type{type},
	_window{ "Canny " + Dataset::types[static_cast<int>(type)] }
{
	/*
	* If we use the same thresholds for template and test image
	cv::GaussianBlur(_input_img, _input_img, cv::Size(9, 9), 3.0);
	*/
	//cv::GaussianBlur(_input_img, _input_img, cv::Size(3, 3), 0);
	//cv::blur(_input_img, _input_img, cv::Size(3, 3));
}

CannyDetector::CannyDetector(cv::Mat img, Dataset::Type type, int threshold1, int threshold2) :
	_input_img{ img.clone() },
	_result_img{ img.clone() },
	_dataset_type{ type },
	_window{ "Canny " + Dataset::types[static_cast<int>(type)] },
	_threshold1{ threshold1 },
	_threshold2{ threshold2 }
{ }

void CannyDetector::detect()
{
	//cv::namedWindow(_window, cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO | cv::WINDOW_GUI_EXPANDED);
	//cv::createTrackbar("Threshold 1", _window, &(_threshold1), 2000, cannyTrackbar, (void*)this);
	//cv::createTrackbar("Threshold 2", _window, &(_threshold2), 2000, cannyTrackbar, (void*)this);
	cv::Canny(_input_img, _result_img, ((double) _threshold1) * 0.1, ((double) _threshold2) * 0.1);
	//cv::imshow(_window, _result_img);
	//cv::waitKey(0);
}

void CannyDetector::cannyTrackbar(int value, void* params)
{
	CannyDetector* cd = (CannyDetector*)params;
	cv::Canny(cd->_input_img, cd->_result_img, ((double) cd->_threshold1) * 0.1, ((double) cd->_threshold2) * 0.1);
	//cv::imshow(cd->_window, cd->_result_img);
}

cv::Mat CannyDetector::getResult()
{
	return _result_img.clone();
}