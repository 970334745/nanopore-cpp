#include "ReportGenerator.h"
#include "StatisticalAnalysis.h"
#include "SpectralAnalysis.h"
#include "EventClassification.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <chrono>

namespace Analysis {

// ReportGenerator的PIMPL实现
class ReportGenerator::Impl {
public:
    std::shared_ptr<StatisticalAnalyzer> statistical_analyzer_;
    std::shared_ptr<SpectralAnalyzer> spectral_analyzer_;
    std::shared_ptr<EventClassificationManager> classification_manager_;
    
    ReportData current_report_;
    
    Impl() {
        statistical_analyzer_ = std::make_shared<StatisticalAnalyzer>();
        spectral_analyzer_ = std::make_shared<SpectralAnalyzer>();
        classification_manager_ = std::make_shared<EventClassificationManager>();
    }
    
    std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
    
    std::string formatNumber(double value, int precision = 3) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(precision) << value;
        return ss.str();
    }
    
    std::string generateHTMLHeader(const std::string& title) {
        std::stringstream html;
        html << "<!DOCTYPE html>\n";
        html << "<html lang=\"en\">\n";
        html << "<head>\n";
        html << "    <meta charset=\"UTF-8\">\n";
        html << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
        html << "    <title>" << title << "</title>\n";
        html << "    <style>\n";
        html << "        body { font-family: Arial, sans-serif; margin: 20px; background-color: #f5f5f5; }\n";
        html << "        .container { max-width: 1200px; margin: 0 auto; background-color: white; padding: 20px; border-radius: 8px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }\n";
        html << "        h1 { color: #2c3e50; border-bottom: 3px solid #3498db; padding-bottom: 10px; }\n";
        html << "        h2 { color: #34495e; border-bottom: 1px solid #bdc3c7; padding-bottom: 5px; margin-top: 30px; }\n";
        html << "        h3 { color: #7f8c8d; }\n";
        html << "        .summary-box { background-color: #ecf0f1; padding: 15px; border-radius: 5px; margin: 10px 0; }\n";
        html << "        .metric { display: inline-block; margin: 10px 20px 10px 0; }\n";
        html << "        .metric-label { font-weight: bold; color: #2c3e50; }\n";
        html << "        .metric-value { color: #e74c3c; font-size: 1.1em; }\n";
        html << "        table { border-collapse: collapse; width: 100%; margin: 10px 0; }\n";
        html << "        th, td { border: 1px solid #bdc3c7; padding: 8px; text-align: left; }\n";
        html << "        th { background-color: #3498db; color: white; }\n";
        html << "        tr:nth-child(even) { background-color: #f8f9fa; }\n";
        html << "        .timestamp { color: #7f8c8d; font-size: 0.9em; }\n";
        html << "        .plot-container { text-align: center; margin: 20px 0; }\n";
        html << "        .warning { background-color: #fff3cd; border: 1px solid #ffeaa7; color: #856404; padding: 10px; border-radius: 5px; margin: 10px 0; }\n";
        html << "        .info { background-color: #d1ecf1; border: 1px solid #bee5eb; color: #0c5460; padding: 10px; border-radius: 5px; margin: 10px 0; }\n";
        html << "    </style>\n";
        html << "</head>\n";
        html << "<body>\n";
        html << "<div class=\"container\">\n";
        return html.str();
    }
    
    std::string generateHTMLFooter() {
        std::stringstream html;
        html << "</div>\n";
        html << "</body>\n";
        html << "</html>\n";
        return html.str();
    }
};

ReportGenerator::ReportGenerator() : pimpl_(std::make_unique<Impl>()) {}
ReportGenerator::~ReportGenerator() = default;

void ReportGenerator::createReport(const ReportConfig& config) {
    pimpl_->current_report_.config = config;
    pimpl_->current_report_.sections.clear();
    pimpl_->current_report_.global_metadata.clear();
    pimpl_->current_report_.references.clear();
    pimpl_->current_report_.summary.clear();
    pimpl_->current_report_.conclusion.clear();
}

void ReportGenerator::addSection(const ReportSection& section) {
    pimpl_->current_report_.sections.push_back(section);
}

void ReportGenerator::addTextContent(const std::string& section_title, const TextContent& content) {
    for (auto& section : pimpl_->current_report_.sections) {
        if (section.title == section_title) {
            section.text_contents.push_back(content);
            return;
        }
    }
    
    // 如果没有找到对应的section，创建一个新的
    ReportSection new_section;
    new_section.title = section_title;
    new_section.text_contents.push_back(content);
    pimpl_->current_report_.sections.push_back(new_section);
}

void ReportGenerator::addChart(const std::string& section_title, const ChartData& chart) {
    for (auto& section : pimpl_->current_report_.sections) {
        if (section.title == section_title) {
            section.charts.push_back(chart);
            return;
        }
    }
    
    // 如果没有找到对应的section，创建一个新的
    ReportSection new_section;
    new_section.title = section_title;
    new_section.charts.push_back(chart);
    pimpl_->current_report_.sections.push_back(new_section);
}

void ReportGenerator::addTable(const std::string& section_title, const TableData& table) {
    for (auto& section : pimpl_->current_report_.sections) {
        if (section.title == section_title) {
            section.tables.push_back(table);
            return;
        }
    }
    
    // 如果没有找到对应的section，创建一个新的
    ReportSection new_section;
    new_section.title = section_title;
    new_section.tables.push_back(table);
    pimpl_->current_report_.sections.push_back(new_section);
}

void ReportGenerator::addImage(const std::string& section_title, const ImageContent& image) {
    for (auto& section : pimpl_->current_report_.sections) {
        if (section.title == section_title) {
            section.images.push_back(image);
            return;
        }
    }
    
    // 如果没有找到对应的section，创建一个新的
    ReportSection new_section;
    new_section.title = section_title;
    new_section.images.push_back(image);
    pimpl_->current_report_.sections.push_back(new_section);
}

