#pragma once
#include <QObject>
#include <QWidget>
#include <QTimer>
#include <QTextStream>
#include <QFile>
#include <vector>
#include <memory>
#include "../hardware/DeviceInterface.h"
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QDateTimeAxis>

class QComboBox;
class QPushButton;
class QLabel;
class QLineEdit;
class QTableWidget;
class QGroupBox;
class QCheckBox;
class QSpinBox;
class QDoubleSpinBox;
class QProgressBar;
class QTextEdit;
class QSplitter;
class DataAnalysisDialog;
class DeviceManagementDialog;
class DataExportDialog;

class CollectionPage : public QWidget {
    Q_OBJECT
public:
    explicit CollectionPage(QWidget* parent = nullptr);

private:
    // 基础设备控制
    QComboBox* deviceCombo_;
    QPushButton* refreshBtn_; // 获取设备序列号
    QPushButton* savePathBtn_;
    QPushButton* connectBtn_;
    QPushButton* disconnectBtn_;
    QPushButton* startBtn_;
    QLabel* statusLabel_;
    QLabel* connIndicator_;
    QLabel* storageIndicator_;
    QString saveDir_;
    bool connected_ = false;
    bool storing_ = false;
    
    // 设备管理
    std::unique_ptr<DeviceInterface> device_;
    std::vector<DeviceInfo> availableDevices_;
    QGroupBox* deviceStatusGroup_;
    QLabel* serialNumberLabel_;
    QLabel* firmwareVersionLabel_;
    QLabel* temperatureLabel_;
    QLabel* voltageLabel_;
    QLabel* currentLabel_;
    QPushButton* selfTestBtn_;
    QPushButton* calibrateBtn_;
    QTextEdit* deviceLogEdit_;
    QProgressBar* operationProgress_;
    
    // 偏置电压控制
    QGroupBox* biasGroup_;
    QLineEdit* biasVoltageEdit_;
    QPushButton* applyBiasBtn_;
    QLabel* currentBiasLabel_;
    
    // 纳米孔径计算器
    QGroupBox* poreCalcGroup_;
    QLineEdit* conductanceEdit_;
    QLineEdit* lengthEdit_;
    QLineEdit* conductivityEdit_;
    QLabel* diameterResultLabel_;
    
    // 基线补偿
    QGroupBox* baselineGroup_;
    QCheckBox* enableBaselineCompensation_;
    QLabel* baselineValueLabel_;
    QLabel* rmsNoiseLabel_;
    
    // 事件特征提取
    QGroupBox* eventGroup_;
    QLineEdit* thresholdEdit_;
    QLineEdit* minDurationEdit_;
    QCheckBox* enableEventDetection_;
    
    // 事件数据表格
    QTableWidget* eventTable_;
    
    // 数据采集相关
    QTimer* dataTimer_;
    QFile* dataFile_;
    QTextStream* dataStream_;
    std::vector<double> timeData_;
    std::vector<double> currentData_;
    int dataPointCount_;
    double startTime_;
    
    // 实时数据分析
    double currentBaseline_;
    double currentRmsNoise_;
    std::vector<double> eventTimes_;
    std::vector<double> eventAmplitudes_;
    std::vector<double> eventDurations_;
    
    // 高级数据导出和分析
    QGroupBox* exportGroup_;
    QPushButton* exportRawDataBtn_;
    QPushButton* exportEventsBtn_;
    QPushButton* exportStatisticsBtn_;
    QPushButton* exportReportBtn_;
    QComboBox* exportFormatCombo_;
    QCheckBox* includeMetadataCheck_;
    QCheckBox* compressDataCheck_;
    
    // 数据分析参数
    QGroupBox* analysisGroup_;
    QDoubleSpinBox* samplingRateSpinBox_;
    QDoubleSpinBox* filterCutoffSpinBox_;
    QSpinBox* movingAverageWindowSpinBox_;
    QCheckBox* enableFilteringCheck_;
    QCheckBox* enableDenoisingCheck_;
    QPushButton* analyzeDataBtn_;
    QPushButton* generateReportBtn_;
    
    // 统计数据存储
    struct DataStatistics {
        double mean = 0.0;
        double std = 0.0;
        double min = 0.0;
        double max = 0.0;
        double median = 0.0;
        double rms = 0.0;
        size_t eventCount = 0;
        double eventRate = 0.0;
        double snr = 0.0;
        double baseline = 0.0;
    };
    DataStatistics currentStats_;
    
    // 实时数据可视化图表
    QSplitter* mainSplitter_;
    QChartView* realTimeChart_;
    QChart* chart_;
    QLineSeries* currentSeries_;
    QLineSeries* baselineSeries_;
    QScatterSeries* eventSeries_;
    QValueAxis* timeAxis_;
    QValueAxis* currentAxis_;
    
    // 统计图表
    QChartView* statisticsChart_;
    QChart* statsChart_;
    QLineSeries* meanSeries_;
    QLineSeries* rmsNoiseSeries_;
    QLineSeries* eventRateSeries_;
    
    // 图表控制组件
    QGroupBox* chartControlGroup_;
    QPushButton* pauseChartBtn_;
    QPushButton* resetZoomBtn_;
    QPushButton* exportChartBtn_;
    QCheckBox* autoScaleCheck_;
    bool chartsPaused_ = false;
    
    // 折叠控件
    QWidget* controlsContainer_;
    QPushButton* toggleControlsBtn_;
    bool controlsVisible_ = true;

private slots:
    void collectData();

private:
    void refreshDevices();
    void chooseSavePath();
    void connectDevice();
    void disconnectDevice();
    void startAcquisition();
    void stopAcquisition();
    void updateIndicators();
    
    // 高级功能方法
    void applyBiasVoltage();
    void calculatePoreDiameter();
    void performBaselineCompensation();
    void calculateBaseline();
    void detectEvents();
    void addEventToTable(double time, double amplitude, double duration);
    void setupAdvancedUI();
    
    // 设备管理方法
    void setupDeviceManagement();
    void updateDeviceStatus();
    void performSelfTest();
    void performCalibration();
    void logDeviceMessage(const QString& message);
    void onDeviceConnected();
    void onDeviceDisconnected();
    void updateDeviceInfo();
    
    // 高级数据导出和分析方法
    void setupDataExportUI();
    void setupDataAnalysisUI();
    void exportRawData();
    void exportEventData();
    void exportStatistics();
    void exportFullReport();
    void analyzeCurrentData();
    void generateAnalysisReport();
    DataStatistics calculateDataStatistics();
    std::vector<double> applyDataFiltering(const std::vector<double>& data);
    std::vector<double> applyDenoising(const std::vector<double>& data);
    QString formatDataForExport(const std::vector<double>& timeData, 
                               const std::vector<double>& currentData, 
                               const QString& format);
    QString generateMetadata();
    void compressExportFile(const QString& fileName);
    
    // 图表相关方法
    void setupVisualization();
    void updateRealTimeChart(double time, double current);
    void updateStatisticsChart();
    void resetChartData();
    void exportChartImage(const QString& fileName);
    
    // 图表控制方法
    void setupChartControls();
    void toggleChartPause();
    void resetChartZoom();
    void exportCurrentChart();
    void toggleAutoScale();
    void toggleControlsVisibility();
    void openDataAnalysisDialog();
    void openDeviceManagementDialog();
    void openDataExportDialog();
};