#ifndef HOUGH_TRANSFORM
#define HOUGH_TRANSFORM

#include "Lab4.h"

//void HoughLines(InputArray image, OutputArray lines, double rho, double theta, int threshold, double srn=0, double stn=0 )
// Generic class implementing a filter with the input and output image data and the parameters
class HoughTransform {

public:
	//Constructor
	HoughTransform(cv::Mat input_img, double rho, double theta, int threshold);

	//detect lines through Hough transform
	void detectLines();

	//detect circles through Hough transform
	void detectCircles();

	//set the input image
	void setInput(cv::Mat img);

	//get the input image
	cv::Mat getInput();

	// get the output
	cv::Mat getResult();

	//set the distance resolution
	void setRho(double rho);

	//get the distance resolution
	double getRho();

	//set the angle resolution
	void setTheta(double theta);

	//get the angle resolution
	double getTheta();

	//set the accumulator threshold
	void setThresh(int threshold);

	//get the accumulator threshold
	int getThresh();


private:
	// input image
	cv::Mat _input_img;

	//distance resolution of the accumulator in pixels
	double _rho;

	//angle resolution of the accumulator in radians
	double _theta;

	//accumulator threshold
	int _threshold;

	// output image
	cv::Mat _result_img;
};

#endif