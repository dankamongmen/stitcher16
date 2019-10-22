#include <cstdlib>
#include <iostream>
#include "pathware.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

int main(int argc, char** argv) {
  while(*++argv){
    auto img = cv::imread(*argv, cv::IMREAD_ANYDEPTH | cv::IMREAD_ANYCOLOR);
    if(img.empty()){
      std::cerr << "error reading " << *argv << std::endl;
      return EXIT_FAILURE;
    }
    print_mat(img, *argv, std::cout);
    if(!imwrite("out16.tiff", img)){
      std::cerr << "error writing out16.tiff" << std::endl;
      return EXIT_FAILURE;
    }
    cv::Mat matu8;
    // normalize(img, matu8, 0, 255, cv::NORM_MINMAX);
    // equalizeHist(img, matu8);
    img.convertTo(matu8, CV_8UC3, 1/255.0);
    print_mat(matu8, "matu8.tiff", std::cout);
    if(!imwrite("matu8.tiff", matu8)){
      std::cerr << "error writing matu8.tiff" << std::endl;
      return EXIT_FAILURE;
    }
    cv::Mat mat16s;
    img.convertTo(mat16s, CV_16SC3, 1/255.0);
    print_mat(mat16s, "mat16s.tiff", std::cout);
    if(!imwrite("mat16s.tiff", mat16s)){
      std::cerr << "error writing mat16s.tiff" << std::endl;
      return EXIT_FAILURE;
    }
    cv::Mat mat32s;
    img.convertTo(mat32s, CV_32SC3, 1/255.0);
    print_mat(mat32s, "mat32s.tiff", std::cout);
    if(!imwrite("mat32s.tiff", mat32s)){
      std::cerr << "error writing mat32s.tiff" << std::endl;
      return EXIT_FAILURE;
    }
    cv::Mat mat16f;
    img.convertTo(mat16f, CV_16FC3, 1/255.0);
    print_mat(mat16f, "mat16f.tiff", std::cout);
    if(!imwrite("mat16f.tiff", mat16f)){
      std::cerr << "error writing mat16f.tiff" << std::endl;
      return EXIT_FAILURE;
    }
    cv::Mat backto16;
    mat32s.convertTo(backto16, CV_16UC3, 255.0);
    print_mat(backto16, "backto16.tiff", std::cout);
    if(!imwrite("backto16.tiff", backto16)){
      std::cerr << "error writing backto16.tiff" << std::endl;
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}
