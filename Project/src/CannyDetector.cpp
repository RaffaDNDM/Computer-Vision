/**
	@file CannyDetector.cpp
	@brief Implementation of class for canny detection.
	@author Di Nardo Di Maio Raffaele 1204879
*/

#include "CannyDetector.hpp"

CannyDetector::CannyDetector(cv::Mat img, Utility::Type type, double low_threshold, double high_threshold) :
	_input_img{ img.clone() },
	_result_img{ img.clone() },
	_dataset_type{ type },
	_low_threshold{ low_threshold },
	_high_threshold{ high_threshold }
{ }

void CannyDetector::detect()
{
	//Computation of Canny detection
	cv::Canny(_input_img, _result_img, _low_threshold, _high_threshold);
}

cv::Mat CannyDetector::getResult()
{
	return _result_img.clone();
}