#include "frame_data_t.h"

std::vector<cv::Mat> read_emir(cv::Mat img)
{
  Size sz = img.size();
  int height = sz.height / 3;
  int width = sz.width;
  std::vector<cv::Mat>channels;
  channels.push_back(img( Rect(0, 0,         width, height)));
  channels.push_back(img( Rect(0, height,    width, height)));
  channels.push_back(img( Rect(0, 2*height,  width, height)));
  return channels;
}

int main(int argc, char** argv)
{
  auto emir = read_emir(cv::imread(DATA_FOLDER "/images/emir.jpg"));
  cv::Ptr<cv::ORB> orb = cv::ORB::create();
  frame_data_t blue{orb, emir[0]};
  frame_data_t green{orb, emir[1]};
  frame_data_t red{orb, emir[2]};

  auto matchesBlueGreen = green.match(blue);
  auto matchesRedGreen = green.match(red);

  return 0;
}
