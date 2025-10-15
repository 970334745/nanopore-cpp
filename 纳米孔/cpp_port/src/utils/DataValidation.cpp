#include "FileManager.h"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <chrono>

namespace FileManager {
namespace DataValidation {

ValidationResult validateDataset(const Dataset& dataset) {
    ValidationResult result;
    result.is_valid = true;
    
    // 检查基本结构
    if (dataset.data.empty()) {
        result.errors.push_back("数据集为空");
        result.is_valid = false;
        return result;
    }
    
    // 检查列名数量是否匹配
    if (!dataset.column_names.empty() && 
        dataset.column_names.size() != dataset.data.size()) {
        result.warnings.push_back("列名数量与数据列数量不匹配");
    }
    
    // 检查每列数据
    size_t max_rows = 0;
    size_t min_rows = SIZE_MAX;
    
    for (size_t i = 0; i < dataset.data.size(); ++i) {
        const auto& column = dataset.data[i];
        max_rows = std::max(max_rows, column.size());
        min_rows = std::min(min_rows, column.size());
        
        // 验证数值数据
        auto validation = validateNumericData(column);
        if (!validation.is_valid) {
            result.is_valid = false;
            for (const auto& error : validation.errors) {
                result.errors.push_back("列 " + std::to_string(i) + ": " + error);
            }
        }
        
        for (const auto& warning : validation.warnings) {
            result.warnings.push_back("列 " + std::to_string(i) + ": " + warning);
        }
        
        // 合并统计信息
        for (const auto& [key, value] : validation.statistics) {
            result.statistics["column_" + std::to_string(i) + "_" + key] = value;
        }
    }
    
    // 检查行数一致性
    if (max_rows != min_rows) {
        result.warnings.push_back("各列数据行数不一致");
    }
    
    // 添加整体统计信息
    result.statistics["total_columns"] = dataset.data.size();
    result.statistics["max_rows"] = max_rows;
    result.statistics["min_rows"] = min_rows;
    
    return result;
}

ValidationResult validateNumericData(const std::vector<double>& data) {
    ValidationResult result;
    result.is_valid = true;
    
    if (data.empty()) {
        result.errors.push_back("数据为空");
        result.is_valid = false;
        return result;
    }
    
    // 检查NaN和无穷值
    size_t nan_count = 0;
    size_t inf_count = 0;
    std::vector<double> valid_data;
    
    for (double value : data) {
        if (std::isnan(value)) {
            nan_count++;
        } else if (std::isinf(value)) {
            inf_count++;
        } else {
            valid_data.push_back(value);
        }
    }
    
    if (nan_count > 0) {
        result.warnings.push_back("包含 " + std::to_string(nan_count) + " 个NaN值");
    }
    
    if (inf_count > 0) {
        result.warnings.push_back("包含 " + std::to_string(inf_count) + " 个无穷值");
    }
    
    if (valid_data.empty()) {
        result.errors.push_back("没有有效的数值数据");
        result.is_valid = false;
        return result;
    }
    
    // 计算统计信息
    double sum = std::accumulate(valid_data.begin(), valid_data.end(), 0.0);
    double mean = sum / valid_data.size();
    
    double variance = 0.0;
    for (double value : valid_data) {
        variance += (value - mean) * (value - mean);
    }
    variance /= valid_data.size();
    double std_dev = std::sqrt(variance);
    
    auto minmax = std::minmax_element(valid_data.begin(), valid_data.end());
    
    result.statistics["count"] = data.size();
    result.statistics["valid_count"] = valid_data.size();
    result.statistics["nan_count"] = nan_count;
    result.statistics["inf_count"] = inf_count;
    result.statistics["mean"] = mean;
    result.statistics["std_dev"] = std_dev;
    result.statistics["min"] = *minmax.first;
    result.statistics["max"] = *minmax.second;
    result.statistics["range"] = *minmax.second - *minmax.first;
    
    // 检查异常值
    if (hasOutliers(valid_data)) {
        result.warnings.push_back("检测到异常值");
        auto outliers = findOutliers(valid_data);
        result.statistics["outlier_count"] = outliers.size();
    }
    
    return result;
}

ValidationResult validateTimeSeriesData(const std::vector<double>& time, 
                                       const std::vector<double>& values) {
    ValidationResult result;
    result.is_valid = true;
    
    if (time.size() != values.size()) {
        result.errors.push_back("时间序列和数值序列长度不匹配");
        result.is_valid = false;
        return result;
    }
    
    // 验证时间序列
    auto time_validation = validateNumericData(time);
    auto values_validation = validateNumericData(values);
    
    // 合并验证结果
    if (!time_validation.is_valid || !values_validation.is_valid) {
        result.is_valid = false;
    }
    
    result.errors.insert(result.errors.end(), 
                        time_validation.errors.begin(), time_validation.errors.end());
    result.errors.insert(result.errors.end(), 
                        values_validation.errors.begin(), values_validation.errors.end());
    
    result.warnings.insert(result.warnings.end(), 
                          time_validation.warnings.begin(), time_validation.warnings.end());
    result.warnings.insert(result.warnings.end(), 
                          values_validation.warnings.begin(), values_validation.warnings.end());
    
    // 检查时间序列单调性
    if (!isMonotonic(time)) {
        result.warnings.push_back("时间序列不是单调递增的");
    }
    
    // 计算采样率
    if (time.size() > 1) {
        std::vector<double> intervals;
        for (size_t i = 1; i < time.size(); ++i) {
            intervals.push_back(time[i] - time[i-1]);
        }
        
        double mean_interval = std::accumulate(intervals.begin(), intervals.end(), 0.0) / intervals.size();
        result.statistics["mean_sampling_interval"] = mean_interval;
        result.statistics["sampling_rate"] = 1.0 / mean_interval;
        
        // 检查采样间隔一致性
        double interval_std = 0.0;
        for (double interval : intervals) {
            interval_std += (interval - mean_interval) * (interval - mean_interval);
        }
        interval_std = std::sqrt(interval_std / intervals.size());
        result.statistics["sampling_interval_std"] = interval_std;
        
        if (interval_std / mean_interval > 0.1) {  // 10%的变异系数
            result.warnings.push_back("采样间隔不均匀");
        }
    }
    
    return result;
}

bool hasNaN(const std::vector<double>& data) {
    return std::any_of(data.begin(), data.end(), [](double x) { return std::isnan(x); });
}

bool hasInfinite(const std::vector<double>& data) {
    return std::any_of(data.begin(), data.end(), [](double x) { return std::isinf(x); });
}

bool isMonotonic(const std::vector<double>& data) {
    if (data.size() <= 1) return true;
    
    bool increasing = true;
    bool decreasing = true;
    
    for (size_t i = 1; i < data.size(); ++i) {
        if (data[i] < data[i-1]) {
            increasing = false;
        }
        if (data[i] > data[i-1]) {
            decreasing = false;
        }
    }
    
    return increasing || decreasing;
}

bool hasOutliers(const std::vector<double>& data, double threshold) {
    if (data.size() < 3) return false;
    
    double sum = std::accumulate(data.begin(), data.end(), 0.0);
    double mean = sum / data.size();
    
    double variance = 0.0;
    for (double value : data) {
        variance += (value - mean) * (value - mean);
    }
    variance /= data.size();
    double std_dev = std::sqrt(variance);
    
    for (double value : data) {
        if (std::abs(value - mean) > threshold * std_dev) {
            return true;
        }
    }
    
    return false;
}

std::vector<int> findNaN(const std::vector<double>& data) {
    std::vector<int> indices;
    for (size_t i = 0; i < data.size(); ++i) {
        if (std::isnan(data[i])) {
            indices.push_back(static_cast<int>(i));
        }
    }
    return indices;
}

std::vector<int> findInfinite(const std::vector<double>& data) {
    std::vector<int> indices;
    for (size_t i = 0; i < data.size(); ++i) {
        if (std::isinf(data[i])) {
            indices.push_back(static_cast<int>(i));
        }
    }
    return indices;
}

std::vector<int> findOutliers(const std::vector<double>& data, double threshold) {
    std::vector<int> indices;
    
    if (data.size() < 3) return indices;
    
    double sum = std::accumulate(data.begin(), data.end(), 0.0);
    double mean = sum / data.size();
    
    double variance = 0.0;
    for (double value : data) {
        variance += (value - mean) * (value - mean);
    }
    variance /= data.size();
    double std_dev = std::sqrt(variance);
    
    for (size_t i = 0; i < data.size(); ++i) {
        if (std::abs(data[i] - mean) > threshold * std_dev) {
            indices.push_back(static_cast<int>(i));
        }
    }
    
    return indices;
}

} // namespace DataValidation

namespace DataConversion {

std::vector<double> stringToDouble(const std::vector<std::string>& strings) {
    std::vector<double> doubles;
    doubles.reserve(strings.size());
    
    for (const auto& str : strings) {
        try {
            double value = std::stod(str);
            doubles.push_back(value);
        } catch (const std::exception&) {
            doubles.push_back(std::numeric_limits<double>::quiet_NaN());
        }
    }
    
    return doubles;
}

std::vector<std::string> doubleToString(const std::vector<double>& doubles, 
                                      int precision) {
    std::vector<std::string> strings;
    strings.reserve(doubles.size());
    
    for (double value : doubles) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(precision) << value;
        strings.push_back(oss.str());
    }
    
