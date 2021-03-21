//
// Created by kartik on 3/13/21.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <my_cv_utils.h>

double var_abs_laplacian(cv::Mat image)
{
  cv::Mat copy;
  cv::cvtColor(image, copy, cv::COLOR_BGR2GRAY);
  copy.convertTo(copy, CV_32FC1);
  cv::Mat lap;
  cv::Laplacian(copy, lap, -1, 3);
  cv::Mat mean, stddev;
  cv::meanStdDev(cv::abs(lap), mean, stddev);
  cv::pow(stddev, 2, stddev);
  return stddev.at<double>(0, 0);
}

double sum_modified_laplacian(cv::Mat image)
{
  cv::Mat copy;
  cv::cvtColor(image, copy, cv::COLOR_BGR2GRAY);
  copy.convertTo(copy, CV_32F);
  cv::Mat lapX, lapY, lap;

  cv::Sobel(copy, lapX, -1, 2, 0, 3);
  cv::Sobel(copy, lapY, -1, 0, 2, 3);
  cv::add(cv::abs(lapX), cv::abs(lapY), lap);
  return cv::sum(lap)[0];
}

int main()
{
  // Read input video filename
  std::string filename = IMAGES_PATH "/../videos/focus-test.mp4";

// Create a VideoCapture object
  cv::VideoCapture cap(filename);

// Read first frame from the video
  cv::Mat frame;
  cap >> frame;

// Display total number of frames in the video
  std::cout << "Total number of frames : " << (int) cap.get(cv::CAP_PROP_FRAME_COUNT) << std::endl;

  double maxV1 = 0;
  double maxV2 = 0;

// Frame with maximum measure of focus
// Obtained using methods 1 and 2
  cv::Mat bestFrame1;
  cv::Mat bestFrame2;

// Frame ID of frame with maximum measure
// of focus
// Obtained using methods 1 and 2
  int bestFrameId1 = 0;
  int bestFrameId2 = 0;

// Get measures of focus from both methods
  double val1 = var_abs_laplacian(frame);
  double val2 = sum_modified_laplacian(frame);

// Specify the ROI for flower in the frame
// UPDATE THE VALUES BELOW
  int topCorner = 41;
  int leftCorner = 386;
  int bottomCorner = topCorner + (647 - topCorner);
  int rightCorner = leftCorner + (1014 - leftCorner);

  cv::Mat flower;
  flower = frame(cv::Range(topCorner, bottomCorner), cv::Range(leftCorner, rightCorner));

//  cv::imshow("flower", flower);
//  cv::setMouseCallback("flower", [](int event, int x, int y, int flags, void* client_data){
//    if(event == cv::EVENT_LBUTTONDBLCLK)
//    {
//      std::cout<<"("<<x<<","<<y<<")"<<std::endl;
//    }
//  });
//  cv::waitKey();

// Iterate over all the frames present in the video
  while (1) {
    // Crop the flower region out of the frame
    flower = frame(cv::Range(topCorner, bottomCorner), cv::Range(leftCorner, rightCorner));
    // Get measures of focus from both methods
    val1 = var_abs_laplacian(flower);
    val2 = sum_modified_laplacian(flower);
    // If the current measure of focus is greater
    // than the current maximum
    if (val1 > maxV1) {
      // Revise the current maximum
      maxV1 = val1;
      // Get frame ID of the new best frame
      bestFrameId1 = (int) cap.get(cv::CAP_PROP_POS_FRAMES);
      // Revise the new best frame
      bestFrame1 = frame.clone();
      std::cout << "Frame ID of the best frame [Method 1]: " << bestFrameId1 << std::endl;
    }
    // If the current measure of focus is greater
    // than the current maximum
    if (val2 > maxV2) {
      // Revise the current maximum
      maxV2 = val2;
      // Get frame ID of the new best frame
      bestFrameId2 = (int) cap.get(cv::CAP_PROP_POS_FRAMES);
      // Revise the new best frame
      bestFrame2 = frame.clone();
      std::cout << "Frame ID of the best frame [Method 2]: " << bestFrameId2 << std::endl;
    }
    cap >> frame;
    if (frame.empty())
      break;
  }

  std::cout << "================================================" << std::endl;

// Print the Frame ID of the best frame
  std::cout << "Frame ID of the best frame [Method 1]: " << bestFrameId1 << std::endl;
  std::cout << "Frame ID of the best frame [Method 2]: " << bestFrameId2 << std::endl;

  cap.release();

  cv::Mat out;
  hconcat(bestFrame1, bestFrame2, out);
  return 0;
}