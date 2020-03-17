#include<opencv2/opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;

int main()
{
    Mat img = imread("lena.jpg", IMREAD_COLOR);
    
    if (img.empty())
        cout << "Could not open or find the image";
    
    namedWindow("image", WINDOW_AUTOSIZE);
    waitKey(0);

    return 0;
}
