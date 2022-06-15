#include "obstacle.h"

using namespace std;

// Obstacle

cv::Point2f Obstacle::DistanceFromBottom() {
  // 根据目标底边接地点投影来计算横、纵向距离
  // TODO<CL>: 具体实现
  cv::Point2f bottom;
  return p_camera_->ProjectPoint_UV2XY(bottom);
}

float Obstacle::DistanceFromLaneWidth() {
  // 根据目标底边所在位置的车道线像素宽，推算目标纵向距离
  // 需要的先验：车道线实际宽度
  // TODO<CL>: 具体实现
  float distance;
  return distance;
}

ostream &Obstacle::Print(ostream &os) const {
  cout << "[" << bbox_.rec[0] << " " << bbox_.rec[1] << " " << bbox_.rec[2]
       << " " << bbox_.rec[3] << "] (" << bbox_.type_basic << ")";
  return os;
}

// Vehicle

const float Vehicle::width_prior_basic_[3] = {1.7, 2.3, 2.3};
const float Vehicle::width_prior_detail_[10] = {1.5, 1.7, 1.7, 2.0, 2.4,
                                                1.6, 1.8, 2.0, 2.4, 2.4};

float Vehicle::DistanceFromVehProperty() {
  // 根据车辆的像素宽度，以及其他附加属性，推算目标纵向距离
  // 需要的先验：详细车型及其实际宽度
  // TODO<CL>: 具体实现
  float distance;
  return distance;
}

ostream &Vehicle::Print(ostream &os) const {
  cout << "[" << bbox_.rec[0] << " " << bbox_.rec[1] << " " << bbox_.rec[2]
       << " " << bbox_.rec[3] << "] (" << bbox_.type_basic << ") "
       << bbox_.conf;
  return os;
}

// Pedenstrain

float Pedenstrain::DistanceFromKeyPoints() {
  // 根据行人各关键点的相互位置关系，推算目标纵向距离
  // TODO<CL>: 具体实现
  float distance;
  return distance;
}

ostream &Pedenstrain::Print(ostream &os) const {
  cout << "[" << bbox_.rec[0] << " " << bbox_.rec[1] << " " << bbox_.rec[2]
       << " " << bbox_.rec[3] << "] (" << bbox_.type_basic << ") "
       << bbox_.conf;
  return os;
}

ostream &operator<<(ostream &os, const Obstacle &obst) {
  return obst.Print(os);
}
