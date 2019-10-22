#ifndef STITCHER16_PATHWARE
#define STITCHER16_PATHWARE

#include <string>
#include <iostream>
#include <opencv2/imgcodecs.hpp>

std::ostream& print_mat(const cv::Mat& m, const std::string& s, std::ostream& os) {
  os << "Image " << s << " (" << m.size() << ")\n depth: " <<
     m.depth() << " chan: " << m.channels() << " type: " <<
     m.type() << std::endl;
  return os;
}

#endif
