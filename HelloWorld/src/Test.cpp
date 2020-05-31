/**
    @file Test.cpp
    @brief Source file used to change input image.
    @author Di Nardo Di Maio Raffaele
*/

#include "Test.h"

using namespace cv;

void show_image(char* filename)
{
    Mat img = imread(filename);

    if (img.empty())
    {
        std::cout << "Could not open or find the image";
    }
    else
    {
        namedWindow("Burano");
        imshow("Burano", img);
        waitKey(TIMEOUT);
    }
}

void show_imageBW(char* filename)
{
    Mat img = imread(filename);

    if (img.empty())
    {
        std::cout << "Could not open or find the image";
    }
    else
    {
        Mat img_BW;
        cvtColor(img, img_BW, COLOR_RGB2GRAY);
        namedWindow("Burano BW");
        imshow("Burano BW", img_BW);
        waitKey(TIMEOUT);
    }
}

void component_select(char* filename, int channel)
{
    Mat img = imread(filename);

    if (img.empty())
    {
        std::cout << "Could not open or find the image";
    }
    else
    {
        for (int i = 0; i < img.rows; i++)
        {
            for (int j = 0; j < img.cols; j++)
            {
                for (int k = 0; k < NUM_RGB; k++)
                {
                    if (k != channel)
                        img.at<Vec3b>(i, j)[k] = 0;
                }
            }
        }

        char name[6];

        if (channel == 0)
            strcpy(name, "BLUE");
        else if (channel == 1)
            strcpy(name, "GREEN");
        else
            strcpy(name, "RED");

        namedWindow(name);
        imshow(name, img);
        waitKey(TIMEOUT);
    }
}

void gray_variation_diagonal(void)
{
    Mat img(256, 256, CV_8U);

    for (int i = 0; i < img.rows; i++)
        for (int j = 0; j < img.cols; j++)
            img.at<unsigned char> (i,j) = std::min(i + j, 255);
   
    namedWindow("Diagonal");
    imshow("Diagonal", img);
    waitKey(TIMEOUT);
}

void gray_variation_horizontal(void)
{
    Mat img(256, 256, CV_8U);

    for (int i = 0; i < img.rows; i++)
        for (int j = 0; j < img.cols; j++)
            img.at<unsigned char>(i, j) = j;

    namedWindow("Horizontal");
    imshow("Horizontal", img);
    waitKey(TIMEOUT);
}

void gray_variation_vertical(void)
{
    Mat img(256, 256, CV_8U);

    for (int i = 0; i < img.rows; i++)
        for (int j = 0; j < img.cols; j++)
            img.at<unsigned char>(i, j) = i;

    namedWindow("Vertical");
    imshow("Vertical", img);
    waitKey(TIMEOUT);
}

void b2w(char* filename)
{
    Mat img = imread(filename);

    if (img.empty())
    {
        std::cout << "Could not open or find the image";
    }
    else
    {
        for (int i = 0; i < img.rows; i++)
        {
            for (int j = 0; j < img.cols; j++)
            {
                //Tollerance of 70 (<= value of black ((R,G,B)=(0,0,0) + 70) 
                int checkB = img.at<Vec3b>(i, j)[0] <= 70 && img.at<Vec3b>(i, j)[0] <= 70 && img.at<Vec3b>(i, j)[0] <= 70;
                
                if (checkB)
                {
                    //(R,G,B)=(255,255,255) WHITE
                    img.at<Vec3b>(i, j)[0] = 255;
                    img.at<Vec3b>(i, j)[1] = 255;
                    img.at<Vec3b>(i, j)[2] = 255;
                }
            }
        }

        namedWindow("B2W");
        imshow("B2W", img);
        waitKey(TIMEOUT);
    }
}

void w2b(char* filename)
{
    Mat img = imread(filename);

    if (img.empty())
    {
        std::cout << "Could not open or find the image";
    }
    else
    {
        for (int i = 0; i < img.rows; i++)
        {
            for (int j = 0; j < img.cols; j++)
            {
                //Tollerance of 70 (>= value of black ((R,G,B)=(255,255,255) - 70) 
                int checkB = img.at<Vec3b>(i, j)[0] >= 185 && img.at<Vec3b>(i, j)[0] >= 185 && img.at<Vec3b>(i, j)[0] >= 185;

                if (checkB)
                {
                    //(R,G,B)=(255,255,255) WHITE
                    img.at<Vec3b>(i, j)[0] = 0;
                    img.at<Vec3b>(i, j)[1] = 0;
                    img.at<Vec3b>(i, j)[2] = 0;
                }
            }
        }

        namedWindow("W2B");
        imshow("W2B", img);
        waitKey(TIMEOUT);
    }
}