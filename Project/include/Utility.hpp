/**
	@file Utility.hpp
	@brief Class for definition of utility variables and values.
	@author Di Nardo Di Maio Raffaele 1204879
*/

#ifndef UTILITY
#define UTILITY

#include "Project.hpp"

//Colors of print statemets
#define LINE "_______________________________________________\n"
#define BLUE "\033[0;34m"
#define CYAN "\033[0;36m"
#define GREEN "\033[0;32m"
#define MAGENTA "\033[0;35m"
#define RED "\033[0;31m"
#define YELLOW "\033[0;33m"
#define BOLD_BLUE "\033[1;34m"
#define BOLD_CYAN "\033[1;36m"
#define BOLD_GREEN "\033[1;32m"
#define BOLD_MAGENTA "\033[1;35m"
#define BOLD_RED "\033[1;31m"
#define BOLD_YELLOW "\033[1;33m"
#define DEFAULT "\033[0m"

namespace Utility
{
	//Value for exit from program
	const int EXIT = 4;
	
	//Menu colors
	namespace Menu
	{
		const std::string title = BOLD_CYAN;
		const std::string option_num = CYAN;
		const std::string option_string = YELLOW;
		const std::string error = BOLD_RED;
		const std::string help_arg = BOLD_BLUE;
		const std::string user_arg = CYAN;
		const std::vector<std::string> commands{ "-i", "-r", "-o", "-dist"};
	};

	//Dataset type
	enum class Type
	{
		CAN,
		DRILLER,
		DUCK,
		DATASETS_NUM
	};

	//Pattern type
	enum class PatternIMG
	{
		MASKS,
		VIEWS,
		TEST,
		SET_IMAGES
	};

	//Dataset type
	enum class ColorSpace
	{
		B,
		G,
		R
	};

	//Gamma values for gamma transform for each dataset type
	const std::vector<float> gammas = { 1.4f, 1.3f, 1.4f };
	
	//Thresholds for Canny detection of a dataset 
	struct Parameter {
		double threshold_test; //Threshold for test images
		double threshold_template; //Threshold for template images
	};

	//All the thresholds of Canny detector
	const std::vector<Parameter> canny_params = { {150.0, 100.0}, //CAN thresholds
		                                          {150.0, 160.0}, //DRILLER thresholds
		                                          {100.0, 100.0} }; //DUCK thresholds
	
	
	const double view_range_Canny = 30.0f;
	const double test_range_Canny = 40.0f;

	//Subfolders of user folder on which we can find input images [MASKS, VIEWS, TEST]
	const std::vector<cv::String> sub_folders = { "/models", "/models", "/test_images" };
	//Patterns of image extensions and names w.r.t. types of input images [MASKS, VIEWS, TEST]
	const std::vector<cv::String> patterns = { "mask*.png", "model*.png", "*.jpg" };
	//Types of datasets
	const std::vector<cv::String> types = { "can", "driller", "duck" };
	//Bold colors for each dataset
	const std::vector<cv::String> bold_colors = { BOLD_BLUE, BOLD_RED, BOLD_GREEN };
	//Colors for each dataset
	const std::vector<cv::String> colors = { BLUE, RED, GREEN };
	//Folder that contains the output files (???_results.txt) with found matches infos
	const cv::String results_files_path = "../../../";
	//Color for mask in result image (one image for each match)
	const cv::Vec3b MASK_CANNY(0, 0, 255);
	//Color of the mask name in result image (one image for each match)
	const cv::Scalar MASK_NAME(255, 255, 255);
}

#endif