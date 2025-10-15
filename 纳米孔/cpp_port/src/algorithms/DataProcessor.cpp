#include "DataProcessor.h"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <complex>
#include <iostream>
#include <sstream>

namespace DataProcessor {

// Filter类实现
std::vector<double> Filter::lowPass(const std::vector<double>& data, const FilterParams& params) {
    // 简单的低通滤波器实现 (移动平均)
    int window = static_cast<int>(params.sampling_rate / params.cutoff_frequency);
    window = std::max(1, std::min(window, static_cast<int>(data.size() / 4)));
    return movingAverage(data, window);
}

std::vector<double> Filter::highPass(const std::vector<double>& data, const FilterParams& params) {
    // 高通滤波 = 原信号 - 低通滤波
    auto lowpass = lowPass(data, params);
    std::vector<double> result(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = data[i] - lowpass[i];
    }
    return result;
}

std::vector<double> Filter::bandPass(const std::vector<double>& data, const FilterParams& params) {
    // 带通滤波 = 低通(高频) - 低通(低频)
    FilterParams high_params = params;
    high_params.cutoff_frequency = params.high_frequency;
    FilterParams low_params = params;
    low_params.cutoff_frequency = params.low_frequency;
    
    auto high_filtered = lowPass(data, high_params);
    auto low_filtered = lowPass(data, low_params);
    
    std::vector<double> result(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = high_filtered[i] - low_filtered[i];
    }
    return result;
}

std::vector<double> Filter::bandStop(const std::vector<double>& data, const FilterParams& params) {
    // 带阻滤波 = 原信号 - 带通滤波
    auto bandpass = bandPass(data, params);
    std::vector<double> result(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = data[i] - bandpass[i];
    }
    return result;
}

std::vector<double> Filter::butterworth(const std::vector<double>& data, const FilterParams& params) {
    // 简化的Butterworth滤波器实现
    return lowPass(data, params);
}

std::vector<double> Filter::movingAverage(const std::vector<double>& data, int window_size) {
    if (data.empty() || window_size <= 0) return data;
    
    window_size = std::min(window_size, static_cast<int>(data.size()));
    std::vector<double> result(data.size());
    
    // 计算初始窗口的和
    double sum = 0.0;
    for (int i = 0; i < window_size; ++i) {
        sum += data[i];
    }
    
    // 滑动窗口
    for (size_t i = 0; i < data.size(); ++i) {
        if (i < window_size / 2) {
            // 前半部分：使用可用的数据点
            double local_sum = 0.0;
            int count = 0;
            for (int j = 0; j <= static_cast<int>(i) + window_size / 2 && j < static_cast<int>(data.size()); ++j) {
                local_sum += data[j];
                count++;
            }
            result[i] = local_sum / count;
        } else if (i >= data.size() - window_size / 2) {
            // 后半部分：使用可用的数据点
            double local_sum = 0.0;
            int count = 0;
            for (int j = static_cast<int>(i) - window_size / 2; j < static_cast<int>(data.size()); ++j) {
                local_sum += data[j];
                count++;
            }
            result[i] = local_sum / count;
        } else {
            // 中间部分：使用完整窗口
            if (i == window_size / 2) {
                result[i] = sum / window_size;
            } else {
                sum = sum - data[i - window_size / 2 - 1] + data[i + window_size / 2];
                result[i] = sum / window_size;
            }
        }
    }
    
    return result;
}

std::vector<double> Filter::medianFilter(const std::vector<double>& data, int window_size) {
    if (data.empty() || window_size <= 0) return data;
    
    window_size = std::min(window_size, static_cast<int>(data.size()));
    std::vector<double> result(data.size());
    
    for (size_t i = 0; i < data.size(); ++i) {
        std::vector<double> window;
        
        int start = std::max(0, static_cast<int>(i) - window_size / 2);
        int end = std::min(static_cast<int>(data.size()), static_cast<int>(i) + window_size / 2 + 1);
        
        for (int j = start; j < end; ++j) {
            window.push_back(data[j]);
        }
        
        std::sort(window.begin(), window.end());
        result[i] = window[window.size() / 2];
    }
    
    return result;
}

std::vector<double> Filter::gaussianFilter(const std::vector<double>& data, double sigma) {
    if (data.empty() || sigma <= 0) return data;
    
    // 生成高斯核
    int kernel_size = static_cast<int>(6 * sigma + 1);
    if (kernel_size % 2 == 0) kernel_size++;
    
    std::vector<double> kernel(kernel_size);
    double sum = 0.0;
    int center = kernel_size / 2;
    
    for (int i = 0; i < kernel_size; ++i) {
        double x = i - center;
        kernel[i] = std::exp(-(x * x) / (2 * sigma * sigma));
        sum += kernel[i];
    }
    
    // 归一化核
    for (double& k : kernel) {
        k /= sum;
    }
    
    // 应用卷积
    return MathUtils::convolve(data, kernel);
}

std::vector<double> Filter::apply(const std::vector<double>& data, const FilterParams& params) {
    switch (params.type) {
        case FilterType::LowPass:
            return lowPass(data, params);
        case FilterType::HighPass:
            return highPass(data, params);
        case FilterType::BandPass:
            return bandPass(data, params);
        case FilterType::BandStop:
            return bandStop(data, params);
        case FilterType::Butterworth:
            return butterworth(data, params);
        case FilterType::MovingAverage:
            return movingAverage(data, params.window_size);
        case FilterType::Median:
            return medianFilter(data, params.window_size);
        case FilterType::Gaussian:
            return gaussianFilter(data, params.sigma);
        default:
            return data;
    }
}

// BaselineCorrection类实现
std::vector<double> BaselineCorrection::linear(const std::vector<double>& data) {
    if (data.size() < 2) return std::vector<double>(data.size(), 0.0);
    
    double start = data.front();
    double end = data.back();
    double slope = (end - start) / (data.size() - 1);
    
    std::vector<double> baseline(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        baseline[i] = start + slope * i;
    }
    
    std::vector<double> result(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = data[i] - baseline[i];
    }
    
    return result;
}

std::vector<double> BaselineCorrection::polynomial(const std::vector<double>& data, int order) {
    if (data.size() < order + 1) return linear(data);
    
    // 简化的多项式拟合（使用最小二乘法）
    size_t n = data.size();
    std::vector<std::vector<double>> A(order + 1, std::vector<double>(order + 1, 0.0));
    std::vector<double> b(order + 1, 0.0);
    
    // 构建正规方程
    for (size_t i = 0; i < n; ++i) {
        double x = static_cast<double>(i);
        for (int j = 0; j <= order; ++j) {
            for (int k = 0; k <= order; ++k) {
                A[j][k] += std::pow(x, j + k);
            }
            b[j] += data[i] * std::pow(x, j);
        }
    }
    
    // 简单的高斯消元法求解
    std::vector<double> coeffs(order + 1, 0.0);
    for (int i = 0; i <= order; ++i) {
        // 找到主元
        int max_row = i;
        for (int k = i + 1; k <= order; ++k) {
            if (std::abs(A[k][i]) > std::abs(A[max_row][i])) {
                max_row = k;
            }
        }
        
        // 交换行
        if (max_row != i) {
            std::swap(A[i], A[max_row]);
            std::swap(b[i], b[max_row]);
        }
        
        // 消元
        for (int k = i + 1; k <= order; ++k) {
            if (std::abs(A[i][i]) > 1e-10) {
                double factor = A[k][i] / A[i][i];
                for (int j = i; j <= order; ++j) {
                    A[k][j] -= factor * A[i][j];
                }
                b[k] -= factor * b[i];
            }
        }
    }
    
    // 回代求解
    for (int i = order; i >= 0; --i) {
        coeffs[i] = b[i];
        for (int j = i + 1; j <= order; ++j) {
            coeffs[i] -= A[i][j] * coeffs[j];
        }
        if (std::abs(A[i][i]) > 1e-10) {
            coeffs[i] /= A[i][i];
        }
    }
    
    // 计算基线并减去
    std::vector<double> result(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        double baseline_value = 0.0;
        double x = static_cast<double>(i);
        for (int j = 0; j <= order; ++j) {
            baseline_value += coeffs[j] * std::pow(x, j);
        }
        result[i] = data[i] - baseline_value;
    }
    
    return result;
}

std::vector<double> BaselineCorrection::movingAverage(const std::vector<double>& data, int window_size) {
    auto baseline = Filter::movingAverage(data, window_size);
    std::vector<double> result(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = data[i] - baseline[i];
    }
    return result;
}

std::vector<double> BaselineCorrection::asymmetricLeastSquares(const std::vector<double>& data, const BaselineParams& params) {
    // 简化的ALS实现
    size_t n = data.size();
    std::vector<double> baseline = data;  // 初始基线
    std::vector<double> weights(n, 1.0);
    
    for (int iter = 0; iter < params.max_iterations; ++iter) {
        std::vector<double> old_baseline = baseline;
        
        // 更新权重
        for (size_t i = 0; i < n; ++i) {
            if (data[i] > baseline[i]) {
                weights[i] = params.p;
            } else {
                weights[i] = 1.0 - params.p;
            }
        }
        
        // 简化的平滑更新
        for (size_t i = 1; i < n - 1; ++i) {
            double weighted_sum = weights[i-1] * data[i-1] + weights[i] * data[i] + weights[i+1] * data[i+1];
            double weight_sum = weights[i-1] + weights[i] + weights[i+1];
            baseline[i] = weighted_sum / weight_sum;
        }
        
        // 检查收敛
        double diff = 0.0;
        for (size_t i = 0; i < n; ++i) {
            diff += std::abs(baseline[i] - old_baseline[i]);
        }
        if (diff / n < params.tolerance) break;
    }
    
    std::vector<double> result(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = data[i] - baseline[i];
    }
    
    return result;
}

std::vector<double> BaselineCorrection::correct(const std::vector<double>& data, const BaselineParams& params) {
    switch (params.method) {
        case BaselineMethod::Linear:
            return linear(data);
        case BaselineMethod::Polynomial:
            return polynomial(data, params.polynomial_order);
        case BaselineMethod::MovingAverage:
            return movingAverage(data, params.window_size);
        case BaselineMethod::AsymmetricLeastSquares:
            return asymmetricLeastSquares(data, params);
        default:
            return linear(data);
    }
}

std::vector<double> BaselineCorrection::getBaseline(const std::vector<double>& data, const BaselineParams& params) {
    auto corrected = correct(data, params);
    std::vector<double> baseline(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        baseline[i] = data[i] - corrected[i];
    }
    return baseline;
}

// NoiseAnalyzer类实现
NoiseAnalysis NoiseAnalyzer::analyze(const std::vector<double>& data, double sampling_rate) {
    NoiseAnalysis result;
    
    if (data.empty()) {
        result.rms = 0.0;
        result.snr = 0.0;
        result.peak_to_peak = 0.0;
        result.standard_deviation = 0.0;
        result.mean_absolute_deviation = 0.0;
        return result;
    }
    
    result.rms = calculateRMS(data);
    result.peak_to_peak = calculatePeakToPeak(data);
    
    // 计算标准差
    double mean = std::accumulate(data.begin(), data.end(), 0.0) / data.size();
    double variance = 0.0;
    double mad = 0.0;
    
    for (double value : data) {
        double diff = value - mean;
        variance += diff * diff;
        mad += std::abs(diff);
    }
    
    result.standard_deviation = std::sqrt(variance / data.size());
    result.mean_absolute_deviation = mad / data.size();
    
    // 计算功率谱
    result.power_spectrum = calculatePowerSpectrum(data, sampling_rate);
    result.frequencies = generateFrequencyAxis(data.size(), sampling_rate);
    
    // 估算信噪比（使用信号功率与噪声功率的比值）
    double signal_power = 0.0;
    double noise_power = result.rms * result.rms;
    for (double value : data) {
        signal_power += value * value;
    }
    signal_power /= data.size();
    
    if (noise_power > 0) {
        result.snr = 10.0 * std::log10(signal_power / noise_power);
    } else {
        result.snr = std::numeric_limits<double>::infinity();
    }
    
    return result;
}

double NoiseAnalyzer::calculateRMS(const std::vector<double>& data) {
    if (data.empty()) return 0.0;
    
    double sum_squares = 0.0;
    for (double value : data) {
        sum_squares += value * value;
    }
    
    return std::sqrt(sum_squares / data.size());
}

double NoiseAnalyzer::calculateSNR(const std::vector<double>& signal, const std::vector<double>& noise) {
    double signal_power = calculateRMS(signal);
    double noise_power = calculateRMS(noise);
    
    if (noise_power > 0) {
        return 20.0 * std::log10(signal_power / noise_power);
    }
    
    return std::numeric_limits<double>::infinity();
}

double NoiseAnalyzer::calculatePeakToPeak(const std::vector<double>& data) {
    if (data.empty()) return 0.0;
    
    auto minmax = std::minmax_element(data.begin(), data.end());
    return *minmax.second - *minmax.first;
}

std::vector<double> NoiseAnalyzer::calculatePowerSpectrum(const std::vector<double>& data, double sampling_rate) {
    // 简化的功率谱计算
    std::vector<double> spectrum;
    size_t n = data.size();
    
    if (n < 2) return spectrum;
    
    // 计算每个频率bin的功率
    for (size_t k = 0; k < n / 2; ++k) {
        double real_part = 0.0;
        double imag_part = 0.0;
        
        for (size_t i = 0; i < n; ++i) {
            double angle = -2.0 * M_PI * k * i / n;
            real_part += data[i] * std::cos(angle);
            imag_part += data[i] * std::sin(angle);
        }
        
        double power = (real_part * real_part + imag_part * imag_part) / (n * n);
        spectrum.push_back(power);
    }
    
    return spectrum;
}

std::vector<double> NoiseAnalyzer::generateFrequencyAxis(int data_size, double sampling_rate) {
    std::vector<double> frequencies;
    
    for (int k = 0; k < data_size / 2; ++k) {
        double freq = k * sampling_rate / data_size;
        frequencies.push_back(freq);
    }
    
    return frequencies;
}

// MathUtils命名空间实现
namespace MathUtils {

std::vector<double> convolve(const std::vector<double>& signal, const std::vector<double>& kernel) {
    if (signal.empty() || kernel.empty()) return {};
    
    size_t result_size = signal.size() + kernel.size() - 1;
    std::vector<double> result(result_size, 0.0);
    
    for (size_t i = 0; i < signal.size(); ++i) {
        for (size_t j = 0; j < kernel.size(); ++j) {
            result[i + j] += signal[i] * kernel[j];
        }
    }
    
    // 返回与原信号相同大小的结果
    size_t start = kernel.size() / 2;
    std::vector<double> trimmed_result;
    for (size_t i = start; i < start + signal.size() && i < result.size(); ++i) {
        trimmed_result.push_back(result[i]);
    }
    
    return trimmed_result;
}

std::vector<double> derivative(const std::vector<double>& data, double dx) {
    if (data.size() < 2) return {};
    
    std::vector<double> result(data.size());
    
    // 前向差分（第一个点）
    result[0] = (data[1] - data[0]) / dx;
    
    // 中心差分（中间点）
    for (size_t i = 1; i < data.size() - 1; ++i) {
        result[i] = (data[i + 1] - data[i - 1]) / (2.0 * dx);
    }
    
    // 后向差分（最后一个点）
    result[data.size() - 1] = (data[data.size() - 1] - data[data.size() - 2]) / dx;
    
    return result;
}

std::vector<double> integrate(const std::vector<double>& data, double dx) {
    if (data.empty()) return {};
    
    std::vector<double> result(data.size());
    result[0] = 0.0;
    
    // 梯形积分
    for (size_t i = 1; i < data.size(); ++i) {
        result[i] = result[i - 1] + 0.5 * (data[i] + data[i - 1]) * dx;
    }
    
    return result;
}

double pearsonCorrelation(const std::vector<double>& x, const std::vector<double>& y) {
    if (x.size() != y.size() || x.empty()) return 0.0;
    
    double mean_x = std::accumulate(x.begin(), x.end(), 0.0) / x.size();
    double mean_y = std::accumulate(y.begin(), y.end(), 0.0) / y.size();
    
    double numerator = 0.0;
    double sum_sq_x = 0.0;
    double sum_sq_y = 0.0;
    
    for (size_t i = 0; i < x.size(); ++i) {
        double dx = x[i] - mean_x;
        double dy = y[i] - mean_y;
        numerator += dx * dy;
        sum_sq_x += dx * dx;
        sum_sq_y += dy * dy;
    }
    
    double denominator = std::sqrt(sum_sq_x * sum_sq_y);
    
    return (denominator > 0) ? numerator / denominator : 0.0;
}

std::vector<double> resample(const std::vector<double>& data, int new_size) {
    if (data.empty() || new_size <= 0) return {};
    
    std::vector<double> result(new_size);
    double ratio = static_cast<double>(data.size() - 1) / (new_size - 1);
    
    for (int i = 0; i < new_size; ++i) {
        double index = i * ratio;
        int lower = static_cast<int>(std::floor(index));
        int upper = std::min(lower + 1, static_cast<int>(data.size() - 1));
        
        if (lower == upper) {
            result[i] = data[lower];
        } else {
            double weight = index - lower;
            result[i] = data[lower] * (1.0 - weight) + data[upper] * weight;
        }
    }
    
    return result;
}

} // namespace MathUtils

} // namespace DataProcessor