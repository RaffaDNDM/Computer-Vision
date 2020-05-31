/**
	@file Lab2.cpp
	@brief Source file with main().
	@author Di Nardo Di Maio Raffaele
*/

#include "Lab2.h"
#include "Calibration.h"

int main(int argc, char** argv)
{
    //argv[1] = chessboard images folder
    //argv[2] = test folder
    if (argc != 3)
    {
        std::cerr << LINE << std::endl;
        std::cerr << "You need to specify as command line arguments, in this order:" << std::endl;
        std::cerr << "    chessboard_images_folder     test_images_folder" << std::endl;
        std::cerr << LINE << std::endl;
        return 1;
    }

    std::vector<cv::String> chess_imgs;

    //Selection of all the files png in folder "argv[1]"
    cv::utils::fs::glob(argv[1], pattern, chess_imgs);

    //print all the names of the files selected
    for_each(chess_imgs.begin(), chess_imgs.end(), printName);

    std::vector<cv::String> test_imgs;

    //Selection of all the test files png in folder "argv[2]"
    cv::utils::fs::glob(argv[2], pattern, test_imgs);

    //calibration phase
    calibrate(chess_imgs, test_imgs);

    return 0;
}


void printName(cv::String s)
{
    //Counter of number of images
    static int count = 0;
    std::cout<<"File"<<(++count)<<": "<<s<<std::endl;
}
