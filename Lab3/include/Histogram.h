/**
	@file Histogram.h
	@brief Compute histogram and equalization.
	@author Di Nardo Di Maio Raffaele
*/

#ifndef HISTOGRAM
#define HISTOGRAM

#include "Lab3.h"

void compute_hist(Mat& m);
void compute_histHSV(Mat& m);
void showHistogram(vector<Mat>& hists);
Mat equalization(Mat& img);
vector<Mat> hsv_eq(Mat& img);
Mat histogram_equalization(Mat& img);

#endif