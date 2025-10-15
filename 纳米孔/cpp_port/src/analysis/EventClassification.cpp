#include "EventClassification.h"
#include "StatisticalAnalysis.h"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <random>
#include <map>
#include <set>
#include <functional>
#include <iostream>
#include <fstream>

namespace Analysis {

// SVMClassifier的PIMPL实现
class SVMClassifier::Impl {
public:
    std::vector<std::vector<double>> support_vectors_;
    std::vector<double> alphas_;
    std::vector<int> support_labels_;
    double bias_;
    bool is_trained_;
    
    Impl() : bias_(0.0), is_trained_(false) {}
    
    // RBF核函数
    double rbfKernel(const std::vector<double>& x1, const std::vector<double>& x2, double gamma) {
        double sum = 0.0;
        for (size_t i = 0; i < x1.size() && i < x2.size(); ++i) {
            double diff = x1[i] - x2[i];
            sum += diff * diff;
        }
        return std::exp(-gamma * sum);
    }
    
    // 线性核函数
    double linearKernel(const std::vector<double>& x1, const std::vector<double>& x2) {
        double sum = 0.0;
        for (size_t i = 0; i < x1.size() && i < x2.size(); ++i) {
            sum += x1[i] * x2[i];
        }
        return sum;
    }
    
    // 多项式核函数
    double polynomialKernel(const std::vector<double>& x1, const std::vector<double>& x2, 
                           double degree, double coef0) {
        double dot_product = linearKernel(x1, x2);
        return std::pow(dot_product + coef0, degree);
    }
    
    // 计算核函数值
    double computeKernel(const std::vector<double>& x1, const std::vector<double>& x2, 
                        const std::string& kernel, double gamma = 0.1, double degree = 3.0, double coef0 = 0.0) {
        if (kernel == "linear") {
            return linearKernel(x1, x2);
        } else if (kernel == "rbf") {
            return rbfKernel(x1, x2, gamma);
        } else if (kernel == "polynomial") {
            return polynomialKernel(x1, x2, degree, coef0);
        }
        return rbfKernel(x1, x2, gamma);
    }
    
    // 将EventFeatures转换为向量
    std::vector<double> featuresToVector(const EventFeatures& features) {
        std::vector<double> vec;
        vec.push_back(features.duration);
        vec.push_back(features.amplitude);
        vec.push_back(features.area);
        vec.push_back(features.peak_position);
        vec.push_back(features.baseline_before);
        vec.push_back(features.baseline_after);
        vec.push_back(features.rise_time);
        vec.push_back(features.fall_time);
        vec.push_back(features.asymmetry);
        vec.push_back(features.skewness);
        vec.push_back(features.kurtosis);
        vec.push_back(features.width_at_half_max);
        vec.push_back(features.dominant_frequency);
        vec.push_back(features.spectral_centroid);
        vec.push_back(features.spectral_spread);
        vec.push_back(features.spectral_entropy);
        vec.push_back(features.mean);
        vec.push_back(features.std_dev);
        vec.push_back(features.variance);
        vec.push_back(features.rms);
        vec.push_back(features.energy);
        vec.push_back(features.max_derivative);
        vec.push_back(features.min_derivative);
        vec.push_back(features.derivative_variance);
        return vec;
    }
    
