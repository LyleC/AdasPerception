from ...sensors.camera import Camera


class BBox2D:
    BasicType = {0: "CAR", 1: "BUS", 2: "TRUCK", 3: "PERSON", 4: "RIDER"}

    def __init__(self, rec, type: BasicType, conf: float = 0) -> None:
        self.rec = rec
        self.conf = conf
        self.type = type


class Obstacle:
    def __init__(self, bbox: BBox2D, camera: Camera) -> None:
        self.bbox = bbox
        self.camera = camera