ReportData ReportGenerator::generateAnalysisReport(const AnalysisSummary& summary) {
    ReportConfig config;
    config.title = "Comprehensive Analysis Report";
    config.author = "Nanopore Analysis System";
    config.creation_time = std::chrono::system_clock::now();
    
    createReport(config);
    
    // 添加概览部分
    ReportSection overview_section;
    overview_section.title = "Analysis Overview";
    overview_section.description = "Summary of the analysis performed on the dataset";
    
    TextContent overview_text;
    overview_text.title = "Dataset Information";
    std::stringstream overview_content;
    overview_content << "Dataset: " << summary.dataset_name << "\n";
    overview_content << "Total Events: " << summary.total_events << "\n";
    overview_content << "Total Data Points: " << summary.total_data_points << "\n";
    overview_content << "Duration: " << pimpl_->formatNumber(summary.duration) << " seconds\n";
    overview_content << "Sampling Rate: " << pimpl_->formatNumber(summary.sampling_rate) << " Hz\n";
    overview_text.content = overview_content.str();
    overview_section.text_contents.push_back(overview_text);
    
    addSection(overview_section);
    
    // 添加统计分析部分
    if (summary.overall_statistics) {
        ReportSection stats_section;
        stats_section.title = "Statistical Analysis";
        stats_section.description = "Basic statistical properties of the data";
        
        TableData stats_table = createStatisticsTable({*summary.overall_statistics}, {"Dataset"});
        stats_section.tables.push_back(stats_table);
        
        addSection(stats_section);
    }
    
    // 添加分类结果部分
    if (!summary.class_distribution.empty()) {
        ReportSection classification_section;
        classification_section.title = "Classification Results";
        classification_section.description = "Event classification distribution and accuracy";
        
        TextContent classification_text;
        classification_text.title = "Classification Summary";
        std::stringstream classification_content;
        classification_content << "Classification Accuracy: " << pimpl_->formatNumber(summary.classification_accuracy * 100) << "%\n\n";
        classification_content << "Class Distribution:\n";
        for (const auto& pair : summary.class_distribution) {
            classification_content << "  " << pair.first << ": " << pair.second << " events\n";
        }
        classification_text.content = classification_content.str();
        classification_section.text_contents.push_back(classification_text);
        
        addSection(classification_section);
    }
    
    // 添加频谱分析部分
    ReportSection spectral_section;
    spectral_section.title = "Spectral Analysis";
    spectral_section.description = "Frequency domain analysis results";
    
    TextContent spectral_text;
    spectral_text.title = "Spectral Properties";
    std::stringstream spectral_content;
    spectral_content << "Dominant Frequency: " << pimpl_->formatNumber(summary.dominant_frequency) << " Hz\n";
    spectral_content << "Bandwidth: " << pimpl_->formatNumber(summary.bandwidth) << " Hz\n";
    spectral_content << "Signal-to-Noise Ratio: " << pimpl_->formatNumber(summary.signal_to_noise_ratio) << " dB\n";
    spectral_text.content = spectral_content.str();
    spectral_section.text_contents.push_back(spectral_text);
    
    addSection(spectral_section);
    
    // 添加数据质量部分
    ReportSection quality_section;
    quality_section.title = "Data Quality Assessment";
    quality_section.description = "Assessment of data quality and recommendations";
    
    TextContent quality_text;
    quality_text.title = "Quality Score";
    std::stringstream quality_content;
    quality_content << "Data Quality Score: " << pimpl_->formatNumber(summary.data_quality_score) << "/100\n\n";
    
    if (!summary.quality_issues.empty()) {
        quality_content << "Quality Issues:\n";
        for (const auto& issue : summary.quality_issues) {
            quality_content << "  - " << issue << "\n";
        }
        quality_content << "\n";
    }
    
    if (!summary.recommendations.empty()) {
        quality_content << "Recommendations:\n";
        for (const auto& recommendation : summary.recommendations) {
            quality_content << "  - " << recommendation << "\n";
        }
    }
    
    quality_text.content = quality_content.str();
    quality_section.text_contents.push_back(quality_text);
    
    addSection(quality_section);
    
    return pimpl_->current_report_;
}

ReportData ReportGenerator::generateStatisticalReport(const std::vector<BasicStatistics>& stats,
                                                    const std::vector<std::string>& data_names) {
    ReportConfig config;
    config.title = "Statistical Analysis Report";
    config.author = "Nanopore Analysis System";
    config.creation_time = std::chrono::system_clock::now();
    
    createReport(config);
    
    ReportSection stats_section;
    stats_section.title = "Statistical Analysis";
    stats_section.description = "Comprehensive statistical analysis of the datasets";
    
    TableData stats_table = createStatisticsTable(stats, data_names);
    stats_section.tables.push_back(stats_table);
    
    addSection(stats_section);
    
    return pimpl_->current_report_;
}

ReportData ReportGenerator::generateSpectralReport(const std::vector<SpectrumResult>& spectra,
                                                  const std::vector<std::string>& signal_names) {
    ReportConfig config;
    config.title = "Spectral Analysis Report";
    config.author = "Nanopore Analysis System";
    config.creation_time = std::chrono::system_clock::now();
    
    createReport(config);
    
    ReportSection spectral_section;
    spectral_section.title = "Spectral Analysis";
    spectral_section.description = "Frequency domain analysis of the signals";
    
    // 为每个频谱创建表格
    for (size_t i = 0; i < spectra.size() && i < signal_names.size(); ++i) {
        const auto& spectrum = spectra[i];
        const auto& name = signal_names[i];
        
        TableData spectrum_table;
        spectrum_table.title = "Spectral Properties - " + name;
        spectrum_table.headers = {"Property", "Value"};
        
        spectrum_table.rows.push_back({"Sampling Rate", pimpl_->formatNumber(spectrum.sampling_rate) + " Hz"});
        spectrum_table.rows.push_back({"Frequency Resolution", pimpl_->formatNumber(spectrum.frequency_resolution) + " Hz"});
        spectrum_table.rows.push_back({"FFT Size", std::to_string(spectrum.fft_size)});
        spectrum_table.rows.push_back({"Frequency Bins", std::to_string(spectrum.frequencies.size())});
        spectrum_table.rows.push_back({"Magnitude Bins", std::to_string(spectrum.magnitude.size())});
        
        spectral_section.tables.push_back(spectrum_table);
    }
    
    addSection(spectral_section);
    
    return pimpl_->current_report_;
}

