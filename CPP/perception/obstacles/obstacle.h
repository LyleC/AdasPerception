#ifndef _OBSTACLES_OBSTACLE_H_
#define _OBSTACLES_OBSTACLE_H_

#include <iostream>

#include "../common/data_obstacle.h"
#include "../sensors/camera.h"

class Obstacle {
 public:
  Obstacle(const BBox2D& bbox, Camera* const pcam)
      : bbox_(bbox), p_camera_(pcam) {}
  cv::Point2f DistanceFromBottom();
  float DistanceFromLaneWidth();
  virtual std::ostream& Print(std::ostream& os = std::cout) const;
  const BBox2D bbox() const { return bbox_; }

 protected:
  const BBox2D bbox_;
  Camera* const p_camera_;
};

class Vehicle : public Obstacle {
 public:
  Vehicle(const BBox2D& bbox, Camera* const pcam, const VehDetails& details)
      : Obstacle(bbox, pcam), details_(details) {}
  float DistanceFromVehProperty();
  virtual std::ostream& Print(std::ostream& os = std::cout) const;

 private:
  const VehDetails details_;
  static const float width_prior_basic_[3];
  static const float width_prior_detail_[10];
};

class Pedenstrain : public Obstacle {
 public:
  Pedenstrain(const BBox2D& bbox, Camera* const pcam, const PedDetails& details)
      : Obstacle(bbox, pcam), details_(details) {}
  float DistanceFromKeyPoints();
  virtual std::ostream& Print(std::ostream& os = std::cout) const;

 private:
  const PedDetails details_;
};

std::ostream& operator<<(std::ostream& os, const Obstacle& obst);

#endif /* _OBSTACLES_OBSTACLE_H_ */
