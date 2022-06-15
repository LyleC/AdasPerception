#include "ego_vehicle.h"

using namespace std;

EgoVehicle::EgoVehicle(float width) : width_(width) {}

ostream &EgoVehicle::Print(ostream &os) const {
  os << "Vehicle params:" << width_ << endl;
  os << "Vehicle status:" << speed_ << " " << yaw_rate_ << endl;
  return os;
}

ostream &operator<<(ostream &os, const EgoVehicle &ego) {
  return ego.Print(os);
}