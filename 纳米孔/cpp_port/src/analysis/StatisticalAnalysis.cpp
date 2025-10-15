#include "StatisticalAnalysis.h"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <random>
#include <map>
#include <set>
#include <functional>
#include <iostream>

namespace Analysis {

// StatisticalAnalyzer的PIMPL实现
class StatisticalAnalyzer::Impl {
public:
    std::mt19937 rng;
    
    Impl(unsigned int seed = 42) : rng(seed) {}
    
    // 计算分位数
    double calculateQuantile(std::vector<double> data, double q) {
        if (data.empty()) return 0.0;
        
        std::sort(data.begin(), data.end());
        double index = q * (data.size() - 1);
        size_t lower = static_cast<size_t>(std::floor(index));
        size_t upper = static_cast<size_t>(std::ceil(index));
        
        if (lower == upper) {
            return data[lower];
        }
        
        double weight = index - lower;
        return data[lower] * (1.0 - weight) + data[upper] * weight;
    }
    
    // 矩阵运算
    std::vector<std::vector<double>> matrixMultiply(
        const std::vector<std::vector<double>>& A,
        const std::vector<std::vector<double>>& B) {
        
        if (A.empty() || B.empty() || A[0].size() != B.size()) {
            return {};
        }
        
        size_t rows = A.size();
        size_t cols = B[0].size();
        size_t inner = A[0].size();
        
        std::vector<std::vector<double>> result(rows, std::vector<double>(cols, 0.0));
        
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                for (size_t k = 0; k < inner; ++k) {
                    result[i][j] += A[i][k] * B[k][j];
                }
            }
        }
        
        return result;
    }
    
    // 矩阵转置
    std::vector<std::vector<double>> matrixTranspose(
        const std::vector<std::vector<double>>& matrix) {
        
        if (matrix.empty()) return {};
        
        size_t rows = matrix.size();
        size_t cols = matrix[0].size();
        
        std::vector<std::vector<double>> result(cols, std::vector<double>(rows));
        
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result[j][i] = matrix[i][j];
            }
        }
        
        return result;
    }
    
    // 高斯-约旦消元法求矩阵逆
    std::vector<std::vector<double>> matrixInverse(
        const std::vector<std::vector<double>>& matrix) {
        
        if (matrix.empty() || matrix.size() != matrix[0].size()) {
            return {};
        }
        
        size_t n = matrix.size();
        std::vector<std::vector<double>> augmented(n, std::vector<double>(2 * n, 0.0));
        
        // 创建增广矩阵 [A|I]
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                augmented[i][j] = matrix[i][j];
            }
            augmented[i][i + n] = 1.0;
        }
        
        // 高斯-约旦消元
        for (size_t i = 0; i < n; ++i) {
            // 寻找主元
            size_t pivot_row = i;
            for (size_t k = i + 1; k < n; ++k) {
                if (std::abs(augmented[k][i]) > std::abs(augmented[pivot_row][i])) {
                    pivot_row = k;
                }
            }
            
            // 交换行
            if (pivot_row != i) {
                std::swap(augmented[i], augmented[pivot_row]);
            }
            
            // 检查奇异性
            if (std::abs(augmented[i][i]) < 1e-12) {
                return {}; // 矩阵奇异
            }
            
            // 归一化主元行
            double pivot = augmented[i][i];
            for (size_t j = 0; j < 2 * n; ++j) {
                augmented[i][j] /= pivot;
            }
            
            // 消元
            for (size_t k = 0; k < n; ++k) {
                if (k != i) {
                    double factor = augmented[k][i];
                    for (size_t j = 0; j < 2 * n; ++j) {
                        augmented[k][j] -= factor * augmented[i][j];
                    }
                }
            }
        }
        
        // 提取逆矩阵
        std::vector<std::vector<double>> inverse(n, std::vector<double>(n));
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                inverse[i][j] = augmented[i][j + n];
            }
        }
        
        return inverse;
    }
};

StatisticalAnalyzer::StatisticalAnalyzer() : pimpl_(std::make_unique<Impl>()) {}

StatisticalAnalyzer::~StatisticalAnalyzer() = default;

