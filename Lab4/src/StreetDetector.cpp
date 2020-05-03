#include "StreetDetector.h"

//Constructor
StreetDetector::StreetDetector(cv::Mat input_img):
	_input_img{ input_img },
	_cd{ CannyDetector::CannyDetector(_input_img, 0, 0, 3) },
	_hld{ HoughLinesDetector::HoughLinesDetector(_input_img, 1, 1, 0) },
	_hcd{ HoughCirclesDetector::HoughCirclesDetector(_input_img, 1, 4, 1, 1, 4, 8) }
{}

// perform detection (Canny + Hough)
void StreetDetector::detect()
{
	/*
	_cd.detect();
	_hld.detectLines();    

	while(true)
	{
		cv::waitKey(MAX_TIMEOUT);
		//std::cout << _cd.getModified() << std::endl;

		if (_cd.getModified())
		{
			_cd.setModified(false);
			_hld.setInput(_cd.getResult());
			_hld.houghLines();
		} 
	}
	*/


}

// get the output of the filter
cv::Mat StreetDetector::getResult()
{
	return _result_img;
}

//set input image
void StreetDetector::setInput(cv::Mat img)
{
	_input_img = img;
}

//get input image
cv::Mat StreetDetector::getInput()
{
	return _input_img;
}