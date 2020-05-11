/**
	@file StreetDetector.h
	@brief Class for Street and Signal detection.
	@author Di Nardo Di Maio Raffaele 1204879
*/

#ifndef STREET_DETECTOR
#define STREET_DETECTOR

#include "Lab4.h"
#include "CannyDetector.h"
#include "HoughLinesDetector.h"
#include "HoughCirclesDetector.h"

class StreetSignalDetector {

public:
	/**
		@brief Constructor.
		@param input_img input image for which we want to highlight street and signal
	*/
	StreetSignalDetector(cv::Mat input_img);

	/**
		@brief Perform detection (Canny + Hough).
	*/
	void detect();

	/**
		@brief Get the output of the filter.
		@return result image
	*/
	cv::Mat getResult();

	/**
		@brief Set input image.
		@param img new input image
	*/
	void setInput(cv::Mat img);

	/**
		@brief Get input image.
		@return input image
	*/
	cv::Mat getInput();

private:
	//Input image
	cv::Mat _input_img;

	//Output image (image with highlighted street and signal)
	cv::Mat _result_img;

	//Canny detector
	CannyDetector _cd;

	//Hough transform to detect street
	HoughLinesDetector _hld;

	//Hough transform to detect signal
	HoughCirclesDetector _hcd;
};

#endif
