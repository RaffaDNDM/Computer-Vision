#include "Lab4.h"
#include "StreetDetector.h"

int main(int argc, char** argv)
{
    //argv[1] = path+filename of input image
    if (argc < 2)
        std::cerr<<"Few arguments"<<std::endl;

    cv::Mat img = cv::imread(argv[1], cv::IMREAD_COLOR);

    //Detection of street and signal
    StreetSignalDetector ssd(img);
    ssd.detect();

    return 0;
}