import numpy as np
import matplotlib.pyplot as plt

from ..targets.lanes import Line


def lane_projection(clusters, camera):
    lines = []
    if clusters is not None:
        for points in clusters:
            line = Line(points, camera)
            lines.append(line)
            x = line.points_xy[:, 0]
            y = line.points_xy[:, 1]
            plt.scatter(-y, x, color="blue")
        plt.show()
    return
