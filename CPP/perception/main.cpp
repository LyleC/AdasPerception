#include <algorithm>
#include <iostream>
#include <vector>

#include "common/data_camera.h"
#include "common/data_lane.h"
#include "common/data_obstacle.h"
#include "ego/ego_vehicle.h"
#include "files/read_files.h"
#include "interface/interface.h"
#include "lanes/lane.h"
#include "lanes/line.h"
#include "lanes/road.h"
#include "obstacles/obstacle.h"
#include "obstacles/track.h"
#include "obstacles/track_list.h"
#include "opencv2/opencv.hpp"
#include "process/process.h"
#include "sensors/camera.h"

using namespace std;

int main(int argc, char **argv) {
  cout << "hello world!" << endl;

  if (2 > argc) {
    cerr << "usage: ./perception bin_file" << endl;
    return -1;
  }

  // 读取相机内外参，建立相机模型
  string fname_in = "../configs/intrinsics.txt";
  string fname_ex = "../configs/extrinsics.txt";
  Intrinsic intrinsic;
  Extrinsic extrinsic;
  ReadCameraIntrinsic(fname_in, intrinsic);
  ReadCameraExtrinsic(fname_ex, extrinsic);
  Camera *p_camera = new Camera(intrinsic, extrinsic, cv::Size(1920, 1080));

  // 读取自车参数，建立自车模型
  float vehicle_width = 1.7;
  EgoVehicle *p_ego_veh = new EgoVehicle(vehicle_width);

  // 建立车辆、行人轨迹库
  TrackList *p_tracklist_veh = new TrackList();
  TrackList *p_tracklist_ped = new TrackList();

  // 读取预测结果
  string bin_path = string(argv[1]);
  ProtobufReader *proto_reader = new ProtobufReader();
  if (!proto_reader->Open(bin_path.c_str())) {
    cerr << "bin file read error." << endl;
    return -1;
  }

  // 读取视频文件
  string video_path = string(argv[1]);
  video_path.replace(video_path.find_last_of("."), 4, ".mp4");
  size_t pos = video_path.find("/bin/");
  video_path.erase(pos, 5);
  video_path.insert(pos, "/video/");
  cv::VideoCapture *p_cap = new cv::VideoCapture(video_path);
  if (!p_cap->isOpened()) {
    cerr << "video file read error" << endl;
    return -1;
  }

  ProtobufFrame proto_frame;
  vector<Vehicle> list_veh_l;
  vector<Pedenstrain> list_ped_l;
  vector<BorderLine> list_line_l;
  vector<Vehicle> list_veh_s;
  vector<Pedenstrain> list_ped_s;
  vector<BorderLine> list_line_s;
  PredictResults pred_results;

  size_t idx_frame = 0;
  while (proto_reader->Read(proto_frame)) {
    cout << "IdxFrame: " << idx_frame++ << endl;

    cv::Mat frame;
    *p_cap >> frame;

    // 3 帧一组，找到每组开头，也就是大图
    if (transform::proto::Region::FULL != proto_frame.region()) {
      continue;
    }
    // large
    ReadProtobufFrame(proto_frame, list_veh_l, list_ped_l, list_line_l,
                      p_camera);

    proto_frame.Clear();
    if (!proto_reader->Read(proto_frame)) {
      break;
    }
    // small
    ReadProtobufFrame(proto_frame, list_veh_s, list_ped_s, list_line_s,
                      p_camera);
    // post, not used
    proto_frame.Clear();
    if (!proto_reader->Read(proto_frame)) {
      break;
    }

    // process
    MergeObstacles(list_veh_l, list_veh_s);
    // MergeObstacles(list_ped_l, list_ped_s);

    // plot results
    pred_results = {list_veh_l, list_veh_s,  list_ped_l,
                    list_ped_s, list_line_l, list_line_s};
    PlotResults(frame, pred_results);

    // clear and repeat
    list_veh_l.clear();
    list_veh_s.clear();
    list_ped_l.clear();
    list_ped_s.clear();
    list_line_l.clear();
    list_line_s.clear();
    proto_frame.Clear();
    cout << endl;
  }

  delete proto_reader;
  delete p_camera;
  delete p_ego_veh;
  delete p_tracklist_veh;
  delete p_tracklist_ped;
  return 0;
}
