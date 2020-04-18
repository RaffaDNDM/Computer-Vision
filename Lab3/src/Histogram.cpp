#include "Histogram.h"

Mat histogram_equalization(Mat& img)
{
    Mat img1 = img.clone();
    compute_hist(img1);
    namedWindow("image", WINDOW_NORMAL | WINDOW_KEEPRATIO | WINDOW_GUI_EXPANDED);
    imshow("image", img1);
    waitKey(0);
    destroyWindow("image");

    Mat eq_img = equalization(img1);
    compute_hist(eq_img);
    namedWindow("Equalized image", WINDOW_NORMAL | WINDOW_KEEPRATIO | WINDOW_GUI_EXPANDED);
    imshow("Equalized image", eq_img);
    waitKey(0);

    destroyAllWindows();
    cvtColor(img1, img1, COLOR_RGB2HSV);

    vector<Mat> eq_imgs = hsv_eq(img1);
    for (int i = 0; i < 3; i++)
    {
        cvtColor(eq_imgs[i], eq_imgs[i], COLOR_HSV2RGB);
        namedWindow("HSV Equalized image" + to_string(i), WINDOW_NORMAL | WINDOW_KEEPRATIO | WINDOW_GUI_EXPANDED);
        imshow("HSV Equalized image" + to_string(i), eq_imgs[i]);
        waitKey(0);
        destroyWindow("HSV Equalized image" + to_string(i));
    }

    return eq_imgs[2];
}

void compute_hist(Mat& img)
{
    int bins[] = { 256 };
    float range[] = { 0,256 };
    const float* ranges[] = { range };
    vector<MatND> hists = { MatND(), MatND(), MatND() };
    int channel[] = { 0 };
    vector<Mat> m = { Mat(), Mat(), Mat() };
    split(img, m);

    calcHist(&m[0], 1, channel, Mat(), hists[0], 1, bins, ranges, true, false);
    calcHist(&m[1], 1, channel, Mat(), hists[1], 1, bins, ranges, true, false);
    calcHist(&m[2], 1, channel, Mat(), hists[2], 1, bins, ranges, true, false);

    showHistogram(hists);

    waitKey(0);
}

Mat equalization(Mat& img)
{
    vector<Mat> m = { Mat(), Mat(), Mat() };
    split(img, m);

    Mat eq_img;

    for (int i = 0; i < 3; i++)
        equalizeHist(m[i], m[i]);

    merge(m, eq_img);
    return eq_img;
}

vector<Mat> hsv_eq(Mat& img)
{
    vector<Mat> rgb;
    vector<Mat> eq_imgs;
    split(img, rgb);

    for (int i = 0; i < 3; i++)
    {
        vector<Mat> m;

        for (int j = 0; j < 3; j++)
        {
            if (j == i)
            {
                Mat eq_m;
                equalizeHist(rgb[i], eq_m);
                m.push_back(eq_m);
            }
            else
                m.push_back(rgb[j]);
        }

        Mat eq;
        merge(m, eq);
        eq_imgs.push_back(eq);
    }

    return eq_imgs;
}

void compute_histHSV(Mat& img)
{
    int hbins[] = { 180 };
    int sbins[] = { 256 };
    int vbins[] = { 256 };
    float hrange[] = { 0,180 };
    float srange[] = { 0,256 };
    float vrange[] = { 0,256 };
    const float* hranges[] = { hrange };
    const float* sranges[] = { srange };
    const float* vranges[] = { vrange };
    vector<MatND> hists = { MatND(), MatND(), MatND() };
    int channel[] = { 0 };
    vector<Mat> m = { Mat(), Mat(), Mat() };
    split(img, m);

    calcHist(&m[0], 1, channel, Mat(), hists[0], 1, hbins, hranges, true, false);
    calcHist(&m[1], 1, channel, Mat(), hists[1], 1, sbins, sranges, true, false);
    calcHist(&m[2], 1, channel, Mat(), hists[2], 1, vbins, vranges, true, false);

    showHistogram(hists);

    waitKey(0);
}


// hists = vector of 3 cv::mat of size nbins=256 with the 3 histograms
// e.g.: hists[0] = cv:mat of size 256 with the blue histogram
//       hists[1] = cv:mat of size 256 with the green histogram
//       hists[2] = cv:mat of size 256 with the red histogram
void showHistogram(std::vector<cv::Mat>& hists)
{
    // Min/Max computation
    double hmax[3] = { 0,0,0 };
    double min;
    cv::minMaxLoc(hists[0], &min, &hmax[0]);
    cv::minMaxLoc(hists[1], &min, &hmax[1]);
    cv::minMaxLoc(hists[2], &min, &hmax[2]);

    std::string wname[3] = { "blue", "green", "red" };
    cv::Scalar colors[3] = { cv::Scalar(255,0,0), cv::Scalar(0,255,0),
                             cv::Scalar(0,0,255) };

    std::vector<cv::Mat> canvas(hists.size());

    // Display each histogram in a canvas
    for (int i = 0, end = hists.size(); i < end; i++)
    {
        canvas[i] = cv::Mat::ones(125, hists[0].rows, CV_8UC3);

        for (int j = 0, rows = canvas[i].rows; j < hists[0].rows - 1; j++)
        {
            cv::line(
                canvas[i],
                cv::Point(j, rows),
                cv::Point(j, rows - (hists[i].at<float>(j) * rows / hmax[i])),
                hists.size() == 1 ? cv::Scalar(200, 200, 200) : colors[i],
                1, 8, 0
                );
        }

        cv::imshow(hists.size() == 1 ? "value" : wname[i], canvas[i]);
    }
}