/**
	@file PanoramicImage.cpp
	@brief Implementation of the class used for panoramic view.
	@author Di Nardo Di Maio Raffaele 1204879
*/

#include "ObjectRecognition.h"

ObjectRecognition::ObjectRecognition(cv::String video_path, cv::String objects_path, float ratio):
	_ratio{ratio}
{
	loadVideo(video_path);
	loadObjects(objects_path);
}

ObjectRecognition::ObjectRecognition(std::vector<cv::Mat> &frames, std::vector<cv::Mat>& objects, float ratio):
	_ratio{ ratio }
{
	_frames.reserve(frames.size());
	copy(frames.begin(), frames.end(), std::back_inserter(_frames));
	
	_objects.reserve(objects.size());
	copy(objects.begin(), objects.end(), std::back_inserter(_objects));
}

void ObjectRecognition::recognition(bool isORB)
{
	cv::Ptr<cv::BFMatcher> matcher;

	if (isORB)
	{
		_method_name = "ORB";
		cv::Ptr<cv::ORB> orb;
		key_desc<cv::ORB>(orb, _frames[0], _keypointsFrame, _descriptorsFrame);

		int i = 0;
		for (auto img : _objects)
		{
			key_desc<cv::ORB>(orb, img, _keypointsObjects[i++], _descriptorsObjects[i++]);
		}
		matcher = cv::BFMatcher::create(cv::NORM_HAMMING, true);
	}
	else
	{
		_method_name = "SIFT";
		cv::Ptr<cv::xfeatures2d::SIFT> sift;		
		key_desc<cv::xfeatures2d::SIFT>(sift, _frames[0], _keypointsFrame, _descriptorsFrame);

		int i = 0;
		for (auto img : _objects)
		{
			key_desc<cv::xfeatures2d::SIFT>(sift, img, _keypointsObjects[i++], _descriptorsObjects[i++]);
		}

		matcher = cv::BFMatcher::create(cv::NORM_L2, true);
	}
	
	//Compute thresholds and matches
	std::vector<std::vector<cv::DMatch>> matches;
	match(matcher, matches, _thresholds);

	//Compute translations
	computeMatches(matches, _thresholds);
}

void ObjectRecognition::loadVideo(cv::String video_path)
{
	cv::VideoCapture cap(video_path);

	if (cap.isOpened())
	{
		for (;;)
		{
			cv::Mat frame; 
			cap >> frame;

			if (frame.empty())
				break;

			_frames.push_back(frame);
		}
	}

	std::cout << LINE << "Number of video frames:   " << _frames.size() << "\n" << LINE;
}


void ObjectRecognition::loadObjects(cv::String path)
{
	std::vector<cv::String> imgs_names;
	cv::utils::fs::glob(path, objects_pattern, imgs_names);

	if (imgs_names.size() == 0)
	{
		std::cout << LINE << "Error in the video path" << LINE;
		exit(1);
	}
	else
	{
		for (auto name : imgs_names)
		{
			_objects.push_back(imread(name, cv::IMREAD_COLOR));
		}

		std::cout << LINE << "Number of objects:   " << _objects.size() << "\n" << LINE;
	}
}

float ObjectRecognition::findMin(std::vector<cv::DMatch> d_matches)
{
	float min = std::numeric_limits<float>::max();

	for (cv::DMatch match : d_matches)
	{
		if (match.distance < min)
			min = match.distance;
	}

	return min;
}

template <class T>
void ObjectRecognition::key_desc(cv::Ptr<T>& method, cv::Mat frame, 
	std::vector<cv::KeyPoint> &keypoints,
	std::vector<cv::Mat>& descriptors)
{
	method = T::create();

	std::vector<cv::KeyPoint> kpts;
	cv::Mat desc;

	method->detectAndCompute(frame, cv::Mat(), kpts, desc);

	descriptors.push_back(desc);
	keypoints.push_back(kpts);
}

void ObjectRecognition::match(cv::Ptr<cv::BFMatcher> matcher,
						   std::vector<std::vector<cv::DMatch>>& matches, 
	                       std::vector<float>& thresholds)
{
	for (auto descriptor : _descriptorsObjects)
	{
		std::vector<cv::DMatch> match;
		matcher->match(descriptor, _descriptorsFrame, match);
		float min = findMin(match);
		matches.push_back(match);
		thresholds.push_back(_ratio * min);
	}

	/**
		cv::Mat res;
		cv::drawMatches(img1, kpts1, img2, kpts2, matches, res, Scalar::all(-1), Scalar::all(-1), match_mask, DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	*/
	std::cout << LINE;
}

void ObjectRecognition::computeMatches(std::vector<std::vector<cv::DMatch>> matches,
                                       std::vector<float> thresholds)
{
	for (int i = 0; i < matches.size(); i++)
	{
		/*
		std::vector<cv::Point2f> pointsO;
		std::vector<cv::Point2f> pointsS;
		cv::Mat mask;

		for (int j = 0; j < matches[i].size(); j++)
		{
			if (matches[i][j].distance <= thresholds[i])
			{
				//indice del keypoint dell'immagine i-1 
				pointsO.push_back(_keypoints[i][matches[i][j].queryIdx].pt);

				//indice del keypoint dell'immagine i
				pointsS.push_back(_keypoints[i + 1][matches[i][j].trainIdx].pt);

			}
		}
		*/

		cv::Mat H = cv::findHomography(pointsO, pointsS, mask, cv::RANSAC);

		std::vector <cv::Point2f> inliers_frame;
		std::vector <cv::Point2f> inliers_object;

		cv::Point2f transl(0.0, 0.0);

		int k = 0;
		for (; k < mask.rows; k++)
		{
			if ((unsigned int)mask.at<uchar>(k))
			{
				inliers_frame.push_back(pointsS[k]);
			}
		}
	}
}

std::vector<cv::Mat> ObjectRecognition::getFrames()
{
	return _frames;
}

std::vector<cv::Mat> ObjectRecognition::getObjects()
{
	return _objects;
}

cv::Mat ObjectRecognition::getResult()
{
	return _detected_frame;
}

void ObjectRecognition::printInfo()
{
	std::cout << LINE << "[" << _method_name << "] Thresholds\n" << LINE;

	for (int i = 0; i < _thresholds.size(); i++)
		std::cout << i << ":  " << _thresholds[i] << std::endl;

	std::cout << LINE << "[" << _method_name << "] Translations\n" << LINE;

	std::cout << LINE;
}