//
// Created by Kartik Aiyer on 3/22/21.
//

#include <opencv2/opencv.hpp>
#include <filesystem>

namespace fs = std::filesystem;
int main(int argc, char** argv)
{
  cv::VideoCapture cap{0};
  std::string filename{DATA_PATH "/temp.mp4"};
  if(fs::exists(filename)) fs::remove(filename);
  cv::VideoWriter writer{DATA_PATH "/temp.mp4",
                         cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
                         15,
                         cv::Size(1280,720)};
  char key = 0;
  while(key != 'q')
  {
    cv::Mat frame;
    cap >> frame;
    cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY );
    cv::medianBlur(frame,frame, 3);
    cv::Laplacian(frame, frame, -1, 5, 1);
    cv::bitwise_not(frame, frame);
    writer << frame;
    cv::imshow("output", frame);
    key = cv::waitKey(10);
  }
  writer.release();
  cap.release();
  return 0;
}
