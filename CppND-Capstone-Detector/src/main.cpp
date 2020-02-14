#include <iostream>
#include <string>
#include <memory>
#include <opencv2/opencv.hpp>

#include "face_detection.h"
#include "text_detection.h"

int main() {
  std::cout << "Hello World!" << "\n";

  std::cout << "Hi, what would you like to detect?" << std::endl;

  // user input
  std::string target;
  while (target != "face" && target != "text") {
    std::cout << "Please enter your choice(face, or text): ";
    std::getline(std::cin, target);
  }

  // init network
  std::unique_ptr<Detector> det;
  if (target == "face") {
    det = std::unique_ptr<Detector>(new FaceDetector);
    det->setInSize(300, 300);
  } else {
    det = std::unique_ptr<Detector>(new TextDetector);
    det->setInSize(320, 320);
  }

  // run
  det->run();

  return 0;
}

