/**
	@file ObjectRecognition.cpp
	@brief Implementation of the class used for object recognition in videos.
	@author Di Nardo Di Maio Raffaele
*/

#include "ObjectRecognition.h"

ObjectRecognition::ObjectRecognition() 
{}

ObjectRecognition::ObjectRecognition(cv::String video_path, cv::String objects_path, float ratio):
	_ratio{ratio},
	_cap{ cv::VideoCapture(video_path) },
	_frame_rate{ _cap.get(cv::CAP_PROP_FPS) },
	_size{ static_cast<int>(_cap.get(cv::CAP_PROP_FRAME_COUNT)) }
{
	if (!_cap.isOpened())
	{
		throw VideoException();
	}

	//Loading images of the objects
	loadObjects(objects_path);
}

void ObjectRecognition::recognition()
{
	cv::Ptr<cv::BFMatcher> matcher;

	std::cout << " Keypoints detection on first frame" << std::endl;

	//Loading first frame
	_cap >> _frame;
	cv::resize(_frame, _frame, cv::Size(_frame.cols / 2, _frame.rows / 2));
	
	if (_frame.empty())
	{
		throw NoFirstFrameException();
		exit(1);
	}

	_detected_frame=_frame.clone();
	
	//SIFT detection
	key_desc();
	matcher = cv::BFMatcher::create(cv::NORM_L2, true);
	
	//Compute thresholds and matches
	std::vector<std::vector<cv::DMatch>> matches;
	match(matcher, matches, _thresholds);
	computeMatches(matches, _thresholds);

	//Showing first frame
	cv::namedWindow(_window_name, cv::WINDOW_AUTOSIZE);
	cv::imshow(_window_name, _detected_frame);

	std::cout << LINE << "Tracking phase on other frames" << std::endl;

	/*
		Store detected video on the disk
		_out=cv::VideoWriter(_output_filename, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), _frame_rate, _frame.size());
	*/
	bool check = false;
	int i = 1;
	int upload_step = _size / 10;

	while(!check)
	{
		//Loading next frame
		_cap >> _frame_next;

		if (_frame_next.empty())
		{
			//Video is ended
			check = true;
			continue;
		}

		cv::resize(_frame_next, _frame_next, cv::Size(_frame_next.cols / 2, _frame_next.rows / 2));
		i++;
		_detected_frame =_frame_next.clone();

		//Tracking (one for each object)
		for(int h=0; h<_inliers_frame_prev.size(); h++)
		{
			std::vector<cv::Point2f> inliers_frame_next;
			std::vector<uchar> status;
			std::vector<float> err;

			//Lukas-Kanade algorithm
			cv::calcOpticalFlowPyrLK(_frame, _frame_next, _inliers_frame_prev[h], inliers_frame_next, status, err);
			std::vector<cv::Point2f> inliers_prev_ok;
			std::vector<cv::Point2f> inliers_next_ok;

			for (int k = 0; k < status.size(); k++)
			{
				if (status[k] == 1) //If the point k is tracked
				{
					inliers_prev_ok.push_back(_inliers_frame_prev[h][k]);
					inliers_next_ok.push_back(inliers_frame_next[k]);

					//Draw a circle for the new tracked positions of the points
					cv::circle(_detected_frame, inliers_frame_next[k], 2, _colors[h]);
				}
			}

			//No tracked points by Lukas-Kanade
			if (inliers_prev_ok.size() == 0)
				throw NoTrackedPointsException();

			//Computation of perspective transformation between previous and next frame, looking to tracked point by Lukas-Kanade
			cv::Mat mask;
			cv::Mat H = cv::findHomography(inliers_prev_ok, inliers_next_ok, mask, cv::RANSAC);

			//Reprojection of corners of the object into the new frame
			std::vector<cv::Point2f> corners;
			cv::perspectiveTransform(_corners_frame_prev[h], corners, H);
			_corners_frame_prev[h] = corners;

			_inliers_frame_prev[h] = inliers_next_ok;

			//Draw lines and markers, looking to projected corners of the object
			for (int k = 0; k < _corners_frame_prev[0].size(); k++)
			{
				cv::line(_detected_frame, _corners_frame_prev[h][k], _corners_frame_prev[h][(k + 1) % 4], _colors[h], 2, cv::LineTypes::LINE_AA);
				cv::drawMarker(_detected_frame, _corners_frame_prev[h][k], _marker_color, cv::MARKER_CROSS);
			}
		}
	
		_frame = _frame_next.clone();
		
		cv::imshow(_window_name, _detected_frame);
		cv::waitKey(1);
		
		/*
			Store the final image on disk
			_out << _detected_frame;
		*/

		//Tracking bar with percentage of frame already tracked
		if (i % upload_step == 0)
		{
			int k = 0;
			int percentage_done = i / upload_step;
			
			std::cout << "\r[";

			for (; k < percentage_done; k++)
				std::cout << "=";
			for(;k<10; k++)
				std::cout << " ";
			std::cout << "] " << percentage_done*10 << "%";
		}
	}
	std::cout << LINE;
}

