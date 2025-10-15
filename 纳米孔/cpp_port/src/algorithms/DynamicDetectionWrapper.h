#pragma once
#include <vector>

namespace DynamicDetectionWrapper {

std::vector<double> baseline(const std::vector<double>& y, int window_samples);
std::vector<double> stddev(const std::vector<double>& y, int window_samples);
std::pair<std::vector<double>, std::vector<double>> thresholds(const std::vector<double>& y, int window_samples, double sigma);

}