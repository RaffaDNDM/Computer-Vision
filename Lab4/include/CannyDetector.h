#ifndef CANNY_DETECTOR
#define CANNY_DETECTOR

#include "Lab4.h"

// Generic class implementing Canny edge detection
class CannyDetector {

public:
	//Constructor
	CannyDetector(cv::Mat input_img, int threshold1, int threshold2, int aperture_size);

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
	void setThresh1(int threshold);

	//get the first threshold
	double getThresh1();

	//set the second threshold
	void setThresh2(int threshold);

	//get the second threshold
	double getThresh2();

	//trackbar of Canny image
	static void cannyTrackbar(int value, void* params);

	//set the modified state
	void setModified(bool value);

	//get the modified state
	bool getModified();

private:
	// input image
	cv::Mat _input_img;

	//First threshold
	int _threshold1;

	//Second threshold	
	int _threshold2;

	//Aperture size of Sobel operator
	int _aperture_size;

	// output image (filter result)
	cv::Mat _result_img;

	bool _modified = false;

	const std::string window = "Canny image";
	const double RATIO1 = 1.0;
	const double RATIO2 = 1.0;
};
#endif