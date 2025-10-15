#include "DeviceManagementDialog.h"
#include <QApplication>
#include <QHeaderView>
#include <QFileDialog>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThread>

DeviceManagementDialog::DeviceManagementDialog(QWidget *parent)
    : QDialog(parent)
    , isConnected_(false)
    , statusUpdateTimer_(new QTimer(this))
{
    setWindowTitle("设备管理");
    setMinimumSize(900, 700);
    resize(1000, 800);
    
    setupUI();
    
    // 设置定时器更新状态
    connect(statusUpdateTimer_, &QTimer::timeout, this, &DeviceManagementDialog::refreshDeviceStatus);
    statusUpdateTimer_->start(2000); // 每2秒更新一次
    
    // 初始化设备参数
    deviceParams_.biasVoltage = 0.1;
    deviceParams_.samplingRate = 250000.0;
    deviceParams_.gain = 1;
    deviceParams_.filterMode = "Low Pass";
    deviceParams_.autoRange = true;
    
    // 扫描可用设备
    scanDevices();
}

DeviceManagementDialog::~DeviceManagementDialog()
{
    if (isConnected_) {
        disconnectFromDevice();
    }
}

void DeviceManagementDialog::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    mainTabWidget_ = new QTabWidget;
    mainLayout->addWidget(mainTabWidget_);
    
    setupDeviceListPanel();
    setupConnectionPanel();
    setupConfigurationPanel();
    setupStatusPanel();
    setupCalibrationPanel();
    setupMaintenancePanel();
    
    // 添加底部按钮
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    QPushButton* closeBtn = new QPushButton("关闭");
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeBtn);
    mainLayout->addLayout(buttonLayout);
}

void DeviceManagementDialog::setupDeviceListPanel()
{
    QWidget* deviceListWidget = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(deviceListWidget);
    
    deviceListGroup_ = new QGroupBox("可用设备");
    QVBoxLayout* groupLayout = new QVBoxLayout(deviceListGroup_);
    
    // 设备表格
    deviceTable_ = new QTableWidget(0, 5);
    QStringList headers = {"设备名称", "型号", "序列号", "IP地址", "状态"};
    deviceTable_->setHorizontalHeaderLabels(headers);
    deviceTable_->horizontalHeader()->setStretchLastSection(true);
    deviceTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    // 控制按钮
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    scanBtn_ = new QPushButton("扫描设备");
    refreshBtn_ = new QPushButton("刷新列表");
    deviceCountLabel_ = new QLabel("设备数量: 0");
    
    buttonLayout->addWidget(scanBtn_);
    buttonLayout->addWidget(refreshBtn_);
    buttonLayout->addStretch();
    buttonLayout->addWidget(deviceCountLabel_);
    
    groupLayout->addWidget(deviceTable_);
    groupLayout->addLayout(buttonLayout);
    
    layout->addWidget(deviceListGroup_);
    
    // 连接信号
    connect(scanBtn_, &QPushButton::clicked, this, &DeviceManagementDialog::scanDevices);
    connect(refreshBtn_, &QPushButton::clicked, this, &DeviceManagementDialog::updateDeviceList);
    connect(deviceTable_, &QTableWidget::itemSelectionChanged, this, &DeviceManagementDialog::onDeviceSelectionChanged);
    
    mainTabWidget_->addTab(deviceListWidget, "设备列表");
}

