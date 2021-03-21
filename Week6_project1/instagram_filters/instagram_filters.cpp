//
// Created by Kartik Aiyer on 3/21/21.
//


#include <opencv2/opencv.hpp>
#include <iostream>
#include <exception>

int main(int argc, char **argv)
{
  cv::Mat image;
  std::string filename{DATA_PATH "/trump.jpg"};
  std::cout << filename << std::endl;
  image = cv::imread(filename);

  cv::imshow("Image", image);
  cv::waitKey();
}
