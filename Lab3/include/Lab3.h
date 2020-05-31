/**
	@file Lab3.h
	@brief Header of source file with main.
	@author Di Nardo Di Maio Raffaele
*/

#ifndef LAB3
#define LAB3

#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>

#define STEP_SIGMA 0.1

using namespace std;
using namespace cv;

struct median_param
{
	int kernel_size;
	Mat src;
	Mat out;
};

struct gauss_param
{
	int kernel_size;
	int sigma_x;
	Mat src;
	Mat out;
};

struct bilateral_param
{
	int sigma_range;
	int sigma_space;
	Mat src;
	Mat out;
};

namespace colors {

	const int num_channels = 3;

	enum class Rgb {
		B,
		G,
		R,
		R_LEVELS = 256,
		G_LEVELS = 256,
		B_LEVELS = 256
	};

	enum class Hsv {
		H,
		S,
		V,
		H_LEVELS = 180,
		S_LEVELS = 256,
		V_LEVELS = 256
	};
};

void image_filtering(Mat& img);
void gaussianTrackbar(int value, void* params);
void morph_operators(Mat& img);
void morphTrackbar(int value, void* params);

#endif