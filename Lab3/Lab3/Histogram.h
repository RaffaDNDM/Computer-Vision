#include "Lab3.h"

void compute_hist(Mat& m);
void compute_histHSV(Mat& m);
void showHistogram(vector<Mat>& hists);
Mat equalization(Mat& img);
vector<Mat> hsv_eq(Mat& img);
void histogram_equalization(Mat& img);