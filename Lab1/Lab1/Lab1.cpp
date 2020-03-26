#include "Lab1.h"

using namespace std;

int main(int argc, char** argv)
{
    Mat img = imread("robocup.jpg", IMREAD_COLOR);

    if (img.empty())
        cout << "Could not open or find the image";

    resize(img, img, Size(img.cols / 2.0, img.rows / 2.0));

    namedWindow("image", WINDOW_AUTOSIZE);
    imshow("image", img);
    setMouseCallback("image", changeColor, (void*)&img);
    waitKey(0);
    return 0;
}

void changeColor(int event, int x, int y, int f, void* userdata)
{
    if (event == EVENT_LBUTTONDOWN)
    {
        Mat img = *(Mat*)userdata;

        changeOnClickRGB(img, x, y);
        changeOnClickHSV(img, x, y);
    }
}
