import cv2
import numpy as np


def draw_lane_points(image, clusters, scale, crop=None):
    COLOURS = [
        (0, 255, 0),
        (255, 0, 0),
        (120, 255, 0),
        (255, 0, 120),
        (0, 0, 255),
        (0, 120, 120),
        (120, 120, 0),
        (0, 0, 0),
    ]

    if clusters is not None:
        for points in clusters:
            p_last = (
                int(points[0, 1] * scale),
                int(points[0, 2] * scale),
            )
            for p in points[1:]:
                p_cur = (
                    int(p[1] * scale),
                    int(p[2] * scale),
                )
                color = COLOURS[int(p[4])]
                cv2.circle(image, p_cur, 3, color, 1)
                cv2.line(image, p_last, p_cur, color, 1)
                p_last = p_cur
    if crop is not None:
        p1 = (int(crop[0] * scale), int(crop[1] * scale))
        p2 = (int(crop[2] * scale), int(crop[3] * scale))
        cv2.rectangle(image, p1, p2, (255, 255, 255), 1)
    return image


def draw_vehicles(image, objects, scale, crop=None):
    COLOURS = [
        (0, 255, 0),
        (255, 0, 0),
        (120, 255, 0),
        (255, 0, 120),
        (0, 0, 255),
    ]

    if objects is not None:
        for obj in objects:
            if obj[6] > 0:
                obj[7:11] *= scale
                obj[14:19] *= scale
                image = render_details(image, obj[6:], 2)
            p1 = (int(obj[1] * scale), int(obj[2] * scale))
            p2 = (int(obj[3] * scale), int(obj[4] * scale))
            clss = int(obj[5])
            cv2.rectangle(image, p1, p2, COLOURS[clss], 1)
    if crop is not None:
        p1 = (int(crop[0] * scale), int(crop[1] * scale))
        p2 = (int(crop[2] * scale), int(crop[3] * scale))
        cv2.rectangle(image, p1, p2, (0, 0, 0), 1)
    return image


def render_details(img, details, thickness=1):
    (
        left,
        top,
        right,
        bottom,
        cls_c,
        cls_b,
        cls_t,
        side_edge_x,
        x1,
        y1,
        x2,
        y2,
        viewpoint,
    ) = details[1:]
    blue = (255, 0, 0)
    yellow = (0, 255, 255)
    green = (0, 255, 0)
    if np.sum((np.isnan(x1), np.isnan(x2), np.isnan(y1), np.isnan(y2))) == 0:
        cv2.line(img, (int(x1), int(y1)), (int(x2), int(y2)), green, thickness)
    if not np.isnan(side_edge_x):
        cv2.line(
            img,
            (int(side_edge_x), int(top)),
            (int(side_edge_x), int(bottom)),
            yellow,
            thickness,
        )
    cv2.rectangle(
        img, (int(left), int(top)), (int(right), int(bottom)), blue, thickness
    )
    return img


def draw_pedenstrains(image, objects, scale, crop=None):
    COLOURS = [
        (0, 255, 0),
        (255, 0, 0),
        (0, 0, 255),
    ]

    if objects is not None:
        for obj in objects:
            p1 = (int(obj[1] * scale), int(obj[2] * scale))
            p2 = (int(obj[3] * scale), int(obj[4] * scale))
            clss = int(obj[5])
            cv2.rectangle(image, p1, p2, COLOURS[clss], 1)
            if obj[6] > 0:
                image = render_kpoints(image, obj[7:], scale)
    if crop is not None:
        p1 = (int(crop[0] * scale), int(crop[1] * scale))
        p2 = (int(crop[2] * scale), int(crop[3] * scale))
        cv2.rectangle(image, p1, p2, (0, 0, 0), 1)
    return image


def render_kpoints(image, kpoints, scale):
    for idx in range(7):
        x = int(kpoints[3 * idx + 1] * scale)
        y = int(kpoints[3 * idx + 2] * scale)
        cv2.circle(image, (x, y), 3, (0, 0, 255), -1)
    return image


def plot_original_predicts(frame, results, idx, scale=0.5):
    frame_plot = cv2.resize(frame, (0, 0), fx=scale, fy=scale)

    frame_plot = draw_lane_points(frame_plot, results["lane_resize"][idx], scale)
    frame_plot = draw_vehicles(frame_plot, results["veh_resize"][idx], scale)
    frame_plot = draw_pedenstrains(frame_plot, results["ped_resize"][idx], scale)

    return frame_plot
