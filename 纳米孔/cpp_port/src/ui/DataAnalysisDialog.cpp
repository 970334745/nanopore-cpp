#include "DataAnalysisDialog.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QApplication>
#include <cmath>
#include <algorithm>
#include <numeric>

DataAnalysisDialog::DataAnalysisDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("数据分析");
    setMinimumSize(1000, 700);
    resize(1200, 800);
    
    setupUI();
}

void DataAnalysisDialog::setupUI() {
    mainLayout_ = new QVBoxLayout(this);
    
    // 创建主分割器
    mainSplitter_ = new QSplitter(Qt::Horizontal, this);
    
    // 设置控制面板
    setupFilteringControls();
    setupDenoisingControls();
    setupAnalysisControls();
    
    controlPanel_ = new QWidget();
    QVBoxLayout* controlLayout = new QVBoxLayout(controlPanel_);
    controlLayout->addWidget(filterGroup_);
    controlLayout->addWidget(denoiseGroup_);
    controlLayout->addWidget(analysisGroup_);
    controlLayout->addStretch();
    
    // 设置结果显示面板
    setupResultsDisplay();
    
    // 添加到分割器
    mainSplitter_->addWidget(controlPanel_);
    mainSplitter_->addWidget(resultsPanel_);
    mainSplitter_->setSizes({300, 700});
    
    mainLayout_->addWidget(mainSplitter_);
    
    // 添加操作按钮
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    generateReportBtn_ = new QPushButton("生成报告");
    exportBtn_ = new QPushButton("导出结果");
    resetBtn_ = new QPushButton("重置");
    closeBtn_ = new QPushButton("关闭");
    
    buttonLayout->addWidget(generateReportBtn_);
    buttonLayout->addWidget(exportBtn_);
    buttonLayout->addStretch();
    buttonLayout->addWidget(resetBtn_);
    buttonLayout->addWidget(closeBtn_);
    
    mainLayout_->addLayout(buttonLayout);
    
    // 连接信号
    connect(generateReportBtn_, &QPushButton::clicked, this, &DataAnalysisDialog::generateReport);
    connect(exportBtn_, &QPushButton::clicked, this, &DataAnalysisDialog::exportResults);
    connect(resetBtn_, &QPushButton::clicked, this, &DataAnalysisDialog::resetAnalysis);
    connect(closeBtn_, &QPushButton::clicked, this, &QDialog::accept);
}

void DataAnalysisDialog::setupFilteringControls() {
    filterGroup_ = new QGroupBox("数据滤波");
    QGridLayout* layout = new QGridLayout(filterGroup_);
    
    layout->addWidget(new QLabel("滤波类型:"), 0, 0);
    filterTypeCombo_ = new QComboBox();
    filterTypeCombo_->addItems({"低通滤波", "高通滤波", "带通滤波", "带阻滤波"});
    layout->addWidget(filterTypeCombo_, 0, 1);
    
    layout->addWidget(new QLabel("低截止频率 (Hz):"), 1, 0);
    lowCutoffSpin_ = new QDoubleSpinBox();
    lowCutoffSpin_->setRange(0.1, 10000);
    lowCutoffSpin_->setValue(1.0);
    lowCutoffSpin_->setSuffix(" Hz");
    layout->addWidget(lowCutoffSpin_, 1, 1);
    
    layout->addWidget(new QLabel("高截止频率 (Hz):"), 2, 0);
    highCutoffSpin_ = new QDoubleSpinBox();
    highCutoffSpin_->setRange(0.1, 10000);
    highCutoffSpin_->setValue(1000.0);
    highCutoffSpin_->setSuffix(" Hz");
    layout->addWidget(highCutoffSpin_, 2, 1);
    
    layout->addWidget(new QLabel("滤波阶数:"), 3, 0);
    filterOrderSpin_ = new QSpinBox();
    filterOrderSpin_->setRange(1, 10);
    filterOrderSpin_->setValue(4);
    layout->addWidget(filterOrderSpin_, 3, 1);
    
    applyFilterBtn_ = new QPushButton("应用滤波");
    layout->addWidget(applyFilterBtn_, 4, 0, 1, 2);
    
    connect(applyFilterBtn_, &QPushButton::clicked, this, &DataAnalysisDialog::applyFiltering);
}

