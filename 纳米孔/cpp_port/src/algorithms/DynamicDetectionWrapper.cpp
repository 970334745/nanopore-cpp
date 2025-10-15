#include "DynamicDetectionWrapper.h"
#include "dynamic_detection.h"

namespace DynamicDetectionWrapper {

std::vector<double> baseline(const std::vector<double>& y, int window_samples) {
    return calculate_dynamic_baseline_fast_trimmed(y, window_samples);
}

std::vector<double> stddev(const std::vector<double>& y, int window_samples) {
    return calculate_dynamic_std(y, window_samples);
}

std::pair<std::vector<double>, std::vector<double>> thresholds(const std::vector<double>& y, int window_samples, double sigma) {
    return detection_y(y, window_samples, sigma);
}

}