ReportData ReportGenerator::generateClassificationReport(
    const std::vector<ClassificationResult>& results,
    const std::vector<EventFeatures>& features) {
    
    ReportConfig config;
    config.title = "Event Classification Report";
    config.author = "Nanopore Analysis System";
    config.creation_time = std::chrono::system_clock::now();
    
    createReport(config);
    
    ReportSection classification_section;
    classification_section.title = "Classification Results";
    classification_section.description = "Event classification analysis";
    
    TableData classification_table = createClassificationTable(results);
    classification_section.tables.push_back(classification_table);
    
    // 添加特征统计
    if (!features.empty()) {
        ReportSection features_section;
        features_section.title = "Feature Analysis";
        features_section.description = "Statistical analysis of event features";
        
        TableData features_table;
        features_table.title = "Feature Statistics";
        features_table.headers = {"Feature", "Mean", "Std Dev", "Min", "Max"};
        
        // 计算特征统计
        std::vector<double> durations, amplitudes, areas, rise_times, fall_times;
        for (const auto& feature : features) {
            durations.push_back(feature.duration);
            amplitudes.push_back(feature.amplitude);
            areas.push_back(feature.area);
            rise_times.push_back(feature.rise_time);
            fall_times.push_back(feature.fall_time);
        }
        
        auto duration_stats = pimpl_->statistical_analyzer_->calculateBasicStatistics(durations);
        auto amplitude_stats = pimpl_->statistical_analyzer_->calculateBasicStatistics(amplitudes);
        auto area_stats = pimpl_->statistical_analyzer_->calculateBasicStatistics(areas);
        auto rise_time_stats = pimpl_->statistical_analyzer_->calculateBasicStatistics(rise_times);
        auto fall_time_stats = pimpl_->statistical_analyzer_->calculateBasicStatistics(fall_times);
        
        features_table.rows.push_back({"Duration", 
            pimpl_->formatNumber(duration_stats.mean),
            pimpl_->formatNumber(duration_stats.std_dev),
            pimpl_->formatNumber(duration_stats.min),
            pimpl_->formatNumber(duration_stats.max)});
        
        features_table.rows.push_back({"Amplitude", 
            pimpl_->formatNumber(amplitude_stats.mean),
            pimpl_->formatNumber(amplitude_stats.std_dev),
            pimpl_->formatNumber(amplitude_stats.min),
            pimpl_->formatNumber(amplitude_stats.max)});
        
        features_table.rows.push_back({"Area", 
            pimpl_->formatNumber(area_stats.mean),
            pimpl_->formatNumber(area_stats.std_dev),
            pimpl_->formatNumber(area_stats.min),
            pimpl_->formatNumber(area_stats.max)});
        
        features_table.rows.push_back({"Rise Time", 
            pimpl_->formatNumber(rise_time_stats.mean),
            pimpl_->formatNumber(rise_time_stats.std_dev),
            pimpl_->formatNumber(rise_time_stats.min),
            pimpl_->formatNumber(rise_time_stats.max)});
        
        features_table.rows.push_back({"Fall Time", 
            pimpl_->formatNumber(fall_time_stats.mean),
            pimpl_->formatNumber(fall_time_stats.std_dev),
            pimpl_->formatNumber(fall_time_stats.min),
            pimpl_->formatNumber(fall_time_stats.max)});
        
        features_section.tables.push_back(features_table);
        addSection(features_section);
    }
    
    addSection(classification_section);
    
    return pimpl_->current_report_;
}

ChartData ReportGenerator::createLineChart(const std::vector<double>& x_data,
                                         const std::vector<double>& y_data,
                                         const std::string& title,
                                         const std::string& x_label,
                                         const std::string& y_label) {
    ChartData chart;
    chart.type = ChartType::LINE_PLOT;
    chart.title = title;
    chart.x_label = x_label;
    chart.y_label = y_label;
    chart.data = {x_data, y_data};
    chart.series_names = {"Data"};
    return chart;
}

ChartData ReportGenerator::createMultiLineChart(const std::vector<double>& x_data,
                                              const std::vector<std::vector<double>>& y_data,
                                              const std::vector<std::string>& series_names,
                                              const std::string& title,
                                              const std::string& x_label,
                                              const std::string& y_label) {
    ChartData chart;
    chart.type = ChartType::LINE_PLOT;
    chart.title = title;
    chart.x_label = x_label;
    chart.y_label = y_label;
    chart.data.push_back(x_data);
    for (const auto& y : y_data) {
        chart.data.push_back(y);
    }
    chart.series_names = series_names;
    return chart;
}

ChartData ReportGenerator::createHistogram(const std::vector<double>& data,
                                         int bins,
                                         const std::string& title,
                                         const std::string& x_label,
                                         const std::string& y_label) {
    ChartData chart;
    chart.type = ChartType::HISTOGRAM;
    chart.title = title;
    chart.x_label = x_label;
    chart.y_label = y_label;
    
    // 计算直方图
    auto histogram = pimpl_->statistical_analyzer_->generateHistogram(data, bins);
    
    // 生成bin中心
    std::vector<double> bin_centers;
    if (!data.empty()) {
        auto min_val = *std::min_element(data.begin(), data.end());
        auto max_val = *std::max_element(data.begin(), data.end());
        double bin_width = (max_val - min_val) / bins;
        
        for (int i = 0; i < bins; ++i) {
            bin_centers.push_back(min_val + (i + 0.5) * bin_width);
        }
    }
    
    chart.data = {bin_centers, histogram};
    chart.series_names = {"Frequency"};
    return chart;
}

