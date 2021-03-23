//
// Created by Kartik Aiyer on 3/22/21.
//


#include <opencv2/opencv.hpp>
#include <iostream>
#include <algorithm>

cv::Mat get_patch(cv::Mat image, int x, int y, int radius = 15)
{
  int startx = cv::max(0, x - radius);
  int starty = cv::max(0, y - radius);
  int stopx = cv::min(startx + (radius * 2), image.cols);
  int stopy = cv::min(starty + (radius * 2), image.rows);

  return image(cv::Range(starty, stopy), (cv::Range(startx, stopx)));
}

cv::Mat get_patch(cv::Mat image, cv::Point center, int radius = 15)
{
  return get_patch(image, center.x, center.y, radius);
}

cv::Mat find_best_patch(cv::Mat image, cv::Point center, int radius)
{
  cv::Point neighbors[] = {cv::Point{-radius, -radius},
                           cv::Point{-radius, 0},
                           cv::Point{-radius, radius},
                           cv::Point{0, radius},
                           cv::Point{radius, radius},
                           cv::Point{radius, 0},
                           cv::Point{radius, -radius},
                           cv::Point{0, -radius}};
  double smooth_score = DBL_MAX;
  cv::Mat minpatch;
  for(uint32_t i = 0; i < 8; i++)
  {
    cv::Mat patch = get_patch(image, center + neighbors[i], radius);
    cv::Mat gradx, grady, grad;

    cv::Sobel(patch, gradx, CV_32F, 1, 0);
    cv::Sobel(patch, grady, CV_32F, 0 ,1);

    cv::convertScaleAbs(gradx, gradx);
    cv::convertScaleAbs(grady, grady);
    cv::addWeighted(gradx, 0.5, grady, 0.5, 0, grad);
    double score = cv::mean(grad).val[0];

    if(score < smooth_score) {
      smooth_score = score;
      minpatch = patch;
    }
  }
  return minpatch;
}

cv::Mat deblemish(cv::Mat image, cv::Point center, int radius = 15)
{
  cv::Mat image_gray;
  cv::cvtColor(image, image_gray, cv::COLOR_BGR2GRAY);
  cv::Mat patch = find_best_patch(image, center, radius);
  cv::Rect patch_roi{0, 0, patch.size().width, patch.size().height};
  cv::Mat patch_color = image(patch_roi);
  cv::Mat mask = 255 * cv::Mat::ones(patch_roi.size(), CV_8U);
  cv::seamlessClone(patch_color, image, mask, center, image, cv::NORMAL_CLONE);

  return image;
}
cv::Mat s_image;
int main(int argc, char** argv)
{
  // Please note that DATA_PATH should point to the folder than contains the test file.
  // Recommend adding a compile definition using `target_compile_definitions` with the
  // macro defined using -DDATA_PATH="<path to folder containing image>"
  std::string filename{DATA_PATH "/blemish.png"};
  s_image = cv::imread(filename);
  const int radius = 15;

  cv::imshow("deblemish", s_image);
  auto win = cv::getWindowImageRect("deblemish");
  cv::imshow("output", s_image);
  cv::moveWindow("output", win.x + win.width, win.y);
  cv::setMouseCallback("deblemish",
                       [](int event, int x, int y, int flags, void* userdata) {
    if(event == cv::EVENT_LBUTTONDBLCLK) {
      auto grad = deblemish(s_image, cv::Point{x, y});
      cv::imshow("output", grad);
    }
  }, 0);
  auto key = cv::waitKey();

  return 0;
}