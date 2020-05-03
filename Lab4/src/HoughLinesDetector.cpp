#include "HoughLinesDetector.h"

//Constructor
HoughLinesDetector::HoughLinesDetector(cv::Mat input_img, int rho, int theta, int threshold):
	_input_img{ input_img.clone() },
	_rho{ rho },
	_theta { theta },
	_threshold { threshold },
	_result_img{ input_img.clone()}
{
	cv::cvtColor(_input_img, _input_img, cv::COLOR_BGR2GRAY);
}

//detect lines through Hough transform
void HoughLinesDetector::detectLines()
{
	_rho = 1;
	_theta = 37;
	_threshold = 26;
	cv::namedWindow(window, cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO | cv::WINDOW_GUI_EXPANDED);
	cv::createTrackbar("Rho", window, &(_rho), 5, houghLinesTrackbar, (void*) this);
	cv::createTrackbar("Theta", window, &(_theta), 150, houghLinesTrackbar, (void*) this);
	cv::createTrackbar("Threshold", window, &(_threshold), 100, houghLinesTrackbar, (void*) this);
	cv::imshow(window, _result_img);
}


void HoughLinesDetector::houghLinesTrackbar(int value, void* params)
{
	HoughLinesDetector* hld = (HoughLinesDetector*) params;
	hld->houghLines();
}

void HoughLinesDetector::houghLines()
{
	cv::Mat result = _result_img.clone();

	if (getRho() == 0.0)
		setRho(1);

	if (getTheta() == 0.0)
		setTheta(1);

	std::vector<cv::Vec2f> lines;
	std::cout << "Theta: " << getTheta() << std::endl;
	std::cout << "Rho: " << getRho() << std::endl;
	std::cout << "Thresh:  " << getThresh() << std::endl << std::endl;

	cv::HoughLines(getInput(), lines, getRho(), getTheta(), getThresh());
	
	/*
		To print all the lines

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

	cv::imshow(window, result);
}

//detect circles through Hough transform
void HoughLinesDetector::detectCircles()
{

}

//set the input image
void HoughLinesDetector::setInput(cv::Mat img)
{
	_input_img = img.clone();
}

//get the input image
cv::Mat HoughLinesDetector::getInput()
{
	return _input_img;
}

// get the output
cv::Mat HoughLinesDetector::getResult()
{
	return _result_img;
}

//set the distance resolution
void HoughLinesDetector::setRho(int rho)
{
	_rho = rho;
}

//get the distance resolution
double HoughLinesDetector::getRho()
{
	return _rho;
}

//set the angle resolution
void HoughLinesDetector::setTheta(int theta)
{
	_theta = theta;
}

//get the angle resolution
double HoughLinesDetector::getTheta()
{
	return _theta*(CV_PI/150);
}

//set the accumulator threshold
void HoughLinesDetector::setThresh(int threshold)
{
	_threshold = threshold;
}

//get the accumulator threshold
int HoughLinesDetector::getThresh()
{
	return _threshold*5;
}