void DeviceManagementDialog::setupConnectionPanel()
{
    QWidget* connectionWidget = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(connectionWidget);
    
    connectionGroup_ = new QGroupBox("设备连接");
    QGridLayout* gridLayout = new QGridLayout(connectionGroup_);
    
    // 设备选择
    gridLayout->addWidget(new QLabel("选择设备:"), 0, 0);
    deviceCombo_ = new QComboBox;
    gridLayout->addWidget(deviceCombo_, 0, 1, 1, 2);
    
    // IP地址和端口
    gridLayout->addWidget(new QLabel("IP地址:"), 1, 0);
    ipAddressEdit_ = new QLineEdit("192.168.1.100");
    gridLayout->addWidget(ipAddressEdit_, 1, 1);
    
    gridLayout->addWidget(new QLabel("端口:"), 1, 2);
    portSpin_ = new QSpinBox;
    portSpin_->setRange(1, 65535);
    portSpin_->setValue(5025);
    gridLayout->addWidget(portSpin_, 1, 3);
    
    // 连接按钮
    connectBtn_ = new QPushButton("连接设备");
    disconnectBtn_ = new QPushButton("断开连接");
    testBtn_ = new QPushButton("测试连接");
    
    gridLayout->addWidget(connectBtn_, 2, 0);
    gridLayout->addWidget(disconnectBtn_, 2, 1);
    gridLayout->addWidget(testBtn_, 2, 2);
    
    // 连接状态
    gridLayout->addWidget(new QLabel("连接状态:"), 3, 0);
    connectionStatusLabel_ = new QLabel("未连接");
    connectionStatusLabel_->setStyleSheet("QLabel { color: red; font-weight: bold; }");
    gridLayout->addWidget(connectionStatusLabel_, 3, 1, 1, 3);
    
    layout->addWidget(connectionGroup_);
    layout->addStretch();
    
    // 连接信号
    connect(connectBtn_, &QPushButton::clicked, this, &DeviceManagementDialog::connectToDevice);
    connect(disconnectBtn_, &QPushButton::clicked, this, &DeviceManagementDialog::disconnectFromDevice);
    connect(testBtn_, &QPushButton::clicked, this, &DeviceManagementDialog::testConnection);
    connect(deviceCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DeviceManagementDialog::onDeviceSelectionChanged);
    
    mainTabWidget_->addTab(connectionWidget, "设备连接");
}

void DeviceManagementDialog::setupConfigurationPanel()
{
    QWidget* configWidget = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(configWidget);
    
    configGroup_ = new QGroupBox("设备配置");
    QGridLayout* gridLayout = new QGridLayout(configGroup_);
    
    // 偏置电压
    gridLayout->addWidget(new QLabel("偏置电压 (V):"), 0, 0);
    biasVoltageSpin_ = new QDoubleSpinBox;
    biasVoltageSpin_->setRange(-2.0, 2.0);
    biasVoltageSpin_->setSingleStep(0.01);
    biasVoltageSpin_->setDecimals(3);
    biasVoltageSpin_->setValue(deviceParams_.biasVoltage);
    gridLayout->addWidget(biasVoltageSpin_, 0, 1);
    
    // 采样率
    gridLayout->addWidget(new QLabel("采样率 (Hz):"), 1, 0);
    samplingRateSpin_ = new QDoubleSpinBox;
    samplingRateSpin_->setRange(1000, 1000000);
    samplingRateSpin_->setValue(deviceParams_.samplingRate);
    gridLayout->addWidget(samplingRateSpin_, 1, 1);
    
    // 增益
    gridLayout->addWidget(new QLabel("增益:"), 2, 0);
    gainSpin_ = new QSpinBox;
    gainSpin_->setRange(1, 1000);
    gainSpin_->setValue(deviceParams_.gain);
    gridLayout->addWidget(gainSpin_, 2, 1);
    
    // 滤波模式
    gridLayout->addWidget(new QLabel("滤波模式:"), 3, 0);
    filterModeCombo_ = new QComboBox;
    filterModeCombo_->addItems({"Low Pass", "High Pass", "Band Pass", "Band Stop"});
    filterModeCombo_->setCurrentText(deviceParams_.filterMode);
    gridLayout->addWidget(filterModeCombo_, 3, 1);
    
    // 自动量程
    autoRangeCheck_ = new QCheckBox("自动量程");
    autoRangeCheck_->setChecked(deviceParams_.autoRange);
    gridLayout->addWidget(autoRangeCheck_, 4, 0, 1, 2);
    
    // 配置按钮
    QHBoxLayout* configButtonLayout = new QHBoxLayout;
    saveConfigBtn_ = new QPushButton("保存配置");
    loadConfigBtn_ = new QPushButton("加载配置");
    resetConfigBtn_ = new QPushButton("重置配置");
    
    configButtonLayout->addWidget(saveConfigBtn_);
    configButtonLayout->addWidget(loadConfigBtn_);
    configButtonLayout->addWidget(resetConfigBtn_);
    configButtonLayout->addStretch();
    
    gridLayout->addLayout(configButtonLayout, 5, 0, 1, 2);
    
    layout->addWidget(configGroup_);
    layout->addStretch();
    
    // 连接信号
    connect(biasVoltageSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &DeviceManagementDialog::onParameterChanged);
    connect(samplingRateSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &DeviceManagementDialog::onParameterChanged);
    connect(gainSpin_, QOverload<int>::of(&QSpinBox::valueChanged), this, &DeviceManagementDialog::onParameterChanged);
    connect(filterModeCombo_, QOverload<const QString&>::of(&QComboBox::currentTextChanged), this, &DeviceManagementDialog::onParameterChanged);
    connect(autoRangeCheck_, &QCheckBox::toggled, this, &DeviceManagementDialog::onParameterChanged);
    
    connect(saveConfigBtn_, &QPushButton::clicked, this, &DeviceManagementDialog::saveDeviceConfig);
    connect(loadConfigBtn_, &QPushButton::clicked, this, &DeviceManagementDialog::loadDeviceConfig);
    connect(resetConfigBtn_, &QPushButton::clicked, this, &DeviceManagementDialog::onParameterChanged);
    
    mainTabWidget_->addTab(configWidget, "设备配置");
}

