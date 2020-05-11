/**
	@file HoughCirclesDetector.cpp
	@brief Class for Hough transform to detect signal.
	@author Di Nardo Di Maio Raffaele 1204879
*/

#include "HoughCirclesDetector.h"

//Constructor
HoughCirclesDetector::HoughCirclesDetector(cv::Mat input_img, int dp, int minDist, int param1,
	                                       int param2, int minRadius, int maxRadius):
	_input_img{ input_img.clone() },
	_result_img{ input_img.clone() },
	_dp{ dp },
	_minDist{ minDist },
	_param1{ param1 },
	_param2{ param2 },
	_minRadius{ minRadius },
	_maxRadius{ maxRadius }
{}

//detect lines through Hough transform
void HoughCirclesDetector::detectCircles()
{
	//Initialization with original image (so we need to convert it)
	cvtColor(_input_img, _input_img, cv::COLOR_BGR2GRAY);

	//Gaussian applied to input image to increase efficency of Canny
	GaussianBlur(_input_img, _input_img, cv::Size(9, 9), 2, 2);

	cv::namedWindow(window, cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO | cv::WINDOW_GUI_EXPANDED);
	cv::createTrackbar("Min dist", window, &(_minDist), 100, houghCirclesTrackbar, (void*)this);
	cv::createTrackbar("Param1", window, &(_param1), 100, houghCirclesTrackbar, (void*)this);
	cv::createTrackbar("Param2", window, &(_param2), 100, houghCirclesTrackbar, (void*)this);
	cv::createTrackbar("Min radius", window, &(_minRadius), 200, houghCirclesTrackbar, (void*)this);
	cv::createTrackbar("Max radius", window, &(_maxRadius), 200, houghCirclesTrackbar, (void*)this);
	cv::imshow(window, _result_img);
}

void HoughCirclesDetector::houghCirclesTrackbar(int value, void* params)
{
	HoughCirclesDetector* hcd = (HoughCirclesDetector*)params;

	//mindist, minRadius, maxRadius, treshold1, threshold2 != 0
	if (hcd->getMinDist() == 0.0)
	{
		hcd->setMinDist(1);
		cv::setTrackbarPos("Min dist", hcd->window, 1);
	}

	if (hcd->getMinRadius() == 0)
	{
		hcd->setMinRadius(1);
		cv::setTrackbarPos("Min radius", hcd->window, 1);
	}

	if (hcd->getMaxRadius() == 0)
	{
		hcd->setMaxRadius(1);
		cv::setTrackbarPos("Max radius", hcd->window, 1);
	}

	if (hcd->getParam1() == 0.0)
	{
		hcd->setParam1(1);
		cv::setTrackbarPos("Param1", hcd->window, 1);
	}

	if (hcd->getParam2() == 0.0)
	{
		hcd->setParam2(1);
		cv::setTrackbarPos("Param2", hcd->window, 1);
	}

	//Hough transform for detection of circles
	hcd->houghCircles();
}

void HoughCirclesDetector::houghCircles()
{
	cv::Mat result = _detected_img.clone();

	std::vector<cv::Vec3f> circles;

	/*
		The check about the parameter is repeated because
		this function could be called by user without
		trackbars in future version of the program
	*/
	if (getMinDist() == 0.0)
	{
		setMinDist(1);
		std::cout << "Min dist set to 1 because it can't be 0.0" << std::endl;
	}

	if (getMinRadius() == 0)
	{
		setMinRadius(1);
		std::cout << "Min radius set to 1 because it can't be 0.0" << std::endl;
	}

	if (getMaxRadius() == 0)
	{
		setMaxRadius(1);
		std::cout << "Max radius set to 1 because it can't be 0.0" << std::endl;
	}

	if (getParam1() == 0.0)
	{
		setParam1(1);
		std::cout << "Param1 set to 1 because it can't be 0.0" << std::endl;
	}

	if (getParam2() == 0.0)
	{
		setParam2(1);
		std::cout << "Param2 set to 1 because it can't be 0.0" << std::endl;
	}

	/// Apply the Hough Transform to find the circles
	cv::HoughCircles(getInput(), circles, cv::HOUGH_GRADIENT, getdp(), getMinDist(), getParam1(), getParam2(), getMinRadius(), getMaxRadius());


	std::cout << LINE << std::endl;
	std::cout << "Number of detected circles:" << circles.size() << std::endl;
	std::cout << LINE << std::endl;
	std::cout << "dp:         " << getdp() << std::endl;
	std::cout << "min_dist:   " << getMinDist() << std::endl;
	std::cout << "param1:     " << getParam1() << std::endl;
	std::cout << "param2:     " << getParam2() << std::endl;
	std::cout << "min_radius: " << getMinRadius() << std::endl;
	std::cout << "max_radius: " << getMaxRadius() << std::endl;
	std::cout << LINE << std::endl;



	/*
		Draw all the detected circles

		for (size_t i = 0; i < circles.size(); i++)
		{
			cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			// circle center
			//circle(result, center, 3, cv::Scalar(0, 255, 0), -1, 8, 0);
			// circle outline
			circle(result, center, radius, cv::Scalar(0, 255, 0), cv::FILLED, 8, 0);
		}
	*/

	//Print the circle with highest score
	if (circles.size() >= 1)
	{
		cv::Point center(cvRound(circles[0][0]), cvRound(circles[0][1]));
		int radius = cvRound(circles[0][2]);
		circle(result, center, radius, cv::Scalar(0, 255, 0), cv::FILLED, 8, 0);
	}

	setResult(result.clone());

	cv::imshow(window, result);
}

//set the input image
void HoughCirclesDetector::setInput(cv::Mat img)
{
	_input_img = img.clone();
	_detected_img = img.clone();
}

//get the input image
cv::Mat HoughCirclesDetector::getInput()
{
	return _input_img;
}

// get the output
cv::Mat HoughCirclesDetector::getResult()
{
	return _result_img;
}

// set the output
void HoughCirclesDetector::setResult(cv::Mat img)
{
	_result_img = img;
}

//set the distance resolution
void HoughCirclesDetector::setdp(int dp)
{
	_dp = dp;
}

//get the distance resolution
double HoughCirclesDetector::getdp()
{
	return _dp * RATIO_DP;
}

//set the angle resolution
void HoughCirclesDetector::setMinDist(int dist)
{
	_minDist = dist;
}

//get the angle resolution
double HoughCirclesDetector::getMinDist()
{
	return _minDist * RATIO_MIN_DIST;
}

//set the distance resolution
void HoughCirclesDetector::setParam1(int param)
{
	_param1 = param;
}

//get the distance resolution
double HoughCirclesDetector::getParam1()
{
	return _param1 * RATIO_PARAM1;
}

//set the distance resolution
void HoughCirclesDetector::setParam2(int param)
{
	_param1 = param;
}

//get the distance resolution
double HoughCirclesDetector::getParam2()
{
	return _param2 * RATIO_PARAM2;
}

//set the angle resolution
void HoughCirclesDetector::setMinRadius(int radius)
{
	_minRadius = radius;
}

//get the angle resolution
int HoughCirclesDetector::getMinRadius()
{
	return _minRadius;
}

//set the angle resolution
void HoughCirclesDetector::setMaxRadius(int radius)
{
	_maxRadius = radius;
}

//get the angle resolution
int HoughCirclesDetector::getMaxRadius()
{
	return _maxRadius;
}
