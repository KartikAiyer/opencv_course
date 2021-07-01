//
// Created by Kartik Aiyer on 5/3/21.
//

#include "frame_data_t.h"

frame_data_t::frame_data_t(cv::Ptr<cv::ORB> orb, cv::Mat mat) : frame{std::move(mat)}
{
  cv::Mat gray;
  cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
  orb->detectAndCompute(gray, cv::Mat(), keypoints, descriptors);
}

std::vector<cv::DMatch> frame_data_t::match(frame_data_t &other,
                              cv::Ptr<cv::DescriptorMatcher> matcher,
                              float top_percent)
{
  std::vector<cv::DMatch> retval;
  bool should_clear = false;
  if (!matcher) {
    should_clear = true;
    matcher = cv::DescriptorMatcher::create("BruteForce-Hamming");
  }
  matcher->match(descriptors, other.descriptors, retval, cv::Mat());
  std::sort(retval.begin(), retval.end());

  const int numGoodMatches = retval.size() * top_percent;
  retval.erase(retval.begin()+numGoodMatches, retval.end());
  if (should_clear && !matcher.empty()) {
    matcher.release();
  }
  return retval;
}