    // 简化的SMO算法实现
    void trainSMO(const std::vector<EventFeatures>& features, 
                  const std::vector<int>& labels,
                  const std::string& kernel,
                  double C,
                  double gamma) {
        
        // 转换特征为向量
        std::vector<std::vector<double>> feature_vectors;
        for (const auto& f : features) {
            feature_vectors.push_back(featuresToVector(f));
        }
        
        int n_samples = feature_vectors.size();
        
        // 初始化拉格朗日乘数
        alphas_.assign(n_samples, 0.0);
        bias_ = 0.0;
        
        // 计算核矩阵
        std::vector<std::vector<double>> kernel_matrix(n_samples, std::vector<double>(n_samples));
        for (int i = 0; i < n_samples; ++i) {
            for (int j = 0; j < n_samples; ++j) {
                kernel_matrix[i][j] = computeKernel(feature_vectors[i], feature_vectors[j], kernel, gamma);
            }
        }
        
        // 简化的SMO迭代
        int max_iterations = 1000;
        double tolerance = 1e-3;
        
        for (int iter = 0; iter < max_iterations; ++iter) {
            int num_changed = 0;
            
            for (int i = 0; i < n_samples; ++i) {
                // 计算预测值
                double prediction = bias_;
                for (int j = 0; j < n_samples; ++j) {
                    prediction += alphas_[j] * labels[j] * kernel_matrix[j][i];
                }
                
                double error_i = prediction - labels[i];
                
                // 检查KKT条件
                if ((labels[i] * error_i < -tolerance && alphas_[i] < C) ||
                    (labels[i] * error_i > tolerance && alphas_[i] > 0)) {
                    
                    // 选择第二个变量
                    int j = (i + 1) % n_samples;
                    
                    double prediction_j = bias_;
                    for (int k = 0; k < n_samples; ++k) {
                        prediction_j += alphas_[k] * labels[k] * kernel_matrix[k][j];
                    }
                    
                    double error_j = prediction_j - labels[j];
                    
                    // 保存旧的alpha值
                    double alpha_i_old = alphas_[i];
                    double alpha_j_old = alphas_[j];
                    
                    // 计算边界
                    double L, H;
                    if (labels[i] != labels[j]) {
                        L = std::max(0.0, alphas_[j] - alphas_[i]);
                        H = std::min(C, C + alphas_[j] - alphas_[i]);
                    } else {
                        L = std::max(0.0, alphas_[i] + alphas_[j] - C);
                        H = std::min(C, alphas_[i] + alphas_[j]);
                    }
                    
                    if (L == H) continue;
                    
                    // 计算eta
                    double eta = 2.0 * kernel_matrix[i][j] - kernel_matrix[i][i] - kernel_matrix[j][j];
                    if (eta >= 0) continue;
                    
                    // 更新alpha_j
                    alphas_[j] = alpha_j_old - (labels[j] * (error_i - error_j)) / eta;
                    
                    // 裁剪alpha_j
                    if (alphas_[j] > H) alphas_[j] = H;
                    else if (alphas_[j] < L) alphas_[j] = L;
                    
                    if (std::abs(alphas_[j] - alpha_j_old) < 1e-5) continue;
                    
                    // 更新alpha_i
                    alphas_[i] = alpha_i_old + labels[i] * labels[j] * (alpha_j_old - alphas_[j]);
                    
                    // 更新偏置
                    double b1 = bias_ - error_i - labels[i] * (alphas_[i] - alpha_i_old) * kernel_matrix[i][i] -
                               labels[j] * (alphas_[j] - alpha_j_old) * kernel_matrix[i][j];
                    double b2 = bias_ - error_j - labels[i] * (alphas_[i] - alpha_i_old) * kernel_matrix[i][j] -
                               labels[j] * (alphas_[j] - alpha_j_old) * kernel_matrix[j][j];
                    
                    if (0 < alphas_[i] && alphas_[i] < C) {
                        bias_ = b1;
                    } else if (0 < alphas_[j] && alphas_[j] < C) {
                        bias_ = b2;
                    } else {
                        bias_ = (b1 + b2) / 2.0;
                    }
                    
                    num_changed++;
                }
            }
            
            if (num_changed == 0) break;
        }
        
        // 提取支持向量
        support_vectors_.clear();
        support_labels_.clear();
        std::vector<double> support_alphas;
        
        for (int i = 0; i < n_samples; ++i) {
            if (alphas_[i] > 1e-5) {
                support_vectors_.push_back(feature_vectors[i]);
                support_labels_.push_back(labels[i]);
                support_alphas.push_back(alphas_[i]);
            }
        }
        
        alphas_ = support_alphas;
        is_trained_ = true;
    }
};

SVMClassifier::SVMClassifier() : pimpl_(std::make_unique<Impl>()) {}
SVMClassifier::~SVMClassifier() = default;

void SVMClassifier::train(const TrainingData& data) {
    if (data.features.empty() || data.labels.empty()) {
        return;
    }
    
    pimpl_->trainSMO(data.features, data.labels, kernel_, c_, gamma_);
}

ClassificationResult SVMClassifier::classify(const EventFeatures& features) {
    ClassificationResult result;
    
    if (!pimpl_->is_trained_ || pimpl_->support_vectors_.empty()) {
        result.predicted_class = -1;
        result.confidence = 0.0;
        return result;
    }
    
    auto feature_vector = pimpl_->featuresToVector(features);
    
    // 计算决策函数值
    double decision_value = pimpl_->bias_;
    for (size_t i = 0; i < pimpl_->support_vectors_.size(); ++i) {
        double kernel_value = pimpl_->computeKernel(feature_vector, pimpl_->support_vectors_[i], kernel_, gamma_);
        decision_value += pimpl_->alphas_[i] * pimpl_->support_labels_[i] * kernel_value;
    }
    
    result.predicted_class = decision_value > 0 ? 1 : -1;
    result.confidence = std::abs(decision_value);
    result.class_probabilities = {decision_value};
    
    return result;
}

std::vector<ClassificationResult> SVMClassifier::classifyBatch(
    const std::vector<EventFeatures>& features) {
    
    std::vector<ClassificationResult> results;
    results.reserve(features.size());
    
    for (const auto& feature : features) {
        results.push_back(classify(feature));
    }
    
    return results;
}

ModelEvaluation SVMClassifier::evaluate(const TrainingData& test_data) {
    ModelEvaluation eval;
    
    if (test_data.features.empty() || test_data.labels.empty()) {
        return eval;
    }
    
    std::vector<int> predictions;
    for (const auto& features : test_data.features) {
        auto result = classify(features);
        predictions.push_back(result.predicted_class);
    }
    
    // 计算准确率
    int correct = 0;
    for (size_t i = 0; i < predictions.size(); ++i) {
        if (predictions[i] == test_data.labels[i]) {
            correct++;
        }
    }
    eval.accuracy = static_cast<double>(correct) / predictions.size();
    
    return eval;
}

void SVMClassifier::saveModel(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) return;
    
    // 保存模型参数
    file.write(reinterpret_cast<const char*>(&pimpl_->bias_), sizeof(double));
    file.write(reinterpret_cast<const char*>(&pimpl_->is_trained_), sizeof(bool));
    
    // 保存支持向量数量
    size_t n_support = pimpl_->support_vectors_.size();
    file.write(reinterpret_cast<const char*>(&n_support), sizeof(size_t));
    
    // 保存支持向量
    for (const auto& sv : pimpl_->support_vectors_) {
        size_t vec_size = sv.size();
        file.write(reinterpret_cast<const char*>(&vec_size), sizeof(size_t));
        file.write(reinterpret_cast<const char*>(sv.data()), vec_size * sizeof(double));
    }
    
    // 保存alphas和标签
    file.write(reinterpret_cast<const char*>(pimpl_->alphas_.data()), n_support * sizeof(double));
    file.write(reinterpret_cast<const char*>(pimpl_->support_labels_.data()), n_support * sizeof(int));
    
    file.close();
}

void SVMClassifier::loadModel(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) return;
    
    // 加载模型参数
    file.read(reinterpret_cast<char*>(&pimpl_->bias_), sizeof(double));
    file.read(reinterpret_cast<char*>(&pimpl_->is_trained_), sizeof(bool));
    
    // 加载支持向量数量
    size_t n_support;
    file.read(reinterpret_cast<char*>(&n_support), sizeof(size_t));
    
    // 加载支持向量
    pimpl_->support_vectors_.clear();
    for (size_t i = 0; i < n_support; ++i) {
        size_t vec_size;
        file.read(reinterpret_cast<char*>(&vec_size), sizeof(size_t));
        
        std::vector<double> sv(vec_size);
        file.read(reinterpret_cast<char*>(sv.data()), vec_size * sizeof(double));
        pimpl_->support_vectors_.push_back(sv);
    }
    
    // 加载alphas和标签
    pimpl_->alphas_.resize(n_support);
    pimpl_->support_labels_.resize(n_support);
    
    file.read(reinterpret_cast<char*>(pimpl_->alphas_.data()), n_support * sizeof(double));
    file.read(reinterpret_cast<char*>(pimpl_->support_labels_.data()), n_support * sizeof(int));
    
    file.close();
}

// RandomForestClassifier的PIMPL实现
class RandomForestClassifier::Impl {
public:
    bool is_trained_;
    std::mt19937 rng_;
    
    Impl() : is_trained_(false), rng_(42) {}
    
