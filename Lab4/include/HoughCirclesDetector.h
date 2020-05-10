#ifndef HOUGH_CIRCLES
#define HOUGH_CIRCLES

#include "Lab4.h"

/**
	Class implementing Hough transform to detect signal
*/
class HoughCirclesDetector {

public:
	/**
		Constructor
		\param input_img
		\param dp Inverse ratio of the accumulator resolution
		\param minDist Minimum distance between the centers of the detected circles
		\param param1 First method-specific parameter
		\param param2 Second method-specific parameter
		\param minRadius Minimum circle radius
		\param maxRadius Maximum circle radius
	*/
	HoughCirclesDetector(cv::Mat input_img, int dp, int minDist, int param1, int param2, int minRadius, int maxRadius);

	/**
		Initialization of Hough transform window for circle detection
	*/
	void detectCircles();

	/**
		Callback for the trackbars of Hough transform window
		\param value value of modfied trackbar that User set
		\param params object passed as parameter to trackbar
	*/
	static void houghCirclesTrackbar(int value, void* params);

	/**
		Detection of circles
	*/
	void houghCircles();

	/**
		Set the input image
		\param img new input image
	*/
	void setInput(cv::Mat img);

	/**
		Get the input image
		\returns input image
	*/
	cv::Mat getInput();

	/**
		Set the output image
		\param new output image
	*/
	void setResult(cv::Mat img);

	/**
		Get the output image
		\returns output image
	*/
	cv::Mat getResult();

	/**
		Set the inverse ratio of the accumulator resolution
		\param dp new dp value 
	*/
	void setdp(int dp);

	/**
		Get the inverse ratio of the accumulator resolution
		\returns dp value
	*/
	double getdp();

	/**
		Set the minimum distance between the centers of the detected circles
		\param dist new minimum distance 
	*/
	void setMinDist(int dist);

	/**
		Get the minimum distance between the centers of the detected circles
		\returns minimum distance 
	*/
	double getMinDist();

	/**
		Set the first method-specific parameter
		\param param new first parameter value
	*/
	void setParam1(int param);

	/**
		Get the first method-specific parameter
		\param first parameter value
	*/
	double getParam1();

	/**
		Set the second method-specific parameter
		\param param new second parameter value
	*/
	void setParam2(int param);

	/**
		Get the second method-specific parameter
		\param second parameter value
	*/
	double getParam2();

	/**
		Set the minimum circle radius
		\param radius new minimum circle radius value
	*/
	void setMinRadius(int radius);

	/**
		Get the minimum circle radius
		\returns minimum circle radius value
	*/
	int getMinRadius();

	/**
		Set the maximum circle radius
		\param radius new maximum circle radius value
	*/
	void setMaxRadius(int radius);

	/**
		Get the maximum circle radius
		\returns maximum circle radius value
	*/
	int getMaxRadius();

private:
	//Input image
	cv::Mat _input_img;

	//Detected input img
	cv::Mat _detected_img;

	//Output image
	cv::Mat _result_img;

	//Inverse ratio of the accumulator resolution
	int _dp;

	//Minimum distance between the centers of the detected circles
	int _minDist;

	//First method-specific parameter
	int _param1;

	//Second method-specific parameter
	int _param2;

	//Minimum circle radius
	int _minRadius;

	//Maximum circle radius
	int _maxRadius;

	//Name of the window
	const std::string window = "Hough Signal Detection";
	
	//Step of dp trackbar
	const double RATIO_DP = 1.0;

	//Step of minimum distance trackbar
	const double RATIO_MIN_DIST = 1.0;

	//Step of first parameter trackbar
	const double RATIO_PARAM1 = 1.0;

	//Step of second parameter trackbar
	const double RATIO_PARAM2 = 1.0;
};

#endif