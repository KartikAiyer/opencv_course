//
// Created by kartik on 1/24/21.
//

#include <opencv2/opencv.hpp>
#include <iostream>


static inline void display_resized(cv::Mat& img, float scale, const std::string& window_name)
{
  cv::Mat temp;
  cv::resize(img, temp, cv::Size{}, scale, scale, cv::INTER_NEAREST);
  cv::imshow(window_name, temp * 255);
  cv::waitKey();
}

void my_dilate(cv::Mat& src, cv::Mat& dst, cv::Mat element)
{
  int ksize = element.size().height;
  int height, width;

  height = src.size().height;
  width = src.size().width;

  std::string output_file{DATA_PATH "/output.avi"};
  cv::VideoWriter writer{output_file,
                         cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
                         10,
                         cv::Size{50,50}};

  int border = ksize / 2;
  cv::Mat padded_image = cv::Mat::zeros(cv::Size(height + border * 2, width + border * 2), CV_8UC1);
  copyMakeBorder(src,
                 padded_image,
                 border, border, border, border,
                 cv::BORDER_CONSTANT,
                 0);
  cv::Mat bitAnd;
  src.copyTo(dst);
  for(uint32_t h = border; h < height + border; h++) {
    for(uint32_t w = border; w < width + border; w++) {
      auto hood = padded_image(cv::Range(h - border, h + border + 1),
                      cv::Range(w-border, w+border + 1));
      cv::bitwise_and(hood,
                      element,
                      bitAnd);
      std::cout << hood << " AND " << std::endl << element << " IS " << std::endl << bitAnd << std::endl;
      double min = 0, max = 0;
      cv::minMaxLoc(bitAnd, &min, &max);
      dst.at<uchar>(h-border, w-border) = max;
      cv::Mat temp;
      cv::resize(dst, temp, cv::Size(), 5, 5, cv::INTER_NEAREST);
      temp *= 255;
      cv::cvtColor(temp, temp, cv::COLOR_GRAY2BGR);
      writer << temp;
    }
  }
  display_resized(dst, 10, "Image");
  writer.release();
}

void my_erode(cv::Mat& src, cv::Mat& dst, cv::Mat element)
{
  int ksize = element.size().height;
  int height, width;

  height = src.size().height;
  width = src.size().width;

  std::string output_file{DATA_PATH "/eroded.avi"};
  cv::VideoWriter writer{output_file,
                         cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
                         10,
                         cv::Size{50,50}};

  int border = ksize / 2;
  cv::Mat padded_image;// = cv::Mat::zeros(cv::Size(height + border * 2, width + border * 2), CV_8UC1);
  copyMakeBorder(src,
                 padded_image,
                 border, border, border, border,
                 cv::BORDER_CONSTANT,
                 0);
  cv::Mat bitAnd;
  dst = src.clone();
  for(uint32_t h_i = border; h_i < border + height; h_i++) {
    for(uint32_t w_i = border; w_i < border + width; w_i++) {
      auto hood = padded_image(cv::Range(h_i - border, h_i + border + 1),
                               cv::Range(w_i - border, w_i + border + 1));
      std::cout << "Hood = " << hood << std::endl;
      cv::bitwise_and(hood, element, bitAnd);

      double min = 0, max = 0;
      cv::minMaxLoc(bitAnd, &min, &max, 0, 0, element);
      std::cout <<"Min = " << min << " Max = " << max << std::endl;
      if((int)min == 0 && (int)max == 1) {
        dst.at<uchar>(h_i - border, w_i - border) = 0;
      }
      cv::Mat temp;
      cv::resize(dst, temp, cv::Size(), 5, 5, cv::INTER_NEAREST);
      cv::cvtColor(temp * 255, temp, cv::COLOR_GRAY2BGR);
      writer << temp;
    }
  }
  display_resized(dst, 10, "Image");
  writer.release();
}

int main(int argc, char** argv)
{
  cv::Mat demo;

  cv::Mat demoImage = cv::Mat::zeros(cv::Size(10, 10), CV_8U);

  demoImage.at<uchar>(0, 1) = 1;
  demoImage.at<uchar>(9, 0) = 1;
  demoImage.at<uchar>(8, 9) = 1;
  demoImage.at<uchar>(2, 2) = 1;
  demoImage(cv::Range(5, 8), cv::Range(5, 8)).setTo(1);


  cv::Mat element = getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));
  cv::Mat my_dilated;
  my_dilate(demoImage, my_dilated, element);

  cv::Mat opencv_dilated;
  cv::dilate(demoImage, opencv_dilated, element);
  display_resized(opencv_dilated, 10, "Compare");

  cv::Mat my_eroded;
  my_erode(demoImage, my_eroded, element);

  cv::Mat opencv_eroded;
  cv::erode(demoImage, opencv_eroded, element);
  display_resized(opencv_eroded, 10, "Compare");
  return 0;
}
