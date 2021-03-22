//
// Created by Kartik Aiyer on 3/22/21.
//


#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char** argv)
{
  std::string filename{DATA_PATH "/blemish.png"};
  cv::Mat image = cv::imread(filename);
  cv::imshow("blemish", image);
  cv::waitKey();
  return 0;
}