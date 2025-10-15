#include "ParameterConfigPage.h"
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>
#include <QJsonDocument>
#include <QJsonObject>

ParameterConfigPage::ParameterConfigPage(QWidget *parent)
    : QWidget(parent)
    , mainLayout_(nullptr)
    , tabWidget_(nullptr)
    , buttonLayout_(nullptr)
    , deviceTab_(nullptr)
    , deviceScrollArea_(nullptr)
    , analysisTab_(nullptr)
    , analysisScrollArea_(nullptr)
    , displayTab_(nullptr)
    , displayScrollArea_(nullptr)
{
    setupUI();
    connectSignals();
    loadDefaultParameters();
}

void ParameterConfigPage::setupUI()
{
    mainLayout_ = new QVBoxLayout(this);
    
    // Create tab widget
    tabWidget_ = new QTabWidget();
    mainLayout_->addWidget(tabWidget_);
    
    // Setup parameter tabs
    setupDeviceParameters();
    setupAnalysisParameters();
    setupDisplayParameters();
    
    // Setup control buttons
    setupControlButtons();
    
    setLayout(mainLayout_);
}

void ParameterConfigPage::setupDeviceParameters()
{
    deviceTab_ = new QWidget();
    deviceScrollArea_ = new QScrollArea();
    deviceScrollArea_->setWidget(deviceTab_);
    deviceScrollArea_->setWidgetResizable(true);
    
    QVBoxLayout* deviceLayout = new QVBoxLayout(deviceTab_);
    
    // Connection settings group
    connectionGroup_ = new QGroupBox("连接设置");
    QFormLayout* connectionLayout = new QFormLayout(connectionGroup_);
    
    deviceTypeCombo_ = new QComboBox();
    deviceTypeCombo_->addItems({"USB", "串口", "网络", "模拟器"});
    connectionLayout->addRow("设备类型:", deviceTypeCombo_);
    
    devicePortEdit_ = new QLineEdit("/dev/ttyUSB0");
    connectionLayout->addRow("端口:", devicePortEdit_);
    
    baudRateSpinBox_ = new QSpinBox();
    baudRateSpinBox_->setRange(9600, 921600);
    baudRateSpinBox_->setValue(115200);
    connectionLayout->addRow("波特率:", baudRateSpinBox_);
    
    autoConnectCheckBox_ = new QCheckBox("自动连接");
    connectionLayout->addRow(autoConnectCheckBox_);
    
    deviceLayout->addWidget(connectionGroup_);
    
    // Acquisition settings group
    acquisitionGroup_ = new QGroupBox("数据采集设置");
    QFormLayout* acquisitionLayout = new QFormLayout(acquisitionGroup_);
    
    samplingRateSpinBox_ = new QDoubleSpinBox();
    samplingRateSpinBox_->setRange(1.0, 10000.0);
    samplingRateSpinBox_->setValue(4000.0);
    samplingRateSpinBox_->setSuffix(" Hz");
    acquisitionLayout->addRow("采样率:", samplingRateSpinBox_);
    
    bufferSizeSpinBox_ = new QSpinBox();
    bufferSizeSpinBox_->setRange(1024, 1048576);
    bufferSizeSpinBox_->setValue(8192);
    acquisitionLayout->addRow("缓冲区大小:", bufferSizeSpinBox_);
    
    QHBoxLayout* gainLayout = new QHBoxLayout();
    gainSlider_ = new QSlider(Qt::Horizontal);
    gainSlider_->setRange(1, 100);
    gainSlider_->setValue(50);
    gainValueLabel_ = new QLabel("50");
    gainLayout->addWidget(gainSlider_);
    gainLayout->addWidget(gainValueLabel_);
    acquisitionLayout->addRow("增益:", gainLayout);
    
    offsetSpinBox_ = new QDoubleSpinBox();
    offsetSpinBox_->setRange(-1000.0, 1000.0);
    offsetSpinBox_->setValue(0.0);
    offsetSpinBox_->setSuffix(" mV");
    acquisitionLayout->addRow("偏移:", offsetSpinBox_);
    
    deviceLayout->addWidget(acquisitionGroup_);
    
    // Calibration settings group
    calibrationGroup_ = new QGroupBox("校准设置");
    QFormLayout* calibrationLayout = new QFormLayout(calibrationGroup_);
    
    enableFilterCheckBox_ = new QCheckBox("启用滤波器");
    calibrationLayout->addRow(enableFilterCheckBox_);
    
    filterFreqSpinBox_ = new QDoubleSpinBox();
    filterFreqSpinBox_->setRange(1.0, 1000.0);
    filterFreqSpinBox_->setValue(100.0);
    filterFreqSpinBox_->setSuffix(" Hz");
    calibrationLayout->addRow("滤波频率:", filterFreqSpinBox_);
    
    deviceLayout->addWidget(calibrationGroup_);
    
    deviceLayout->addStretch();
    tabWidget_->addTab(deviceScrollArea_, "设备参数");
}

