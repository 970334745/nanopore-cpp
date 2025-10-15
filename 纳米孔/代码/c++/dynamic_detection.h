#ifndef DYNAMIC_DETECTION_H
#define DYNAMIC_DETECTION_H

#include <vector>


std::vector<double> calculate_dynamic_baseline_fast_trimmed(const std::vector<double>& y, int window_size);


std::vector<double> calculate_dynamic_std(const std::vector<double>& y, int window_size);

std::pair<std::vector<double>, std::vector<double>> detection_y(
    const std::vector<double>& y, int window_size, double sigma
);

#endif // DYNAMIC_DETECTION_H