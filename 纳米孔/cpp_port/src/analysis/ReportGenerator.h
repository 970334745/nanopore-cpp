#ifndef REPORT_GENERATOR_H
#define REPORT_GENERATOR_H

#include <vector>
#include <string>
#include <map>
#include <memory>
#include <chrono>

namespace Analysis {

// 前向声明
struct BasicStatistics;
struct SpectrumResult;
struct ClassificationResult;
struct EventFeatures;

// 图表类型
enum class ChartType {
    LINE_PLOT,
    SCATTER_PLOT,
    HISTOGRAM,
    BAR_CHART,
    PIE_CHART,
    HEATMAP,
    SPECTROGRAM,
    BOX_PLOT,
    VIOLIN_PLOT,
    CORRELATION_MATRIX
};

// 图表数据
struct ChartData {
    ChartType type = ChartType::LINE_PLOT;
    std::string title;
    std::string x_label;
    std::string y_label;
    std::vector<std::vector<double>> data;
    std::vector<std::string> series_names;
    std::vector<std::string> labels;
    std::map<std::string, std::string> properties;
    std::string color_scheme = "default";
    bool show_legend = true;
    bool show_grid = true;
};

// 表格数据
struct TableData {
    std::string title;
    std::vector<std::string> headers;
    std::vector<std::vector<std::string>> rows;
    std::map<std::string, std::string> formatting;
    bool show_index = false;
    std::string alignment = "left";
};

// 文本内容
struct TextContent {
    std::string title;
    std::string content;
    std::string format = "plain"; // plain, markdown, html
    int heading_level = 1;
    std::map<std::string, std::string> style;
};

// 图像内容
struct ImageContent {
    std::string title;
    std::string file_path;
    std::string caption;
    double width = 0.0;  // 0 means auto
    double height = 0.0; // 0 means auto
    std::string alignment = "center";
};

// 报告节
struct ReportSection {
    std::string title;
    std::string description;
    std::vector<TextContent> text_contents;
    std::vector<ChartData> charts;
    std::vector<TableData> tables;
    std::vector<ImageContent> images;
    std::map<std::string, std::string> metadata;
    int section_level = 1;
};

// 报告配置
struct ReportConfig {
    std::string title = "Analysis Report";
    std::string author = "Nanopore Analysis System";
    std::string organization = "";
    std::chrono::system_clock::time_point creation_time;
    std::string template_name = "default";
    std::string output_format = "html"; // html, pdf, markdown, docx
    std::string language = "en";
    std::map<std::string, std::string> custom_settings;
    
    // 样式设置
    std::string theme = "default";
    std::string font_family = "Arial";
    int font_size = 12;
    std::string color_scheme = "blue";
    bool include_toc = true;
    bool include_summary = true;
    bool include_appendix = false;
};

// 报告数据
struct ReportData {
    ReportConfig config;
    std::vector<ReportSection> sections;
    std::map<std::string, std::string> global_metadata;
    std::vector<std::string> references;
    std::string summary;
    std::string conclusion;
};

// 分析摘要
struct AnalysisSummary {
    // 数据概览
    std::string dataset_name;
    size_t total_events = 0;
    size_t total_data_points = 0;
    double duration = 0.0;
    double sampling_rate = 0.0;
    
    // 统计摘要
    std::shared_ptr<BasicStatistics> overall_statistics;
    std::map<std::string, double> key_metrics;
    
    // 分类摘要
    std::map<std::string, int> class_distribution;
    double classification_accuracy = 0.0;
    
    // 频谱摘要
    double dominant_frequency = 0.0;
    double bandwidth = 0.0;
    double signal_to_noise_ratio = 0.0;
    
    // 质量评估
    double data_quality_score = 0.0;
    std::vector<std::string> quality_issues;
    std::vector<std::string> recommendations;
};

// 报告生成器
class ReportGenerator {
public:
    ReportGenerator();
    ~ReportGenerator();
    
    // 报告创建
    void createReport(const ReportConfig& config);
    void addSection(const ReportSection& section);
    void addTextContent(const std::string& section_title, const TextContent& content);
    void addChart(const std::string& section_title, const ChartData& chart);
    void addTable(const std::string& section_title, const TableData& table);
    void addImage(const std::string& section_title, const ImageContent& image);
    
    // 自动报告生成
    ReportData generateAnalysisReport(const AnalysisSummary& summary);
    ReportData generateStatisticalReport(const std::vector<BasicStatistics>& stats,
                                       const std::vector<std::string>& data_names);
    ReportData generateSpectralReport(const std::vector<SpectrumResult>& spectra,
                                    const std::vector<std::string>& signal_names);
    ReportData generateClassificationReport(
        const std::vector<ClassificationResult>& results,
        const std::vector<EventFeatures>& features);
    
    // 图表生成
    ChartData createLineChart(const std::vector<double>& x_data,
                            const std::vector<double>& y_data,
                            const std::string& title = "",
                            const std::string& x_label = "",
                            const std::string& y_label = "");
    
    ChartData createMultiLineChart(const std::vector<double>& x_data,
                                 const std::vector<std::vector<double>>& y_data,
                                 const std::vector<std::string>& series_names,
                                 const std::string& title = "",
                                 const std::string& x_label = "",
                                 const std::string& y_label = "");
    
