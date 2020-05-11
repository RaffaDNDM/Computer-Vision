/**
	@file Calibration.cpp
	@brief Camera Calibration.
	@author Di Nardo Di Maio Raffaele 1204879
*/
#include "Calibration.h"

void calibrate(const std::vector<cv::String> chess_imgs, const std::vector<cv::String> test_imgs)
{
    //Detection of chessboard corners in input image
    std::cout << phase_2D << std::endl;
    std::vector<std::vector<cv::Point2f>> corners2D;
    corners2D_detect(chess_imgs, corners2D);

    //Definition of position of corners in 3D space w.r.t chessboard
    std::cout << phase_3D << std::endl;
    std::vector<std::vector<cv::Point3f>> corners3D;
    int imgs_num = chess_imgs.size();
    corners3D_definition(imgs_num, corners3D);

    //Calibration of camera
    cv::Mat K = cv::Mat::eye(3, 3, CV_64F);
    std::vector<float> distCoeffs;
    std::vector<cv::Mat> rvecs, tvecs;
    std::cout << phase_calibration << std::endl;
    cv::Mat img = cv::imread(chess_imgs[0], cv::IMREAD_COLOR);
    calibrateCamera(corners3D, corners2D, img.size(), K, distCoeffs, rvecs, tvecs, 0);

    //Calibration parameters
    print_calibration(K, distCoeffs);

    //Computation of mean reprojection error w.r.t. all input images
    error(chess_imgs, corners3D, corners2D, rvecs, tvecs, K, distCoeffs);

    for (int i = 0; i < test_imgs.size(); i++)
    {
        std::cout << "Test" << i + 1 << ": " << test_imgs[i] << std::endl;
        std::cout << LINE << std::endl;
        cv::Mat img = cv::imread(test_imgs[i], cv::IMREAD_COLOR);

        //Rectify the test image
        cv::Mat m = rectify_image(img, K, distCoeffs);

        //Comparison between original image (on the left) and rectified image (on the right)
        cv::namedWindow(window_name +"("+test_imgs[i]+")", cv::WINDOW_AUTOSIZE);
        cv::imshow(window_name+"(" + test_imgs[i] + ")", m);
        cv::waitKey(0);
        cv::destroyAllWindows();
    }
}

void corners2D_detect(const std::vector<cv::String> img_names, std::vector<std::vector<cv::Point2f>>& corners2D)
{
    cv::Size chessboard_size(num_cols, num_rows);

    for (int i = 0; i < img_names.size(); i++)
    {
        std::vector<cv::Point2f> coord2;

        cv::Mat img = cv::imread(img_names[i], cv::IMREAD_GRAYSCALE);

        bool found = cv::findChessboardCorners(img, chessboard_size, coord2, 0);

        //Improvement of precision for found corners
        if (found)
            cv::cornerSubPix(img, coord2, cv::Size(5, 5), cv::Size(-1, -1), cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));

        /*
            I used this instruction to store images, used in report

            cv::cvtColor(img, img, cv::COLOR_GRAY2BGR);
            cv::drawChessboardCorners(img, chessboard_size, cv::Mat(coord2), found);
            cv::imwrite("../../../result/chessboard" + std::to_string(i + 1) + ".png", img);
        */

        corners2D.push_back(coord2);
    }
}

void corners3D_definition(const int imgs_num, std::vector<std::vector<cv::Point3f>>& corners3D)
{
    std::vector<cv::Point3f> coord3;

    //Insertion of corners points, adding them by rows
    for (int i = 0; i < num_rows; i++)
    {
        for (int j = 0; j < num_cols; j++)
        {
            cv::Point3f p = cv::Point3f(j * 0.11, i * 0.11, 0.0);
            coord3.push_back(p);
        }
    }

    //Adding all the previous corners points,
    //one for each image
    for (int i = 0; i < imgs_num; i++)
    {
        corners3D.push_back(coord3);
    }
}

