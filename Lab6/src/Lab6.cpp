/**
	@file Lab5.cpp
	@brief Source file with main.
	@author Di Nardo Di Maio Raffaele 1204879
*/
#include "Lab6.h"
#include "ObjectRecognition.h"

std::mutex mutex;

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr << LINE << std::endl;
		std::cerr << "You need to insert, as command line arguments, in order" << std::endl;
		std::cerr << "       <input_images_folder>     <1|2>" << std::endl;
		std::cerr << " 1 = ORB   2 = SIFT  "<< std::endl;
		std::cerr << LINE << std::endl;
		return 1;
	}

	float ratio;
	std::cout << LINE;
	std::cout << "Insert ratio " << std::endl;
	std::cin >> ratio;
	std::cout << "\n" << LINE;

	ObjectRecognition or1(argv[1], argv[2], ratio);
	std::vector<cv::Mat> frames = or1.getFrames();
	std::vector<cv::Mat> objects = or1.getObjects();
	ObjectRecognition or2(frames, objects, ratio);
	
	std::thread t1(recognition, std::ref(or1), true);
	std::thread t2(recognition, std::ref(or2), false);

	t1.join();
	t2.join();

	cv::namedWindow(window_ORB, cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO | cv::WINDOW_GUI_EXPANDED);
	cv::imshow(window_ORB, or1.getResult());
	cv::namedWindow(window_SIFT, cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO | cv::WINDOW_GUI_EXPANDED);
	cv::imshow(window_SIFT, or2.getResult());
	cv::waitKey(0);
}


//Aggiungere 2 thread e funzione con riferimento a oggetto PanoramicImage e tipo di procedura isORB
void recognition(ObjectRecognition &or, bool isORB)
{
	or.panoramicImage(isORB);
}