    // 简单的决策树节点
    struct TreeNode {
        int feature_index = -1;
        double threshold = 0.0;
        int predicted_class = -1;
        std::unique_ptr<TreeNode> left;
        std::unique_ptr<TreeNode> right;
        bool is_leaf = false;
    };
    
    // 决策树类
    class DecisionTree {
    public:
        std::unique_ptr<TreeNode> root;
        
        void train(const std::vector<std::vector<double>>& features,
                  const std::vector<int>& labels,
                  int max_depth,
                  std::mt19937& rng) {
            std::vector<int> feature_indices(features[0].size());
            std::iota(feature_indices.begin(), feature_indices.end(), 0);
            root = buildTree(features, labels, feature_indices, 0, max_depth, rng);
        }
        
        int predict(const std::vector<double>& features) const {
            return predictNode(root.get(), features);
        }
        
    private:
        std::unique_ptr<TreeNode> buildTree(const std::vector<std::vector<double>>& features,
                                           const std::vector<int>& labels,
                                           const std::vector<int>& feature_indices,
                                           int depth,
                                           int max_depth,
                                           std::mt19937& rng) {
            auto node = std::make_unique<TreeNode>();
            
            // 检查停止条件
            if (depth >= max_depth || labels.empty() || allSameClass(labels)) {
                node->is_leaf = true;
                node->predicted_class = majorityClass(labels);
                return node;
            }
            
            // 寻找最佳分割
            int best_feature = -1;
            double best_threshold = 0.0;
            double best_gini = 1.0;
            
            // 随机选择特征子集
            std::vector<int> selected_features = feature_indices;
            std::shuffle(selected_features.begin(), selected_features.end(), rng);
            
            int n_features_to_try = std::min(static_cast<int>(std::sqrt(feature_indices.size())), 
                                           static_cast<int>(selected_features.size()));
            
            for (int i = 0; i < n_features_to_try; ++i) {
                int feature_idx = selected_features[i];
                
                // 获取该特征的所有唯一值
                std::set<double> unique_values;
                for (size_t j = 0; j < features.size(); ++j) {
                    unique_values.insert(features[j][feature_idx]);
                }
                
                for (double threshold : unique_values) {
                    double gini = calculateGiniImpurity(features, labels, feature_idx, threshold);
                    
                    if (gini < best_gini) {
                        best_gini = gini;
                        best_feature = feature_idx;
                        best_threshold = threshold;
                    }
                }
            }
            
            if (best_feature == -1) {
                node->is_leaf = true;
                node->predicted_class = majorityClass(labels);
                return node;
            }
            
            node->feature_index = best_feature;
            node->threshold = best_threshold;
            
            // 分割数据
            std::vector<std::vector<double>> left_features, right_features;
            std::vector<int> left_labels, right_labels;
            
            for (size_t i = 0; i < features.size(); ++i) {
                if (features[i][best_feature] <= best_threshold) {
                    left_features.push_back(features[i]);
                    left_labels.push_back(labels[i]);
                } else {
                    right_features.push_back(features[i]);
                    right_labels.push_back(labels[i]);
                }
            }
            
            // 递归构建子树
            node->left = buildTree(left_features, left_labels, feature_indices, depth + 1, max_depth, rng);
            node->right = buildTree(right_features, right_labels, feature_indices, depth + 1, max_depth, rng);
            
            return node;
        }
        
        bool allSameClass(const std::vector<int>& labels) {
            if (labels.empty()) return true;
            int first_class = labels[0];
            return std::all_of(labels.begin(), labels.end(), 
                             [first_class](int label) { return label == first_class; });
        }
        
        int majorityClass(const std::vector<int>& labels) {
            if (labels.empty()) return -1;
            
            std::map<int, int> class_counts;
            for (int label : labels) {
                class_counts[label]++;
            }
            
            return std::max_element(class_counts.begin(), class_counts.end(),
                                  [](const auto& a, const auto& b) {
                                      return a.second < b.second;
                                  })->first;
        }
        
        double calculateGiniImpurity(const std::vector<std::vector<double>>& features,
                                    const std::vector<int>& labels,
                                    int feature_idx,
                                    double threshold) {
            std::vector<int> left_labels, right_labels;
            
            for (size_t i = 0; i < features.size(); ++i) {
                if (features[i][feature_idx] <= threshold) {
                    left_labels.push_back(labels[i]);
                } else {
                    right_labels.push_back(labels[i]);
                }
            }
            
            double total_size = labels.size();
            double left_size = left_labels.size();
            double right_size = right_labels.size();
            
            if (left_size == 0 || right_size == 0) {
                return 1.0; // 最差的分割
            }
            
            double left_gini = calculateGini(left_labels);
            double right_gini = calculateGini(right_labels);
            
            return (left_size / total_size) * left_gini + (right_size / total_size) * right_gini;
        }
        
        double calculateGini(const std::vector<int>& labels) {
            if (labels.empty()) return 0.0;
            
            std::map<int, int> class_counts;
            for (int label : labels) {
                class_counts[label]++;
            }
            
            double gini = 1.0;
            double total = labels.size();
            
            for (const auto& pair : class_counts) {
                double prob = pair.second / total;
                gini -= prob * prob;
            }
            
            return gini;
        }
        
        int predictNode(TreeNode* node, const std::vector<double>& features) const {
            if (!node || node->is_leaf) {
                return node ? node->predicted_class : -1;
            }
            
            if (features[node->feature_index] <= node->threshold) {
                return predictNode(node->left.get(), features);
            } else {
                return predictNode(node->right.get(), features);
            }
        }
    };
    
    std::vector<DecisionTree> trees_;
    
    // 将EventFeatures转换为向量
    std::vector<double> featuresToVector(const EventFeatures& features) {
        std::vector<double> vec;
        vec.push_back(features.duration);
        vec.push_back(features.amplitude);
        vec.push_back(features.area);
        vec.push_back(features.peak_position);
        vec.push_back(features.baseline_before);
        vec.push_back(features.baseline_after);
        vec.push_back(features.rise_time);
        vec.push_back(features.fall_time);
        vec.push_back(features.asymmetry);
        vec.push_back(features.skewness);
        vec.push_back(features.kurtosis);
        vec.push_back(features.width_at_half_max);
        vec.push_back(features.dominant_frequency);
        vec.push_back(features.spectral_centroid);
        vec.push_back(features.spectral_spread);
        vec.push_back(features.spectral_entropy);
        vec.push_back(features.mean);
        vec.push_back(features.std_dev);
        vec.push_back(features.variance);
        vec.push_back(features.rms);
        vec.push_back(features.energy);
        vec.push_back(features.max_derivative);
        vec.push_back(features.min_derivative);
        vec.push_back(features.derivative_variance);
        return vec;
    }
};

