import cv2

from ..ego.ego_vehicle import EgoVehicle
from ..interface.plot_predicts import plot_original_predicts
from ..interface.plot_bev import plot_bev_results
from ..sensors.camera import Camera
from ..targets.obstacles import TrackList
from ..utils.io_funcs import decode_json_config
from .process_lane import lane_projection


def run_perception(vcap, results, cfg):
    num_frames = int(vcap.get(7))

    # build camera
    cfg_camera = decode_json_config(cfg.configs.camera)
    camera = Camera(cfg_camera.intrinsic, cfg_camera.extrinsic, cfg_camera.size)

    # build ego vehicle
    ego_vehicle = EgoVehicle(cfg.ego.width)

    # build tracking list
    tracklist_veh = TrackList()
    tracklist_ped = TrackList()

    for idx_frame in range(num_frames):
        _, frame = vcap.read()

        lane_projection(results["lane_resize"][idx_frame], camera)

        frame_pred = plot_original_predicts(frame, results, idx_frame)
        frame_bev = plot_bev_results()
        cv2.imshow("Predicts", frame_pred)
        cv2.imshow("BEV", frame_bev)
        if cv2.waitKey(0) == ord("q"):
            break
    return