void ObjectRecognition::loadObjects(cv::String path)
{
	std::vector<cv::String> imgs_names;
	cv::utils::fs::glob(path, _objects_pattern, imgs_names);

	if (imgs_names.size() == 0) //No frames found with png extension
		throw InputIMGException();
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

void ObjectRecognition::key_desc()
{
	cv::Ptr<cv::xfeatures2d::SIFT> sift = cv::xfeatures2d::SIFT::create();
	
	//Computation of keypoints and descriptors of first frame
	sift->detectAndCompute(_frame, cv::Mat(), _keypointsFrame, _descriptorsFrame);
	
	//Computation of keypoints and descriptors of objects
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
	//Compute matches and thresholds, looking to minima and ratio
	for (auto descriptor : _descriptorsObjects)
	{
		std::vector<cv::DMatch> match;
		matcher->match(descriptor, _descriptorsFrame, match);
		float min = findMin(match);
		matches.push_back(match);
		thresholds.push_back(_ratio * min);
	}

	printInfo();
}

void ObjectRecognition::computeMatches(std::vector<std::vector<cv::DMatch>> matches,
                                       std::vector<float> thresholds)
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
				pointsO.push_back(_keypointsObjects[i][matches[i][j].queryIdx].pt);

				pointsS.push_back(_keypointsFrame[matches[i][j].trainIdx].pt);

			}
		}


		cv::Mat H;
		H = cv::findHomography(pointsO, pointsS, mask, cv::RANSAC);

		//Computation of inliers
		std::vector<cv::Point2f> inliers;
		int count = 0;
		int k = 0;
		for (; k < mask.rows; k++)
		{
			if ((unsigned int)mask.at<uchar>(k))
			{
				count++;
				cv::Point2f inlier(cvRound(pointsS[k].x), cvRound(pointsS[k].y));
				inliers.push_back(inlier);

				//Draw a circle for each found inlier
				cv::circle(_detected_frame, inlier, 2, _colors[i]);
			}
		}
		_inliers_frame_prev.push_back(inliers);

		if (count == 0)
			throw NoInliersException();
		
		std::cout << "[Object " << "] Found " << count << "/" << k << " inliers" << std::endl;

		//Define corners of the object
		std::vector<cv::Point2f> corners_object;
		std::vector<cv::Point2f> corners;
		corners_object.push_back(cv::Point2f(_objects[i].cols - 1, 0));
		corners_object.push_back(cv::Point2f(_objects[i].cols - 1, _objects[i].rows - 1));
		corners_object.push_back(cv::Point2f(0, _objects[i].rows - 1));
		corners_object.push_back(cv::Point2f(0, 0));

		//Project corners into the first frame
		cv::perspectiveTransform(corners_object, corners, H);
		
		_corners_frame_prev.push_back(corners);
		
		//Draw lines and markers, looking to projected corners of the object
		for (int h = 0; h < _corners_frame_prev[i].size(); h++)
		{
			cv::Mat res;
			cv::line(_detected_frame, _corners_frame_prev[i][h], _corners_frame_prev[i][(h + 1) % 4], _colors[i], 2, cv::LineTypes::LINE_AA);
			cv::drawMarker(_detected_frame, _corners_frame_prev[i][h], _marker_color, cv::MARKER_CROSS);
		}

	}
}

double ObjectRecognition::getFrameRate()
{
	return _frame_rate;
}

int ObjectRecognition::getNumFrames()
{
	return _size;
}

void ObjectRecognition::printInfo()
{
	std::cout << LINE << "[" << _window_name << "] Thresholds\n" << LINE;

	for (int i = 0; i < _thresholds.size(); i++)
		std::cout << i << ":  " << _thresholds[i] << std::endl;

	std::cout << LINE;
}