#include "track_list.h"

using namespace std;

TrackList::TrackList() {}

void TrackList::Run(vector<Obstacle> &obstacles) {
  for (auto &track : tracks_) {
    for (auto &obst : obstacles) {
      if (track.IsMatched(obst)) {
        track.UpdateTracked(obst);
        break;
      }
    }
  }

  Assign(obstacles);
  Repair();
  Delete();
  Create(obstacles);
  Combine();
  return;
}

void TrackList::Assign(vector<Obstacle> &obstacles) {}

void TrackList::Repair() {}

void TrackList::Delete() {}

void TrackList::Create(vector<Obstacle> &obstacles) {}

void TrackList::Combine() {}

void TrackList::Update() {}
