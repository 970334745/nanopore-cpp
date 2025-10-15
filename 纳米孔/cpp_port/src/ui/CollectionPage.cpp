#include "CollectionPage.h"
#include "DataAnalysisDialog.h"
#include "DeviceManagementDialog.h"
#include "DataExportDialog.h"
#include "../hardware/Er4Device.h"
#include "../hardware/DeviceInterface.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QFileDialog>
#include <QString>
#include <QDir>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QProgressBar>
#include <QTextEdit>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardPaths>
#include <QApplication>
#include <QProcess>
#include <QMimeData>
#include <QClipboard>
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <sstream>
#include <QSplitter>
#include <QScrollArea>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>
#include <QPainter>
#include <QPixmap>
#include <limits>

CollectionPage::CollectionPage(QWidget* parent) : QWidget(parent) {
    // 初始化数据采集相关成员
    dataTimer_ = new QTimer(this);
    dataFile_ = nullptr;
    dataStream_ = nullptr;
    dataPointCount_ = 0;
    startTime_ = 0.0;
    
    // 初始化实时数据分析变量
    currentBaseline_ = 0.0;
    currentRmsNoise_ = 0.0;
    
    // 初始化数据导出和分析UI组件
    exportGroup_ = nullptr;
    exportRawDataBtn_ = nullptr;
    exportEventsBtn_ = nullptr;
    exportStatisticsBtn_ = nullptr;
    exportReportBtn_ = nullptr;
    exportFormatCombo_ = nullptr;
    includeMetadataCheck_ = nullptr;
    compressDataCheck_ = nullptr;
    
    analysisGroup_ = nullptr;
    samplingRateSpinBox_ = nullptr;
    filterCutoffSpinBox_ = nullptr;
    movingAverageWindowSpinBox_ = nullptr;
    enableFilteringCheck_ = nullptr;
    enableDenoisingCheck_ = nullptr;
    analyzeDataBtn_ = nullptr;
    generateReportBtn_ = nullptr;
    
    // 初始化统计数据
    currentStats_ = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, static_cast<size_t>(0), 0.0, 0.0, 0.0};
    
    // 初始化图表组件
    mainSplitter_ = nullptr;
    realTimeChart_ = nullptr;
    chart_ = nullptr;
    currentSeries_ = nullptr;
    baselineSeries_ = nullptr;
    eventSeries_ = nullptr;
    timeAxis_ = nullptr;
    currentAxis_ = nullptr;
    statisticsChart_ = nullptr;
    statsChart_ = nullptr;
    meanSeries_ = nullptr;
    rmsNoiseSeries_ = nullptr;
    eventRateSeries_ = nullptr;
    
    // 初始化设备管理
    device_ = std::make_unique<Er4Device>();
    
    connect(dataTimer_, &QTimer::timeout, this, &CollectionPage::collectData);
    
    // 创建滚动区域
    auto* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    // 创建内容容器
    auto* contentWidget = new QWidget();
    auto* mainLayout = new QVBoxLayout(contentWidget);

    // 顶部状态：连接状态 + 数据存储
    auto* header = new QHBoxLayout();
    header->addWidget(new QLabel(QStringLiteral("连接状态"), contentWidget));
    connIndicator_ = new QLabel(contentWidget);
    connIndicator_->setFixedSize(16,16);
    connIndicator_->setStyleSheet("background:#C62828;border-radius:8px;");
    header->addWidget(connIndicator_);
    header->addSpacing(30);
    header->addWidget(new QLabel(QStringLiteral("数据存储"), contentWidget));
    storageIndicator_ = new QLabel(contentWidget);
    storageIndicator_->setFixedSize(16,16);
    storageIndicator_->setStyleSheet("background:#C62828;border-radius:8px;");
    header->addWidget(storageIndicator_);
    mainLayout->addLayout(header);

    // 设备与保存路径
    auto* row1 = new QHBoxLayout();
    refreshBtn_ = new QPushButton(QStringLiteral("获取设备序列号"), contentWidget);
    deviceCombo_ = new QComboBox(contentWidget);
    savePathBtn_ = new QPushButton(QStringLiteral("保存路径"), contentWidget);
    row1->addWidget(refreshBtn_);
    row1->addWidget(deviceCombo_, 1);
    row1->addWidget(savePathBtn_);
    mainLayout->addLayout(row1);

    // 连接/断连与开始按钮
    auto* row2 = new QHBoxLayout();
    connectBtn_ = new QPushButton(QStringLiteral("连接"), contentWidget);
    disconnectBtn_ = new QPushButton(QStringLiteral("断开连接"), contentWidget);
    startBtn_ = new QPushButton(QStringLiteral("▶"), contentWidget);
    startBtn_->setFixedSize(60,40);
    startBtn_->setStyleSheet("background:#2E7D32;color:white;font-size:22px;border-radius:6px;");
    row2->addWidget(connectBtn_);
    row2->addWidget(disconnectBtn_);
    row2->addStretch();
    row2->addWidget(startBtn_);
    mainLayout->addLayout(row2);

    statusLabel_ = new QLabel(QStringLiteral("未连接"), contentWidget);
    mainLayout->addWidget(statusLabel_);
    
    // 创建折叠控件按钮
    toggleControlsBtn_ = new QPushButton(QStringLiteral("▼ 显示/隐藏高级控件"), contentWidget);
    toggleControlsBtn_->setStyleSheet("background:#1976D2;color:white;font-size:14px;padding:8px;border-radius:4px;");
    mainLayout->addWidget(toggleControlsBtn_);
    
    // 创建控件容器
    controlsContainer_ = new QWidget(contentWidget);
    mainLayout->addWidget(controlsContainer_);

    connect(refreshBtn_, &QPushButton::clicked, this, [this]{ refreshDevices(); });
    connect(savePathBtn_, &QPushButton::clicked, this, [this]{ chooseSavePath(); });
    connect(connectBtn_, &QPushButton::clicked, this, [this]{ connectDevice(); });
    connect(disconnectBtn_, &QPushButton::clicked, this, [this]{ disconnectDevice(); });
    connect(startBtn_, &QPushButton::clicked, this, [this]{ startAcquisition(); });
    connect(toggleControlsBtn_, &QPushButton::clicked, this, &CollectionPage::toggleControlsVisibility);

    // 设置滚动区域
    scrollArea->setWidget(contentWidget);
    
    // 设置页面的主布局
    auto* pageLayout = new QVBoxLayout(this);
    pageLayout->setContentsMargins(0, 0, 0, 0);
    pageLayout->addWidget(scrollArea);
    setLayout(pageLayout);
    
    // 设置高级功能UI
    setupAdvancedUI();
    
    // 初始化图表可视化
    setupVisualization();
}

void CollectionPage::refreshDevices() {
    updateDeviceInfo();
    statusLabel_->setText(QStringLiteral("已获取设备序列号"));
    logDeviceMessage(QStringLiteral("刷新设备列表"));
}

void CollectionPage::chooseSavePath() {
    QString dir = QFileDialog::getExistingDirectory(this, QStringLiteral("选择保存路径"), saveDir_.isEmpty() ? QDir::currentPath() : saveDir_);
    if (dir.isEmpty()) return;
    saveDir_ = dir;
    statusLabel_->setText(QStringLiteral("保存路径: ") + dir);
}

void CollectionPage::connectDevice() {
    if (!device_) {
        QMessageBox::warning(this, QStringLiteral("错误"), QStringLiteral("设备接口未初始化"));
        return;
    }
    
    int currentIndex = deviceCombo_->currentIndex();
    if (currentIndex < 0 || currentIndex >= static_cast<int>(availableDevices_.size())) {
        QMessageBox::warning(this, QStringLiteral("错误"), QStringLiteral("请选择有效的设备"));
        return;
    }
    
    const DeviceInfo& selectedDevice = availableDevices_[currentIndex];
    bool success = device_->connect(selectedDevice.id);
    
    if (success) {
        connected_ = true;
        onDeviceConnected();
        statusLabel_->setText(QStringLiteral("已连接设备"));
        updateIndicators();
        connectBtn_->setEnabled(false);
        disconnectBtn_->setEnabled(true);
    } else {
        QString error = QString::fromStdString(device_->getLastError());
        QMessageBox::critical(this, QStringLiteral("连接失败"), 
                             QStringLiteral("无法连接到设备: ") + error);
        logDeviceMessage(QStringLiteral("连接失败: ") + error);
    }
}

void CollectionPage::disconnectDevice() {
    if (device_) {
        device_->disconnect();
        onDeviceDisconnected();
    }
    
    connected_ = false;
    storing_ = false;
    statusLabel_->setText(QStringLiteral("已断开设备"));
    updateIndicators();
    connectBtn_->setEnabled(true);
    disconnectBtn_->setEnabled(false);
}

void CollectionPage::startAcquisition() {
    if (!connected_ || !device_ || !device_->isConnected()) {
        statusLabel_->setText(QStringLiteral("请先连接设备"));
        return;
    }
    
    if (saveDir_.isEmpty()) {
        statusLabel_->setText(QStringLiteral("请选择保存路径"));
        return;
    }
    
    if (storing_) {
        // 如果正在采集，则停止
        stopAcquisition();
        return;
    }
    
    // 启动设备数据采集
    if (!device_->startAcquisition()) {
        QString error = QString::fromStdString(device_->getLastError());
        QMessageBox::critical(this, QStringLiteral("采集失败"), 
                             QStringLiteral("无法启动数据采集: ") + error);
        logDeviceMessage(QStringLiteral("采集启动失败: ") + error);
        return;
    }
    
    // 开始数据采集
    storing_ = true;
    dataPointCount_ = 0;
    timeData_.clear();
    currentData_.clear();
    startTime_ = QDateTime::currentMSecsSinceEpoch() / 1000.0;
    
    // 重置图表数据
    resetChartData();
    
    // 创建数据文件
    QString fileName = QString("nanopore_data_%1.txt").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    QString filePath = saveDir_ + "/" + fileName;
    
    dataFile_ = new QFile(filePath, this);
    if (dataFile_->open(QIODevice::WriteOnly | QIODevice::Text)) {
        dataStream_ = new QTextStream(dataFile_);
        *dataStream_ << "Time(s)\tCurrent(nA)\n";  // 写入表头
        
        // 开始定时采集数据（每10ms采集一次）
        dataTimer_->start(10);
        
        startBtn_->setText(QStringLiteral("⏹"));
        startBtn_->setStyleSheet("background:#C62828;color:white;font-size:22px;border-radius:6px;");
        statusLabel_->setText(QStringLiteral("正在采集数据，保存到: ") + filePath);
        logDeviceMessage(QStringLiteral("开始数据采集"));
    } else {
        statusLabel_->setText(QStringLiteral("无法创建数据文件"));
        device_->stopAcquisition();
        storing_ = false;
    }
    
    updateIndicators();
}

