#pragma once
#include <vector>
#include <string>
#include <memory>

namespace DataProcessor {

// 滤波器类型
enum class FilterType {
    LowPass,
    HighPass,
    BandPass,
    BandStop,
    Butterworth,
    Chebyshev,
    Bessel,
    MovingAverage,
    Median,
    Gaussian
};

// 基线校正方法
enum class BaselineMethod {
    Linear,
    Polynomial,
    Spline,
    MovingAverage,
    AsymmetricLeastSquares,
    Airpls,
    Arpls
};

// 噪声分析结果
struct NoiseAnalysis {
    double rms;                    // 均方根噪声
    double snr;                    // 信噪比
    double peak_to_peak;           // 峰峰值
    double standard_deviation;     // 标准差
    double mean_absolute_deviation; // 平均绝对偏差
    std::vector<double> power_spectrum; // 功率谱
    std::vector<double> frequencies;    // 频率轴
};

// 滤波参数
struct FilterParams {
    FilterType type = FilterType::LowPass;
    double cutoff_frequency = 1000.0;     // 截止频率 (Hz)
    double low_frequency = 100.0;         // 低频截止 (Hz, 用于带通/带阻)
    double high_frequency = 1000.0;       // 高频截止 (Hz, 用于带通/带阻)
    int order = 4;                        // 滤波器阶数
    double ripple = 0.5;                  // 纹波 (dB, 用于Chebyshev)
    int window_size = 5;                  // 窗口大小 (用于移动平均/中值滤波)
    double sigma = 1.0;                   // 高斯滤波标准差
    double sampling_rate = 10000.0;       // 采样率 (Hz)
};

// 基线校正参数
struct BaselineParams {
    BaselineMethod method = BaselineMethod::Linear;
    int polynomial_order = 3;             // 多项式阶数
    int spline_knots = 10;                // 样条节点数
    int window_size = 100;                // 移动平均窗口大小
    double lambda = 1e6;                  // ALS平滑参数
    double p = 0.001;                     // ALS不对称参数
    int max_iterations = 10;              // 最大迭代次数
    double tolerance = 1e-6;              // 收敛容差
};

// 数据质量评估
struct DataQuality {
    double completeness;          // 数据完整性 (0-1)
    double consistency;           // 数据一致性 (0-1)
    double stability;             // 数据稳定性 (0-1)
    int outlier_count;           // 异常值数量
    std::vector<int> outlier_indices; // 异常值索引
    double drift_rate;           // 漂移率
    std::string quality_grade;   // 质量等级 (A/B/C/D)
};

// 滤波函数
class Filter {
public:
    static std::vector<double> lowPass(const std::vector<double>& data, 
                                      const FilterParams& params);
    static std::vector<double> highPass(const std::vector<double>& data, 
                                       const FilterParams& params);
    static std::vector<double> bandPass(const std::vector<double>& data, 
                                       const FilterParams& params);
    static std::vector<double> bandStop(const std::vector<double>& data, 
                                       const FilterParams& params);
    static std::vector<double> butterworth(const std::vector<double>& data, 
                                          const FilterParams& params);
    static std::vector<double> movingAverage(const std::vector<double>& data, 
                                            int window_size);
    static std::vector<double> medianFilter(const std::vector<double>& data, 
                                           int window_size);
    static std::vector<double> gaussianFilter(const std::vector<double>& data, 
                                             double sigma);
    
    // 通用滤波接口
    static std::vector<double> apply(const std::vector<double>& data, 
                                    const FilterParams& params);
};

// 基线校正类
class BaselineCorrection {
public:
    static std::vector<double> linear(const std::vector<double>& data);
    static std::vector<double> polynomial(const std::vector<double>& data, 
                                         int order);
    static std::vector<double> spline(const std::vector<double>& data, 
                                     int knots);
    static std::vector<double> movingAverage(const std::vector<double>& data, 
                                            int window_size);
    static std::vector<double> asymmetricLeastSquares(const std::vector<double>& data, 
                                                     const BaselineParams& params);
    static std::vector<double> airpls(const std::vector<double>& data, 
                                     const BaselineParams& params);
    