BasicStatistics StatisticalAnalyzer::calculateBasicStatistics(const std::vector<double>& data) {
    BasicStatistics stats;
    
    if (data.empty()) {
        return stats;
    }
    
    stats.count = data.size();
    
    // 过滤有效数据
    std::vector<double> valid_data;
    for (double value : data) {
        if (!std::isnan(value) && !std::isinf(value)) {
            valid_data.push_back(value);
        }
    }
    
    stats.valid_count = valid_data.size();
    stats.missing_count = stats.count - stats.valid_count;
    
    if (valid_data.empty()) {
        return stats;
    }
    
    // 基本统计量
    stats.mean = StatUtils::calculateMean(valid_data);
    stats.median = StatUtils::calculateMedian(valid_data);
    stats.mode = StatUtils::calculateMode(valid_data);
    stats.std_dev = StatUtils::calculateStandardDeviation(valid_data);
    stats.variance = StatUtils::calculateVariance(valid_data);
    stats.skewness = StatUtils::calculateSkewness(valid_data);
    stats.kurtosis = StatUtils::calculateKurtosis(valid_data);
    
    auto minmax = std::minmax_element(valid_data.begin(), valid_data.end());
    stats.min = *minmax.first;
    stats.max = *minmax.second;
    stats.range = stats.max - stats.min;
    
    // 四分位数
    stats.q1 = pimpl_->calculateQuantile(valid_data, 0.25);
    stats.q3 = pimpl_->calculateQuantile(valid_data, 0.75);
    stats.iqr = stats.q3 - stats.q1;
    
    return stats;
}

std::vector<double> StatisticalAnalyzer::calculatePercentiles(
    const std::vector<double>& data, const std::vector<double>& percentiles) {
    
    std::vector<double> result;
    result.reserve(percentiles.size());
    
    for (double p : percentiles) {
        result.push_back(pimpl_->calculateQuantile(data, p / 100.0));
    }
    
    return result;
}

std::vector<double> StatisticalAnalyzer::calculateMovingAverage(
    const std::vector<double>& data, int window_size) {
    
    std::vector<double> result;
    if (data.empty() || window_size <= 0) {
        return result;
    }
    
    result.reserve(data.size());
    
    for (size_t i = 0; i < data.size(); ++i) {
        double sum = 0.0;
        int count = 0;
        
        int start = std::max(0, static_cast<int>(i) - window_size / 2);
        int end = std::min(static_cast<int>(data.size()), 
                          static_cast<int>(i) + window_size / 2 + 1);
        
        for (int j = start; j < end; ++j) {
            if (!std::isnan(data[j]) && !std::isinf(data[j])) {
                sum += data[j];
                count++;
            }
        }
        
        result.push_back(count > 0 ? sum / count : std::numeric_limits<double>::quiet_NaN());
    }
    
    return result;
}

std::vector<double> StatisticalAnalyzer::calculateMovingStd(
    const std::vector<double>& data, int window_size) {
    
    std::vector<double> result;
    if (data.empty() || window_size <= 0) {
        return result;
    }
    
    result.reserve(data.size());
    
    for (size_t i = 0; i < data.size(); ++i) {
        std::vector<double> window_data;
        
        int start = std::max(0, static_cast<int>(i) - window_size / 2);
        int end = std::min(static_cast<int>(data.size()), 
                          static_cast<int>(i) + window_size / 2 + 1);
        
        for (int j = start; j < end; ++j) {
            if (!std::isnan(data[j]) && !std::isinf(data[j])) {
                window_data.push_back(data[j]);
            }
        }
        
        if (window_data.size() > 1) {
            result.push_back(StatUtils::calculateStandardDeviation(window_data));
        } else {
            result.push_back(std::numeric_limits<double>::quiet_NaN());
        }
    }
    
    return result;
}

DistributionTest StatisticalAnalyzer::testNormality(const std::vector<double>& data) {
    DistributionTest result;
    result.distribution_name = "Normal";
    result.confidence_level = confidence_level_;
    
    if (data.size() < 3) {
        result.p_value = 0.0;
        result.is_significant = false;
        return result;
    }
    
    // Shapiro-Wilk测试的简化实现
    double skewness = StatUtils::calculateSkewness(data);
    double kurtosis = StatUtils::calculateKurtosis(data);
    
    // 计算测试统计量
    double n = static_cast<double>(data.size());
    double skew_stat = skewness * std::sqrt(n * (n - 1)) / std::sqrt(6.0 * (n - 2));
    double kurt_stat = (kurtosis - 3.0) * std::sqrt(n * (n - 1)) / 
                       std::sqrt(24.0 * (n - 2) * (n - 3));
    
    result.test_statistic = skew_stat * skew_stat + kurt_stat * kurt_stat;
    
    // 简化的p值计算
    result.p_value = std::exp(-result.test_statistic / 2.0);
    result.is_significant = result.p_value < (1.0 - confidence_level_);
    
    result.parameters["skewness"] = skewness;
    result.parameters["kurtosis"] = kurtosis;
    
    return result;
}