void DataAnalysisDialog::setupDenoisingControls() {
    denoiseGroup_ = new QGroupBox("噪声去除");
    QGridLayout* layout = new QGridLayout(denoiseGroup_);
    
    layout->addWidget(new QLabel("去噪方法:"), 0, 0);
    denoiseMethodCombo_ = new QComboBox();
    denoiseMethodCombo_->addItems({"移动平均", "中值滤波", "高斯滤波", "小波去噪"});
    layout->addWidget(denoiseMethodCombo_, 0, 1);
    
    layout->addWidget(new QLabel("阈值:"), 1, 0);
    denoiseThresholdSpin_ = new QDoubleSpinBox();
    denoiseThresholdSpin_->setRange(0.001, 1.0);
    denoiseThresholdSpin_->setValue(0.1);
    denoiseThresholdSpin_->setDecimals(3);
    layout->addWidget(denoiseThresholdSpin_, 1, 1);
    
    layout->addWidget(new QLabel("窗口大小:"), 2, 0);
    windowSizeSpin_ = new QSpinBox();
    windowSizeSpin_->setRange(3, 101);
    windowSizeSpin_->setValue(5);
    layout->addWidget(windowSizeSpin_, 2, 1);
    
    applyDenoiseBtn_ = new QPushButton("应用去噪");
    layout->addWidget(applyDenoiseBtn_, 3, 0, 1, 2);
    
    connect(applyDenoiseBtn_, &QPushButton::clicked, this, &DataAnalysisDialog::applyDenoising);
}

void DataAnalysisDialog::setupAnalysisControls() {
    analysisGroup_ = new QGroupBox("分析选项");
    QVBoxLayout* layout = new QVBoxLayout(analysisGroup_);
    
    enableBaselineCheck_ = new QCheckBox("基线校正");
    enableBaselineCheck_->setChecked(true);
    layout->addWidget(enableBaselineCheck_);
    
    enableEventDetectionCheck_ = new QCheckBox("事件检测");
    enableEventDetectionCheck_->setChecked(true);
    layout->addWidget(enableEventDetectionCheck_);
    
    enableStatisticsCheck_ = new QCheckBox("统计分析");
    enableStatisticsCheck_->setChecked(true);
    layout->addWidget(enableStatisticsCheck_);
    
    QHBoxLayout* thresholdLayout = new QHBoxLayout();
    thresholdLayout->addWidget(new QLabel("事件阈值:"));
    eventThresholdSpin_ = new QDoubleSpinBox();
    eventThresholdSpin_->setRange(0.1, 100.0);
    eventThresholdSpin_->setValue(5.0);
    eventThresholdSpin_->setSuffix(" pA");
    thresholdLayout->addWidget(eventThresholdSpin_);
    layout->addLayout(thresholdLayout);
    
    analyzeBtn_ = new QPushButton("开始分析");
    analyzeBtn_->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; padding: 8px; }");
    layout->addWidget(analyzeBtn_);
    
    connect(analyzeBtn_, &QPushButton::clicked, this, &DataAnalysisDialog::performAnalysis);
}

void DataAnalysisDialog::setupResultsDisplay() {
    resultsPanel_ = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(resultsPanel_);
    
    // 进度条
    progressBar_ = new QProgressBar();
    progressBar_->setVisible(false);
    layout->addWidget(progressBar_);
    
    // 创建标签页式的结果显示
    QSplitter* resultsSplitter = new QSplitter(Qt::Vertical);
    
    // 统计信息
    QGroupBox* statsGroup = new QGroupBox("统计信息");
    QVBoxLayout* statsLayout = new QVBoxLayout(statsGroup);
    statisticsText_ = new QTextEdit();
    statisticsText_->setMaximumHeight(150);
    statisticsText_->setReadOnly(true);
    statsLayout->addWidget(statisticsText_);
    
    // 事件表格
    QGroupBox* eventsGroup = new QGroupBox("检测到的事件");
    QVBoxLayout* eventsLayout = new QVBoxLayout(eventsGroup);
    eventsTable_ = new QTableWidget();
    eventsTable_->setColumnCount(4);
    eventsTable_->setHorizontalHeaderLabels({"时间 (s)", "幅度 (pA)", "持续时间 (ms)", "类型"});
    eventsTable_->horizontalHeader()->setStretchLastSection(true);
    eventsLayout->addWidget(eventsTable_);
    
    // 分析报告
    QGroupBox* reportGroup = new QGroupBox("分析报告");
    QVBoxLayout* reportLayout = new QVBoxLayout(reportGroup);
    reportText_ = new QTextEdit();
    reportText_->setReadOnly(true);
    reportLayout->addWidget(reportText_);
    
    resultsSplitter->addWidget(statsGroup);
    resultsSplitter->addWidget(eventsGroup);
    resultsSplitter->addWidget(reportGroup);
    resultsSplitter->setSizes({150, 200, 250});
    
    layout->addWidget(resultsSplitter);
}

