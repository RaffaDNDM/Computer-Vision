/**
	@file Project.cpp
	@brief Source file with main.
	@author Di Nardo Di Maio Raffaele 1204879
*/

#include "TemplateMatching.hpp"
#include "Project.hpp"

std::mutex mutex; //mutex
void matching(std::string path, Dataset::Type type); //Function called by thread

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
		std::thread t1(matching, argv[1], Dataset::Type::CAN);
		std::thread t2(matching, argv[1], Dataset::Type::DRILL);
		std::thread t3(matching, argv[1], Dataset::Type::DUCK);

		t1.join();
		t2.join();
		t3.join();
	}
	catch (const InputIMGException& e)
	{
		//No png images or bmp images found in input folder
		std::cout << e.what() << std::endl;
		return 1;
	}

	return 0;
}

void matching(std::string path, Dataset::Type type)
{			 
	TemplateMatching tm = TemplateMatching(path, type);

	mutex.lock();
	tm.printInfo();
	mutex.unlock();
}