void CollectionPage::stopAcquisition() {
    if (!storing_) return;
    
    // 停止设备数据采集
    if (device_ && device_->isConnected()) {
        device_->stopAcquisition();
    }
    
    // 停止数据采集
    dataTimer_->stop();
    storing_ = false;
    
    // 关闭文件
    if (dataStream_) {
        delete dataStream_;
        dataStream_ = nullptr;
    }
    if (dataFile_) {
        dataFile_->close();
        delete dataFile_;
        dataFile_ = nullptr;
    }
    
    // 恢复按钮状态
    startBtn_->setText(QStringLiteral("▶"));
    startBtn_->setStyleSheet("background:#2E7D32;color:white;font-size:22px;border-radius:6px;");
    statusLabel_->setText(QStringLiteral("数据采集已停止，共采集 ") + QString::number(dataPointCount_) + QStringLiteral(" 个数据点"));
    logDeviceMessage(QStringLiteral("停止数据采集"));
    
    updateIndicators();
}

void CollectionPage::collectData() {
    if (!storing_ || !dataStream_ || !device_ || !device_->isConnected()) return;
    
    // 从设备读取数据
    std::vector<double> samples = device_->readData();
    if (samples.empty()) {
        // 如果没有数据，检查是否有错误
        std::string error = device_->getLastError();
        if (!error.empty()) {
            logDeviceMessage(QStringLiteral("数据读取错误: ") + QString::fromStdString(error));
        }
        return;
    }
    
    double currentTime = QDateTime::currentMSecsSinceEpoch() / 1000.0 - startTime_;
    
    // 处理每个采样点
    for (double current : samples) {
        // 保存数据到内存
        timeData_.push_back(currentTime);
        currentData_.push_back(current);
        dataPointCount_++;
        
        // 写入文件
        *dataStream_ << QString::number(currentTime, 'f', 6) << "\t" 
                     << QString::number(current, 'f', 6) << "\n";
        
        // 更新实时图表
        updateRealTimeChart(currentTime, current);
        
        // 更新时间（假设采样间隔为10ms）
        currentTime += 0.01;
    }
    
    // 每100个数据点刷新一次文件和统计图表
    if (dataPointCount_ % 100 == 0) {
        dataStream_->flush();
        statusLabel_->setText(QStringLiteral("正在采集数据，已采集 ") + QString::number(dataPointCount_) + QStringLiteral(" 个数据点"));
        
        // 更新统计图表
        updateStatisticsChart();
    }
    
    // 执行基线补偿
    performBaselineCompensation();
    
    // 执行事件检测
    detectEvents();
}

void CollectionPage::updateIndicators() {
    connIndicator_->setStyleSheet(QStringLiteral("background:%1;border-radius:8px;")
        .arg(connected_ ? "#2E7D32" : "#C62828"));
    storageIndicator_->setStyleSheet(QStringLiteral("background:%1;border-radius:8px;")
        .arg(storing_ ? "#2E7D32" : "#C62828"));
}

void CollectionPage::setupAdvancedUI() {
    // 为控件容器创建布局
    auto* containerLayout = new QVBoxLayout(controlsContainer_);
    
    // 创建高级功能区域
    QHBoxLayout* advancedLayout = new QHBoxLayout();
    
    // 左侧：偏置电压和纳米孔径计算器
    QVBoxLayout* leftLayout = new QVBoxLayout();
    
    // 偏置电压控制组
    QGroupBox* biasGroup = new QGroupBox(QStringLiteral("偏置电压控制"));
    QGridLayout* biasLayout = new QGridLayout(biasGroup);
    
    biasLayout->addWidget(new QLabel(QStringLiteral("偏置电压 (mV):")), 0, 0);
    biasVoltageEdit_ = new QLineEdit(QStringLiteral("100"));
    biasLayout->addWidget(biasVoltageEdit_, 0, 1);
    
    QPushButton* applyBiasBtn = new QPushButton(QStringLiteral("应用偏置"));
    biasLayout->addWidget(applyBiasBtn, 0, 2);
    connect(applyBiasBtn, &QPushButton::clicked, this, &CollectionPage::applyBiasVoltage);
    
    leftLayout->addWidget(biasGroup);
    
    // 纳米孔径计算器组
    QGroupBox* calculatorGroup = new QGroupBox(QStringLiteral("纳米孔径计算器"));
    QGridLayout* calcLayout = new QGridLayout(calculatorGroup);
    
    calcLayout->addWidget(new QLabel(QStringLiteral("电导 (nS):")), 0, 0);
    conductanceEdit_ = new QLineEdit(QStringLiteral("1.0"));
    calcLayout->addWidget(conductanceEdit_, 0, 1);
    
    calcLayout->addWidget(new QLabel(QStringLiteral("长度 (nm):")), 1, 0);
    lengthEdit_ = new QLineEdit(QStringLiteral("10"));
    calcLayout->addWidget(lengthEdit_, 1, 1);
    
    calcLayout->addWidget(new QLabel(QStringLiteral("电导率 (S/m):")), 2, 0);
    conductivityEdit_ = new QLineEdit(QStringLiteral("10.9"));
    calcLayout->addWidget(conductivityEdit_, 2, 1);
    
    QPushButton* calcBtn = new QPushButton(QStringLiteral("计算孔径"));
    calcLayout->addWidget(calcBtn, 3, 0);
    connect(calcBtn, &QPushButton::clicked, this, &CollectionPage::calculatePoreDiameter);
    
    diameterResultLabel_ = new QLabel(QStringLiteral("孔径: -- nm"));
    calcLayout->addWidget(diameterResultLabel_, 3, 1);
    
    leftLayout->addWidget(calculatorGroup);
    
    // 基线补偿组
    QGroupBox* baselineGroup = new QGroupBox(QStringLiteral("基线补偿"));
    QGridLayout* baselineLayout = new QGridLayout(baselineGroup);
    
    enableBaselineCompensation_ = new QCheckBox(QStringLiteral("启用基线补偿"));
    baselineLayout->addWidget(enableBaselineCompensation_, 0, 0);
    
    QPushButton* calcBaselineBtn = new QPushButton(QStringLiteral("计算基线"));
    baselineLayout->addWidget(calcBaselineBtn, 0, 1);
    connect(calcBaselineBtn, &QPushButton::clicked, this, &CollectionPage::calculateBaseline);
    
    baselineValueLabel_ = new QLabel(QStringLiteral("基线: 0.0 pA"));
    baselineLayout->addWidget(baselineValueLabel_, 1, 0);
    
    rmsNoiseLabel_ = new QLabel(QStringLiteral("RMS噪声: 0.0 pA"));
    baselineLayout->addWidget(rmsNoiseLabel_, 1, 1);
    
    leftLayout->addWidget(baselineGroup);
    
    advancedLayout->addLayout(leftLayout);
    
    // 右侧：事件检测和分析
    QVBoxLayout* rightLayout = new QVBoxLayout();
    
    // 事件检测设置组
    QGroupBox* eventGroup = new QGroupBox(QStringLiteral("事件检测设置"));
    QGridLayout* eventLayout = new QGridLayout(eventGroup);
    
    eventLayout->addWidget(new QLabel(QStringLiteral("阈值 (pA):")), 0, 0);
    thresholdEdit_ = new QLineEdit(QStringLiteral("50"));
    eventLayout->addWidget(thresholdEdit_, 0, 1);
    
    eventLayout->addWidget(new QLabel(QStringLiteral("最小持续时间 (ms):")), 1, 0);
    minDurationEdit_ = new QLineEdit(QStringLiteral("0.1"));
    eventLayout->addWidget(minDurationEdit_, 1, 1);
    
    enableEventDetection_ = new QCheckBox(QStringLiteral("启用事件检测"));
    eventLayout->addWidget(enableEventDetection_, 2, 0);
    
    rightLayout->addWidget(eventGroup);
    
    // 事件列表
    QGroupBox* eventListGroup = new QGroupBox(QStringLiteral("检测到的事件"));
    QVBoxLayout* eventListLayout = new QVBoxLayout(eventListGroup);
    
    eventTable_ = new QTableWidget(0, 4);
    QStringList headers;
    headers << QStringLiteral("时间 (s)") << QStringLiteral("幅度 (pA)") 
            << QStringLiteral("持续时间 (ms)") << QStringLiteral("类型");
    eventTable_->setHorizontalHeaderLabels(headers);
    eventTable_->horizontalHeader()->setStretchLastSection(true);
    eventListLayout->addWidget(eventTable_);
    
    rightLayout->addWidget(eventListGroup);
    
    advancedLayout->addLayout(rightLayout);
    
    // 将高级功能布局添加到容器布局
    containerLayout->addLayout(advancedLayout);
    
    // 设置设备管理UI
    setupDeviceManagement();
    containerLayout->addWidget(deviceStatusGroup_);
    
    // 设置数据导出和分析UI
    setupDataExportUI();
    setupDataAnalysisUI();
    containerLayout->addWidget(exportGroup_);
    containerLayout->addWidget(analysisGroup_);
}

void CollectionPage::applyBiasVoltage() {
    if (!device_ || !device_->isConnected()) {
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("设备未连接"));
        return;
    }
    
    bool ok;
    double voltage = biasVoltageEdit_->text().toDouble(&ok);
    if (!ok) {
        QMessageBox::warning(this, QStringLiteral("错误"), QStringLiteral("无效的电压值"));
        return;
    }
    
    if (device_->setVoltage(voltage)) {
        statusLabel_->setText(QStringLiteral("已设置偏置电压: ") + QString::number(voltage, 'f', 3) + " V");
        logDeviceMessage(QStringLiteral("设置偏置电压: ") + QString::number(voltage, 'f', 3) + " V");
        updateDeviceStatus(); // 更新设备状态显示
    } else {
        QString error = QString::fromStdString(device_->getLastError());
        QMessageBox::critical(this, QStringLiteral("设置失败"), 
                             QStringLiteral("无法设置偏置电压: ") + error);
        logDeviceMessage(QStringLiteral("偏置电压设置失败: ") + error);
    }
}

void CollectionPage::calculatePoreDiameter() {
    bool ok1, ok2, ok3;
    double conductance = conductanceEdit_->text().toDouble(&ok1);
    double length = lengthEdit_->text().toDouble(&ok2);
    double conductivity = conductivityEdit_->text().toDouble(&ok3);
    
    if (ok1 && ok2 && ok3 && conductance > 0 && length > 0 && conductivity > 0) {
        // 使用公式: G = σ * π * d² / (4 * L)
        // 解出 d = sqrt(4 * G * L / (σ * π))
        double diameter = sqrt(4.0 * conductance * 1e-9 * length * 1e-9 / (conductivity * M_PI)) * 1e9;
        diameterResultLabel_->setText(QStringLiteral("孔径: %1 nm").arg(diameter, 0, 'f', 2));
    } else {
        diameterResultLabel_->setText(QStringLiteral("孔径: 输入无效"));
    }
}

void CollectionPage::performBaselineCompensation() {
    if (enableBaselineCompensation_->isChecked() && !currentData_.empty()) {
        // 对当前数据进行基线补偿
        for (size_t i = 0; i < currentData_.size(); ++i) {
            currentData_[i] -= currentBaseline_;
        }
    }
}

