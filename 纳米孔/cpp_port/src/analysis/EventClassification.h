#ifndef EVENT_CLASSIFICATION_H
#define EVENT_CLASSIFICATION_H

#include <vector>
#include <string>
#include <map>
#include <memory>

namespace Analysis {

// 事件特征结构
struct EventFeatures {
    // 基本特征
    double duration = 0.0;
    double amplitude = 0.0;
    double area = 0.0;
    double peak_position = 0.0;
    double baseline_before = 0.0;
    double baseline_after = 0.0;
    
    // 形状特征
    double rise_time = 0.0;
    double fall_time = 0.0;
    double asymmetry = 0.0;
    double skewness = 0.0;
    double kurtosis = 0.0;
    double width_at_half_max = 0.0;
    
    // 频域特征
    double dominant_frequency = 0.0;
    double spectral_centroid = 0.0;
    double spectral_spread = 0.0;
    double spectral_entropy = 0.0;
    
    // 统计特征
    double mean = 0.0;
    double std_dev = 0.0;
    double variance = 0.0;
    double rms = 0.0;
    double energy = 0.0;
    
    // 导数特征
    double max_derivative = 0.0;
    double min_derivative = 0.0;
    double derivative_variance = 0.0;
    
    // 自定义特征
    std::map<std::string, double> custom_features;
};

// 事件类别
struct EventClass {
    std::string name;
    int class_id = -1;
    std::string description;
    std::map<std::string, double> typical_features;
    double confidence_threshold = 0.5;
    std::vector<std::string> required_features;
};

// 分类结果
struct ClassificationResult {
    int predicted_class = -1;
    std::string class_name;
    double confidence = 0.0;
    std::vector<double> class_probabilities;
    std::vector<std::string> class_names;
    std::map<std::string, double> feature_importance;
    bool is_reliable = false;
};

// 训练数据
struct TrainingData {
    std::vector<EventFeatures> features;
    std::vector<int> labels;
    std::vector<std::string> class_names;
    std::map<int, std::string> label_to_name;
    std::map<std::string, int> name_to_label;
};

// 模型评估结果
struct ModelEvaluation {
    double accuracy = 0.0;
    double precision = 0.0;
    double recall = 0.0;
    double f1_score = 0.0;
    std::vector<std::vector<int>> confusion_matrix;
    std::vector<double> class_accuracies;
    std::vector<double> class_precisions;
    std::vector<double> class_recalls;
    std::vector<double> class_f1_scores;
    std::map<std::string, double> metrics;
};

// 特征选择结果
struct FeatureSelection {
    std::vector<std::string> selected_features;
    std::vector<double> feature_scores;
    std::vector<int> feature_indices;
    std::string selection_method;
    std::map<std::string, double> selection_parameters;
};

// 前向声明 - ClusteringResult在StatisticalAnalysis.h中定义
struct ClusteringResult;

// 异常检测结果
struct AnomalyDetection {
    std::vector<bool> is_anomaly;
    std::vector<double> anomaly_scores;
    double threshold = 0.0;
    int n_anomalies = 0;
    std::string detection_method;
};

// 事件分类器基类
class EventClassifier {
public:
    virtual ~EventClassifier() = default;
    
    virtual void train(const TrainingData& data) = 0;
    virtual ClassificationResult classify(const EventFeatures& features) = 0;
    virtual std::vector<ClassificationResult> classifyBatch(
        const std::vector<EventFeatures>& features) = 0;
    virtual ModelEvaluation evaluate(const TrainingData& test_data) = 0;
    virtual void saveModel(const std::string& filename) = 0;
    virtual void loadModel(const std::string& filename) = 0;
    virtual std::string getModelType() const = 0;
};

// 支持向量机分类器
class SVMClassifier : public EventClassifier {
public:
    SVMClassifier();
    ~SVMClassifier() override;
    
    void train(const TrainingData& data) override;
    ClassificationResult classify(const EventFeatures& features) override;
    std::vector<ClassificationResult> classifyBatch(
        const std::vector<EventFeatures>& features) override;
    ModelEvaluation evaluate(const TrainingData& test_data) override;
    void saveModel(const std::string& filename) override;
    void loadModel(const std::string& filename) override;
    std::string getModelType() const override { return "SVM"; }
    
    // SVM特定参数
    void setKernel(const std::string& kernel) { kernel_ = kernel; }
    void setC(double c) { c_ = c; }
    void setGamma(double gamma) { gamma_ = gamma; }

private:
    class Impl;
    std::unique_ptr<Impl> pimpl_;
    std::string kernel_ = "rbf";
    double c_ = 1.0;
    double gamma_ = 0.1;
};

// 随机森林分类器
class RandomForestClassifier : public EventClassifier {
public:
    RandomForestClassifier();
    ~RandomForestClassifier() override;
    
    void train(const TrainingData& data) override;
    ClassificationResult classify(const EventFeatures& features) override;
    std::vector<ClassificationResult> classifyBatch(
        const std::vector<EventFeatures>& features) override;
    ModelEvaluation evaluate(const TrainingData& test_data) override;
    void saveModel(const std::string& filename) override;
    void loadModel(const std::string& filename) override;
    std::string getModelType() const override { return "RandomForest"; }
    
    // 随机森林特定参数
    void setNumTrees(int n_trees) { n_trees_ = n_trees; }
    void setMaxDepth(int max_depth) { max_depth_ = max_depth; }
    void setMinSamplesSplit(int min_samples) { min_samples_split_ = min_samples; }

private:
    class Impl;
    std::unique_ptr<Impl> pimpl_;
    int n_trees_ = 100;
    int max_depth_ = 10;
    int min_samples_split_ = 2;
};

// 神经网络分类器
class NeuralNetworkClassifier : public EventClassifier {
public:
    NeuralNetworkClassifier();
    ~NeuralNetworkClassifier() override;
    