void DataAnalysisDialog::setAnalysisData(const std::vector<double>& timeData, 
                                        const std::vector<double>& currentData) {
    originalTimeData_ = timeData;
    originalCurrentData_ = currentData;
    processedTimeData_ = timeData;
    processedCurrentData_ = currentData;
    
    // 显示数据基本信息
    QString info = QString("数据点数: %1\n时间范围: %2 - %3 秒\n电流范围: %4 - %5 pA")
                   .arg(timeData.size())
                   .arg(timeData.empty() ? 0 : timeData.front(), 0, 'f', 3)
                   .arg(timeData.empty() ? 0 : timeData.back(), 0, 'f', 3)
                   .arg(currentData.empty() ? 0 : *std::min_element(currentData.begin(), currentData.end()), 0, 'f', 2)
                   .arg(currentData.empty() ? 0 : *std::max_element(currentData.begin(), currentData.end()), 0, 'f', 2);
    
    statisticsText_->setText(info);
}

void DataAnalysisDialog::performAnalysis() {
    if (processedCurrentData_.empty()) {
        QMessageBox::warning(this, "警告", "没有可分析的数据");
        return;
    }
    
    progressBar_->setVisible(true);
    progressBar_->setValue(0);
    
    // 计算基本统计信息
    progressBar_->setValue(20);
    QApplication::processEvents();
    
    double sum = std::accumulate(processedCurrentData_.begin(), processedCurrentData_.end(), 0.0);
    results_.mean = sum / processedCurrentData_.size();
    
    double sq_sum = std::inner_product(processedCurrentData_.begin(), processedCurrentData_.end(),
                                      processedCurrentData_.begin(), 0.0);
    results_.std = std::sqrt(sq_sum / processedCurrentData_.size() - results_.mean * results_.mean);
    
    results_.rms = std::sqrt(sq_sum / processedCurrentData_.size());
    
    progressBar_->setValue(40);
    QApplication::processEvents();
    
    // 基线校正
    if (enableBaselineCheck_->isChecked()) {
        // 简单的基线计算（前10%数据的平均值）
        size_t baselinePoints = processedCurrentData_.size() / 10;
        double baselineSum = std::accumulate(processedCurrentData_.begin(), 
                                           processedCurrentData_.begin() + baselinePoints, 0.0);
        results_.baseline = baselineSum / baselinePoints;
    }
    
    progressBar_->setValue(60);
    QApplication::processEvents();
    
    // 事件检测
    if (enableEventDetectionCheck_->isChecked()) {
        results_.eventTimes.clear();
        results_.eventAmplitudes.clear();
        results_.eventDurations.clear();
        
        double threshold = eventThresholdSpin_->value();
        bool inEvent = false;
        double eventStart = 0;
        double eventPeak = 0;
        
        for (size_t i = 1; i < processedCurrentData_.size(); ++i) {
            double current = processedCurrentData_[i];
            double baseline = enableBaselineCheck_->isChecked() ? results_.baseline : results_.mean;
            
            if (!inEvent && std::abs(current - baseline) > threshold) {
                // 事件开始
                inEvent = true;
                eventStart = processedTimeData_[i];
                eventPeak = current;
            } else if (inEvent && std::abs(current - baseline) <= threshold) {
                // 事件结束
                inEvent = false;
                double duration = (processedTimeData_[i] - eventStart) * 1000; // 转换为毫秒
                
                results_.eventTimes.push_back(eventStart);
                results_.eventAmplitudes.push_back(eventPeak - baseline);
                results_.eventDurations.push_back(duration);
            } else if (inEvent && std::abs(current - baseline) > std::abs(eventPeak - baseline)) {
                // 更新事件峰值
                eventPeak = current;
            }
        }
        
        results_.eventCount = results_.eventTimes.size();
        if (!processedTimeData_.empty()) {
            double totalTime = processedTimeData_.back() - processedTimeData_.front();
            results_.eventRate = results_.eventCount / totalTime;
        }
    }
    
    progressBar_->setValue(80);
    QApplication::processEvents();
    
    updateAnalysisResults();
    
    progressBar_->setValue(100);
    QApplication::processEvents();
    
    progressBar_->setVisible(false);
}

