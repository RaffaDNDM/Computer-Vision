/**
	@file Project.cpp
	@brief Source file with main.
	@author Di Nardo Di Maio Raffaele 1204879
*/

#include "TemplateMatching.hpp"
#include "Project.hpp"

void matching(std::string path, Utility::Type type); //Function called by thread

int main(int argc, char** argv)
{
	//Analysis of command line arguments
	if (argc != 2)
	{
		std::cerr << LINE << std::endl;
		std::cerr << "You need to insert, as command line arguments, in order:" << std::endl;
		std::cerr << "              <input_images_folder>" << std::endl;
		std::cerr << LINE << std::endl;
		return 1;
	}

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
					std::thread t(matching, argv[1], Utility::Type::CAN);
					t.join();
					break;
				}

				case static_cast<int>(Utility::Type::DRILLER) :
				{
					std::thread t(matching, argv[1], Utility::Type::DRILLER);
					t.join();
					break;
				}

				case static_cast<int>(Utility::Type::DUCK) :
				{
					std::thread t(matching, argv[1], Utility::Type::DUCK);
					t.join();
					break;
				}

				case static_cast<int>(Utility::Type::DATASETS_NUM) :
				{
					std::thread t1(matching, argv[1], Utility::Type::CAN);
					std::thread t2(matching, argv[1], Utility::Type::DRILLER);
					std::thread t3(matching, argv[1], Utility::Type::DUCK);

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
	std::cout << Utility::Menu::title << "Select which dataset you want to analyze" << std::endl << LINE;
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

void matching(std::string path, Utility::Type type)
{			 
	TemplateMatching tm = TemplateMatching(path, type);
	tm.match();
}