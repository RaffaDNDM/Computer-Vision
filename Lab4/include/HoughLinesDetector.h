/**
	@file HoughLinesDetector.h
	@brief Class for Hough transform to detect street.
	@author Di Nardo Di Maio Raffaele 1204879
*/

#ifndef HOUGH_LINES
#define HOUGH_LINES

#include "Lab4.h"

class HoughLinesDetector {

public:
	/**
		@brief Constructor.
		@param input_img input image
		@param rho distance resolution
		@param theta angle resolution
		@param threshold accumulator threshold
	*/
	HoughLinesDetector(cv::Mat input_img, int rho, int theta, int threshold);

	/**
		@brief Initialization of Hough transform window for line detection.
	*/
	void detectLines();

	/**
		@brief Callback for the trackbars of Hough transform window.
		@param value value of modfied trackbar that User set
		@param params object passed as parameter to trackbar
	*/
	static void houghLinesTrackbar(int value, void* params);

	/**
		@brief Detection of lines.
	*/
	void houghLines();

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
		@brief Get the image with detected street.
		@param img new result image
	*/
	void setResult(cv::Mat img);

	/**
		@brief Get the image with detected street.
		@return output image
	*/
	cv::Mat getResult();

	/**
		@brief Set the distance resolution.
		@param rho new value for distance resolution
	*/
	void setRho(int rho);

	/**
		@brief Get the distance resolution.
		@return distance resolution
	*/
	double getRho();

	/**
		@brief Set the angle resolution.
		@param theta new value for angle resolution
	*/
	void setTheta(int theta);

	/**
		@brief Get the angle resolution.
		@return angle resolution
	*/
	double getTheta();

	/**
		@brief Set the accumulator threshold.
		@param threshold new value for accumulator threshold
	*/
	void setThresh(int threshold);

	/**
		@brief Get the accumulator threshold.
		@return accumulator threshold
	*/
	int getThresh();

private:
	//Input image (GRAY image)
	cv::Mat _input_img;

	//Detected image
	cv::Mat _detected_img;

	//Output image
	cv::Mat _result_img;

	//Distance resolution of the accumulator in pixels
	int _rho;

	//Angle resolution of the accumulator in radians
	int _theta;

	//Accumulator threshold
	int _threshold;

	//Name of the window
	const std::string window = "Hough Street Detection";

	//Step of distance resolution trackbar
	const double RATIO_RHO = 1.0;

	//Step of angle resolution trackbar
	const double RATIO_THETA = (CV_PI / 150);

	//Step of accumulator threshold trackbar
	const int RATIO_THRESHOLD = 5;
};

#endif
