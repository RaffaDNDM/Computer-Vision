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
		@param img input RGB image
		@param type type of dataset (CAN, DRILLER or DUCK)
		@param threshold1
	*/
	CannyDetector(cv::Mat img, Utility::Type type, double low_threshold, double high_threshold);

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
	//Input image
	cv::Mat _input_img;

	//Image with detected edges
	cv::Mat _result_img;

	//Type of dataset on which the detection is performeed
	Utility::Type _dataset_type;

	//Low threshold for Hysteresis phase
	double _low_threshold = 1.0;

	//High threshold for Hysteresis phase
	double _high_threshold = 1.0;
};
#endif