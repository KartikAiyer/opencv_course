//
// Created by kartik on 2/26/21.
//


#include <opencv2/opencv.hpp>
#include <my_cv_utils.h>

cv::Mat convertBGRtoGray(cv::Mat image)
{
  cv::Mat split_image[3];
  cv::Mat output;

  cv::split(image, split_image);

  output = (split_image[0] * 0.114) + (split_image[1] * 0.587) + (split_image[2] * 0.299);
  return output;
}

cv::Mat max_mat_chan(cv::Mat image)
{
  cv::Mat planes[3];
  cv::split(image, planes);
  return cv::Mat(cv::max(planes[2], cv::max(planes[1], planes[0])));
}

cv::Mat convertBGRtoHSV(cv::Mat image)
{
  cv::Mat normalized;
  image.convertTo(normalized, CV_32F, 1/255.0, 0);
  cv::Mat V(image.rows, image.cols, CV_32FC1);
  cv::Mat S(image.rows, image.cols, CV_32FC1);
  cv::Mat H(image.rows, image.cols, CV_32FC1);
  cv::Mat output;
  for(uint32_t i = 0; i < normalized.rows; i++)
  {
    auto* v_row = V.ptr<float>(i);
    auto* s_row = S.ptr<float>(i);
    auto* h_row = H.ptr<float>(i);
    auto* in = normalized.ptr<float>(i);

    uint32_t k = 0;
    for(uint32_t j = 0; j < normalized.cols; j++)
    {
      float b = in[k++];
      float g = in[k++];
      float r = in[k++];
      float pix_min = cv::min(b, cv::min(g, r));

      v_row[j] = cv::max(b, cv::max(g, r));
      s_row[j] = (cv::abs(v_row[j] - 0.0) > FLT_EPSILON) ? (v_row[j] - pix_min) / v_row[j]:0.0;

      if(cv::abs(v_row[j] - r) < FLT_EPSILON) {
        h_row[j] = 60 * (g - b)/(v_row[j] - pix_min);
      } else if(cv::abs(v_row[j] - g) < FLT_EPSILON) {
        h_row[j] = (120 + 60)*(b - r)/(v_row[j] - pix_min);
      } else
      {
        h_row[j] = (240 + 60)*(r -g)/(v_row[j] - pix_min);
      }
      if(h_row[j] < 0) { h_row[j] += 360.0;}

      v_row[j] = 255 * v_row[j];
      s_row[j] = 255 * s_row[j];
      h_row[j] = h_row[j] / 2;
    }
  }
  cv::Mat temp_arr[] = {H, S, V};
  cv::merge(temp_arr, 3, output);
  output.convertTo(output, CV_8UC3);
  return output;
}

int main()
{
  std::string filename = IMAGES_PATH "/capsicum.jpg";

  cv::Mat image = cv::imread(filename);

  cv::Mat my_out = convertBGRtoGray(image);
  cv::Mat cvOut;
  cv::cvtColor(image, cvOut, cv::COLOR_BGR2GRAY);

  cv::Mat diff_gray;
  cv::absdiff(my_out, cvOut, diff_gray);

  cv::Mat my_hsv = convertBGRtoHSV(image);
  cv::Mat cv_hsv;
  cv::cvtColor(image, cv_hsv, cv::COLOR_BGR2HSV);

  std::cout << "My Type = " << my_cv_utils::type2str(cv_hsv.type()) << std::endl;
  std::cout << "CV Type = " << my_cv_utils::type2str(cv_hsv.type()) << std::endl;

  cv::Mat diff_hsv;
  cv::absdiff(my_hsv, cv_hsv, diff_hsv);


  cv::imshow("diff", diff_hsv);

  cv::Mat split_diff[3];
  cv::split(diff_hsv, split_diff);
  cv::imshow("diffH", split_diff[0]);
//  cv::imshow("diffS", split_diff[1]);
//  cv::imshow("diffV", split_diff[2]);
  cv::waitKey();
  return 0;
}