/**
	@file PanoramicImage.cpp
	@brief Implementation of the class used for panoramic view.
	@author Di Nardo Di Maio Raffaele 1204879
*/

#include "ObjectRecognition.h"

ObjectRecognition::ObjectRecognition(cv::String video_path, cv::String objects_path, float ratio):
	_ratio{ratio},
	_cap{ cv::VideoCapture(video_path) },
	_frame_rate{ _cap.get(cv::CAP_PROP_FPS) }
{
	if (!_cap.isOpened())
	{
		throw "Video file can't be opened";
	}

	loadObjects(objects_path);
}

void ObjectRecognition::recognition(bool isORB)
{
	cv::Ptr<cv::BFMatcher> matcher;

	_cap >> _frame;
	_detected_frames.push_back(_frame.clone());
	key_desc(isORB);

	if (isORB)
	{
		_method_name = "ORB";
		matcher = cv::BFMatcher::create(cv::NORM_HAMMING, true);
	}
	else
	{
		_method_name = "SIFT";
		matcher = cv::BFMatcher::create(cv::NORM_L2, true);
	}

	//Compute thresholds and matches
	std::vector<std::vector<cv::DMatch>> matches;
	match(matcher, matches, _thresholds);
	computeMatches(matches, _thresholds, isORB);

	bool check = false;
	int i = 0;

	while(!check)
	{
		_cap >> _frame_next;

		if (_frame_next.empty())
		{
			check = true;
			continue;
		}

		i++;
		_detected_frames.push_back(_frame_next.clone());

		for(int h=0; h<_inliers_frame_prev.size(); h++)
		{
			std::vector<cv::Point2f> inliers_frame_next;
			std::vector<cv::Point2f> corners_frame_next;
			std::vector<uchar> status;
			std::vector<float> err;

			cv::calcOpticalFlowPyrLK(_frame, _frame_next, _inliers_frame_prev[h], inliers_frame_next, status, err);

			_inliers_frame_prev[h].clear();
			for (int k = 0; k < status.size(); k++)
			{
				if (status[k] == 1)
				{
					_inliers_frame_prev[h].push_back(inliers_frame_next[k]);
					cv::circle(_detected_frames[i], inliers_frame_next[k], 2, _colors[h]);
				}
			}

			cv::calcOpticalFlowPyrLK(_frame, _frame_next, _corners_frame_prev[h], corners_frame_next, status, err);

			for (int k = 0; k < status.size(); k++)
			{
				if (status[k] == 1)
				{
					_corners_frame_prev[h][k] = corners_frame_next[k];
				}
			}

			for(int k=0; k<_corners_frame_prev[h].size(); k++)
				cv::line(_detected_frames[i], _corners_frame_prev[h][k], _corners_frame_prev[h][(k + 1) % 4], _colors[h], 2);
		}
	
		_frame = _frame_next.clone();
	}
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

void ObjectRecognition::key_desc(bool isORB)
{
	cv::Ptr<cv::xfeatures2d::SIFT> sift = cv::xfeatures2d::SIFT::create();
	sift->detectAndCompute(_frame, cv::Mat(), _keypointsFrame, _descriptorsFrame);
	
	for (auto img : _objects)
	{
		std::vector < cv::KeyPoint> kpt;
		cv::Mat desc;
		sift->detectAndCompute(img, cv::Mat(), kpt, desc);
		_keypointsObjects.push_back(kpt);
		_descriptorsObjects.push_back(desc);
	}
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
                                       std::vector<float> thresholds, bool isORB)
{
	for (int i = 0; i < matches.size(); i++)
	{
		std::vector<cv::Point2f> pointsO;
		std::vector<cv::Point2f> pointsS;
		cv::Mat mask;

		for (int j = 0; j < matches[i].size(); j++)
		{
			if (matches[i][j].distance <= thresholds[i])
			{
				//indice del keypoint dell'immagine i-1 
				pointsO.push_back(_keypointsObjects[i][matches[i][j].queryIdx].pt);

				//indice del keypoint dell'immagine i
				pointsS.push_back(_keypointsFrame[matches[i][j].trainIdx].pt);

			}
		}

		cv::Mat H;

		if(isORB)
			H = cv::findHomography(pointsO, pointsS, mask, cv::RANSAC, thresholds[i]);
		else
			H = cv::findHomography(pointsO, pointsS, mask, cv::RANSAC);

		cv::Point2f transl(0.0, 0.0);
		std::vector<cv::Point2f> inliers;
		int k = 0;
		for (; k < mask.rows; k++)
		{
			if ((unsigned int)mask.at<uchar>(k))
			{

				cv::Point2f inlier(cvRound(pointsS[k].x), cvRound(pointsS[k].y));
				inliers.push_back(inlier);

				cv::circle(_detected_frames[0], inlier, 2, _colors[i]);
			}
		}
		_inliers_frame_prev.push_back(inliers);

		//Define corners of the object
		std::vector<cv::Point2f> corners_object;
		std::vector<cv::Point2f> corners;
		corners_object.push_back(cv::Point2f(_objects[i].cols - 1, 0));
		corners_object.push_back(cv::Point2f(_objects[i].cols - 1, _objects[i].rows - 1));
		corners_object.push_back(cv::Point2f(0, _objects[i].rows - 1));
		corners_object.push_back(cv::Point2f(0, 0));

		cv::perspectiveTransform(corners_object, corners, H);
		
		_corners_frame_prev.push_back(corners);
		for (int h = 0; h < _corners_frame_prev[0].size(); h++)
			cv::line(_detected_frames[0], _corners_frame_prev[i][h], _corners_frame_prev[i][(h+1)%4], _colors[i], 2);
	}
}

std::vector<cv::Mat> ObjectRecognition::getObjects()
{
	return _objects;
}

std::vector<cv::Mat> ObjectRecognition::getDetectedFrame()
{
	return _detected_frames;
}

double ObjectRecognition::getFrameRate()
{
	return _frame_rate;
}

int ObjectRecognition::getNumFrames()
{
	return _detected_frames.size();
}

void ObjectRecognition::printInfo()
{
	std::cout << LINE << "[" << _method_name << "] Thresholds\n" << LINE;

	for (int i = 0; i < _thresholds.size(); i++)
		std::cout << i << ":  " << _thresholds[i] << std::endl;

	std::cout << LINE << "[" << _method_name << "] Translations\n" << LINE;

	std::cout << LINE;
}