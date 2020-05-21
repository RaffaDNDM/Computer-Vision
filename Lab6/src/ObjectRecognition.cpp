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

	for (auto& frame : _frames)
	{
		_detected_frame.push_back(frame.clone());
	}

	loadObjects(objects_path);
}

ObjectRecognition::ObjectRecognition(std::vector<cv::Mat> &frames, std::vector<cv::Mat>& objects, float ratio):
	_ratio{ ratio }
{
	_frames.reserve(frames.size());
	copy(frames.begin(), frames.end(), std::back_inserter(_frames));
	
	for (auto& frame : _frames)
	{
		_detected_frame.push_back(frame.clone());
	}

	_objects.reserve(objects.size());
	copy(objects.begin(), objects.end(), std::back_inserter(_objects));
}

void ObjectRecognition::recognition(bool isORB)
{
	cv::Ptr<cv::BFMatcher> matcher;

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

	//Compute translations
		//Compute thresholds and matches
	std::vector<std::vector<cv::DMatch>> matches;
	match(matcher, matches, _thresholds);
	computeMatches(matches, _thresholds, isORB);

	
	std::vector<cv::Point>;
	for(int i=1; i<_frames.size(); i++)
	{ 
		std::vector<uchar> status;
		std::vector<float> err;
		std::vector<cv::Point> inliers_frame_next;
		cv::calcOpticalFlowPyrLK(_frames[i-1], _frames[i], _inliers_frame_prev, inliers_frame_next, status, err);

		cv::Mat H = findHomography(_inliers_frame_prev, inliers_frame_next, cv::RANSAC);

		for(auto& corners: _corners_frame)
			cv::perspectiveTransform(corners, corners, H);

		for (int h = 0; h < _corners_frame[0].size(); h++)
			cv::line(_detected_frame[i], _corners_frame[i][h], _corners_frame[i][(h + 1) % 4], _colors[i], 2);

		for (int h = 0; h < inliers_frame_next.size(); h++)
			_inliers_frame_prev[h] = inliers_frame_next[h];
	}
}

void ObjectRecognition::loadVideo(cv::String video_path)
{
	cv::VideoCapture cap(video_path);

	_frame_rate = cap.get(cv::CAP_PROP_FPS);

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

void ObjectRecognition::key_desc(bool isORB)
{
	if (isORB)
	{
		cv::Ptr<cv::ORB> orb = cv::ORB::create();
		orb->setMaxFeatures(20000);
		orb->detectAndCompute(_frames[0], cv::Mat(), _keypointsFrame, _descriptorsFrame);
		orb->setMaxFeatures(5000);
	
		for (auto img : _objects)
		{
			std::vector < cv::KeyPoint> kpt;
			cv::Mat desc;
			orb->detectAndCompute(img, cv::Mat(), kpt, desc);
			_keypointsObjects.push_back(kpt);
			_descriptorsObjects.push_back(desc);
		}
	}
	else
	{
		cv::Ptr<cv::xfeatures2d::SIFT> sift = cv::xfeatures2d::SIFT::create();
		sift->detectAndCompute(_frames[0], cv::Mat(), _keypointsFrame, _descriptorsFrame);
	
		for (auto img : _objects)
		{
			std::vector < cv::KeyPoint> kpt;
			cv::Mat desc;
			sift->detectAndCompute(img, cv::Mat(), kpt, desc);
			_keypointsObjects.push_back(kpt);
			_descriptorsObjects.push_back(desc);
		}
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

		int k = 0;
		for (; k < mask.rows; k++)
		{
			if ((unsigned int)mask.at<uchar>(k))
			{
				cv::Point inlier(cvRound(pointsS[k].x), cvRound(pointsS[k].y));
				_inliers_frame_prev.push_back(inlier);
				cv::circle(_detected_frame[0], inlier, 2, _colors[i]);
			}
		}

		//Define corners of the object
		std::vector<cv::Point2f> corners_object;
		std::vector<cv::Point2f> corners;
		corners_object.push_back(cv::Point2f(_objects[i].cols - 1, 0));
		corners_object.push_back(cv::Point2f(_objects[i].cols - 1, _objects[i].rows - 1));
		corners_object.push_back(cv::Point2f(0, _objects[i].rows - 1));
		corners_object.push_back(cv::Point2f(0, 0));

		cv::perspectiveTransform(corners_object, corners, H);
		
		_corners_frame.push_back(corners);
		for (int h = 0; h < _corners_frame[0].size(); h++)
			cv::line(_detected_frame[0], _corners_frame[i][h], _corners_frame[i][(h+1)%4], _colors[i], 2);
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

std::vector<cv::Mat> ObjectRecognition::getDetectedFrame()
{
	return _detected_frame;
}

double ObjectRecognition::getFrameRate()
{
	return _frame_rate;
}

int ObjectRecognition::getNumFrames()
{
	return _frames.size();
}

void ObjectRecognition::printInfo()
{
	std::cout << LINE << "[" << _method_name << "] Thresholds\n" << LINE;

	for (int i = 0; i < _thresholds.size(); i++)
		std::cout << i << ":  " << _thresholds[i] << std::endl;

	std::cout << LINE << "[" << _method_name << "] Translations\n" << LINE;

	std::cout << LINE;
}