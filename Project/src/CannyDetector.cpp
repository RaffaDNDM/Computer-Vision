/**
	@file CannyDetector.cpp
	@brief Implementation of class for canny detection.
	@author Di Nardo Di Maio Raffaele 1204879
*/

#include "CannyDetector.hpp"

CannyDetector::CannyDetector(cv::Mat img, Utility::Type type) :
	_input_img{img.clone()},
	_result_img{img.clone()},
	_dataset_type{type},
	_window{ "Canny " + Utility::types[static_cast<int>(type)] }
{}

CannyDetector::CannyDetector(cv::Mat img, Utility::Type type, double threshold1, double threshold2) :
	_input_img{ img.clone() },
	_result_img{ img.clone() },
	_dataset_type{ type },
	_window{ "Canny " + Utility::types[static_cast<int>(type)] },
	_threshold1{ threshold1 },
	_threshold2{ threshold2 }
{ }

void CannyDetector::detect()
{
	cv::Canny(_input_img, _result_img, _threshold1, _threshold2);
}

cv::Mat CannyDetector::getResult()
{
	return _result_img.clone();
}