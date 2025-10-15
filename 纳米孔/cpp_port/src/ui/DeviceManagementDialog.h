#ifndef DEVICEMANAGEMENTDIALOG_H
#define DEVICEMANAGEMENTDIALOG_H

#include <QDialog>
#include <QWidget>
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
#include <QTabWidget>
#include <QTimer>
#include <QMessageBox>
#include <QSplitter>

class DeviceManagementDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeviceManagementDialog(QWidget *parent = nullptr);
    ~DeviceManagementDialog();

private slots:
    void scanDevices();
    void connectToDevice();
    void disconnectFromDevice();
    void refreshDeviceStatus();
    void calibrateDevice();
    void resetDevice();
    void updateFirmware();
    void saveDeviceConfig();
    void loadDeviceConfig();
    void testConnection();
    void onDeviceSelectionChanged();
    void onParameterChanged();

private:
    void setupUI();
    void setupDeviceListPanel();
    void setupConnectionPanel();
    void setupConfigurationPanel();
    void setupStatusPanel();
    void setupCalibrationPanel();
    void setupMaintenancePanel();
    
    void updateDeviceList();
    void updateConnectionStatus();
    void updateDeviceParameters();
    void updateStatusDisplay();
    
    // UI组件
    QTabWidget* mainTabWidget_;
    
    // 设备列表面板
    QGroupBox* deviceListGroup_;
    QTableWidget* deviceTable_;
    QPushButton* scanBtn_;
    QPushButton* refreshBtn_;
    QLabel* deviceCountLabel_;
    
    // 连接面板
    QGroupBox* connectionGroup_;
    QComboBox* deviceCombo_;
    QLineEdit* ipAddressEdit_;
    QSpinBox* portSpin_;
    QPushButton* connectBtn_;
    QPushButton* disconnectBtn_;
    QPushButton* testBtn_;
    QLabel* connectionStatusLabel_;
    
    // 配置面板
    QGroupBox* configGroup_;
    QDoubleSpinBox* biasVoltageSpin_;
    QDoubleSpinBox* samplingRateSpin_;
    QSpinBox* gainSpin_;
    QComboBox* filterModeCombo_;
    QCheckBox* autoRangeCheck_;
    QPushButton* saveConfigBtn_;
    QPushButton* loadConfigBtn_;
    QPushButton* resetConfigBtn_;
    
    // 状态面板
    QGroupBox* statusGroup_;
    QLabel* deviceModelLabel_;
    QLabel* firmwareVersionLabel_;
    QLabel* serialNumberLabel_;
    QLabel* temperatureLabel_;
    QLabel* voltageLabel_;
    QLabel* currentLabel_;
    QProgressBar* signalQualityBar_;
    QTextEdit* statusLog_;
    
    // 校准面板
    QGroupBox* calibrationGroup_;
    QPushButton* calibrateCurrentBtn_;
    QPushButton* calibrateVoltageBtn_;
    QPushButton* calibrateOffsetBtn_;
    QProgressBar* calibrationProgress_;
    QTextEdit* calibrationLog_;
    
    // 维护面板
    QGroupBox* maintenanceGroup_;
    QPushButton* updateFirmwareBtn_;
    QPushButton* resetDeviceBtn_;
    QPushButton* diagnosticsBtn_;
    QPushButton* exportLogsBtn_;
    QProgressBar* maintenanceProgress_;
    QTextEdit* maintenanceLog_;
    
    // 数据成员
    struct DeviceInfo {
        QString name;
        QString model;
        QString serialNumber;
        QString ipAddress;
        int port;
        bool isConnected;
        QString status;
    };
    
    QList<DeviceInfo> availableDevices_;
    DeviceInfo currentDevice_;
    bool isConnected_;
    QTimer* statusUpdateTimer_;
    
    // 设备参数
    struct DeviceParameters {
        double biasVoltage;
        double samplingRate;
        int gain;
        QString filterMode;
        bool autoRange;
    } deviceParams_;
};

#endif // DEVICEMANAGEMENTDIALOG_H