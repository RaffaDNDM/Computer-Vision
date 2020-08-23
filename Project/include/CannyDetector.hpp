/**
	@file CannyDetector.hpp
	@brief Class for canny detection.
	@author Di Nardo Di Maio Raffaele 1204879
*/

#ifndef CANNY_DETECTOR
#define CANNY_DETECTOR

#include "Project.hpp"
#include "Utility.hpp"

class CannyDetector
{
public:
	/**
		@brief Canny detector.
		@param vector of test images
	*/
	CannyDetector(cv::Mat img, Utility::Type type);

	CannyDetector(cv::Mat img, Utility::Type type, double threshold1, double threshold2);

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
	cv::Mat _input_img;
	cv::Mat _result_img;
	cv::String _window;
	Utility::Type _dataset_type;
	double _threshold1 = 1.0;
	double _threshold2 = 1.0;
};
#endif