void print_calibration(const cv::Mat K, const std::vector<float> distCoeffs)
{
    std::cout << std::endl;
    std::cout << "Intrinsics parameters" << std::endl;
    std::cout << LINE << std::endl;
    std::cout << "Alpha_u: " << K.at<double>(0, 0) << std::endl;
    std::cout << "Alpha_v: " << K.at<double>(1, 1) << std::endl;
    std::cout << "u_c: " << K.at<double>(0, 2) << std::endl;
    std::cout << "v_c: " << K.at<double>(1, 2) << std::endl;
    std::cout << LINE << std::endl;
    std::cout << "Radial Distortion" << std::endl;
    std::cout << "k1: " << distCoeffs[0] << std::endl;
    std::cout << "k2: " << distCoeffs[1] << std::endl;
    std::cout << "k3: " << distCoeffs[4] << std::endl;
    std::cout << "Tangential Distortion" << std::endl;
    std::cout << "p1: " << distCoeffs[2] << std::endl;
    std::cout << "p2: " << distCoeffs[3] << std::endl;
    std::cout << LINE << std::endl;
    std::cout << std::endl;
}

void error(const std::vector<cv::String> img_names, std::vector<std::vector<cv::Point3f>> corners3D,
           const std::vector<std::vector<cv::Point2f>> corners2D, const std::vector<cv::Mat> rvecs,
           const std::vector<cv::Mat> tvecs, const cv::Mat K, const std::vector<float> distCoeffs)
{
    //Best mean reprojection error
    double best_err = std::numeric_limits<double>::max();
    //Index of image with best_err
    int best = 0;

    //Worst mean reprojection error
    double worst_err = 0.0;
    //Index of image with worst_err
    int worst = 0;

    //Weighted avarage of mean reprojection errors of all images
    double mean_err = 0.0;

    for (int i = 0; i < img_names.size(); i++)
    {
        std::vector<cv::Point2f> output;
        projectPoints(corners3D[i], rvecs[i], tvecs[i], K, distCoeffs, output);

        //Mean reprojection error of this image
        double error = 0.0;
        for (int j = 0; j < corners3D[i].size(); j++)
        {
            cv::Point2f dist = corners2D[i][j] - output[j];
            error += sqrt((dist.x * dist.x) + (dist.y * dist.y));
        }

        error /= corners3D[i].size();
        mean_err += error;

        //Update best mean reprojection error
        if (error < best_err)
        {
            best_err = error;
            best = i;
        }
        //update worst mean reprojection error
        else if (error > worst_err)
        {
            worst_err = error;
            worst = i;
        }
    }

    mean_err /= img_names.size();
    std::cout << "Mean error: " << mean_err << std::endl;
    std::cout << "Error: " << worst_err << "  Worst: " << img_names[worst] << std::endl;
    std::cout << "Error: " << best_err << "   Best: " << img_names[best] << std::endl;
    std::cout << LINE << std::endl;
}

cv::Mat rectify_image(cv::Mat img, const cv::Mat K, const std::vector<float> distCoeffs)
{
    static int count_test = 0;
    cv::Mat rect_img = img.clone();
    cv::Mat m1 = cv::Mat::eye(img.size(), CV_32FC1);
    cv::Mat m2 = cv::Mat::eye(img.size(), CV_32FC1);
    cv::Mat new_K;

    //Rectify the image, using previously computed parameters with calibration
    initUndistortRectifyMap(K, distCoeffs, cv::Mat(), new_K, img.size(), CV_32FC1, m1, m2);
    remap(rect_img, rect_img, m1, m2, cv::INTER_LINEAR);

    //Concatenate original image and rectified image
    cv::Mat m;
    resize(img, img, cv::Size(img.cols / 3, img.rows / 3));
    resize(rect_img, rect_img, cv::Size(rect_img.cols / 3, rect_img.rows / 3));
    hconcat(img, rect_img, m);

    /*
        I used this instruction to print the image for the report
        cv::imwrite("../../../result/comparison" + std::to_string(++count_test) + ".png", m);
    */

    cv::imwrite("../../../result/comparison" + std::to_string(++count_test) + ".png", m);
    return m;
}
