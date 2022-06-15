#ifndef _LANES_ROAD_H_
#define _LANES_ROAD_H_

#include <vector>

#include "lane.h"
#include "line.h"

class Road {
 public:
 private:
  std::vector<Line> lines;
  std::vector<Lane> lanes;
};

#endif /* _LANES_ROAD_H_ */
