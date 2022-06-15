#ifndef _FILES_READ_FILES_H_
#define _FILES_READ_FILES_H_

#include <string>

#include "../common/data_camera.h"
#include "../lanes/line.h"
#include "../obstacles/obstacle.h"
#include "../sensors/camera.h"
#include "protobuf_reader.h"
#include "transform.pb.h"

typedef transform::proto::Frame ProtobufFrame;

int ReadCameraIntrinsic(std::string& fname, Intrinsic& intr);

int ReadCameraExtrinsic(std::string& fname, Extrinsic& extr);

void ReadProtobufFrame(ProtobufFrame& proto_frame,
                       std::vector<Vehicle>& list_veh_l,
                       std::vector<Pedenstrain>& list_ped_l,
                       std::vector<BorderLine>& list_line_l, Camera* p_camera);

#endif /* _FILES_READ_FILES_H_ */
