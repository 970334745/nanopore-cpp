#include "DataProcessor.h"
#include <deque>
#include <chrono>
#include <thread>
#include <mutex>
#include <numeric>

namespace DataProcessor {

// PreprocessingPipeline::Impl
struct PreprocessingPipeline::Impl {
    struct ProcessingStep {
        enum Type { FILTER, BASELINE, NOISE_REDUCTION, OUTLIER_REMOVAL };
        Type type;
        FilterParams filter_params;
        BaselineParams baseline_params;
        double threshold;
        std::string description;
    };
    
    std::vector<ProcessingStep> steps;
    std::vector<std::string> history;
    
    void addStep(const ProcessingStep& step) {
        steps.push_back(step);
        history.push_back(step.description);
    }
};

PreprocessingPipeline::PreprocessingPipeline() : pImpl_(std::make_unique<Impl>()) {}

PreprocessingPipeline::~PreprocessingPipeline() = default;

void PreprocessingPipeline::addFilter(const FilterParams& params) {
    Impl::ProcessingStep step;
    step.type = Impl::ProcessingStep::FILTER;
    step.filter_params = params;
    
    std::string filter_name;
    switch (params.type) {
        case FilterType::LowPass: filter_name = "低通滤波"; break;
        case FilterType::HighPass: filter_name = "高通滤波"; break;
        case FilterType::BandPass: filter_name = "带通滤波"; break;
        case FilterType::BandStop: filter_name = "带阻滤波"; break;
        case FilterType::MovingAverage: filter_name = "移动平均滤波"; break;
        case FilterType::Median: filter_name = "中值滤波"; break;
        case FilterType::Gaussian: filter_name = "高斯滤波"; break;
        default: filter_name = "未知滤波"; break;
    }
    
    step.description = filter_name + " (截止频率: " + std::to_string(params.cutoff_frequency) + " Hz)";
    pImpl_->addStep(step);
}

void PreprocessingPipeline::addBaselineCorrection(const BaselineParams& params) {
    Impl::ProcessingStep step;
    step.type = Impl::ProcessingStep::BASELINE;
    step.baseline_params = params;
    
    std::string method_name;
    switch (params.method) {
        case BaselineMethod::Linear: method_name = "线性基线校正"; break;
        case BaselineMethod::Polynomial: method_name = "多项式基线校正"; break;
        case BaselineMethod::MovingAverage: method_name = "移动平均基线校正"; break;
        case BaselineMethod::AsymmetricLeastSquares: method_name = "ALS基线校正"; break;
        default: method_name = "未知基线校正"; break;
    }
    
    step.description = method_name;
    pImpl_->addStep(step);
}

void PreprocessingPipeline::addNoiseReduction(double threshold) {
    Impl::ProcessingStep step;
    step.type = Impl::ProcessingStep::NOISE_REDUCTION;
    step.threshold = threshold;
    step.description = "噪声抑制 (阈值: " + std::to_string(threshold) + ")";
    pImpl_->addStep(step);
}

void PreprocessingPipeline::addOutlierRemoval(double threshold) {
    Impl::ProcessingStep step;
    step.type = Impl::ProcessingStep::OUTLIER_REMOVAL;
    step.threshold = threshold;
    step.description = "异常值移除 (阈值: " + std::to_string(threshold) + "σ)";
    pImpl_->addStep(step);
}

std::vector<double> PreprocessingPipeline::process(const std::vector<double>& data) {
    std::vector<double> result = data;
    
    for (const auto& step : pImpl_->steps) {
        switch (step.type) {
            case Impl::ProcessingStep::FILTER:
                result = Filter::apply(result, step.filter_params);
                break;
                
            case Impl::ProcessingStep::BASELINE:
                result = BaselineCorrection::correct(result, step.baseline_params);
                break;
                
            case Impl::ProcessingStep::NOISE_REDUCTION: {
                // 简单的噪声抑制：小于阈值的值设为0
                double rms = NoiseAnalyzer::calculateRMS(result);
                double noise_threshold = step.threshold * rms;
                for (double& value : result) {
                    if (std::abs(value) < noise_threshold) {
                        value = 0.0;
                    }
                }
                break;
            }
                
            case Impl::ProcessingStep::OUTLIER_REMOVAL: {
                // 基于标准差的异常值移除
                double mean = std::accumulate(result.begin(), result.end(), 0.0) / result.size();
                double variance = 0.0;
                for (double value : result) {
                    variance += (value - mean) * (value - mean);
                }
                double std_dev = std::sqrt(variance / result.size());
                double outlier_threshold = step.threshold * std_dev;
                
                for (double& value : result) {
                    if (std::abs(value - mean) > outlier_threshold) {
                        value = mean;  // 用均值替换异常值
                    }
                }
                break;
            }
        }
    }
    
    return result;
}

std::vector<std::string> PreprocessingPipeline::getProcessingHistory() const {
    return pImpl_->history;
}

void PreprocessingPipeline::clear() {
    pImpl_->steps.clear();
    pImpl_->history.clear();
}

// RealTimeProcessor::Impl
struct RealTimeProcessor::Impl {
    std::deque<double> buffer;
    int max_buffer_size;
    FilterParams filter_params;
    BaselineParams baseline_params;
    bool real_time_analysis_enabled;
    