void DeviceManagementDialog::setupStatusPanel()
{
    QWidget* statusWidget = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(statusWidget);
    
    statusGroup_ = new QGroupBox("设备状态");
    QGridLayout* gridLayout = new QGridLayout(statusGroup_);
    
    // 设备信息
    gridLayout->addWidget(new QLabel("设备型号:"), 0, 0);
    deviceModelLabel_ = new QLabel("未连接");
    gridLayout->addWidget(deviceModelLabel_, 0, 1);
    
    gridLayout->addWidget(new QLabel("固件版本:"), 1, 0);
    firmwareVersionLabel_ = new QLabel("未知");
    gridLayout->addWidget(firmwareVersionLabel_, 1, 1);
    
    gridLayout->addWidget(new QLabel("序列号:"), 2, 0);
    serialNumberLabel_ = new QLabel("未知");
    gridLayout->addWidget(serialNumberLabel_, 2, 1);
    
    // 实时状态
    gridLayout->addWidget(new QLabel("温度:"), 3, 0);
    temperatureLabel_ = new QLabel("-- °C");
    gridLayout->addWidget(temperatureLabel_, 3, 1);
    
    gridLayout->addWidget(new QLabel("电压:"), 4, 0);
    voltageLabel_ = new QLabel("-- V");
    gridLayout->addWidget(voltageLabel_, 4, 1);
    
    gridLayout->addWidget(new QLabel("电流:"), 5, 0);
    currentLabel_ = new QLabel("-- A");
    gridLayout->addWidget(currentLabel_, 5, 1);
    
    // 信号质量
    gridLayout->addWidget(new QLabel("信号质量:"), 6, 0);
    signalQualityBar_ = new QProgressBar;
    signalQualityBar_->setRange(0, 100);
    signalQualityBar_->setValue(0);
    gridLayout->addWidget(signalQualityBar_, 6, 1);
    
    layout->addWidget(statusGroup_);
    
    // 状态日志
    QGroupBox* logGroup = new QGroupBox("状态日志");
    QVBoxLayout* logLayout = new QVBoxLayout(logGroup);
    statusLog_ = new QTextEdit;
    statusLog_->setMaximumHeight(200);
    statusLog_->setReadOnly(true);
    logLayout->addWidget(statusLog_);
    
    layout->addWidget(logGroup);
    
    mainTabWidget_->addTab(statusWidget, "设备状态");
}

void DeviceManagementDialog::setupCalibrationPanel()
{
    QWidget* calibrationWidget = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(calibrationWidget);
    
    calibrationGroup_ = new QGroupBox("设备校准");
    QVBoxLayout* groupLayout = new QVBoxLayout(calibrationGroup_);
    
    // 校准按钮
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    calibrateCurrentBtn_ = new QPushButton("电流校准");
    calibrateVoltageBtn_ = new QPushButton("电压校准");
    calibrateOffsetBtn_ = new QPushButton("偏移校准");
    
    buttonLayout->addWidget(calibrateCurrentBtn_);
    buttonLayout->addWidget(calibrateVoltageBtn_);
    buttonLayout->addWidget(calibrateOffsetBtn_);
    buttonLayout->addStretch();
    
    groupLayout->addLayout(buttonLayout);
    
    // 校准进度
    calibrationProgress_ = new QProgressBar;
    groupLayout->addWidget(calibrationProgress_);
    
    // 校准日志
    calibrationLog_ = new QTextEdit;
    calibrationLog_->setReadOnly(true);
    groupLayout->addWidget(calibrationLog_);
    
    layout->addWidget(calibrationGroup_);
    
    // 连接信号
    connect(calibrateCurrentBtn_, &QPushButton::clicked, this, &DeviceManagementDialog::calibrateDevice);
    connect(calibrateVoltageBtn_, &QPushButton::clicked, this, &DeviceManagementDialog::calibrateDevice);
    connect(calibrateOffsetBtn_, &QPushButton::clicked, this, &DeviceManagementDialog::calibrateDevice);
    
    mainTabWidget_->addTab(calibrationWidget, "设备校准");
}

