/**
	@file HoughLinesDetector.cpp
	@brief Class for Hough transform to detect street.
	@author Di Nardo Di Maio Raffaele
*/
#include "HoughLinesDetector.h"

HoughLinesDetector::HoughLinesDetector(cv::Mat input_img, int rho, int theta, int threshold):
	_input_img{ input_img.clone() },
	_detected_img{ input_img.clone() },
	_rho{ rho },
	_theta { theta },
	_threshold { threshold },
	_result_img{ input_img.clone()}
{
	cv::cvtColor(_input_img, _input_img, cv::COLOR_BGR2GRAY);
}

void HoughLinesDetector::detectLines()
{
	cv::namedWindow(window, cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO | cv::WINDOW_GUI_EXPANDED);
	cv::createTrackbar("Rho", window, &(_rho), 5, houghLinesTrackbar, (void*) this);
	cv::createTrackbar("Theta", window, &(_theta), 150, houghLinesTrackbar, (void*) this);
	cv::createTrackbar("Threshold", window, &(_threshold), 100, houghLinesTrackbar, (void*) this);
	cv::imshow(window, _result_img);
}


void HoughLinesDetector::houghLinesTrackbar(int value, void* params)
{
	HoughLinesDetector* hld = (HoughLinesDetector*) params;

	// rho, theta != 0
	if (hld->getRho() == 0.0)
	{
		hld->setRho(1);
		cv::setTrackbarPos("Rho", hld->window, 1);
	}

	if (hld->getTheta() == 0.0)
	{
		hld->setTheta(1);
		cv::setTrackbarPos("Theta", hld->window, 1);
	}

	//Hough transform for detection of lines
	hld->houghLines();
}

void HoughLinesDetector::houghLines()
{
	cv::Mat result = _detected_img.clone();

	/*
		The check about the parameter is repeated because
		this function could be called by user without
		trackbars in future version of the program
	*/
	if (getRho() == 0.0)
	{
		setRho(1);
		std::cout << "Theta set to 1 because it can't be 0.0" << std::endl;
	}

	if (getTheta() == 0.0)
	{
		setTheta(1);
		std::cout<<"Theta set to 1 because it can't be 0.0"<<std::endl;
	}

	std::vector<cv::Vec2f> lines;

	cv::HoughLines(getInput(), lines, getRho(), getTheta(), getThresh());

	std::cout << LINE << std::endl;
	std::cout << "Number of detected lines: " << lines.size()<<std::endl;
	std::cout << LINE << std::endl;
	std::cout << "Theta:  " << getTheta() << std::endl;
	std::cout << "Rho:    " << getRho() << std::endl;
	std::cout << "Thresh: " << getThresh() << std::endl;
	std::cout << LINE << std::endl;

	/*
		//To print all the lines, instead of the for used next

		for (size_t i = 0; i < lines.size() && lines.size() >= 2; i++)
		{
			float rho = lines[i][0];
			float theta = lines[i][1];
			double a = cos(theta), b = sin(theta);
			double x0 = a * rho, y0 = b * rho;
			cv::Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * (a)));
			cv::Point pt2(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * (a)));

			line(result, pt1, pt2, cv::Scalar(0, 0, 255), 3, 8);
		}
	*/


	std::vector<std::vector<cv::Point>> triangle_corners;

	if(lines.size() >= 2)
	{
		std::vector<cv::Point> corners;
		float rho1 = lines[0][0];
		float theta1 = lines[0][1];
		double cos1 = cos(theta1), sin1 = sin(theta1);
		float rho2 = lines[1][0];
		float theta2 = lines[1][1];
		double cos2 = cos(theta2), sin2 = sin(theta2);

		cv::Point intersection;
		intersection.x = cvRound((rho1 / sin1 - rho2 / sin2) / (cos1 / sin1 - cos2 / sin2));
		intersection.y = cvRound(-((cos1/sin1)*intersection.x) + (rho1/sin1));
		corners.push_back(intersection);

		cv::Point point1, point2;
		point1.y = point2.y = result.rows-1;
		point1.x = cvRound((rho1 / cos1) - (point1.y * (sin1 / cos1)));
		point2.x = cvRound((rho2 / cos2) - (point2.y * (sin2 / cos2)));
		corners.push_back(point1);
		corners.push_back(point2);
		triangle_corners.push_back(corners);

		cv::fillPoly(result, triangle_corners, cv::Scalar(0, 0, 255));
	}

	setResult(result.clone());
	cv::imshow(window, result);
}

void HoughLinesDetector::setInput(cv::Mat img)
{
	_input_img = img.clone();
}

cv::Mat HoughLinesDetector::getInput()
{
	return _input_img;
}

void HoughLinesDetector::setResult(cv::Mat img)
{
	_result_img = img;
}

cv::Mat HoughLinesDetector::getResult()
{
	return _result_img;
}

void HoughLinesDetector::setRho(int rho)
{
	_rho = rho;
}

double HoughLinesDetector::getRho()
{
	return _rho*RATIO_RHO;
}

void HoughLinesDetector::setTheta(int theta)
{
	_theta = theta;
}

double HoughLinesDetector::getTheta()
{
	return _theta*RATIO_THETA;
}

void HoughLinesDetector::setThresh(int threshold)
{
	_threshold = threshold;
}

int HoughLinesDetector::getThresh()
{
	return _threshold*RATIO_THRESHOLD;
}
