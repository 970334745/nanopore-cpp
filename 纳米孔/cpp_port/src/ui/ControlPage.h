#pragma once
#include <QWidget>
#include <QString>
#include <QStringList>
#include <QColor>
#include <QTimer>
#include <vector>

#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QPieSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLogValueAxis>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QLegend>

class QLabel;
class QPushButton;
class QComboBox;
class QLineEdit;
class QFrame;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QGroupBox;

class ControlPage : public QWidget {
    Q_OBJECT
public:
    explicit ControlPage(QWidget* parent = nullptr);

private slots:
    void onModeChanged();
    void onStartProcess();
    void onStopProcess();
    void updateData();
    // 打开独立图表窗口
    void openHistogramWindow();
    void openScatterWindow();
    void openSpectrumWindow();
    void openStatisticsWindow();
    void openEventWindow();

private:
    // 顶部设备/存储路径区域
    QPushButton* btnCheckDevices_{};
    QComboBox* comboDevices_{};
    QPushButton* btnConnect_{};
    QLineEdit* deviceStatusEdit_{};
    QPushButton* btnSavePath_{};
    QLineEdit* savePathEdit_{};
    
    // 模式选择区域
    QComboBox* modeCombo_{};
    QPushButton* btnStart_{};
    QPushButton* btnStop_{};
    
    // 芯片检查参数
    QLineEdit* scanVoltageEdit_{};
    QLineEdit* avgCurrentEdit_{};
    QLineEdit* avgImpedanceEdit_{};
    QPushButton* btnChipCheck_{};
    
    // 打孔参数
    QLineEdit* poreVoltageEdit_{};
    QLineEdit* solutionConductivityEdit_{};
    QLineEdit* sinThicknessEdit_{};
    QLineEdit* thresholdCurrentEdit_{};
    QLineEdit* pulseTimeEdit_{};
    QPushButton* btnStartPoring_{};
    
    // 扩孔参数
    QLineEdit* expandVoltageEdit_{};
    QLineEdit* targetApertureEdit_{};
    QLineEdit* expandTimeEdit_{};
    QPushButton* btnStartExpanding_{};
    
    // 高级设置
    QComboBox* currentRangeCombo_{};
    QComboBox* samplingRateCombo_{};
    QComboBox* probeTypeCombo_{};
    QComboBox* sampleTypeCombo_{};
    QLineEdit* baselineEdit_{};
    QLineEdit* rmsNoiseEdit_{};
    
    // 实时数据采集
    QTimer* dataTimer_{};
    std::vector<double> timeData_{};
    std::vector<double> currentData_{};
    int dataPointCount_{};
    bool isCollecting_{};

    // 右侧流程指示灯
    QLabel* indicatorSeal_{};
    QLabel* indicatorMake_{};
    QLabel* indicatorExpand_{};
    QLabel* indicatorFinish_{};

    // 下方黑色占位图
    QChartView* plotCBD_{};
    QChartView* plotCBDVoltage_{};     // CBD电压子图
    QChartView* plotCBDCurrent_{};     // CBD电流子图
    QChartView* plotCBDResistance_{};  // CBD电阻子图
    QChartView* plotCBDSigma_{};       // CBD Sigma子图
    QChartView* plotCheck_{};
    QChartView* plotExpand_{};
    QChartView* plotExpandVoltage_{};  // 扩孔电压子图
    QChartView* plotExpandPore_{};     // 扩孔孔径子图
    
    // 新增的多图表显示
    QChartView* plotHistogram_{};      // 直方图
    QChartView* plotScatter_{};        // 散点图
    QChartView* plotSpectrum_{};       // 频谱图
    QChartView* plotStatistics_{};     // 统计图表
    QChartView* plotEventAnalysis_{};  // 事件分析图
    QChartView* plotRealTime3D_{};     // 3D实时图
    
    // 图表数据存储
    std::vector<double> histogramData_{};
    std::vector<double> frequencyData_{};
    std::vector<double> amplitudeData_{};
    std::vector<double> eventDurations_{};
    std::vector<double> eventAmplitudes_{};
    std::vector<int> eventCounts_{};
    
    // 图表控制
    QPushButton* btnToggleCharts_{};
    QComboBox* chartTypeCombo_{};
    QPushButton* btnExportChart_{};
    QPushButton* btnResetZoom_{};
    QLabel* chartStatsLabel_{};
    QPushButton* btnZoomIn_{};
    QPushButton* btnZoomOut_{};
    QPushButton* btnPanMode_{};
    QPushButton* btnSelectMode_{};
    QLabel* mousePositionLabel_{};
    QLabel* selectedDataLabel_{};

    // 布局成员，用于将绝对坐标改为更整齐的网格/行列布局
    QVBoxLayout* rootLayout_{};
    QHBoxLayout* topAreaLayout_{};  // 顶部区域的水平布局
    QVBoxLayout* topRowLayout_{};   // 面板的垂直布局
    QHBoxLayout* bottomRowLayout_{};

    void createTopBoxes();
    void createIndicators();
    void createPlots();
    void createAdvancedCharts();
    QChartView* createAdvancedChart(const QString& title, const QString& xLabel, const QString& yLabel);
    void updatePlotData();
    void updateHistogram();
    void updateScatterPlot();
    void updateSpectrumPlot();
    void updateStatisticsPlot();
    void updateEventAnalysis();
    void exportChartData();
    void resetChartZoom();
    void onChartTypeChanged();
    void calculateStatistics();
    void onZoomIn();
    void onZoomOut();
    void onPanModeToggle();
    void onSelectModeToggle();
    void onChartMouseMove(const QPointF& position);
    void onChartMousePress(const QPointF& position);
    void setupChartInteractions(QChartView* chartView);
    // Removed absolute positioning helper functions - using proper layout management instead
};