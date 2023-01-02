//
// Created by Kartik Aiyer on 10/15/22.
//

#include "faceBlendCommon.hpp"
#include <opencv2/opencv.hpp>


static inline cv::Mat input_touchup(const cv::Mat& input)
{
  cv::Mat smaller;
  cv::resize(input, smaller, cv::Size(), 0.33, 0.33);
  return smaller;
}
static inline cv::Mat draw_landmark_points(const cv::Mat& image, const vector<cv::Point2f>& points)
{
  cv::Mat out_img;
  image.copyTo(out_img);
  for(auto &point : points){
    cv::circle(out_img,
               point,
               1,
               cv::Scalar(0,255,0),
               cv::FILLED,
               cv::LINE_8);
  }
  return out_img;
}

int main(int argc, char** argv)
{
  auto image = cv::imread(DATA_FOLDER "/images/KartikFace.jpg");
  auto face_detector = dlib::get_frontal_face_detector();
  dlib::shape_predictor landmark_detector;
  dlib::deserialize(DATA_FOLDER "/model/shape_predictor_68_face_landmarks.dat") >> landmark_detector;

  auto spectacles = cv::imread(DATA_FOLDER "/images/spectacles.png", IMREAD_UNCHANGED);
  auto smaller = input_touchup(image);

  std::cout << "Spectacles num Channels = " << spectacles.channels() << std::endl;
  cv::Mat alpha_specs;
  std::vector<cv::Mat> channels;
  cv::split(spectacles, channels);
  std::vector<cv::Mat> bgrchannels;

  bgrchannels.push_back(channels[0]);
  bgrchannels.push_back(channels[1]);
  bgrchannels.push_back(channels[2]);
  cv::merge(bgrchannels, alpha_specs);
  cv::imshow("main", spectacles);
  cv::waitKey();
  auto lm_points = getLandmarks(face_detector, landmark_detector, smaller, 1.0);
  std::cout<< "Number of facial landmarks = " << lm_points.size() << std::endl;

  return 0;
}