void CollectionPage::calculateBaseline() {
    if (currentData_.size() < 100) {
        statusLabel_->setText(QStringLiteral("数据不足，无法计算基线"));
        return;
    }
    
    // 使用改进的基线计算算法
    size_t windowSize = std::min(static_cast<size_t>(2000), currentData_.size());
    size_t startIdx = currentData_.size() - windowSize;
    
    // 创建数据副本用于统计分析
    std::vector<double> recentData(currentData_.begin() + startIdx, currentData_.end());
    
    // 方法1: 使用中位数作为基线（对异常值更鲁棒）
    std::vector<double> sortedData = recentData;
    std::sort(sortedData.begin(), sortedData.end());
    double medianBaseline = sortedData[sortedData.size() / 2];
    
    // 方法2: 使用截断均值（去除极值后的均值）
    double q25 = sortedData[sortedData.size() / 4];
    double q75 = sortedData[3 * sortedData.size() / 4];
    double iqr = q75 - q25;
    double lowerBound = q25 - 1.5 * iqr;
    double upperBound = q75 + 1.5 * iqr;
    
    std::vector<double> trimmedData;
    for (double value : recentData) {
        if (value >= lowerBound && value <= upperBound) {
            trimmedData.push_back(value);
        }
    }
    
    double trimmedMean = 0.0;
    if (!trimmedData.empty()) {
        trimmedMean = std::accumulate(trimmedData.begin(), trimmedData.end(), 0.0) / trimmedData.size();
    } else {
        trimmedMean = medianBaseline;
    }
    
    // 方法3: 使用指数加权移动平均（EWMA）进行平滑
    static double ewmaBaseline = 0.0;
    static bool ewmaInitialized = false;
    
    if (!ewmaInitialized) {
        ewmaBaseline = medianBaseline;
        ewmaInitialized = true;
    } else {
        double alpha = 0.01;  // 平滑因子
        ewmaBaseline = alpha * trimmedMean + (1.0 - alpha) * ewmaBaseline;
    }
    
    // 选择最终基线值（结合多种方法）
    currentBaseline_ = ewmaBaseline;
    
    // 计算更准确的噪声估计
    double noiseSum = 0.0;
    size_t noiseCount = 0;
    
    // 使用MAD (Median Absolute Deviation) 估计噪声
    std::vector<double> deviations;
    for (double value : trimmedData) {
        deviations.push_back(std::abs(value - currentBaseline_));
    }
    
    if (!deviations.empty()) {
        std::sort(deviations.begin(), deviations.end());
        double mad = deviations[deviations.size() / 2];
        currentRmsNoise_ = mad * 1.4826;  // 转换为标准差等效值
    } else {
        // 备用方法：计算标准差
        for (double value : recentData) {
            double diff = value - currentBaseline_;
            noiseSum += diff * diff;
            noiseCount++;
        }
        currentRmsNoise_ = sqrt(noiseSum / noiseCount);
    }
    
    // 基线漂移检测
    static double previousBaseline = currentBaseline_;
    static int stableCount = 0;
    
    double baselineDrift = std::abs(currentBaseline_ - previousBaseline);
    if (baselineDrift < currentRmsNoise_ * 0.5) {
        stableCount++;
    } else {
        stableCount = 0;
    }
    
    // 更新显示
    QString baselineStatus = stableCount > 50 ? QStringLiteral("稳定") : QStringLiteral("漂移");
    baselineValueLabel_->setText(QStringLiteral("基线: %1 pA (%2)")
                                .arg(currentBaseline_ * 1e12, 0, 'f', 2)
                                .arg(baselineStatus));
    rmsNoiseLabel_->setText(QStringLiteral("RMS噪声: %1 pA")
                           .arg(currentRmsNoise_ * 1e12, 0, 'f', 2));
    
    previousBaseline = currentBaseline_;
}

void CollectionPage::detectEvents() {
    if (!enableEventDetection_->isChecked() || currentData_.empty() || currentData_.size() < 10) return;
    
    bool ok1, ok2;
    double threshold = thresholdEdit_->text().toDouble(&ok1);
    double minDuration = minDurationEdit_->text().toDouble(&ok2);
    
    if (!ok1 || !ok2) return;
    
    // 改进的事件检测算法
    static bool inEvent = false;
    static double eventStart = 0.0;
    static double eventMin = 0.0;
    static double eventMax = 0.0;
    static std::vector<double> eventBuffer;
    static int consecutiveAboveThreshold = 0;
    static int consecutiveBelowThreshold = 0;
    
    const int minConsecutivePoints = 3;  // 需要连续3个点才确认事件开始/结束
    const double hysteresisRatio = 0.7;  // 滞后比例，避免噪声触发
    
    double currentValue = currentData_.back();
    double currentTime = timeData_.back();
    
    // 计算动态基线（使用最近100个点的中位数）
    double dynamicBaseline = currentBaseline_;
    if (currentData_.size() >= 100) {
        std::vector<double> recentData(currentData_.end() - 100, currentData_.end());
        std::sort(recentData.begin(), recentData.end());
        dynamicBaseline = recentData[50];  // 中位数
    }
    
    // 相对于动态基线的阈值
    double absoluteThreshold = dynamicBaseline - threshold * 1e-12;  // 转换为安培
    double returnThreshold = dynamicBaseline - threshold * hysteresisRatio * 1e-12;
    
    if (!inEvent) {
        // 检测事件开始
        if (currentValue < absoluteThreshold) {
            consecutiveBelowThreshold++;
            consecutiveAboveThreshold = 0;
            
            if (consecutiveBelowThreshold >= minConsecutivePoints) {
                // 事件开始确认
                inEvent = true;
                eventStart = currentTime - (minConsecutivePoints - 1) * 0.01;  // 回溯到实际开始点
                eventMin = currentValue;
                eventMax = currentValue;
                eventBuffer.clear();
                eventBuffer.push_back(currentValue);
                consecutiveBelowThreshold = 0;
            }
        } else {
            consecutiveBelowThreshold = 0;
        }
    } else {
        // 在事件中
        eventBuffer.push_back(currentValue);
        
        // 更新事件极值
        if (currentValue < eventMin) {
            eventMin = currentValue;
        }
        if (currentValue > eventMax) {
            eventMax = currentValue;
        }
        
        // 检测事件结束
        if (currentValue > returnThreshold) {
            consecutiveAboveThreshold++;
            consecutiveBelowThreshold = 0;
            
            if (consecutiveAboveThreshold >= minConsecutivePoints) {
                // 事件结束确认
                double duration = (currentTime - eventStart) * 1000; // 转换为毫秒
                
                // 验证事件有效性
                bool validEvent = true;
                
                // 检查持续时间
                if (duration < minDuration) {
                    validEvent = false;
                }
                
                // 检查事件深度（相对于基线的变化）
                double eventDepth = std::abs(eventMin - dynamicBaseline);
                if (eventDepth < threshold * 0.5 * 1e-12) {
                    validEvent = false;
                }
                
                // 检查信噪比
                if (eventBuffer.size() > 5) {
                    double eventMean = std::accumulate(eventBuffer.begin(), eventBuffer.end(), 0.0) / eventBuffer.size();
                    double eventStd = 0.0;
                    for (double val : eventBuffer) {
                        eventStd += (val - eventMean) * (val - eventMean);
                    }
                    eventStd = std::sqrt(eventStd / eventBuffer.size());
                    
                    double snr = std::abs(eventMean - dynamicBaseline) / (eventStd + 1e-15);
                    if (snr < 2.0) {  // 信噪比太低
                        validEvent = false;
                    }
                }
                
                if (validEvent) {
                    // 计算事件特征
                    double amplitude = eventMin - dynamicBaseline;
                    addEventToTable(eventStart, amplitude, duration);
                    
                    // 保存事件数据用于统计
                    eventTimes_.push_back(eventStart);
                    eventAmplitudes_.push_back(amplitude);
                    eventDurations_.push_back(duration / 1000.0);  // 转换为秒
                }
                
                // 重置事件状态
                inEvent = false;
                consecutiveAboveThreshold = 0;
                eventBuffer.clear();
            }
        } else {
            consecutiveAboveThreshold = 0;
        }
        
        // 防止事件过长（可能是基线漂移）
        double currentDuration = (currentTime - eventStart) * 1000;
        if (currentDuration > 10000) {  // 超过10秒
            inEvent = false;
            consecutiveAboveThreshold = 0;
            eventBuffer.clear();
        }
    }
}

void CollectionPage::addEventToTable(double time, double amplitude, double duration) {
    int row = eventTable_->rowCount();
    eventTable_->insertRow(row);
    
    eventTable_->setItem(row, 0, new QTableWidgetItem(QString::number(time, 'f', 3)));
    eventTable_->setItem(row, 1, new QTableWidgetItem(QString::number(amplitude * 1e12, 'f', 1)));
    eventTable_->setItem(row, 2, new QTableWidgetItem(QString::number(duration * 1000, 'f', 2)));
    
    QString eventType = (amplitude > 0) ? QStringLiteral("正向事件") : QStringLiteral("负向事件");
    eventTable_->setItem(row, 3, new QTableWidgetItem(eventType));
    
    // 添加事件到散点图
    if (eventSeries_) {
        eventSeries_->append(time, amplitude);
        
        // 保持事件散点图显示最近10秒的事件
        const double timeWindow = 10.0;
        auto eventPoints = eventSeries_->points();
        while (!eventPoints.isEmpty() && eventPoints.first().x() < time - timeWindow) {
            eventSeries_->removePoints(0, 1);
            eventPoints = eventSeries_->points();
        }
    }
    
    // 自动滚动到最新事件
    eventTable_->scrollToBottom();
}