    // 通用基线校正接口
    static std::vector<double> correct(const std::vector<double>& data, 
                                      const BaselineParams& params);
    
    // 获取基线
    static std::vector<double> getBaseline(const std::vector<double>& data, 
                                          const BaselineParams& params);
};

// 噪声分析类
class NoiseAnalyzer {
public:
    static NoiseAnalysis analyze(const std::vector<double>& data, 
                                double sampling_rate = 10000.0);
    static double calculateRMS(const std::vector<double>& data);
    static double calculateSNR(const std::vector<double>& signal, 
                              const std::vector<double>& noise);
    static double calculatePeakToPeak(const std::vector<double>& data);
    static std::vector<double> calculatePowerSpectrum(const std::vector<double>& data, 
                                                     double sampling_rate);
    static std::vector<double> generateFrequencyAxis(int data_size, 
                                                    double sampling_rate);
};

// 数据质量评估类
class QualityAssessment {
public:
    static DataQuality assess(const std::vector<double>& data, 
                             double sampling_rate = 10000.0);
    static std::vector<int> detectOutliers(const std::vector<double>& data, 
                                          double threshold = 3.0);
    static double calculateDriftRate(const std::vector<double>& data, 
                                   double sampling_rate);
    static double calculateStability(const std::vector<double>& data);
    static std::string gradeQuality(const DataQuality& quality);
};

// 数据预处理管道
class PreprocessingPipeline {
public:
    PreprocessingPipeline();
    ~PreprocessingPipeline();
    
    // 添加处理步骤
    void addFilter(const FilterParams& params);
    void addBaselineCorrection(const BaselineParams& params);
    void addNoiseReduction(double threshold = 0.1);
    void addOutlierRemoval(double threshold = 3.0);
    
    // 执行预处理
    std::vector<double> process(const std::vector<double>& data);
    
    // 获取处理历史
    std::vector<std::string> getProcessingHistory() const;
    
    // 清除所有步骤
    void clear();

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl_;
};

// 实时数据处理器
class RealTimeProcessor {
public:
    RealTimeProcessor(int buffer_size = 1000);
    ~RealTimeProcessor();
    
    // 添加新数据点
    void addDataPoint(double value);
    void addDataPoints(const std::vector<double>& values);
    
    // 获取处理后的数据
    std::vector<double> getFilteredData() const;
    std::vector<double> getBaselineCorrectedData() const;
    
    // 配置实时处理
    void setFilterParams(const FilterParams& params);
    void setBaselineParams(const BaselineParams& params);
    void enableRealTimeAnalysis(bool enable);
    
    // 获取实时统计
    NoiseAnalysis getCurrentNoiseAnalysis() const;
    DataQuality getCurrentQuality() const;
    
    // 重置缓冲区
    void reset();

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl_;
};

// 批量数据处理器
class BatchProcessor {
public:
    static std::vector<std::vector<double>> processMultiple(
        const std::vector<std::vector<double>>& datasets,
        const FilterParams& filter_params,
        const BaselineParams& baseline_params);
    
    static std::vector<NoiseAnalysis> analyzeMultiple(
        const std::vector<std::vector<double>>& datasets,
        double sampling_rate = 10000.0);
    
    static std::vector<DataQuality> assessMultiple(
        const std::vector<std::vector<double>>& datasets,
        double sampling_rate = 10000.0);
};

// 数学工具函数
namespace MathUtils {
    std::vector<double> convolve(const std::vector<double>& signal, 
                                const std::vector<double>& kernel);
    std::vector<double> correlate(const std::vector<double>& x, 
                                 const std::vector<double>& y);
    std::vector<double> fft(const std::vector<double>& data);
    std::vector<double> ifft(const std::vector<double>& data);
    std::vector<double> hilbertTransform(const std::vector<double>& data);
    std::vector<double> derivative(const std::vector<double>& data, 
                                  double dx = 1.0);
    std::vector<double> integrate(const std::vector<double>& data, 
                                 double dx = 1.0);
    double pearsonCorrelation(const std::vector<double>& x, 
                             const std::vector<double>& y);
    std::vector<double> resample(const std::vector<double>& data, 
                                int new_size);
}

} // namespace DataProcessor