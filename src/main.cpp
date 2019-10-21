#include <iostream>
#include <tiffio.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/stitching.hpp>

// Adapted from https://docs.opencv.org/master/d8/d19/tutorial_stitcher.html

void usage(const char* name, std::ostream& os, int ret) {
  os << "usage: " << name << " output input1 [ input2...]" << std::endl;
  exit(ret);
}

int main(int argc, char** argv) {
  cv::Stitcher::Mode mode = cv::Stitcher::SCANS;
  std::vector<cv::Mat> imgs;

  if(argc < 3){
    usage(argv[0], std::cerr, EXIT_FAILURE);
  }
  std::cout << "Using OpenCV " << CV_VERSION << std::endl;
  std::cout << "TIFF " << TIFFLIB_VERSION << std::endl;
  const std::string result_name = argv[1];
  for(auto arg = argv + 2 ; *arg ; ++arg){
    cv::Mat img = cv::imread(cv::samples::findFile(*arg), cv::IMREAD_UNCHANGED);
    if(img.empty()){
      std::cerr << "Couldn't read image at " << *arg << std::endl;
      usage(argv[0], std::cerr, EXIT_FAILURE);
    }
    std::cout << "Read " << *arg << " (" << img.size() << ")" << std::endl;
    std::cout << " chans: " << img.channels() << " depth: " << img.depth()
              << " type: " << img.type() << std::endl;
    imgs.push_back(img);
  }
  cv::Mat pano;
  cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(mode);
  cv::Stitcher::Status status = stitcher->stitch(imgs, pano);
  if(status != cv::Stitcher::OK){
    std::cerr << "Can't stitch images, error code = " << int(status) << std::endl;
    return EXIT_FAILURE;
  }
  if(!cv::imwrite(result_name, pano)){
    std::cerr << "Couldn't write output to " << result_name << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "saved result to " << result_name << std::endl;
  return EXIT_SUCCESS;
}
