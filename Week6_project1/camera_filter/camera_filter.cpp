//
// Created by Kartik Aiyer on 3/22/21.
//

#include <opencv2/opencv.hpp>
#include <filesystem>
#include <optional>

namespace fs = std::filesystem;

cv::Mat get_a_few_frames(cv::VideoCapture &cap, int nframes = 30)
{
  cv::Mat frames;
  if(cap.isOpened())
  {
    for(uint32_t i = 0; i < nframes; i++)
    {
      cap >> frames;
    }
  }
  return frames;
}

cv::Mat get_a_frame(cv::VideoCapture &cap)
{
  cv::Mat frame;
  if(cap.isOpened())
  {
    cap >> frame;
  }
  return frame;
}

void frame_processor(cv::VideoCapture &cap, std::string filename)
{
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
}
struct app_state_t
{
  app_state_t() = default;
  ~app_state_t() = default;
  std::optional<cv::Scalar> color_patch;
};

static app_state_t s_state;
int main(int argc, char** argv)
{
  std::string input_file{ DATA_PATH "/greenscreen-asteroid.mp4"};
  cv::VideoCapture cap{input_file};
  std::string filename{DATA_PATH "/temp.mp4"};
  std::string sample_file(DATA_PATH "/sample_background.jpeg");
  cv::Mat background = cv::imread(sample_file);
  std::string window_name = "green_screen";

  auto frame = get_a_few_frames(cap);
  cv::resize(frame, frame, cv::Size(0,0), 0.5, 0.5);
  cv::resize(background, background, frame.size() );
  cv::Mat hsv_frame;
  cv::flip(frame, frame, 1);
  cv::cvtColor(frame, hsv_frame, cv::COLOR_BGR2HSV);
  cv::Mat mask;

  cv::inRange(hsv_frame,
              cv::Scalar(30, 120, 70),
              cv::Scalar(60, 255, 255),
              mask);
  cv::imshow("frame", frame);
  cv::moveWindow("frame", 0, 20);
  auto window_position = cv::getWindowImageRect("frame");
  std::cout << "Mask Size = " << mask.size()
  << " Frame size = " << frame.size()
  << " Bk Size = " << background.size()
  << std::endl;

  background.copyTo(frame, mask);
  cv::imshow("other", frame);
  cv::waitKey();
  return 0;
}
