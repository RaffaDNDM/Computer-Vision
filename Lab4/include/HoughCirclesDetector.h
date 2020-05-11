/**
	@file HoughCirclesDetector.h
	@brief Class for Hough transform to detect signal.
	@author Di Nardo Di Maio Raffaele 1204879
*/

#ifndef HOUGH_CIRCLES
#define HOUGH_CIRCLES

#include "Lab4.h"

class HoughCirclesDetector {

public:
	/**
		@brief Constructor.
		@param input_img
		@param dp Inverse ratio of the accumulator resolution
		@param minDist Minimum distance between the centers of the detected circles
		@param param1 First method-specific parameter
		@param param2 Second method-specific parameter
		@param minRadius Minimum circle radius
		@param maxRadius Maximum circle radius
	*/
	HoughCirclesDetector(cv::Mat input_img, int dp, int minDist, int param1, int param2, int minRadius, int maxRadius);

	/**
		@brief Initialization of Hough transform window for circle detection.
	*/
	void detectCircles();

	/**
		@brief Callback for the trackbars of Hough transform window.
		@param value value of modfied trackbar that User set
		@param params object passed as parameter to trackbar
	*/
	static void houghCirclesTrackbar(int value, void* params);

	/**
		@brief Detection of circles.
	*/
	void houghCircles();

	/**
		@brief Set the input image.
		@param img new input image
	*/
	void setInput(cv::Mat img);

	/**
		@brief Get the input image.
		@return input image
	*/
	cv::Mat getInput();

	/**
		@brief Set the output image.
		@param img new output image
	*/
	void setResult(cv::Mat img);

	/**
		@brief Get the output image.
		@return output image
	*/
	cv::Mat getResult();

	/**
		@brief Set the inverse ratio of the accumulator resolution.
		@param dp new dp value
	*/
	void setdp(int dp);

	/**
		@brief Get the inverse ratio of the accumulator resolution.
		@return dp value
	*/
	double getdp();

	/**
		@brief Set the minimum distance between the centers of the detected circles.
		@param dist new minimum distance
	*/
	void setMinDist(int dist);

	/**
		@brief Get the minimum distance between the centers of the detected circles.
		@return minimum distance
	*/
	double getMinDist();

	/**
		@brief Set the first method-specific parameter.
		@param param new first parameter value
	*/
	void setParam1(int param);

	/**
		@brief Get the first method-specific parameter.
		@return first parameter value
	*/
	double getParam1();

	/**
		@brief Set the second method-specific parameter.
		@param param new second parameter value
	*/
	void setParam2(int param);

	/**
		@brief Get the second method-specific parameter.
		@return second parameter value
	*/
	double getParam2();

	/**
		@brief Set the minimum circle radius.
		@param radius new minimum circle radius value
	*/
	void setMinRadius(int radius);

	/**
		@brief Get the minimum circle radius.
		@return minimum circle radius value
	*/
	int getMinRadius();

	/**
		@brief Set the maximum circle radius.
		@param radius new maximum circle radius value
	*/
	void setMaxRadius(int radius);

	/**
		@brief Get the maximum circle radius.
		@return maximum circle radius value
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
