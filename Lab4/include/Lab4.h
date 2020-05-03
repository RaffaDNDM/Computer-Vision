#ifndef LAB4

#define LAB4
#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>
#include <chrono>
#include <ctime>

#define MAX_TIMEOUT 50
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

#endif