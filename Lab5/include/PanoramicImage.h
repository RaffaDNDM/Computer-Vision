#ifndef PANORAMIC_IMAGE
#define PANORAMIC_IMAGE

#include "Lab5.h"
#include "PanoramicUtils.h"

/**
@brief Transform of images to panoramic image
*/
class PanoramicImage
{
public:

	PanoramicImage(cv::String path);

	/**
		@brief It create panoramic image from a set of images
		@param isORB true if you want to use ORB, false if you want to use SIFT
	*/
	void panoramicImage(bool isORB);

private:
	/**
		@brief Load input images
		@param path input images (PNG) folder
	*/
	void loadImages(cv::String path);

	/**
		@brief Find min distance between all decriptors matches and passed min
		@param min minimum value of distance in this moment
		@param d_matches actual vector of descriptors matches
	*/
	void findMin(float& min, std::vector<cv::DMatch> d_matches);

	//Array of names of images
	std::vector<cv::Mat> _input_imgs;

	//Array of projected images
	std::vector<cv::Mat> _projected_imgs;

	//Pattern to look for PNG images in folder 
	const std::vector<cv::String> patterns{ "*.bmp","*.png" };

	const double FOV = 66.0;

	const std::string window = "Panoramic View";

	float _ratio = 0.0;
};

#endif