void CollectionPage::setupDeviceManagement() {
    // 设备状态组
    deviceStatusGroup_ = new QGroupBox(QStringLiteral("设备状态"));
    QGridLayout* statusLayout = new QGridLayout(deviceStatusGroup_);
    
    // 设备信息显示
    statusLayout->addWidget(new QLabel(QStringLiteral("序列号:")), 0, 0);
    serialNumberLabel_ = new QLabel(QStringLiteral("未连接"));
    statusLayout->addWidget(serialNumberLabel_, 0, 1);
    
    statusLayout->addWidget(new QLabel(QStringLiteral("固件版本:")), 1, 0);
    firmwareVersionLabel_ = new QLabel(QStringLiteral("未知"));
    statusLayout->addWidget(firmwareVersionLabel_, 1, 1);
    
    statusLayout->addWidget(new QLabel(QStringLiteral("温度:")), 2, 0);
    temperatureLabel_ = new QLabel(QStringLiteral("--°C"));
    statusLayout->addWidget(temperatureLabel_, 2, 1);
    
    statusLayout->addWidget(new QLabel(QStringLiteral("电压:")), 3, 0);
    voltageLabel_ = new QLabel(QStringLiteral("0.000 V"));
    statusLayout->addWidget(voltageLabel_, 3, 1);
    
    statusLayout->addWidget(new QLabel(QStringLiteral("电流:")), 4, 0);
    currentLabel_ = new QLabel(QStringLiteral("0.000 pA"));
    statusLayout->addWidget(currentLabel_, 4, 1);
    
    // 设备操作按钮
    QHBoxLayout* deviceOpsLayout = new QHBoxLayout();
    selfTestBtn_ = new QPushButton(QStringLiteral("自检"));
    calibrateBtn_ = new QPushButton(QStringLiteral("校准"));
    QPushButton* deviceMgmtBtn = new QPushButton(QStringLiteral("设备管理"));
    deviceMgmtBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; padding: 6px; }");
    deviceOpsLayout->addWidget(selfTestBtn_);
    deviceOpsLayout->addWidget(calibrateBtn_);
    deviceOpsLayout->addWidget(deviceMgmtBtn);
    statusLayout->addLayout(deviceOpsLayout, 5, 0, 1, 2);
    
    // 操作进度条
    operationProgress_ = new QProgressBar();
    operationProgress_->setVisible(false);
    statusLayout->addWidget(operationProgress_, 6, 0, 1, 2);
    
    // 设备日志
    deviceLogEdit_ = new QTextEdit();
    deviceLogEdit_->setMaximumHeight(100);
    deviceLogEdit_->setReadOnly(true);
    statusLayout->addWidget(deviceLogEdit_, 7, 0, 1, 2);
    
    // 连接信号
    connect(selfTestBtn_, &QPushButton::clicked, this, &CollectionPage::performSelfTest);
    connect(calibrateBtn_, &QPushButton::clicked, this, &CollectionPage::performCalibration);
    connect(deviceMgmtBtn, &QPushButton::clicked, this, &CollectionPage::openDeviceManagementDialog);
}

void CollectionPage::updateDeviceStatus() {
    if (!device_ || !device_->isConnected()) {
        serialNumberLabel_->setText(QStringLiteral("未连接"));
        firmwareVersionLabel_->setText(QStringLiteral("未知"));
        temperatureLabel_->setText(QStringLiteral("--°C"));
        voltageLabel_->setText(QStringLiteral("0.000 V"));
        currentLabel_->setText(QStringLiteral("0.000 pA"));
        return;
    }
    
    DeviceStatus status = device_->getStatus();
    serialNumberLabel_->setText(QString::fromStdString(device_->getSerialNumber()));
    firmwareVersionLabel_->setText(QString::fromStdString(device_->getFirmwareVersion()));
    temperatureLabel_->setText(QString::number(status.temperature, 'f', 1) + "°C");
    voltageLabel_->setText(QString::number(status.voltage, 'f', 3) + " V");
    currentLabel_->setText(QString::number(status.current * 1e12, 'f', 3) + " pA");
    
    if (!status.lastError.empty()) {
        logDeviceMessage(QString::fromStdString("错误: " + status.lastError));
    }
}

void CollectionPage::performSelfTest() {
    if (!device_ || !device_->isConnected()) {
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("设备未连接"));
        return;
    }
    
    operationProgress_->setVisible(true);
    operationProgress_->setRange(0, 0); // 不确定进度
    selfTestBtn_->setEnabled(false);
    
    logDeviceMessage(QStringLiteral("开始设备自检..."));
    
    bool result = device_->performSelfTest();
    
    operationProgress_->setVisible(false);
    selfTestBtn_->setEnabled(true);
    
    if (result) {
        logDeviceMessage(QStringLiteral("设备自检完成 - 通过"));
        QMessageBox::information(this, QStringLiteral("自检结果"), QStringLiteral("设备自检通过"));
    } else {
        QString error = QString::fromStdString(device_->getLastError());
        logDeviceMessage(QStringLiteral("设备自检失败: ") + error);
        QMessageBox::critical(this, QStringLiteral("自检结果"), QStringLiteral("设备自检失败: ") + error);
    }
}

void CollectionPage::performCalibration() {
    if (!device_ || !device_->isConnected()) {
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("设备未连接"));
        return;
    }
    
    int ret = QMessageBox::question(this, QStringLiteral("确认校准"), 
                                   QStringLiteral("校准过程可能需要几分钟时间，是否继续？"),
                                   QMessageBox::Yes | QMessageBox::No);
    if (ret != QMessageBox::Yes) {
        return;
    }
    
    operationProgress_->setVisible(true);
    operationProgress_->setRange(0, 0);
    calibrateBtn_->setEnabled(false);
    
    logDeviceMessage(QStringLiteral("开始设备校准..."));
    
    bool result = device_->calibrate();
    
    operationProgress_->setVisible(false);
    calibrateBtn_->setEnabled(true);
    
    if (result) {
        logDeviceMessage(QStringLiteral("设备校准完成"));
        QMessageBox::information(this, QStringLiteral("校准结果"), QStringLiteral("设备校准完成"));
    } else {
        QString error = QString::fromStdString(device_->getLastError());
        logDeviceMessage(QStringLiteral("设备校准失败: ") + error);
        QMessageBox::critical(this, QStringLiteral("校准结果"), QStringLiteral("设备校准失败: ") + error);
    }
}

void CollectionPage::logDeviceMessage(const QString& message) {
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    deviceLogEdit_->append(QString("[%1] %2").arg(timestamp, message));
}

void CollectionPage::onDeviceConnected() {
    updateDeviceInfo();
    updateDeviceStatus();
    logDeviceMessage(QStringLiteral("设备已连接"));
}

void CollectionPage::onDeviceDisconnected() {
    updateDeviceStatus();
    logDeviceMessage(QStringLiteral("设备已断开"));
}

void CollectionPage::updateDeviceInfo() {
    if (!device_) return;
    
    availableDevices_ = device_->detect();
    deviceCombo_->clear();
    
    for (const auto& deviceInfo : availableDevices_) {
        QString displayName = QString::fromStdString(deviceInfo.name + " (" + deviceInfo.id + ")");
        deviceCombo_->addItem(displayName);
    }
}

void CollectionPage::setupDataExportUI() {
    exportGroup_ = new QGroupBox(QString::fromUtf8("数据导出"));
    auto* exportLayout = new QVBoxLayout(exportGroup_);
    
    // 导出格式和选项行
    auto* optionsLayout = new QHBoxLayout();
    
    // 导出格式选择
    optionsLayout->addWidget(new QLabel(QString::fromUtf8("导出格式:")));
    exportFormatCombo_ = new QComboBox();
    exportFormatCombo_->addItems({
        QString::fromUtf8("CSV (逗号分隔)"),
        QString::fromUtf8("TSV (制表符分隔)"),
        QString::fromUtf8("JSON (结构化数据)"),
        QString::fromUtf8("HDF5 (科学数据)"),
        QString::fromUtf8("Excel (XLSX)")
    });
    optionsLayout->addWidget(exportFormatCombo_);
    
    // 导出选项
    includeMetadataCheck_ = new QCheckBox(QString::fromUtf8("包含元数据"));
    includeMetadataCheck_->setChecked(true);
    optionsLayout->addWidget(includeMetadataCheck_);
    
    compressDataCheck_ = new QCheckBox(QString::fromUtf8("压缩数据"));
    compressDataCheck_->setChecked(false);
    optionsLayout->addWidget(compressDataCheck_);
    
    optionsLayout->addStretch();
    exportLayout->addLayout(optionsLayout);
    
    // 创建一行紧凑按钮布局
    auto* buttonsLayout = new QHBoxLayout();
    buttonsLayout->setContentsMargins(0, 0, 0, 0);
    buttonsLayout->setSpacing(5);
    
    // 创建五个小按钮
    exportRawDataBtn_ = new QPushButton(QString::fromUtf8("电流分布"));
    exportEventsBtn_ = new QPushButton(QString::fromUtf8("散点图"));
    exportStatisticsBtn_ = new QPushButton(QString::fromUtf8("功率谱"));
    exportReportBtn_ = new QPushButton(QString::fromUtf8("设计分析"));
    auto* advancedExportBtn = new QPushButton(QString::fromUtf8("事件分析"));
    
    // 使用QPalette直接设置按钮颜色，绕过样式表系统
    QPalette greenPalette;
    greenPalette.setColor(QPalette::Button, QColor(76, 175, 80));  // #4CAF50
    greenPalette.setColor(QPalette::ButtonText, QColor(255, 255, 255));  // white
    
    // 设置所有按钮的调色板和大小
    exportRawDataBtn_->setPalette(greenPalette);
    exportRawDataBtn_->setAutoFillBackground(true);
    exportRawDataBtn_->setFixedSize(50, 20);
    
    exportEventsBtn_->setPalette(greenPalette);
    exportEventsBtn_->setAutoFillBackground(true);
    exportEventsBtn_->setFixedSize(50, 20);
    
    exportStatisticsBtn_->setPalette(greenPalette);
    exportStatisticsBtn_->setAutoFillBackground(true);
    exportStatisticsBtn_->setFixedSize(50, 20);
    
    exportReportBtn_->setPalette(greenPalette);
    exportReportBtn_->setAutoFillBackground(true);
    exportReportBtn_->setFixedSize(50, 20);
    
    advancedExportBtn->setPalette(greenPalette);
    advancedExportBtn->setAutoFillBackground(true);
    advancedExportBtn->setFixedSize(50, 20);
    
    // 添加按钮到一行布局
    buttonsLayout->addWidget(exportRawDataBtn_);
    buttonsLayout->addWidget(exportEventsBtn_);
    buttonsLayout->addWidget(exportStatisticsBtn_);
    buttonsLayout->addWidget(exportReportBtn_);
    buttonsLayout->addWidget(advancedExportBtn);
    buttonsLayout->addStretch();  // 剩余空间
    
    exportLayout->addLayout(buttonsLayout);
    
    // 连接信号
    connect(exportRawDataBtn_, &QPushButton::clicked, this, &CollectionPage::exportRawData);
    connect(exportEventsBtn_, &QPushButton::clicked, this, &CollectionPage::exportEventData);
    connect(exportStatisticsBtn_, &QPushButton::clicked, this, &CollectionPage::exportStatistics);
    connect(exportReportBtn_, &QPushButton::clicked, this, &CollectionPage::exportFullReport);
    connect(advancedExportBtn, &QPushButton::clicked, this, &CollectionPage::openDataExportDialog);

    // 强制启用父容器和所有按钮
    exportGroup_->setEnabled(true);
    exportRawDataBtn_->setEnabled(true);
    exportEventsBtn_->setEnabled(true);
    exportStatisticsBtn_->setEnabled(true);
    exportReportBtn_->setEnabled(true);
    advancedExportBtn->setEnabled(true);
}

