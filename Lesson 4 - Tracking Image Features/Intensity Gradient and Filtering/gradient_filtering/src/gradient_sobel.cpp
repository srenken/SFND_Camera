#include <iostream>
#include <numeric>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;

void gradientSobel()
{
    // TODO: Based on the image gradients in both x and y, compute an image 
    // which contains the gradient magnitude according to the equation at the 
    // beginning of this section for every pixel position. Also, apply different 
    // levels of Gaussian blurring before applying the Sobel operator and compare the results.

  	// Step 1 - load image from file
    cv::Mat img;
    img = cv::imread("../images/img1.png");

    //  Step 2 - convert image to grayscale
    cv::Mat imgGray;
    cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);

    // 

    //  Step 3 - create filter kernel
    float sobel_x[9] = {-1, 0, +1,
                        -2, 0, +2, 
                        -1, 0, +1};
    cv::Mat kernel_x = cv::Mat(3, 3, CV_32F, sobel_x);

    float sobel_y[9] = {-1, -2, -1,
                         0, 0, 0,
                        +1, +2, +1};
    cv::Mat kernel_y = cv::Mat(3, 3, CV_32F, sobel_y);


    //  Step 4a - apply filter
    cv::Mat result_x;
    cv::filter2D(imgGray, result_x, -1, kernel_x, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);

    cv::Mat result_y;
    cv::filter2D(imgGray, result_y, -1, kernel_y, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);

    // Combine gradients in x and y direction
    cv::Mat result_x_and_y_gradient;
    cv::addWeighted(result_x, 0.5, result_y, 0.5, 0, result_x_and_y_gradient);

    //  Step 5a - show result
    string windowName = "Sobel operator (x-direction)";
    cv::namedWindow( windowName, 1 ); // create window 
    cv::imshow(windowName, result_x);

    windowName = "Sobel operator (y-direction)";
    cv::namedWindow( windowName, 1 ); // create window 
    cv::imshow(windowName, result_y);

    windowName = "Sobel operator (x and y-direction)";
    cv::namedWindow( windowName, 1 ); // create window 
    cv::imshow(windowName, result_x_and_y_gradient);




    // +++ Apply gaussian blur before Sobel operator +++

    cv::Mat imgGray_GaussianBlur;
    cv::GaussianBlur(imgGray, imgGray_GaussianBlur, cv::Size(5,5), 2.00);


    //  Step 4b with Gaussian Blur - apply filter
    
    cv::filter2D(imgGray_GaussianBlur, result_x, -1, kernel_x, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);

    
    cv::filter2D(imgGray_GaussianBlur, result_y, -1, kernel_y, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);

    // Combine gradients in x and y direction
    
    cv::addWeighted(result_x, 0.5, result_y, 0.5, 0, result_x_and_y_gradient);

    // compute magnitude image
    cv::Mat magnitude = imgGray.clone();
    for (int r = 0; r < magnitude.rows; r++)
    {
        for (int c = 0; c < magnitude.cols; c++)
        {
            magnitude.at<unsigned char>(r, c) = sqrt(pow(result_x.at<unsigned char>(r, c), 2) +
                                                     pow(result_y.at<unsigned char>(r, c), 2));
        }
    }


    //  Step 5b - show result with Gaussian Blur
    windowName = "Sobel operator (x-direction) on filtered image (Gaussian Blur)";
    cv::namedWindow( windowName, 1 ); // create window 
    cv::imshow(windowName, result_x);

    windowName = "Sobel operator (y-direction) on filtered image (Gaussian Blur)";
    cv::namedWindow( windowName, 1 ); // create window 
    cv::imshow(windowName, result_y);

    windowName = "Sobel operator (x and y-direction) on filtered image (Gaussian Blur)";
    cv::namedWindow( windowName, 1 ); // create window 
    cv::imshow(windowName, result_x_and_y_gradient);

    windowName = "Sobel operator (x and y-direction) on filtered image (Gaussian Blur) / Gradient Magnitude";
    cv::namedWindow( windowName, 1 ); // create window 
    cv::imshow(windowName, magnitude);

    cv::waitKey(0); // wait for keyboard input before 

}

int main()
{
    gradientSobel();
}