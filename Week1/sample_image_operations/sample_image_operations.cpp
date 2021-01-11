//
// Created by kartik on 1/9/21.
//

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <docopt/docopt.h>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

static const char USAGE[] =
    R"(     Usage:
                basic_linear_transforms (-a=<alpha> -b=<brightness>) [-h] <filename>

            Arguments:
              <filename>        input file to be converted

            Options:
                -h                Print Help menu
                -a=<alpha>        contrast scale from 1.0 - 3.0
                -b=<brightness>   1 - 100)";

int main(int argc, char** argv)
{

  std::map<std::string, docopt::value> args =
      docopt::docopt(USAGE,
                     {argv + 1, argv + argc});

  std::cout << args["<filename>"] << std::endl;

  fs::path input_file{ args["<filename>"].asString() };
  std::for_each(args.begin(), args.end(),
                [](const std::pair<std::string,docopt::value> &options) {
    std::cout << options.first << ":" << options.second << std::endl;
  });
  input_file = fs::absolute(input_file);
  if(fs::exists(input_file))
  {
    cv::Mat image = cv::imread(input_file);
    if(image.empty())
    {
      std::cout << "Could not open or find the image " << input_file << std::endl;
      std::cout << USAGE << std::endl;
      return -1;
    }
    cv::Mat new_image = cv::Mat::zeros(image.size(), image.type());
    float alpha = std::stof(args["-a"].asString());
    int beta = args["-b"].asLong();

    for(uint32_t y = 0; y < image.rows; y++) {
      for(uint32_t x = 0; x < image.cols; x++) {
        for(uint32_t c = 0; c < image.channels(); c++) {
          new_image.at<cv::Vec3b>(y,x)[c] =
              cv::saturate_cast<uchar>(alpha * image.at<cv::Vec3b>(y,x)[c] + beta);
        }
      }
    }

    cv::imshow("Original Image", image);
    cv::imshow("New Image", new_image);
    cv::waitKey();
  }
  return 0;
}
