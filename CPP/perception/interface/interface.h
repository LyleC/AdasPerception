#ifndef _INTERFACE_INTERFACE_H_
#define _INTERFACE_INTERFACE_H_

#include <vector>

#include "../common/data_common.h"
#include "opencv2/opencv.hpp"

void PlotResults(cv::Mat& frame, const PredictResults& pred_results,
                 const float scale = 0.5);
void DrawVehicle(cv::Mat& frame, const Vehicle& vehicle, const float scale);
void DrawPedenstrain(cv::Mat& frame, const Pedenstrain& pedenstrain,
                     const float scale);
void DrawLaneLine(cv::Mat& frame, const BorderLine& line, const float scale);

#endif /* _INTERFACE_INTERFACE_H_ */