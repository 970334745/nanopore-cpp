#include "EventDetection.h"
#include "Integral.h"
#include <algorithm>
#include <cmath>

namespace {
inline double round6(double v) { return std::round(v * 1e6) / 1e6; }
}

namespace EventDetection {

static int clamp_index(int idx, int n) { return std::max(0, std::min(idx, n - 1)); }

EventDetectionResult detect(
    const std::vector<double>& data_x,
    const std::vector<double>& data_y,
    const std::vector<double>& baseline_data,
    const std::vector<double>& std_up_data,
    const std::vector<double>& std_down_data,
    const Params& p
) {
    EventDetectionResult res;
    int n = static_cast<int>(data_y.size());
    if (n == 0 || (int)data_x.size() != n || (int)baseline_data.size() != n) return res;

    int window_samples = std::max(1, (int)(p.sampling * (p.window_size_ms / 1000.0)));
    int current_pos = 0;
    int last_start_index = -1;

    while (current_pos < n) {
        int search_end = std::min(n - 1, current_pos + window_samples);

        // 方向选择
        if (p.signal_direction == 1 || p.signal_direction == 3) {
            // 正峰：在窗口中找最大值
            auto max_it = std::max_element(data_y.begin() + current_pos, data_y.begin() + search_end + 1);
            int peak_index = (int)std::distance(data_y.begin(), max_it);
            double peak_value = *max_it;
            if (peak_index >= n) break;

            // 阈值判断
            if (peak_index < (int)std_up_data.size() && peak_value > std_up_data[peak_index]) {
                // 起始点：往回到10%幅度
                int back_limit = std::max(0, peak_index - 10000);
                int start_index = peak_index;
                for (int i = peak_index; i >= back_limit; --i) {
                    double thr = baseline_data[i] + (peak_value - baseline_data[i]) * 0.1;
                    if (data_y[i] >= thr) { start_index = i; }
                    else break;
                }

                // 终点：往前到10%幅度
                int fwd_limit = std::min(n - 1, peak_index + 10000);
                int end_index = peak_index;
                for (int i = peak_index; i <= fwd_limit; ++i) {
                    double thr = baseline_data[i] + (peak_value - baseline_data[i]) * 0.1;
                    if (data_y[i] >= thr) { end_index = i; }
                    else break;
                }

                double rise = data_x[peak_index] - data_x[start_index];
                double decay = data_x[end_index] - data_x[peak_index];
                double duration = rise + decay; // 秒
                double amplitude = peak_value - baseline_data[start_index];

                // 约束
                if (amplitude >= p.min_amplitude &&
                    duration * 1000.0 >= p.min_duration_ms &&
                    duration * 1000.0 <= p.max_duration_ms &&
                    start_index != last_start_index) {

                    // 子序列
                    std::vector<double> yseg(data_y.begin() + start_index, data_y.begin() + end_index + 1);
                    double integral_area = Integral::area(yseg, baseline_data[start_index], p.sampling, 1);

                    // theta_duration：按照70%幅度的时间跨度（简化版，与Python相仿）
                    double thr70 = baseline_data[peak_index] + (peak_value - baseline_data[peak_index]) * 0.7;
                    int left = start_index, right = end_index;
                    for (int i = peak_index; i >= start_index; --i) {
                        if (data_y[i] >= thr70) left = i; else break;
                    }
                    for (int i = peak_index; i <= end_index; ++i) {
                        if (data_y[i] >= thr70) right = i; else break;
                    }
                    double theta_duration = (data_x[right] - data_x[left]) * 1000.0;

                    // 上升/下降积分比值
                    std::vector<double> yrise(data_y.begin() + start_index, data_y.begin() + peak_index + 1);
                    std::vector<double> ydecay(data_y.begin() + peak_index, data_y.begin() + end_index + 1);
                    double rise_int = Integral::area(yrise, baseline_data[start_index], p.sampling, 1);
                    double decay_int = Integral::area(ydecay, baseline_data[peak_index], p.sampling, 1);
                    double ratio = (rise_int > 0.0) ? (decay_int / rise_int) : 0.0;

                    Event ev;
                    ev.start_index = start_index;
                    ev.end_index = end_index;
                    ev.peak_index = peak_index;
                    ev.amplitude = amplitude;
                    ev.duration_ms = duration * 1000.0;
                    ev.integral_area = integral_area;
                    ev.theta_duration_ms = theta_duration;
                    ev.integral_ratio = ratio;
                    res.events.push_back(ev);

                    last_start_index = start_index;
                    current_pos = end_index + 1;
                    if ((int)res.events.size() >= p.event_number) break;
                    continue; // 下一轮
                }
            }
        }

        if (p.signal_direction == 2 || p.signal_direction == 3) {
            // 负峰：在窗口中找最小值
            auto min_it = std::min_element(data_y.begin() + current_pos, data_y.begin() + search_end + 1);
            int trough_index = (int)std::distance(data_y.begin(), min_it);
            double trough_value = *min_it;
            if (trough_index >= n) break;

            if (trough_index < (int)std_down_data.size() && trough_value < std_down_data[trough_index]) {
                // 起始点（负向10%幅度）
                int back_limit = std::max(0, trough_index - 10000);
                int start_index = trough_index;
                for (int i = trough_index; i >= back_limit; --i) {
                    double thr = baseline_data[i] - (baseline_data[i] - trough_value) * 0.1;
                    if (data_y[i] <= thr) { start_index = i; }
                    else break;
                }

                // 终点
                int fwd_limit = std::min(n - 1, trough_index + 10000);
                int end_index = trough_index;
                for (int i = trough_index; i <= fwd_limit; ++i) {
                    double thr = baseline_data[i] - (baseline_data[i] - trough_value) * 0.1;
                    if (data_y[i] <= thr) { end_index = i; }
                    else break;
                }

                double rise = data_x[trough_index] - data_x[start_index];
                double decay = data_x[end_index] - data_x[trough_index];
                double duration = rise + decay;
                double amplitude = baseline_data[start_index] - trough_value;

                if (amplitude >= p.min_amplitude &&
                    duration * 1000.0 >= p.min_duration_ms &&
                    duration * 1000.0 <= p.max_duration_ms &&
                    start_index != last_start_index) {

                    std::vector<double> yseg(data_y.begin() + start_index, data_y.begin() + end_index + 1);
                    double integral_area = Integral::area(yseg, baseline_data[start_index], p.sampling, 2);

                    double thr70 = baseline_data[trough_index] - (baseline_data[trough_index] - trough_value) * 0.7;
                    int left = start_index, right = end_index;
                    for (int i = trough_index; i >= start_index; --i) {
                        if (data_y[i] <= thr70) left = i; else break;
                    }
                    for (int i = trough_index; i <= end_index; ++i) {
                        if (data_y[i] <= thr70) right = i; else break;
                    }
                    double theta_duration = (data_x[right] - data_x[left]) * 1000.0;

                    std::vector<double> yrise(data_y.begin() + start_index, data_y.begin() + trough_index + 1);
                    std::vector<double> ydecay(data_y.begin() + trough_index, data_y.begin() + end_index + 1);
                    double rise_int = Integral::area(yrise, baseline_data[start_index], p.sampling, 2);
                    double decay_int = Integral::area(ydecay, baseline_data[trough_index], p.sampling, 2);
                    double ratio = (rise_int > 0.0) ? (decay_int / rise_int) : 0.0;

                    Event ev;
                    ev.start_index = start_index;
                    ev.end_index = end_index;
                    ev.peak_index = trough_index;
                    ev.amplitude = amplitude;
                    ev.duration_ms = duration * 1000.0;
                    ev.integral_area = integral_area;
                    ev.theta_duration_ms = theta_duration;
                    ev.integral_ratio = ratio;
                    res.events.push_back(ev);

                    last_start_index = start_index;
                    current_pos = end_index + 1;
                    if ((int)res.events.size() >= p.event_number) break;
                    continue;
                }
            }
        }

        // 未触发事件，滑动窗口推进
        current_pos = search_end + 1;
    }

    return res;
}

}