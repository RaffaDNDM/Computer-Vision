/**
	@file Lab4.cpp
	@brief Source file with main().
	@author Di Nardo Di Maio Raffaele 1204879
*/

#include "Lab4.h"
#include "StreetDetector.h"

int main(int argc, char** argv)
{
    //argv[1] = path+filename of input image
    if (argc != 2)
    {
        std::cerr << LINE << std::endl;
        std::cerr << "You need to specify image_path, included image" << std::endl;
        std::cerr << "name, as command line arguments" << std::endl;
        std::cerr << LINE << std::endl;
        return 1;
    }

    cv::Mat img = cv::imread(argv[1], cv::IMREAD_COLOR);

    //Detection of street and signal
    StreetSignalDetector ssd(img);
    ssd.detect();

    return 0;
}
