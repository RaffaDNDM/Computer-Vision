/**
	@file Lab6.cpp
	@brief Source file with main.
	@author Di Nardo Di Maio Raffaele
*/

#include "Lab6.h"
#include "ObjectRecognition.h"

int main(int argc, char** argv)
{
	//Management of command line arguments
	if (argc != 3)
	{
		std::cerr << LINE << std::endl;
		std::cerr << "You need to insert, as command line arguments, in order" << std::endl;
		std::cerr << "       <video_pathname>     <objects_folder>" << std::endl;
		std::cerr << LINE << std::endl;
		return 1;
	}

	float ratio;
	while (ratio < 1.0)
	{
		std::cout << LINE;
		std::cout << "Insert ratio for SIFT (ratio >= 1.0)" << std::endl;
		std::cin >> ratio;
	}

	bool check = false;

	while (!check)
	{
		try
		{
			//Creation of the instance
			ObjectRecognition objr(argv[1], argv[2], ratio);
			
			//Recognition of objects in the video
			objr.recognition();
			check = true;
		}
		catch (const InputIMGException& e1)
		{
			//No png images found in input folder, specified on command line
			std::cout << e1.what() << std::endl;
			return 1;
		}
		catch (const VideoException& e2)
		{
			//Video can't be opened
			std::cout << e2.what() << std::endl;
			return 1;
		}
		catch (const NoFirstFrameException& e3)
		{
			//No first frame found
			std::cout << e3.what() << std::endl;
			return 1;
		}
		catch (const NoInliersException& e4)
		{
			//No inliers found during detection on first frame
			std::cout << e4.what() << std::endl;
			ratio = 0.0;

			while (ratio < 1.0)
			{
				std::cout << LINE;
				std::cout << "Insert ratio for SIFT (ratio >= 1.0)" << std::endl;
				std::cin >> ratio;
			}
		}
		catch (const NoTrackedPointsException& e5)
		{
			//No flows found during Lukas-Kanade method
			std::cout << e5.what() << std::endl;
			std::cout << "(Try to improve the number of inliers detected, increasing the ratio)" << std::endl;

			ratio = 0.0;
			while (ratio < 1.0)
			{
				std::cout << LINE;
				std::cout << "Insert ratio for SIFT (ratio >= 1.0)" << std::endl;
				std::cin >> ratio;
			}
		}
	}

	return 0;
}