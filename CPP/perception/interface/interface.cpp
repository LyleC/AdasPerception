#include "interface.h"

void PlotResults(cv::Mat& frame, const PredictResults& pred_results,
                 const float scale) {
  cv::Mat frame_plot;
  cv::resize(frame, frame_plot, cv::Size(0, 0), scale, scale);
  for (auto& obj : pred_results.list_veh_l) {
    DrawVehicle(frame_plot, obj, scale);
  }
  for (auto& obj : pred_results.list_veh_s) {
    DrawVehicle(frame_plot, obj, scale);
  }
  for (auto& obj : pred_results.list_ped_l) {
    DrawPedenstrain(frame_plot, obj, scale);
  }
  for (auto& obj : pred_results.list_ped_s) {
    DrawPedenstrain(frame_plot, obj, scale);
  }
  for (auto& obj : pred_results.list_line_l) {
    DrawLaneLine(frame_plot, obj, scale);
  }
  for (auto& obj : pred_results.list_line_s) {
    DrawLaneLine(frame_plot, obj, scale);
  }

  cv::imshow("Predict", frame_plot);
  cv::waitKey(10);
}

void DrawVehicle(cv::Mat& frame, const Vehicle& vehicle, const float scale) {
  cv::Rect rect(vehicle.bbox().rec[0] * scale, vehicle.bbox().rec[1] * scale,
                vehicle.bbox().rec[2] * scale, vehicle.bbox().rec[3] * scale);
  cv::rectangle(frame, rect, cv::Scalar(255, 0, 0), 2);
}

void DrawPedenstrain(cv::Mat& frame, const Pedenstrain& pedenstrain,
                     const float scale) {
  cv::Rect rect(
      pedenstrain.bbox().rec[0] * scale, pedenstrain.bbox().rec[1] * scale,
      pedenstrain.bbox().rec[2] * scale, pedenstrain.bbox().rec[3] * scale);
  cv::rectangle(frame, rect, cv::Scalar(0, 0, 255), 2);
}

void DrawLaneLine(cv::Mat& frame, const BorderLine& line, const float scale) {
  for (auto& p : line.points()) {
    cv::Point line_point(p.coordinate[0] * scale, p.coordinate[1] * scale);
    cv::circle(frame, line_point, 2, cv::Scalar(0, 255, 0));
  }
}