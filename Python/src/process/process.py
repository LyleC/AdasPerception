import cv2

from ..ego.ego_vehicle import EgoVehicle
from ..interface.plot_predicts import plot_original_predicts
from ..sensors.camera import Camera
from ..utils.io_funcs import decode_json_config


def run_perception(vcap, results, cfg):
    num_frames = int(vcap.get(7))

    # build camera
    cfg_camera = decode_json_config(cfg.configs.camera)
    camera = Camera(cfg_camera.intrinsic, cfg_camera.extrinsic, cfg_camera.size)

    # build ego vehicle
    ego_vehicle = EgoVehicle(cfg.ego.width)

    for idx_frame in range(num_frames):
        _, frame = vcap.read()

        frame_predicts = plot_original_predicts(frame, results, idx_frame)

        cv2.imshow("Predicts", frame_predicts)
        if cv2.waitKey(2) == ord("q"):
            break
    return


def init_system(cfg):

    return
