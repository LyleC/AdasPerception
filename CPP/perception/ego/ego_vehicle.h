#ifndef _EGO_EGO_VEHICLE_H_
#define _EGO_EGO_VEHICLE_H_

#include <iostream>

class EgoVehicle {
 public:
  EgoVehicle(float width = 1.8);
  std::ostream& Print(std::ostream& os = std::cout) const;

 private:
  float width_;
  float speed_;
  float yaw_rate_;
};

std::ostream& operator<<(std::ostream& os, const EgoVehicle& ego);

#endif /* _EGO_EGO_VEHICLE_H_ */