#include "dynamic_detection.h"
#include <vector>
#include <cmath>
#include <stdexcept>

std::vector<double> calculate_dynamic_baseline_fast_trimmed(const std::vector<double>& y, int window_size) {
    int n = y.size();
    if (n == 0) return {};

    if (window_size % 2 == 0) window_size++;
    window_size = std::min(window_size, n);
    int half = window_size / 2;


    std::vector<double> prefix(n + 1, 0.0);
    for (int i = 0; i < n; i++) prefix[i + 1] = prefix[i] + y[i];

    std::vector<double> baseline(n);
    for (int i = 0; i < n; i++) {
        int start = std::max(0, i - half);
        int end = std::min(n, i + half + 1);
        baseline[i] = (prefix[end] - prefix[start]) / (end - start);
    }
    return baseline;
}

std::vector<double> calculate_dynamic_std(const std::vector<double>& y, int window_size) {
    int n = y.size();
    if (n == 0) return {};

    if (window_size % 2 == 0) window_size++;
    window_size = std::min(window_size, n);
    int half = window_size / 2;

    std::vector<double> prefix(n + 1, 0.0), prefix_sq(n + 1, 0.0);
    for (int i = 0; i < n; i++) {
        prefix[i + 1] = prefix[i] + y[i];
        prefix_sq[i + 1] = prefix_sq[i] + y[i] * y[i];
    }

    std::vector<double> std_values(n);
    for (int i = 0; i < n; i++) {
        int start = std::max(0, i - half);
        int end = std::min(n, i + half + 1);
        int len = end - start;
        double mean = (prefix[end] - prefix[start]) / len;
        double var = (prefix_sq[end] - prefix_sq[start]) / len - mean * mean;
        std_values[i] = std::sqrt(std::max(var, 0.0)); 
    }
    return std_values;
}

std::pair<std::vector<double>, std::vector<double>> detection_y(
    const std::vector<double>& y, int window_size, double sigma
) {
    auto baseline = calculate_dynamic_baseline_fast_trimmed(y, window_size);
    auto std_dev = calculate_dynamic_std(y, window_size);

    if (baseline.size() != y.size() || std_dev.size() != y.size()) {
        throw std::runtime_error("Length mismatch");
    }

    std::vector<double> down(y.size()), up(y.size());
    for (int i = 0; i < y.size(); i++) {
        down[i] = baseline[i] - sigma * std_dev[i];
        up[i] = baseline[i] + sigma * std_dev[i];
    }
    return { down, up };
}