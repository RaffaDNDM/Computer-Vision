#ifndef HOUGH_CIRCLES
#define HOUGH_CIRCLES

#include "Lab4.h"

class HoughCirclesDetector {

public:
	//Constructor
	HoughCirclesDetector(cv::Mat input_img, int dp, int minDist, int param1, int param2, int minRadius, int maxRadius);

	//detect lines through Hough transform
	void detectCircles();

	//set the input image
	void setInput(cv::Mat img);

	//get the input image
	cv::Mat getInput();

	// get the output
	cv::Mat getResult();

	//set the distance resolution
	void setdp(int dp);

	//get the distance resolution
	double getdp();

	//set the angle resolution
	void setMinDist(int dist);

	//get the angle resolution
	double getMinDist();

	//set the distance resolution
	void setParam1(int param);

	//get the distance resolution
	double getParam1();

	//set the distance resolution
	void setParam2(int param);

	//get the distance resolution
	double getParam2();

	//set the angle resolution
	void setMinRadius(int radius);

	//get the angle resolution
	int getMinRadius();

	//set the angle resolution
	void setMaxRadius(int radius);

	//get the angle resolution
	int getMaxRadius();

	static void houghCirclesTrackbar(int value, void* params);

	void houghCircles();

private:
	// input image
	cv::Mat _input_img;

	//distance resolution of the accumulator in pixels
	int _dp;

	//angle resolution of the accumulator in radians
	int _minDist;

	//accumulator threshold
	int _param1;

	int _param2;

	int _minRadius;

	int _maxRadius;

	// output image
	cv::Mat _result_img;

	const std::string window = "Hough Signal Detection";
	const double RATIO_DP = 1.0;
	const double RATIO_MIN_DIST = 1.0;
	const double RATIO_PARAM1 = 1.0;
	const double RATIO_PARAM2 = 1.0;
};

#endif