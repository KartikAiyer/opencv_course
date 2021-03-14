//
// Created by kartik on 2/27/21.
//

#include <opencv2/opencv.hpp>
#include <utility>

cv::Mat gaussian_denoise(cv::Mat& image, cv::Size kernel_size)
{
  cv::Mat output;
  cv::GaussianBlur(image, output, std::move(kernel_size), 1, 1);
  return output;
}

int main()
{
  cv::Mat image = cv::imread(IMAGES_PATH "/gaussian-noise.png");
  cv::imshow("Original", image);
  cv::imshow("Denoised", gaussian_denoise(image, cv::Size{5, 5}));
  cv::waitKey();
  return 0;
}