    return strings;
}

std::vector<double> convertUnits(const std::vector<double>& data, 
                               const std::string& from_unit, 
                               const std::string& to_unit) {
    // 简化的单位转换实现
    // 实际项目中应该有完整的单位转换表
    
    std::vector<double> converted = data;
    double conversion_factor = 1.0;
    
    // 长度单位转换示例
    if (from_unit == "mm" && to_unit == "m") {
        conversion_factor = 0.001;
    } else if (from_unit == "m" && to_unit == "mm") {
        conversion_factor = 1000.0;
    } else if (from_unit == "cm" && to_unit == "m") {
        conversion_factor = 0.01;
    } else if (from_unit == "m" && to_unit == "cm") {
        conversion_factor = 100.0;
    }
    // 电压单位转换
    else if (from_unit == "mV" && to_unit == "V") {
        conversion_factor = 0.001;
    } else if (from_unit == "V" && to_unit == "mV") {
        conversion_factor = 1000.0;
    }
    // 电流单位转换
    else if (from_unit == "mA" && to_unit == "A") {
        conversion_factor = 0.001;
    } else if (from_unit == "A" && to_unit == "mA") {
        conversion_factor = 1000.0;
    } else if (from_unit == "uA" && to_unit == "A") {
        conversion_factor = 0.000001;
    } else if (from_unit == "A" && to_unit == "uA") {
        conversion_factor = 1000000.0;
    }
    
    for (double& value : converted) {
        value *= conversion_factor;
    }
    
    return converted;
}

std::vector<double> parseTimeStrings(const std::vector<std::string>& time_strings, 
                                   const std::string& format) {
    std::vector<double> time_values;
    time_values.reserve(time_strings.size());
    
    for (const auto& time_str : time_strings) {
        try {
            if (format == "auto" || format == "seconds") {
                // 尝试解析为秒数
                double seconds = std::stod(time_str);
                time_values.push_back(seconds);
            } else {
                // 其他格式的时间解析
                // 这里应该实现更复杂的时间解析逻辑
                double seconds = std::stod(time_str);
                time_values.push_back(seconds);
            }
        } catch (const std::exception&) {
            time_values.push_back(std::numeric_limits<double>::quiet_NaN());
        }
    }
    
    return time_values;
}

std::vector<std::string> formatTimeValues(const std::vector<double>& time_values, 
                                        const std::string& format) {
    std::vector<std::string> time_strings;
    time_strings.reserve(time_values.size());
    
    for (double time_value : time_values) {
        std::ostringstream oss;
        
        if (format == "ISO8601") {
            // 简化的ISO8601格式
            auto time_point = std::chrono::system_clock::from_time_t(static_cast<time_t>(time_value));
            auto time_t = std::chrono::system_clock::to_time_t(time_point);
            oss << std::put_time(std::gmtime(&time_t), "%Y-%m-%dT%H:%M:%SZ");
        } else {
            // 默认格式：秒数
            oss << std::fixed << std::setprecision(6) << time_value;
        }
        
        time_strings.push_back(oss.str());
    }
    
    return time_strings;
}

Dataset resampleDataset(const Dataset& dataset, double new_sampling_rate) {
    Dataset resampled = dataset;
    
    if (dataset.data.empty() || dataset.data[0].empty()) {
        return resampled;
    }
    
    // 假设第一列是时间数据
    const auto& time_data = dataset.data[0];
    double original_duration = time_data.back() - time_data.front();
    size_t new_size = static_cast<size_t>(original_duration * new_sampling_rate);
    
    if (new_size == 0) {
        return resampled;
    }
    
    // 生成新的时间序列
    std::vector<double> new_time(new_size);
    for (size_t i = 0; i < new_size; ++i) {
        new_time[i] = time_data.front() + (i * original_duration) / (new_size - 1);
    }
    
    // 重采样所有列
    resampled.data.clear();
    resampled.data.push_back(new_time);
    
    for (size_t col = 1; col < dataset.data.size(); ++col) {
        auto interpolated = interpolate(time_data, dataset.data[col], new_time);
        resampled.data.push_back(interpolated);
    }
    
    resampled.metadata.data_points = new_size;
    resampled.metadata.sampling_rate = new_sampling_rate;
    
    return resampled;
}

std::vector<double> interpolate(const std::vector<double>& x_old, 
                              const std::vector<double>& y_old, 
                              const std::vector<double>& x_new) {
    std::vector<double> y_new;
    y_new.reserve(x_new.size());
    
    if (x_old.size() != y_old.size() || x_old.empty()) {
        return y_new;
    }
    
    for (double x : x_new) {
        // 线性插值
        if (x <= x_old.front()) {
            y_new.push_back(y_old.front());
        } else if (x >= x_old.back()) {
            y_new.push_back(y_old.back());
        } else {
            // 找到插值区间
            auto it = std::lower_bound(x_old.begin(), x_old.end(), x);
            size_t i = std::distance(x_old.begin(), it);
            
            if (i > 0) {
                double x0 = x_old[i-1];
                double x1 = x_old[i];
                double y0 = y_old[i-1];
                double y1 = y_old[i];
                
                double y = y0 + (y1 - y0) * (x - x0) / (x1 - x0);
                y_new.push_back(y);
            } else {
                y_new.push_back(y_old[0]);
            }
        }
    }
    
    return y_new;
}

} // namespace DataConversion

} // namespace FileManager