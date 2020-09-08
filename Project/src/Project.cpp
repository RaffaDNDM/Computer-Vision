/**
	@file Project.cpp
	@brief Source file with main.
	@author Di Nardo Di Maio Raffaele 1204879
*/

#include "TemplateMatching.hpp"
#include "Project.hpp"
#include "Menu.hpp"

void matching(std::string input_path, std::string results_path, std::string output_path, 
	          Utility::Type type, bool distance_transform);

int main(int argc, char** argv)
{
	//Analysis of command line arguments
	std::string input_path;
	std::string results_path;
	std::string output_path;
	bool distance_transform;
	Menu::args_parser(argc, argv, input_path, results_path, output_path, distance_transform);

	Menu::print_specified_args(input_path, results_path, output_path, distance_transform);

	try
	{
		bool end = false;

		while (!end)
		{
			int choice = -1;

			while (choice<0 || choice > 4)
			{
				Menu::show_menu();
				std::cin >> choice;
			}

			switch (choice)
			{
				case static_cast<int>(Utility::Type::CAN) :
				{
					//Only CAN dataset
					std::thread t(matching, input_path, results_path, output_path, 
						          Utility::Type::CAN, distance_transform);
					t.join();
					break;
				}

				case static_cast<int>(Utility::Type::DRILLER) :
				{
					//Only DRILLER dataset
					std::thread t(matching, input_path, results_path, output_path, 
						          Utility::Type::DRILLER, distance_transform);
					t.join();
					break;
				}

				case static_cast<int>(Utility::Type::DUCK) :
				{
					//Only DUCK dataset
					std::thread t(matching, input_path, results_path, output_path, 
						          Utility::Type::DUCK, distance_transform);
					t.join();
					break;
				}

				case static_cast<int>(Utility::Type::DATASETS_NUM) :
				{
					//All datasets
					std::thread t1(matching, input_path, results_path, output_path, 
						           Utility::Type::CAN, distance_transform);
					std::thread t2(matching, input_path, results_path, output_path, 
						           Utility::Type::DRILLER, distance_transform);
					std::thread t3(matching, input_path, results_path, output_path, 
						           Utility::Type::DUCK, distance_transform);
					
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

void matching(std::string input_path, std::string results_path, std::string output_path, 
	          Utility::Type type, bool distance_transform)
{			 
	try
	{
		TemplateMatching tm = TemplateMatching(input_path, results_path, output_path, 
			                                   type, distance_transform);
		tm.match();
	}
	catch (const std::ios_base::failure e)
	{
		std::cout << "Error opening output results file" << std::endl;
	}
}