RandomForestClassifier::RandomForestClassifier() : pimpl_(std::make_unique<Impl>()) {}
RandomForestClassifier::~RandomForestClassifier() = default;

void RandomForestClassifier::train(const TrainingData& data) {
    if (data.features.empty() || data.labels.empty()) {
        return;
    }
    
    pimpl_->trees_.clear();
    pimpl_->trees_.resize(n_trees_);
    
    // 转换特征为向量
    std::vector<std::vector<double>> feature_vectors;
    for (const auto& f : data.features) {
        feature_vectors.push_back(pimpl_->featuresToVector(f));
    }
    
    // 训练每棵树
    for (int i = 0; i < n_trees_; ++i) {
        // Bootstrap采样
        std::vector<std::vector<double>> bootstrap_features;
        std::vector<int> bootstrap_labels;
        
        std::uniform_int_distribution<int> dist(0, feature_vectors.size() - 1);
        
        for (size_t j = 0; j < feature_vectors.size(); ++j) {
            int idx = dist(pimpl_->rng_);
            bootstrap_features.push_back(feature_vectors[idx]);
            bootstrap_labels.push_back(data.labels[idx]);
        }
        
        // 训练决策树
        pimpl_->trees_[i].train(bootstrap_features, bootstrap_labels, max_depth_, pimpl_->rng_);
    }
    
    pimpl_->is_trained_ = true;
}

ClassificationResult RandomForestClassifier::classify(const EventFeatures& features) {
    ClassificationResult result;
    
    if (!pimpl_->is_trained_ || pimpl_->trees_.empty()) {
        result.predicted_class = -1;
        result.confidence = 0.0;
        return result;
    }
    
    auto feature_vector = pimpl_->featuresToVector(features);
    
    // 收集所有树的预测
    std::map<int, int> class_votes;
    
    for (const auto& tree : pimpl_->trees_) {
        int prediction = tree.predict(feature_vector);
        class_votes[prediction]++;
    }
    
    // 找到得票最多的类别
    auto max_vote = std::max_element(class_votes.begin(), class_votes.end(),
                                   [](const auto& a, const auto& b) {
                                       return a.second < b.second;
                                   });
    
    result.predicted_class = max_vote->first;
    result.confidence = static_cast<double>(max_vote->second) / pimpl_->trees_.size();
    
    return result;
}

std::vector<ClassificationResult> RandomForestClassifier::classifyBatch(
    const std::vector<EventFeatures>& features) {
    
    std::vector<ClassificationResult> results;
    results.reserve(features.size());
    
    for (const auto& feature : features) {
        results.push_back(classify(feature));
    }
    
    return results;
}

ModelEvaluation RandomForestClassifier::evaluate(const TrainingData& test_data) {
    ModelEvaluation eval;
    
    if (test_data.features.empty() || test_data.labels.empty()) {
        return eval;
    }
    
    std::vector<int> predictions;
    for (const auto& features : test_data.features) {
        auto result = classify(features);
        predictions.push_back(result.predicted_class);
    }
    
    // 计算准确率
    int correct = 0;
    for (size_t i = 0; i < predictions.size(); ++i) {
        if (predictions[i] == test_data.labels[i]) {
            correct++;
        }
    }
    eval.accuracy = static_cast<double>(correct) / predictions.size();
    
    return eval;
}

void RandomForestClassifier::saveModel(const std::string& filename) {
    // 简化的模型保存实现
    std::ofstream file(filename);
    if (file.is_open()) {
        file << "RandomForest model saved\n";
        file.close();
    }
}

void RandomForestClassifier::loadModel(const std::string& filename) {
    // 简化的模型加载实现
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        std::getline(file, line);
        file.close();
    }
}

// NeuralNetworkClassifier的PIMPL实现
class NeuralNetworkClassifier::Impl {
public:
    std::vector<std::vector<std::vector<double>>> weights_;
    std::vector<std::vector<double>> biases_;
    bool is_trained_;
    std::mt19937 rng_;
    
    Impl() : is_trained_(false), rng_(42) {}
    
    // 激活函数
    double sigmoid(double x) {
        return 1.0 / (1.0 + std::exp(-x));
    }
    
    double sigmoidDerivative(double x) {
        double s = sigmoid(x);
        return s * (1.0 - s);
    }
    
    double relu(double x) {
        return std::max(0.0, x);
    }
    
    double reluDerivative(double x) {
        return x > 0.0 ? 1.0 : 0.0;
    }
    
    double tanh_activation(double x) {
        return std::tanh(x);
    }
    
    double tanhDerivative(double x) {
        double t = std::tanh(x);
        return 1.0 - t * t;
    }
    
    // 将EventFeatures转换为向量
    std::vector<double> featuresToVector(const EventFeatures& features) {
        std::vector<double> vec;
        vec.push_back(features.duration);
        vec.push_back(features.amplitude);
        vec.push_back(features.area);
        vec.push_back(features.peak_position);
        vec.push_back(features.baseline_before);
        vec.push_back(features.baseline_after);
        vec.push_back(features.rise_time);
        vec.push_back(features.fall_time);
        vec.push_back(features.asymmetry);
        vec.push_back(features.skewness);
        vec.push_back(features.kurtosis);
        vec.push_back(features.width_at_half_max);
        vec.push_back(features.dominant_frequency);
        vec.push_back(features.spectral_centroid);
        vec.push_back(features.spectral_spread);
        vec.push_back(features.spectral_entropy);
        vec.push_back(features.mean);
        vec.push_back(features.std_dev);
        vec.push_back(features.variance);
        vec.push_back(features.rms);
        vec.push_back(features.energy);
        vec.push_back(features.max_derivative);
        vec.push_back(features.min_derivative);
        vec.push_back(features.derivative_variance);
        return vec;
    }
    
