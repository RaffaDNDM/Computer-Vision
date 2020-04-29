#include "Lab4.h"
#include "StreetDetector.h"

int main(int argc, char** argv)
{
    cv::Mat img = cv::imread("Images/input.png", cv::IMREAD_COLOR);
    
    /*
    cv::namedWindow("ciao", cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO | cv::WINDOW_GUI_EXPANDED);
    cv::imshow("ciao", img);
    cv::waitKey(0);
    */

    StreetDetector sd(img);

    return 0;
}