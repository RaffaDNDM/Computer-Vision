#ifndef STREET_DETECTOR
#define STREET_DETECTOR

#include "Lab4.h"
#include "CannyDetector.h"
#include "HoughLinesDetector.h"
#include "HoughCirclesDetector.h"

// Generic class implementing a filter with the input and output image data and the parameters
class StreetDetector {

public:
	//Constructor
	StreetDetector(cv::Mat input_img);

	// perform detection (Canny + Hough)
	void detect();

	// get the output of the filter
	cv::Mat getResult();

	//set input image
	void setInput(cv::Mat img);

	//get input image
	cv::Mat getInput();

private:
	// input image
	cv::Mat _input_img;

	// output image (filter result)
	cv::Mat _result_img;

	//Canny detector
	CannyDetector _cd;

	//Hough transform
	HoughLinesDetector _hld;
	HoughCirclesDetector _hcd;
};

#endif