void CollectionPage::setupDataAnalysisUI() {
    analysisGroup_ = new QGroupBox(QString::fromUtf8("数据分析"));
    auto* analysisLayout = new QGridLayout(analysisGroup_);
    
    // 采样率设置
    analysisLayout->addWidget(new QLabel(QString::fromUtf8("采样率 (Hz):")), 0, 0);
    samplingRateSpinBox_ = new QDoubleSpinBox();
    samplingRateSpinBox_->setRange(0.1, 10000.0);
    samplingRateSpinBox_->setValue(20.0);
    samplingRateSpinBox_->setDecimals(1);
    analysisLayout->addWidget(samplingRateSpinBox_, 0, 1);
    
    // 滤波器截止频率
    analysisLayout->addWidget(new QLabel(QString::fromUtf8("滤波截止频率 (Hz):")), 0, 2);
    filterCutoffSpinBox_ = new QDoubleSpinBox();
    filterCutoffSpinBox_->setRange(0.1, 1000.0);
    filterCutoffSpinBox_->setValue(10.0);
    filterCutoffSpinBox_->setDecimals(1);
    analysisLayout->addWidget(filterCutoffSpinBox_, 0, 3);
    
    // 移动平均窗口
    analysisLayout->addWidget(new QLabel(QString::fromUtf8("移动平均窗口:")), 1, 0);
    movingAverageWindowSpinBox_ = new QSpinBox();
    movingAverageWindowSpinBox_->setRange(1, 1000);
    movingAverageWindowSpinBox_->setValue(50);
    analysisLayout->addWidget(movingAverageWindowSpinBox_, 1, 1);
    
    // 分析选项
    enableFilteringCheck_ = new QCheckBox(QString::fromUtf8("启用数据滤波"));
    enableFilteringCheck_->setChecked(true);
    analysisLayout->addWidget(enableFilteringCheck_, 1, 2);
    
    enableDenoisingCheck_ = new QCheckBox(QString::fromUtf8("启用去噪处理"));
    enableDenoisingCheck_->setChecked(false);
    analysisLayout->addWidget(enableDenoisingCheck_, 1, 3);
    
    // 分析按钮
    analyzeDataBtn_ = new QPushButton(QString::fromUtf8("快速分析"));
    generateReportBtn_ = new QPushButton(QString::fromUtf8("生成报告"));
    QPushButton* advancedAnalysisBtn = new QPushButton(QString::fromUtf8("高级分析"));
    advancedAnalysisBtn->setStyleSheet("QPushButton { background-color: #2196F3; color: white; font-weight: bold; padding: 6px; }");
    
    analysisLayout->addWidget(analyzeDataBtn_, 2, 0);
    analysisLayout->addWidget(generateReportBtn_, 2, 1);
    analysisLayout->addWidget(advancedAnalysisBtn, 2, 2, 1, 2);
    
    // 连接信号
    connect(analyzeDataBtn_, &QPushButton::clicked, this, &CollectionPage::analyzeCurrentData);
    connect(generateReportBtn_, &QPushButton::clicked, this, &CollectionPage::generateAnalysisReport);
    connect(advancedAnalysisBtn, &QPushButton::clicked, this, &CollectionPage::openDataAnalysisDialog);
}

void CollectionPage::exportRawData() {
    if (timeData_.empty() || currentData_.empty()) {
        QMessageBox::warning(this, QString::fromUtf8("警告"), QString::fromUtf8("没有可导出的数据"));
        return;
    }
    
    QString defaultFileName = QString("nanopore_raw_data_%1").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    QString format = exportFormatCombo_->currentText();
    QString filter;
    QString extension;
    
    if (format.contains("CSV")) {
        filter = QString::fromUtf8("CSV文件 (*.csv)");
        extension = ".csv";
    } else if (format.contains("TSV")) {
        filter = QString::fromUtf8("TSV文件 (*.tsv)");
        extension = ".tsv";
    } else if (format.contains("JSON")) {
        filter = QString::fromUtf8("JSON文件 (*.json)");
        extension = ".json";
    } else if (format.contains("Excel")) {
        filter = QString::fromUtf8("Excel文件 (*.xlsx)");
        extension = ".xlsx";
    } else {
        filter = QString::fromUtf8("HDF5文件 (*.h5)");
        extension = ".h5";
    }
    
    QString fileName = QFileDialog::getSaveFileName(this, 
        QString::fromUtf8("导出原始数据"), 
        defaultFileName + extension, 
        filter);
    
    if (fileName.isEmpty()) return;
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, QString::fromUtf8("错误"), QString::fromUtf8("无法创建文件"));
        return;
    }
    
    QTextStream out(&file);
    
    // 写入元数据（如果选择）
    if (includeMetadataCheck_->isChecked()) {
        out << generateMetadata() << "\n";
    }
    
    // 写入数据头
    if (format.contains("CSV")) {
        out << QString::fromUtf8("时间(s),电流(nA)\n");
        for (size_t i = 0; i < timeData_.size() && i < currentData_.size(); ++i) {
            out << QString::number(timeData_[i], 'f', 6) << "," 
                << QString::number(currentData_[i], 'f', 6) << "\n";
        }
    } else if (format.contains("TSV")) {
        out << QString::fromUtf8("时间(s)\t电流(nA)\n");
        for (size_t i = 0; i < timeData_.size() && i < currentData_.size(); ++i) {
            out << QString::number(timeData_[i], 'f', 6) << "\t" 
                << QString::number(currentData_[i], 'f', 6) << "\n";
        }
    } else if (format.contains("JSON")) {
        QJsonObject rootObj;
        QJsonArray timeArray, currentArray;
        
        for (size_t i = 0; i < timeData_.size() && i < currentData_.size(); ++i) {
            timeArray.append(timeData_[i]);
            currentArray.append(currentData_[i]);
        }
        
        rootObj["time"] = timeArray;
        rootObj["current"] = currentArray;
        rootObj["metadata"] = QJsonObject::fromVariantMap(QVariantMap{
            {"export_time", QDateTime::currentDateTime().toString()},
            {"data_points", static_cast<int>(timeData_.size())},
            {"sampling_rate", samplingRateSpinBox_->value()}
        });
        
        QJsonDocument doc(rootObj);
        out << doc.toJson();
    }
    
    file.close();
    
    if (compressDataCheck_->isChecked()) {
        compressExportFile(fileName);
    }
    
    QMessageBox::information(this, QString::fromUtf8("成功"), 
        QString::fromUtf8("原始数据已导出到: ") + fileName);
}

void CollectionPage::exportEventData() {
    if (eventTimes_.empty() || eventAmplitudes_.empty() || eventDurations_.empty()) {
        QMessageBox::warning(this, QString::fromUtf8("警告"), QString::fromUtf8("没有检测到事件数据"));
        return;
    }
    
    QString fileName = QFileDialog::getSaveFileName(this, 
        QString::fromUtf8("导出事件数据"), 
        QString("nanopore_events_%1.csv").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")),
        QString::fromUtf8("CSV文件 (*.csv)"));
    
    if (fileName.isEmpty()) return;
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, QString::fromUtf8("错误"), QString::fromUtf8("无法创建文件"));
        return;
    }
    
    QTextStream out(&file);
    
    if (includeMetadataCheck_->isChecked()) {
        out << generateMetadata() << "\n";
    }
    
    out << QString::fromUtf8("事件时间(s),事件幅度(nA),持续时间(ms),事件类型\n");
    
    size_t minSize = std::min({eventTimes_.size(), eventAmplitudes_.size(), eventDurations_.size()});
    for (size_t i = 0; i < minSize; ++i) {
        QString eventType = (eventAmplitudes_[i] > 1.0) ? QString::fromUtf8("大事件") : QString::fromUtf8("小事件");
        out << QString::number(eventTimes_[i], 'f', 6) << ","
            << QString::number(eventAmplitudes_[i], 'f', 6) << ","
            << QString::number(eventDurations_[i], 'f', 3) << ","
            << eventType << "\n";
    }
    
    file.close();
    QMessageBox::information(this, QString::fromUtf8("成功"), 
        QString::fromUtf8("事件数据已导出到: ") + fileName);
}

void CollectionPage::exportStatistics() {
    DataStatistics stats = calculateDataStatistics();
    
    QString fileName = QFileDialog::getSaveFileName(this, 
        QString::fromUtf8("导出统计数据"), 
        QString("nanopore_statistics_%1.json").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")),
        QString::fromUtf8("JSON文件 (*.json)"));
    
    if (fileName.isEmpty()) return;
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, QString::fromUtf8("错误"), QString::fromUtf8("无法创建文件"));
        return;
    }
    
    QJsonObject statsObj;
    statsObj["mean"] = stats.mean;
    statsObj["std"] = stats.std;
    statsObj["min"] = stats.min;
    statsObj["max"] = stats.max;
    statsObj["median"] = stats.median;
    statsObj["rms"] = stats.rms;
    statsObj["event_count"] = static_cast<int>(stats.eventCount);
    statsObj["event_rate"] = stats.eventRate;
    statsObj["snr"] = stats.snr;
    statsObj["baseline"] = stats.baseline;
    
    QJsonObject rootObj;
    rootObj["statistics"] = statsObj;
    rootObj["metadata"] = QJsonObject::fromVariantMap(QVariantMap{
        {"export_time", QDateTime::currentDateTime().toString()},
        {"data_points", static_cast<int>(currentData_.size())},
        {"analysis_duration", QString::number(timeData_.empty() ? 0 : timeData_.back(), 'f', 3) + "s"}
    });
    
    QJsonDocument doc(rootObj);
    QTextStream out(&file);
    out << doc.toJson();
    file.close();
    
    QMessageBox::information(this, QString::fromUtf8("成功"), 
        QString::fromUtf8("统计数据已导出到: ") + fileName);
}