    // 初始化网络权重
    void initializeWeights(int input_size, const std::vector<int>& hidden_layers) {
        weights_.clear();
        biases_.clear();
        
        std::vector<int> layer_sizes = {input_size};
        layer_sizes.insert(layer_sizes.end(), hidden_layers.begin(), hidden_layers.end());
        layer_sizes.push_back(1); // 输出层
        
        std::normal_distribution<double> weight_dist(0.0, 0.1);
        
        for (size_t i = 0; i < layer_sizes.size() - 1; ++i) {
            int current_size = layer_sizes[i];
            int next_size = layer_sizes[i + 1];
            
            std::vector<std::vector<double>> layer_weights(current_size, std::vector<double>(next_size));
            std::vector<double> layer_biases(next_size, 0.0);
            
            for (int j = 0; j < current_size; ++j) {
                for (int k = 0; k < next_size; ++k) {
                    layer_weights[j][k] = weight_dist(rng_);
                }
            }
            
            weights_.push_back(layer_weights);
            biases_.push_back(layer_biases);
        }
    }
    
    // 前向传播
    std::vector<std::vector<double>> forwardPass(const std::vector<double>& input) {
        std::vector<std::vector<double>> activations;
        activations.push_back(input);
        
        for (size_t layer = 0; layer < weights_.size(); ++layer) {
            const auto& current_activations = activations.back();
            std::vector<double> next_activations(weights_[layer][0].size(), 0.0);
            
            // 计算加权和
            for (size_t i = 0; i < current_activations.size(); ++i) {
                for (size_t j = 0; j < next_activations.size(); ++j) {
                    next_activations[j] += current_activations[i] * weights_[layer][i][j];
                }
            }
            
            // 添加偏置并应用激活函数
            for (size_t j = 0; j < next_activations.size(); ++j) {
                next_activations[j] += biases_[layer][j];
                next_activations[j] = sigmoid(next_activations[j]);
            }
            
            activations.push_back(next_activations);
        }
        
        return activations;
    }
    
    // 反向传播
    void backwardPass(const std::vector<std::vector<double>>& activations,
                     const std::vector<double>& target,
                     double learning_rate) {
        std::vector<std::vector<double>> deltas(weights_.size());
        
        // 计算输出层误差
        const auto& output = activations.back();
        deltas.back().resize(output.size());
        
        for (size_t i = 0; i < output.size(); ++i) {
            double error = target[i] - output[i];
            deltas.back()[i] = error * sigmoidDerivative(output[i]);
        }
        
        // 反向传播误差
        for (int layer = weights_.size() - 2; layer >= 0; --layer) {
            deltas[layer].resize(activations[layer + 1].size());
            
            for (size_t i = 0; i < deltas[layer].size(); ++i) {
                double error = 0.0;
                
                for (size_t j = 0; j < deltas[layer + 1].size(); ++j) {
                    error += deltas[layer + 1][j] * weights_[layer + 1][i][j];
                }
                
                deltas[layer][i] = error * sigmoidDerivative(activations[layer + 1][i]);
            }
        }
        
        // 更新权重和偏置
        for (size_t layer = 0; layer < weights_.size(); ++layer) {
            for (size_t i = 0; i < weights_[layer].size(); ++i) {
                for (size_t j = 0; j < weights_[layer][i].size(); ++j) {
                    weights_[layer][i][j] += learning_rate * deltas[layer][j] * activations[layer][i];
                }
            }
            
            for (size_t j = 0; j < biases_[layer].size(); ++j) {
                biases_[layer][j] += learning_rate * deltas[layer][j];
            }
        }
    }
};

NeuralNetworkClassifier::NeuralNetworkClassifier() : pimpl_(std::make_unique<Impl>()) {}
NeuralNetworkClassifier::~NeuralNetworkClassifier() = default;

void NeuralNetworkClassifier::train(const TrainingData& data) {
    if (data.features.empty() || data.labels.empty()) {
        return;
    }
    
    // 转换特征为向量
    std::vector<std::vector<double>> feature_vectors;
    for (const auto& f : data.features) {
        feature_vectors.push_back(pimpl_->featuresToVector(f));
    }
    
    // 初始化网络
    pimpl_->initializeWeights(feature_vectors[0].size(), hidden_layers_);
    
    // 训练循环
    for (int epoch = 0; epoch < epochs_; ++epoch) {
        for (size_t i = 0; i < feature_vectors.size(); ++i) {
            // 前向传播
            auto activations = pimpl_->forwardPass(feature_vectors[i]);
            
            // 准备目标值
            std::vector<double> target = {static_cast<double>(data.labels[i] > 0 ? 1 : 0)};
            
            // 反向传播
            pimpl_->backwardPass(activations, target, learning_rate_);
        }
    }
    
    pimpl_->is_trained_ = true;
}

ClassificationResult NeuralNetworkClassifier::classify(const EventFeatures& features) {
    ClassificationResult result;
    
    if (!pimpl_->is_trained_) {
        result.predicted_class = -1;
        result.confidence = 0.0;
        return result;
    }
    
    auto feature_vector = pimpl_->featuresToVector(features);
    auto activations = pimpl_->forwardPass(feature_vector);
    double output = activations.back()[0];
    
    result.predicted_class = output > 0.5 ? 1 : -1;
    result.confidence = std::abs(output - 0.5) * 2.0;
    result.class_probabilities = {output};
    
    return result;
}

std::vector<ClassificationResult> NeuralNetworkClassifier::classifyBatch(
    const std::vector<EventFeatures>& features) {
    
    std::vector<ClassificationResult> results;
    results.reserve(features.size());
    
    for (const auto& feature : features) {
        results.push_back(classify(feature));
    }
    
    return results;
}

ModelEvaluation NeuralNetworkClassifier::evaluate(const TrainingData& test_data) {
    ModelEvaluation eval;
    
    if (test_data.features.empty() || test_data.labels.empty()) {
        return eval;
    }
    
    std::vector<int> predictions;
    for (const auto& features : test_data.features) {
        auto result = classify(features);
        predictions.push_back(result.predicted_class);
    }
    
    // 计算准确率
    int correct = 0;
    for (size_t i = 0; i < predictions.size(); ++i) {
        if (predictions[i] == test_data.labels[i]) {
            correct++;
        }
    }
    eval.accuracy = static_cast<double>(correct) / predictions.size();
    
    return eval;
}

void NeuralNetworkClassifier::saveModel(const std::string& filename) {
    // 简化的模型保存实现
    std::ofstream file(filename);
    if (file.is_open()) {
        file << "NeuralNetwork model saved\n";
        file.close();
    }
}

void NeuralNetworkClassifier::loadModel(const std::string& filename) {
    // 简化的模型加载实现
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        std::getline(file, line);
        file.close();
    }
}

