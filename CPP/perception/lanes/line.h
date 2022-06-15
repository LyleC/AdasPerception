#ifndef _LANES_LINE_H_
#define _LANES_LINE_H_

#include <iostream>
#include <vector>

#include "../common/data_lane.h"
#include "../sensors/camera.h"

class Line {
 public:
  Line(Camera* pcam) : p_camera_(pcam) {}
  virtual std::ostream& Print(std::ostream& os = std::cout) const;

 protected:
  float C_[4];
  Camera* const p_camera_;
};

class BorderLine : public Line {
 public:
  BorderLine(std::vector<LinePoint>& points, Camera* pcam)
      : points_(points), Line(pcam) {}
  virtual std::ostream& Print(std::ostream& os = std::cout) const;
  const std::vector<LinePoint>& points() const { return points_; }

 private:
  const std::vector<LinePoint> points_;
};

class CenterLine : public Line {
 public:
  CenterLine(float C0, Camera* pcam) : Line(pcam) {}
  virtual std::ostream& Print(std::ostream& os = std::cout) const;

 private:
};

std::ostream& operator<<(std::ostream& os, const Line& line);

#endif /* _LANES_LINE_H_ */