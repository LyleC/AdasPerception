import sys
from pathlib import Path

sys.path.append(str(Path(__file__).resolve().parents[1]))

from src.sensors.camera import Camera
from src.utils.io_funcs import decode_json_config


def test_camera():
    cfg_camera = decode_json_config("./configs/camera.json")
    camera = Camera(cfg_camera.intrinsic, cfg_camera.extrinsic, cfg_camera.size)
    return


if __name__ == "__main__":
    test_camera()