void ParameterConfigPage::setupAnalysisParameters()
{
    analysisTab_ = new QWidget();
    analysisScrollArea_ = new QScrollArea();
    analysisScrollArea_->setWidget(analysisTab_);
    analysisScrollArea_->setWidgetResizable(true);
    
    QVBoxLayout* analysisLayout = new QVBoxLayout(analysisTab_);
    
    // Detection settings group
    detectionGroup_ = new QGroupBox("检测设置");
    QFormLayout* detectionLayout = new QFormLayout(detectionGroup_);
    
    thresholdSpinBox_ = new QDoubleSpinBox();
    thresholdSpinBox_->setRange(0.1, 100.0);
    thresholdSpinBox_->setValue(5.0);
    thresholdSpinBox_->setSuffix(" pA");
    detectionLayout->addRow("阈值:", thresholdSpinBox_);
    
    windowSizeSpinBox_ = new QSpinBox();
    windowSizeSpinBox_->setRange(10, 10000);
    windowSizeSpinBox_->setValue(100);
    detectionLayout->addRow("窗口大小:", windowSizeSpinBox_);
    
    baselineSpinBox_ = new QDoubleSpinBox();
    baselineSpinBox_->setRange(-1000.0, 1000.0);
    baselineSpinBox_->setValue(0.0);
    baselineSpinBox_->setSuffix(" pA");
    detectionLayout->addRow("基线:", baselineSpinBox_);
    
    enableNoiseReductionCheckBox_ = new QCheckBox("启用降噪");
    detectionLayout->addRow(enableNoiseReductionCheckBox_);
    
    analysisLayout->addWidget(detectionGroup_);
    
    // Classification settings group
    classificationGroup_ = new QGroupBox("分类设置");
    QFormLayout* classificationLayout = new QFormLayout(classificationGroup_);
    
    algorithmCombo_ = new QComboBox();
    algorithmCombo_->addItems({"阈值检测", "机器学习", "深度学习", "统计分析"});
    classificationLayout->addRow("算法:", algorithmCombo_);
    
    minEventLengthSpinBox_ = new QSpinBox();
    minEventLengthSpinBox_->setRange(1, 1000);
    minEventLengthSpinBox_->setValue(10);
    classificationLayout->addRow("最小事件长度:", minEventLengthSpinBox_);
    
    maxEventLengthSpinBox_ = new QSpinBox();
    maxEventLengthSpinBox_->setRange(10, 10000);
    maxEventLengthSpinBox_->setValue(1000);
    classificationLayout->addRow("最大事件长度:", maxEventLengthSpinBox_);
    
    confidenceThresholdSpinBox_ = new QDoubleSpinBox();
    confidenceThresholdSpinBox_->setRange(0.0, 1.0);
    confidenceThresholdSpinBox_->setValue(0.8);
    confidenceThresholdSpinBox_->setDecimals(2);
    classificationLayout->addRow("置信度阈值:", confidenceThresholdSpinBox_);
    
    analysisLayout->addWidget(classificationGroup_);
    
    // Statistics settings group
    statisticsGroup_ = new QGroupBox("统计设置");
    QFormLayout* statisticsLayout = new QFormLayout(statisticsGroup_);
    
    enableRealTimeAnalysisCheckBox_ = new QCheckBox("启用实时分析");
    statisticsLayout->addRow(enableRealTimeAnalysisCheckBox_);
    
    analysisLayout->addWidget(statisticsGroup_);
    
    analysisLayout->addStretch();
    tabWidget_->addTab(analysisScrollArea_, "分析参数");
}

