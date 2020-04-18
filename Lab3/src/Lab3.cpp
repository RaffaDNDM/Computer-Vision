#include "Lab3.h"

#include "Histogram.h"
#include "Filter.h"

int main()
{
    Mat img = imread("Images/image.jpg", IMREAD_COLOR);
    
    if (img.empty())
        cout << "Could not open or find the image";

    Mat noise_img;
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
                noise_img = histogram_equalization(img);
                break;

            case 2:
            {
                if (noise_img.empty())
                    cout <<"You need to choose first histogram equalization option" << endl;
                else
                   image_filtering(noise_img);
                
                break;
            }
            
            case 3:
            {
                if (noise_img.empty())
                    cout << "You need to choose first histogram equalization option" << endl;
                else
                    morph_operators(noise_img);
                break;
            }

            default:
                cout << "----------No option----------" << endl;
        }
    }

    return 0;
}

void gaussianTrackbar(int value, void* params)
{
    gauss_param *gauss = (gauss_param*) params;
    double sigma_x= STEP_SIGMA * gauss->sigma_x;
    GaussianFilter filter(gauss->src, gauss->kernel_size, sigma_x);

    filter.doFilter();

    gauss->out = filter.getResult();
    imshow("Gaussian image", gauss->out);
}

void medianTrackbar(int value, void* params)
{
    median_param* median = (median_param*)params;
    MedianFilter filter(median->src, median->kernel_size);

    filter.doFilter();

    median->out = filter.getResult();
    imshow("Median image", median->out);
}

void bilateralTrackbar(int value, void* params)
{
    bilateral_param* bilateral = (bilateral_param*)params;
    double sigma_space = STEP_SIGMA * bilateral->sigma_space;
    BilateralFilter filter(bilateral->src, bilateral->sigma_range, sigma_space);

    filter.doFilter();

    bilateral->out = filter.getResult();
    imshow("Bilateral image", bilateral->out);
}

void image_filtering(Mat& img)
{
    gauss_param gauss;
    vector<Mat> imgs = { img.clone(), img.clone(), img.clone() };
    namedWindow("Gaussian image", WINDOW_NORMAL | WINDOW_KEEPRATIO | WINDOW_GUI_EXPANDED);
    gauss.kernel_size = 3;
    gauss.sigma_x = 0;
    gauss.src = img;
    gauss.out = imgs[0];
    createTrackbar("Kernel size", "Gaussian image", &(gauss.kernel_size), 11, gaussianTrackbar, (void *)&gauss);
    createTrackbar("Sigma x", "Gaussian image", &(gauss.sigma_x), 25, gaussianTrackbar, (void*)&gauss);
    imshow("Gaussian image", imgs[0]);
    
    median_param median;
    namedWindow("Median image", WINDOW_NORMAL | WINDOW_KEEPRATIO | WINDOW_GUI_EXPANDED);
    median.kernel_size = 3;
    median.src = img;
    median.out = imgs[1];
    createTrackbar("Kernel size", "Median image", &(median.kernel_size), 11, medianTrackbar, (void*)&median);
    imshow("Median image", imgs[1]);
    
    bilateral_param bilateral;
    namedWindow("Bilateral image", WINDOW_NORMAL | WINDOW_KEEPRATIO | WINDOW_GUI_EXPANDED);
    bilateral.sigma_range = 3;
    bilateral.sigma_space = 3;
    bilateral.src = img;
    bilateral.out = imgs[2];
    createTrackbar("Sigma range", "Bilateral image", &(bilateral.sigma_range), 255, bilateralTrackbar, (void*)&bilateral);
    createTrackbar("Sigma space", "Bilateral image", &(bilateral.sigma_space), 25, bilateralTrackbar, (void*)&bilateral);
    imshow("Bilateral image", imgs[2]);
    waitKey(0);

    destroyAllWindows();
}

struct morph_op {
    int kernel_size;
    Mat img, out_delate, out_erode;
};

void morph_operators(Mat& img)
{
    morph_op morph;
    morph.kernel_size = 3;
    morph.img = img.clone();
    morph.out_delate = img.clone();
    morph.out_erode = img.clone();
    namedWindow("Original image", WINDOW_NORMAL | WINDOW_KEEPRATIO | WINDOW_GUI_EXPANDED);
    createTrackbar("Kernel size", "Original image", &(morph.kernel_size), 5, morphTrackbar, (void*)&morph);
    imshow("Original image", img);
    waitKey(0);

    destroyAllWindows();
}

void morphTrackbar(int value, void* params)
{
    morph_op* morph = (morph_op*)params;

    Mat elem = getStructuringElement(MORPH_RECT,
        Size(2 * morph->kernel_size + 1, 2 * morph->kernel_size + 1),
        Point(morph->kernel_size, morph->kernel_size));

    erode(morph->img, morph->out_erode, elem);
    dilate(morph->img, morph->out_delate, elem);

    namedWindow("Dilate image", WINDOW_NORMAL | WINDOW_KEEPRATIO | WINDOW_GUI_EXPANDED);
    imshow("Dilate image", morph->out_delate);
    namedWindow("Erode image", WINDOW_NORMAL | WINDOW_KEEPRATIO | WINDOW_GUI_EXPANDED);
    imshow("Erode image", morph->out_erode);
}