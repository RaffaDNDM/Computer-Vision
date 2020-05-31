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
	@brief Transform images to panoramic image.
*/
class PanoramicImage
{
public:
	/**
		@brief Constructor.
		@param path path of the folder with images 
		@param ratio ratio for threshold
		@param FOV field of view
		@throws InputIMGException for not found bmp png images in input folder 
	*/
	PanoramicImage(cv::String path, float ratio, double FOV);
	
	/**
		@brief Constructor.
		@param projected_imgs vector of input projected 
		@param ratio ratio for threshold
		@param FOV field of view
	*/
	PanoramicImage(std::vector<cv::Mat> &projected_imgs, float ratio, double FOV);

	/**
		@brief It create panoramic image from a set of images.
		@param isORB true if you want to use ORB, false if you want to use SIFT
	*/
	void panoramicImage(bool isORB);

	/**
		@brief Get projected images.
		@return vector of projected images
	*/
	std::vector<cv::Mat> getProjected();

	/**
		@brief Get panoramic view image.
		@return panoramic view image 
	*/
	cv::Mat getResult();

	/**
		@brief Print useful informations.
	*/
	void printInfo();

	/**
		@brief Set ratio.
		@param ratio new ratio
	*/
	void setRatio(float ratio);

	/*
		@brief Clear computed keypoints, descriptors, thresholds, translations.
	*/
	void clearUsedVectors();
	
	/**
		@brief Get method name.
		@return method name
	*/
	std::string getMethod();

private:

	/**
		@brief Load input images.
		@param path folder of input images (PNG)
		@throws InputIMGException if no images found with png, bmp extensions
	*/
	void loadImages(cv::String path);

	/**
		@brief Find min distance between all descriptors matches.
		@param d_matches actual vector of descriptors matches
		@return min distance among all the matches
	*/
	float findMin(std::vector<cv::DMatch> d_matches);


	template <class T>
	/**
		@brief Compute keypoints and descriptors.
		@param method pointer to method object
	*/
	void key_desc(cv::Ptr<T>& method);

	/**
		@brief Compute matches and thresholds looking to descriptors.
		@param matches all the matches found between descriptors
		@param thresholds all the threshold (ratio*min), each one related to a vector of @matches 
	*/
	void match(cv::Ptr<cv::BFMatcher> matcher, std::vector<std::vector<cv::DMatch>>& matches, std::vector<float>& thresholds);

	/**
		@brief Compute all the mean translations.
		@param translations vector where there will be stores translations, represented as points in 2D-space
		@param matches all the matches found between descriptors
		@param thresholds all the threshold (ratio*min), each one related to a vector of @matches
	*/
	void translate_imgs(std::vector<cv::Point2f>& translations,
		                std::vector<std::vector<cv::DMatch>> matches,
		                std::vector<float> thresholds);

	/**
		@brief Compute all the mean translations and final panoramic view image.
		@param translations vector where there will be store translations, represented as points in 2D-space
	*/
	void final_image(std::vector<cv::Point2f>& translations);

	/**
		@brief Compute cylindrical projection of all input images.
	*/
	void cylinderProjection();

	//Array of names of images
	std::vector<cv::Mat> _input_imgs;

	//Array of projected images
	std::vector<cv::Mat> _projected_imgs;

	//Keypoints vector
	std::vector<std::vector<cv::KeyPoint>> _keypoints;
	
	//Descriptors vector
	std::vector<cv::Mat> _descriptors;

	//Result panoramic image
	cv::Mat _panoramic_view;

	//Pattern to look for PNG images in folder 
	const std::vector<cv::String> _patterns{ "*.bmp","*.png" };

	//Field of view used in the set of images
	double _FOV = 0.0;

	//Ratio parameter used to compute the threshold
	float _ratio = 0.0;

	//Name of the method selected
	std::string _method_name;

	//Computed thresholds
	std::vector<float> _thresholds;
	
	//Computed translations
	std::vector<cv::Point2f> _translations;
};

/*
	@brief Thrown if no images found in folder with extension png, bmp.
*/
class InputIMGException : public std::exception
{
public:
	virtual char const* what() const 
	{ 
		return "No images found with bmp or png extension in <input_folder>."; 
	}
};

/*
	@brief Thrown if no inliers found through findHomography.
*/
class NoInliersException : public std::exception
{
public:
	virtual char const* what() const
	{
		return "No inliers found, increase the ratio.";
	}
};

#endif