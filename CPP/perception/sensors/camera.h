#ifndef _SENSORS_CAMERA_H_
#define _SENSORS_CAMERA_H_

#include <iostream>
#include <vector>

#include "../common/data_camera.h"
#include "opencv2/opencv.hpp"

class Camera {
 public:
  Camera(const Intrinsic intrinsic, const Extrinsic extrinsic,
         const cv::Size image_size);
  bool Update(const float delta_pitch);
  cv::Point2f ProjectPoint_UV2XY(cv::Point2f point);
  cv::Point2f ProjectPoint_XY2UV(cv::Point2f point);
  float ProjectDistance_UV2XY(float);
  float ProjectDistance_XY2UV(float);
  float ProjectWidth_UV2XY(float, float);
  float ProjectWidth_XY2UV(float, float);
  std::ostream& Print(std::ostream& os = std::cout) const;

 private:
  void CalcTransMatrix();
  cv::Mat BuildIntrinsicMatrix(Intrinsic&);
  cv::Mat BuildExtrinsicMatrix(Extrinsic&);

  Intrinsic intrinsic_;
  Extrinsic extrinsic_;
  cv::Size image_size_;
  cv::Mat intrinsic_matrix_;
  cv::Mat extrinsic_matrix_;
  cv::Mat trans_matrix_uv2xy_;
  cv::Mat trans_matrix_xy2uv_;
};

std::ostream& operator<<(std::ostream& os, const Camera& cam);

#endif /* _SENSORS_CAMERA_H_ */