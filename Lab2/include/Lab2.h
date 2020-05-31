/**
	@file Lab2.h
	@brief Header of source file with main().
	@author Di Nardo Di Maio Raffaele
*/

#ifndef LAB2
#define LAB2

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utils/filesystem.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/core/types_c.h>
#include <opencv2/core.hpp>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <string.h>
#include <limits>

#define LINE "-----------------------------------------------"

/**
	@brief Print the string s, specifying also what is the number of times the function was called.
	@param s string (representing the image path)
*/
void printName(cv::String s);

//pattern that we are going to look for in folders (all png images)
const cv::String pattern = "*.png";
//Number of cols in the chessboard used in input images
const int num_cols = 6;
//Number of rows in the chessboard used in input images
const int num_rows = 5;

#endif