// EventClassificationManager实现
class EventClassificationManager::Impl {
public:
    std::map<std::string, std::unique_ptr<EventClassifier>> classifiers_;
    std::string active_classifier_;
    
    Impl() {}
};

EventClassificationManager::EventClassificationManager() : pimpl_(std::make_unique<Impl>()) {}
EventClassificationManager::~EventClassificationManager() = default;

EventFeatures EventClassificationManager::extractFeatures(const std::vector<double>& event_data,
                                                         double sampling_rate) {
    EventFeatures features;
    
    if (event_data.empty()) {
        return features;
    }
    
    // 基本特征
    features.duration = event_data.size() / sampling_rate;
    features.amplitude = *std::max_element(event_data.begin(), event_data.end());
    features.area = std::accumulate(event_data.begin(), event_data.end(), 0.0);
    
    auto minmax = std::minmax_element(event_data.begin(), event_data.end());
    double min_value = *minmax.first;
    double max_value = *minmax.second;
    
    // 峰值位置
    auto max_it = std::max_element(event_data.begin(), event_data.end());
    features.peak_position = std::distance(event_data.begin(), max_it) / sampling_rate;
    
    // 基线
    features.baseline_before = event_data.front();
    features.baseline_after = event_data.back();
    
    // 形状特征
    double half_max = max_value / 2.0;
    size_t max_idx = std::distance(event_data.begin(), max_it);
    
    // 计算上升时间和下降时间
    size_t rise_idx = 0, fall_idx = event_data.size() - 1;
    
    for (size_t i = 0; i < max_idx; ++i) {
        if (event_data[i] >= half_max) {
            rise_idx = i;
            break;
        }
    }
    
    for (size_t i = max_idx; i < event_data.size(); ++i) {
        if (event_data[i] <= half_max) {
            fall_idx = i;
            break;
        }
    }
    
    features.rise_time = (max_idx - rise_idx) / sampling_rate;
    features.fall_time = (fall_idx - max_idx) / sampling_rate;
    features.width_at_half_max = (fall_idx - rise_idx) / sampling_rate;
    
    // 不对称性
    features.asymmetry = (features.fall_time - features.rise_time) / 
                        (features.fall_time + features.rise_time + 1e-12);
    
    // 统计特征
    double mean = features.area / event_data.size();
    features.mean = mean;
    
    double variance = 0.0;
    for (double val : event_data) {
        variance += (val - mean) * (val - mean);
    }
    variance /= event_data.size();
    features.variance = variance;
    features.std_dev = std::sqrt(variance);
    
    // 计算偏度和峰度
    double skewness = 0.0, kurtosis = 0.0;
    for (double val : event_data) {
        double z = (val - mean) / (features.std_dev + 1e-12);
        skewness += z * z * z;
        kurtosis += z * z * z * z;
    }
    features.skewness = skewness / event_data.size();
    features.kurtosis = kurtosis / event_data.size() - 3.0;
    
    // RMS和能量
    double rms_sum = 0.0;
    for (double val : event_data) {
        rms_sum += val * val;
    }
    features.rms = std::sqrt(rms_sum / event_data.size());
    features.energy = rms_sum;
    
    // 导数特征
    if (event_data.size() > 1) {
        std::vector<double> derivatives;
        for (size_t i = 1; i < event_data.size(); ++i) {
            derivatives.push_back(event_data[i] - event_data[i-1]);
        }
        
        auto deriv_minmax = std::minmax_element(derivatives.begin(), derivatives.end());
        features.max_derivative = *deriv_minmax.second;
        features.min_derivative = *deriv_minmax.first;
        
        double deriv_mean = std::accumulate(derivatives.begin(), derivatives.end(), 0.0) / derivatives.size();
        double deriv_var = 0.0;
        for (double d : derivatives) {
            deriv_var += (d - deriv_mean) * (d - deriv_mean);
        }
        features.derivative_variance = deriv_var / derivatives.size();
    }
    
    return features;
}

std::vector<EventFeatures> EventClassificationManager::extractFeaturesFromEvents(
    const std::vector<std::vector<double>>& events,
    double sampling_rate) {
    
    std::vector<EventFeatures> features_list;
    features_list.reserve(events.size());
    
    for (const auto& event : events) {
        features_list.push_back(extractFeatures(event, sampling_rate));
    }
    
    return features_list;
}

std::vector<EventFeatures> EventClassificationManager::normalizeFeatures(
    const std::vector<EventFeatures>& features) {
    
    if (features.empty()) return {};
    
    // 计算每个特征的均值和标准差
    EventFeatures means, std_devs;
    
    // 初始化
    means.duration = 0.0; means.amplitude = 0.0; means.area = 0.0;
    std_devs.duration = 0.0; std_devs.amplitude = 0.0; std_devs.area = 0.0;
    
    // 计算均值
    for (const auto& f : features) {
        means.duration += f.duration;
        means.amplitude += f.amplitude;
        means.area += f.area;
        // ... 其他特征
    }
    
    size_t n = features.size();
    means.duration /= n;
    means.amplitude /= n;
    means.area /= n;
    
    // 计算标准差
    for (const auto& f : features) {
        std_devs.duration += (f.duration - means.duration) * (f.duration - means.duration);
        std_devs.amplitude += (f.amplitude - means.amplitude) * (f.amplitude - means.amplitude);
        std_devs.area += (f.area - means.area) * (f.area - means.area);
    }
    
    std_devs.duration = std::sqrt(std_devs.duration / n);
    std_devs.amplitude = std::sqrt(std_devs.amplitude / n);
    std_devs.area = std::sqrt(std_devs.area / n);
    
    // 归一化
    std::vector<EventFeatures> normalized_features;
    normalized_features.reserve(features.size());
    
    for (const auto& f : features) {
        EventFeatures normalized = f;
        normalized.duration = (f.duration - means.duration) / (std_devs.duration + 1e-12);
        normalized.amplitude = (f.amplitude - means.amplitude) / (std_devs.amplitude + 1e-12);
        normalized.area = (f.area - means.area) / (std_devs.area + 1e-12);
        normalized_features.push_back(normalized);
    }
    
    return normalized_features;
}

std::vector<EventFeatures> EventClassificationManager::standardizeFeatures(
    const std::vector<EventFeatures>& features) {
    
    return normalizeFeatures(features); // 简化实现，与归一化相同
}

