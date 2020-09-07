/**
	@file Project.cpp
	@brief Source file with main.
	@author Di Nardo Di Maio Raffaele 1204879
*/

#include "TemplateMatching.hpp"
#include "Project.hpp"

void matching(std::string input_path, std::string results_path,
              std::string output_path, Utility::Type type);

int main(int argc, char** argv)
{
	//Analysis of command line arguments
	std::string input_path;
	std::string results_path;
	std::string output_path;
	args_parser(argc, argv, input_path, results_path, output_path);

	try
	{
		bool end = false;
		
		while (!end)
		{
			int choice = -1;

			while (choice<0 || choice > 4)
			{
				show_menu();
				std::cin >> choice;
			}

			switch (choice)
			{
				case static_cast<int>(Utility::Type::CAN) :
				{
					//Only CAN dataset
					std::thread t(matching, input_path, results_path, 
						          output_path, Utility::Type::CAN);
					t.join();
					break;
				}

				case static_cast<int>(Utility::Type::DRILLER) :
				{
					//Only DRILLER dataset
					std::thread t(matching, input_path, results_path,
						          output_path, Utility::Type::DRILLER);
					t.join();
					break;
				}

				case static_cast<int>(Utility::Type::DUCK) :
				{
					//Only DUCK dataset
					std::thread t(matching, input_path, results_path,
					        	  output_path, Utility::Type::DUCK);
					t.join();
					break;
				}

				case static_cast<int>(Utility::Type::DATASETS_NUM) :
				{
					//All datasets
					std::thread t1(matching, input_path, results_path,
		    					  output_path, Utility::Type::CAN);
					std::thread t2(matching, input_path, results_path,
						          output_path, Utility::Type::DRILLER);
					std::thread t3(matching, input_path, results_path,
						          output_path, Utility::Type::DUCK);
					
					t1.join();
					t2.join();
					t3.join();

					break;
				}

				case Utility::EXIT:
					end = true;
			}
		}
	}
	catch (const InputIMGException& e)
	{
		//No png images or bmp images found in input folder
		std::cout << e.what() << std::endl;
		return 1;
	}

	return 0;
}

void show_menu()
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
	std::cout << Utility::Menu::title <<LINE << DEFAULT;
}

void args_parser(int argc, char** argv, std::string &input_path, std::string &results_path, std::string &output_path)
{
	bool help = false;

	input_path = "";
	results_path = "";
	output_path = "";

	for (int i = 0; i < argc; i++)
	{
		if (std::string(argv[i]) == "-i" || std::string(argv[i]) == "-input")
		{
			//Doubled -i 
			if (input_path != "")
			{
				std::cout << Utility::Menu::error << "\nDoubled arg: -i" << std::endl;
				cmd_help();
			}

			if (i != (argc - 1))
				input_path = std::string(argv[++i]);
			else
			{
				std::cout << Utility::Menu::error << "\nMissing path of arg: -i" << std::endl;
				cmd_help();
			}

			continue;
		}
		else if (std::string(argv[i]) == "-r" || std::string(argv[i]) == "-results")
		{
			//Doubled -i 
			if (results_path != "")
			{
				std::cout << Utility::Menu::error << "\nDoubled arg: -r" << std::endl;
				cmd_help();
			}

			if (i != (argc - 1))
				results_path = std::string(argv[++i]);
			else
			{
				std::cout << Utility::Menu::error << "\nMissing path of arg: -r" << std::endl;
				cmd_help();
			}

			continue;
		}
		else if (std::string(argv[i]) == "-o" || std::string(argv[i]) == "-output")
		{
			//Doubled -i 
			if (output_path != "")
			{
				std::cout << Utility::Menu::error << "\nDoubled arg: -o" << std::endl;
				cmd_help();
			}

			if (i != (argc - 1))
				output_path = std::string(argv[++i]);
			else
			{
				std::cout << Utility::Menu::error << "\nMissing path of arg: -o" << std::endl;
				cmd_help();
			}

			continue;
		}
		else if (std::string(argv[i]) == "-h" || std::string(argv[i]) == "-help")
		{
			std::cout << "\n";
			cmd_help();
		}
	}

	if (input_path == "" || input_path == "-r" || input_path == "-o")
	{
		std::cout << Utility::Menu::error << "\nMissing mandatory argument: -i input_path" << std::endl;
		cmd_help();
	}

	if (output_path == "-i" || output_path == "-r")
	{
		std::cout << Utility::Menu::error << "\nMissing path of arg: -o" << std::endl;
		cmd_help();
	}

	if (results_path == "-i" || input_path == "-o")
	{
		std::cout << Utility::Menu::error << "\nMissing path of arg: -r" << std::endl;
		cmd_help();
	}
}

void cmd_help()
{
	std::cout << Utility::Menu::title << "./Project.exe -i input_path [-r results_input] [-o output_path]\n" << DEFAULT;
	std::cout << std::endl << Utility::Menu::cmd_arg << std::setw(14) << std::left << "input_path:   " << DEFAULT;
	std::cout << "Input folder containg the 3 subfolders" << std::endl <<
			  std::setw(14) << std::left << "" << "(can/, driller/ and duck/) of the 3 datasets" << std::endl;
	std::cout << std::endl << Utility::Menu::cmd_arg << std::setw(14) << std::left << "results_path: " << DEFAULT;
	std::cout << "Output folder that will contain the results files " << std::endl <<
		      std::setw(14) << std::left << "" << "(can_results.txt, driller_results.txt, duck_results.txt)" << std::endl;
	std::cout << std::endl << Utility::Menu::cmd_arg << std::setw(14) << std::left << "output_path: " << DEFAULT;
	std::cout << "Output folder that will contain the result images" << std::endl <<
		      std::setw(14) << std::left << "" << "with detected matches" << std::endl;
	exit(0);
}

void matching(std::string input_path, std::string results_path, 
	          std::string output_path, Utility::Type type)
{			 
	try
	{
		TemplateMatching tm = TemplateMatching(input_path, results_path, output_path, type);
		tm.match();
	}
	catch (const std::ios_base::failure e)
	{
		std::cout << "Error opening output results file" << std::endl;
	}
}