void CollectionPage::exportFullReport() {
    QString fileName = QFileDialog::getSaveFileName(this, 
        QString::fromUtf8("导出完整报告"), 
        QString("nanopore_report_%1.html").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")),
        QString::fromUtf8("HTML文件 (*.html)"));
    
    if (fileName.isEmpty()) return;
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, QString::fromUtf8("错误"), QString::fromUtf8("无法创建文件"));
        return;
    }
    
    QTextStream out(&file);
    
    // HTML报告头部
    out << "<!DOCTYPE html>\n<html>\n<head>\n";
    out << "<meta charset=\"UTF-8\">\n";
    out << "<title>" << QString::fromUtf8("纳米孔数据分析报告") << "</title>\n";
    out << "<style>\n";
    out << "body { font-family: Arial, sans-serif; margin: 20px; }\n";
    out << "h1, h2 { color: #333; }\n";
    out << "table { border-collapse: collapse; width: 100%; margin: 10px 0; }\n";
    out << "th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }\n";
    out << "th { background-color: #f2f2f2; }\n";
    out << ".summary { background-color: #f9f9f9; padding: 15px; border-radius: 5px; }\n";
    out << "</style>\n</head>\n<body>\n";
    
    // 报告内容
    out << "<h1>" << QString::fromUtf8("纳米孔数据分析报告") << "</h1>\n";
    out << "<div class=\"summary\">\n";
    out << "<h2>" << QString::fromUtf8("实验概要") << "</h2>\n";
    out << "<p><strong>" << QString::fromUtf8("生成时间：") << "</strong>" << QDateTime::currentDateTime().toString() << "</p>\n";
    out << "<p><strong>" << QString::fromUtf8("数据点数：") << "</strong>" << currentData_.size() << "</p>\n";
    out << "<p><strong>" << QString::fromUtf8("采样时长：") << "</strong>" << (timeData_.empty() ? 0 : timeData_.back()) << " s</p>\n";
    out << "<p><strong>" << QString::fromUtf8("检测事件数：") << "</strong>" << eventTimes_.size() << "</p>\n";
    out << "</div>\n";
    
    // 统计数据表格
    DataStatistics stats = calculateDataStatistics();
    out << "<h2>" << QString::fromUtf8("统计数据") << "</h2>\n";
    out << "<table>\n";
    out << "<tr><th>" << QString::fromUtf8("参数") << "</th><th>" << QString::fromUtf8("数值") << "</th><th>" << QString::fromUtf8("单位") << "</th></tr>\n";
    out << "<tr><td>" << QString::fromUtf8("平均值") << "</td><td>" << QString::number(stats.mean, 'f', 3) << "</td><td>nA</td></tr>\n";
    out << "<tr><td>" << QString::fromUtf8("标准差") << "</td><td>" << QString::number(stats.std, 'f', 3) << "</td><td>nA</td></tr>\n";
    out << "<tr><td>" << QString::fromUtf8("最小值") << "</td><td>" << QString::number(stats.min, 'f', 3) << "</td><td>nA</td></tr>\n";
    out << "<tr><td>" << QString::fromUtf8("最大值") << "</td><td>" << QString::number(stats.max, 'f', 3) << "</td><td>nA</td></tr>\n";
    out << "<tr><td>" << QString::fromUtf8("中位数") << "</td><td>" << QString::number(stats.median, 'f', 3) << "</td><td>nA</td></tr>\n";
    out << "<tr><td>" << QString::fromUtf8("RMS") << "</td><td>" << QString::number(stats.rms, 'f', 3) << "</td><td>nA</td></tr>\n";
    out << "<tr><td>" << QString::fromUtf8("信噪比") << "</td><td>" << QString::number(stats.snr, 'f', 2) << "</td><td>dB</td></tr>\n";
    out << "<tr><td>" << QString::fromUtf8("基线电流") << "</td><td>" << QString::number(stats.baseline, 'f', 3) << "</td><td>nA</td></tr>\n";
    out << "<tr><td>" << QString::fromUtf8("事件频率") << "</td><td>" << QString::number(stats.eventRate, 'f', 2) << "</td><td>Hz</td></tr>\n";
    out << "</table>\n";
    
    out << "</body>\n</html>\n";
    file.close();
    
    QMessageBox::information(this, QString::fromUtf8("成功"), 
        QString::fromUtf8("完整报告已导出到: ") + fileName);
}

QString CollectionPage::generateMetadata() {
    QStringList metadata;
    metadata << QString::fromUtf8("# 纳米孔数据元数据");
    metadata << QString("# Export Time: %1").arg(QDateTime::currentDateTime().toString());
    metadata << QString("# Data Points: %1").arg(currentData_.size());
    metadata << QString("# Sampling Rate: %1 Hz").arg(samplingRateSpinBox_->value());
    metadata << QString("# Duration: %1 s").arg(timeData_.empty() ? 0 : timeData_.back());
    metadata << QString("# Events Detected: %1").arg(eventTimes_.size());
    metadata << QString::fromUtf8("# 软件版本: NanoporeAnalyzer v1.0");
    metadata << "#";
    return metadata.join("\n");
}

void CollectionPage::compressExportFile(const QString& fileName) {
    // 简单的压缩实现（实际项目中可能需要使用专门的压缩库）
    QProcess gzip;
    gzip.start("gzip", QStringList() << fileName);
    gzip.waitForFinished();
    
    if (gzip.exitCode() == 0) {
        QMessageBox::information(this, QString::fromUtf8("压缩完成"), 
            QString::fromUtf8("文件已压缩为: ") + fileName + ".gz");
    }
}

void CollectionPage::analyzeCurrentData() {
    if (currentData_.empty()) {
        QMessageBox::warning(this, QString::fromUtf8("警告"), QString::fromUtf8("没有数据可分析"));
        return;
    }
    
    // 应用数据过滤
    std::vector<double> filteredData = currentData_;
    if (enableFilteringCheck_->isChecked()) {
        filteredData = applyDataFiltering(filteredData);
    }
    
    // 应用去噪
    if (enableDenoisingCheck_->isChecked()) {
        filteredData = applyDenoising(filteredData);
    }
    
    // 计算统计数据
    currentStats_ = calculateDataStatistics();
    
    // 更新状态显示
    QString analysisResult = QString::fromUtf8("数据分析完成:\n");
    analysisResult += QString::fromUtf8("数据点数: %1\n").arg(filteredData.size());
    analysisResult += QString::fromUtf8("平均值: %1 nA\n").arg(QString::number(currentStats_.mean, 'f', 3));
    analysisResult += QString::fromUtf8("标准差: %1 nA\n").arg(QString::number(currentStats_.std, 'f', 3));
    analysisResult += QString::fromUtf8("信噪比: %1 dB\n").arg(QString::number(currentStats_.snr, 'f', 2));
    analysisResult += QString::fromUtf8("检测事件: %1 个\n").arg(currentStats_.eventCount);
    
    QMessageBox::information(this, QString::fromUtf8("分析结果"), analysisResult);
}

void CollectionPage::generateAnalysisReport() {
    if (currentData_.empty()) {
        QMessageBox::warning(this, QString::fromUtf8("警告"), QString::fromUtf8("没有数据可生成报告"));
        return;
    }
    
    QString fileName = QFileDialog::getSaveFileName(this, 
        QString::fromUtf8("生成分析报告"), 
        QString("analysis_report_%1.txt").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")),
        QString::fromUtf8("文本文件 (*.txt)"));
    
    if (fileName.isEmpty()) return;
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, QString::fromUtf8("错误"), QString::fromUtf8("无法创建报告文件"));
        return;
    }
    
    QTextStream out(&file);
    
    // 报告头部
    out << QString::fromUtf8("纳米孔数据分析报告\n");
    out << QString("==================================================") << "\n";
    out << QString::fromUtf8("生成时间: ") << QDateTime::currentDateTime().toString() << "\n";
    out << QString::fromUtf8("分析参数:\n");
    out << QString::fromUtf8("  采样频率: %1 Hz\n").arg(samplingRateSpinBox_->value());
    out << QString::fromUtf8("  滤波截止频率: %1 Hz\n").arg(filterCutoffSpinBox_->value());
    out << QString::fromUtf8("  移动平均窗口: %1\n").arg(movingAverageWindowSpinBox_->value());
    out << QString::fromUtf8("  启用滤波: %1\n").arg(enableFilteringCheck_->isChecked() ? QString::fromUtf8("是") : QString::fromUtf8("否"));
    out << QString::fromUtf8("  启用去噪: %1\n").arg(enableDenoisingCheck_->isChecked() ? QString::fromUtf8("是") : QString::fromUtf8("否"));
    out << "\n";
    
    // 统计数据
    DataStatistics stats = calculateDataStatistics();
    out << QString::fromUtf8("统计数据:\n");
    out << QString::fromUtf8("  数据点数: %1\n").arg(currentData_.size());
    out << QString::fromUtf8("  采样时长: %1 秒\n").arg(timeData_.empty() ? 0 : timeData_.back());
    out << QString::fromUtf8("  平均值: %1 nA\n").arg(QString::number(stats.mean, 'f', 6));
    out << QString::fromUtf8("  标准差: %1 nA\n").arg(QString::number(stats.std, 'f', 6));
    out << QString::fromUtf8("  最小值: %1 nA\n").arg(QString::number(stats.min, 'f', 6));
    out << QString::fromUtf8("  最大值: %1 nA\n").arg(QString::number(stats.max, 'f', 6));
    out << QString::fromUtf8("  中位数: %1 nA\n").arg(QString::number(stats.median, 'f', 6));
    out << QString::fromUtf8("  RMS: %1 nA\n").arg(QString::number(stats.rms, 'f', 6));
    out << QString::fromUtf8("  信噪比: %1 dB\n").arg(QString::number(stats.snr, 'f', 3));
    out << QString::fromUtf8("  基线电流: %1 nA\n").arg(QString::number(stats.baseline, 'f', 6));
    out << "\n";
    
    // 事件分析
    out << QString::fromUtf8("事件分析:\n");
    out << QString::fromUtf8("  检测事件数: %1\n").arg(stats.eventCount);
    out << QString::fromUtf8("  事件频率: %1 Hz\n").arg(QString::number(stats.eventRate, 'f', 3));
    
    if (!eventAmplitudes_.empty()) {
        double avgAmplitude = std::accumulate(eventAmplitudes_.begin(), eventAmplitudes_.end(), 0.0) / eventAmplitudes_.size();
        double avgDuration = std::accumulate(eventDurations_.begin(), eventDurations_.end(), 0.0) / eventDurations_.size();
        out << QString::fromUtf8("  平均事件幅度: %1 nA\n").arg(QString::number(avgAmplitude, 'f', 3));
        out << QString::fromUtf8("  平均事件持续时间: %1 ms\n").arg(QString::number(avgDuration, 'f', 3));
    }
    
    file.close();
    QMessageBox::information(this, QString::fromUtf8("成功"), 
        QString::fromUtf8("分析报告已生成: ") + fileName);
}

CollectionPage::DataStatistics CollectionPage::calculateDataStatistics() {
    DataStatistics stats;
    
    if (currentData_.empty()) {
        return stats; // 返回默认初始化的结构体
    }
    
    size_t n = currentData_.size();
    
    // 计算基本统计量
    stats.min = *std::min_element(currentData_.begin(), currentData_.end());
    stats.max = *std::max_element(currentData_.begin(), currentData_.end());
    stats.mean = std::accumulate(currentData_.begin(), currentData_.end(), 0.0) / n;
    
    // 计算标准差
    double variance = 0.0;
    for (double value : currentData_) {
        variance += (value - stats.mean) * (value - stats.mean);
    }
    stats.std = std::sqrt(variance / n);
    
    // 计算中位数
    std::vector<double> sortedData = currentData_;
    std::sort(sortedData.begin(), sortedData.end());
    if (n % 2 == 0) {
        stats.median = (sortedData[n/2 - 1] + sortedData[n/2]) / 2.0;
    } else {
        stats.median = sortedData[n/2];
    }
    
    // 计算RMS
    double sumSquares = 0.0;
    for (double value : currentData_) {
        sumSquares += value * value;
    }
    stats.rms = std::sqrt(sumSquares / n);
    
    // 计算基线（使用中位数作为基线估计）
    stats.baseline = stats.median;
    
    // 计算信噪比（简化计算）
    double signal = std::abs(stats.mean - stats.baseline);
    double noise = stats.std;
    stats.snr = (noise > 0) ? 20.0 * std::log10(signal / noise) : 0.0;
    
    // 事件统计
    stats.eventCount = eventTimes_.size();
    if (!timeData_.empty() && timeData_.back() > 0) {
        stats.eventRate = static_cast<double>(stats.eventCount) / timeData_.back();
    } else {
        stats.eventRate = 0.0;
    }
    
    return stats;
}

