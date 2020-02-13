#include "text_detection.h"

void TextDetector::run() {
  _mean_val = cv::Scalar(123.68, 116.78, 103.94);

  // Network model
  _net = cv::dnn::readNet(_TFPbFile);

  // input source
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
    cv::putText(_frame, cv::format("OpenCV EAST; FPS = %.2f",fps), cv::Point(10, 50), cv::FONT_HERSHEY_SIMPLEX, 1.4, cv::Scalar(0, 0, 255), 4);
    cv::imshow( "OpenCV - EAST Text Detection", _frame );
  }
  cv::destroyAllWindows();
  
  return;
}

void TextDetector::detect() {
  cv::Mat blob;
  std::vector<cv::String> outputLayers(2);
  outputLayers[0] = "feature_fusion/Conv_7/Sigmoid";
  outputLayers[1] = "feature_fusion/concat_3";

  cv::dnn::blobFromImage(_frame, blob, _in_scale_factor, cv::Size(_in_width, _in_height), _mean_val, true, false);
  
  std::vector<cv::Mat> output;
  _net.setInput(blob);
  _net.forward(output, outputLayers);

  cv::Mat scores = output[0];
  cv::Mat geometry = output[1];

  // Decode predicted bounding boxes.
  std::vector<cv::RotatedRect> boxes;
  std::vector<float> confidences;
  decode(scores, geometry, _conf_threshold, boxes, confidences);

  // Apply non-maximum suppression procedure.
  std::vector<int> indices;
  cv::dnn::NMSBoxes(boxes, confidences, _conf_threshold, _nms_threshold, indices);

  // Render detections.
  cv::Point2f ratio((float)_frame.cols / _in_width, (float)_frame.rows / _in_height);
  for (size_t i = 0; i < indices.size(); ++i) {
    cv::RotatedRect& box = boxes[indices[i]];

    cv::Point2f vertices[4];
    box.points(vertices);
    for (int j = 0; j < 4; ++j) {
        vertices[j].x *= ratio.x;
        vertices[j].y *= ratio.y;
    }
    for (int j = 0; j < 4; ++j)
        line(_frame, vertices[j], vertices[(j + 1) % 4], cv::Scalar(0, 255, 0), 2, cv::LINE_AA);
  }
}


void decode(const cv::Mat& scores, const cv::Mat& geometry, float scoreThresh,
            std::vector<cv::RotatedRect>& detections, std::vector<float>& confidences)
{
    detections.clear();
    CV_Assert(scores.dims == 4); CV_Assert(geometry.dims == 4); CV_Assert(scores.size[0] == 1);
    CV_Assert(geometry.size[0] == 1); CV_Assert(scores.size[1] == 1); CV_Assert(geometry.size[1] == 5);
    CV_Assert(scores.size[2] == geometry.size[2]); CV_Assert(scores.size[3] == geometry.size[3]);

    const int height = scores.size[2];
    const int width = scores.size[3];
    for (int y = 0; y < height; ++y)
    {
        const float* scoresData = scores.ptr<float>(0, 0, y);
        const float* x0_data = geometry.ptr<float>(0, 0, y);
        const float* x1_data = geometry.ptr<float>(0, 1, y);
        const float* x2_data = geometry.ptr<float>(0, 2, y);
        const float* x3_data = geometry.ptr<float>(0, 3, y);
        const float* anglesData = geometry.ptr<float>(0, 4, y);
        for (int x = 0; x < width; ++x)
        {
            float score = scoresData[x];
            if (score < scoreThresh)
                continue;

            // Decode a prediction.
            // Multiple by 4 because feature maps are 4 time less than input image.
            float offsetX = x * 4.0f, offsetY = y * 4.0f;
            float angle = anglesData[x];
            float cosA = std::cos(angle);
            float sinA = std::sin(angle);
            float h = x0_data[x] + x2_data[x];
            float w = x1_data[x] + x3_data[x];

            cv::Point2f offset(offsetX + cosA * x1_data[x] + sinA * x2_data[x],
                           offsetY - sinA * x1_data[x] + cosA * x2_data[x]);
            cv::Point2f p1 = cv::Point2f(-sinA * h, -cosA * h) + offset;
            cv::Point2f p3 = cv::Point2f(-cosA * w, sinA * w) + offset;
            cv::RotatedRect r(0.5f * (p1 + p3), cv::Size2f(w, h), -angle * 180.0f / (float)CV_PI);
            detections.push_back(r);
            confidences.push_back(score);
        }
    }
}