void ParameterConfigPage::setupDisplayParameters()
{
    displayTab_ = new QWidget();
    displayScrollArea_ = new QScrollArea();
    displayScrollArea_->setWidget(displayTab_);
    displayScrollArea_->setWidgetResizable(true);
    
    QVBoxLayout* displayLayout = new QVBoxLayout(displayTab_);
    
    // Chart settings group
    chartGroup_ = new QGroupBox("图表设置");
    QFormLayout* chartLayout = new QFormLayout(chartGroup_);
    
    refreshRateSpinBox_ = new QSpinBox();
    refreshRateSpinBox_->setRange(1, 100);
    refreshRateSpinBox_->setValue(30);
    refreshRateSpinBox_->setSuffix(" FPS");
    chartLayout->addRow("刷新率:", refreshRateSpinBox_);
    
    maxDataPointsSpinBox_ = new QSpinBox();
    maxDataPointsSpinBox_->setRange(100, 100000);
    maxDataPointsSpinBox_->setValue(10000);
    chartLayout->addRow("最大数据点:", maxDataPointsSpinBox_);
    
    enableAnimationCheckBox_ = new QCheckBox("启用动画");
    chartLayout->addRow(enableAnimationCheckBox_);
    
    chartThemeCombo_ = new QComboBox();
    chartThemeCombo_->addItems({"浅色主题", "深色主题", "蓝色主题", "高对比度"});
    chartLayout->addRow("图表主题:", chartThemeCombo_);
    
    enableGridCheckBox_ = new QCheckBox("显示网格");
    chartLayout->addRow(enableGridCheckBox_);
    
    enableLegendCheckBox_ = new QCheckBox("显示图例");
    chartLayout->addRow(enableLegendCheckBox_);
    
    displayLayout->addWidget(chartGroup_);
    
    // Color settings group
    colorGroup_ = new QGroupBox("颜色设置");
    QFormLayout* colorLayout = new QFormLayout(colorGroup_);
    
    colorSchemeCombo_ = new QComboBox();
    colorSchemeCombo_->addItems({"默认", "彩虹", "热力图", "灰度", "自定义"});
    colorLayout->addRow("配色方案:", colorSchemeCombo_);
    
    QHBoxLayout* transparencyLayout = new QHBoxLayout();
    transparencySlider_ = new QSlider(Qt::Horizontal);
    transparencySlider_->setRange(0, 100);
    transparencySlider_->setValue(80);
    transparencyValueLabel_ = new QLabel("80%");
    transparencyLayout->addWidget(transparencySlider_);
    transparencyLayout->addWidget(transparencyValueLabel_);
    colorLayout->addRow("透明度:", transparencyLayout);
    
    lineWidthSpinBox_ = new QSpinBox();
    lineWidthSpinBox_->setRange(1, 10);
    lineWidthSpinBox_->setValue(2);
    colorLayout->addRow("线条宽度:", lineWidthSpinBox_);
    
    displayLayout->addWidget(colorGroup_);
    
    // Performance settings group
    performanceGroup_ = new QGroupBox("性能设置");
    QFormLayout* performanceLayout = new QFormLayout(performanceGroup_);
    
    displayLayout->addWidget(performanceGroup_);
    
    displayLayout->addStretch();
    tabWidget_->addTab(displayScrollArea_, "显示参数");
}

void ParameterConfigPage::setupControlButtons()
{
    buttonLayout_ = new QHBoxLayout();
    
    saveButton_ = new QPushButton("保存配置");
    loadButton_ = new QPushButton("加载配置");
    resetButton_ = new QPushButton("重置默认");
    applyButton_ = new QPushButton("应用");
    cancelButton_ = new QPushButton("取消");
    
    buttonLayout_->addWidget(saveButton_);
    buttonLayout_->addWidget(loadButton_);
    buttonLayout_->addStretch();
    buttonLayout_->addWidget(resetButton_);
    buttonLayout_->addWidget(applyButton_);
    buttonLayout_->addWidget(cancelButton_);
    
    mainLayout_->addLayout(buttonLayout_);
}

