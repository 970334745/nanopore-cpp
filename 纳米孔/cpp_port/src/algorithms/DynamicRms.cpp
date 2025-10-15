#include "DynamicRms.h"
#include <cmath>
#include <algorithm>

namespace DynamicRms {

std::vector<double> calculate(const std::vector<double>& y, double window_ratio) {
    int n = (int)y.size();
    if (n == 0) return {};
    int w = std::max(1, (int)std::round(n * std::clamp(window_ratio, 0.0, 1.0)));
    int half = w / 2;
    std::vector<double> out(n, 0.0);
    for (int i = 0; i < n; ++i) {
        int s = std::max(0, i - half);
        int e = std::min(n, i + half + 1);
        double sumsq = 0.0;
        for (int j = s; j < e; ++j) sumsq += y[j] * y[j];
        double mean = sumsq / (e - s);
        out[i] = std::sqrt(mean);
    }
    return out;
}

}