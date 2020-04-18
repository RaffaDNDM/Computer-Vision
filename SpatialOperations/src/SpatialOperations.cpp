#include "SpatialOperations.h"

int main(int argc, char**argv)
{
    Mat img = imread("data/IMG_3194.jpg", IMREAD_GRAYSCALE);
    img.convertTo(img, CV_8U);

    if (img.empty())
        cerr << "Could not open or find the image"<<endl;

    resize(img, img, Size(img.cols / 8, img.rows / 8));
    
    namedWindow("image", WINDOW_AUTOSIZE);
    imshow("image", img);
    waitKey(0);
    destroyWindow("image");

    bool check = false;

    while(!check)
    {
        int n = -1;
        cout << "Select one the following option:"<<endl;
        cout << "1) Negative image" << endl;
        cout << "2) Log transformation" << endl;
        cout << "3) Gamma transformation" << endl;
        cout << "4) Contrast stretching" << endl;
        cout << "5) Contrast Thresholding" << endl;
        cout << "6) Intensity slicing" << endl;
        cout << "7) Bit-plane slicing" << endl;
        cout << "0) Exit from application" << endl;

        cin >> n;

        switch (n)
        {
            case(Option::Exit):
                check = true;
                break;

            case(Option::Negative):
                transform::negative(img);
                break;

            case(Option::Log):
                transform::log(img);
                break;

            case(Option::Gamma):
                transform::gamma(img);
                break;

            case(Option::Contrast_Strech):
                transform::contrast_stretch(img);
                break;

            case(Option::Contrast_threshold):
                transform::contrast_thresh(img);
                break;

            case(Option::Intensity_slice):
                transform::intensity_slice(img);
                break;

            case(Option::Bitplane_slice):
                transform::bitplane_slice(img);
                break;

            default:
                break;
        }
    }

    return 0; 
}

void transform::negative(Mat &img)
{
    Mat img1 = img.clone();

    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            img1.at<char>(i,j) = 255 - img.at<char>(i, j);
        }
    }

    namedWindow("Negative transformation", WINDOW_AUTOSIZE);
    imshow("Negative transformation", img1);
    waitKey(4000);
    destroyAllWindows();
}

void transform::log(Mat &img)
{
    Mat img1 = img.clone();

    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            double log_arg = std::log(1.0 + img.at<char>(i, j));
            double log_coeff = 255 / std::log(256);
            img1.at<char>(i, j) = static_cast<char>((log_arg*log_coeff)+0.49);
        }
    }

    namedWindow("Log transformation", WINDOW_AUTOSIZE);
    imshow("Log transformation", img1);
    waitKey(4000);
    destroyAllWindows();
}

void transform::gamma(Mat &img)
{
    double gamma = 0.2;

    Mat img1 = img.clone();

    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            double gamma_arg = std::pow(img.at<char>(i, j),gamma);
            double gamma_coeff = std::pow(255,gamma);
            img1.at<char>(i, j) = static_cast<char>((gamma_arg * gamma_coeff) + 0.49);
        }
    }

    namedWindow("Gamma transformation", WINDOW_AUTOSIZE);
    imshow("Gamma transformation", img);
    waitKey(4000);
    destroyAllWindows();
}

void transform::contrast_stretch(Mat &img)
{
    char r1 = 256 / 4;
    char s1 = 256 / 8;
    char r2 = (256*3) / 4;
    char s2 = (256 * 7) / 8;

    Mat img1 = img.clone();

    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            if(img.at<char>(i, j)>=r1 && img.at<char>(i, j) < r2)
                img1.at<char>(i, j) = 0.0;
            else
                img1.at<char>(i, j) = 0.0;
        }
    }

    namedWindow("Contrast stretching", WINDOW_AUTOSIZE);
    imshow("Contrast stretching", img1);
    waitKey(4000);
    destroyAllWindows();
}

void transform::contrast_thresh(Mat &img)
{
    char threshold = 256 / 4;

    Mat img1 = img.clone();

    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            if (img.at<char>(i, j) < threshold)
                img1.at<char>(i, j) = 0.0;
            else
                img1.at<char>(i, j) = 255;
        }
    }

    namedWindow("Contrast threshold", WINDOW_AUTOSIZE);
    imshow("Contrast threshold", img1);
    waitKey(4000);
    destroyAllWindows();
}

void transform::intensity_slice(Mat &img)
{
    int A = 100;
    int B = 180;

    Mat img1 = img.clone();

    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            if (img.at<char>(i, j) > A && img.at<char>(i, j) <= B)
                img1.at<char>(i, j) = 30;
            else
                img1.at<char>(i, j) = 220;
        }
    }

    Mat img2 = img.clone();

    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            if (img.at<char>(i, j) > A && img.at<char>(i, j) <= B)
                img2.at<char>(i, j) = 220;
        }
    }

    namedWindow("Intensity slicing 1", WINDOW_AUTOSIZE);
    imshow("Intensity slicing 1", img1);
    waitKey(4000);
    namedWindow("Intensity slicing 2", WINDOW_AUTOSIZE);
    imshow("Intensity slicing 2", img2);
    waitKey(4000);
    destroyAllWindows();
}

void transform::bitplane_slice(Mat &img)
{
    Mat imgs[8];
    
    for (int i = 0; i < 8; i++)
        imgs[i] = img.clone();


    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            char mask = 1;
            for (int k = 0; k < 8; k++)
            {
                mask = mask << 1;
                imgs[k].at<char>(i, j) = img.at<char>(i, j) & mask;
            }
        }
    }
    
    for (int i = 0; i<8; i++)
    {
        namedWindow("Bit-plane slicing "+to_string(i), WINDOW_AUTOSIZE);
        imshow("Bit-plane slicing "+to_string(i), imgs[i]);
        waitKey(4000);
    }
    destroyAllWindows();
}