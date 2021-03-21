// Coin Detection Assignment
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;


void displayImage(const Mat &image, const std::string &name = "Image", bool clear = true, bool scale_down = false)
{
  auto imageCopy = image.clone();
  if (scale_down) {
    resize(image, imageCopy, Size{}, 0.5, 0.5, INTER_AREA);
  }
  imshow(name, imageCopy);
  waitKey(0);
  if (clear)
    destroyAllWindows();
}

Mat displayConnectedComponents(Mat &im)
{
  // Make a copy of the image
  Mat imLabels = im.clone();

  // First let's find the min and max values in imLabels
  Point minLoc, maxLoc;
  double min, max;

  // The following line finds the min and max pixel values
  // and their locations in an image.
  minMaxLoc(imLabels, &min, &max, &minLoc, &maxLoc);

  // Normalize the image so the min value is 0 and max value is 255.
  imLabels = 255 * (imLabels - min) / (max - min);

  // Convert image to 8-bits
  imLabels.convertTo(imLabels, CV_8U);

  // Apply a color map
  Mat imColorMap;
  applyColorMap(imLabels, imColorMap, COLORMAP_JET);

  return imColorMap;
}

int main()
{

  // Image path
  string imagePath = DATA_PATH "/images/CoinsA.png";
  // Read image
  // Store it in the variable image
  Mat image = imread(imagePath);
  Mat imageCopy = image.clone();

//	displayImage(image, "Source");

  // Convert image to grayscale
  // Store it in the variable imageGray
  Mat imageGray;
  cvtColor(imageCopy, imageGray, COLOR_RGB2GRAY);
//	displayImage(imageGray);

  // Split cell into channels
  // Store them in variables imageB, imageG, imageR
  Mat bands[3];
  Mat imageB, imageG, imageR;
  split(imageCopy, bands);
  imageB = bands[0];
  imageG = bands[1];
  imageR = bands[2];

//	displayImage(imageB, "Blue");
//	displayImage(imageG, "Green");
//	displayImage(imageR, "Red");

  // Perform thresholding
  Mat dst;
  threshold(imageGray, dst, 35, 255, THRESH_BINARY_INV);

  // Modify as required
//	displayImage(dst, "Thresholded");
  // Perform morphological operations
  Mat imageDilated, imageEroded;
  // Get structuring element/kernel which will be used for dilation
  Mat element = getStructuringElement(MORPH_ELLIPSE, {5, 5});
  Mat element2 = getStructuringElement(MORPH_ELLIPSE, {3, 3});
  morphologyEx(dst, imageDilated, MORPH_DILATE, element2, Point{-1, -1}, 5);
  morphologyEx(imageDilated, imageDilated, MORPH_ERODE, element2, Point{-1, -1}, 7);
  morphologyEx(imageDilated, imageEroded, MORPH_DILATE, element, Point{-1, -1}, 4);

//	displayImage(imageEroded, "Morphed", false);


  // Setup SimpleBlobDetector parameters.
  SimpleBlobDetector::Params params;

  params.blobColor = 0;

  params.minDistBetweenBlobs = 2;

  // Filter by Area
  params.filterByArea = false;

  // Filter by Circularity
  params.filterByCircularity = true;
  params.minCircularity = 0.8;

  // Filter by Convexity
  params.filterByConvexity = true;
  params.minConvexity = 0.8;

  // Filter by Inertia
  params.filterByInertia = true;
  params.minInertiaRatio = 0.8;

  // Set up detector with params
  Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

  // Detect blobs
  std::vector<KeyPoint> keypoints;
  detector->detect(imageEroded, keypoints);
  // Print number of coins detected
  std::cout << "Blob Detector detected " << keypoints.size() << " points" << std::endl;

  // Mark coins using image annotation concepts we have studied so far

  std::for_each(keypoints.begin(), keypoints.end(), [&imageCopy](KeyPoint &point) {
    int radius = point.size / 2;
    double diameter = point.size;
    circle(imageCopy, point.pt, radius, Scalar{0, 255, 0}, 2);
    circle(imageCopy, point.pt, 1, Scalar{255, 0, 0}, 2);
  });
//	displayImage(imageCopy);
//  displayImage(imageEroded);
  // Find connected components
  Mat components;
  Mat inverted;
  bitwise_not(imageEroded, inverted);
  int n_components = connectedComponents(inverted, components);
  std::cout << "Connected Components = " << n_components << std::endl;

  // Use displayConnectedComponents function provided above
  Mat colorMap = displayConnectedComponents(components);

//	displayImage(colorMap);

  std::vector<std::vector<cv::Point>> contours;
  std::vector<Vec4i> hierarchy;
  // Print the number of contours found
  findContours(imageEroded, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);
  std::cout << "Found " << contours.size() << " contours" << std::endl;

  // Draw all contours
  imageCopy = image.clone();
  drawContours(imageCopy, contours, -1, Scalar{0, 255, 0}, 2);
  // Remove the inner contours
  // Display the result
  imageCopy = image.clone();
  drawContours(imageCopy, contours, 9, Scalar{0, 255, 0}, 3);
//  displayImage(imageCopy, "Contours");

  // Print area and perimeter of all contours
  for (uint32_t i = 0; i < contours.size(); i++) {
    float area = contourArea(contours[i]);
    std::cout << "Area of Contour #" << i << " = " << area << std::endl;

  }
  auto maxPos = std::max_element(contours.begin(), contours.end(), [](auto &a, auto &b) {
    return contourArea(a) < contourArea(b);
  });
  // Print maximum area of contour
  std::cout << "Max Area is " << contourArea(*maxPos) << std::endl;
  // Remove this contour and plot others
  contours.erase(maxPos);
  imageCopy = image.clone();
  drawContours(imageCopy, contours, -1, Scalar{0, 255, 0}, 3);
//  displayImage(imageCopy, "Contours filtered");
  // Fit circles on coins
  imageCopy = image.clone();
  for (uint32_t i = 0; i < contours.size(); i++) {
    Point2f center;
    float radius;
    minEnclosingCircle(contours[i], center, radius);
    circle(imageCopy, center, radius, Scalar(0, 255, 0), 2);
  }

//  displayImage(imageCopy);


  // Image path
  imagePath = DATA_PATH  "/images/CoinsB.png";
  // Read image
  // Store it in variable image
  image = imread(imagePath);
  imageCopy = image.clone();

  // Convert image to grayscale
  // Store it in the variable imageGray

  cvtColor(imageCopy, imageGray, COLOR_RGB2GRAY);

//	displayImage(imageGray);

  // Split cell into channels
  // Store them in variables imageB, imageG, imageR
  split(imageCopy, bands);
  imageB = bands[0];
  imageG = bands[1];
  imageR = bands[2];

//	displayImage(imageB);
//	displayImage(imageG);
//	displayImage(imageR);

  // Perform thresholding
  threshold(imageB, dst, 127, 255, THRESH_BINARY);
//	displayImage(dst, "DstB", true, true);

  // Perform morphological operations
  element2 = getStructuringElement(MORPH_ELLIPSE, Size{11, 11});
  morphologyEx(dst, imageDilated, MORPH_OPEN, element2, Point{-1, -1}, 1);
  morphologyEx(imageDilated, imageEroded, MORPH_CLOSE, element2, Point{-1, -1}, 2);
  morphologyEx(imageEroded, imageEroded, MORPH_ERODE, element2, Point{-1, -1}, 12);
  morphologyEx(imageEroded, imageEroded, MORPH_DILATE, element2, Point{-1, -1}, 6);
  displayImage(imageDilated, "Dilated", false, true);
  displayImage(imageEroded, "Eroded", true, true);
  // Setup SimpleBlobDetector parameters.

  params.blobColor = 0;

  params.minDistBetweenBlobs = 2;

  // Filter by Area
  params.filterByArea = false;

  // Filter by Circularity
  params.filterByCircularity = true;
  params.minCircularity = 0.8;

  // Filter by Convexity
  params.filterByConvexity = true;
  params.minConvexity = 0.8;

  // Filter by Inertia
  params.filterByInertia = true;
  params.minInertiaRatio = 0.8;

  // Set up detector with params
  detector = SimpleBlobDetector::create(params);

  // Detect blobs
  detector->detect(imageEroded, keypoints);
  // Print number of coins detected
  std::cout << "Num Coins Detected = " << keypoints.size() << std::endl;

  // Mark coins using image annotation concepts we have studied so far
  for (auto &point : keypoints) {
    int radius = point.size / 2;
    circle(imageCopy, point.pt, radius, Scalar{0, 255, 0}, 2);
    circle(imageCopy, point.pt, 1, Scalar{255, 0, 0}, 2);
  }
  displayImage(imageCopy, "Annotated", true, true);
  // Find connected components
  bitwise_not(imageEroded, inverted);
  n_components = connectedComponents(inverted, components);
  std::cout << "Num of connected Components = " << n_components << std::endl;

  colorMap = displayConnectedComponents(components);
  displayImage(colorMap, "components", true, true);
  // Find all contours in the image
  findContours(imageEroded, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);

  // Print the number of contours found
  std::cout << "Number of contours = " << contours.size() << std::endl;

  // Draw all contours
  imageCopy = image.clone();
  drawContours(imageCopy, contours, -1, Scalar{255, 0, 0}, 3, LINE_AA);
  displayImage(imageCopy, "contours", true, true);

  // Remove the inner contours
  // Display the result
  std::sort(contours.begin(), contours.end(), [](auto &a, auto &b) { return contourArea(a) > contourArea(b); });
  contours.pop_back();
  contours.pop_back();
  imageCopy = image.clone();
  drawContours(imageCopy, contours, -1, Scalar{255, 0, 0}, 3, LINE_AA);
  displayImage(imageCopy, "contours filtered", true, true);
  // Print area and perimeter of all contours
  for (uint32_t i = 0; i < contours.size(); i++) {
    float area = contourArea(contours[i]);
    float perimeter = arcLength(contours[i], true);
    std::cout << "Area of Contour #" << i << " = " << area << " Perimeter = " << perimeter << std::endl;

  }
  maxPos = std::max_element(contours.begin(), contours.end(), [](auto &a, auto &b) {
    return contourArea(a) < contourArea(b);
  });
  // Print maximum area of contour
  std::cout << "Max Area is " << contourArea(*maxPos) << std::endl;
  // Remove this contour and plot others
  contours.erase(maxPos);
  imageCopy = image.clone();
  drawContours(imageCopy, contours, -1, Scalar{255, 0, 0}, 3, LINE_AA);
  displayImage(imageCopy, "contours Filtered", true, true);

  // Remove contours
  ///
  /// YOUR CODE HERE
  ///

  // Draw revised contours
  ///
  /// YOUR CODE HERE
  ///

  // Fit circles on coins
  imageCopy = image.clone();
  for (uint32_t i = 0; i < contours.size(); i++) {
    Point2f center;
    float radius;
    minEnclosingCircle(contours[i], center, radius);
    circle(imageCopy, center, radius, Scalar(0, 255, 0), 5);
  }
  displayImage(imageCopy, "Final", true, true);

  return 0;
}