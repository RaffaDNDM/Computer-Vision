/**
	@file Lab5.h
	@brief Header of source file with main.
	@author Di Nardo Di Maio Raffaele 1204879
*/

#ifndef LAB5
#define LAB5

#include <memory>
#include <iostream>
#include <limits>

#include <opencv2/core.hpp>
#include <opencv2/core/utils/filesystem.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/stitching.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#define LINE "-----------------------------------------------\n"


//Name of the ORB window
const std::string window_ORB = "ORB Panoramic View";

//Name of the SIFT window
const std::string window_SIFT = "SIFT Panoramic View";

#endif