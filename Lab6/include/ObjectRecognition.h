/**
	@file ObjectRecognition.h
	@brief Class for objection recognition in videos.
	@author Di Nardo Di Maio Raffaele
*/

#ifndef OBJECT_RECOGNITION
#define OBJECT_RECOGNITION

#include "Lab6.h"

/**
@brief Detect video and detect objects.
*/
class ObjectRecognition
{
public:

	ObjectRecognition();
	/**
		@brief Constructor.
		@param video_path complete pathname of the video
		@param objects_path path of the folder with png images of the objects
		@param ratio ratio for threshold
	*/
	ObjectRecognition(cv::String video_path, cv::String objects_path, float ratio);

	/**
		@brief Object recognition and traking among all video frames.
	*/
	void recognition(bool save);

	/**
		@brief Print useful informations.
	*/
	void printInfo();

	/**
		@brief Frame rate of input video.
		@return frame rate
	*/
	double getFrameRate();

	/**
		@brief Number of frame in input video.
		@return number of frames
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
		@brief Compute keypoints and descriptors of objects and first frame.
	*/
	void key_desc();

	/**
		@brief Compute matches and thresholds looking to descriptors
		@param matcher matcher we use to compute matches
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

	//Vector of object images
	std::vector<cv::Mat> _objects;

	//Keypoints vector of first frame
	std::vector<cv::KeyPoint> _keypointsFrame;

	//Keypoints vector (one for each object image)
	std::vector< std::vector<cv::KeyPoint>> _keypointsObjects;

	//Descriptors vector of first frame
	cv::Mat _descriptorsFrame;

	//Descriptors vector of objects 
	std::vector<cv::Mat> _descriptorsObjects;

	//Computed thresholds
	std::vector<float> _thresholds;

	//Current frame with detected objects
	cv::Mat _detected_frame;

	//Pattern to look for PNG image in the folder 
	const cv::String _objects_pattern = "*.png";

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

	const cv::Scalar _marker_color = cv::Scalar(255, 255, 255);

	//Filename of output detected video
	const cv::String _output_filename = "output.avi";

	//Inlier points on the frames
	std::vector <std::vector<cv::Point2f>> _inliers_frame_prev;

	//Corners of objects 
	std::vector<std::vector<cv::Point2f>> _corners_frame_prev;

	//Frame rate of the camera
	double _frame_rate = 0.0;
};

class VideoException : public std::exception
{
public:
	virtual char const* what() const
	{
		return "Video file can't be opened (Check if you insert the correct path)";
	}
};

class InputIMGException : public std::exception
{
public:
	virtual char const* what() const
	{
		return "No images found with bmp or png extension in <objects_folder>.";
	}
};

class NoFirstFrameException : public std::exception
{
public:
	virtual char const* what() const
	{
		return "Empty first frame";
	}
};

class NoTrackedPointsException : public std::exception
{
public:
	virtual char const* what() const
	{

		return "Lucas-Kanade wasn't able to track any points.";
	}
};

class NoInliersException : public std::exception
{
public:
	virtual char const* what() const
	{
		return "No inliers found, increase the ratio.";
	}
};

#endif