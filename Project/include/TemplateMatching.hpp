/**
	@file TemplateMatching.h
	@brief Class for template matching.
	@author Di Nardo Di Maio Raffaele 1204879
*/

#include "Project.hpp"
#include "Utility.hpp"
#include "BestResults.hpp"
#include "GammaTransform.hpp"

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

	/**
		@brief Shows best matches found.
		@param img_num number of the image for which we found match
		@param min_index index of the mask that matches
		@param abs_min value of the score for the mask that matches better 
		@param min_pos position in the image in which we found match
	*/
	void printBestMatch(BestResults best_results, cv::Mat mask_result);


	void findMax(cv::Mat result, BestResults &r, int img_index, int mask_index);


	void equalization(cv::Mat& img);

	void computeHist(std::vector<cv::Mat> imgs);

	double compareHistH(cv::Mat test_img, int view_num);
	/*
		Input images
		_input_imgs[0] = masks;
		_input_imgs[1] = views;
		_input_imgs[2] = test images;
	*/
	std::vector<std::vector<cv::Mat>> _input_imgs;
	//TEST Images with detected edges using Canny + distance transform
	std::vector<cv::Mat> _canny_imgs;
	//MASKS with detected edges using Canny + distance transform
	std::vector<cv::Mat> _filter_masks;
	//VIEWS with detected edges using Canny + distance transform
	std::vector<cv::Mat> _canny_views;
	//Histograms of views
	std::vector<cv::MatND> _hist_views;
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