void ParameterConfigPage::connectSignals()
{
    // Device parameter signals
    connect(deviceTypeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ParameterConfigPage::onDeviceParameterChanged);
    connect(devicePortEdit_, &QLineEdit::textChanged,
            this, &ParameterConfigPage::onDeviceParameterChanged);
    connect(samplingRateSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ParameterConfigPage::onDeviceParameterChanged);
    connect(gainSlider_, &QSlider::valueChanged, [this](int value) {
        gainValueLabel_->setText(QString::number(value));
        onDeviceParameterChanged();
    });
    
    // Analysis parameter signals
    connect(thresholdSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ParameterConfigPage::onAnalysisParameterChanged);
    connect(algorithmCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ParameterConfigPage::onAnalysisParameterChanged);
    
    // Display parameter signals
    connect(refreshRateSpinBox_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ParameterConfigPage::onDisplayParameterChanged);
    connect(chartThemeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ParameterConfigPage::onDisplayParameterChanged);
    connect(transparencySlider_, &QSlider::valueChanged, [this](int value) {
        transparencyValueLabel_->setText(QString::number(value) + "%");
        onDisplayParameterChanged();
    });
    
    // Button signals
    connect(saveButton_, &QPushButton::clicked, this, &ParameterConfigPage::onSaveParameters);
    connect(loadButton_, &QPushButton::clicked, this, &ParameterConfigPage::onLoadParameters);
    connect(resetButton_, &QPushButton::clicked, this, &ParameterConfigPage::onResetToDefaults);
    connect(applyButton_, &QPushButton::clicked, this, &ParameterConfigPage::onApplyParameters);
    connect(cancelButton_, &QPushButton::clicked, this, &QWidget::close);
}

void ParameterConfigPage::loadDefaultParameters()
{
    // Load default device parameters
    deviceParameters_["deviceType"] = 0;
    deviceParameters_["devicePort"] = "/dev/ttyUSB0";
    deviceParameters_["baudRate"] = 115200;
    deviceParameters_["samplingRate"] = 4000.0;
    deviceParameters_["bufferSize"] = 8192;
    deviceParameters_["autoConnect"] = false;
    deviceParameters_["gain"] = 50;
    deviceParameters_["offset"] = 0.0;
    deviceParameters_["enableFilter"] = false;
    deviceParameters_["filterFreq"] = 100.0;
    
    // Load default analysis parameters
    analysisParameters_["threshold"] = 5.0;
    analysisParameters_["windowSize"] = 100;
    analysisParameters_["baseline"] = 0.0;
    analysisParameters_["enableNoiseReduction"] = true;
    analysisParameters_["algorithm"] = 0;
    analysisParameters_["minEventLength"] = 10;
    analysisParameters_["maxEventLength"] = 1000;
    analysisParameters_["enableRealTimeAnalysis"] = true;
    analysisParameters_["confidenceThreshold"] = 0.8;
    
    // Load default display parameters
    displayParameters_["refreshRate"] = 30;
    displayParameters_["maxDataPoints"] = 10000;
    displayParameters_["enableAnimation"] = true;
    displayParameters_["chartTheme"] = 0;
    displayParameters_["transparency"] = 80;
    displayParameters_["enableGrid"] = true;
    displayParameters_["enableLegend"] = true;
    displayParameters_["colorScheme"] = 0;
    displayParameters_["lineWidth"] = 2;
    
    updateParameterDisplay();
}

