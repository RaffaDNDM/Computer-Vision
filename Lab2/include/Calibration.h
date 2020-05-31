/**
	@file Calibration.h
	@brief Camera Calibration.
	@author Di Nardo Di Maio Raffaele
*/

#ifndef CALIBRATION
#define CALIBRATION

#include "Lab2.h"

/**
    @brief Calibration phase of input images set and rectification of test images set.
    @param chess_imgs vector of input images names
    @param test_imgs vector of test images names
*/
void calibrate(const std::vector<cv::String> chess_imgs, const std::vector<cv::String> test_imgs);

/**
	  @brief Detection of 2D-corners in input images.
    @param img_names vector of input images names
    @param corners2D vector of all detected corners in images, specified by img_names
*/
void corners2D_detect(const std::vector<cv::String> img_names, std::vector<std::vector<cv::Point2f>>& corners2D);

/**
    @brief Definition of 3D-corners.
    @param imgs_num num of input images
    @param corners3D vector of all corners in 3D space, for input images
*/
void corners3D_definition(const int imgs_num, std::vector<std::vector<cv::Point3f>>& corners3D);

/**
    @brief Print the calibration parameters.
    @param K camera matrix
    @param distCoeffs vector of distortion coefficients
*/
void print_calibration(const cv::Mat K, const std::vector<float> distCoeffs);

/**
    @brief Detection of 2D-corners in input images.
    @param img_names vector of input images names
    @param corners3D vector of all corners in 3D space, for input images
    @param corners2D vector of all detected corners in images, specified by img_names
    @param rvecs vector of rotation vectors
    @param tvecs vector of translation vectors
    @param K camera matrix
    @param distCoeffs vector of distortion coefficients
*/
void error(const std::vector<cv::String> img_names, std::vector<std::vector<cv::Point3f>> corners3D,
    const std::vector<std::vector<cv::Point2f>> corners2D, const std::vector<cv::Mat> rvecs,
    const std::vector<cv::Mat> tvecs, const cv::Mat K, const std::vector<float> distCoeffs);

/**
    @brief Detection of 2D-corners in input images.
    @param img image to rectify
    @param K camera matrix
    @param distCoeffs vector of distortion coefficients
    @return comparison image
*/
cv::Mat rectify_image(cv::Mat img, const cv::Mat K, const std::vector<float> distCoeffs);


//printed on stdout before corners2D_detect calls
const std::string phase_2D = "2d";
//printed on stdout before corners3D_definition call
const std::string phase_3D = "3d";
//printed on stdout before calibrate call
const std::string phase_calibration = "Calibration";
//name of window with comparison between original image and rectified image
const std::string window_name = "Image vs Rectified Image  ";

#endif