void DeviceManagementDialog::setupMaintenancePanel()
{
    QWidget* maintenanceWidget = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(maintenanceWidget);
    
    maintenanceGroup_ = new QGroupBox("设备维护");
    QVBoxLayout* groupLayout = new QVBoxLayout(maintenanceGroup_);
    
    // 维护按钮
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    updateFirmwareBtn_ = new QPushButton("更新固件");
    resetDeviceBtn_ = new QPushButton("重置设备");
    diagnosticsBtn_ = new QPushButton("设备诊断");
    exportLogsBtn_ = new QPushButton("导出日志");
    
    buttonLayout->addWidget(updateFirmwareBtn_);
    buttonLayout->addWidget(resetDeviceBtn_);
    buttonLayout->addWidget(diagnosticsBtn_);
    buttonLayout->addWidget(exportLogsBtn_);
    
    groupLayout->addLayout(buttonLayout);
    
    // 维护进度
    maintenanceProgress_ = new QProgressBar;
    groupLayout->addWidget(maintenanceProgress_);
    
    // 维护日志
    maintenanceLog_ = new QTextEdit;
    maintenanceLog_->setReadOnly(true);
    groupLayout->addWidget(maintenanceLog_);
    
    layout->addWidget(maintenanceGroup_);
    
    // 连接信号
    connect(updateFirmwareBtn_, &QPushButton::clicked, this, &DeviceManagementDialog::updateFirmware);
    connect(resetDeviceBtn_, &QPushButton::clicked, this, &DeviceManagementDialog::resetDevice);
    connect(exportLogsBtn_, &QPushButton::clicked, this, &DeviceManagementDialog::saveDeviceConfig);
    
    mainTabWidget_->addTab(maintenanceWidget, "设备维护");
}

// 槽函数实现
void DeviceManagementDialog::scanDevices()
{
    statusLog_->append("正在扫描设备...");
    
    // 模拟设备扫描
    availableDevices_.clear();
    
    DeviceInfo device1;
    device1.name = "Keithley 6487";
    device1.model = "6487";
    device1.serialNumber = "SN001234";
    device1.ipAddress = "192.168.1.100";
    device1.port = 5025;
    device1.isConnected = false;
    device1.status = "可用";
    availableDevices_.append(device1);
    
    DeviceInfo device2;
    device2.name = "ER-4 Amplifier";
    device2.model = "ER-4";
    device2.serialNumber = "SN005678";
    device2.ipAddress = "192.168.1.101";
    device2.port = 5025;
    device2.isConnected = false;
    device2.status = "可用";
    availableDevices_.append(device2);
    
    updateDeviceList();
    statusLog_->append(QString("扫描完成，发现 %1 个设备").arg(availableDevices_.size()));
}

void DeviceManagementDialog::updateDeviceList()
{
    deviceTable_->setRowCount(availableDevices_.size());
    deviceCombo_->clear();
    
    for (int i = 0; i < availableDevices_.size(); ++i) {
        const DeviceInfo& device = availableDevices_[i];
        
        deviceTable_->setItem(i, 0, new QTableWidgetItem(device.name));
        deviceTable_->setItem(i, 1, new QTableWidgetItem(device.model));
        deviceTable_->setItem(i, 2, new QTableWidgetItem(device.serialNumber));
        deviceTable_->setItem(i, 3, new QTableWidgetItem(device.ipAddress));
        deviceTable_->setItem(i, 4, new QTableWidgetItem(device.status));
        
        deviceCombo_->addItem(device.name);
    }
    
    deviceCountLabel_->setText(QString("设备数量: %1").arg(availableDevices_.size()));
}

void DeviceManagementDialog::connectToDevice()
{
    if (deviceCombo_->currentIndex() < 0) {
        QMessageBox::warning(this, "警告", "请先选择要连接的设备");
        return;
    }
    
    statusLog_->append("正在连接设备...");
    
    // 模拟连接过程
    QApplication::processEvents();
    
    isConnected_ = true;
    currentDevice_ = availableDevices_[deviceCombo_->currentIndex()];
    currentDevice_.isConnected = true;
    
    updateConnectionStatus();
    updateStatusDisplay();
    
    statusLog_->append("设备连接成功");
}