ChartData ReportGenerator::createScatterPlot(const std::vector<double>& x_data,
                                           const std::vector<double>& y_data,
                                           const std::string& title,
                                           const std::string& x_label,
                                           const std::string& y_label) {
    ChartData chart;
    chart.type = ChartType::SCATTER_PLOT;
    chart.title = title;
    chart.x_label = x_label;
    chart.y_label = y_label;
    chart.data = {x_data, y_data};
    chart.series_names = {"Data"};
    return chart;
}

ChartData ReportGenerator::createBarChart(const std::vector<std::string>& categories,
                                        const std::vector<double>& values,
                                        const std::string& title,
                                        const std::string& x_label,
                                        const std::string& y_label) {
    ChartData chart;
    chart.type = ChartType::BAR_CHART;
    chart.title = title;
    chart.x_label = x_label;
    chart.y_label = y_label;
    chart.labels = categories;
    chart.data = {values};
    chart.series_names = {"Values"};
    return chart;
}

ChartData ReportGenerator::createPieChart(const std::vector<std::string>& labels,
                                        const std::vector<double>& values,
                                        const std::string& title) {
    ChartData chart;
    chart.type = ChartType::PIE_CHART;
    chart.title = title;
    chart.labels = labels;
    chart.data = {values};
    chart.series_names = {"Values"};
    return chart;
}

ChartData ReportGenerator::createHeatmap(const std::vector<std::vector<double>>& matrix,
                                       const std::vector<std::string>& row_labels,
                                       const std::vector<std::string>& col_labels,
                                       const std::string& title) {
    ChartData chart;
    chart.type = ChartType::HEATMAP;
    chart.title = title;
    chart.data = matrix;
    
    // 设置标签
    if (!row_labels.empty()) {
        chart.properties["row_labels"] = "";
        for (size_t i = 0; i < row_labels.size(); ++i) {
            if (i > 0) chart.properties["row_labels"] += ",";
            chart.properties["row_labels"] += row_labels[i];
        }
    }
    
    if (!col_labels.empty()) {
        chart.properties["col_labels"] = "";
        for (size_t i = 0; i < col_labels.size(); ++i) {
            if (i > 0) chart.properties["col_labels"] += ",";
            chart.properties["col_labels"] += col_labels[i];
        }
    }
    
    return chart;
}

ChartData ReportGenerator::createSpectrogram(const std::vector<std::vector<double>>& spectrogram,
                                           const std::vector<double>& time_axis,
                                           const std::vector<double>& frequency_axis,
                                           const std::string& title) {
    ChartData chart;
    chart.type = ChartType::SPECTROGRAM;
    chart.title = title;
    chart.x_label = "Time";
    chart.y_label = "Frequency";
    chart.data = spectrogram;
    
    // 设置轴信息
    chart.properties["time_axis"] = "";
    for (size_t i = 0; i < time_axis.size(); ++i) {
        if (i > 0) chart.properties["time_axis"] += ",";
        chart.properties["time_axis"] += std::to_string(time_axis[i]);
    }
    
    chart.properties["frequency_axis"] = "";
    for (size_t i = 0; i < frequency_axis.size(); ++i) {
        if (i > 0) chart.properties["frequency_axis"] += ",";
        chart.properties["frequency_axis"] += std::to_string(frequency_axis[i]);
    }
    
    return chart;
}

TableData ReportGenerator::createStatisticsTable(const std::vector<BasicStatistics>& stats,
                                                const std::vector<std::string>& data_names) {
    TableData table;
    table.title = "Statistical Summary";
    table.headers = {"Dataset", "Mean", "Median", "Std Dev", "Min", "Max", "Skewness", "Kurtosis"};
    
    for (size_t i = 0; i < stats.size(); ++i) {
        const auto& stat = stats[i];
        std::string name = (i < data_names.size()) ? data_names[i] : ("Dataset " + std::to_string(i + 1));
        
        std::vector<std::string> row = {
            name,
            pimpl_->formatNumber(stat.mean),
            pimpl_->formatNumber(stat.median),
            pimpl_->formatNumber(stat.std_dev),
            pimpl_->formatNumber(stat.min),
            pimpl_->formatNumber(stat.max),
            pimpl_->formatNumber(stat.skewness),
            pimpl_->formatNumber(stat.kurtosis)
        };
        
        table.rows.push_back(row);
    }
    
    return table;
}

TableData ReportGenerator::createClassificationTable(const std::vector<ClassificationResult>& results) {
    TableData table;
    table.title = "Classification Results";
    table.headers = {"Event #", "Predicted Class", "Confidence"};
    
    for (size_t i = 0; i < results.size(); ++i) {
        const auto& result = results[i];
        
        std::vector<std::string> row = {
            std::to_string(i + 1),
            std::to_string(result.predicted_class),
            pimpl_->formatNumber(result.confidence * 100) + "%"
        };
        
        table.rows.push_back(row);
    }
    
    return table;
}

TableData ReportGenerator::createCorrelationTable(const std::vector<std::vector<double>>& correlation_matrix,
                                                 const std::vector<std::string>& variable_names) {
    TableData table;
    table.title = "Correlation Matrix";
    
    // 设置表头
    table.headers.push_back("Variable");
    for (const auto& name : variable_names) {
        table.headers.push_back(name);
    }
    
    // 填充数据
    for (size_t i = 0; i < correlation_matrix.size() && i < variable_names.size(); ++i) {
        std::vector<std::string> row;
        row.push_back(variable_names[i]);
        
        for (size_t j = 0; j < correlation_matrix[i].size() && j < variable_names.size(); ++j) {
            row.push_back(pimpl_->formatNumber(correlation_matrix[i][j]));
        }
        
        table.rows.push_back(row);
    }
    
    return table;
}

