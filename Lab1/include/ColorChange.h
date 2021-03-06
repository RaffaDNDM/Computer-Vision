/**
	@file ColorChange.cpp
	@brief Header of source file for processing of image.
	@author Di Nardo Di Maio Raffaele
*/

#ifndef COLOR_CHANGE
#define COLOR_CHANGE

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

#define NEIGHBORHOOD_Y 9
#define NEIGHBORHOOD_X 9
#define GAP 4

using namespace cv;

void square_mean(Mat img, int x, int y, Vec3i &color);
void changeOnClickRGB(Mat img, int x, int y);
void changeOnClickHSV(Mat img, int x, int y);

#endif