void DeviceManagementDialog::disconnectFromDevice()
{
    if (!isConnected_) {
        return;
    }
    
    statusLog_->append("正在断开设备连接...");
    
    isConnected_ = false;
    currentDevice_.isConnected = false;
    
    updateConnectionStatus();
    updateStatusDisplay();
    
    statusLog_->append("设备已断开连接");
}

void DeviceManagementDialog::updateConnectionStatus()
{
    if (isConnected_) {
        connectionStatusLabel_->setText("已连接");
        connectionStatusLabel_->setStyleSheet("QLabel { color: green; font-weight: bold; }");
        connectBtn_->setEnabled(false);
        disconnectBtn_->setEnabled(true);
    } else {
        connectionStatusLabel_->setText("未连接");
        connectionStatusLabel_->setStyleSheet("QLabel { color: red; font-weight: bold; }");
        connectBtn_->setEnabled(true);
        disconnectBtn_->setEnabled(false);
    }
}

void DeviceManagementDialog::updateStatusDisplay()
{
    if (isConnected_) {
        deviceModelLabel_->setText(currentDevice_.model);
        firmwareVersionLabel_->setText("v1.2.3");
        serialNumberLabel_->setText(currentDevice_.serialNumber);
        temperatureLabel_->setText("25.3 °C");
        voltageLabel_->setText("0.100 V");
        currentLabel_->setText("1.23 nA");
        signalQualityBar_->setValue(85);
    } else {
        deviceModelLabel_->setText("未连接");
        firmwareVersionLabel_->setText("未知");
        serialNumberLabel_->setText("未知");
        temperatureLabel_->setText("-- °C");
        voltageLabel_->setText("-- V");
        currentLabel_->setText("-- A");
        signalQualityBar_->setValue(0);
    }
}

void DeviceManagementDialog::refreshDeviceStatus()
{
    if (isConnected_) {
        // 模拟状态更新
        static int counter = 0;
        counter++;
        
        double temp = 25.0 + (counter % 10) * 0.1;
        double voltage = deviceParams_.biasVoltage + (counter % 5) * 0.001;
        double current = 1.0 + (counter % 20) * 0.1;
        int quality = 80 + (counter % 20);
        
        temperatureLabel_->setText(QString("%1 °C").arg(temp, 0, 'f', 1));
        voltageLabel_->setText(QString("%1 V").arg(voltage, 0, 'f', 3));
        currentLabel_->setText(QString("%1 nA").arg(current, 0, 'f', 2));
        signalQualityBar_->setValue(quality);
    }
}

void DeviceManagementDialog::testConnection()
{
    statusLog_->append("正在测试连接...");
    
    // 模拟连接测试
    QApplication::processEvents();
    
    if (deviceCombo_->currentIndex() >= 0) {
        statusLog_->append("连接测试成功");
        QMessageBox::information(this, "测试结果", "设备连接测试成功");
    } else {
        statusLog_->append("连接测试失败");
        QMessageBox::warning(this, "测试结果", "设备连接测试失败");
    }
}

void DeviceManagementDialog::calibrateDevice()
{
    if (!isConnected_) {
        QMessageBox::warning(this, "警告", "请先连接设备");
        return;
    }
    
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    QString calibrationType;
    
    if (btn == calibrateCurrentBtn_) {
        calibrationType = "电流";
    } else if (btn == calibrateVoltageBtn_) {
        calibrationType = "电压";
    } else if (btn == calibrateOffsetBtn_) {
        calibrationType = "偏移";
    }
    
    calibrationLog_->append(QString("开始%1校准...").arg(calibrationType));
    calibrationProgress_->setValue(0);
    
    // 模拟校准过程
    for (int i = 0; i <= 100; i += 10) {
        calibrationProgress_->setValue(i);
        QApplication::processEvents();
        QThread::msleep(100);
    }
    
    calibrationLog_->append(QString("%1校准完成").arg(calibrationType));
}

