import math as m

import numpy as np


class Camera:
    def __init__(self, intrinsics, extrinsics, image_size):
        self.intrinsics = intrinsics
        self.extrinsics = extrinsics
        self.image_size = image_size

        self.fx = intrinsics["fx"]
        self.fy = intrinsics["fy"]
        self.u0 = intrinsics["u0"]
        self.v0 = intrinsics["v0"]
        self.K = self._build_intrinsic_matrix()
        self.x = -displacement["X"]
        self.y = -displacement["Y"]
        self.z = -displacement["H"]
        self.T_cw = self._build_T_cw()
        self.roll = angles["roll"]
        self.pitch = angles["pitch"]
        self.yaw = angles["yaw"]
        self.R_cw = self._reconstruct_R_cw_from_Euler_Angles()
        RT = np.append(self.R_cw, self.T_cw, axis=1)
        K_RT = self.K @ RT
        self.P = np.delete(K_RT, 2, 1)  # 画没有Z方向的2D点， 用这个self.P
        # self.P = K_RT # 如果画3D点到画面中，用这个self.P
        self.Q = np.linalg.inv(self.P)

        return

    def _build_intrinsic_matrix(self):
        """
        从焦距fx,fy和画面中心点u0,v0求相机内参矩阵。
        """
        return np.array(
            [
                [self.intrinsics["fx"], 0, self.intrinsics["cx"]],
                [0, self.intrinsics["fy"], self.intrinsics["cy"]],
                [0, 0, 1],
            ]
        )

    def _build_extrinsic_matrix(self):

        return

    def _build_T_cw(self):
        """
        世界O在相机O中的坐标
        """
        T_cw = np.array([[self.x], [self.y], [self.z]])
        return T_cw

    def _build_R_cw(self):
        """
        从roll,pitch,yaw来构造世界坐标系到相机坐标系的旋转矩阵R_cw
        """
        alpha = m.radians(self.roll)
        beta = m.radians(self.pitch)
        gamma = m.radians(self.yaw)
        Rx = np.array(
            [
                [1, 0, 0],
                [0, m.cos(alpha), -m.sin(alpha)],
                [0, m.sin(alpha), m.cos(alpha)],
            ]
        )
        Ry = np.array(
            [[m.cos(beta), 0, m.sin(beta)], [0, 1, 0], [-m.sin(beta), 0, m.cos(beta)]]
        )
        Rz = np.array(
            [
                [m.cos(gamma), -m.sin(gamma), 0],
                [m.sin(gamma), m.cos(gamma), 0],
                [0, 0, 1],
            ]
        )
        R_cw = Rx @ Ry @ Rz
        return R_cw

    def manip_projection(self, corners, A):
        N = len(corners)
        corners = np.append(corners, np.ones((N, 1)), axis=1)
        corners = np.transpose(corners, axes=(1, 0))
        matrix = A @ corners
        res = matrix[[0, 1], :] / matrix[[2], :]
        res = np.transpose(res, axes=(1, 0))
        return res

    def project_points_w2i(self, corners_world):
        """
        把世界中的点对应到图像上
        Args:
            input: a numpy array of shape N x 2
            input: a numpy array of shape N x 2
        """
        # TODO: try... exception: too large or too small 1920x1080
        corners_image = self.manip_projection(corners_world, self.P).astype(float)
        return corners_image

    def project_points_i2w(self, corners_image):
        """
        把图像上的点对应到世界中
        """
        corners_world = self.manip_projection(corners_image, self.Q).astype(float)
        return corners_world

    def project_points_v2x(self, array_v):  # TODO： debug this one
        """
        把图像上等间隔的v投影到世界中得到不等间隔的x
        """
        N = len(array_v)
        Q = np.delete(self.Q, 1, 1)
        Q = np.delete(Q, 0, 0)
        print(f"self.Q:\n{self.Q}\n{self.Q.shape}")
        print(f"Q:\n{Q}\n{Q.shape}")
        array_v = np.expand_dims(array_v, axis=1)
        array_v = np.append(array_v, np.ones((N, 1)), axis=1)
        array_v = np.transpose(array_v, axes=(1, 0))
        print(f"array_v:\n{array_v}\n{array_v.shape}")
        matrix = Q @ array_v
        print(f"matrix:\n{matrix}\n{matrix.shape}")
        array_x = np.squeeze(matrix[[0], :] / matrix[[1], :], axis=0)

        return array_x  # 与底边高度测距 同

    def pxl2deg_updown(self):
        """
        靠近相机中心点的画面中，上下动1个像素代表多少角度
        """
        angle = m.atan(1 / self.fy)  # in radian
        return m.degrees(angle)  # in degree

    def pxl2deg_leftright(self):
        """
        靠近相机中心点的画面中，上下动1个像素代表多少角度
        """
        angle = m.atan(1 / self.fx)  # in radian
        return m.degrees(angle)  # in degree

    def deg2pxl(self):
        """
        靠近相机中心点的画面中，镜头上下动一度，代表多少像素
        """
        pxl = self.fy * m.tan(m.radians(1))
        return pxl

    def estimate_distance_from_pixel_width(self, W, p1, p2):
        """
        根据车辆宽度估算距离
        """
        u = abs(p1[0] - p2[0])
        print(f"u: {u}")
        D = self.fx * (W / (u + 1e-7))
        return D

    def estimate_distance_from_pixel_height(self, H, p1, p2):
        """
        根据车辆高度估算距离
        """
        v = abs(p1[1] - p2[1])
        print(f"v: {v}")
        D = self.fy * (H / (v + 1e-7))
        return D

    def estimate_pixel_width_from_distance(self, W, D):
        """
        根据车辆高度&距离估算画面上的像素宽度
        """
        # u = int((self.fx * W) / (D + 1e-7))
        u = np.array((self.fx * W) / (D + 1e-7), dtype=float)
        return u

    def estimate_pixel_height_from_distance(self, H, D):
        """
        根据车辆高度&距离估算画面上的像素高度
        """
        v = int((self.fy * H) / (D + 1e-7))
        return v
