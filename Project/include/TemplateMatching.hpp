/**
	@file TemplateMatching.h
	@brief Class for template matching.
	@author Di Nardo Di Maio Raffaele 1204879
*/

#include "Project.hpp"
#include "Utility.hpp"

#ifndef TEMPLATE_MATCHING
#define TEMPLATE_MATCHING

/**
	@brief Find best template matches in input images.
*/
class TemplateMatching {
public:
	/**
		@brief Constructor of TemplateMatching objects.
		@param path folder containing the 3 subfolders: can/, driller/, duck/
		@param dataset_type type of dataset that you want to analyse (can, driller, duck)
	*/
	TemplateMatching(cv::String path, Dataset::Type dataset_type);

	/**
		@brief Detection of edges from test images.
	*/
	void cannyDetection();

private:
	/**
		@brief Load images from the path.
		@param path folder containing the 3 subfolders: can/, driller/, duck/
	*/
	void loadImages(cv::String path);

	/**
		@brief Shows how many images have been loaded.
		@param path folder containing the 3 subfolders: can/, driller/, duck/
	*/
	void stateAcquisitionImages(cv::String path);

	/*
		Input images
		_input_imgs[0] = masks;
		_input_imgs[1] = views;
		_input_imgs[2] = test images;
	*/
	std::vector<std::vector<cv::Mat>> _input_imgs;
	//Images with detected edges using Canny
	std::vector<cv::Mat> _canny_imgs;
	//Type of dataset, we are analysing (can, driller or duck)
	Dataset::Type _dataset_type;
};

/**
	Exception thrown if the images are not found in specified folder
*/
class InputIMGException : public std::exception
{
public:
	virtual char const* what() const
	{
		return "No images found with this pattern.";
	}
};


#endif