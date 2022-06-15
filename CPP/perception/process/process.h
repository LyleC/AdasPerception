#ifndef _PROCESS_PROCESS_H_
#define _PROCESS_PROCESS_H_

#include "../obstacles/obstacle.h"

template <typename elemType>
void MergeObstacles(std::vector<elemType>& list_main,
                    std::vector<elemType>& list_add) {
  // TODO<CL>: 大/小帧目标合并逻辑
  for (auto& it : list_main) {
    std::cout << it << std::endl;
  }
  for (auto& it : list_add) {
    std::cout << it << std::endl;
  }
}

#endif /* _PROCESS_PROCESS_H_ */