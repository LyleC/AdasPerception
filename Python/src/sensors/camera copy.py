import math as m

import cv2
import numpy as np


class Camera:
    def __init__(self, intrinsics, extrinsics, image_size):
        self.intrinsics = intrinsics
        self.extrinsics = extrinsics
        self.image_size = image_size

        mat_K = self._build_intrinsic_matrix()
        mat_RT = self._build_extrinsic_matrix()
        self.trans_mat_uv2xy, self.trans_mat_xy2uv = self._calc_trans_matrix(
            mat_K, mat_RT
        )

    def _calc_trans_matrix(self, mat_k, mat_rt):
        print(mat_k)
        print(mat_rt)
        mat_krt = mat_k @ mat_rt
        mat_krt = np.delete(mat_krt, 2, axis=1)
        mat_inv = np.linalg.inv(mat_krt)
        trans_matrix_xy2uv = mat_krt / mat_krt[2, 2]
        trans_matrix_uv2xy = mat_inv / mat_inv[2, 2]
        return trans_matrix_uv2xy, trans_matrix_xy2uv

    def _build_intrinsic_matrix(self):
        return np.array(
            [
                [self.intrinsics["cx"], -self.intrinsics["fx"], 0],
                [self.intrinsics["cy"], 0, -self.intrinsics["fy"]],
                [1, 0, 0],
            ]
        )

    def _build_extrinsic_matrix(self):
        mat_r = self._build_R_matrix()
        mat_t = self._build_T_matrix()
        mat_rt = np.hstack((mat_r, mat_t))
        return mat_rt

    def _build_R_matrix(self):
        mat_r, _ = cv2.Rodrigues(
            (
                m.radians(self.extrinsics["roll"]),
                m.radians(self.extrinsics["pitch"]),
                m.radians(self.extrinsics["yaw"]),
            )
        )
        return mat_r

    def _build_T_matrix(self):
        mat_t = np.array(
            [[self.extrinsics["X"]], [self.extrinsics["Y"]], [self.extrinsics["Z"]]]
        )
        return mat_t

    def _run_projection(self, corners, matrix):
        num_points = len(corners)
        corners = np.append(corners, np.ones((num_points, 1)), axis=1)
        corners = np.transpose(corners, axes=(1, 0))
        projected = matrix @ corners
        res = projected[[0, 1], :] / projected[[2], :]
        res = np.transpose(res, axes=(1, 0))
        return res

    def project_points_xy2uv(self, corners_world):
        """
        把世界中的点对应到图像上
        """
        corners_image = self._run_projection(corners_world, self.trans_mat_xy2uv)
        return corners_image

    def project_points_uv2xy(self, corners_image):
        """
        把图像上的点对应到世界中
        """
        corners_world = self._run_projection(corners_image, self.trans_mat_uv2xy)
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
