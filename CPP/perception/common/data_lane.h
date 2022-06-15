#ifndef _COMMON_DATA_LANE_H_
#define _COMMON_DATA_LANE_H_

enum LineType {
  SINGLEDASH,
  SINGLESOLID,
  DOUBLEDASH,
  DOUBLESOLID,
  CURB,
  SINGLEDASH_SP,
  SINGLESOLID_SP
};

struct LinePoint {
  double conf;
  double coordinate[2];
  LineType line_type;
  double angle;
  double feature[2];
};

#endif /* _COMMON_DATA_LANE_H_ */
