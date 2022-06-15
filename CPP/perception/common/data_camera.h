#ifndef _COMMON_DATA_CAMERA_H_
#define _COMMON_DATA_CAMERA_H_

struct Intrinsic {
  float fx;
  float fy;
  float cx;
  float cy;
};

struct Extrinsic {
  float X;
  float Y;
  float Z;
  float pitch;
  float yaw;
  float roll;
};

struct ImageSize {
  size_t w;
  size_t h;
};

#endif /* _COMMON_DATA_CAMERA_H_ */