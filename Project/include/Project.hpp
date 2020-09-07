/**
	@file Project.hpp
	@brief Header of source file with main.
	@author Di Nardo Di Maio Raffaele 1204879
*/

#ifndef PROJECT
#define PROJECT

#include <memory>
#include <iostream>
#include <thread>
#include <mutex>
#include <iomanip>
#include <fstream>


#include <opencv2/core.hpp>
#include <opencv2/core/utils/filesystem.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

/**
	@brief Display menu options.
*/
void show_menu();

/**
	@brief Display command line arguments infos.
*/
void cmd_help();

/**
	@brief Parser of command line arguments.
	@param argc number of inserted arguments
	@param argv list of command line arguments
	@param input_path folder containing subfolders with images inside them
	@param results_path folder that will contain the txt results files
	@param output_path folder that will contain the images with detected templates
*/
void args_parser(int argc, char** argv, std::string& input_path, 
	             std::string& results_path, std::string& output_path);

#endif