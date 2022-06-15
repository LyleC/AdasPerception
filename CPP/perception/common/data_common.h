#ifndef _COMMON_DATA_COMMON_H_
#define _COMMON_DATA_COMMON_H_

#include <vector>

#include "../lanes/line.h"
#include "../obstacles/obstacle.h"

struct PredictResults {
  std::vector<Vehicle> list_veh_l;
  std::vector<Vehicle> list_veh_s;
  std::vector<Pedenstrain> list_ped_l;
  std::vector<Pedenstrain> list_ped_s;
  std::vector<BorderLine> list_line_l;
  std::vector<BorderLine> list_line_s;
};

#endif /* _COMMON_DATA_COMMON_H_ */