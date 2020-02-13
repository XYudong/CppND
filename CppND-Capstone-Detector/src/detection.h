#ifndef DETECTION_H
#define DETECTION_H

#include <string>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/dnn.hpp>


class Detector {
public:
  Detector() {}
  Detector(std::string s) : _input(s) {}

  virtual void run() = 0;
  virtual void detect() = 0;

  void setInSize(int height, int width);

protected:
  std::string _input = "";

  cv::Mat _frame;
  cv::VideoCapture _source;
  cv::dnn::Net _net;

  int _in_height;
  int _in_width;
  cv::Scalar _mean_val;
  float _conf_threshold = 0.5;
  float _in_scale_factor = 1;
};

#endif