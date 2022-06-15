from queue import Queue

import cv2
import numpy as np


class Track:
    TrackStatus = {
        0: "UNTRACKING",
        1: "INITIAL",
        2: "GROWING",
        3: "STABLE",
        4: "UNSTABLE",
    }

    def __init__(self, obst, max_size=20) -> None:
        self.track = Queue(maxsize=max_size)
        self.track.put(obst)
        self.filter = self._init_kalman_filter()

    def _init_kalman_filter(self):
        kalman = cv2.KalmanFilter(4, 2)
        kalman.measurementMatrix = np.array([[1, 0, 0, 0], [0, 1, 0, 0]], np.float32)
        kalman.transitionMatrix = np.array(
            [[1, 0, 1, 0], [0, 1, 0, 1], [0, 0, 1, 0], [0, 0, 0, 1]], np.float32
        )
        kalman.processNoiseCov = (
            np.array(
                [[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]], np.float32
            )
            * 0.003
        )
        kalman.measurementNoiseCov = np.array([[1, 0], [0, 1]], np.float32) * 0.1
        return kalman