void ParameterConfigPage::updateParameterDisplay()
{
    // Update device parameter displays
    deviceTypeCombo_->setCurrentIndex(deviceParameters_["deviceType"].toInt());
    devicePortEdit_->setText(deviceParameters_["devicePort"].toString());
    baudRateSpinBox_->setValue(deviceParameters_["baudRate"].toInt());
    samplingRateSpinBox_->setValue(deviceParameters_["samplingRate"].toDouble());
    bufferSizeSpinBox_->setValue(deviceParameters_["bufferSize"].toInt());
    autoConnectCheckBox_->setChecked(deviceParameters_["autoConnect"].toBool());
    gainSlider_->setValue(deviceParameters_["gain"].toInt());
    gainValueLabel_->setText(QString::number(deviceParameters_["gain"].toInt()));
    offsetSpinBox_->setValue(deviceParameters_["offset"].toDouble());
    enableFilterCheckBox_->setChecked(deviceParameters_["enableFilter"].toBool());
    filterFreqSpinBox_->setValue(deviceParameters_["filterFreq"].toDouble());
    
    // Update analysis parameter displays
    thresholdSpinBox_->setValue(analysisParameters_["threshold"].toDouble());
    windowSizeSpinBox_->setValue(analysisParameters_["windowSize"].toInt());
    baselineSpinBox_->setValue(analysisParameters_["baseline"].toDouble());
    enableNoiseReductionCheckBox_->setChecked(analysisParameters_["enableNoiseReduction"].toBool());
    algorithmCombo_->setCurrentIndex(analysisParameters_["algorithm"].toInt());
    minEventLengthSpinBox_->setValue(analysisParameters_["minEventLength"].toInt());
    maxEventLengthSpinBox_->setValue(analysisParameters_["maxEventLength"].toInt());
    enableRealTimeAnalysisCheckBox_->setChecked(analysisParameters_["enableRealTimeAnalysis"].toBool());
    confidenceThresholdSpinBox_->setValue(analysisParameters_["confidenceThreshold"].toDouble());
    
    // Update display parameter displays
    refreshRateSpinBox_->setValue(displayParameters_["refreshRate"].toInt());
    maxDataPointsSpinBox_->setValue(displayParameters_["maxDataPoints"].toInt());
    enableAnimationCheckBox_->setChecked(displayParameters_["enableAnimation"].toBool());
    chartThemeCombo_->setCurrentIndex(displayParameters_["chartTheme"].toInt());
    transparencySlider_->setValue(displayParameters_["transparency"].toInt());
    transparencyValueLabel_->setText(QString::number(displayParameters_["transparency"].toInt()) + "%");
    enableGridCheckBox_->setChecked(displayParameters_["enableGrid"].toBool());
    enableLegendCheckBox_->setChecked(displayParameters_["enableLegend"].toBool());
    colorSchemeCombo_->setCurrentIndex(displayParameters_["colorScheme"].toInt());
    lineWidthSpinBox_->setValue(displayParameters_["lineWidth"].toInt());
}

void ParameterConfigPage::onDeviceParameterChanged()
{
    // Update device parameters from UI
    deviceParameters_["deviceType"] = deviceTypeCombo_->currentIndex();
    deviceParameters_["devicePort"] = devicePortEdit_->text();
    deviceParameters_["baudRate"] = baudRateSpinBox_->value();
    deviceParameters_["samplingRate"] = samplingRateSpinBox_->value();
    deviceParameters_["bufferSize"] = bufferSizeSpinBox_->value();
    deviceParameters_["autoConnect"] = autoConnectCheckBox_->isChecked();
    deviceParameters_["gain"] = gainSlider_->value();
    deviceParameters_["offset"] = offsetSpinBox_->value();
    deviceParameters_["enableFilter"] = enableFilterCheckBox_->isChecked();
    deviceParameters_["filterFreq"] = filterFreqSpinBox_->value();
}

void ParameterConfigPage::onAnalysisParameterChanged()
{
    // Update analysis parameters from UI
    analysisParameters_["threshold"] = thresholdSpinBox_->value();
    analysisParameters_["windowSize"] = windowSizeSpinBox_->value();
    analysisParameters_["baseline"] = baselineSpinBox_->value();
    analysisParameters_["enableNoiseReduction"] = enableNoiseReductionCheckBox_->isChecked();
    analysisParameters_["algorithm"] = algorithmCombo_->currentIndex();
    analysisParameters_["minEventLength"] = minEventLengthSpinBox_->value();
    analysisParameters_["maxEventLength"] = maxEventLengthSpinBox_->value();
    analysisParameters_["enableRealTimeAnalysis"] = enableRealTimeAnalysisCheckBox_->isChecked();
    analysisParameters_["confidenceThreshold"] = confidenceThresholdSpinBox_->value();
}

