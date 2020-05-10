#ifndef STREET_DETECTOR
#define STREET_DETECTOR

#include "Lab4.h"
#include "CannyDetector.h"
#include "HoughLinesDetector.h"
#include "HoughCirclesDetector.h"

/**
	Class implementing Street and Signal detection
*/
class StreetSignalDetector {

public:
	/**
		Constructor
		\param input_img input image for which we want to highlight street and signal
	*/
	StreetSignalDetector(cv::Mat input_img);

	/**
		Perform detection (Canny + Hough)
	*/
	void detect();

	/**
		Get the output of the filter
	*/
	cv::Mat getResult();

	/**
		Set input image
	*/
	void setInput(cv::Mat img);

	/**
		Get input image
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