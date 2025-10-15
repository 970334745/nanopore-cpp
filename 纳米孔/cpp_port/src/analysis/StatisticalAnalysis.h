#ifndef STATISTICAL_ANALYSIS_H
#define STATISTICAL_ANALYSIS_H

#include <vector>
#include <map>
#include <string>
#include <memory>

namespace Analysis {

// 基础统计信息结构
struct BasicStatistics {
    double mean = 0.0;
    double median = 0.0;
    double mode = 0.0;
    double std_dev = 0.0;
    double variance = 0.0;
    double skewness = 0.0;
    double kurtosis = 0.0;
    double min = 0.0;
    double max = 0.0;
    double range = 0.0;
    double q1 = 0.0;  // 第一四分位数
    double q3 = 0.0;  // 第三四分位数
    double iqr = 0.0; // 四分位距
    size_t count = 0;
    size_t valid_count = 0;
    size_t missing_count = 0;
};

// 分布测试结果
struct DistributionTest {
    std::string distribution_name;
    double test_statistic = 0.0;
    double p_value = 0.0;
    bool is_significant = false;
    double confidence_level = 0.95;
    std::map<std::string, double> parameters;
};

// 相关性分析结果
struct CorrelationResult {
    double pearson_r = 0.0;
    double spearman_rho = 0.0;
    double kendall_tau = 0.0;
    double p_value_pearson = 0.0;
    double p_value_spearman = 0.0;
    double p_value_kendall = 0.0;
    bool is_significant = false;
    double confidence_level = 0.95;
};

// 回归分析结果
struct RegressionResult {
    std::vector<double> coefficients;
    std::vector<std::string> coefficient_names;
    double r_squared = 0.0;
    double adjusted_r_squared = 0.0;
    double f_statistic = 0.0;
    double p_value = 0.0;
    double standard_error = 0.0;
    std::vector<double> residuals;
    std::vector<double> fitted_values;
    std::vector<double> confidence_intervals;
};

// 假设检验结果
struct HypothesisTest {
    std::string test_name;
    double test_statistic = 0.0;
    double p_value = 0.0;
    bool reject_null = false;
    double confidence_level = 0.95;
    std::string alternative = "two-sided";
    std::map<std::string, double> additional_info;
};

// 时间序列分析结果
struct TimeSeriesAnalysis {
    BasicStatistics statistics;
    std::vector<double> trend;
    std::vector<double> seasonal;
    std::vector<double> residual;
    double trend_slope = 0.0;
    double seasonal_period = 0.0;
    bool has_trend = false;
    bool has_seasonality = false;
    std::vector<double> autocorrelation;
    std::vector<double> partial_autocorrelation;
};

// 主成分分析结果
struct PCAResult {
    std::vector<std::vector<double>> principal_components;
    std::vector<double> explained_variance;
    std::vector<double> explained_variance_ratio;
    std::vector<double> cumulative_variance_ratio;
    std::vector<std::vector<double>> loadings;
    std::vector<std::vector<double>> transformed_data;
    int n_components = 0;
};

// 聚类分析结果
struct ClusteringResult {
    std::vector<int> labels;
    std::vector<std::vector<double>> centroids;
    std::vector<double> cluster_sizes;
    double silhouette_score = 0.0;
    double inertia = 0.0;
    int n_clusters = 0;
    std::string algorithm;
};

// 统计分析器类
class StatisticalAnalyzer {
public:
    StatisticalAnalyzer();
    ~StatisticalAnalyzer();

    // 基础统计分析
    BasicStatistics calculateBasicStatistics(const std::vector<double>& data);
    std::vector<double> calculatePercentiles(const std::vector<double>& data, 
                                           const std::vector<double>& percentiles);
    std::vector<double> calculateMovingAverage(const std::vector<double>& data, 
                                             int window_size);
    std::vector<double> calculateMovingStd(const std::vector<double>& data, 
                                         int window_size);

    // 分布分析
    DistributionTest testNormality(const std::vector<double>& data);
    DistributionTest testUniformity(const std::vector<double>& data);
    DistributionTest testExponentiality(const std::vector<double>& data);
    std::vector<double> generateHistogram(const std::vector<double>& data, 
                                        int bins = 50);
    std::vector<double> estimateKernelDensity(const std::vector<double>& data, 
                                            const std::vector<double>& x_points);

    // 相关性分析
    CorrelationResult calculateCorrelation(const std::vector<double>& x, 
                                         const std::vector<double>& y);
    std::vector<std::vector<double>> calculateCorrelationMatrix(
        const std::vector<std::vector<double>>& data);
    std::vector<std::vector<double>> calculateCovarianceMatrix(
        const std::vector<std::vector<double>>& data);

