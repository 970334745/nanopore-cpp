#pragma once
#include <vector>
#include <utility>
#include <string>

struct Event {
    int start_index;
    int end_index;
    int peak_index;
    double amplitude;
    double duration_ms;
    double integral_area;
    double theta_duration_ms;
    double integral_ratio;
};

struct EventDetectionResult {
    std::vector<Event> events;
};

namespace EventDetection {

struct Params {
    int signal_direction; // 1:正峰, 2:负峰, 3:双向
    int sampling;
    int window_size_ms;
    double min_amplitude;
    double min_duration_ms;
    double max_duration_ms;
    int event_number;
};

EventDetectionResult detect(
    const std::vector<double>& data_x,
    const std::vector<double>& data_y,
    const std::vector<double>& baseline_data,
    const std::vector<double>& std_up_data,
    const std::vector<double>& std_down_data,
    const Params& p
);

}