    mutable std::mutex buffer_mutex;
    
    // 缓存的处理结果
    std::vector<double> filtered_data;
    std::vector<double> baseline_corrected_data;
    NoiseAnalysis current_noise_analysis;
    DataQuality current_quality;
    
    Impl(int buffer_size) 
        : max_buffer_size(buffer_size)
        , real_time_analysis_enabled(false) {}
    
    void updateProcessedData() {
        std::vector<double> data(buffer.begin(), buffer.end());
        
        if (!data.empty()) {
            filtered_data = Filter::apply(data, filter_params);
            baseline_corrected_data = BaselineCorrection::correct(filtered_data, baseline_params);
            
            if (real_time_analysis_enabled) {
                current_noise_analysis = NoiseAnalyzer::analyze(baseline_corrected_data);
                current_quality = QualityAssessment::assess(baseline_corrected_data);
            }
        }
    }
};

RealTimeProcessor::RealTimeProcessor(int buffer_size) 
    : pImpl_(std::make_unique<Impl>(buffer_size)) {}

RealTimeProcessor::~RealTimeProcessor() = default;

void RealTimeProcessor::addDataPoint(double value) {
    std::lock_guard<std::mutex> lock(pImpl_->buffer_mutex);
    
    pImpl_->buffer.push_back(value);
    
    if (static_cast<int>(pImpl_->buffer.size()) > pImpl_->max_buffer_size) {
        pImpl_->buffer.pop_front();
    }
    
    pImpl_->updateProcessedData();
}

void RealTimeProcessor::addDataPoints(const std::vector<double>& values) {
    std::lock_guard<std::mutex> lock(pImpl_->buffer_mutex);
    
    for (double value : values) {
        pImpl_->buffer.push_back(value);
        
        if (static_cast<int>(pImpl_->buffer.size()) > pImpl_->max_buffer_size) {
            pImpl_->buffer.pop_front();
        }
    }
    
    pImpl_->updateProcessedData();
}

std::vector<double> RealTimeProcessor::getFilteredData() const {
    std::lock_guard<std::mutex> lock(pImpl_->buffer_mutex);
    return pImpl_->filtered_data;
}

std::vector<double> RealTimeProcessor::getBaselineCorrectedData() const {
    std::lock_guard<std::mutex> lock(pImpl_->buffer_mutex);
    return pImpl_->baseline_corrected_data;
}

void RealTimeProcessor::setFilterParams(const FilterParams& params) {
    std::lock_guard<std::mutex> lock(pImpl_->buffer_mutex);
    pImpl_->filter_params = params;
    pImpl_->updateProcessedData();
}

void RealTimeProcessor::setBaselineParams(const BaselineParams& params) {
    std::lock_guard<std::mutex> lock(pImpl_->buffer_mutex);
    pImpl_->baseline_params = params;
    pImpl_->updateProcessedData();
}

void RealTimeProcessor::enableRealTimeAnalysis(bool enable) {
    std::lock_guard<std::mutex> lock(pImpl_->buffer_mutex);
    pImpl_->real_time_analysis_enabled = enable;
    if (enable) {
        pImpl_->updateProcessedData();
    }
}

NoiseAnalysis RealTimeProcessor::getCurrentNoiseAnalysis() const {
    std::lock_guard<std::mutex> lock(pImpl_->buffer_mutex);
    return pImpl_->current_noise_analysis;
}

DataQuality RealTimeProcessor::getCurrentQuality() const {
    std::lock_guard<std::mutex> lock(pImpl_->buffer_mutex);
    return pImpl_->current_quality;
}

void RealTimeProcessor::reset() {
    std::lock_guard<std::mutex> lock(pImpl_->buffer_mutex);
    pImpl_->buffer.clear();
    pImpl_->filtered_data.clear();
    pImpl_->baseline_corrected_data.clear();
}

// QualityAssessment类实现
DataQuality QualityAssessment::assess(const std::vector<double>& data, double sampling_rate) {
    DataQuality quality;
    
    if (data.empty()) {
        quality.completeness = 0.0;
        quality.consistency = 0.0;
        quality.stability = 0.0;
        quality.outlier_count = 0;
        quality.drift_rate = 0.0;
        quality.quality_grade = "F";
        return quality;
    }
    
    // 数据完整性：检查NaN和无穷大值
    int valid_count = 0;
    for (double value : data) {
        if (std::isfinite(value)) {
            valid_count++;
        }
    }
    quality.completeness = static_cast<double>(valid_count) / data.size();
    
    // 检测异常值
    quality.outlier_indices = detectOutliers(data);
    quality.outlier_count = quality.outlier_indices.size();
    
    // 数据一致性：基于异常值比例
    quality.consistency = 1.0 - static_cast<double>(quality.outlier_count) / data.size();
    
    // 数据稳定性
    quality.stability = calculateStability(data);
    
    // 漂移率
    quality.drift_rate = calculateDriftRate(data, sampling_rate);
    
    // 质量等级
    quality.quality_grade = gradeQuality(quality);
    
    return quality;
}

std::vector<int> QualityAssessment::detectOutliers(const std::vector<double>& data, double threshold) {
    std::vector<int> outliers;
    
    if (data.size() < 3) return outliers;
    
    // 计算均值和标准差
    double mean = std::accumulate(data.begin(), data.end(), 0.0) / data.size();
    double variance = 0.0;
    for (double value : data) {
        variance += (value - mean) * (value - mean);
    }
    double std_dev = std::sqrt(variance / data.size());
    
    // 检测超出阈值的点
    for (size_t i = 0; i < data.size(); ++i) {
        if (std::abs(data[i] - mean) > threshold * std_dev) {
            outliers.push_back(static_cast<int>(i));
        }
    }
    
    return outliers;
}

double QualityAssessment::calculateDriftRate(const std::vector<double>& data, double sampling_rate) {
    if (data.size() < 2) return 0.0;
    
    // 使用线性回归计算趋势
    size_t n = data.size();
    double sum_x = 0.0, sum_y = 0.0, sum_xy = 0.0, sum_x2 = 0.0;
    
    for (size_t i = 0; i < n; ++i) {
        double x = static_cast<double>(i) / sampling_rate;  // 时间（秒）
        double y = data[i];
        
        sum_x += x;
        sum_y += y;
        sum_xy += x * y;
        sum_x2 += x * x;
    }
    
    double slope = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x * sum_x);
    return slope;  // 单位/秒
}

