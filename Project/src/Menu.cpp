#include "Menu.hpp"
#include "Utility.hpp"

void Menu::show_menu()
{
	std::cout << Utility::Menu::title << "\nSelect which dataset you want to analyze" << std::endl << LINE;
	std::cout << Utility::Menu::option_num << "0)" << Utility::Menu::option_string
		<< " CAN dataset" << std::endl;
	std::cout << Utility::Menu::option_num << "1)" << Utility::Menu::option_string
		<< " DRILLER dataset" << std::endl;
	std::cout << Utility::Menu::option_num << "2)" << Utility::Menu::option_string
		<< " DUCK dataset" << std::endl;
	std::cout << Utility::Menu::option_num << "3)" << Utility::Menu::option_string
		<< " All datasets" << std::endl;
	std::cout << Utility::Menu::option_num << "4)" << Utility::Menu::option_string
		<< " Exit from the program" << std::endl;
	std::cout << Utility::Menu::title << LINE << DEFAULT;
}

void Menu::args_parser(int argc, char** argv, std::string& input_path, std::string& results_path,
	std::string& output_path, bool& distance_transform)
{
	distance_transform = false;
	input_path = "";
	results_path = "";
	output_path = "";

	for (int i = 1; i < argc; i++)
	{
		if (std::string(argv[i]) == "-i" || std::string(argv[i]) == "-input")
		{
			//Doubled -i: problem
			if (input_path != "")
			{
				std::cout << Utility::Menu::error << "\nDoubled arg: -i" << std::endl;
				cmd_help();
			}

			//If -i is not last arg
			if (i != (argc - 1))
			{
				input_path = std::string(argv[++i]);
				checkPath(input_path, "-i");	
			}
			else
			{
				//If -i is last arg: problem missing path
				std::cout << Utility::Menu::error << "\nMissing path of arg: -i" << std::endl;
				cmd_help();
			}

			continue;
		}
		else if (std::string(argv[i]) == "-r")
		{
			//Doubled -r: problem
			if (results_path != "")
			{
				std::cout << Utility::Menu::error << "\nDoubled arg: -r" << std::endl;
				cmd_help();
			}

			//If -r is not last arg
			if (i != (argc - 1))
			{
				results_path = std::string(argv[++i]);
				checkPath(results_path, "-r");
			}
			else
			{
				//If -r is last arg: problem missing path
				std::cout << Utility::Menu::error << "\nMissing path of arg: -r" << std::endl;
				cmd_help();
			}

			continue;
		}
		else if (std::string(argv[i]) == "-o")
		{
			//Doubled -o: problem
			if (output_path != "")
			{
				std::cout << Utility::Menu::error << "\nDoubled arg: -o" << std::endl;
				cmd_help();
			}

			//If -o is not last arg
			if (i != (argc - 1))
			{
				output_path = std::string(argv[++i]);
				checkPath(output_path, "-o");
			}
			else
			{
				//If -o is last arg: problem missing path
				std::cout << Utility::Menu::error << "\nMissing path of arg: -o" << std::endl;
				cmd_help();
			}

			continue;
		}
		else if (std::string(argv[i]) == "-dist")
		{
			//Doubled -dist: no problem (skip it)
			distance_transform = true;
			continue;
		}
		else if (std::string(argv[i]) == "-h")
		{
			//Doubled -h: no problem (skip it)
			std::cout << "\n";
			cmd_help();
		}
		else
		{
			std::cout << Utility::Menu::error << "\nCommand not found:" << argv[i] << std::endl;
			cmd_help();
		}
	}

	//-i is the last argument
	if (input_path == "")
	{
		std::cout << Utility::Menu::error << "\nMissing mandatory argument: -i input_path" << std::endl;
		cmd_help();
	}
}

void Menu::cmd_help()
{
	std::cout << Utility::Menu::title << "./Project.exe -i input_path [-r results_input] [-o output_path]\n" << DEFAULT;
	std::cout << std::endl << Utility::Menu::help_arg << std::setw(14) << std::left << "input_path:   " << DEFAULT;
	std::cout << "Input folder containg the 3 subfolders" << std::endl <<
		std::setw(14) << std::left << "" << "(can/, driller/ and duck/) of the 3 datasets" << std::endl;
	std::cout << std::endl << Utility::Menu::help_arg << std::setw(14) << std::left << "results_path: " << DEFAULT;
	std::cout << "Output folder that will contain the results files " << std::endl <<
		std::setw(14) << std::left << "" << "(can_results.txt, driller_results.txt, duck_results.txt)" << std::endl;
	std::cout << std::endl << Utility::Menu::help_arg << std::setw(14) << std::left << "output_path: " << DEFAULT;
	std::cout << "Output folder that will contain the result images" << std::endl <<
		std::setw(14) << std::left << "" << "with detected matches" << std::endl;
	exit(0);
}

void Menu::print_specified_args(std::string input_path, std::string results_path,
	                            std::string output_path, bool distance_transform)
{
	std::cout << Utility::Menu::user_arg << std::setw(14) << std::left << "Input path:";
	std::cout << DEFAULT << input_path << std::endl;
	std::cout << Utility::Menu::user_arg << std::setw(14) << std::left << "Results path:";
	std::cout << DEFAULT << std::left << results_path << std::endl;
	std::cout << Utility::Menu::user_arg << std::setw(14) << std::left << "Output path:";
	std::cout << DEFAULT << std::left << output_path << std::endl;
	std::cout << Utility::Menu::user_arg << std::setw(14) << std::left << "Method:";
	
	std::string method = (distance_transform) ? "Distance transform" : "Histogram Refinement";
	std::cout << DEFAULT << std::left << method << std::endl;
}

void Menu::checkPath(std::string path, std::string arg)
{
	for (auto cmd : Utility::Menu::commands)
	{
		if (path == cmd)
		{
			std::cout << Utility::Menu::error << "\nMissing mandatory argument: " << 
				         arg << " input_path" << std::endl;
			cmd_help();
		}
	}

	if (path == "-h")
		cmd_help();
}