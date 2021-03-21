// QR-Code-Assignment
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>

std::string type2str(int type)
{
  std::string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch (depth) {
    case CV_8U:
      r = "8U";
      break;
    case CV_8S:
      r = "8S";
      break;
    case CV_16U:
      r = "16U";
      break;
    case CV_16S:
      r = "16S";
      break;
    case CV_32S:
      r = "32S";
      break;
    case CV_32F:
      r = "32F";
      break;
    case CV_64F:
      r = "64F";
      break;
    default:
      r = "User";
      break;
  }

  r += "C";
  r += (chans + '0');

  return r;
}

int main()
{
  // Image Path
  std::string imgPath = IMAGE_PATH "/IDCard-Satya.png";

  // Read image and store it in variable img
  ///
  /// YOUR CODE HERE
  ///
  auto img = cv::imread(imgPath);

  std::cout << img.size().height << " " << img.size().width << std::endl;

  cv::Mat bbox, rectifiedImage;

  // Create a QRCodeDetector Object
  // Variable name should be qrDecoder
  ///
  /// YOUR CODE HERE
  ///
  cv::QRCodeDetector qrDecoder{};

  // Detect QR Code in the Image
  // Output should be stored in opencvData

  auto opencvData = qrDecoder.detectAndDecode(img, bbox, rectifiedImage);
  std::cout << type2str(bbox.type()) << std::endl;
  // Check if a QR Code has been detected
  if (opencvData.length() > 0)
    std::cout << "QR Code Detected : " << opencvData << ": channels = " << bbox.channels() << std::endl;
  else
    std::cout << "QR Code NOT Detected" << std::endl;

  std::for_each(bbox.begin<cv::Point2f>(), bbox.end<cv::Point2f>(), [](auto &v) { std::cout << v << std::endl; });
  for (uint32_t i = 0; i < bbox.rows * bbox.cols; i++) {
    cv::line(img, bbox.at<cv::Point2f>(i),
             bbox.at<cv::Point2f>((i + 1) % (bbox.rows * bbox.cols)),
             cv::Scalar{0, 0, 255}, 2, cv::LINE_AA);
  }

  // Draw the bounding box
  // Since we have already detected and decoded the QR Code
  // using qrDecoder.detectAndDecode, we will directly
  // use the decoded text we obtained at that step (opencvData)
  std::cout << "QR Code Detected!" << std::endl;
  cv::imshow("Output", img);
  cv::waitKey();

  // Write the result image
  std::string resultImagePath = "./QRCode-Output.png";
  cv::imwrite(resultImagePath, img);

  return 0;
}