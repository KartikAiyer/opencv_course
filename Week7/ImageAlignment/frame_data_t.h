//
// Created by Kartik Aiyer on 5/3/21.
//

#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>

struct frame_data_t
{
  frame_data_t(cv::Ptr<cv::ORB> orb, cv::Mat mat);

  std::vector<cv::DMatch> match(frame_data_t &other,
                                cv::Ptr<cv::DescriptorMatcher> matcher = cv::Ptr<cv::DescriptorMatcher>(),
                                float top_percent = 0.15f);

  template<typename InputIterator,
      typename T = typename std::iterator_traits<InputIterator>::value_type,
      typename = std::enable_if<std::is_same_v<T, frame_data_t>>>
  std::vector<std::tuple<frame_data_t,std::vector<cv::DMatch>, int>> ordered_matches(InputIterator other_begin,
      InputIterator other_end)
  {
    std::vector<std::tuple<frame_data_t, std::vector<cv::DMatch>, int>> retval;
    int i = 0;
    std::for_each(other_begin, other_end, [this, &retval, &i](auto &other_frame) {
      retval.push_back(std::make_tuple(other_frame, match(other_frame), i++));
    });
    std::sort(retval.begin(), retval.end(), [](auto &item1, auto &item2) {
      return std::get<1>(item1).size() < std::get<1>(item2).size();
    });
    return retval;
  }

  cv::Mat find_homography(cv::Mat other);

  cv::Mat frame;
  std::vector<cv::KeyPoint> keypoints;
  cv::Mat descriptors;
};
