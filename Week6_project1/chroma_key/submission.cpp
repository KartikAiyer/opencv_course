//
// Created by Kartik Aiyer on 4/5/21.
//

#include <opencv2/opencv.hpp>
#include <iostream>
#include <optional>
#include <my_cv_utils.h>

struct app_state_t
{
  app_state_t() = default;
  ~app_state_t() = default;

  std::optional<cv::Vec3b> color_patch;
  cv::Mat current_frame;
  int tolerance;
  std::optional<std::pair<int,int>> patch_center;
};

static struct app_state_t s_state;

static cv::Mat get_a_frame(cv::VideoCapture &cap)
{
  cv::Mat frame;
  if(cap.isOpened())
  {
    cap >> frame;
  }
  return frame;
}

static cv::Mat get_patch(cv::Mat image, int x, int y, int radius = 15)
{
  int startx = cv::max(0, x - radius);
  int starty = cv::max(0, y - radius);
  int stopx = cv::min(startx + (radius * 2), image.cols);
  int stopy = cv::min(starty + (radius * 2), image.rows);

  return image(cv::Range(starty, stopy), (cv::Range(startx, stopx)));
}

static void mouse_f(int event, int x, int y, int flags, void* userdata)
{
  if(event == cv::EVENT_LBUTTONDBLCLK)
  {
    std::cout << "Patch Center = " << x << ", " << y << std::endl;
    s_state.patch_center = std::make_pair(x,y);
  }
}

int main(int argc, char** argv)
{
  std::string input_filename;
  std::string background_filename;

  input_filename = DATA_PATH "/greenscreen-asteroid.mp4";
  background_filename = DATA_PATH "/sample_background.jpeg";

  std::cout<<"Will Open File " << input_filename << std::endl;
  cv::VideoCapture cap;
  cv::Mat background;
  try {
    cap.open(input_filename);
    background = cv::imread(background_filename);
  }
  catch(std::exception& exp)
  {
    std::cout << "Got Exception " << exp.what() << std::endl;
    return 1;
  }

  auto width =  int(cap.get(cv::CAP_PROP_FRAME_WIDTH))/2;
  auto height = int(cap.get(cv::CAP_PROP_FRAME_HEIGHT))/2;
  std::cout << "Opened Capture File... status" << cap.isOpened() << std::endl;
  std::cout << "Dims Background = " << background.size() << std::endl;
  try {
    cv::resize(background,
               background,
               cv::Size(width, height),
                        0, 0, cv::INTER_AREA);
  } catch(std::exception &exp)
  {
    std::cout << "Got Exception " << exp.what() << std::endl;
    return 1;
  }
  if(cap.isOpened()) {

    auto key = 0;
    const std::string k_window_name{"green_screen"};
    cv::namedWindow(k_window_name);
    cv::setMouseCallback(k_window_name, mouse_f, 0);
    cv::createTrackbar("Tolerance",
                       k_window_name,
                       &s_state.tolerance,
                       100);

    while(key != 'q')
    {
      s_state.current_frame = get_a_frame(cap);
      cv::resize(s_state.current_frame,
                 s_state.current_frame,
                 cv::Size{width,height},
                 0, 0, cv::INTER_AREA);
      if(s_state.current_frame.empty()) break;
      cv::Mat frame_to_display = s_state.current_frame.clone();
      if(s_state.patch_center && !s_state.current_frame.empty())
      {
        cv::Mat mask;
        if(!s_state.color_patch) {
          s_state.color_patch = s_state.current_frame.at<cv::Vec3b>(cv::Point(s_state.patch_center->first,
                                                         s_state.patch_center->second));
          std::cout << "Getting COlor Patch " << *s_state.color_patch << std::endl;
          std::cout << "Frame Type = " << my_cv_utils::type2str(s_state.current_frame.type()) << std::endl;
        }
        cv::Vec3b lower_bound = *s_state.color_patch - cv::Vec3b(s_state.tolerance, s_state.tolerance, s_state.tolerance);
        cv::Vec3b upper_bound = *s_state.color_patch + cv::Vec3b(s_state.tolerance, s_state.tolerance, s_state.tolerance);

        cv::inRange(s_state.current_frame,
                    lower_bound,
                    upper_bound,
                    mask);

        background.copyTo(frame_to_display, mask);
      }
      cv::imshow(k_window_name, frame_to_display);
      key = cv::waitKey(30);
    }
  } else {
    std::cout << "Error opening file " << input_filename << std::endl;
    return 1;
  }

  return 0;
}
