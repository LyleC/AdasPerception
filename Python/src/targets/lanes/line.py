from ...sensors.camera import Camera


class Line:
    def __init__(self, points, camera: Camera) -> None:
        self.points = points
        self.camera = camera
        self.points_xy = self.get_points_xy()
        # print(self.points[:, 1:3])
        # print()
        # print(self.points_xy)
        # print()

    def get_points_xy(self):
        return self.camera.project_points_i2w(self.points[:, 1:3])
