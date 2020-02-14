#include "face_detection.h"

void FaceDetector::run() {
  _mean_val = cv::Scalar(104.0, 177.0, 123.0);
  
  _net = cv::dnn::readNet(_caffeConfigFile, _caffeWeightFile);

  if (_input.empty()) {
    _source.open(0);
  } else {
    _source.open(_input);
  }

  while (cv::waitKey(1) != 27) {
    double t = cv::getTickCount();
    _source >> _frame;
    if (_frame.empty()) {
      break;
    }
    
    detect();
    
    double tt = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
    float fps = 1 / tt;
    cv::putText(_frame, cv::format("OpenCV DNN; FPS = %.2f", fps), cv::Point(10, 50), cv::FONT_HERSHEY_SIMPLEX, 1.2, cv::Scalar(0, 0, 255), 4);
    cv::imshow( "OpenCV - DNN Face Detection", _frame );
  }
  cv::destroyAllWindows();
  
  return;
}

void FaceDetector::detect() {
  int frame_height = _frame.rows;
  int frame_width = _frame.cols;

  cv::Mat input_blob = cv::dnn::blobFromImage(_frame, _in_scale_factor, cv::Size(_in_width, _in_height), _mean_val, false, false);
  _net.setInput(input_blob, "data");
  
  cv::Mat output = _net.forward("detection_out");

  cv::Mat detectionMat(output.size[2], output.size[3], CV_32F, output.ptr<float>());

  for(int i = 0; i < detectionMat.rows; i++) {
    float confidence = detectionMat.at<float>(i, 2);

    if(confidence > _conf_threshold) {
      int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * frame_width);
      int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * frame_height);
      int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * frame_width);
      int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * frame_height);

      cv::rectangle(_frame, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 255, 0),2, 4);
    }
  }
}
