#include <cstdlib>
#include <iostream>
#include <opencv2/imgcodecs.hpp>

int main(int argc, char** argv) {
  while(*++argv){
    auto img = cv::imread(*argv, cv::IMREAD_ANYDEPTH | cv::IMREAD_ANYCOLOR);
    if(img.empty()){
      std::cerr << "error reading " << *argv << std::endl;
      return EXIT_FAILURE;
    }
    if(!imwrite("out16.tiff", img)){
      std::cerr << "error writing out16.tiff" << std::endl;
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}