TableData ReportGenerator::createFrequencyTable(const std::vector<std::string>& categories,
                                               const std::vector<int>& counts) {
    TableData table;
    table.title = "Frequency Distribution";
    table.headers = {"Category", "Count", "Percentage"};
    
    int total = 0;
    for (int count : counts) {
        total += count;
    }
    
    for (size_t i = 0; i < categories.size() && i < counts.size(); ++i) {
        double percentage = (total > 0) ? (100.0 * counts[i] / total) : 0.0;
        
        std::vector<std::string> row = {
            categories[i],
            std::to_string(counts[i]),
            pimpl_->formatNumber(percentage) + "%"
        };
        
        table.rows.push_back(row);
    }
    
    return table;
}

TextContent ReportGenerator::createSummaryText(const AnalysisSummary& summary) {
    TextContent text;
    text.title = "Analysis Summary";
    text.format = "markdown";
    
    std::stringstream content;
    content << "## Dataset Overview\n\n";
    content << "- **Dataset Name**: " << summary.dataset_name << "\n";
    content << "- **Total Events**: " << summary.total_events << "\n";
    content << "- **Total Data Points**: " << summary.total_data_points << "\n";
    content << "- **Duration**: " << pimpl_->formatNumber(summary.duration) << " seconds\n";
    content << "- **Sampling Rate**: " << pimpl_->formatNumber(summary.sampling_rate) << " Hz\n\n";
    
    content << "## Key Findings\n\n";
    content << "- **Classification Accuracy**: " << pimpl_->formatNumber(summary.classification_accuracy * 100) << "%\n";
    content << "- **Dominant Frequency**: " << pimpl_->formatNumber(summary.dominant_frequency) << " Hz\n";
    content << "- **Signal-to-Noise Ratio**: " << pimpl_->formatNumber(summary.signal_to_noise_ratio) << " dB\n";
    content << "- **Data Quality Score**: " << pimpl_->formatNumber(summary.data_quality_score) << "/100\n\n";
    
    if (!summary.quality_issues.empty()) {
        content << "## Quality Issues\n\n";
        for (const auto& issue : summary.quality_issues) {
            content << "- " << issue << "\n";
        }
        content << "\n";
    }
    
    if (!summary.recommendations.empty()) {
        content << "## Recommendations\n\n";
        for (const auto& recommendation : summary.recommendations) {
            content << "- " << recommendation << "\n";
        }
    }
    
    text.content = content.str();
    return text;
}

TextContent ReportGenerator::createMethodologyText(const std::vector<std::string>& methods) {
    TextContent text;
    text.title = "Methodology";
    text.format = "markdown";
    
    std::stringstream content;
    content << "## Analysis Methods\n\n";
    content << "The following methods were used in this analysis:\n\n";
    
    for (size_t i = 0; i < methods.size(); ++i) {
        content << (i + 1) << ". " << methods[i] << "\n";
    }
    
    text.content = content.str();
    return text;
}

TextContent ReportGenerator::createConclusionText(const std::vector<std::string>& findings) {
    TextContent text;
    text.title = "Conclusions";
    text.format = "markdown";
    
    std::stringstream content;
    content << "## Key Findings\n\n";
    
    for (size_t i = 0; i < findings.size(); ++i) {
        content << "- " << findings[i] << "\n";
    }
    
    text.content = content.str();
    return text;
}

void ReportGenerator::exportToHTML(const ReportData& report, const std::string& filename) {
    std::ofstream file(output_directory_ + "/" + filename);
    if (!file.is_open()) {
        return;
    }
    
    file << pimpl_->generateHTMLHeader(report.config.title);
    
    // 标题和元信息
    file << "<h1>" << report.config.title << "</h1>\n";
    file << "<p class=\"timestamp\">Generated on: " << pimpl_->getCurrentTimestamp() << "</p>\n";
    file << "<p>Author: " << report.config.author << "</p>\n";
    
    // 目录
    if (report.config.include_toc && !report.sections.empty()) {
        file << "<h2>Table of Contents</h2>\n";
        file << "<ul>\n";
        for (const auto& section : report.sections) {
            file << "<li><a href=\"#" << section.title << "\">" << section.title << "</a></li>\n";
        }
        file << "</ul>\n";
    }
    
    // 摘要
    if (report.config.include_summary && !report.summary.empty()) {
        file << "<h2>Summary</h2>\n";
        file << "<p>" << report.summary << "</p>\n";
    }
    
    // 各个部分
    for (const auto& section : report.sections) {
        file << "<h2 id=\"" << section.title << "\">" << section.title << "</h2>\n";
        if (!section.description.empty()) {
            file << "<p>" << section.description << "</p>\n";
        }
        
        // 文本内容
        for (const auto& text : section.text_contents) {
            file << "<h3>" << text.title << "</h3>\n";
            if (text.format == "markdown") {
                // 简单的markdown转换
                std::string html_content = text.content;
                // 这里可以添加更复杂的markdown转HTML转换
                file << "<pre>" << html_content << "</pre>\n";
            } else {
                file << "<p>" << text.content << "</p>\n";
            }
        }
        
        // 表格
        for (const auto& table : section.tables) {
            file << "<h3>" << table.title << "</h3>\n";
            file << "<table>\n";
            
            // 表头
            if (!table.headers.empty()) {
                file << "<tr>";
                for (const auto& header : table.headers) {
                    file << "<th>" << header << "</th>";
                }
                file << "</tr>\n";
            }
            
            // 数据行
            for (const auto& row : table.rows) {
                file << "<tr>";
                for (const auto& cell : row) {
                    file << "<td>" << cell << "</td>";
                }
                file << "</tr>\n";
            }
            
            file << "</table>\n";
        }
        
        // 图表占位符
        for (const auto& chart : section.charts) {
            file << "<div class=\"plot-container\">\n";
            file << "<h3>" << chart.title << "</h3>\n";
            file << "<p>[Chart: " << chart.title << " - " << static_cast<int>(chart.type) << "]</p>\n";
            file << "</div>\n";
        }
        
        // 图像
        for (const auto& image : section.images) {
            file << "<div class=\"plot-container\">\n";
            file << "<h3>" << image.title << "</h3>\n";
            file << "<img src=\"" << image.file_path << "\" alt=\"" << image.caption << "\"";
            if (image.width > 0) file << " width=\"" << image.width << "\"";
            if (image.height > 0) file << " height=\"" << image.height << "\"";
            file << ">\n";
            if (!image.caption.empty()) {
                file << "<p>" << image.caption << "</p>\n";
            }
            file << "</div>\n";
        }
    }
    
    // 结论
    if (!report.conclusion.empty()) {
        file << "<h2>Conclusion</h2>\n";
        file << "<p>" << report.conclusion << "</p>\n";
    }
    
    file << pimpl_->generateHTMLFooter();
    file.close();
}

