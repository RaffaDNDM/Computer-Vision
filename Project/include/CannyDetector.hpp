#include "Project.hpp"
#include "Utility.hpp"

#ifndef CANNY_DETECTOR
#define CANNY_DETECTOR

class CannyDetector
{
public:
	/**
		@brief Canny detector.
		@param vector of test images
	*/
	CannyDetector(cv::Mat img, Dataset::Type type);

	/**
		@brief Performs the Canny detection.
		@param result_imgs result imgs with detected edges
	*/
	void detect();

	/**
		@brief Returns detected image.
		@return result img with detected edges
	*/
	cv::Mat getResult();

private:
	/**
		@brief Callback for the trackbars of Canny window.
		@param value value of modfied trackbar that User set
		@param params object passed as parameter to trackbar
	*/
	static void cannyTrackbar(int value, void* params);

	cv::Mat _input_img;
	cv::Mat _result_img;
	cv::String _window;
	Dataset::Type _dataset_type;
	int _threshold1=1421;
	int _threshold2=300;

};
#endif