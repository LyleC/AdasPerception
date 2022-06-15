#ifndef _OBSTACLES_TRACK_LIST_H_
#define _OBSTACLES_TRACK_LIST_H_

#include <vector>

#include "obstacle.h"
#include "track.h"

class TrackList {
 public:
  TrackList();
  void Run(std::vector<Obstacle> &obstacles);

 private:
  void Assign(std::vector<Obstacle> &obstacles);
  void Repair();
  void Delete();
  void Create(std::vector<Obstacle> &obstacles);
  void Combine();
  void Update();

  std::vector<Track> tracks_;
};

#endif /* _OBSTACLES_TRACK_LIST_H_ */