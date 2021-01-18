//
// Created by kartik on 1/16/21.
//

#include <opencv2/opencv.hpp>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

int main(void)
{
  // Load the Face Image
  std::string faceImagePath = IMAGE_PATH  "/musk.jpg";
  std::string glassimagePath = IMAGE_PATH  "/sunglass.png";

  if (fs::exists(faceImagePath) && fs::exists(glassimagePath)) {

    cv::Mat faceImage = cv::imread(faceImagePath);
//    faceImage.convertTo(faceImage, CV_32FC3);
    // Load the Sunglass image with Alpha channel
    // (http://pluspng.com/sunglass-png-1104.html)

    // Read the image
    cv::Mat glassPNG = cv::imread(glassimagePath, -1);

    std::cout << "Musk Size = " << faceImage.size() << std::endl;
    std::cout << "Pre-resize Size = " << glassPNG.size() << std::endl;
    // Resize the image to fit over the eye region
    cv::resize(glassPNG, glassPNG, cv::Size(300, 100));

    std::cout << "Image Dimension = " << glassPNG.size() << std::endl;
    std::cout << "Number of channels = " << glassPNG.channels();

    // Separate the Color and alpha channels
    cv::Mat glassRGBAChannels[4];
    cv::Mat glassRGBChannels[3];
    cv::split(glassPNG, glassRGBAChannels);

    for (int i = 0; i < 3; i++) {
      // Copy R,G,B channel from RGBA to RGB
      glassRGBChannels[i] = glassRGBAChannels[i];
    }

    cv::Mat glassBGR, glassMask1;
    // Prepare BGR Image
    cv::merge(glassRGBChannels, 3, glassBGR);
    // Alpha channel is the 4th channel in RGBA Image
    glassMask1 = glassRGBAChannels[3];
    cv::Mat glassMask;
    cv::Mat glassMaskChannels[3] = {glassMask1, glassMask1, glassMask1};
    cv::merge(glassMaskChannels, 3, glassMask);

    glassMask /= 255;

    auto faceImageDupe = faceImage.clone();
    cv::Mat eyeRoiChannels[3];
    cv::Mat eyeRoi = faceImageDupe(cv::Range(150, 250),
                                   cv::Range(140, 440));
    cv::split(eyeRoi, eyeRoiChannels);
    cv::Mat maskedEyeChannels[3];
    cv::Mat maskedEye;

    for (uint32_t i = 0; i < 3; i++) {
      cv::multiply(eyeRoiChannels[i], (1 - (glassMaskChannels[i])), maskedEyeChannels[i]);
    }
    cv::merge(maskedEyeChannels, 3, maskedEye);
    cv::imshow("masked", maskedEye);
    cv::waitKey();

    cv::Mat maskedGlass;
    cv::multiply(glassBGR, glassMask, maskedGlass);
    cv::imshow("glassMask", maskedGlass);
    cv::waitKey();
    std::cout << maskedGlass<< std::endl;

    cv::Mat blendedMask;
    cv::add(maskedGlass, maskedEye, blendedMask);

    blendedMask.copyTo(eyeRoi);
    cv::imshow("Elon", faceImageDupe);
    cv::waitKey();

//    cv::Mat lEyeRoi = faceImageDupe(cv::Range(210, 250), cv::Range(200, 240));
//    cv::Mat lEyeRoiChan[3];
//    cv::split(lEyeRoi, lEyeRoiChan);
//    cv::Mat maskedLeftEyeChan[3];
//    cv::Mat maskedLeftEye;
//    for(uint32_t i = 0; i < 3; i++) {
//      cv::multiply(lEyeRoiChan[i], (1-glassMaskChannels[i]), maskedLeftEyeChan[i]);
//    }
//    cv::Mat rEyeRoi = faceImageDupe(cv::Range(210,250), cv::Range(340, 390));
//    cv::Mat rEyrRoiChan[3];
//    cv::split(rEyeRoi, rEyrRoiChan);


  }
  return 0;
}