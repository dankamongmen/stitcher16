#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/stitching.hpp>

// Adapted from https://docs.opencv.org/master/d8/d19/tutorial_stitcher.html

void usage(const char* name, int ret) {
  std::cout << "usage: " << name << "output input1 [ input2...]" << std::endl;
  exit(ret);
}

int main(int argc, char** argv) {
  cv::Stitcher::Mode mode = cv::Stitcher::SCANS;
  std::vector<cv::Mat> imgs;

  if(argc < 3){
    usage(argv[0], EXIT_FAILURE);
  }
  const std::string result_name = argv[1];
  for(auto arg = argv + 2 ; arg ; ++arg){
    cv::Mat img = cv::imread(cv::samples::findFile(*arg));
  }
  cv::Mat pano;
  cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(mode);
  cv::Stitcher::Status status = stitcher->stitch(imgs, pano);
  if(status != cv::Stitcher::OK){
      std::cerr << "Can't stitch images, error code = " << int(status) << std::endl;
      return EXIT_FAILURE;
  }
  cv::imwrite(result_name, pano);
  std::cout << "stitching completed successfully\n" << result_name << " saved!";
  return EXIT_SUCCESS;
}
