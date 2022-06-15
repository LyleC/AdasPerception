#include "read_files.h"

using std::ifstream;
using std::string;
using std::vector;

int ReadCameraIntrinsic(string &fname, Intrinsic &intr) {
  ifstream fin;
  fin.open(fname, std::ios::in);
  if (!fin.is_open()) {
    std::cerr << "Can't open intrinsic file" << std::endl;
    return -1;
  }
  char buf[100] = {0};
  vector<float> params;
  while (fin.getline(buf, sizeof(buf))) {
    params.push_back(atof(buf));
  }
  fin.close();
  intr.fx = params[0];
  intr.fy = params[1];
  intr.cx = params[2];
  intr.cy = params[3];
  return 0;
}

int ReadCameraExtrinsic(string &fname, Extrinsic &extr) {
  ifstream fin;
  fin.open(fname, std::ios::in);
  if (!fin.is_open()) {
    std::cerr << "Can't open extrinsic file" << std::endl;
    return -1;
  }
  char buf[100] = {0};
  vector<float> params;
  while (fin.getline(buf, sizeof(buf))) {
    params.push_back(atof(buf));
  }
  fin.close();
  extr.X = params[0];
  extr.Y = params[1];
  extr.Z = params[2];
  extr.pitch = params[3];
  extr.yaw = params[4];
  extr.roll = params[5];
  return 0;
}

void ReadProtobufFrame(ProtobufFrame &proto_frame, vector<Vehicle> &list_veh,
                       vector<Pedenstrain> &list_ped,
                       vector<BorderLine> &list_line, Camera *p_camera) {
  // vehicle
  for (int i = 0; i < proto_frame.object_size(); i++) {
    auto &pb_veh = proto_frame.object(i);
    auto &bbox = pb_veh.bbox();
    BBox2D bbox_2d = {pb_veh.confidence(),
                      {bbox.x(), bbox.y(), bbox.width(), bbox.height()},
                      (BasicType)pb_veh.class_()};
    VehDetails details = {(VehicleType)pb_veh.detail_class(),
                          (ViewPoint)pb_veh.viewpoint(),
                          pb_veh.side_ratio(),
                          {pb_veh.tire_line().x1(), pb_veh.tire_line().y1(),
                           pb_veh.tire_line().x2(), pb_veh.tire_line().y2()}};
    Vehicle cur_vehicle(bbox_2d, p_camera, details);
    list_veh.push_back(cur_vehicle);
  }
  // pedenstrian
  for (int i = 0; i < proto_frame.ped_size(); i++) {
    auto &pb_ped = proto_frame.ped(i);
    auto &bbox = pb_ped.bbox();
    auto &ped_points = pb_ped.ppoint();
    BBox2D bbox_2d = {pb_ped.confidence(),
                      {bbox.x(), bbox.y(), bbox.width(), bbox.height()},
                      (BasicType)pb_ped.class_()};
    PedDetails details = {
        {ped_points.conf1(), ped_points.conf2(), ped_points.conf3(),
         ped_points.conf4(), ped_points.conf5(), ped_points.conf6(),
         ped_points.conf7()},
        {{ped_points.x1(), ped_points.y1()},
         {ped_points.x2(), ped_points.y2()},
         {ped_points.x3(), ped_points.y3()},
         {ped_points.x4(), ped_points.y4()},
         {ped_points.x5(), ped_points.y5()},
         {ped_points.x6(), ped_points.y6()},
         {ped_points.x7(), ped_points.y7()}}};
    Pedenstrain cur_pedenstrain(bbox_2d, p_camera, details);
    list_ped.push_back(cur_pedenstrain);
  }
  // lane
  for (int i = 0; i < proto_frame.lane_size(); i++) {
    auto &pb_lane = proto_frame.lane(i);
    vector<LinePoint> line_points;
    for (int j = 0; j < pb_lane.lane_origin_size(); j++) {
      auto &pbl_p = pb_lane.lane_origin(j);
      LinePoint line_point = {pbl_p.conf(),
                              {pbl_p.x(), pbl_p.y()},
                              (LineType)pbl_p.tps(),
                              pbl_p.k(),
                              {pbl_p.f1(), pbl_p.f2()}};
      line_points.push_back(line_point);
    }
    BorderLine cur_line(line_points, p_camera);
    list_line.push_back(cur_line);
  }
}