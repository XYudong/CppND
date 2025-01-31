#ifndef FACE_DETECTION_H
#define FACE_DETECTION_H

#include "detection.h"

#include <string>

class FaceDetector : public Detector {
public:
  FaceDetector() : Detector() {}
  FaceDetector(std::string s) : Detector(s) {}

  void run() override;
  void detect() override;

private:
  const float _confidenceThreshold = 0.7;
  const std::string _caffeConfigFile = "../models/opencv_dnn_face/deploy.prototxt";
  const std::string _caffeWeightFile = "../models/opencv_dnn_face/res10_300x300_ssd_iter_140000_fp16.caffemodel";

  
};


#endif