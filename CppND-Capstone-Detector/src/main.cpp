#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

#include "face_detection.h"
#include "text_detection.h"

int main() {
  std::cout << "Hello World!" << "\n";

  std::cout << "Hi, what would you like to detect?" << std::endl;

  std::string target;
  while (target != "face" && target != "text") {
    std::cout << "face? Or text?" << std::endl;
    std::getline(std::cin, target);
  }

  if (target == "face") {
    FaceDetector face_det;
    face_det.setInSize(300, 300);
    face_det.run();
  } else {
    TextDetector text_det;
    text_det.setInSize(320, 320);
    text_det.run();
  }

  return 0;
}

