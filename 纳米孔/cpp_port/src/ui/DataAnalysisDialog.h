#ifndef DATAANALYSISDIALOG_H
#define DATAANALYSISDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QTextEdit>
#include <QProgressBar>
#include <QTableWidget>
#include <QSplitter>
#include <vector>

class DataAnalysisDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DataAnalysisDialog(QWidget *parent = nullptr);
    
    // 设置要分析的数据
    void setAnalysisData(const std::vector<double>& timeData, 
                        const std::vector<double>& currentData);

private slots:
    void performAnalysis();
    void applyFiltering();
    void applyDenoising();
    void generateReport();
    void exportResults();
    void resetAnalysis();

private:
    void setupUI();
    void setupFilteringControls();
    void setupDenoisingControls();
    void setupAnalysisControls();
    void setupResultsDisplay();
    
    void updateAnalysisResults();
    void displayStatistics();
    
    // UI组件
    QVBoxLayout* mainLayout_;
    QSplitter* mainSplitter_;
    
    // 控制面板
    QWidget* controlPanel_;
    QGroupBox* filterGroup_;
    QGroupBox* denoiseGroup_;
    QGroupBox* analysisGroup_;
    
    // 滤波控制
    QComboBox* filterTypeCombo_;
    QDoubleSpinBox* lowCutoffSpin_;
    QDoubleSpinBox* highCutoffSpin_;
    QSpinBox* filterOrderSpin_;
    QPushButton* applyFilterBtn_;
    
    // 去噪控制
    QComboBox* denoiseMethodCombo_;
    QDoubleSpinBox* denoiseThresholdSpin_;
    QSpinBox* windowSizeSpin_;
    QPushButton* applyDenoiseBtn_;
    
    // 分析控制
    QCheckBox* enableBaselineCheck_;
    QCheckBox* enableEventDetectionCheck_;
    QCheckBox* enableStatisticsCheck_;
    QDoubleSpinBox* eventThresholdSpin_;
    QPushButton* analyzeBtn_;
    
    // 结果显示
    QWidget* resultsPanel_;
    QTextEdit* statisticsText_;
    QTableWidget* eventsTable_;
    QTextEdit* reportText_;
    QProgressBar* progressBar_;
    
    // 操作按钮
    QPushButton* generateReportBtn_;
    QPushButton* exportBtn_;
    QPushButton* resetBtn_;
    QPushButton* closeBtn_;
    
    // 数据
    std::vector<double> originalTimeData_;
    std::vector<double> originalCurrentData_;
    std::vector<double> processedTimeData_;
    std::vector<double> processedCurrentData_;
    
    // 分析结果
    struct AnalysisResults {
        double mean;
        double std;
        double rms;
        double baseline;
        int eventCount;
        double eventRate;
        std::vector<double> eventTimes;
        std::vector<double> eventAmplitudes;
        std::vector<double> eventDurations;
    } results_;
};

#endif // DATAANALYSISDIALOG_H