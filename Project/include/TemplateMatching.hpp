/**
	@file PanoramicImage.h
	@brief Class for template matching.
	@author Di Nardo Di Maio Raffaele 1204879
*/

#ifndef TEMPLATE_MATCHING
#define TEMPLATE_MATCHING

#include "Project.hpp"
#include "Utility.hpp"

/**
	@brief Find best template matches in input images.
*/
class TemplateMatching {
public:
	TemplateMatching(cv::String path, Dataset::Type dataset_type);
	void printInfo();

private:
	void loadImages(cv::String path);

	/*
		Input images
		_input_imgs[0] = masks
		_input_imgs[1] = views
		_input_imgs[2] = test images
	*/
	std::vector<std::vector<cv::Mat>> _input_imgs;
	Dataset::Type _dataset_type;
};

class InputIMGException : public std::exception
{
public:
	virtual char const* what() const
	{
		return "No images found with this pattern.";
	}
};


#endif