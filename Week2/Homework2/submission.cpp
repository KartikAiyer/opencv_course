//
// Created by kartik on 1/23/21.
//

#include <iostream>
#include <opencv2/opencv.hpp>

static cv::Point2d s_point1, s_point2;
static cv::Mat s_img;

void drawBoundingBox(int action, int x, int y, int flags, void *userdata)
{
  if (action == cv::EVENT_LBUTTONDOWN) {
    s_point1.x = x;
    s_point1.y = y;
  } else if (action == cv::EVENT_LBUTTONUP) {
    s_point2.x = x;
    s_point2.y = y;
    cv::rectangle(s_img, s_point1, s_point2, cv::Scalar{0, 255, 0}, 2, cv::LINE_AA);
    cv::imshow("Window", s_img);
    cv::Mat crop = s_img(cv::Range(s_point1.y, s_point2.y), cv::Range(s_point1.x, s_point2.x));
    cv::imwrite(DATA_PATH "/cropped_sample.jpg", crop);
    cv::imshow("Cropped", crop);
  }
}

int main(int argc, char **argv)
{
  s_img = cv::imread(DATA_PATH "/sample.jpg");
  if (s_img.data) {
    cv::namedWindow("Window");
    cv::setMouseCallback("Window", drawBoundingBox);
    int k = 0;
    while (k != 27) {
      cv::putText(s_img,
                  "Click and Drag a Bounding box around the face, press Esc to exit",
                  cv::Point(10, s_img.rows - 20),
                  cv::FONT_HERSHEY_SIMPLEX,
                  0.45,
                  cv::Scalar(255, 255, 255),
                  1, cv::LINE_AA);
      cv::imshow("Window", s_img);
      k = cv::waitKey() & 0xFF;
    }
  } else {
    std::cout << "Couldn't open sample.jpg" << std::endl;
  }
  return 0;
}
