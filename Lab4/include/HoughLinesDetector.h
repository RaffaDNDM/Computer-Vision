#ifndef HOUGH_LINES
#define HOUGH_LINES

#include "Lab4.h"

/**
	Class implementing Hough transform to detect street
*/
class HoughLinesDetector {

public:
	/**
		Constructor
		\param input_img input image
		\param rho
		\param theta
		\threshold
	*/
	HoughLinesDetector(cv::Mat input_img, int rho, int theta, int threshold);

	/**
		Initialization of Hough transform window for line detection
	*/
	void detectLines();

	/**
		Callback for the trackbars of Hough transform window
		\param value value of modfied trackbar that User set
		\param params object passed as parameter to trackbar
	*/
	static void houghLinesTrackbar(int value, void* params);

	/**
		Detection of lines
	*/
	void houghLines();

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
		Get the image with detected street
		\returns detected street image
	*/
	void setResult(cv::Mat img);

	/**
		Get the image with detected street
		\returns detected street image
	*/
	cv::Mat getResult();

	/**
		Set the distance resolution
		\param rho new value for distance resolution
	*/
	void setRho(int rho);

	/**
		Get the distance resolution
		\returns distance resolution
	*/
	double getRho();

	/**
		Set the angle resolution
		\param theta new value for angle resolution 
	*/
	void setTheta(int theta);

	/**
		Get the angle resolution
		\returns angle resolution
	*/
	double getTheta();

	/**
		Set the accumulator threshold
		\param threshold new value for accumulator threshold 
	*/
	void setThresh(int threshold);

	/**
		Get the accumulator threshold
		\returns accumulator threshold
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


	const std::string window = "Hough Street Detection";
	const double RATIO_RHO = 1.0;
	const double RATIO_THETA = (CV_PI / 150);
	const int RATIO_THRESHOLD = 5;
};

#endif