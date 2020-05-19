/**
	@file PanoramicImage.h
	@brief Class for creation of panoramic view.
	@author Di Nardo Di Maio Raffaele 1204879
*/

#ifndef PANORAMIC_IMAGE
#define PANORAMIC_IMAGE

#include "Lab5.h"
#include "PanoramicUtils.h"

/**
@brief Transform images to panoramic image
*/
class PanoramicImage
{
public:

	PanoramicImage(cv::String path, float ratio);

	PanoramicImage(std::vector<cv::Mat> &projected_imgs, float ratio);

	/**
		@brief It create panoramic image from a set of images
		@param isORB true if you want to use ORB, false if you want to use SIFT
	*/
	void panoramicImage(bool isORB);

	std::vector<cv::Mat> getProjected();

	cv::Mat getResult();

private:

	/**
		@brief Load input images
		@param path input images (PNG) folder
	*/
	void loadImages(cv::String path);

	/**
		@brief Find min distance between all decriptors matches and passed min
		@param d_matches actual vector of descriptors matches
		@return min distance among all the matches
	*/
	float findMin(std::vector<cv::DMatch> d_matches);


	template <class T>
	/**
		@brief Compute keypoints and descriptors
		@param method pointer to method object ()
	*/
	void key_desc(cv::Ptr<T>& method);

	/**
		@brief Compute matches and thresholds looking to descriptors
		@param matches all the matches found between descriptors
		@param thresholds all the threshold (ratio*min), each one related to a vector of @matches 
	*/
	void match(cv::Ptr<cv::BFMatcher> matcher, std::vector<std::vector<cv::DMatch>>& matches, std::vector<float>& thresholds);

	/**
		@brief Compute all the mean translations 
		@param translations vector where there will be store translations, represented as points in 2D-space
		@param matches all the matches found between descriptors
		@param thresholds all the threshold (ratio*min), each one related to a vector of @matches
	*/
	void translate_imgs(std::vector<cv::Point2f>& translations,
		                std::vector<std::vector<cv::DMatch>> matches,
		                std::vector<float> thresholds);

	/**
		@brief Compute all the mean translations and final panoramic view image
		@param translations vector where there will be store translations, represented as points in 2D-space
	*/
	void final_image(std::vector<cv::Point2f>& translations);

	/**
		@brief Compute cylindrical projection of all input images
	*/
	void cylinderProjection();

	//Array of names of images
	std::vector<cv::Mat> _input_imgs;

	//Array of projected images
	std::vector<cv::Mat> _projected_imgs;

	//Keypoints vector
	std::vector<std::vector<cv::KeyPoint>> keypoints;
	
	//Descriptors vector
	std::vector<cv::Mat> descriptors;

	//Result panoramic image
	cv::Mat _panoramic_view;

	//Pattern to look for PNG images in folder 
	const std::vector<cv::String> patterns{ "*.bmp","*.png" };

	//Field of view used in the set of images
	const double FOV = 66.0;

	//Ratio parameter used to compute the threshold
	float _ratio = 0.0;
};

#endif