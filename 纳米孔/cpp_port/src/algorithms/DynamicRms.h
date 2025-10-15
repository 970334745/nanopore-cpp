#pragma once
#include <vector>

namespace DynamicRms {

// 窗口大小为数据长度的比例（0-1），返回每点的RMS
std::vector<double> calculate(const std::vector<double>& y, double window_ratio);

}