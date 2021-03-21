//
// Created by kartik on 1/24/21.
//

#include <opencv2/opencv.hpp>
#include <iostream>

int maxScaleUp = 100;
int scaleFactor = 1;
int scaleType = 0;
int maxType = 1;
cv::Mat im;

std::string windowName = "Resize Image";
std::string trackbarValue = "Scale";
std::string trackbarType = "Type: \n 0: Scale Up \n 1: Scale Down";

void scaleImage(int pos, void *userdata);

int main(int argc, char **argv)
{
  im = cv::imread(DATA_PATH "/truth.png");
  cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
  cv::createTrackbar(trackbarValue, windowName, &scaleFactor, maxScaleUp, scaleImage);
  cv::createTrackbar(trackbarType, windowName, &scaleType, maxType, scaleImage);

  scaleImage(25, 0);

  while (true) {
    int c;
    c = cv::waitKey(20);
    if (static_cast<char>(c) == 27)
      break;
  }
  return 0;
}

void scaleImage(int pos, void *userdata)
{
  double scaleFactorDouble;
  if (scaleType) {
    scaleFactorDouble = 1 - scaleFactor / 100.0;
  } else {
    scaleFactorDouble = 1 + scaleFactor / 100.0;
  }
  if (scaleFactorDouble == 0) {
    scaleFactorDouble = (scaleType) ? 0.1 : 1;
  }
  cv::Mat scaledImage;
  cv::resize(im,
             scaledImage,
             cv::Size(),
             scaleFactorDouble,
             scaleFactorDouble,
             (scaleType) ? cv::INTER_AREA : cv::INTER_LINEAR);
  cv::imshow(windowName, scaledImage);
}

