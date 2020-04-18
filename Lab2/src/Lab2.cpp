#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utils/filesystem.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/core/types_c.h>
#include <opencv2/core.hpp>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <limits>

#define LINE "-----------------------------------------------"

using namespace std;
using namespace cv;

void printName(String s);
void calibrate(const vector<String> img_names);

const String pathname = "data/checkerboard_images/";
const String pattern = "*.png";
const int num_cols = 6;
const int num_rows = 5;

int main()
{
    vector<String> img_names;
    utils::fs::glob(pathname, pattern, img_names);

    for_each(img_names.begin(), img_names.end(), printName);
    
    calibrate(img_names);
    return 0;
}

void printName(String s)
{
    static int count = 0;
    cout<<"File"<<(++count)<<": "<<s<<endl;
}

void calibrate(const vector<String> img_names)
{
    vector<vector<Point3f>> corners3D;
    vector<vector<Point2f>> corners2D;
    Size size(num_cols, num_rows);
    cout << "2d" << endl;
    for (int i = 0; i < img_names.size(); i++)
    {
        vector<Point2f> coord2;

        Mat img = imread(img_names[i], IMREAD_COLOR);
        cvtColor(img, img, COLOR_RGB2GRAY);
        bool found = findChessboardCorners(img, size, coord2, 0);

        if (found)
            cornerSubPix(img, coord2, Size(5, 5), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));

        drawChessboardCorners(img, size, Mat(coord2), found);

        corners2D.push_back(coord2);

        //resize(img, img, Size(img.cols / 2, img.rows / 2));
        //namedWindow("image", WINDOW_AUTOSIZE);
        //imshow("image", img);
        //waitKey(0);
    }

    cout << "3d" << endl;

    vector<Point3f> coord3;
    for (int i = 0; i < num_rows; i++)
    {
        for (int j = 0; j < num_cols; j++)
        {
            Point3f p = Point3f(j * 0.11, i * 0.11, 0);
            coord3.push_back(p);
        }
    }

    for (int i = 0; i<img_names.size(); i++)
    {
        corners3D.push_back(coord3);
    }


    Mat K = Mat::eye(3,3, CV_64F);
    vector<float> distCoeffs;
    vector<Mat> rvecs, tvecs;

    Mat img = imread(img_names[0], IMREAD_COLOR);
    cout << "calib" << endl;
    calibrateCamera(corners3D, corners2D, img.size(), K, distCoeffs, rvecs, tvecs, 0);

    //at(i,j) i=y=row j=x=column
    cout << endl;
    cout << "Intrinsics parameters" << endl;
    cout << LINE << endl;
    cout << "Alpha_u: " << K.at<double>(0,0) << endl;
    cout << "Alpha_v: " << K.at<double>(1, 1) << endl;
    cout << "u_c: " << K.at<double>(0, 2) << endl;
    cout << "v_c: " << K.at<double>(1, 2) << endl;
    cout << LINE << endl;
    cout << "Radial Distortion" << endl;
    cout << "k1: " << distCoeffs[0] << endl;
    cout << "k2: " << distCoeffs[1] << endl;
    cout << "k3: " << distCoeffs[4] << endl;
    cout << "Tangential Distortion" << endl;
    cout << "p1: " << distCoeffs[2] << endl;
    cout << "p2: " << distCoeffs[3] << endl;
    cout << LINE << endl;
    cout << endl;

    double best_err = numeric_limits<double>::max();
    double worst_err = 0.0;
    double mean_err = 0.0;
    int best = 0;
    int worst = 0;

    for (int i=0; i<img_names.size(); i++)
    {
        vector<Point2f> output;
        projectPoints(corners3D[i], rvecs[i], tvecs[i], K, distCoeffs, output);
        
        double error=0.0;
        for (int j = 0; j < corners3D[i].size(); j++)
        {
            Point2f dist = corners2D[i][j]-output[j];
            error += sqrt((dist.x*dist.x) + (dist.y*dist.y));            
        }
        
        error /= corners3D[i].size();
        mean_err += error;

        if (error < best_err)
        {
            best_err = error;
            best = i;
        }
        else if(error > worst_err)
        {
            worst_err = error;
            worst = i;
        }
    }

    mean_err /= img_names.size();
    cout << "Mean error: " << mean_err << endl;
    cout << "Error: "<< worst_err << "  Worst: " << img_names[worst] << endl;
    cout << "Error: "<< best_err << "   Best: " << img_names[best] << endl;

    img = imread("data/test_image.png", IMREAD_COLOR);
    Mat img1 = imread("data/test_image.png", IMREAD_COLOR);
    Mat m1 = Mat::eye(img.size(), CV_32FC1);
    Mat m2 = Mat::eye(img.size(), CV_32FC1);
    Mat new_K;
    initUndistortRectifyMap(K, distCoeffs, Mat(), new_K, img.size(), CV_32FC1, m1, m2);
    remap(img, img, m1, m2, INTER_LINEAR);

    resize(img, img, Size(img.cols / 3, img.rows / 3));
    resize(img1, img1, Size(img1.cols / 3, img1.rows / 3));
    
    Mat m;
    hconcat(img, img1, m);
    namedWindow("image", WINDOW_AUTOSIZE);
    imshow("image", m);
    waitKey(0);

}