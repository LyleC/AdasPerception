#ifndef _LANES_LANE_H_
#define _LANES_LANE_H_

#include <iostream>

#include "line.h"

class Lane {
 public:
  Lane(Line* pleft, Line* pright)
      : p_line_left_(pleft), p_line_right_(pright) {}
  std::ostream& Print(std::ostream& os = std::cout) const;

 private:
  Line* p_line_left_;
  Line* p_line_right_;
  Line* p_line_center_;
};

std::ostream& operator<<(std::ostream& os, const Lane& lane);

#endif /* _LANES_LANE_H_ */
