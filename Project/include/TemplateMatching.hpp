/**
	@file TemplateMatching.hpp
	@brief Class for template matching.
	@author Di Nardo Di Maio Raffaele 1204879
*/

#ifndef TEMPLATE_MATCHING
#define TEMPLATE_MATCHING

#include "Project.hpp"
#include "Utility.hpp"
#include "BestResults.hpp"
#include "GammaTransform.hpp"

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
	TemplateMatching(cv::String path, Utility::Type dataset_type);

	/**
		@brief Compute template matching.
	*/
	void match();

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
	void printBestMatch(BestResults best_results, cv::Mat img);

	/**
		@brief Equalize an image.
		@param img image to be equalized
	*/
	void equalization(cv::Mat& img);

	/**
		@brief Compute histograms of a view images.
		@param imgs input set of view images
	*/
	void computeHistViews(std::vector<cv::Mat> imgs);

	/**
		@brief Compute HUE histogram of an image
		@param img input image
	*/
	cv::MatND computeHist(cv::Mat img);

	/**
		@brief Compare histograms of two images.
		@param tes_img first image to be compared
		@param view_num index of view image in the buffer to be compared
	*/
	double compareHistH(cv::Mat test_img, int view_num);

	/**
		@brief Detection of edges from test images.
	*/
	void cannyDetection();

	/**
		@brief Compute template matching on Canny image.
		@param canny_img image with edges detected through Canny detector
		@param img_index index of the image on which compute template matching
		@param r buffer of best results
	*/
	void match(cv::Mat canny_img, int img_index, BestResults& r);

	/**
		@brief Refinement of results obtained in r1 using histogram comparison.
		@param img image on which we are refining the results
		@param r1 oldest buffer of results
		@param r2 improved buffer of results
	*/
	void refinement(cv::Mat img, BestResults& r1, BestResults& r2);

	/*   Input images
		_input_imgs[0] = masks;
		_input_imgs[1] = views;
		_input_imgs[2] = test images;
	*/
	std::vector<std::vector<cv::Mat>> _input_imgs;	

	//TEST Images with detected edges using Canny + distance transform
	std::vector<cv::Mat> _canny_imgs;
	
	//MASKS with detected edges using  Canny + distance transform
	std::vector<cv::Mat> _filter_masks; 

	//VIEWS with detected edges using  Canny + distance transform
	std::vector<cv::Mat> _canny_views;

	// Histograms of views
	std::vector<cv::MatND> _hist_views;

	//Type of dataset, we are analysing (can, driller or duck)
	Utility::Type _dataset_type;

	//File stream for output
	std::fstream _fs;

	//Parameter used by canny detection
	Utility::Parameter _param;
};

/**
	@brief Exception thrown if the images are not found in specified folder.
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