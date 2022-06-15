#ifndef _OBSTACLES_TRACK_H_
#define _OBSTACLES_TRACK_H_

#include <deque>
#include <iostream>

#include "obstacle.h"
#include "opencv2/opencv.hpp"

class Track {
  enum TrackStatus { UNTRACKING, INITIAL, GROWING, STABLE, UNSTABLE };

 public:
  Track(const Obstacle& obst);
  bool IsMatched(const Obstacle& obst, const float thresh = 0.7);
  bool IsLost();
  void UpdateTracked(const Obstacle& obst);
  void UpdateUntracked();
  std::ostream& Print(std::ostream& os = std::cout) const;

 private:
  float MatchingDistance(const Obstacle& obst);
  void Predict();

  TrackStatus status_ = UNTRACKING;
  cv::KalmanFilter filter_;
  std::deque<Obstacle> track_;  // 最新的目标新增在队列的最前端
  static constexpr size_t MAX_SIZE_ = 20;
};

std::ostream& operator<<(std::ostream& os, const Track& track);

#endif /* _OBSTACLES_TRACK_H_ */
