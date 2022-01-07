#include <iostream>
#include <numeric>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <array>
#include <cmath>

using namespace std;

void gaussianSmoothing1()
{
    // load image from file
    cv::Mat img;
    img = cv::imread("../images/img1gray.png");

    // create filter kernel
    int gauss_data_size = 25;
    float gauss_data[gauss_data_size] = {1, 4, 7, 4, 1,
                            4, 16, 26, 16, 4,
                            7, 26, 41, 26, 7,
                            4, 16, 26, 16, 4,
                            1, 4, 7, 4, 1};

    int kernel_sum = accumulate(gauss_data, gauss_data + gauss_data_size, 0);

    for(int i = 0; i < gauss_data_size; i++)
        gauss_data[i] = gauss_data[i] / kernel_sum;

    cv::Mat kernel = cv::Mat(5, 5, CV_32F, gauss_data);

    /* cv::Mat kernel = (cv::Mat_<char>(3,3) <<  0, -1,  0,
                                   -1,  45, -1,
                                    0, -1,  0); */

    string windowNameOrg = "Gaussian Blurring Original";
    cv::namedWindow(windowNameOrg, 1); // create window
    cv::imshow(windowNameOrg, img);

    // apply filter
    cv::Mat result;
    cv::filter2D(img, result, img.depth(), kernel);

    // show result
    string windowName = "Gaussian Blurring with own Kernel";
    cv::namedWindow(windowName, 1); // create window
    cv::imshow(windowName, result);
    //cv::waitKey(0); // wait for keyboard input before continuing

    // show result with cv::gau

    cv::GaussianBlur(img, result, cv::Size(5,5), 1.00);
    

    string windowName3 = "Gaussian Blurring with OpenCV Buildin function";
    cv::namedWindow(windowName3, 1); // create window
    cv::imshow(windowName3, result);
    cv::waitKey(0); // wait for keyboard input before continuing

}

int main()
{
    gaussianSmoothing1();
}