void DeviceManagementDialog::resetDevice()
{
    if (!isConnected_) {
        QMessageBox::warning(this, "警告", "请先连接设备");
        return;
    }
    
    int ret = QMessageBox::question(this, "确认", "确定要重置设备吗？这将恢复所有默认设置。");
    if (ret != QMessageBox::Yes) {
        return;
    }
    
    maintenanceLog_->append("正在重置设备...");
    maintenanceProgress_->setValue(0);
    
    // 模拟重置过程
    for (int i = 0; i <= 100; i += 20) {
        maintenanceProgress_->setValue(i);
        QApplication::processEvents();
        QThread::msleep(200);
    }
    
    maintenanceLog_->append("设备重置完成");
    
    // 重置参数到默认值
    deviceParams_.biasVoltage = 0.1;
    deviceParams_.samplingRate = 250000.0;
    deviceParams_.gain = 1;
    deviceParams_.filterMode = "Low Pass";
    deviceParams_.autoRange = true;
    
    updateDeviceParameters();
}

void DeviceManagementDialog::updateFirmware()
{
    if (!isConnected_) {
        QMessageBox::warning(this, "警告", "请先连接设备");
        return;
    }
    
    QString fileName = QFileDialog::getOpenFileName(this, "选择固件文件", "", "固件文件 (*.bin *.hex)");
    if (fileName.isEmpty()) {
        return;
    }
    
    maintenanceLog_->append("正在更新固件...");
    maintenanceProgress_->setValue(0);
    
    // 模拟固件更新过程
    for (int i = 0; i <= 100; i += 5) {
        maintenanceProgress_->setValue(i);
        QApplication::processEvents();
        QThread::msleep(100);
    }
    
    maintenanceLog_->append("固件更新完成");
    firmwareVersionLabel_->setText("v1.3.0");
}

void DeviceManagementDialog::saveDeviceConfig()
{
    QString fileName = QFileDialog::getSaveFileName(this, "保存设备配置", 
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/device_config.json",
        "JSON文件 (*.json)");
    
    if (fileName.isEmpty()) {
        return;
    }
    
    QJsonObject config;
    config["biasVoltage"] = deviceParams_.biasVoltage;
    config["samplingRate"] = deviceParams_.samplingRate;
    config["gain"] = deviceParams_.gain;
    config["filterMode"] = deviceParams_.filterMode;
    config["autoRange"] = deviceParams_.autoRange;
    
    QJsonDocument doc(config);
    
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        statusLog_->append("配置已保存到: " + fileName);
    } else {
        QMessageBox::warning(this, "错误", "无法保存配置文件");
    }
}

void DeviceManagementDialog::loadDeviceConfig()
{
    QString fileName = QFileDialog::getOpenFileName(this, "加载设备配置",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        "JSON文件 (*.json)");
    
    if (fileName.isEmpty()) {
        return;
    }
    
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "错误", "无法读取配置文件");
        return;
    }
    
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject config = doc.object();
    
    deviceParams_.biasVoltage = config["biasVoltage"].toDouble();
    deviceParams_.samplingRate = config["samplingRate"].toDouble();
    deviceParams_.gain = config["gain"].toInt();
    deviceParams_.filterMode = config["filterMode"].toString();
    deviceParams_.autoRange = config["autoRange"].toBool();
    
    updateDeviceParameters();
    statusLog_->append("配置已从文件加载: " + fileName);
}

void DeviceManagementDialog::updateDeviceParameters()
{
    biasVoltageSpin_->setValue(deviceParams_.biasVoltage);
    samplingRateSpin_->setValue(deviceParams_.samplingRate);
    gainSpin_->setValue(deviceParams_.gain);
    filterModeCombo_->setCurrentText(deviceParams_.filterMode);
    autoRangeCheck_->setChecked(deviceParams_.autoRange);
}

void DeviceManagementDialog::onDeviceSelectionChanged()
{
    int index = deviceCombo_->currentIndex();
    if (index >= 0 && index < availableDevices_.size()) {
        const DeviceInfo& device = availableDevices_[index];
        ipAddressEdit_->setText(device.ipAddress);
        portSpin_->setValue(device.port);
    }
}

void DeviceManagementDialog::onParameterChanged()
{
    deviceParams_.biasVoltage = biasVoltageSpin_->value();
    deviceParams_.samplingRate = samplingRateSpin_->value();
    deviceParams_.gain = gainSpin_->value();
    deviceParams_.filterMode = filterModeCombo_->currentText();
    deviceParams_.autoRange = autoRangeCheck_->isChecked();
    
    if (isConnected_) {
        statusLog_->append("设备参数已更新");
    }
}