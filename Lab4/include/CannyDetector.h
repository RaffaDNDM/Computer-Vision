/**
	@file CannyDetector.h
	@brief Class for Canny edge detection.
	@author Di Nardo Di Maio Raffaele 1204879
*/

#ifndef CANNY_DETECTOR
#define CANNY_DETECTOR

#include "Lab4.h"

class CannyDetector {

public:
	/**
		@brief Constructor.
		@param input_img RGB input image
		@param threshold1 first threshold for the hysteresis procedure
		@param threshold2 second threshold for the hysteresis procedure
		@param aperture_size aperture size for the Sobel() operator
	*/
	CannyDetector(cv::Mat input_img, int threshold1, int threshold2, int aperture_size);

	/**
		@brief Initialization of edge detection window.
	*/
	void detect();

	/**
		@brief Callback for the trackbars of Canny window.
		@param value value of modfied trackbar that User set
		@param params object passed as parameter to trackbar
	*/
	static void cannyTrackbar(int value, void* params);

	/**
		@brief Performs edge detection.
	*/
	void canny();

	/**
		@brief Set the input image equal to img.
		@param img image that is going to be set as input image in detection
	*/
	void setInput(cv::Mat img);

	/**
		@brief Get the input image.
		@return input image
	*/
	cv::Mat getInput();

	/**
		@brief Get the output of the detection.
		@return result image
	*/
	cv::Mat getResult();

	/**
		@brief Set the aperture size.
		@param size new value for aperture size
	*/
	void setApertureSize(int size);

	/**
		@brief Get the aperture size.
		@return aperture size
	*/
	int getApertureSize();

	/**
		@brief Set the first threshold for the hysteresis procedure.
		@param threshold new value for the threshold
	*/
	void setThresh1(int threshold);

	/**
		@brief Get the first threshold for the hysteresis procedure.
		@return value of the first threshold
	*/
	double getThresh1();

	/**
		@brief Set the second threshold for the hysteresis procedure.
		@param threshold new value for the threshold
	*/
	void setThresh2(int threshold);

	/**
		@brief Get the second threshold for the hysteresis procedure.
		@return value of the second threshold
	*/
	double getThresh2();

	/**
		@brief Set the modified state value.
		@param value new value of modified state
	*/
	void setModified(bool value);

	/**
		@brief Get the modified state value.
		@return value of modified state
	*/
	bool getModified();

private:
	//Input image
	cv::Mat _input_img;

	//Output image (image with detected edges)
	cv::Mat _result_img;

	//First threshold
	int _threshold1;

	//Second threshold
	int _threshold2;

	//Aperture size of Sobel operator
	int _aperture_size;

	//Modified state
	bool _modified = false;

	//Name of the window
	const std::string window = "Canny image";

	//Ratio for which first threshold is multiplied
	const double RATIO1 = 1.0;

	//Ratio for which first threshold is multiplied
	const double RATIO2 = 1.0;
};
#endif
