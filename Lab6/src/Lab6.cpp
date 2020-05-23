/**
	@file Lab5.cpp
	@brief Source file with main.
	@author Di Nardo Di Maio Raffaele 1204879
*/
#include "Lab6.h"
#include "ObjectRecognition.h"

void recognition(ObjectRecognition& or_inst, bool isORB);
std::mutex mutex;

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cerr << LINE << std::endl;
		std::cerr << "You need to insert, as command line arguments, in order" << std::endl;
		std::cerr << "       <video_pathname>     <objects_folder>" << std::endl;
		std::cerr << LINE << std::endl;
		return 1;
	}

	float ratio;
	std::cout << LINE;
	std::cout << "Insert ratio " << std::endl;
	std::cin >> ratio;
	std::cout << "\n" << LINE;

	bool isORB = false;

	ObjectRecognition or1(argv[1], argv[2], ratio);
	or1.recognition(isORB);

	std::string window = (isORB) ? window_ORB : window_SIFT;

	
	for (int i = 0; i < or1.getNumFrames(); i++)
	{
		cv::namedWindow(window, cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO | cv::WINDOW_GUI_EXPANDED);
		cv::imshow(window, or1.getDetectedFrame()[i]);
		cv::waitKey(1000 / or1.getFrameRate());
	}

	/*
	cv::namedWindow(window, cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO | cv::WINDOW_GUI_EXPANDED);
	cv::imshow(window, or1.getDetectedFrame()[0]);
	cv::waitKey(0);
	cv::waitKey(1000 / or1.getFrameRate());
	*/
	
	/*
	std::vector<cv::Mat> frames = or1.getFrames();
	std::vector<cv::Mat> objects = or1.getObjects();
	ObjectRecognition or2(frames, objects, ratio);
	
	std::thread t1(recognition, std::ref(or1), true);
	std::thread t2(recognition, std::ref(or2), false);

	t1.join();
	t2.join()
	*/
}


//Aggiungere 2 thread e funzione con riferimento a oggetto PanoramicImage e tipo di procedura isORB
void recognition(ObjectRecognition& or_inst, bool isORB)
{
	/*
	or_inst.recognition(isORB);

	std::string window = (isORB) ? window_ORB : window_SIFT;

	for (int i = 0; i < or_inst.getNumFrames(); i++)
	{
		cv::namedWindow(window, cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO | cv::WINDOW_GUI_EXPANDED);
		cv::imshow(window, or_inst.getDetectedFrame());
		cv::waitKey(1000 / or_inst.getFrameRate());
	}
	*/
}