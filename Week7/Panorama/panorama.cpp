//
// Created by Kartik Aiyer on 4/18/21.
//

#include <opencv2/opencv.hpp>
#include <algorithm>
#include <filesystem>
#include <my_cv_utils.h>

static void process_frame(cv::Mat &frame)
{

}

struct frame_data_t
{
  frame_data_t(cv::Ptr<cv::ORB> orb, cv::Mat mat) : frame{std::move(mat)}
  {
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    orb->detectAndCompute(gray, cv::Mat(), keypoints, descriptors);
  }

  std::vector<cv::DMatch> match(frame_data_t &other,
                                cv::Ptr<cv::DescriptorMatcher> matcher = cv::Ptr<cv::DescriptorMatcher>(),
                                    float top_percent = 0.15f)
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

  cv::Mat frame;
  std::vector<cv::KeyPoint> keypoints;
  cv::Mat descriptors;
};

namespace fs = std::filesystem;

#if 1
int main(int argc, char** argv)
{
  fs::path input_dir{DATA_FOLDER "/images/scene"};
  cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create();
  std::vector<cv::Mat> input_mats;
  for(fs::directory_entry entry : fs::directory_iterator(input_dir)) {
    input_mats.push_back(cv::imread(entry.path()));
  }
  cv::Mat out;
  stitcher->stitch(input_mats, out);
  cv::imshow("stitched", my_cv_utils::fit_large_to_display(out));
  cv::moveWindow("stitched", 0, 0);
  cv::waitKey();
  return 0;
}
#else
int main(int argc, char **argv)
{
  fs::path input_dir{DATA_FOLDER "/images/scene"};

  cv::Ptr<cv::ORB> orb = cv::ORB::create();
  std::optional<frame_data_t> ref_frame;
  std::vector<frame_data_t> other_frames;
  uint32_t count = 0;
  for( fs::directory_entry entry : fs::directory_iterator(input_dir))
  {
    count++;
    if(!ref_frame) {
      ref_frame = frame_data_t(orb, cv::imread(entry.path()));
    } else {
      other_frames.push_back(frame_data_t(orb, cv::imread(entry.path())));
    }
    if(count == 3)
    {
      break;
    }
  }
  cv::Mat concat = ref_frame->frame.clone();

  while(!other_frames.empty())
  {
    auto ordered_matches = ref_frame->ordered_matches(std::begin(other_frames),
                                                     std::end(other_frames));
    auto &best_match = ordered_matches.front();

    cv::Mat best_mat = std::get<0>(best_match).frame;
    cv::hconcat(ref_frame->frame, best_mat, concat);
    std::vector<cv::Point2f> points1, points2;
    for( size_t i = 0; i < std::get<1>(best_match).size(); i++ )
    {
      points1.push_back( ref_frame->keypoints[ std::get<1>(best_match)[i].queryIdx ].pt );
      points2.push_back( std::get<0>(best_match).keypoints[std::get<1>(best_match)[i].trainIdx].pt);
    }
    cv::Mat h = cv::findHomography(points2, points1, cv::RANSAC);
    int im2_height = std::get<0>(best_match).frame.rows;
    int im2_width = std::get<0>(best_match).frame.cols;
    int im1_height = ref_frame->frame.rows;
    int im1_width = ref_frame->frame.cols;

    cv::Mat output;
    cv::warpPerspective(std::get<0>(best_match).frame,
                        output,
                        h,
                        cv::Size(im2_width + im1_width, im2_height));
    cv::imshow("ref", my_cv_utils::fit_large_to_display(concat));
    cv::moveWindow("ref", 0, 0);
    cv::waitKey();
    cv::imshow("ref", std::get<0>(best_match).frame);
    cv::waitKey();
    cv::imshow("ref", output);
    cv::waitKey();
    cv::Rect roi{0, 0, im1_width, im1_height};
    ref_frame->frame.copyTo(output(roi));
    ref_frame = frame_data_t(orb, output);

    cv::imshow("ref", my_cv_utils::fit_large_to_display(ref_frame->frame));
    cv::waitKey();
    other_frames.erase(std::begin(other_frames) + std::get<int>(best_match));
  }
  return 0;
}
#endif