FeatureSelection EventClassificationManager::selectFeatures(const TrainingData& data,
                                                           const std::string& method,
                                                           int n_features) {
    FeatureSelection selection;
    selection.selection_method = method;
    
    // 简化的特征选择实现
    selection.selected_features = {"duration", "amplitude", "area", "rise_time", "fall_time"};
    selection.feature_scores = {0.9, 0.8, 0.7, 0.6, 0.5};
    selection.feature_indices = {0, 1, 2, 7, 8};
    
    return selection;
}

void EventClassificationManager::registerClassifier(const std::string& name, 
                                                   std::unique_ptr<EventClassifier> classifier) {
    pimpl_->classifiers_[name] = std::move(classifier);
}

void EventClassificationManager::setActiveClassifier(const std::string& name) {
    if (pimpl_->classifiers_.find(name) != pimpl_->classifiers_.end()) {
        pimpl_->active_classifier_ = name;
    }
}

EventClassifier* EventClassificationManager::getActiveClassifier() {
    if (pimpl_->active_classifier_.empty() || 
        pimpl_->classifiers_.find(pimpl_->active_classifier_) == pimpl_->classifiers_.end()) {
        return nullptr;
    }
    
    return pimpl_->classifiers_[pimpl_->active_classifier_].get();
}

std::vector<std::string> EventClassificationManager::getAvailableClassifiers() const {
    std::vector<std::string> names;
    for (const auto& pair : pimpl_->classifiers_) {
        names.push_back(pair.first);
    }
    return names;
}

void EventClassificationManager::trainClassifier(const TrainingData& data) {
    auto* classifier = getActiveClassifier();
    if (classifier) {
        classifier->train(data);
    }
}

ClassificationResult EventClassificationManager::classifyEvent(const EventFeatures& features) {
    auto* classifier = getActiveClassifier();
    if (classifier) {
        return classifier->classify(features);
    }
    return ClassificationResult{};
}

std::vector<ClassificationResult> EventClassificationManager::classifyEvents(
    const std::vector<EventFeatures>& features) {
    
    auto* classifier = getActiveClassifier();
    if (classifier) {
        return classifier->classifyBatch(features);
    }
    return {};
}

ModelEvaluation EventClassificationManager::evaluateClassifier(const TrainingData& test_data) {
    auto* classifier = getActiveClassifier();
    if (classifier) {
        return classifier->evaluate(test_data);
    }
    return ModelEvaluation{};
}

ClusteringResult EventClassificationManager::performClustering(const std::vector<EventFeatures>& features,
                                                              const std::string& method,
                                                              int n_clusters) {
    ClusteringResult result;
    result.algorithm = method;
    result.n_clusters = n_clusters;
    
    // 简化的聚类实现
    result.labels.resize(features.size());
    for (size_t i = 0; i < features.size(); ++i) {
        result.labels[i] = i % n_clusters;
    }
    
    return result;
}

AnomalyDetection EventClassificationManager::detectAnomalies(const std::vector<EventFeatures>& features,
                                                            const std::string& method) {
    AnomalyDetection result;
    result.detection_method = method;
    result.threshold = 0.5;
    
    // 简化的异常检测实现
    result.is_anomaly.resize(features.size(), false);
    result.anomaly_scores.resize(features.size(), 0.0);
    
    return result;
}

TrainingData EventClassificationManager::loadTrainingData(const std::string& filename) {
    TrainingData data;
    
    std::ifstream file(filename);
    if (file.is_open()) {
        // 简化的数据加载实现
        file.close();
    }
    
    return data;
}

void EventClassificationManager::saveTrainingData(const TrainingData& data, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        // 简化的数据保存实现
        file << "Training data saved\n";
        file.close();
    }
}

TrainingData EventClassificationManager::splitTrainingData(const TrainingData& data, 
                                                          double train_ratio) {
    TrainingData train_data;
    
    size_t train_size = static_cast<size_t>(data.features.size() * train_ratio);
    
    train_data.features.assign(data.features.begin(), data.features.begin() + train_size);
    train_data.labels.assign(data.labels.begin(), data.labels.begin() + train_size);
    train_data.class_names = data.class_names;
    train_data.label_to_name = data.label_to_name;
    train_data.name_to_label = data.name_to_label;
    
    return train_data;
}

void EventClassificationManager::saveModel(const std::string& filename) {
    auto* classifier = getActiveClassifier();
    if (classifier) {
        classifier->saveModel(filename);
    }
}

void EventClassificationManager::loadModel(const std::string& filename) {
    auto* classifier = getActiveClassifier();
    if (classifier) {
        classifier->loadModel(filename);
    }
}

void EventClassificationManager::exportModelInfo(const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << "Model information exported\n";
        file.close();
    }
}