double QualityAssessment::calculateStability(const std::vector<double>& data) {
    if (data.size() < 2) return 1.0;
    
    // 计算相对标准差（变异系数）
    double mean = std::accumulate(data.begin(), data.end(), 0.0) / data.size();
    if (std::abs(mean) < 1e-10) return 1.0;  // 避免除零
    
    double variance = 0.0;
    for (double value : data) {
        variance += (value - mean) * (value - mean);
    }
    double std_dev = std::sqrt(variance / data.size());
    double cv = std_dev / std::abs(mean);  // 变异系数
    
    // 将变异系数转换为稳定性分数（0-1）
    return std::exp(-cv);
}

std::string QualityAssessment::gradeQuality(const DataQuality& quality) {
    double overall_score = (quality.completeness + quality.consistency + quality.stability) / 3.0;
    
    if (overall_score >= 0.9) return "A";
    else if (overall_score >= 0.8) return "B";
    else if (overall_score >= 0.7) return "C";
    else if (overall_score >= 0.6) return "D";
    else return "F";
}

// BatchProcessor类实现
std::vector<std::vector<double>> BatchProcessor::processMultiple(
    const std::vector<std::vector<double>>& datasets,
    const FilterParams& filter_params,
    const BaselineParams& baseline_params) {
    
    std::vector<std::vector<double>> results;
    results.reserve(datasets.size());
    
    for (const auto& dataset : datasets) {
        auto filtered = Filter::apply(dataset, filter_params);
        auto corrected = BaselineCorrection::correct(filtered, baseline_params);
        results.push_back(corrected);
    }
    
    return results;
}

std::vector<NoiseAnalysis> BatchProcessor::analyzeMultiple(
    const std::vector<std::vector<double>>& datasets,
    double sampling_rate) {
    
    std::vector<NoiseAnalysis> results;
    results.reserve(datasets.size());
    
    for (const auto& dataset : datasets) {
        results.push_back(NoiseAnalyzer::analyze(dataset, sampling_rate));
    }
    
    return results;
}

std::vector<DataQuality> BatchProcessor::assessMultiple(
    const std::vector<std::vector<double>>& datasets,
    double sampling_rate) {
    
    std::vector<DataQuality> results;
    results.reserve(datasets.size());
    
    for (const auto& dataset : datasets) {
        results.push_back(QualityAssessment::assess(dataset, sampling_rate));
    }
    
    return results;
}

} // namespace DataProcessor