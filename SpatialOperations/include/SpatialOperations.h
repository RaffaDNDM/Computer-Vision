#pragma once
#include<opencv2/opencv.hpp>
#include<iostream>
#include<cmath>
using namespace std;
using namespace cv;

enum Option {
	Exit,
	Negative,
	Log,
	Gamma,
	Contrast_Strech,
	Contrast_threshold,
	Intensity_slice,
	Bitplane_slice,
};

namespace transform
{
	void negative(Mat &img);
	void log(Mat &img);
	void gamma(Mat &img);
	void contrast_stretch(Mat &img);
	void contrast_thresh(Mat &img);
	void intensity_slice(Mat &img);
	void bitplane_slice(Mat &img);
};

void spatialOperations(int, int, int, int, void*);