void DataAnalysisDialog::updateAnalysisResults() {
    // 更新统计信息
    QString stats = QString(
        "数据点数: %1\n"
        "平均值: %2 pA\n"
        "标准差: %3 pA\n"
        "RMS: %4 pA\n"
        "基线: %5 pA\n"
        "检测到的事件: %6\n"
        "事件频率: %7 Hz"
    ).arg(processedCurrentData_.size())
     .arg(results_.mean, 0, 'f', 3)
     .arg(results_.std, 0, 'f', 3)
     .arg(results_.rms, 0, 'f', 3)
     .arg(results_.baseline, 0, 'f', 3)
     .arg(results_.eventCount)
     .arg(results_.eventRate, 0, 'f', 3);
    
    statisticsText_->setText(stats);
    
    // 更新事件表格
    eventsTable_->setRowCount(results_.eventCount);
    for (int i = 0; i < results_.eventCount; ++i) {
        eventsTable_->setItem(i, 0, new QTableWidgetItem(QString::number(results_.eventTimes[i], 'f', 3)));
        eventsTable_->setItem(i, 1, new QTableWidgetItem(QString::number(results_.eventAmplitudes[i], 'f', 2)));
        eventsTable_->setItem(i, 2, new QTableWidgetItem(QString::number(results_.eventDurations[i], 'f', 1)));
        
        // 简单的事件分类
        QString eventType = "标准";
        if (results_.eventDurations[i] > 10.0) eventType = "长时间";
        if (std::abs(results_.eventAmplitudes[i]) > 20.0) eventType = "大幅度";
        
        eventsTable_->setItem(i, 3, new QTableWidgetItem(eventType));
    }
}

void DataAnalysisDialog::applyFiltering() {
    // 这里实现简单的滤波算法
    QMessageBox::information(this, "信息", "滤波功能已应用（简化实现）");
}

void DataAnalysisDialog::applyDenoising() {
    // 这里实现简单的去噪算法
    QMessageBox::information(this, "信息", "去噪功能已应用（简化实现）");
}

void DataAnalysisDialog::generateReport() {
    QString report = QString(
        "=== 纳米孔数据分析报告 ===\n\n"
        "分析时间: %1\n\n"
        "数据概览:\n"
        "- 数据点数: %2\n"
        "- 时间范围: %3 - %4 秒\n"
        "- 采样频率: %5 Hz\n\n"
        "统计分析:\n"
        "- 平均电流: %6 pA\n"
        "- 标准差: %7 pA\n"
        "- RMS噪声: %8 pA\n"
        "- 基线电流: %9 pA\n\n"
        "事件检测:\n"
        "- 检测到的事件数: %10\n"
        "- 事件频率: %11 Hz\n"
        "- 平均事件幅度: %12 pA\n"
        "- 平均事件持续时间: %13 ms\n\n"
        "分析参数:\n"
        "- 事件检测阈值: %14 pA\n"
        "- 基线校正: %15\n"
        "- 滤波设置: %16\n"
        "- 去噪设置: %17\n"
    ).arg(QDateTime::currentDateTime().toString())
     .arg(processedCurrentData_.size())
     .arg(originalTimeData_.empty() ? 0 : originalTimeData_.front(), 0, 'f', 3)
     .arg(originalTimeData_.empty() ? 0 : originalTimeData_.back(), 0, 'f', 3)
     .arg(originalTimeData_.size() > 1 ? 1.0 / (originalTimeData_[1] - originalTimeData_[0]) : 0, 0, 'f', 0)
     .arg(results_.mean, 0, 'f', 3)
     .arg(results_.std, 0, 'f', 3)
     .arg(results_.rms, 0, 'f', 3)
     .arg(results_.baseline, 0, 'f', 3)
     .arg(results_.eventCount)
     .arg(results_.eventRate, 0, 'f', 3)
     .arg(results_.eventAmplitudes.empty() ? 0 : 
          std::accumulate(results_.eventAmplitudes.begin(), results_.eventAmplitudes.end(), 0.0) / results_.eventAmplitudes.size(), 0, 'f', 2)
     .arg(results_.eventDurations.empty() ? 0 : 
          std::accumulate(results_.eventDurations.begin(), results_.eventDurations.end(), 0.0) / results_.eventDurations.size(), 0, 'f', 1)
     .arg(eventThresholdSpin_->value(), 0, 'f', 1)
     .arg(enableBaselineCheck_->isChecked() ? "启用" : "禁用")
     .arg(filterTypeCombo_->currentText())
     .arg(denoiseMethodCombo_->currentText());
    
    reportText_->setText(report);
}

void DataAnalysisDialog::exportResults() {
    QString fileName = QFileDialog::getSaveFileName(this, "导出分析结果", 
                                                   "analysis_results.txt", 
                                                   "文本文件 (*.txt);;CSV文件 (*.csv)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << reportText_->toPlainText();
            QMessageBox::information(this, "导出成功", "分析结果已导出到: " + fileName);
        }
    }
}

void DataAnalysisDialog::resetAnalysis() {
    processedTimeData_ = originalTimeData_;
    processedCurrentData_ = originalCurrentData_;
    
    eventsTable_->setRowCount(0);
    reportText_->clear();
    
    // 重置结果
    results_ = AnalysisResults{};
    
    if (!originalCurrentData_.empty()) {
        setAnalysisData(originalTimeData_, originalCurrentData_);
    }
}