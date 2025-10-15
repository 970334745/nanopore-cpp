#include "Integral.h"
#include <cmath>

namespace Integral {

double area(const std::vector<double>& y, double baseline, int sampling, int signal_direction) {
    if (y.size() < 2 || sampling <= 0) return 0.0;
    double dt = 1.0 / sampling;
    double sum = 0.0;
    for (size_t i = 0; i + 1 < y.size(); ++i) {
        double a = y[i];
        double b = y[i + 1];
        double mx = std::max(a, b);
        double mn = std::min(a, b);
        double trapezoid = 0.5 * (mx - mn) * dt;
        double rect = 0.0;
        if (signal_direction == 1) {
            rect = (mn - baseline) * dt;
        } else if (signal_direction == 2) {
            rect = (baseline - mn) * dt;
        } else {
            rect = (mn - baseline) * dt; // 默认按正向处理
        }
        sum += trapezoid + rect;
    }
    return std::abs(sum);
}

}