    ChartData createHistogram(const std::vector<double>& data,
                            int bins = 50,
                            const std::string& title = "",
                            const std::string& x_label = "",
                            const std::string& y_label = "Frequency");
    
    ChartData createScatterPlot(const std::vector<double>& x_data,
                              const std::vector<double>& y_data,
                              const std::string& title = "",
                              const std::string& x_label = "",
                              const std::string& y_label = "");
    
    ChartData createBarChart(const std::vector<std::string>& categories,
                           const std::vector<double>& values,
                           const std::string& title = "",
                           const std::string& x_label = "",
                           const std::string& y_label = "");
    
    ChartData createPieChart(const std::vector<std::string>& labels,
                           const std::vector<double>& values,
                           const std::string& title = "");
    
    ChartData createHeatmap(const std::vector<std::vector<double>>& matrix,
                          const std::vector<std::string>& row_labels = {},
                          const std::vector<std::string>& col_labels = {},
                          const std::string& title = "");
    
    ChartData createSpectrogram(const std::vector<std::vector<double>>& spectrogram,
                              const std::vector<double>& time_axis,
                              const std::vector<double>& frequency_axis,
                              const std::string& title = "Spectrogram");
    
    // 表格生成
    TableData createStatisticsTable(const std::vector<BasicStatistics>& stats,
                                   const std::vector<std::string>& data_names);
    TableData createClassificationTable(const std::vector<ClassificationResult>& results);
    TableData createCorrelationTable(const std::vector<std::vector<double>>& correlation_matrix,
                                    const std::vector<std::string>& variable_names);
    TableData createFrequencyTable(const std::vector<std::string>& categories,
                                  const std::vector<int>& counts);
    
    // 文本生成
    TextContent createSummaryText(const AnalysisSummary& summary);
    TextContent createMethodologyText(const std::vector<std::string>& methods);
    TextContent createConclusionText(const std::vector<std::string>& findings);
    
    // 报告输出
    void exportToHTML(const ReportData& report, const std::string& filename);
    void exportToPDF(const ReportData& report, const std::string& filename);
    void exportToMarkdown(const ReportData& report, const std::string& filename);
    void exportToJSON(const ReportData& report, const std::string& filename);
    
    // 模板管理
    void loadTemplate(const std::string& template_name);
    void saveTemplate(const std::string& template_name, const ReportConfig& config);
    std::vector<std::string> getAvailableTemplates() const;
    
    // 配置
    void setOutputDirectory(const std::string& directory) { output_directory_ = directory; }
    void setImageFormat(const std::string& format) { image_format_ = format; }
    void setImageDPI(int dpi) { image_dpi_ = dpi; }
    void setChartTheme(const std::string& theme) { chart_theme_ = theme; }

private:
    class Impl;
    std::unique_ptr<Impl> pimpl_;
    
    std::string output_directory_ = "./reports";
    std::string image_format_ = "png";
    int image_dpi_ = 300;
    std::string chart_theme_ = "default";
};

// 报告模板管理器
class ReportTemplateManager {
public:
    ReportTemplateManager();
    ~ReportTemplateManager();
    
    // 模板操作
    void createTemplate(const std::string& name, const ReportConfig& config);
    ReportConfig loadTemplate(const std::string& name);
    void deleteTemplate(const std::string& name);
    std::vector<std::string> listTemplates() const;
    
    // 预定义模板
    ReportConfig getDefaultTemplate();
    ReportConfig getStatisticalTemplate();
    ReportConfig getSpectralTemplate();
    ReportConfig getClassificationTemplate();
    ReportConfig getComprehensiveTemplate();

private:
    class Impl;
    std::unique_ptr<Impl> pimpl_;
};

// 工具函数
namespace ReportUtils {
    // 数据格式化
    std::string formatNumber(double value, int precision = 3);
    std::string formatPercentage(double value, int precision = 1);
    std::string formatScientific(double value, int precision = 2);
    std::string formatDuration(double seconds);
    std::string formatFileSize(size_t bytes);
    
    // 文本处理
    std::string escapeHTML(const std::string& text);
    std::string escapeMarkdown(const std::string& text);
    std::string convertMarkdownToHTML(const std::string& markdown);
    
    // 颜色和样式
    std::vector<std::string> generateColorPalette(int n_colors, 
                                                 const std::string& scheme = "default");
    std::string rgbToHex(int r, int g, int b);
    std::tuple<int, int, int> hexToRgb(const std::string& hex);
    
    // 图表工具
    std::vector<double> calculateBinEdges(const std::vector<double>& data, int n_bins);
    std::vector<int> calculateHistogram(const std::vector<double>& data, 
                                      const std::vector<double>& bin_edges);
    
    // 文件操作
    bool createDirectory(const std::string& path);
    std::string generateUniqueFilename(const std::string& base_name, 
                                     const std::string& extension);
    std::string getTimestamp(const std::string& format = "%Y-%m-%d_%H-%M-%S");
    
    // 数据验证
    bool validateChartData(const ChartData& chart);
    bool validateTableData(const TableData& table);
    std::vector<std::string> checkReportConsistency(const ReportData& report);
}

} // namespace Analysis

#endif // REPORT_GENERATOR_H