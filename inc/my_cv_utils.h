//
// Created by kartik on 2/27/21.
//

#pragma once

#include <opencv2/opencv.hpp>
#include <string.h>

namespace my_cv_utils
{

  std::string type2str(int type)
  {
    std::string r;

    uchar depth = type & CV_MAT_DEPTH_MASK;
    uchar chans = 1 + (type >> CV_CN_SHIFT);

    switch (depth) {
      case CV_8U:
        r = "8U";
        break;
      case CV_8S:
        r = "8S";
        break;
      case CV_16U:
        r = "16U";
        break;
      case CV_16S:
        r = "16S";
        break;
      case CV_32S:
        r = "32S";
        break;
      case CV_32F:
        r = "32F";
        break;
      case CV_64F:
        r = "64F";
        break;
      default:
        r = "User";
        break;
    }

    r += "C";
    r += (chans + '0');

    return r;
  }

  cv::Mat fit_large_to_display(cv::Mat& input, cv::Size fit_to = cv::Size{1280, 720})
  {
    cv::Mat output;
    if(input.size().width > fit_to.width || input.size().height > fit_to.height)
    {
      double dx = (double)fit_to.width / input.size().width;
      double dy =  (double)fit_to.height / input.size().height;
      cv::resize(input,output, cv::Size{0,0}, dx, dy, cv::INTER_AREA);
    } else {
      output = input;
    }
    return output;
  }
};