void ReportGenerator::exportToPDF(const ReportData& report, const std::string& filename) {
    // PDF导出需要外部库支持，这里提供接口
    // 实际实现可能需要使用如wkhtmltopdf或其他PDF生成库
    
    // 先生成HTML，然后转换为PDF
    std::string html_filename = filename + ".html";
    exportToHTML(report, html_filename);
    
    // 这里可以调用外部工具将HTML转换为PDF
    // 例如: system(("wkhtmltopdf " + html_filename + " " + filename).c_str());
}

void ReportGenerator::exportToMarkdown(const ReportData& report, const std::string& filename) {
    std::ofstream file(output_directory_ + "/" + filename);
    if (!file.is_open()) {
        return;
    }
    
    // 标题和元信息
    file << "# " << report.config.title << "\n\n";
    file << "**Generated on:** " << pimpl_->getCurrentTimestamp() << "\n";
    file << "**Author:** " << report.config.author << "\n\n";
    file << "---\n\n";
    
    // 摘要
    if (report.config.include_summary && !report.summary.empty()) {
        file << "## Summary\n\n";
        file << report.summary << "\n\n";
    }
    
    // 各个部分
    for (const auto& section : report.sections) {
        file << "## " << section.title << "\n\n";
        if (!section.description.empty()) {
            file << section.description << "\n\n";
        }
        
        // 文本内容
        for (const auto& text : section.text_contents) {
            file << "### " << text.title << "\n\n";
            file << text.content << "\n\n";
        }
        
        // 表格
        for (const auto& table : section.tables) {
            file << "### " << table.title << "\n\n";
            
            if (!table.headers.empty()) {
                // 表头
                file << "|";
                for (const auto& header : table.headers) {
                    file << " " << header << " |";
                }
                file << "\n";
                
                // 分隔线
                file << "|";
                for (size_t i = 0; i < table.headers.size(); ++i) {
                    file << "---|";
                }
                file << "\n";
            }
            
            // 数据行
            for (const auto& row : table.rows) {
                file << "|";
                for (const auto& cell : row) {
                    file << " " << cell << " |";
                }
                file << "\n";
            }
            
            file << "\n";
        }
        
        // 图表占位符
        for (const auto& chart : section.charts) {
            file << "### " << chart.title << "\n\n";
            file << "[Chart: " << chart.title << "]\n\n";
        }
        
        // 图像
        for (const auto& image : section.images) {
            file << "### " << image.title << "\n\n";
            file << "![" << image.caption << "](" << image.file_path << ")\n\n";
        }
    }
    
    // 结论
    if (!report.conclusion.empty()) {
        file << "## Conclusion\n\n";
        file << report.conclusion << "\n\n";
    }
    
    file.close();
}

void ReportGenerator::exportToJSON(const ReportData& report, const std::string& filename) {
    std::ofstream file(output_directory_ + "/" + filename);
    if (!file.is_open()) {
        return;
    }
    
    file << "{\n";
    file << "  \"report_metadata\": {\n";
    file << "    \"title\": \"" << report.config.title << "\",\n";
    file << "    \"author\": \"" << report.config.author << "\",\n";
    file << "    \"generated_at\": \"" << pimpl_->getCurrentTimestamp() << "\",\n";
    file << "    \"format\": \"JSON\",\n";
    file << "    \"version\": \"1.0\"\n";
    file << "  },\n";
    
    if (!report.summary.empty()) {
        file << "  \"summary\": \"" << report.summary << "\",\n";
    }
    
    file << "  \"sections\": [\n";
    for (size_t i = 0; i < report.sections.size(); ++i) {
        if (i > 0) file << ",\n";
        const auto& section = report.sections[i];
        
        file << "    {\n";
        file << "      \"title\": \"" << section.title << "\",\n";
        file << "      \"description\": \"" << section.description << "\",\n";
        
        // 文本内容
        if (!section.text_contents.empty()) {
            file << "      \"text_contents\": [\n";
            for (size_t j = 0; j < section.text_contents.size(); ++j) {
                if (j > 0) file << ",\n";
                const auto& text = section.text_contents[j];
                file << "        {\n";
                file << "          \"title\": \"" << text.title << "\",\n";
                file << "          \"content\": \"" << text.content << "\",\n";
                file << "          \"format\": \"" << text.format << "\"\n";
                file << "        }";
            }
            file << "\n      ],\n";
        }
        
        // 表格
        if (!section.tables.empty()) {
            file << "      \"tables\": [\n";
            for (size_t j = 0; j < section.tables.size(); ++j) {
                if (j > 0) file << ",\n";
                const auto& table = section.tables[j];
                file << "        {\n";
                file << "          \"title\": \"" << table.title << "\",\n";
                file << "          \"headers\": [";
                for (size_t k = 0; k < table.headers.size(); ++k) {
                    if (k > 0) file << ", ";
                    file << "\"" << table.headers[k] << "\"";
                }
                file << "],\n";
                file << "          \"rows\": [\n";
                for (size_t k = 0; k < table.rows.size(); ++k) {
                    if (k > 0) file << ",\n";
                    file << "            [";
                    for (size_t l = 0; l < table.rows[k].size(); ++l) {
                        if (l > 0) file << ", ";
                        file << "\"" << table.rows[k][l] << "\"";
                    }
                    file << "]";
                }
                file << "\n          ]\n";
                file << "        }";
            }
            file << "\n      ]\n";
        } else {
            file << "      \"tables\": []\n";
        }
        
        file << "    }";
    }
    file << "\n  ]";
    
    if (!report.conclusion.empty()) {
        file << ",\n  \"conclusion\": \"" << report.conclusion << "\"";
    }
    
    file << "\n}\n";
    file.close();
}

