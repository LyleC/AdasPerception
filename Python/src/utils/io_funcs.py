import json

import cv2
import yaml
import numpy as np
from easydict import EasyDict


def decode_json_config(cfg_file):
    with open(cfg_file, "r", encoding="utf-8") as f:
        cfg = json.load(f)
    return EasyDict(cfg)


def decode_yaml_config(cfg_file):
    with open(cfg_file, "r", encoding="utf-8") as f:
        cfg = yaml.load(f, Loader=yaml.FullLoader)
    return EasyDict(cfg)


def decode_predict_results(npz_file):
    results = np.load(npz_file, allow_pickle=True)
    return results


def decode_video(video_file):
    vcap = cv2.VideoCapture(video_file)
    return vcap


def data_check(cap, results):
    data_splits = [
        "lane_resize",
        "lane_crop",
        "veh_resize",
        "veh_crop",
        "ped_resize",
        "ped_crop",
    ]
    num_frames = int(cap.get(7))
    print(f"Totally {num_frames} frames")
    for split in data_splits:
        result = results[split]
        assert num_frames == len(result)
    return
