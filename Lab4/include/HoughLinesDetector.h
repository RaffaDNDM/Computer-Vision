#ifndef HOUGH_LINES
#define HOUGH_LINES

#include "Lab4.h"

class HoughLinesDetector {

public:
	//Constructor
	HoughLinesDetector(cv::Mat input_img, int rho, int theta, int threshold);

	//detect lines through Hough transform
	void detectLines();

	//detect circles through Hough transform
	void detectCircles();

	//set the input image
	void setInput(cv::Mat img);

	//get the input image
	cv::Mat getInput();

	// get the output
	cv::Mat getResult();

	//set the distance resolution
	void setRho(int rho);

	//get the distance resolution
	double getRho();

	//set the angle resolution
	void setTheta(int theta);

	//get the angle resolution
	double getTheta();

	//set the accumulator threshold
	void setThresh(int threshold);

	//get the accumulator threshold
	int getThresh();

	static void houghLinesTrackbar(int value, void* params);

	void houghLines();

private:
	// input image
	cv::Mat _input_img;

	//distance resolution of the accumulator in pixels
	int _rho;

	//angle resolution of the accumulator in radians
	int _theta;

	//accumulator threshold
	int _threshold;

	// output image
	cv::Mat _result_img;

	const std::string window = "Hough Street Detection";
	const double RATIO_RHO = 0.1;
	const double RATIO2 = 100.0;
};

#endif