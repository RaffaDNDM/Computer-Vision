/**
	@file VideoMatching.h
	@brief Class for creation of panoramic view.
	@author Di Nardo Di Maio Raffaele 1204879
	@author Bonato Dario 
*/

#ifndef PANORAMIC_IMAGE
#define PANORAMIC_IMAGE

#include "Lab6.h"

/**
@brief Transform images to panoramic image.
*/
class ObjectRecognition
{
public:
	/**
		@brief Constructor.
		@param path path of the folder with images 
		@param ratio ratio for threshold
	*/
	ObjectRecognition(cv::String video_path, cv::String objects_path, float ratio);
	
	/**
		@brief Constructor.
		@param projected_imgs vector of input projected 
		@param ratio ratio for threshold
	*/
	ObjectRecognition(std::vector<cv::Mat>& frames, std::vector<cv::Mat>& objects, float ratio);

	/**
		@brief It create panoramic image from a set of images.
		@param isORB true if you want to use ORB, false if you want to use SIFT
	*/
	void recognition(bool isORB);

	/**
		@brief Get projected images.
		@return vector of projected images
	*/
	std::vector<cv::Mat> getFrames();

	/**
		@brief Get projected images.
		@return vector of projected images
	*/
	std::vector<cv::Mat> getObjects();

	/**
		@brief Get panoramic view image.
		@return panoramic view image 
	*/
	cv::Mat getResult();

	/**
		Print useful informations
	*/
	void printInfo();

private:

	/**
		@brief Load video
		@param path input images (PNG) folder
	*/
	void loadVideo(cv::String path);

	/**
		@brief Load objects images
		@param path input images (PNG) folder
	*/
	void loadObjects(cv::String path);

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
	void key_desc(cv::Ptr<T>& method, 
		          cv::Mat frame, 
		          std::vector<cv::KeyPoint>& keypoints, 
		          std::vector<cv::Mat>& descriptors);

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
	void computeMatches(std::vector<std::vector<cv::DMatch>> matches,
		                std::vector<float> thresholds);

	//Descriptors vector
	std::vector<cv::Mat> _frames;

	//Descriptors vector
	std::vector<cv::Mat> _objects;

	//Keypoints vector
	std::vector<cv::KeyPoint> _keypointsFrame;

	//Descriptors vector
	std::vector< std::vector<cv::KeyPoint>> _keypointsObjects;

	//Descriptors vector
	std::vector<cv::Mat> _descriptorsFrame;

	//Descriptors vector
	std::vector< std::vector<cv::Mat>> _descriptorsObjects;

	//Computed thresholds
	std::vector<float> _thresholds;

	//First frame with detected objects
	cv::Mat _detected_frame;

	//Pattern to look for video in the folder 
	const cv::String video_pattern = "*.mov";

	//Pattern to look for PNG image in the folder 
	const cv::String objects_pattern = "*.png";

	//Ratio parameter used to compute the threshold
	float _ratio = 0.0;

	//Name of the method selected
	std::string _method_name;
};

#endif