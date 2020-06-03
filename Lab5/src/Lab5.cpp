/**
	@file Lab5.cpp
	@brief Source file with main.
	@author Di Nardo Di Maio Raffaele 1204879
*/

#include "PanoramicImage.h"

std::mutex mutex; //mutex
void panoramic(PanoramicImage &p, bool isORB); //Function called by thread

int main(int argc, char** argv)
{
	//Analysis of command line arguments
	if (argc != 3)
	{
		std::cerr << LINE << std::endl;
		std::cerr << "You need to insert, as command line arguments, in order:" << std::endl;
		std::cerr << "       <input_images_folder>       <FOV>" << std::endl;
		std::cerr << LINE << std::endl;
		return 1;
	}

	double FOV = std::stod(argv[2]);
	while (FOV < 1.0)
	{
		std::cout << LINE;
		std::cout << "Insert Field Of View for the set of images" << std::endl;
		std::cin >> FOV;
	}

	//Ratio for ORB and SIFT
	float ratio1=0.0, ratio2 = 0.0;
	while (ratio1 < 1.0)
	{
		std::cout << LINE;
		std::cout << "Insert ratio for ORB (ratio >= 1.0)" << std::endl;
		std::cin >> ratio1;
	}

	while (ratio2 < 1.0)
	{
		std::cout << LINE;
		std::cout << "Insert ratio for SIFT (ratio >= 1.0)" << std::endl;
		std::cin >> ratio2;
	}

	std::cout << LINE;

	try
	{
		//p1=ORB, p2=SIFT
		PanoramicImage p1(argv[1], ratio1, FOV);
		std::vector<cv::Mat> projections = p1.getProjected();
		PanoramicImage p2(projections, ratio2, FOV);

		std::thread t1(panoramic, std::ref(p1), true);
		std::thread t2(panoramic, std::ref(p2), false);

		t1.join();
		t2.join();


		//Width and height of final comparison image
		int width = (p1.getResult().cols >= p2.getResult().cols) ? p1.getResult().cols : p2.getResult().cols;
		int height = p1.getResult().rows + p2.getResult().rows;

		//Final comparison image (top = ORB, bottom = SIFT)
		cv::Mat panoramic_comparison(cv::Size(width, height), p1.getResult().type());
		cv::Mat img = panoramic_comparison(cv::Range(0, p1.getResult().rows), cv::Range(0, p1.getResult().cols));
		(p1.getResult()).copyTo(img);
		img = panoramic_comparison(cv::Range(p1.getResult().rows, height), cv::Range(0, p2.getResult().cols));
		(p2.getResult()).copyTo(img);

		//Show final comparison image
		cv::namedWindow(window, cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO | cv::WINDOW_GUI_EXPANDED);
		cv::imshow(window, panoramic_comparison);
		cv::waitKey(0);

		cv::imwrite("../../../data/output1.png", panoramic_comparison);
	}
	catch (const InputIMGException& e)
	{
		//No png images or bmp images found in input folder
		std::cout << e.what() << std::endl;
		return 1;
	}

	return 0;
}

void panoramic(PanoramicImage &p, bool isORB)
{
	bool check = false;
	
	while (!check)
	{
		try
		{
			p.panoramic_view(isORB);
			check = true;
		}
		catch (const NoInliersException& e)
		{
			//No inliers found in between 2 images
			float ratio=0.0;
			mutex.lock();
			std::cout << LINE;
			std::cout << e.what() << std::endl;

			while (ratio < 1.0)
			{
				std::cout << LINE;
				std::cout << "Insert ratio for " << p.getMethod() << " (ratio >= 1.0)" << std::endl;
				std::cin >> ratio;
			}
			
			std::cout << LINE;
			mutex.unlock();

			p.setRatio(ratio);
			p.clearUsedVectors();
		}
	}

	//Print thresholds and detected trnaslations
	mutex.lock();
	p.printInfo();
	mutex.unlock();
}