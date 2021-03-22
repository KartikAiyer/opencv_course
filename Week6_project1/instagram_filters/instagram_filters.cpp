// // Created by Kartik Aiyer on 3/21/21.
//


#include <opencv2/opencv.hpp>
#include <iostream>
#include <exception>
#include <my_cv_utils.h>
#include <filesystem>

namespace fs = std::filesystem;

cv::Mat pencilSketch(cv::Mat image, float scale = 1, float delta = 0, int argument = 0)
{
  cv::Mat image_copy;
  cv::medianBlur(image, image_copy, 7);
  cv::cvtColor(image, image_copy, cv::COLOR_BGR2GRAY );
  try
  {
    cv::Laplacian(image_copy, image_copy, -1, 5, scale, delta);
    cv::bitwise_not(image_copy, image_copy);
  }catch(std::exception& exp)
  {
    std::cout << exp.what() << std::endl;
  }
  return image_copy;
}
cv::Mat cartoonify(cv::Mat image,
                   float sigma_s=10,
                   float sigma_r = .15f,
                   float scale = 1,
                   float delta = 0,
                   int arguments=0){

  cv::Mat cartoonImage = image.clone();
  cv::edgePreservingFilter(cartoonImage, cartoonImage, cv::RECURS_FILTER, sigma_s, sigma_r);
  cv::Mat pencil_sketch = pencilSketch(image, scale, delta);
  cv::cvtColor(pencil_sketch, pencil_sketch, cv::COLOR_GRAY2BGR);
  std::cout << pencil_sketch.size() << " " << pencil_sketch.channels() << std::endl;
  std::cout << cartoonImage.size() << " " << cartoonImage.channels() << std::endl;
  cv::addWeighted(pencil_sketch, 0.4, cartoonImage, 1, -50, cartoonImage);
  return cartoonImage;
}

int main(int argc, char **argv)
{
  cv::Mat image;
  std::string filename{DATA_PATH "/trump.jpg"};
  std::cout << filename << std::endl;
  image = cv::imread(filename);
  cv::Mat pencil_image = pencilSketch(image, 1, -120);
  cv::Mat cartoon_image = cartoonify(image, 10, .45, 1, -120);

  cv::imshow("pencil", pencil_image);
  cv::imshow("cartoon", cartoon_image);
  cv::waitKey();

  #if 0
  std::string cartoon_file{DATA_PATH "/trump_cartoon.mp4"};
  if(fs::exists(cartoon_file)) fs::remove(cartoon_file);
  std::string pencil_file{DATA_PATH "/trump_pencil.mp4"};
  if(fs::exists(pencil_file)) fs::remove(pencil_file);
  auto fps = 15;
  cv::VideoWriter cartoon_writer(cartoon_file,
                                 cv::VideoWriter::fourcc('M','J','P','G'),
                                 fps,
                                 image.size());
  cv::VideoWriter pencil_writer(pencil_file,
                                cv::VideoWriter::fourcc('M','J','P','G'),
                                fps,
                                image.size());
  for(uint32_t i = 0; i < 100 ; i++)
  {
    float sigma_r = 0.01 + i*0.005;
    float sigma_s = 60;
    float scale = 1;
    float delta = -128 + (i * 0.5);
    cartoon_writer << cartoonify(image, sigma_s, sigma_r);
    pencil_writer << pencilSketch(image, scale, delta);
  }
  cartoon_writer.release();
  pencil_writer.release();
  #endif
}
