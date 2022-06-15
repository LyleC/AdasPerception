#ifndef _COMMON_DATA_OBSTACLE_H_
#define _COMMON_DATA_OBSTACLE_H_

enum BasicType { CAR, BUS, TRUCK, PERSON, RIDER };
enum VehicleType { C0, C1, B0, B1, B2, T0, T1, T2, T3, T4 };
enum ViewPoint {
  BACK,
  RIGHT_BACK,
  RIGHT,
  RIGHT_FRONT,
  FRONT,
  LEFT_FRONT,
  LEFT,
  LEFT_BACK
};

struct BBox2D {
  float conf;
  float rec[4];
  BasicType type_basic;
};

struct VehDetails {
  VehicleType type_veh;
  ViewPoint view_point;
  float side_edge;
  float tire_line[4];
};

struct PedDetails {
  float conf[7];
  float key_points[7][2];
};

#endif /* _COMMON_DATA_OBSTACLE_H_ */