void ReportGenerator::loadTemplate(const std::string& template_name) {
    // 模板加载实现
    // 这里可以从文件或数据库加载预定义的模板
}

void ReportGenerator::saveTemplate(const std::string& template_name, const ReportConfig& config) {
    // 模板保存实现
    // 这里可以将配置保存为模板文件
}

std::vector<std::string> ReportGenerator::getAvailableTemplates() const {
    // 返回可用的模板列表
    return {"default", "statistical", "spectral", "classification", "comprehensive"};
}

// ReportTemplateManager实现
class ReportTemplateManager::Impl {
public:
    std::map<std::string, ReportConfig> templates_;
    
    Impl() {
        // 初始化默认模板
        initializeDefaultTemplates();
    }
    
    void initializeDefaultTemplates() {
        // 默认模板
        ReportConfig default_config;
        default_config.title = "Analysis Report";
        default_config.author = "Nanopore Analysis System";
        default_config.template_name = "default";
        default_config.output_format = "html";
        default_config.include_toc = true;
        default_config.include_summary = true;
        templates_["default"] = default_config;
        
        // 统计分析模板
        ReportConfig statistical_config = default_config;
        statistical_config.title = "Statistical Analysis Report";
        statistical_config.template_name = "statistical";
        statistical_config.theme = "statistical";
        templates_["statistical"] = statistical_config;
        
        // 频谱分析模板
        ReportConfig spectral_config = default_config;
        spectral_config.title = "Spectral Analysis Report";
        spectral_config.template_name = "spectral";
        spectral_config.theme = "spectral";
        templates_["spectral"] = spectral_config;
        
        // 分类分析模板
        ReportConfig classification_config = default_config;
        classification_config.title = "Event Classification Report";
        classification_config.template_name = "classification";
        classification_config.theme = "classification";
        templates_["classification"] = classification_config;
        
        // 综合分析模板
        ReportConfig comprehensive_config = default_config;
        comprehensive_config.title = "Comprehensive Analysis Report";
        comprehensive_config.template_name = "comprehensive";
        comprehensive_config.theme = "comprehensive";
        comprehensive_config.include_appendix = true;
        templates_["comprehensive"] = comprehensive_config;
    }
};

ReportTemplateManager::ReportTemplateManager() : pimpl_(std::make_unique<Impl>()) {}
ReportTemplateManager::~ReportTemplateManager() = default;

void ReportTemplateManager::createTemplate(const std::string& name, const ReportConfig& config) {
    pimpl_->templates_[name] = config;
}

ReportConfig ReportTemplateManager::loadTemplate(const std::string& name) {
    auto it = pimpl_->templates_.find(name);
    if (it != pimpl_->templates_.end()) {
        return it->second;
    }
    return pimpl_->templates_["default"];
}

void ReportTemplateManager::deleteTemplate(const std::string& name) {
    if (name != "default") {  // 不允许删除默认模板
        pimpl_->templates_.erase(name);
    }
}

std::vector<std::string> ReportTemplateManager::listTemplates() const {
    std::vector<std::string> template_names;
    for (const auto& pair : pimpl_->templates_) {
        template_names.push_back(pair.first);
    }
    return template_names;
}

ReportConfig ReportTemplateManager::getDefaultTemplate() {
    return pimpl_->templates_["default"];
}

ReportConfig ReportTemplateManager::getStatisticalTemplate() {
    return pimpl_->templates_["statistical"];
}

ReportConfig ReportTemplateManager::getSpectralTemplate() {
    return pimpl_->templates_["spectral"];
}

ReportConfig ReportTemplateManager::getClassificationTemplate() {
    return pimpl_->templates_["classification"];
}

ReportConfig ReportTemplateManager::getComprehensiveTemplate() {
    return pimpl_->templates_["comprehensive"];
}

// ReportUtils命名空间实现
namespace ReportUtils {

std::string formatNumber(double value, int precision) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(precision) << value;
    return ss.str();
}

std::string formatPercentage(double value, int precision) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(precision) << (value * 100) << "%";
    return ss.str();
}

std::string formatScientific(double value, int precision) {
    std::stringstream ss;
    ss << std::scientific << std::setprecision(precision) << value;
    return ss.str();
}

std::string formatDuration(double seconds) {
    if (seconds < 60) {
        return formatNumber(seconds, 2) + " seconds";
    } else if (seconds < 3600) {
        return formatNumber(seconds / 60, 2) + " minutes";
    } else {
        return formatNumber(seconds / 3600, 2) + " hours";
    }
}

std::string formatFileSize(size_t bytes) {
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int unit = 0;
    double size = static_cast<double>(bytes);
    
    while (size >= 1024 && unit < 4) {
        size /= 1024;
        unit++;
    }
    
    return formatNumber(size, 2) + " " + units[unit];
}

std::string escapeHTML(const std::string& text) {
    std::string result = text;
    std::string::size_type pos = 0;
    
    // 替换特殊字符
    while ((pos = result.find("&", pos)) != std::string::npos) {
        result.replace(pos, 1, "&amp;");
        pos += 5;
    }
    
    pos = 0;
    while ((pos = result.find("<", pos)) != std::string::npos) {
        result.replace(pos, 1, "&lt;");
        pos += 4;
    }
    
    pos = 0;
    while ((pos = result.find(">", pos)) != std::string::npos) {
        result.replace(pos, 1, "&gt;");
        pos += 4;
    }
    
    pos = 0;
    while ((pos = result.find("\"", pos)) != std::string::npos) {
        result.replace(pos, 1, "&quot;");
        pos += 6;
    }
    
    return result;
}

