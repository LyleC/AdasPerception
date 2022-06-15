from .obstacle import Obstacle, BBox2D
from ...sensors.camera import Camera


class PedDetails:
    def __init__(self, conf, key_points) -> None:
        self.conf = conf
        self.key_points = key_points


class Pedenstrain(Obstacle):
    def __init__(self, bbox: BBox2D, camera: Camera, details: PedDetails) -> None:
        super().__init__(bbox, camera)
        self.details = details
