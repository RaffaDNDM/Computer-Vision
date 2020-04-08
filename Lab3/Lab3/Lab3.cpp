#include "Lab3.h"
#include "Histogram.h"

int main()
{
    Mat img = imread("Images/countryside.jpg", IMREAD_COLOR);
    
    if (img.empty())
        cout << "Could not open or find the image";

    bool check = false;

    while (!check)
    {
        int n = 0;
        cout << "Select the exercise you want to see, typing its number:" << endl;
        cout << "1) Histogram equalization" << endl;
        cout << "2) Image filtering" << endl;
        cout << "3) Morphological Operators" << endl;
        cout << "0) Exit from program" << endl;

        cin >> n;

        switch (n)
        {
            case 0:
                check = true;
                break;

            case 1:
                histogram_equalization(img);
                break;

            case 2:
                image_filtering(img);
                break;

            case 3:
                morph_operators(img);
                break;

            default:
                cout << "----------No option----------" << endl;
        }
    }

    return 0;
}

void image_filtering(Mat& img)
{

}

void morph_operators(Mat& img)
{

}