// 继续实现其他方法...
DistributionTest StatisticalAnalyzer::testUniformity(const std::vector<double>& data) {
    DistributionTest result;
    result.distribution_name = "Uniform";
    result.confidence_level = confidence_level_;
    
    if (data.size() < 5) {
        result.p_value = 0.0;
        result.is_significant = false;
        return result;
    }
    
    // Kolmogorov-Smirnov测试的简化实现
    std::vector<double> sorted_data = data;
    std::sort(sorted_data.begin(), sorted_data.end());
    
    double min_val = sorted_data.front();
    double max_val = sorted_data.back();
    double range = max_val - min_val;
    
    if (range == 0.0) {
        result.test_statistic = 1.0;
        result.p_value = 0.0;
        result.is_significant = true;
        return result;
    }
    
    double max_diff = 0.0;
    for (size_t i = 0; i < sorted_data.size(); ++i) {
        double empirical_cdf = static_cast<double>(i + 1) / sorted_data.size();
        double theoretical_cdf = (sorted_data[i] - min_val) / range;
        max_diff = std::max(max_diff, std::abs(empirical_cdf - theoretical_cdf));
    }
    
    result.test_statistic = max_diff;
    
    // 简化的p值计算
    double n = static_cast<double>(data.size());
    result.p_value = 2.0 * std::exp(-2.0 * n * max_diff * max_diff);
    result.is_significant = result.p_value < (1.0 - confidence_level_);
    
    return result;
}

DistributionTest StatisticalAnalyzer::testExponentiality(const std::vector<double>& data) {
    DistributionTest result;
    result.distribution_name = "Exponential";
    result.confidence_level = confidence_level_;
    
    if (data.size() < 3) {
        result.p_value = 0.0;
        result.is_significant = false;
        return result;
    }
    
    // 检查数据是否为非负
    bool all_positive = std::all_of(data.begin(), data.end(), 
                                   [](double x) { return x >= 0.0; });
    
    if (!all_positive) {
        result.test_statistic = 1.0;
        result.p_value = 0.0;
        result.is_significant = true;
        return result;
    }
    
    double mean = StatUtils::calculateMean(data);
    double std_dev = StatUtils::calculateStandardDeviation(data);
    
    // 对于指数分布，均值应该等于标准差
    result.test_statistic = std::abs(mean - std_dev) / mean;
    
    // 简化的p值计算
    result.p_value = std::exp(-result.test_statistic * data.size());
    result.is_significant = result.p_value < (1.0 - confidence_level_);
    
    result.parameters["lambda"] = 1.0 / mean;
    
    return result;
}

std::vector<double> StatisticalAnalyzer::generateHistogram(
    const std::vector<double>& data, int bins) {
    
    if (data.empty() || bins <= 0) {
        return {};
    }
    
    auto minmax = std::minmax_element(data.begin(), data.end());
    double min_val = *minmax.first;
    double max_val = *minmax.second;
    
    if (min_val == max_val) {
        return std::vector<double>(bins, 0.0);
    }
    
    double bin_width = (max_val - min_val) / bins;
    std::vector<double> histogram(bins, 0.0);
    
    for (double value : data) {
        if (!std::isnan(value) && !std::isinf(value)) {
            int bin_index = static_cast<int>((value - min_val) / bin_width);
            bin_index = std::max(0, std::min(bins - 1, bin_index));
            histogram[bin_index]++;
        }
    }
    
    return histogram;
}

std::vector<double> StatisticalAnalyzer::estimateKernelDensity(
    const std::vector<double>& data, const std::vector<double>& x_points) {
    
    if (data.empty() || x_points.empty()) {
        return {};
    }
    
    // 使用Silverman's rule计算带宽
    double std_dev = StatUtils::calculateStandardDeviation(data);
    double n = static_cast<double>(data.size());
    double bandwidth = 1.06 * std_dev * std::pow(n, -0.2);
    
    std::vector<double> density(x_points.size(), 0.0);
    
    for (size_t i = 0; i < x_points.size(); ++i) {
        for (double data_point : data) {
            if (!std::isnan(data_point) && !std::isinf(data_point)) {
                double z = (x_points[i] - data_point) / bandwidth;
                density[i] += StatUtils::normalPDF(z) / bandwidth;
            }
        }
        density[i] /= data.size();
    }
    
    return density;
}

// 注意：setConfidenceLevel和setSeed方法需要在头文件中声明后才能实现