void ParameterConfigPage::onDisplayParameterChanged()
{
    // Update display parameters from UI
    displayParameters_["refreshRate"] = refreshRateSpinBox_->value();
    displayParameters_["maxDataPoints"] = maxDataPointsSpinBox_->value();
    displayParameters_["enableAnimation"] = enableAnimationCheckBox_->isChecked();
    displayParameters_["chartTheme"] = chartThemeCombo_->currentIndex();
    displayParameters_["transparency"] = transparencySlider_->value();
    displayParameters_["enableGrid"] = enableGridCheckBox_->isChecked();
    displayParameters_["enableLegend"] = enableLegendCheckBox_->isChecked();
    displayParameters_["colorScheme"] = colorSchemeCombo_->currentIndex();
    displayParameters_["lineWidth"] = lineWidthSpinBox_->value();
}

void ParameterConfigPage::onSaveParameters()
{
    QString fileName = QFileDialog::getSaveFileName(this, 
        "保存参数配置", "", "JSON Files (*.json)");
    
    if (!fileName.isEmpty()) {
        QJsonObject jsonObj;
        QJsonObject deviceObj, analysisObj, displayObj;
        
        // Convert QVariantMap to QJsonObject
        for (auto it = deviceParameters_.begin(); it != deviceParameters_.end(); ++it) {
            deviceObj[it.key()] = QJsonValue::fromVariant(it.value());
        }
        for (auto it = analysisParameters_.begin(); it != analysisParameters_.end(); ++it) {
            analysisObj[it.key()] = QJsonValue::fromVariant(it.value());
        }
        for (auto it = displayParameters_.begin(); it != displayParameters_.end(); ++it) {
            displayObj[it.key()] = QJsonValue::fromVariant(it.value());
        }
        
        jsonObj["device"] = deviceObj;
        jsonObj["analysis"] = analysisObj;
        jsonObj["display"] = displayObj;
        
        QJsonDocument doc(jsonObj);
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(doc.toJson());
            QMessageBox::information(this, "成功", "参数配置已保存");
        } else {
            QMessageBox::warning(this, "错误", "无法保存文件");
        }
    }
}

void ParameterConfigPage::onLoadParameters()
{
    QString fileName = QFileDialog::getOpenFileName(this, 
        "加载参数配置", "", "JSON Files (*.json)");
    
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray data = file.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(data);
            QJsonObject jsonObj = doc.object();
            
            // Load device parameters
            if (jsonObj.contains("device")) {
                QJsonObject deviceObj = jsonObj["device"].toObject();
                for (auto it = deviceObj.begin(); it != deviceObj.end(); ++it) {
                    deviceParameters_[it.key()] = it.value().toVariant();
                }
            }
            
            // Load analysis parameters
            if (jsonObj.contains("analysis")) {
                QJsonObject analysisObj = jsonObj["analysis"].toObject();
                for (auto it = analysisObj.begin(); it != analysisObj.end(); ++it) {
                    analysisParameters_[it.key()] = it.value().toVariant();
                }
            }
            
            // Load display parameters
            if (jsonObj.contains("display")) {
                QJsonObject displayObj = jsonObj["display"].toObject();
                for (auto it = displayObj.begin(); it != displayObj.end(); ++it) {
                    displayParameters_[it.key()] = it.value().toVariant();
                }
            }
            
            updateParameterDisplay();
            QMessageBox::information(this, "成功", "参数配置已加载");
        } else {
            QMessageBox::warning(this, "错误", "无法读取文件");
        }
    }
}

void ParameterConfigPage::onResetToDefaults()
{
    int ret = QMessageBox::question(this, "确认", 
        "确定要重置所有参数为默认值吗？", 
        QMessageBox::Yes | QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        loadDefaultParameters();
        QMessageBox::information(this, "成功", "参数已重置为默认值");
    }
}

void ParameterConfigPage::onApplyParameters()
{
    // Update parameters from UI
    onDeviceParameterChanged();
    onAnalysisParameterChanged();
    onDisplayParameterChanged();
    
    // Here you would typically emit signals or call functions to apply the parameters
    // to the actual system components
    
    QMessageBox::information(this, "成功", "参数已应用");
}