#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

/*
namespace my {

	const int num_channels = 3;

	enum Rgb {
		B,
		G,
		R,
		R_LEVELS = 256,
		G_LEVELS = 256,
		B_LEVELS = 256
	};

	enum Hsv {
		H,
		S,
		V,
		H_LEVELS = 180,
		S_LEVELS = 256,
		V_LEVELS = 256
	};
};
*/

void image_filtering(Mat& img);
void morph_operators(Mat& img);