std::string escapeMarkdown(const std::string& text) {
    std::string result = text;
    std::string special_chars = "\\`*_{}[]()#+-.!";
    
    for (char c : special_chars) {
        std::string::size_type pos = 0;
        std::string target(1, c);
        std::string replacement = "\\" + target;
        
        while ((pos = result.find(target, pos)) != std::string::npos) {
            result.replace(pos, 1, replacement);
            pos += 2;
        }
    }
    
    return result;
}

std::string convertMarkdownToHTML(const std::string& markdown) {
    // 简单的markdown到HTML转换
    std::string html = markdown;
    
    // 替换标题
    std::string::size_type pos = 0;
    while ((pos = html.find("## ", pos)) != std::string::npos) {
        html.replace(pos, 3, "<h2>");
        size_t end = html.find("\n", pos);
        if (end != std::string::npos) {
            html.insert(end, "</h2>");
        }
        pos = end + 5;
    }
    
    // 替换粗体
    pos = 0;
    while ((pos = html.find("**", pos)) != std::string::npos) {
        html.replace(pos, 2, "<strong>");
        size_t end = html.find("**", pos + 8);
        if (end != std::string::npos) {
            html.replace(end, 2, "</strong>");
        }
        pos = end + 9;
    }
    
    return html;
}

std::vector<std::string> generateColorPalette(int n_colors, const std::string& scheme) {
    std::vector<std::string> colors;
    
    if (scheme == "default" || scheme == "blue") {
        // 蓝色系配色方案
        std::vector<std::string> base_colors = {
            "#1f77b4", "#ff7f0e", "#2ca02c", "#d62728", "#9467bd",
            "#8c564b", "#e377c2", "#7f7f7f", "#bcbd22", "#17becf"
        };
        
        for (int i = 0; i < n_colors; ++i) {
            colors.push_back(base_colors[i % base_colors.size()]);
        }
    } else if (scheme == "viridis") {
        // Viridis配色方案
        for (int i = 0; i < n_colors; ++i) {
            double t = static_cast<double>(i) / (n_colors - 1);
            int r = static_cast<int>(255 * (0.267 + 0.005 * t));
            int g = static_cast<int>(255 * (0.005 + 0.898 * t));
            int b = static_cast<int>(255 * (0.329 + 0.659 * t));
            colors.push_back(rgbToHex(r, g, b));
        }
    }
    
    return colors;
}

std::string rgbToHex(int r, int g, int b) {
    std::stringstream ss;
    ss << "#" << std::hex << std::setfill('0') << std::setw(2) << r
       << std::setw(2) << g << std::setw(2) << b;
    return ss.str();
}

std::tuple<int, int, int> hexToRgb(const std::string& hex) {
    std::string hex_clean = hex;
    if (hex_clean[0] == '#') {
        hex_clean = hex_clean.substr(1);
    }
    
    int r = std::stoi(hex_clean.substr(0, 2), nullptr, 16);
    int g = std::stoi(hex_clean.substr(2, 2), nullptr, 16);
    int b = std::stoi(hex_clean.substr(4, 2), nullptr, 16);
    
    return std::make_tuple(r, g, b);
}

std::vector<double> calculateBinEdges(const std::vector<double>& data, int n_bins) {
    if (data.empty()) {
        return {};
    }
    
    auto min_val = *std::min_element(data.begin(), data.end());
    auto max_val = *std::max_element(data.begin(), data.end());
    
    std::vector<double> edges;
    double bin_width = (max_val - min_val) / n_bins;
    
    for (int i = 0; i <= n_bins; ++i) {
        edges.push_back(min_val + i * bin_width);
    }
    
    return edges;
}

std::vector<int> calculateHistogram(const std::vector<double>& data, 
                                  const std::vector<double>& bin_edges) {
    if (bin_edges.size() < 2) {
        return {};
    }
    
    std::vector<int> histogram(bin_edges.size() - 1, 0);
    
    for (double value : data) {
        for (size_t i = 0; i < bin_edges.size() - 1; ++i) {
            if (value >= bin_edges[i] && value < bin_edges[i + 1]) {
                histogram[i]++;
                break;
            }
        }
    }
    
    return histogram;
}

bool createDirectory(const std::string& path) {
    // 简单的目录创建实现
    // 实际实现可能需要使用系统调用或filesystem库
    return true;
}

std::string generateUniqueFilename(const std::string& base_name, 
                                 const std::string& extension) {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << base_name << "_" << std::put_time(std::localtime(&time_t), "%Y%m%d_%H%M%S") << extension;
    return ss.str();
}

std::string getTimestamp(const std::string& format) {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), format.c_str());
    return ss.str();
}

bool validateChartData(const ChartData& chart) {
    // 验证图表数据的完整性
    if (chart.data.empty()) {
        return false;
    }
    
    // 检查数据维度一致性
    if (!chart.data.empty()) {
        size_t expected_size = chart.data[0].size();
        for (const auto& series : chart.data) {
            if (series.size() != expected_size) {
                return false;
            }
        }
    }
    
    return true;
}

bool validateTableData(const TableData& table) {
    // 验证表格数据的完整性
    if (!table.headers.empty()) {
        for (const auto& row : table.rows) {
            if (row.size() != table.headers.size()) {
                return false;
            }
        }
    }
    
    return true;
}

std::vector<std::string> checkReportConsistency(const ReportData& report) {
    std::vector<std::string> issues;
    
    // 检查报告配置
    if (report.config.title.empty()) {
        issues.push_back("Report title is empty");
    }
    
    if (report.config.author.empty()) {
        issues.push_back("Report author is empty");
    }
    
    // 检查各个部分
    for (const auto& section : report.sections) {
        if (section.title.empty()) {
            issues.push_back("Section title is empty");
        }
        
        // 检查表格数据
        for (const auto& table : section.tables) {
            if (!validateTableData(table)) {
                issues.push_back("Invalid table data in section: " + section.title);
            }
        }
        
        // 检查图表数据
        for (const auto& chart : section.charts) {
            if (!validateChartData(chart)) {
                issues.push_back("Invalid chart data in section: " + section.title);
            }
        }
    }
    
    return issues;
}

} // namespace ReportUtils

} // namespace Analysis