std::vector<double> CollectionPage::applyDataFiltering(const std::vector<double>& data) {
    if (data.empty()) return data;
    
    std::vector<double> filteredData = data;
    double cutoffFreq = filterCutoffSpinBox_->value();
    double samplingRate = samplingRateSpinBox_->value();
    
    // 简单的低通滤波器实现（移动平均）
    int windowSize = static_cast<int>(samplingRate / cutoffFreq);
    if (windowSize < 3) windowSize = 3;
    if (windowSize > static_cast<int>(data.size() / 2)) windowSize = static_cast<int>(data.size() / 2);
    
    std::vector<double> filtered(data.size());
    
    for (size_t i = 0; i < data.size(); ++i) {
        double sum = 0.0;
        int count = 0;
        
        int start = std::max(0, static_cast<int>(i) - windowSize / 2);
        int end = std::min(static_cast<int>(data.size()) - 1, static_cast<int>(i) + windowSize / 2);
        
        for (int j = start; j <= end; ++j) {
            sum += data[j];
            count++;
        }
        
        filtered[i] = sum / count;
    }
    
    return filtered;
}

std::vector<double> CollectionPage::applyDenoising(const std::vector<double>& data) {
    if (data.empty()) return data;
    
    int windowSize = movingAverageWindowSpinBox_->value();
    if (windowSize < 3) windowSize = 3;
    if (windowSize > static_cast<int>(data.size() / 2)) windowSize = static_cast<int>(data.size() / 2);
    
    std::vector<double> denoised(data.size());
    
    // 移动平均去噪
    for (size_t i = 0; i < data.size(); ++i) {
        double sum = 0.0;
        int count = 0;
        
        int start = std::max(0, static_cast<int>(i) - windowSize / 2);
        int end = std::min(static_cast<int>(data.size()) - 1, static_cast<int>(i) + windowSize / 2);
        
        for (int j = start; j <= end; ++j) {
            sum += data[j];
            count++;
        }
        
        denoised[i] = sum / count;
    }
    
    // 中值滤波进一步去噪
    std::vector<double> medianFiltered(data.size());
    int medianWindow = std::min(5, windowSize);
    
    for (size_t i = 0; i < denoised.size(); ++i) {
        std::vector<double> window;
        
        int start = std::max(0, static_cast<int>(i) - medianWindow / 2);
        int end = std::min(static_cast<int>(denoised.size()) - 1, static_cast<int>(i) + medianWindow / 2);
        
        for (int j = start; j <= end; ++j) {
            window.push_back(denoised[j]);
        }
        
        std::sort(window.begin(), window.end());
        medianFiltered[i] = window[window.size() / 2];
    }
    
    return medianFiltered;
}

QString CollectionPage::formatDataForExport(const std::vector<double>& timeData, 
                                           const std::vector<double>& currentData, 
                                           const QString& format) {
    if (timeData.empty() || currentData.empty()) {
        return QString();
    }
    
    QStringList lines;
    size_t minSize = std::min(timeData.size(), currentData.size());
    
    if (format.contains("CSV")) {
        lines << QString::fromUtf8("时间(s),电流(nA)");
        for (size_t i = 0; i < minSize; ++i) {
            lines << QString("%1,%2")
                     .arg(QString::number(timeData[i], 'f', 6))
                     .arg(QString::number(currentData[i], 'f', 6));
        }
    } else if (format.contains("TSV")) {
        lines << QString::fromUtf8("时间(s)\t电流(nA)");
        for (size_t i = 0; i < minSize; ++i) {
            lines << QString("%1\t%2")
                     .arg(QString::number(timeData[i], 'f', 6))
                     .arg(QString::number(currentData[i], 'f', 6));
        }
    } else if (format.contains("JSON")) {
        QJsonObject rootObj;
        QJsonArray timeArray, currentArray;
        
        for (size_t i = 0; i < minSize; ++i) {
            timeArray.append(timeData[i]);
            currentArray.append(currentData[i]);
        }
        
        rootObj["time"] = timeArray;
        rootObj["current"] = currentArray;
        rootObj["metadata"] = QJsonObject{
            {"export_time", QDateTime::currentDateTime().toString()},
            {"data_points", static_cast<int>(minSize)},
            {"sampling_rate", samplingRateSpinBox_->value()}
        };
        
        QJsonDocument doc(rootObj);
        return doc.toJson();
    }
    
    return lines.join("\n");
}

void CollectionPage::setupVisualization() {
    // 创建主分割器
    mainSplitter_ = new QSplitter(Qt::Horizontal, this);
    
    // 创建实时数据图表
    chart_ = new QChart();
    chart_->setTitle("实时纳米孔电流信号");
    chart_->setAnimationOptions(QChart::NoAnimation);
    
    // 创建数据系列
    currentSeries_ = new QLineSeries();
    currentSeries_->setName("电流信号");
    currentSeries_->setPen(QPen(QColor(0, 120, 215), 2));
    
    baselineSeries_ = new QLineSeries();
    baselineSeries_->setName("基线");
    baselineSeries_->setPen(QPen(QColor(255, 140, 0), 1, Qt::DashLine));
    
    eventSeries_ = new QScatterSeries();
    eventSeries_->setName("事件");
    eventSeries_->setMarkerSize(8);
    eventSeries_->setColor(QColor(220, 20, 60));
    
    // 添加系列到图表
    chart_->addSeries(currentSeries_);
    chart_->addSeries(baselineSeries_);
    chart_->addSeries(eventSeries_);
    
    // 创建坐标轴
    timeAxis_ = new QValueAxis();
    timeAxis_->setTitleText("时间 (s)");
    timeAxis_->setRange(0, 10);
    timeAxis_->setTickCount(6);
    
    currentAxis_ = new QValueAxis();
    currentAxis_->setTitleText("电流 (pA)");
    currentAxis_->setRange(-100, 100);
    currentAxis_->setTickCount(6);
    
    // 设置坐标轴
    chart_->addAxis(timeAxis_, Qt::AlignBottom);
    chart_->addAxis(currentAxis_, Qt::AlignLeft);
    
    currentSeries_->attachAxis(timeAxis_);
    currentSeries_->attachAxis(currentAxis_);
    baselineSeries_->attachAxis(timeAxis_);
    baselineSeries_->attachAxis(currentAxis_);
    eventSeries_->attachAxis(timeAxis_);
    eventSeries_->attachAxis(currentAxis_);
    
    // 创建图表视图
    realTimeChart_ = new QChartView(chart_);
    realTimeChart_->setRenderHint(QPainter::Antialiasing);
    realTimeChart_->setMinimumSize(800, 500);
    
    // 创建统计图表
    statsChart_ = new QChart();
    statsChart_->setTitle("实时统计信息");
    statsChart_->setAnimationOptions(QChart::NoAnimation);
    
    // 创建统计数据系列
    meanSeries_ = new QLineSeries();
    meanSeries_->setName("平均值");
    meanSeries_->setPen(QPen(QColor(46, 125, 50), 2));
    
    rmsNoiseSeries_ = new QLineSeries();
    rmsNoiseSeries_->setName("RMS噪声");
    rmsNoiseSeries_->setPen(QPen(QColor(156, 39, 176), 2));
    
    eventRateSeries_ = new QLineSeries();
    eventRateSeries_->setName("事件率");
    eventRateSeries_->setPen(QPen(QColor(255, 87, 34), 2));
    
    // 添加统计系列到图表
    statsChart_->addSeries(meanSeries_);
    statsChart_->addSeries(rmsNoiseSeries_);
    statsChart_->addSeries(eventRateSeries_);
    
    // 创建统计图表的坐标轴
    auto* statsTimeAxis = new QValueAxis();
    statsTimeAxis->setTitleText("时间 (s)");
    statsTimeAxis->setRange(0, 60);
    statsTimeAxis->setTickCount(7);
    
    auto* statsValueAxis = new QValueAxis();
    statsValueAxis->setTitleText("数值");
    statsValueAxis->setRange(-50, 50);
    statsValueAxis->setTickCount(6);
    
    // 设置统计图表坐标轴
    statsChart_->addAxis(statsTimeAxis, Qt::AlignBottom);
    statsChart_->addAxis(statsValueAxis, Qt::AlignLeft);
    
    meanSeries_->attachAxis(statsTimeAxis);
    meanSeries_->attachAxis(statsValueAxis);
    rmsNoiseSeries_->attachAxis(statsTimeAxis);
    rmsNoiseSeries_->attachAxis(statsValueAxis);
    eventRateSeries_->attachAxis(statsTimeAxis);
    eventRateSeries_->attachAxis(statsValueAxis);
    
    // 创建统计图表视图
    statisticsChart_ = new QChartView(statsChart_);
    statisticsChart_->setRenderHint(QPainter::Antialiasing);
    statisticsChart_->setMinimumSize(800, 400);
    
    // 添加图表到分割器
    mainSplitter_->addWidget(realTimeChart_);
    mainSplitter_->addWidget(statisticsChart_);
    mainSplitter_->setSizes({2, 1}); // 实时图表占2/3，统计图表占1/3
    
    // 将分割器添加到主布局
    QVBoxLayout* pageLayout = qobject_cast<QVBoxLayout*>(layout());
    if (pageLayout) {
        QScrollArea* scrollArea = qobject_cast<QScrollArea*>(pageLayout->itemAt(0)->widget());
        if (scrollArea) {
            QWidget* contentWidget = scrollArea->widget();
            if (contentWidget) {
                QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(contentWidget->layout());
                if (mainLayout) {
                    mainLayout->addWidget(mainSplitter_, 1); // 图表占据剩余空间
                }
            }
        }
    }
    
    // 设置图表控制组件
    setupChartControls();
}