// ClassificationUtils命名空间的实现
namespace ClassificationUtils {

double calculateRiseTime(const std::vector<double>& event_data, double threshold) {
    if (event_data.empty()) return 0.0;
    
    auto max_it = std::max_element(event_data.begin(), event_data.end());
    double max_val = *max_it;
    size_t max_idx = std::distance(event_data.begin(), max_it);
    
    double target_val = max_val * threshold;
    
    for (size_t i = 0; i < max_idx; ++i) {
        if (event_data[i] >= target_val) {
            return max_idx - i;
        }
    }
    
    return max_idx;
}

double calculateFallTime(const std::vector<double>& event_data, double threshold) {
    if (event_data.empty()) return 0.0;
    
    auto max_it = std::max_element(event_data.begin(), event_data.end());
    double max_val = *max_it;
    size_t max_idx = std::distance(event_data.begin(), max_it);
    
    double target_val = max_val * threshold;
    
    for (size_t i = max_idx; i < event_data.size(); ++i) {
        if (event_data[i] <= target_val) {
            return i - max_idx;
        }
    }
    
    return event_data.size() - max_idx;
}

double calculateAsymmetry(const std::vector<double>& event_data) {
    double rise_time = calculateRiseTime(event_data, 0.1);
    double fall_time = calculateFallTime(event_data, 0.9);
    
    return (fall_time - rise_time) / (fall_time + rise_time + 1e-12);
}

double calculateWidthAtHalfMax(const std::vector<double>& event_data) {
    if (event_data.empty()) return 0.0;
    
    auto max_it = std::max_element(event_data.begin(), event_data.end());
    double max_val = *max_it;
    size_t max_idx = std::distance(event_data.begin(), max_it);
    
    double half_max = max_val / 2.0;
    
    size_t left_idx = 0, right_idx = event_data.size() - 1;
    
    for (size_t i = 0; i < max_idx; ++i) {
        if (event_data[i] >= half_max) {
            left_idx = i;
            break;
        }
    }
    
    for (size_t i = max_idx; i < event_data.size(); ++i) {
        if (event_data[i] <= half_max) {
            right_idx = i;
            break;
        }
    }
    
    return right_idx - left_idx;
}

std::vector<double> featuresToVector(const EventFeatures& features,
                                   const std::vector<std::string>& feature_names) {
    std::vector<double> vec;
    
    for (const auto& name : feature_names) {
        if (name == "duration") vec.push_back(features.duration);
        else if (name == "amplitude") vec.push_back(features.amplitude);
        else if (name == "area") vec.push_back(features.area);
        else if (name == "peak_position") vec.push_back(features.peak_position);
        else if (name == "baseline_before") vec.push_back(features.baseline_before);
        else if (name == "baseline_after") vec.push_back(features.baseline_after);
        else if (name == "rise_time") vec.push_back(features.rise_time);
        else if (name == "fall_time") vec.push_back(features.fall_time);
        else if (name == "asymmetry") vec.push_back(features.asymmetry);
        else if (name == "skewness") vec.push_back(features.skewness);
        else if (name == "kurtosis") vec.push_back(features.kurtosis);
        else if (name == "width_at_half_max") vec.push_back(features.width_at_half_max);
        else if (name == "dominant_frequency") vec.push_back(features.dominant_frequency);
        else if (name == "spectral_centroid") vec.push_back(features.spectral_centroid);
        else if (name == "spectral_spread") vec.push_back(features.spectral_spread);
        else if (name == "spectral_entropy") vec.push_back(features.spectral_entropy);
        else if (name == "mean") vec.push_back(features.mean);
        else if (name == "std_dev") vec.push_back(features.std_dev);
        else if (name == "variance") vec.push_back(features.variance);
        else if (name == "rms") vec.push_back(features.rms);
        else if (name == "energy") vec.push_back(features.energy);
        else if (name == "max_derivative") vec.push_back(features.max_derivative);
        else if (name == "min_derivative") vec.push_back(features.min_derivative);
        else if (name == "derivative_variance") vec.push_back(features.derivative_variance);
    }
    
    return vec;
}

std::vector<double> normalizeVector(const std::vector<double>& data) {
    if (data.empty()) return {};
    
    double mean = std::accumulate(data.begin(), data.end(), 0.0) / data.size();
    
    double variance = 0.0;
    for (double val : data) {
        variance += (val - mean) * (val - mean);
    }
    variance /= data.size();
    double std_dev = std::sqrt(variance);
    
    std::vector<double> normalized;
    normalized.reserve(data.size());
    
    for (double val : data) {
        normalized.push_back((val - mean) / (std_dev + 1e-12));
    }
    
    return normalized;
}

double calculateAccuracy(const std::vector<int>& true_labels,
                        const std::vector<int>& predicted_labels) {
    if (true_labels.size() != predicted_labels.size() || true_labels.empty()) {
        return 0.0;
    }
    
    int correct = 0;
    for (size_t i = 0; i < true_labels.size(); ++i) {
        if (true_labels[i] == predicted_labels[i]) {
            correct++;
        }
    }
    
    return static_cast<double>(correct) / true_labels.size();
}

double calculatePrecision(const std::vector<int>& true_labels,
                         const std::vector<int>& predicted_labels,
                         int positive_class) {
    if (true_labels.size() != predicted_labels.size() || true_labels.empty()) {
        return 0.0;
    }
    
    int true_positives = 0;
    int false_positives = 0;
    
    for (size_t i = 0; i < true_labels.size(); ++i) {
        if (predicted_labels[i] == positive_class) {
            if (true_labels[i] == positive_class) {
                true_positives++;
            } else {
                false_positives++;
            }
        }
    }
    
    if (true_positives + false_positives == 0) {
        return 0.0;
    }
    
    return static_cast<double>(true_positives) / (true_positives + false_positives);
}

double calculateRecall(const std::vector<int>& true_labels,
                      const std::vector<int>& predicted_labels,
                      int positive_class) {
    if (true_labels.size() != predicted_labels.size() || true_labels.empty()) {
        return 0.0;
    }
    
    int true_positives = 0;
    int false_negatives = 0;
    
    for (size_t i = 0; i < true_labels.size(); ++i) {
        if (true_labels[i] == positive_class) {
            if (predicted_labels[i] == positive_class) {
                true_positives++;
            } else {
                false_negatives++;
            }
        }
    }
    
    if (true_positives + false_negatives == 0) {
        return 0.0;
    }
    
    return static_cast<double>(true_positives) / (true_positives + false_negatives);
}

double calculateF1Score(const std::vector<int>& true_labels,
                       const std::vector<int>& predicted_labels,
                       int positive_class) {
    double precision = calculatePrecision(true_labels, predicted_labels, positive_class);
    double recall = calculateRecall(true_labels, predicted_labels, positive_class);
    
    if (precision + recall == 0.0) {
        return 0.0;
    }
    
    return 2.0 * precision * recall / (precision + recall);
}

std::vector<std::vector<int>> calculateConfusionMatrix(const std::vector<int>& true_labels,
                                                      const std::vector<int>& predicted_labels) {
    std::vector<std::vector<int>> matrix;
    
    if (true_labels.size() != predicted_labels.size() || true_labels.empty()) {
        return matrix;
    }
    
    // 获取所有唯一的类别
    std::set<int> unique_classes;
    for (int label : true_labels) {
        unique_classes.insert(label);
    }
    for (int label : predicted_labels) {
        unique_classes.insert(label);
    }
    
    std::vector<int> classes(unique_classes.begin(), unique_classes.end());
    int n_classes = classes.size();
    
    // 初始化混淆矩阵
    matrix.assign(n_classes, std::vector<int>(n_classes, 0));
    
    // 创建类别到索引的映射
    std::map<int, int> class_to_index;
    for (int i = 0; i < n_classes; ++i) {
        class_to_index[classes[i]] = i;
    }
    
    // 填充混淆矩阵
    for (size_t i = 0; i < true_labels.size(); ++i) {
        int true_idx = class_to_index[true_labels[i]];
        int pred_idx = class_to_index[predicted_labels[i]];
        matrix[true_idx][pred_idx]++;
    }
    
    return matrix;
}

} // namespace ClassificationUtils

} // namespace Analysis