// StatUtils命名空间的实现
namespace StatUtils {

double calculateMean(const std::vector<double>& data) {
    if (data.empty()) return 0.0;
    
    double sum = 0.0;
    size_t count = 0;
    
    for (double value : data) {
        if (!std::isnan(value) && !std::isinf(value)) {
            sum += value;
            count++;
        }
    }
    
    return count > 0 ? sum / count : 0.0;
}

double calculateMedian(std::vector<double> data) {
    if (data.empty()) return 0.0;
    
    std::vector<double> valid_data;
    for (double value : data) {
        if (!std::isnan(value) && !std::isinf(value)) {
            valid_data.push_back(value);
        }
    }
    
    if (valid_data.empty()) return 0.0;
    
    std::sort(valid_data.begin(), valid_data.end());
    
    size_t n = valid_data.size();
    if (n % 2 == 0) {
        return (valid_data[n/2 - 1] + valid_data[n/2]) / 2.0;
    } else {
        return valid_data[n/2];
    }
}

double calculateMode(const std::vector<double>& data) {
    if (data.empty()) return 0.0;
    
    std::map<double, int> frequency;
    
    for (double value : data) {
        if (!std::isnan(value) && !std::isinf(value)) {
            frequency[value]++;
        }
    }
    
    if (frequency.empty()) return 0.0;
    
    auto max_freq = std::max_element(frequency.begin(), frequency.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; });
    
    return max_freq->first;
}

double calculateVariance(const std::vector<double>& data) {
    if (data.size() < 2) return 0.0;
    
    double mean = calculateMean(data);
    double sum_sq_diff = 0.0;
    size_t count = 0;
    
    for (double value : data) {
        if (!std::isnan(value) && !std::isinf(value)) {
            double diff = value - mean;
            sum_sq_diff += diff * diff;
            count++;
        }
    }
    
    return count > 1 ? sum_sq_diff / (count - 1) : 0.0;
}

double calculateStandardDeviation(const std::vector<double>& data) {
    return std::sqrt(calculateVariance(data));
}

double calculateSkewness(const std::vector<double>& data) {
    if (data.size() < 3) return 0.0;
    
    double mean = calculateMean(data);
    double std_dev = calculateStandardDeviation(data);
    
    if (std_dev == 0.0) return 0.0;
    
    double sum_cubed = 0.0;
    size_t count = 0;
    
    for (double value : data) {
        if (!std::isnan(value) && !std::isinf(value)) {
            double z = (value - mean) / std_dev;
            sum_cubed += z * z * z;
            count++;
        }
    }
    
    return count > 0 ? sum_cubed / count : 0.0;
}

double calculateKurtosis(const std::vector<double>& data) {
    if (data.size() < 4) return 0.0;
    
    double mean = calculateMean(data);
    double std_dev = calculateStandardDeviation(data);
    
    if (std_dev == 0.0) return 0.0;
    
    double sum_fourth = 0.0;
    size_t count = 0;
    
    for (double value : data) {
        if (!std::isnan(value) && !std::isinf(value)) {
            double z = (value - mean) / std_dev;
            sum_fourth += z * z * z * z;
            count++;
        }
    }
    
    return count > 0 ? sum_fourth / count : 0.0;
}

double normalPDF(double x, double mean, double std_dev) {
    if (std_dev <= 0.0) return 0.0;
    
    double z = (x - mean) / std_dev;
    return std::exp(-0.5 * z * z) / (std_dev * std::sqrt(2.0 * M_PI));
}

double normalCDF(double x, double mean, double std_dev) {
    if (std_dev <= 0.0) return (x >= mean) ? 1.0 : 0.0;
    
    double z = (x - mean) / std_dev;
    return 0.5 * (1.0 + std::erf(z / std::sqrt(2.0)));
}

double tCDF(double t, double df) {
    // 简化的t分布CDF实现
    if (df <= 0.0) return 0.5;
    
    // 对于大自由度，近似为标准正态分布
    if (df > 30.0) {
        return normalCDF(t);
    }
    
    // 简化实现，实际应该使用更精确的算法
    double x = t / std::sqrt(df);
    return 0.5 + 0.5 * std::erf(x / std::sqrt(2.0));
}

double chiSquareCDF(double x, double df) {
    if (x <= 0.0 || df <= 0.0) return 0.0;
    
    // 简化的卡方分布CDF实现
    return std::min(1.0, x / (df + x));
}

double fCDF(double f, double df1, double df2) {
    if (f <= 0.0 || df1 <= 0.0 || df2 <= 0.0) return 0.0;
    
    // 简化的F分布CDF实现
    double x = df1 * f / (df1 * f + df2);
    return std::min(1.0, x);
}

std::mt19937& getRandomGenerator() {
    static std::mt19937 rng(42);
    return rng;
}

} // namespace StatUtils

} // namespace Analysis