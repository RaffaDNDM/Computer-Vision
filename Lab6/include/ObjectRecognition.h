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
@brief Detect video and detect objects.
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
	std::vector<cv::Mat> getObjects();

	/**
		@brief Get panoramic view image.
		@return panoramic view image 
	*/
	std::vector<cv::Mat> getDetectedFrame();

	/**
		@brief Print useful informations.
	*/
	void printInfo();

	/**
		@brief.
	*/
	double getFrameRate();

	/**
		@brief.
	*/
	int getNumFrames();
private:

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

	/**
		@brief Compute keypoints and descriptors
		@param method pointer to method object ()
	*/
	void key_desc(bool isORB);

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
		                std::vector<float> thresholds,
						bool isORB);

	//Video capture to detect frames
	cv::VideoCapture _cap;

	//Video writer to write final video
	cv::VideoWriter _out;

	//Actual frame in the video
	cv::Mat _frame;

	//Actual frame in the video
	cv::Mat _frame_next;

	//Number of frames
	int _size = 0;

	//Descriptors vector
	std::vector<cv::Mat> _objects;

	//Keypoints vector
	std::vector<cv::KeyPoint> _keypointsFrame;

	//Descriptors vector
	std::vector< std::vector<cv::KeyPoint>> _keypointsObjects;

	//Descriptors vector
	cv::Mat _descriptorsFrame;

	//Descriptors vector
	std::vector<cv::Mat> _descriptorsObjects;

	//Computed thresholds
	std::vector<float> _thresholds;

	//Frame with detected objects
	cv::Mat _detected_frame;

	//Pattern to look for video in the folder 
	const cv::String video_pattern = "*.mov";

	//Pattern to look for PNG image in the folder 
	const cv::String objects_pattern = "*.png";

	//Ratio parameter used to compute the threshold
	float _ratio = 0.0;

	//Name of the method selected
	std::string _window_name = "SIFT";

	//Used colors to draw keypoints 
	const std::vector<cv::Scalar> _colors = { cv::Scalar(255,0,0),//blue
										  	  cv::Scalar(0,255,0),//green
											  cv::Scalar(0,0,255),//red
											  cv::Scalar(0,255,255)//yellow
											 };

	//Filename of output detected video
	const cv::String _output_filename = "output.avi";

	//Inlier points on the frames
	std::vector <std::vector<cv::Point2f>> _inliers_frame_prev;

	//Corners of objects 
	std::vector<std::vector<cv::Point2f>> _corners_frame_prev;

	//Frame rate of the camera
	double _frame_rate = 0.0;
};

#endif