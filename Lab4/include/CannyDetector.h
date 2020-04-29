#ifndef CANNY_DETECTOR
#define CANNY_DETECTOR

#include "Lab4.h"

// Generic class implementing Canny edge detection
class CannyDetector {

public:
	//Constructor
	CannyDetector(cv::Mat input_img, double threshold1, double threshold2, int aperture_size);

	//perform edge detection
	void detect();

	//set the input image
	void setInput(cv::Mat img);

	//get the input image
	cv::Mat getInput();

	// get the output of the filter
	cv::Mat getResult();

	//set the aperture size
	void setApertureSize(int size);

	//get the aperture Size
	int getApertureSize();

	//set the first threshold
	void setThresh1(double threshold);

	//get the first threshold
	double getThresh1();

	//set the second threshold
	void setThresh2(double threshold);

	//get the second threshold
	double getThresh2();


private:
	// input image
	cv::Mat _input_img;

	//First threshold
	double _threshold1;

	//Second threshold	
	double _threshold2;

	//Aperture size of Sobel operator
	int _aperture_size;

	// output image (filter result)
	cv::Mat _result_img;
};

#endif