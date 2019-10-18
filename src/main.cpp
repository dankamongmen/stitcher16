#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/stitching.hpp>

// Adapted from https://docs.opencv.org/master/d8/d19/tutorial_stitcher.html

int main(int argc, char** argv) {
  cv::Stitcher::Mode mode = cv::Stitcher::PANORAMA;
  std::vector<cv::Mat> imgs;
  std::string result_name = "output.jpg"; // FIXME

  cv::Mat pano;
  cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(mode);
  cv::Stitcher::Status status = stitcher->stitch(imgs, pano);
  if (status != cv::Stitcher::OK)
  {
      std::cerr << "Can't stitch images, error code = " << int(status) << std::endl;
      return EXIT_FAILURE;
  }
  cv::imwrite(result_name, pano);
  std::cout << "stitching completed successfully\n" << result_name << " saved!";
  return EXIT_SUCCESS;
}