    // 回归分析
    RegressionResult linearRegression(const std::vector<double>& x, 
                                    const std::vector<double>& y);
    RegressionResult multipleRegression(const std::vector<std::vector<double>>& X, 
                                      const std::vector<double>& y);
    RegressionResult polynomialRegression(const std::vector<double>& x, 
                                        const std::vector<double>& y, 
                                        int degree);

    // 假设检验
    HypothesisTest tTest(const std::vector<double>& sample1, 
                        const std::vector<double>& sample2 = {});
    HypothesisTest chiSquareTest(const std::vector<std::vector<double>>& observed);
    HypothesisTest anovaTest(const std::vector<std::vector<double>>& groups);
    HypothesisTest kolmogorovSmirnovTest(const std::vector<double>& sample1, 
                                       const std::vector<double>& sample2 = {});

    // 时间序列分析
    TimeSeriesAnalysis analyzeTimeSeries(const std::vector<double>& data, 
                                        double sampling_rate = 1.0);
    std::vector<double> calculateAutocorrelation(const std::vector<double>& data, 
                                                int max_lag = 50);
    std::vector<double> calculatePartialAutocorrelation(const std::vector<double>& data, 
                                                       int max_lag = 50);
    std::vector<double> detrend(const std::vector<double>& data);
    std::vector<double> differencing(const std::vector<double>& data, int order = 1);

    // 多变量分析
    PCAResult performPCA(const std::vector<std::vector<double>>& data, 
                        int n_components = -1);
    ClusteringResult performKMeans(const std::vector<std::vector<double>>& data, 
                                 int n_clusters);
    ClusteringResult performHierarchicalClustering(
        const std::vector<std::vector<double>>& data, int n_clusters);

    // 异常值检测
    std::vector<int> detectOutliersIQR(const std::vector<double>& data, 
                                     double factor = 1.5);
    std::vector<int> detectOutliersZScore(const std::vector<double>& data, 
                                        double threshold = 3.0);
    std::vector<int> detectOutliersModifiedZScore(const std::vector<double>& data, 
                                                 double threshold = 3.5);

    // 重采样和Bootstrap
    std::vector<double> bootstrap(const std::vector<double>& data, 
                                int n_samples = 1000);
    std::vector<double> jackknife(const std::vector<double>& data);
    std::vector<double> permutationTest(const std::vector<double>& sample1, 
                                      const std::vector<double>& sample2, 
                                      int n_permutations = 1000);

    // 配置参数
    void setConfidenceLevel(double level) { confidence_level_ = level; }
    void setRandomSeed(unsigned int seed) { random_seed_ = seed; }

private:
    class Impl;
    std::unique_ptr<Impl> pimpl_;
    
    double confidence_level_ = 0.95;
    unsigned int random_seed_ = 42;
};

// 工具函数
namespace StatUtils {
    double calculateMean(const std::vector<double>& data);
    double calculateMedian(std::vector<double> data);
    double calculateMode(const std::vector<double>& data);
    double calculateStandardDeviation(const std::vector<double>& data);
    double calculateVariance(const std::vector<double>& data);
    double calculateSkewness(const std::vector<double>& data);
    double calculateKurtosis(const std::vector<double>& data);
    double calculatePercentile(std::vector<double> data, double percentile);
    
    // 分布函数
    double normalPDF(double x, double mean = 0.0, double std_dev = 1.0);
    double normalCDF(double x, double mean = 0.0, double std_dev = 1.0);
    double tPDF(double x, double df);
    double tCDF(double x, double df);
    double chiSquarePDF(double x, double df);
    double chiSquareCDF(double x, double df);
    double fPDF(double x, double df1, double df2);
    double fCDF(double x, double df1, double df2);
    
    // 逆分布函数
    double normalInverseCDF(double p, double mean = 0.0, double std_dev = 1.0);
    double tInverseCDF(double p, double df);
    double chiSquareInverseCDF(double p, double df);
    double fInverseCDF(double p, double df1, double df2);
    
    // 矩阵运算
    std::vector<std::vector<double>> matrixMultiply(
        const std::vector<std::vector<double>>& A,
        const std::vector<std::vector<double>>& B);
    std::vector<std::vector<double>> matrixInverse(
        const std::vector<std::vector<double>>& matrix);
    std::vector<std::vector<double>> matrixTranspose(
        const std::vector<std::vector<double>>& matrix);
    double matrixDeterminant(const std::vector<std::vector<double>>& matrix);
    
    // 随机数生成
    std::vector<double> generateNormalRandom(int n, double mean = 0.0, double std_dev = 1.0);
    std::vector<double> generateUniformRandom(int n, double min = 0.0, double max = 1.0);
    std::vector<int> generateRandomIndices(int n, int max_index);
}

} // namespace Analysis

#endif // STATISTICAL_ANALYSIS_H