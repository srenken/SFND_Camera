#include <iostream>
#include <numeric>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>

using namespace std;

// this function illustrates a very simple non-maximum suppression to extract the strongest corners
// in a local neighborhood around each pixel
std::vector<cv::KeyPoint> PerformNMS(cv::Mat corner_img, int minResponse, bool visualizeCornerImg = false)
{
    // Return
    std::vector<cv::KeyPoint> keypoints;


    // define size of sliding window
    int sw_size = 6;                  // should be odd so we can center it on a pixel and have symmetry in all directions
    int sw_dist = floor(sw_size / 2); // number of pixels to left/right and top/down to investigate

    // create output image
    cv::Mat result_img = cv::Mat::zeros(corner_img.rows, corner_img.cols, CV_8U);

    // loop over all pixels in the corner image
    for (int r = sw_dist; r < corner_img.rows - sw_dist - 1; r++) // rows
    {
        for (int c = sw_dist; c < corner_img.cols - sw_dist - 1; c++) // cols
        {

            int response = (int)corner_img.at<float>(r, c);
            std::cout << response << std::endl;
            if (response > minResponse)
            {
                // loop over all pixels within sliding window around the current pixel 
                // -->> to find the local max value
                unsigned int max_val{0}; // keeps track of strongest response
                for (int rs = r - sw_dist; rs <= r + sw_dist; rs++)
                {
                    for (int cs = c - sw_dist; cs <= c + sw_dist; cs++)
                    {
                        // check wether max_val needs to be updated
                        unsigned int new_val = (int)corner_img.at<float>(rs, cs);
                        max_val = max_val < new_val ? new_val : max_val;
                    }
                }

                // check wether current pixel is local maximum
                if (response == max_val)
                {
                    
                        result_img.at<unsigned int>(r, c) = max_val;
                        cv::KeyPoint keypoint((float)c, (float)r, (float)sw_size, (float)max_val);
                        keypoints.push_back(keypoint);
                    
                    
                }
            }
            
                
        }
    }
  	  
    if (visualizeCornerImg)
    {
        // visualize results
        std::string windowName = "NMS Result Image";
        cv::namedWindow(windowName, 5);
        cv::imshow(windowName, result_img);
    }
    
  
    return keypoints;
}


void cornernessHarris()
{
    // load image from file
    cv::Mat img;
    img = cv::imread("../images/img1.png");
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY); // convert to grayscale

    // Detector parameters
    int blockSize = 2;     // for every pixel, a blockSize × blockSize neighborhood is considered
    int apertureSize = 3;  // aperture parameter for Sobel operator (must be odd)
    int minResponse = 100; // minimum value for a corner in the 8bit scaled response matrix
    double k = 0.04;       // Harris parameter (see equation for details)

    // Detect Harris corners and normalize output
    cv::Mat dst, dst_norm, dst_norm_scaled;
    dst = cv::Mat::zeros(img.size(), CV_32FC1);
    cv::cornerHarris(img, dst, blockSize, apertureSize, k, cv::BORDER_DEFAULT);
    cv::normalize(dst, dst_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat());
    cv::convertScaleAbs(dst_norm, dst_norm_scaled);

    // visualize results
    string windowName = "Raw Harris Corner Detector Response Matrix";

    cv::namedWindow(windowName, 4);
    cv::imshow(windowName, dst);
    

    windowName = "Normalized Harris Corner Detector Response Matrix";
    cv::namedWindow(windowName, 4);
    cv::imshow(windowName, dst_norm);

    windowName = "Harris Corner Detector Response Matrix";
    cv::namedWindow(windowName, 4);
    cv::imshow(windowName, dst_norm_scaled);


    

    // TODO: Your task is to locate local maxima in the Harris response matrix 
    // and perforom a non-maximum suppressin (NMS) in a local neighborhood around 
    // each maximum. The resulting coordinates shall be stored in a list of keypoints 
    // of the type `vector<cv::KeyPoint>`.

    std::vector<cv::KeyPoint> keypoints; 
    keypoints = PerformNMS(dst_norm, minResponse, true);

    //-- Draw keypoints
    cv::Mat img_keypoints;
    cv::drawKeypoints( dst_norm_scaled, keypoints, img_keypoints, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

    windowName = "Harris Corner Detector Response with Keypoints";
    cv::namedWindow(windowName, 4);
    cv::imshow(windowName, img_keypoints);

    cv::waitKey(0);
    
}



int main()
{
    cornernessHarris();
}