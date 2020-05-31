/**
	@file Filter.cpp
	@brief Filters.
	@author Di Nardo Di Maio Raffaele
*/

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "Filter.h"

//using namespace cv;

	// constructor
	Filter::Filter(cv::Mat input_img, int size) {

		input_image = input_img;
		if (size % 2 == 0)
			size++;
		filter_size = size;
	}

	// for base class do nothing (in derived classes it performs the corresponding filter)
	void Filter::doFilter() {

		// it just returns a copy of the input image
		result_image = input_image.clone();

	}

	// get output of the filter
	cv::Mat Filter::getResult() {

		return result_image;
	}

	//set window size (it needs to be odd)
	void Filter::setSize(int size) {

		if (size % 2 == 0)
			size++;
		filter_size = size;
	}

	//get window size 
	int Filter::getSize() {

		return filter_size;
	}



	// Write your code to implement the Gaussian, median and bilateral filters
	GaussianFilter::GaussianFilter(cv::Mat input_img, int filter_size, double sigma_x) :
		Filter::Filter(input_img, filter_size),
		sigma{ sigma_x }
	{
	}

	void GaussianFilter::doFilter()
	{
		cv::Size size = cv::Size(filter_size, filter_size);
		cv::GaussianBlur(input_image, result_image, size, sigma);
	}

	// Write your code to implement the Gaussian, median and bilateral filters
	MedianFilter::MedianFilter(cv::Mat input_img, int filter_size) :
		Filter::Filter(input_img, filter_size)
	{}


	void MedianFilter::doFilter()
	{
		cv::medianBlur(input_image, result_image, filter_size);
	}

	// Write your code to implement the Gaussian, median and bilateral filters
	BilateralFilter::BilateralFilter(cv::Mat input_img, double sigma_range_in, double sigma_space_in):
		Filter(input_img, KERNEL_SIZE),
		sigma_range {sigma_range_in},
		sigma_space {sigma_space_in}
	{}

	void BilateralFilter::doFilter()
	{
		cv::bilateralFilter(input_image, result_image, filter_size, sigma_range, sigma_space);
	}