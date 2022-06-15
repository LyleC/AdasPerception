from .obstacle import Obstacle, BBox2D
from ...sensors.camera import Camera


class VehDetails:
    VEHICLE_TYPE = {
        0: "C0",
        1: "C1",
        2: "B0",
        3: "B1",
        4: "B2",
        5: "T0",
        6: "T1",
        7: "T2",
        8: "T3",
        9: "T4",
    }

    VIEWPOINT = {
        0: "BACK",
        1: "RIGHT_BACK",
        2: "RIGHT",
        3: "RIGHT_FRONT",
        4: "FRONT",
        5: "LEFT_FRONT",
        6: "LEFT",
        7: "LEFT_BACK",
    }

    def __init__(self, type: int, viewpoint: int, side_edge, tire_line) -> None:
        self.type_detail = type
        self.viewpoint = viewpoint
        self.side_edge = side_edge
        self.tire_line = tire_line


class Vehicle(Obstacle):
    def __init__(self, bbox: BBox2D, camera: Camera, details: VehDetails) -> None:
        super().__init__(bbox, camera)
        self.details = details
