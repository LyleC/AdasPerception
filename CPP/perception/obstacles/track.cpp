#include "track.h"

using namespace std;

Track::Track(const Obstacle& obst) {
  // TODO<CL>:
  // Kalman滤波器初始化
  track_.push_front(obst);
}

bool Track::IsMatched(const Obstacle& obst, const float thresh) {
  // TODO<CL>:
  // 判断输入的目标与当前轨迹是否匹配
  // 调用 MatchingDistance()
  return true;
}

bool Track::IsLost() {
  // TODO<CL>:
  // 判断当前轨迹是否丢失
  return false;
}

void Track::UpdateTracked(const Obstacle& obst) {
  // TODO<CL>:
  // 根据最新匹配上的目标，更新轨迹
  // 调用 Predict()
}

void Track::UpdateUntracked() {
  // TODO<CL>:
  // 在未匹配上目标的情况下，更新轨迹
  // 调用 Predict()
}

float Track::MatchingDistance(const Obstacle& obst) {
  // TODO<CL>:
  // 计算当前目标与轨迹的匹配距离（匹配代价）
  // 预留，未来可扩展为基于代价矩阵的匹配
  float dist;
  return dist;
}

void Track::Predict() {
  // TODO<CL>:
  // 根据轨迹的当前状态，预测下一帧的状态
}

ostream& Track::Print(ostream& os) const {
  // TODO<CL>:
  os << "Track: " << track_[0] << endl;
  return os;
}

ostream& operator<<(ostream& os, const Track& track) { return track.Print(os); }