void CollectionPage::updateRealTimeChart(double time, double current) {
    if (!currentSeries_ || !baselineSeries_) return;
    
    // 检查图表是否暂停
    if (chartsPaused_) return;
    
    // 性能优化：限制最大数据点数量
    const int maxDataPoints = 1000;
    const double timeWindow = 10.0;
    
    // 添加当前数据点
    currentSeries_->append(time, current);
    baselineSeries_->append(time, currentBaseline_);
    
    // 性能优化：批量移除过旧的数据点
    if (currentSeries_->count() > maxDataPoints) {
        int pointsToRemove = currentSeries_->count() - maxDataPoints;
        currentSeries_->removePoints(0, pointsToRemove);
        baselineSeries_->removePoints(0, pointsToRemove);
    }
    
    // 基于时间窗口移除数据点（较少频率执行）
    static int updateCounter = 0;
    updateCounter++;
    if (updateCounter % 50 == 0) { // 每50个数据点检查一次
        auto currentPoints = currentSeries_->points();
        auto baselinePoints = baselineSeries_->points();
        
        int currentRemoveCount = 0;
        int baselineRemoveCount = 0;
        
        // 计算需要移除的点数
        for (const auto& point : currentPoints) {
            if (point.x() < time - timeWindow) {
                currentRemoveCount++;
            } else {
                break;
            }
        }
        
        for (const auto& point : baselinePoints) {
            if (point.x() < time - timeWindow) {
                baselineRemoveCount++;
            } else {
                break;
            }
        }
        
        // 批量移除
        if (currentRemoveCount > 0) {
            currentSeries_->removePoints(0, currentRemoveCount);
        }
        if (baselineRemoveCount > 0) {
            baselineSeries_->removePoints(0, baselineRemoveCount);
        }
        
        // 更新时间轴范围
        if (timeAxis_) {
            timeAxis_->setRange(time - timeWindow, time);
        }
        
        // 动态调整电流轴范围（降低频率）
        if (currentAxis_) {
            auto points = currentSeries_->points();
            if (!points.isEmpty()) {
                double minCurrent = std::numeric_limits<double>::max();
                double maxCurrent = std::numeric_limits<double>::lowest();
                
                // 只检查最近的点以提高性能
                int startIndex = std::max(0, static_cast<int>(points.size()) - 200);
                for (int i = startIndex; i < points.size(); ++i) {
                    double y = points[i].y();
                    minCurrent = std::min(minCurrent, y);
                    maxCurrent = std::max(maxCurrent, y);
                }
                
                double range = maxCurrent - minCurrent;
                double margin = range * 0.1; // 10% 边距
                currentAxis_->setRange(minCurrent - margin, maxCurrent + margin);
            }
        }
    }
}

void CollectionPage::updateStatisticsChart() {
    if (!meanSeries_ || !rmsNoiseSeries_ || !eventRateSeries_) return;
    
    // 检查图表是否暂停
    if (chartsPaused_) return;
    
    double currentTime = QDateTime::currentMSecsSinceEpoch() / 1000.0 - startTime_;
    
    // 性能优化：限制最大数据点数量
    const int maxStatPoints = 300; // 统计图表数据点较少
    const double timeWindow = 60.0;
    
    // 添加统计数据点
    meanSeries_->append(currentTime, currentStats_.mean);
    rmsNoiseSeries_->append(currentTime, currentStats_.rms);
    
    // 计算事件率 (事件数/分钟)
    double eventRate = 0.0;
    if (currentTime > 0) {
        eventRate = (eventTimes_.size() / currentTime) * 60.0; // 转换为每分钟事件数
    }
    eventRateSeries_->append(currentTime, eventRate);
    
    // 性能优化：批量移除过旧的数据点
    if (meanSeries_->count() > maxStatPoints) {
        int pointsToRemove = meanSeries_->count() - maxStatPoints;
        meanSeries_->removePoints(0, pointsToRemove);
        rmsNoiseSeries_->removePoints(0, pointsToRemove);
        eventRateSeries_->removePoints(0, pointsToRemove);
    }
    
    // 基于时间窗口移除数据点（较少频率执行）
    static int statsUpdateCounter = 0;
    statsUpdateCounter++;
    if (statsUpdateCounter % 10 == 0) { // 每10个统计更新检查一次
        auto meanPoints = meanSeries_->points();
        auto rmsPoints = rmsNoiseSeries_->points();
        auto eventPoints = eventRateSeries_->points();
        
        int meanRemoveCount = 0;
        int rmsRemoveCount = 0;
        int eventRemoveCount = 0;
        
        // 计算需要移除的点数
        for (const auto& point : meanPoints) {
            if (point.x() < currentTime - timeWindow) {
                meanRemoveCount++;
            } else {
                break;
            }
        }
        
        for (const auto& point : rmsPoints) {
            if (point.x() < currentTime - timeWindow) {
                rmsRemoveCount++;
            } else {
                break;
            }
        }
        
        for (const auto& point : eventPoints) {
            if (point.x() < currentTime - timeWindow) {
                eventRemoveCount++;
            } else {
                break;
            }
        }
        
        // 批量移除
        if (meanRemoveCount > 0) {
            meanSeries_->removePoints(0, meanRemoveCount);
        }
        if (rmsRemoveCount > 0) {
            rmsNoiseSeries_->removePoints(0, rmsRemoveCount);
        }
        if (eventRemoveCount > 0) {
            eventRateSeries_->removePoints(0, eventRemoveCount);
        }
        
        // 更新统计图表的时间轴
        auto axes = statsChart_->axes(Qt::Horizontal);
        if (!axes.isEmpty()) {
            auto* timeAxis = qobject_cast<QValueAxis*>(axes.first());
            if (timeAxis) {
                timeAxis->setRange(currentTime - timeWindow, currentTime);
            }
        }
    }
}

void CollectionPage::resetChartData() {
    if (currentSeries_) currentSeries_->clear();
    if (baselineSeries_) baselineSeries_->clear();
    if (eventSeries_) eventSeries_->clear();
    if (meanSeries_) meanSeries_->clear();
    if (rmsNoiseSeries_) rmsNoiseSeries_->clear();
    if (eventRateSeries_) eventRateSeries_->clear();
    
    // 重置坐标轴
    if (timeAxis_) {
        timeAxis_->setRange(0, 10);
    }
    if (currentAxis_) {
        currentAxis_->setRange(-100, 100);
    }
    
    // 重置统计图表坐标轴
    auto axes = statsChart_->axes(Qt::Horizontal);
    if (!axes.isEmpty()) {
        auto* timeAxis = qobject_cast<QValueAxis*>(axes.first());
        if (timeAxis) {
            timeAxis->setRange(0, 60);
        }
    }
}

void CollectionPage::exportChartImage(const QString& fileName) {
    if (!realTimeChart_ || !statisticsChart_) return;
    
    // 创建组合图像
    QPixmap combinedPixmap(800, 600);
    combinedPixmap.fill(Qt::white);
    
    QPainter painter(&combinedPixmap);
    
    // 渲染实时图表
    QPixmap realTimePixmap = realTimeChart_->grab();
    painter.drawPixmap(0, 0, 800, 400, realTimePixmap);
    
    // 渲染统计图表
    QPixmap statsPixmap = statisticsChart_->grab();
    painter.drawPixmap(0, 400, 800, 200, statsPixmap);
    
    painter.end();
    
    // 保存图像
    combinedPixmap.save(fileName);
}

void CollectionPage::setupChartControls() {
    // 创建图表控制组
    chartControlGroup_ = new QGroupBox("图表控制");
    auto* controlLayout = new QHBoxLayout(chartControlGroup_);
    
    // 暂停/恢复按钮
    pauseChartBtn_ = new QPushButton("暂停图表");
    pauseChartBtn_->setCheckable(true);
    connect(pauseChartBtn_, &QPushButton::clicked, this, &CollectionPage::toggleChartPause);
    
    // 重置缩放按钮
    resetZoomBtn_ = new QPushButton("重置缩放");
    connect(resetZoomBtn_, &QPushButton::clicked, this, &CollectionPage::resetChartZoom);
    
    // 导出图表按钮
    exportChartBtn_ = new QPushButton("导出图表");
    connect(exportChartBtn_, &QPushButton::clicked, this, &CollectionPage::exportCurrentChart);
    
    // 自动缩放复选框
    autoScaleCheck_ = new QCheckBox("自动缩放");
    autoScaleCheck_->setChecked(true);
    connect(autoScaleCheck_, &QCheckBox::toggled, this, &CollectionPage::toggleAutoScale);
    
    // 添加控件到布局
    controlLayout->addWidget(pauseChartBtn_);
    controlLayout->addWidget(resetZoomBtn_);
    controlLayout->addWidget(exportChartBtn_);
    controlLayout->addWidget(autoScaleCheck_);
    controlLayout->addStretch();
    
    // 将控制组添加到主布局
    auto* mainLayout = qobject_cast<QVBoxLayout*>(layout());
    if (mainLayout) {
        mainLayout->addWidget(chartControlGroup_);
    }
}

void CollectionPage::toggleChartPause() {
    chartsPaused_ = !chartsPaused_;
    
    if (chartsPaused_) {
        pauseChartBtn_->setText("恢复图表");
        pauseChartBtn_->setStyleSheet("QPushButton { background-color: #ff6b6b; }");
    } else {
        pauseChartBtn_->setText("暂停图表");
        pauseChartBtn_->setStyleSheet("");
    }
}

void CollectionPage::resetChartZoom() {
    if (!realTimeChart_ || !statisticsChart_) return;
    
    // 重置实时图表缩放
    if (timeAxis_ && currentAxis_) {
        timeAxis_->setRange(0, 10);
        currentAxis_->setRange(-100, 100);
    }
    
    // 重置统计图表缩放
    auto* statsTimeAxis = qobject_cast<QValueAxis*>(statsChart_->axes(Qt::Horizontal).first());
    auto* statsValueAxis = qobject_cast<QValueAxis*>(statsChart_->axes(Qt::Vertical).first());
    
    if (statsTimeAxis && statsValueAxis) {
        statsTimeAxis->setRange(0, 60);
        statsValueAxis->setRange(-50, 50);
    }
}

void CollectionPage::exportCurrentChart() {
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "导出图表",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/chart_export.png",
        "PNG图片 (*.png);;JPEG图片 (*.jpg);;所有文件 (*.*)"
    );
    
    if (!fileName.isEmpty()) {
        exportChartImage(fileName);
        QMessageBox::information(this, "导出成功", "图表已成功导出到: " + fileName);
    }
}

void CollectionPage::toggleAutoScale() {
    // 自动缩放功能将在updateRealTimeChart和updateStatisticsChart中使用
    // 这里只是切换状态，实际逻辑在图表更新方法中实现
}

void CollectionPage::toggleControlsVisibility() {
    controlsVisible_ = !controlsVisible_;
    controlsContainer_->setVisible(controlsVisible_);
    
    // 更新按钮文本
    if (controlsVisible_) {
        toggleControlsBtn_->setText("隐藏控制面板 ▲");
    } else {
        toggleControlsBtn_->setText("显示控制面板 ▼");
    }
}

void CollectionPage::openDataAnalysisDialog() {
    if (timeData_.empty() || currentData_.empty()) {
        QMessageBox::information(this, "提示", "没有可分析的数据，请先开始数据采集。");
        return;
    }
    
    DataAnalysisDialog* dialog = new DataAnalysisDialog(this);
    dialog->setAnalysisData(timeData_, currentData_);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

void CollectionPage::openDeviceManagementDialog() {
    DeviceManagementDialog* dialog = new DeviceManagementDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

void CollectionPage::openDataExportDialog() {
    if (timeData_.empty() || currentData_.empty()) {
        QMessageBox::information(this, "提示", "没有可导出的数据，请先开始数据采集。");
        return;
    }
    
    DataExportDialog* dialog = new DataExportDialog(this);
    dialog->setExportData(timeData_, currentData_);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}