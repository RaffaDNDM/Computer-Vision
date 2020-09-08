#ifndef MENU
#define MENU

#include "Project.hpp"

class Menu
{
public:
	/**
	@brief Display menu options.
*/
	static void show_menu();

	/**
		@brief Display command line arguments infos.
	*/
	static void cmd_help();

	/**
		@brief Parser of command line arguments.
		@param argc number of inserted arguments
		@param argv list of command line arguments
		@param input_path folder containing subfolders with images inside them
		@param results_path folder that will contain the txt results files
		@param output_path folder that will contain the images with detected templates
		@param chamfer_distance true if using chamfer distance, false if using template matching + histogram
	*/
	static void args_parser(int argc, char** argv, std::string& input_path, std::string& results_path,
		std::string& output_path, bool& distance_transform);


	/**
		@brief Print command line arguments specified by the user.
		@param input_path folder containing subfolders with images inside them
		@param results_path folder that will contain the txt results files
		@param output_path folder that will contain the images with detected templates
		@param chamfer_distance true if using chamfer distance, false if using template matching + histogram
	*/
	static void print_specified_args(std::string input_path, std::string results_path,
								     std::string output_path, bool distance_transform);


	/**
		@brief Check if the path specified has correct format.
		@param path path specified for arg
		@param arg specified by user
	*/
	static void checkPath(std::string path, std::string arg);
};

#endif