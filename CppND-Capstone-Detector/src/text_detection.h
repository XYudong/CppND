#ifndef TEXT_DETECTION_H
#define TEXT_DETECTION_H

#include "detection.h"

#include <string>

class TextDetector : public Detector {
public:
  TextDetector() : Detector() {}
  TextDetector(std::string s) : Detector(s) {}

  void run() override;
  void detect() override;
  void setInSize(int height, int width) override;

private:
  float _nms_threshold = 0.4;
  std::string _TFPbFile = "../models/east/frozen_east_text_detection.pb";

};


void decode(const cv::Mat& scores, const cv::Mat& geometry, float scoreThresh,
            std::vector<cv::RotatedRect>& detections, std::vector<float>& confidences);



#endif