    void train(const TrainingData& data) override;
    ClassificationResult classify(const EventFeatures& features) override;
    std::vector<ClassificationResult> classifyBatch(
        const std::vector<EventFeatures>& features) override;
    ModelEvaluation evaluate(const TrainingData& test_data) override;
    void saveModel(const std::string& filename) override;
    void loadModel(const std::string& filename) override;
    std::string getModelType() const override { return "NeuralNetwork"; }
    
    // 神经网络特定参数
    void setHiddenLayers(const std::vector<int>& layers) { hidden_layers_ = layers; }
    void setLearningRate(double lr) { learning_rate_ = lr; }
    void setEpochs(int epochs) { epochs_ = epochs; }
    void setBatchSize(int batch_size) { batch_size_ = batch_size; }

private:
    class Impl;
    std::unique_ptr<Impl> pimpl_;
    std::vector<int> hidden_layers_ = {64, 32};
    double learning_rate_ = 0.001;
    int epochs_ = 100;
    int batch_size_ = 32;
};

// 事件分类管理器
class EventClassificationManager {
public:
    EventClassificationManager();
    ~EventClassificationManager();
    
    // 特征提取
    EventFeatures extractFeatures(const std::vector<double>& event_data,
                                double sampling_rate = 1.0);
    std::vector<EventFeatures> extractFeaturesFromEvents(
        const std::vector<std::vector<double>>& events,
        double sampling_rate = 1.0);
    
    // 特征预处理
    std::vector<EventFeatures> normalizeFeatures(
        const std::vector<EventFeatures>& features);
    std::vector<EventFeatures> standardizeFeatures(
        const std::vector<EventFeatures>& features);
    FeatureSelection selectFeatures(const TrainingData& data,
                                  const std::string& method = "mutual_info",
                                  int n_features = 10);
    
    // 分类器管理
    void registerClassifier(const std::string& name, 
                          std::unique_ptr<EventClassifier> classifier);
    void setActiveClassifier(const std::string& name);
    EventClassifier* getActiveClassifier();
    std::vector<std::string> getAvailableClassifiers() const;
    
    // 训练和评估
    void trainClassifier(const TrainingData& data);
    ClassificationResult classifyEvent(const EventFeatures& features);
    std::vector<ClassificationResult> classifyEvents(
        const std::vector<EventFeatures>& features);
    ModelEvaluation evaluateClassifier(const TrainingData& test_data);
    
    // 无监督学习
    ClusteringResult performClustering(const std::vector<EventFeatures>& features,
                                     const std::string& method = "kmeans",
                                     int n_clusters = 3);
    AnomalyDetection detectAnomalies(const std::vector<EventFeatures>& features,
                                   const std::string& method = "isolation_forest");
    
    // 数据管理
    TrainingData loadTrainingData(const std::string& filename);
    void saveTrainingData(const TrainingData& data, const std::string& filename);
    TrainingData splitTrainingData(const TrainingData& data, 
                                 double train_ratio = 0.8);
    
    // 模型管理
    void saveModel(const std::string& filename);
    void loadModel(const std::string& filename);
    void exportModelInfo(const std::string& filename);
    
    // 配置
    void setFeatureNormalization(bool enable) { normalize_features_ = enable; }
    void setConfidenceThreshold(double threshold) { confidence_threshold_ = threshold; }
    void setRandomSeed(unsigned int seed) { random_seed_ = seed; }

private:
    class Impl;
    std::unique_ptr<Impl> pimpl_;
    
    bool normalize_features_ = true;
    double confidence_threshold_ = 0.5;
    unsigned int random_seed_ = 42;
};

// 工具函数
namespace ClassificationUtils {
    // 特征提取工具
    double calculateRiseTime(const std::vector<double>& event_data, 
                           double threshold = 0.1);
    double calculateFallTime(const std::vector<double>& event_data, 
                           double threshold = 0.9);
    double calculateAsymmetry(const std::vector<double>& event_data);
    double calculateWidthAtHalfMax(const std::vector<double>& event_data);
    
    // 特征向量转换
    std::vector<double> featuresToVector(const EventFeatures& features,
                                       const std::vector<std::string>& feature_names);
    EventFeatures vectorToFeatures(const std::vector<double>& vector,
                                 const std::vector<std::string>& feature_names);
    
    // 数据预处理
    std::vector<std::vector<double>> normalizeMatrix(
        const std::vector<std::vector<double>>& matrix);
    std::vector<std::vector<double>> standardizeMatrix(
        const std::vector<std::vector<double>>& matrix);
    
    // 评估指标
    double calculateAccuracy(const std::vector<int>& true_labels,
                           const std::vector<int>& predicted_labels);
    double calculatePrecision(const std::vector<int>& true_labels,
                            const std::vector<int>& predicted_labels,
                            int class_label);
    double calculateRecall(const std::vector<int>& true_labels,
                         const std::vector<int>& predicted_labels,
                         int class_label);
    double calculateF1Score(double precision, double recall);
    std::vector<std::vector<int>> calculateConfusionMatrix(
        const std::vector<int>& true_labels,
        const std::vector<int>& predicted_labels,
        int n_classes);
    
    // 交叉验证
    std::vector<std::pair<TrainingData, TrainingData>> createKFolds(
        const TrainingData& data, int k = 5);
    ModelEvaluation performCrossValidation(EventClassifier* classifier,
                                         const TrainingData& data,
                                         int k = 5);
}

} // namespace Analysis

#endif // EVENT_CLASSIFICATION_H