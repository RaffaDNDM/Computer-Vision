/**
	@file StreetDetector.cpp
	@brief Class for Street and Signal detection.
	@author Di Nardo Di Maio Raffaele
*/

#include "StreetDetector.h"

//Constructor
StreetSignalDetector::StreetSignalDetector(cv::Mat input_img):
	_input_img{ input_img },
	_cd{ CannyDetector::CannyDetector(_input_img, 100, 200, 3) },
	_hld{ HoughLinesDetector::HoughLinesDetector(_input_img, 1, 37, 14) },
	_hcd{ HoughCirclesDetector::HoughCirclesDetector(_input_img, 1, 69, 30, 23, 1, 14) }
{}

// perform detection (Canny + Hough)
void StreetSignalDetector::detect()
{
	bool check = false;

	//Initialization of windows
	_cd.detect();
	_hld.detectLines();

	//Perform Canny detection and Hough line detection
	_cd.canny();
	_hld.setInput(_cd.getResult());
	_hld.houghLines();

	//Continuous refresh of Hough Line window if there is an update of
	//Canny window because of changes of trackbar positions
	while(!check)
	{
		char key = (char) cv::waitKey(MAX_TIMEOUT);

		if (key == (char) 27 || key == 'q' || key == '\r')
			check = true;
		else if (_cd.getModified())
		{

			_cd.setModified(false);
			_hld.setInput(_cd.getResult());
			_hld.houghLines();
		}
	}

	/**
		I used this function call to store the image and use it in the report

		cv::imwrite("../../../result/result_lines.png", _hld.getResult());
	*/

	cv::destroyAllWindows();
	_hcd.setInput(_hld.getResult());
	_hcd.detectCircles();
	_hcd.houghCircles();
	cv::waitKey(0);

	/**
		I used this function call to store the image and use it in the report

		cv::imwrite("../../../result/result.png", _hcd.getResult());
	*/
}

// get the output of the filter
cv::Mat StreetSignalDetector::getResult()
{
	return _result_img;
}

//set input image
void StreetSignalDetector::setInput(cv::Mat img)
{
	_input_img = img;
}

//get input image
cv::Mat StreetSignalDetector::getInput()
{
	return _input_img;
}
