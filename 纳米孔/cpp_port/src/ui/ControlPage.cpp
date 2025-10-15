#include "ControlPage.h"
// Qt Widgets & GUI headers (module-qualified to satisfy static analyzers)
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QFrame>
#include <QtGui/QFont>
#include <QtGui/QPalette>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtGui/QColor>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QSizePolicy>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSplitter>
#include <QtGui/QBrush>
#include <QtGui/QPen>
#include <QtCore/Qt>
#include <utility>
#include <QtCore/QTimer>
#include <cmath>
#include <random>
// Charts
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
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtCore/QTextStream>
#include <QtCore/QDateTime>
#include <algorithm>
#include <numeric>
#include <complex>

static QString squareStyle() {
    return QString::fromUtf8("QFrame{background:#f3f4f6;border:1px solid #d0d7de;border-radius:6px;}");
}

static QString chartViewStyle() {
    // 让图表视图背景与外框一致，坐标轴留给 QChart 控制
    return QString::fromUtf8("QChartView{background:#000;border:1px solid #4b4b4b;border-radius:2px;}");
}

static QString lightStyle(const QColor& c) {
    return QString::fromUtf8("QLabel{border-radius:7px;background:%1;border:1px solid #444;}").arg(c.name());
}

ControlPage::ControlPage(QWidget* parent) : QWidget(parent), dataPointCount_(0), isCollecting_(false) {
    this->setMinimumSize(1280, 800);

    // 页面统一字体，避免各控件大小不一致导致"杂乱"
    QFont baseFont(this->font());
    baseFont.setPointSize(12);
    setFont(baseFont);
    
    // 初始化数据采集定时器
    dataTimer_ = new QTimer(this);
    connect(dataTimer_, &QTimer::timeout, this, &ControlPage::updateData);
    
    // 初始化图表控件
    plotHistogram_ = nullptr;
    plotScatter_ = nullptr;
    plotSpectrum_ = nullptr;
    plotStatistics_ = nullptr;
    plotEventAnalysis_ = nullptr;
    plotRealTime3D_ = nullptr;
    
    // 初始化图表数据
    histogramData_.reserve(1000);
    frequencyData_.reserve(512);
    amplitudeData_.reserve(512);
    eventDurations_.reserve(100);
    eventAmplitudes_.reserve(100);
    eventCounts_.reserve(60); // 1分钟的数据，每秒一个点

    // 主布局：使用垂直分割器分为上下两部分
    rootLayout_ = new QVBoxLayout(this);
    rootLayout_->setContentsMargins(5, 5, 5, 5);
    rootLayout_->setSpacing(5);

    // 头部行：设备与存储路径，使用水平布局统一间距与拉伸
    {
        auto* header = new QHBoxLayout();
        header->setSpacing(10);

        auto* lblConn = new QLabel();
        lblConn->setText(QString::fromUtf8("连接设备"));
        header->addWidget(lblConn);
        btnCheckDevices_ = new QPushButton();
        btnCheckDevices_->setText(QString::fromUtf8("检查设备"));
        header->addWidget(btnCheckDevices_);

        comboDevices_ = new QComboBox();
        comboDevices_->addItem(QString::fromUtf8("未连接"));
        header->addWidget(comboDevices_, 1);

        btnConnect_ = new QPushButton();
        btnConnect_->setText(QString::fromUtf8("连接"));
        header->addWidget(btnConnect_);

        deviceStatusEdit_ = new QLineEdit();
        deviceStatusEdit_->setText(QString::fromUtf8("无连接"));
        deviceStatusEdit_->setReadOnly(true);
        deviceStatusEdit_->setFixedWidth(160);
        header->addWidget(deviceStatusEdit_);

        header->addSpacing(10);
        btnSavePath_ = new QPushButton();
        btnSavePath_->setText(QString::fromUtf8("存储路径"));
        header->addWidget(btnSavePath_);

        savePathEdit_ = new QLineEdit();
        savePathEdit_->setText(QString::fromUtf8("未设置"));
        savePathEdit_->setReadOnly(true);
        header->addWidget(savePathEdit_, 2);

        // 添加右侧弹性空间，使内容向左对齐
        header->addStretch(1);

        rootLayout_->addLayout(header);
    }

    // 模式选择区域
    {
        auto* modeLayout = new QHBoxLayout();
        modeLayout->setSpacing(10);

        auto* lblMode = new QLabel();
        lblMode->setText(QString::fromUtf8("模式选择"));
        modeLayout->addWidget(lblMode);

        modeCombo_ = new QComboBox();
        modeCombo_->addItem(QString::fromUtf8("手动打孔"));
        modeCombo_->addItem(QString::fromUtf8("自动打孔"));
        modeLayout->addWidget(modeCombo_);

        btnStart_ = new QPushButton();
        btnStart_->setText(QString::fromUtf8("开始"));
        btnStart_->setStyleSheet("background:#2E7D32;color:white;font-weight:bold;border-radius:4px;padding:8px 16px;");
        modeLayout->addWidget(btnStart_);

        btnStop_ = new QPushButton();
        btnStop_->setText(QString::fromUtf8("停止"));
        btnStop_->setStyleSheet("background:#C62828;color:white;font-weight:bold;border-radius:4px;padding:8px 16px;");
        btnStop_->setEnabled(false);
        modeLayout->addWidget(btnStop_);

        modeLayout->addStretch(1);
        rootLayout_->addLayout(modeLayout);

        // 连接信号槽
        connect(modeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ControlPage::onModeChanged);
        connect(btnStart_, &QPushButton::clicked, this, &ControlPage::onStartProcess);
        connect(btnStop_, &QPushButton::clicked, this, &ControlPage::onStopProcess);
    }

    // 创建顶部区域的水平布局
    topAreaLayout_ = new QHBoxLayout();
    topAreaLayout_->setSpacing(10);
    topAreaLayout_->setContentsMargins(5, 5, 5, 5);
    rootLayout_->addLayout(topAreaLayout_);
    
    // 左侧：控制面板滚动区域
    auto* controlScrollArea = new QScrollArea();
    controlScrollArea->setWidgetResizable(true);
    controlScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    controlScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    controlScrollArea->setMinimumHeight(300);
    controlScrollArea->setMaximumHeight(400);
    
    auto* controlWidget = new QWidget();
    topRowLayout_ = new QVBoxLayout(controlWidget);
    topRowLayout_->setSpacing(10);
    topRowLayout_->setContentsMargins(5, 5, 5, 5);
    
    controlScrollArea->setWidget(controlWidget);
    topAreaLayout_->addWidget(controlScrollArea, 3);  // 占3份
    
    // 右侧：指示灯区域（直接添加到topAreaLayout_）
    // createIndicators函数会直接添加到topAreaLayout_
    
    // 下半部分：图表区域（可滚动）
    auto* bottomWidget = new QWidget();
    auto* bottomScrollArea = new QScrollArea();
    bottomScrollArea->setWidget(bottomWidget);
    bottomScrollArea->setWidgetResizable(true);
    bottomScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    bottomScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    bottomRowLayout_ = new QHBoxLayout(bottomWidget);
    bottomRowLayout_->setSpacing(10);
    bottomRowLayout_->setContentsMargins(5, 5, 5, 5);
    
    bottomScrollArea->setWidget(bottomWidget);
    rootLayout_->addWidget(bottomScrollArea, 1);  // 占剩余空间
    
    createTopBoxes();
    createIndicators();
    createPlots();
    createAdvancedCharts();
}

void ControlPage::createTopBoxes() {
    auto makePanel = [&](const QString& title, int minHeight = 100) {
        auto* panel = new QFrame();
        panel->setStyleSheet(squareStyle());
        panel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        panel->setMinimumWidth(480);
        panel->setMinimumHeight(minHeight);
        panel->setMaximumHeight(minHeight + 30);
        
        auto* gl = new QGridLayout(panel);
        gl->setContentsMargins(8, 8, 8, 8);
        gl->setHorizontalSpacing(8);
        gl->setVerticalSpacing(6);
        
        gl->setColumnStretch(0, 1);  // 标签列
        gl->setColumnStretch(1, 3);  // 输入列
        gl->setColumnStretch(2, 1);  // 按钮列
        
        if (!title.isEmpty()) {
            auto* t = new QLabel();
            t->setText(title);
            QFont f(t->font()); 
            f.setPointSize(11); 
            f.setBold(true); 
            t->setFont(f);
            t->setAlignment(Qt::AlignCenter);
            gl->addWidget(t, 0, 0, 1, 3, Qt::AlignCenter);
        }
        return std::pair<QFrame*, QGridLayout*>{panel, gl};
    };

    const QString btnStyle = QString::fromUtf8(
        "QPushButton { background-color: #4CAF50; color: white; border: none; "
        "border-radius: 4px; font-size: 10px; font-weight: bold; padding: 4px 8px; }"
        "QPushButton:hover { background-color: #45a049; }"
        "QPushButton:pressed { background-color: #3d8b40; }"
        "QPushButton:disabled { background-color: #cccccc; color: #666666; }"
    );

    // 第一个面板：芯片检查参数
    {
        auto [panel, gl] = makePanel(QString::fromUtf8("芯片检查参数"), 150);
        
        auto* lblVolt = new QLabel(QString::fromUtf8("扫描电压(V)"));
        scanVoltageEdit_ = new QLineEdit("0.1");
        scanVoltageEdit_->setAlignment(Qt::AlignCenter);
        scanVoltageEdit_->setMinimumHeight(35);
        gl->addWidget(lblVolt, 1, 0);
        gl->addWidget(scanVoltageEdit_, 1, 1);

        auto* lblAvgI = new QLabel(QString::fromUtf8("平均电流(nA)"));
        avgCurrentEdit_ = new QLineEdit("-");
        avgCurrentEdit_->setAlignment(Qt::AlignCenter);
        avgCurrentEdit_->setMinimumHeight(35);
        avgCurrentEdit_->setReadOnly(true);
        gl->addWidget(lblAvgI, 2, 0);
        gl->addWidget(avgCurrentEdit_, 2, 1);

        auto* lblImpedance = new QLabel(QString::fromUtf8("平均阻抗(Ω)"));
        avgImpedanceEdit_ = new QLineEdit("-");
        avgImpedanceEdit_->setAlignment(Qt::AlignCenter);
        avgImpedanceEdit_->setMinimumHeight(35);
        avgImpedanceEdit_->setReadOnly(true);
        gl->addWidget(lblImpedance, 3, 0);
        gl->addWidget(avgImpedanceEdit_, 3, 1);

        btnChipCheck_ = new QPushButton(QString::fromUtf8("开始检查"));
        btnChipCheck_->setStyleSheet(btnStyle);
        gl->addWidget(btnChipCheck_, 1, 2, 3, 1);

        topRowLayout_->addWidget(panel);
    }

    // 第二个面板：打孔参数
    {
        auto [panel, gl] = makePanel(QString::fromUtf8("打孔参数"), 160);
        
        auto* lblPoreV = new QLabel(QString::fromUtf8("制造电压(V)"));
        poreVoltageEdit_ = new QLineEdit("19");
        poreVoltageEdit_->setAlignment(Qt::AlignCenter);
        poreVoltageEdit_->setMinimumHeight(35);
        gl->addWidget(lblPoreV, 1, 0);
        gl->addWidget(poreVoltageEdit_, 1, 1);

        auto* lblConductivity = new QLabel(QString::fromUtf8("溶液电导率(S/m)"));
        solutionConductivityEdit_ = new QLineEdit("11.27");
        solutionConductivityEdit_->setAlignment(Qt::AlignCenter);
        solutionConductivityEdit_->setMinimumHeight(35);
        gl->addWidget(lblConductivity, 2, 0);
        gl->addWidget(solutionConductivityEdit_, 2, 1);

        auto* lblThickness = new QLabel(QString::fromUtf8("氮化硅膜厚(nm)"));
        sinThicknessEdit_ = new QLineEdit("30");
        sinThicknessEdit_->setAlignment(Qt::AlignCenter);
        sinThicknessEdit_->setMinimumHeight(35);
        gl->addWidget(lblThickness, 3, 0);
        gl->addWidget(sinThicknessEdit_, 3, 1);

        auto* lblThresh = new QLabel(QString::fromUtf8("阈值电流(nA)"));
        thresholdCurrentEdit_ = new QLineEdit("10");
        thresholdCurrentEdit_->setAlignment(Qt::AlignCenter);
        thresholdCurrentEdit_->setMinimumHeight(35);
        gl->addWidget(lblThresh, 4, 0);
        gl->addWidget(thresholdCurrentEdit_, 4, 1);

        auto* lblPulse = new QLabel(QString::fromUtf8("脉冲时长(ms)"));
        pulseTimeEdit_ = new QLineEdit("50");
        pulseTimeEdit_->setAlignment(Qt::AlignCenter);
        pulseTimeEdit_->setMinimumHeight(35);
        gl->addWidget(lblPulse, 5, 0);
        gl->addWidget(pulseTimeEdit_, 5, 1);

        btnStartPoring_ = new QPushButton(QString::fromUtf8("开始打孔"));
        btnStartPoring_->setStyleSheet(btnStyle);
        gl->addWidget(btnStartPoring_, 1, 2, 5, 1);

        topRowLayout_->addWidget(panel);
    }

    // 第三个面板：扩孔参数
    {
        auto [panel, gl] = makePanel(QString::fromUtf8("扩孔参数"), 120);
        
        auto* lblExpandV = new QLabel(QString::fromUtf8("扩孔电压(V)"));
        expandVoltageEdit_ = new QLineEdit("5");
        expandVoltageEdit_->setAlignment(Qt::AlignCenter);
        expandVoltageEdit_->setMinimumHeight(35);
        gl->addWidget(lblExpandV, 1, 0);
        gl->addWidget(expandVoltageEdit_, 1, 1);

        auto* lblTarget = new QLabel(QString::fromUtf8("目标孔径(nm)"));
        targetApertureEdit_ = new QLineEdit("50");
        targetApertureEdit_->setAlignment(Qt::AlignCenter);
        targetApertureEdit_->setMinimumHeight(35);
        gl->addWidget(lblTarget, 2, 0);
        gl->addWidget(targetApertureEdit_, 2, 1);

        auto* lblExpandTime = new QLabel(QString::fromUtf8("扩孔时间 (s)"));
        expandTimeEdit_ = new QLineEdit("60");
        expandTimeEdit_->setAlignment(Qt::AlignCenter);
        expandTimeEdit_->setMinimumHeight(35);
        gl->addWidget(lblExpandTime, 3, 0);
        gl->addWidget(expandTimeEdit_, 3, 1);

        btnStartExpanding_ = new QPushButton(QString::fromUtf8("开始扩孔"));
        btnStartExpanding_->setStyleSheet(btnStyle);
        gl->addWidget(btnStartExpanding_, 1, 2, 3, 1);

        topRowLayout_->addWidget(panel);
    }

    // 第四个面板：高级设置
    {
        auto [panel, gl] = makePanel(QString::fromUtf8("高级设置"), 160);
        
        auto* lblCurrentRange = new QLabel(QString::fromUtf8("电流范围"));
        currentRangeCombo_ = new QComboBox();
        currentRangeCombo_->addItems({QString::fromUtf8("±1 nA"), QString::fromUtf8("±10 nA"), 
                                     QString::fromUtf8("±100 nA"), QString::fromUtf8("±1 μA")});
        currentRangeCombo_->setCurrentIndex(1);
        currentRangeCombo_->setMinimumHeight(35);
        gl->addWidget(lblCurrentRange, 1, 0);
        gl->addWidget(currentRangeCombo_, 1, 1);

        auto* lblSamplingRate = new QLabel(QString::fromUtf8("采样率"));
        samplingRateCombo_ = new QComboBox();
        samplingRateCombo_->addItems({QString::fromUtf8("1 kHz"), QString::fromUtf8("10 kHz"), 
                                     QString::fromUtf8("100 kHz"), QString::fromUtf8("1 MHz")});
        samplingRateCombo_->setCurrentIndex(2);
        samplingRateCombo_->setMinimumHeight(35);
        gl->addWidget(lblSamplingRate, 2, 0);
        gl->addWidget(samplingRateCombo_, 2, 1);

        auto* lblProbeType = new QLabel(QString::fromUtf8("探针类型"));
        probeTypeCombo_ = new QComboBox();
        probeTypeCombo_->addItems({QString::fromUtf8("标准"), QString::fromUtf8("高精度"), 
                                  QString::fromUtf8("低噪声")});
        probeTypeCombo_->setCurrentIndex(0);
        probeTypeCombo_->setMinimumHeight(35);
        gl->addWidget(lblProbeType, 3, 0);
        gl->addWidget(probeTypeCombo_, 3, 1);

        auto* lblBaseline = new QLabel(QString::fromUtf8("基线(pA)"));
        baselineEdit_ = new QLineEdit("0");
        baselineEdit_->setAlignment(Qt::AlignCenter);
        baselineEdit_->setMinimumHeight(35);
        gl->addWidget(lblBaseline, 4, 0);
        gl->addWidget(baselineEdit_, 4, 1);

        auto* lblRmsNoise = new QLabel(QString::fromUtf8("RMS噪声(pA)"));
        rmsNoiseEdit_ = new QLineEdit("0.5");
        rmsNoiseEdit_->setAlignment(Qt::AlignCenter);
        rmsNoiseEdit_->setMinimumHeight(35);
        gl->addWidget(lblRmsNoise, 5, 0);
        gl->addWidget(rmsNoiseEdit_, 5, 1);

        topRowLayout_->addWidget(panel);
    }
}

void ControlPage::createIndicators() {
    auto* panel = new QFrame();
    panel->setStyleSheet(squareStyle());
    panel->setMinimumSize(320, 160);
    panel->setMaximumSize(320, 160);

    auto* gl = new QGridLayout(panel);
    gl->setContentsMargins(12, 10, 12, 10);
    gl->setHorizontalSpacing(10);
    gl->setVerticalSpacing(12);

    auto addRow = [&](int row, const QString& text, QLabel*& lightRef){
        auto* lbl = new QLabel();
        lbl->setText(text);
        gl->addWidget(lbl, row, 0);
        lightRef = new QLabel();
        lightRef->setFixedSize(16, 16);
        lightRef->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        lightRef->setStyleSheet(lightStyle(QColor("#e74c3c")));
        lightRef->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        gl->addWidget(lightRef, row, 1, 1, 1, Qt::AlignRight | Qt::AlignVCenter);
    };

    addRow(0, QString::fromUtf8("密封打孔"),  indicatorSeal_);
    addRow(1, QString::fromUtf8("CBD打孔"),   indicatorMake_);
    addRow(2, QString::fromUtf8("扩孔"),      indicatorExpand_);
    addRow(3, QString::fromUtf8("结束打孔"),  indicatorFinish_);

    topAreaLayout_->addWidget(panel);
}

void ControlPage::createPlots() {
    // 使用网格布局统一底部三块图区的结构和间距
    auto* grid = new QGridLayout();
    grid->setContentsMargins(0, 0, 0, 0);
    grid->setHorizontalSpacing(12);
    grid->setVerticalSpacing(12);

    auto panelStyle = QString::fromUtf8("QFrame{background:#fff;border:1px solid #d0d7de;border-radius:6px;}");

    auto makeChart = [&](const QString& xLabel, const QString& yLabel) {
        auto* chart = new QChart();
        chart->legend()->hide();
        chart->setBackgroundVisible(true);
        chart->setBackgroundBrush(QBrush(QColor("#e5e5e5")));
        chart->setPlotAreaBackgroundBrush(QBrush(QColor("#000")));
        chart->setPlotAreaBackgroundVisible(true);

        auto* xAxis = new QValueAxis();
        xAxis->setTitleText(xLabel);
        xAxis->setLabelFormat("%.1f");
        xAxis->setMinorTickCount(0);
        xAxis->setGridLineColor(QColor("#444"));
        xAxis->setLinePen(QPen(QColor("#888")));
        chart->addAxis(xAxis, Qt::AlignBottom);

        auto* yAxis = new QValueAxis();
        yAxis->setTitleText(yLabel);
        yAxis->setLabelFormat("%.1f");
        yAxis->setMinorTickCount(0);
        yAxis->setGridLineColor(QColor("#444"));
        yAxis->setLinePen(QPen(QColor("#888")));
        chart->addAxis(yAxis, Qt::AlignLeft);

        // 预置一条空折线，后续可动态更新
        auto* series = new QLineSeries();
        series->setUseOpenGL(false);
        series->setColor(QColor("#00ffaa"));
        chart->addSeries(series);
        series->attachAxis(xAxis);
        series->attachAxis(yAxis);

        return chart;
    };

    auto makePlotPanel = [&](const QString& title, QChartView*& viewRef, int minH,
                             const QString& xLabel, const QString& yLabel) {
        auto* container = new QFrame();
        container->setStyleSheet(panelStyle);
        auto* v = new QVBoxLayout(container);
        v->setContentsMargins(12, 10, 12, 12);
        v->setSpacing(8);

        auto* t = new QLabel();
        t->setText(title);
        QFont tf(t->font()); tf.setPointSize(12); tf.setBold(true); t->setFont(tf);
        v->addWidget(t);

        auto* chart = makeChart(xLabel, yLabel);
        viewRef = new QChartView(chart);
        viewRef->setRenderHint(QPainter::Antialiasing);
        viewRef->setStyleSheet(chartViewStyle());
        viewRef->setMinimumHeight(minH);
        viewRef->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        v->addWidget(viewRef);

        return container;
    };

    // 左侧：CBD（包含4个子图：电压、电流、电阻、Sigma）
    auto* cbdPanel = new QFrame();
    cbdPanel->setStyleSheet(panelStyle);
    auto* cbdLayout = new QVBoxLayout(cbdPanel);
    cbdLayout->setContentsMargins(8, 5, 8, 8);
    cbdLayout->setSpacing(3);

    auto* cbdTitle = new QLabel(QString::fromUtf8("CBD"));
    QFont cbdTitleFont(cbdTitle->font()); 
    cbdTitleFont.setPointSize(10); 
    cbdTitleFont.setBold(true); 
    cbdTitle->setFont(cbdTitleFont);
    cbdTitle->setMaximumHeight(20);
    cbdLayout->addWidget(cbdTitle);

    // 创建4个子图的容器
    auto* cbdChartsWidget = new QWidget();
    auto* cbdChartsLayout = new QVBoxLayout(cbdChartsWidget);
    cbdChartsLayout->setContentsMargins(0, 0, 0, 0);
    cbdChartsLayout->setSpacing(2);

    // 创建4个子图
    auto createSubChart = [&](const QString& yLabel, const QColor& lineColor) {
        auto* chart = new QChart();
        chart->legend()->hide();
        chart->setBackgroundVisible(true);
        chart->setBackgroundBrush(QBrush(QColor("#e5e5e5")));
        chart->setPlotAreaBackgroundBrush(QBrush(QColor("#000")));
        chart->setPlotAreaBackgroundVisible(true);
        chart->setMargins(QMargins(5, 5, 5, 5));

        auto* xAxis = new QValueAxis();
        xAxis->setTitleText("时间 (s)");
        xAxis->setLabelFormat("%.1f");
        xAxis->setMinorTickCount(0);
        xAxis->setGridLineColor(QColor("#444"));
        xAxis->setLinePen(QPen(QColor("#888")));
        xAxis->setLabelsVisible(true);
        chart->addAxis(xAxis, Qt::AlignBottom);

        auto* yAxis = new QValueAxis();
        yAxis->setTitleText(yLabel);
        yAxis->setLabelFormat("%.2f");
        yAxis->setMinorTickCount(0);
        yAxis->setGridLineColor(QColor("#444"));
        yAxis->setLinePen(QPen(QColor("#888")));
        chart->addAxis(yAxis, Qt::AlignLeft);

        auto* series = new QLineSeries();
        series->setUseOpenGL(false);
        series->setColor(lineColor);
        series->setPen(QPen(lineColor, 1));
        chart->addSeries(series);
        series->attachAxis(xAxis);
        series->attachAxis(yAxis);

        auto* chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setStyleSheet(chartViewStyle());
        chartView->setMinimumHeight(150);
        chartView->setMaximumHeight(180);
        chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

        return chartView;
    };

    // 创建4个子图：电压、电流、电阻、Sigma
    plotCBDVoltage_ = createSubChart(QString::fromUtf8("电压(V)"), QColor("#ffffff"));
    plotCBDCurrent_ = createSubChart(QString::fromUtf8("电流(nA)"), QColor("#ffffff"));
    plotCBDResistance_ = createSubChart(QString::fromUtf8("电阻(MΩ)"), QColor("#ffffff"));
    plotCBDSigma_ = createSubChart(QString::fromUtf8("Sigma(I)"), QColor("#ffffff"));

    // 只有最后一个子图显示X轴标签
    auto* lastChart = plotCBDSigma_->chart();
    auto* lastXAxis = qobject_cast<QValueAxis*>(lastChart->axes(Qt::Horizontal).first());
    if (lastXAxis) {
        lastXAxis->setTitleText(QString::fromUtf8("时间(s)"));
        lastXAxis->setLabelsVisible(true);
    }

    cbdChartsLayout->addWidget(plotCBDVoltage_);
    cbdChartsLayout->addWidget(plotCBDCurrent_);
    cbdChartsLayout->addWidget(plotCBDResistance_);
    cbdChartsLayout->addWidget(plotCBDSigma_);

    cbdLayout->addWidget(cbdChartsWidget);

    // 右上：芯片检查
    auto* checkPanel = makePlotPanel(QString::fromUtf8("芯片检查"), plotCheck_, 280,
                                     QString::fromUtf8("时间(s)"), QString::fromUtf8("电流(A)"));
    // 右下：扩孔（包含2个子图：电压和孔径）
    auto* expandPanel = new QFrame();
    expandPanel->setStyleSheet(panelStyle);
    auto* expandLayout = new QVBoxLayout(expandPanel);
    expandLayout->setContentsMargins(8, 5, 8, 8);
    expandLayout->setSpacing(3);

    auto* expandTitle = new QLabel(QString::fromUtf8("扩孔"));
    QFont expandTitleFont(expandTitle->font()); 
    expandTitleFont.setPointSize(10); 
    expandTitleFont.setBold(true); 
    expandTitle->setFont(expandTitleFont);
    expandTitle->setMaximumHeight(20);
    expandLayout->addWidget(expandTitle);

    // 创建2个子图的容器
    auto* expandChartsWidget = new QWidget();
    auto* expandChartsLayout = new QVBoxLayout(expandChartsWidget);
    expandChartsLayout->setContentsMargins(0, 0, 0, 0);
    expandChartsLayout->setSpacing(2);

    // 创建扩孔电压子图
    auto* voltageChart = new QChart();
    voltageChart->legend()->hide();
    voltageChart->setBackgroundVisible(true);
    voltageChart->setBackgroundBrush(QBrush(QColor("#e5e5e5")));
    voltageChart->setPlotAreaBackgroundBrush(QBrush(QColor("#000")));
    voltageChart->setPlotAreaBackgroundVisible(true);
    voltageChart->setMargins(QMargins(5, 5, 5, 5));

    auto* voltageXAxis = new QValueAxis();
    voltageXAxis->setTitleText(QString::fromUtf8("时间(s)"));
    voltageXAxis->setLabelFormat("%.1f");
    voltageXAxis->setMinorTickCount(0);
    voltageXAxis->setGridLineColor(QColor("#444"));
    voltageXAxis->setLinePen(QPen(QColor("#888")));
    voltageXAxis->setLabelsVisible(true);
    voltageXAxis->setLabelsColor(QColor("#ffffff"));
    voltageXAxis->setTickCount(6);
    voltageChart->addAxis(voltageXAxis, Qt::AlignBottom);

    auto* voltageYAxis = new QValueAxis();
    voltageYAxis->setTitleText(QString::fromUtf8("电压(V)"));
    voltageYAxis->setLabelFormat("%.2f");
    voltageYAxis->setMinorTickCount(0);
    voltageYAxis->setGridLineColor(QColor("#444"));
    voltageYAxis->setLinePen(QPen(QColor("#888")));
    voltageYAxis->setLabelsColor(QColor("#ffffff"));
    voltageYAxis->setTickCount(6);
    voltageChart->addAxis(voltageYAxis, Qt::AlignLeft);

    auto* voltageSeries = new QLineSeries();
    voltageSeries->setUseOpenGL(false);
    voltageSeries->setColor(QColor("#ffffff"));
    voltageSeries->setPen(QPen(QColor("#ffffff"), 1));
    voltageChart->addSeries(voltageSeries);
    voltageSeries->attachAxis(voltageXAxis);
    voltageSeries->attachAxis(voltageYAxis);

    plotExpandVoltage_ = new QChartView(voltageChart);
    plotExpandVoltage_->setRenderHint(QPainter::Antialiasing);
    plotExpandVoltage_->setStyleSheet(chartViewStyle());
    plotExpandVoltage_->setMinimumHeight(190);
    plotExpandVoltage_->setMaximumHeight(230);
    plotExpandVoltage_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // 创建孔径子图
    auto* poreChart = new QChart();
    poreChart->legend()->hide();
    poreChart->setBackgroundVisible(true);
    poreChart->setBackgroundBrush(QBrush(QColor("#e5e5e5")));
    poreChart->setPlotAreaBackgroundBrush(QBrush(QColor("#000")));
    poreChart->setPlotAreaBackgroundVisible(true);
    poreChart->setMargins(QMargins(5, 5, 5, 5));

    auto* poreXAxis = new QValueAxis();
    poreXAxis->setTitleText(QString::fromUtf8("时间(s)"));
    poreXAxis->setLabelFormat("%.1f");
    poreXAxis->setMinorTickCount(0);
    poreXAxis->setGridLineColor(QColor("#444"));
    poreXAxis->setLinePen(QPen(QColor("#888")));
    poreXAxis->setLabelsVisible(true);
    poreXAxis->setLabelsColor(QColor("#ffffff"));
    poreXAxis->setTickCount(6);
    poreChart->addAxis(poreXAxis, Qt::AlignBottom);

    auto* poreYAxis = new QValueAxis();
    poreYAxis->setTitleText(QString::fromUtf8("孔径(nm)"));
    poreYAxis->setLabelFormat("%.2f");
    poreYAxis->setMinorTickCount(0);
    poreYAxis->setGridLineColor(QColor("#444"));
    poreYAxis->setLinePen(QPen(QColor("#888")));
    poreYAxis->setLabelsColor(QColor("#ffffff"));
    poreYAxis->setTickCount(6);
    poreChart->addAxis(poreYAxis, Qt::AlignLeft);

    auto* poreSeries = new QLineSeries();
    poreSeries->setUseOpenGL(false);
    poreSeries->setColor(QColor("#ffffff"));
    poreSeries->setPen(QPen(QColor("#ffffff"), 1));
    poreChart->addSeries(poreSeries);
    poreSeries->attachAxis(poreXAxis);
    poreSeries->attachAxis(poreYAxis);

    plotExpandPore_ = new QChartView(poreChart);
    plotExpandPore_->setRenderHint(QPainter::Antialiasing);
    plotExpandPore_->setStyleSheet(chartViewStyle());
    plotExpandPore_->setMinimumHeight(190);
    plotExpandPore_->setMaximumHeight(230);
    plotExpandPore_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    expandChartsLayout->addWidget(plotExpandVoltage_);
    expandChartsLayout->addWidget(plotExpandPore_);

    expandLayout->addWidget(expandChartsWidget);

    grid->addWidget(cbdPanel,   0, 0, 2, 1);
    grid->addWidget(checkPanel, 0, 1, 1, 1);
    grid->addWidget(expandPanel,1, 1, 1, 1);

    grid->setColumnStretch(0, 4);
    grid->setColumnStretch(1, 2);
    grid->setRowStretch(0, 1);
    grid->setRowStretch(1, 1);

    bottomRowLayout_->addLayout(grid);
}

void ControlPage::onModeChanged() {
    int mode = modeCombo_->currentIndex();
    QString modeText = (mode == 0) ? QString::fromUtf8("手动打孔模式") : QString::fromUtf8("自动打孔模式");
    deviceStatusEdit_->setText(modeText);
}

void ControlPage::onStartProcess() {
    btnStart_->setEnabled(false);
    btnStop_->setEnabled(true);
    
    int mode = modeCombo_->currentIndex();
    if (mode == 0) {
        deviceStatusEdit_->setText(QString::fromUtf8("手动打孔进行中..."));
    } else {
        deviceStatusEdit_->setText(QString::fromUtf8("自动打孔进行中..."));
    }
    
    // 开始数据采集
    isCollecting_ = true;
    dataPointCount_ = 0;
    timeData_.clear();
    currentData_.clear();
    dataTimer_->start(50);  // 每50ms更新一次数据
    
    // 更新指示灯状态
    indicatorSeal_->setStyleSheet(lightStyle(QColor("#4CAF50")));  // 绿色表示进行中
}

void ControlPage::onStopProcess() {
    btnStart_->setEnabled(true);
    btnStop_->setEnabled(false);
    deviceStatusEdit_->setText(QString::fromUtf8("进程已停止"));
    
    // 停止数据采集
    isCollecting_ = false;
    dataTimer_->stop();
    
    // 重置指示灯状态
    indicatorSeal_->setStyleSheet(lightStyle(QColor("#9E9E9E")));  // 灰色表示停止
    indicatorMake_->setStyleSheet(lightStyle(QColor("#9E9E9E")));
    indicatorExpand_->setStyleSheet(lightStyle(QColor("#9E9E9E")));
    indicatorFinish_->setStyleSheet(lightStyle(QColor("#9E9E9E")));
}

void ControlPage::updateData() {
    if (!isCollecting_) return;
    
    // 生成模拟数据
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::normal_distribution<double> dis(0.0, 0.1);
    
    double time = dataPointCount_ * 0.05;  // 50ms间隔
    double current = 1.0 + dis(gen);  // 基准电流1.0 + 噪声
    
    // 模拟事件：偶尔产生负向脉冲
    if (dataPointCount_ % 200 == 0 && dataPointCount_ > 0) {
        current -= 0.5;  // 事件脉冲
    }
    
    timeData_.push_back(time);
    currentData_.push_back(current);
    dataPointCount_++;
    
    // 限制数据点数量，保持最近1000个点
    if (timeData_.size() > 1000) {
        timeData_.erase(timeData_.begin());
        currentData_.erase(currentData_.begin());
    }
    
    // 更新图表
    updatePlotData();
}

void ControlPage::updatePlotData() {
    if (timeData_.empty() || currentData_.empty()) return;
    
    // 更新CBD图表
    if (plotCBD_) {
        auto* chart = plotCBD_->chart();
        chart->removeAllSeries();
        
        auto* series = new QLineSeries();
        series->setName(QString::fromUtf8("CBD"));
        series->setColor(QColor("#2196F3"));
        
        for (size_t i = 0; i < timeData_.size(); ++i) {
            series->append(timeData_[i], currentData_[i]);
        }
        
        chart->addSeries(series);
        chart->createDefaultAxes();
        chart->setTitle(QString::fromUtf8("实时电流数据"));
    }
    
    // 更新高级图表
    updateHistogram();
    updateScatterPlot();
    updateSpectrumPlot();
    updateStatisticsPlot();
    updateEventAnalysis();
}

void ControlPage::createAdvancedCharts() {
    // 创建图表控制面板
    auto* controlPanel = new QFrame();
    controlPanel->setStyleSheet(squareStyle());
    auto* controlLayout = new QHBoxLayout(controlPanel);
    controlLayout->setContentsMargins(8, 8, 8, 8);
    
    // 图表类型选择
    auto* lblChartType = new QLabel(QString::fromUtf8("图表类型:"));
    controlLayout->addWidget(lblChartType);
    
    chartTypeCombo_ = new QComboBox();
    chartTypeCombo_->addItem(QString::fromUtf8("直方图"));
    chartTypeCombo_->addItem(QString::fromUtf8("散点图"));
    chartTypeCombo_->addItem(QString::fromUtf8("频谱图"));
    chartTypeCombo_->addItem(QString::fromUtf8("统计图"));
    chartTypeCombo_->addItem(QString::fromUtf8("事件分析"));
    connect(chartTypeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ControlPage::onChartTypeChanged);
    controlLayout->addWidget(chartTypeCombo_);
    
    // 图表控制按钮
    btnToggleCharts_ = new QPushButton(QString::fromUtf8("显示/隐藏高级图表"));
    controlLayout->addWidget(btnToggleCharts_);
    
    btnExportChart_ = new QPushButton(QString::fromUtf8("导出图表数据"));
    connect(btnExportChart_, &QPushButton::clicked, this, &ControlPage::exportChartData);
    controlLayout->addWidget(btnExportChart_);
    
    btnResetZoom_ = new QPushButton(QString::fromUtf8("重置缩放"));
    connect(btnResetZoom_, &QPushButton::clicked, this, &ControlPage::resetChartZoom);
    controlLayout->addWidget(btnResetZoom_);
    
    // 添加缩放控制按钮
    btnZoomIn_ = new QPushButton(QString::fromUtf8("放大"));
    connect(btnZoomIn_, &QPushButton::clicked, this, &ControlPage::onZoomIn);
    controlLayout->addWidget(btnZoomIn_);
    
    btnZoomOut_ = new QPushButton(QString::fromUtf8("缩小"));
    connect(btnZoomOut_, &QPushButton::clicked, this, &ControlPage::onZoomOut);
    controlLayout->addWidget(btnZoomOut_);
    
    // 添加交互模式按钮
    btnPanMode_ = new QPushButton(QString::fromUtf8("拖拽模式"));
    btnPanMode_->setCheckable(true);
    connect(btnPanMode_, &QPushButton::clicked, this, &ControlPage::onPanModeToggle);
    controlLayout->addWidget(btnPanMode_);
    
    btnSelectMode_ = new QPushButton(QString::fromUtf8("选择模式"));
    btnSelectMode_->setCheckable(true);
    connect(btnSelectMode_, &QPushButton::clicked, this, &ControlPage::onSelectModeToggle);
    controlLayout->addWidget(btnSelectMode_);
    
    // 鼠标位置和选择数据标签
    mousePositionLabel_ = new QLabel(QString::fromUtf8("鼠标位置: -"));
    mousePositionLabel_->setMinimumWidth(150);
    controlLayout->addWidget(mousePositionLabel_);
    
    selectedDataLabel_ = new QLabel(QString::fromUtf8("选择数据: -"));
    selectedDataLabel_->setMinimumWidth(150);
    controlLayout->addWidget(selectedDataLabel_);
    
    // 统计信息标签
    chartStatsLabel_ = new QLabel(QString::fromUtf8("统计信息: 等待数据..."));
    controlLayout->addWidget(chartStatsLabel_);
    
    controlLayout->addStretch();
    
    // 将控制面板添加到主布局
    rootLayout_->insertWidget(rootLayout_->count() - 1, controlPanel);
    
    // 创建按钮容器，使用绿色按钮打开独立窗口
    auto* advancedButtonsFrame = new QFrame();
    advancedButtonsFrame->setStyleSheet(squareStyle());
    auto* buttonsLayout = new QGridLayout(advancedButtonsFrame);
    buttonsLayout->setContentsMargins(8, 8, 8, 8);
    buttonsLayout->setSpacing(8);

    const QString btnStyle = QString::fromUtf8(
        "QPushButton { background-color: #4CAF50; color: white; border: none; "
        "border-radius: 4px; font-size: 12px; font-weight: bold; padding: 10px 14px; }"
        "QPushButton:hover { background-color: #45a049; }"
        "QPushButton:pressed { background-color: #3d8b40; }"
        "QPushButton:disabled { background-color: #cccccc; color: #666666; }"
    );

    auto* btnHistogram = new QPushButton(QString::fromUtf8("电流分布直方图"));
    btnHistogram->setStyleSheet(btnStyle);
    connect(btnHistogram, &QPushButton::clicked, this, &ControlPage::openHistogramWindow);
    buttonsLayout->addWidget(btnHistogram, 0, 0);

    auto* btnScatter = new QPushButton(QString::fromUtf8("电流-时间散点图"));
    btnScatter->setStyleSheet(btnStyle);
    connect(btnScatter, &QPushButton::clicked, this, &ControlPage::openScatterWindow);
    buttonsLayout->addWidget(btnScatter, 0, 1);

    auto* btnSpectrum = new QPushButton(QString::fromUtf8("功率谱密度"));
    btnSpectrum->setStyleSheet(btnStyle);
    connect(btnSpectrum, &QPushButton::clicked, this, &ControlPage::openSpectrumWindow);
    buttonsLayout->addWidget(btnSpectrum, 1, 0);

    auto* btnStatistics = new QPushButton(QString::fromUtf8("统计分析"));
    btnStatistics->setStyleSheet(btnStyle);
    connect(btnStatistics, &QPushButton::clicked, this, &ControlPage::openStatisticsWindow);
    buttonsLayout->addWidget(btnStatistics, 1, 1);

    auto* btnEvent = new QPushButton(QString::fromUtf8("事件分析"));
    btnEvent->setStyleSheet(btnStyle);
    connect(btnEvent, &QPushButton::clicked, this, &ControlPage::openEventWindow);
    buttonsLayout->addWidget(btnEvent, 2, 0, 1, 2);

    // 添加按钮容器到主布局
    rootLayout_->addWidget(advancedButtonsFrame);
}

QChartView* ControlPage::createAdvancedChart(const QString& title, const QString& xLabel, const QString& yLabel) {
    auto* chart = new QChart();
    chart->setTitle(title);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setBackgroundVisible(true);
    chart->setBackgroundBrush(QBrush(QColor("#f8f9fa")));
    chart->setPlotAreaBackgroundBrush(QBrush(QColor("#ffffff")));
    chart->setPlotAreaBackgroundVisible(true);
    
    auto* xAxis = new QValueAxis();
    xAxis->setTitleText(xLabel);
    xAxis->setLabelFormat("%.2f");
    xAxis->setGridLineColor(QColor("#e0e0e0"));
    chart->addAxis(xAxis, Qt::AlignBottom);
    
    auto* yAxis = new QValueAxis();
    yAxis->setTitleText(yLabel);
    yAxis->setLabelFormat("%.2f");
    yAxis->setGridLineColor(QColor("#e0e0e0"));
    chart->addAxis(yAxis, Qt::AlignLeft);
    
    auto* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumHeight(300);
    chartView->setStyleSheet("QChartView { border: 1px solid #d0d7de; border-radius: 4px; }");
    
    // 设置图表交互功能
    setupChartInteractions(chartView);
    
    return chartView;
}

void ControlPage::updateHistogram() {
    if (!plotHistogram_ || currentData_.empty()) return;
    
    // 计算直方图数据
    histogramData_.clear();
    const int bins = 50;
    double minVal = *std::min_element(currentData_.begin(), currentData_.end());
    double maxVal = *std::max_element(currentData_.begin(), currentData_.end());
    double binWidth = (maxVal - minVal) / bins;
    
    std::vector<int> histogram(bins, 0);
    for (double value : currentData_) {
        int binIndex = static_cast<int>((value - minVal) / binWidth);
        if (binIndex >= bins) binIndex = bins - 1;
        if (binIndex < 0) binIndex = 0;
        histogram[binIndex]++;
    }
    
    // 更新图表
    auto* chart = plotHistogram_->chart();
    chart->removeAllSeries();
    
    auto* barSet = new QBarSet(QString::fromUtf8("电流分布"));
    for (int i = 0; i < bins; ++i) {
        *barSet << histogram[i];
    }
    
    auto* barSeries = new QBarSeries();
    barSeries->append(barSet);
    chart->addSeries(barSeries);
    chart->createDefaultAxes();
}

void ControlPage::updateScatterPlot() {
    if (!plotScatter_ || timeData_.empty() || currentData_.empty()) return;
    
    auto* chart = plotScatter_->chart();
    chart->removeAllSeries();
    
    auto* scatterSeries = new QScatterSeries();
    scatterSeries->setName(QString::fromUtf8("电流数据点"));
    scatterSeries->setMarkerSize(3.0);
    scatterSeries->setColor(QColor("#2196F3"));
    
    // 只显示最近的500个点以提高性能
    size_t startIndex = timeData_.size() > 500 ? timeData_.size() - 500 : 0;
    for (size_t i = startIndex; i < timeData_.size(); ++i) {
        scatterSeries->append(timeData_[i], currentData_[i]);
    }
    
    chart->addSeries(scatterSeries);
    chart->createDefaultAxes();
}

void ControlPage::updateSpectrumPlot() {
    if (!plotSpectrum_ || currentData_.empty()) return;
    
    // 简化的功率谱计算（不使用FFT库）
    frequencyData_.clear();
    amplitudeData_.clear();
    
    const int N = std::min(static_cast<int>(currentData_.size()), 512);
    const double sampleRate = 20.0; // 20 Hz采样率
    
    // 计算简化的功率谱
    for (int k = 0; k < N/2; ++k) {
        double frequency = k * sampleRate / N;
        double power = 0.0;
        
        // 简化的DFT计算
        for (int n = 0; n < N; ++n) {
            double angle = 2.0 * M_PI * k * n / N;
            power += currentData_[n] * cos(angle);
        }
        power = power * power / N;
        
        frequencyData_.push_back(frequency);
        amplitudeData_.push_back(power);
    }
    
    // 更新图表
    auto* chart = plotSpectrum_->chart();
    chart->removeAllSeries();
    
    auto* lineSeries = new QLineSeries();
    lineSeries->setName(QString::fromUtf8("功率谱"));
    lineSeries->setColor(QColor("#FF9800"));
    
    for (size_t i = 0; i < frequencyData_.size(); ++i) {
        lineSeries->append(frequencyData_[i], amplitudeData_[i]);
    }
    
    chart->addSeries(lineSeries);
    chart->createDefaultAxes();
}

void ControlPage::updateStatisticsPlot() {
    if (!plotStatistics_ || currentData_.empty()) return;
    
    // 计算统计数据
    calculateStatistics();
    
    auto* chart = plotStatistics_->chart();
    chart->removeAllSeries();
    
    // 计算滑动平均和标准差
    const int windowSize = 50;
    std::vector<double> movingAvg, movingStd;
    
    for (size_t i = windowSize; i < currentData_.size(); ++i) {
        double sum = 0.0, sumSq = 0.0;
        for (int j = 0; j < windowSize; ++j) {
            double val = currentData_[i - j];
            sum += val;
            sumSq += val * val;
        }
        double avg = sum / windowSize;
        double variance = (sumSq / windowSize) - (avg * avg);
        double std = sqrt(variance);
        
        movingAvg.push_back(avg);
        movingStd.push_back(std);
    }
    
    // 添加移动平均线
    auto* avgSeries = new QLineSeries();
    avgSeries->setName(QString::fromUtf8("移动平均"));
    avgSeries->setColor(QColor("#4CAF50"));
    
    for (size_t i = 0; i < movingAvg.size(); ++i) {
        avgSeries->append(timeData_[i + windowSize], movingAvg[i]);
    }
    
    // 添加标准差线
    auto* stdSeries = new QLineSeries();
    stdSeries->setName(QString::fromUtf8("标准差"));
    stdSeries->setColor(QColor("#F44336"));
    
    for (size_t i = 0; i < movingStd.size(); ++i) {
        stdSeries->append(timeData_[i + windowSize], movingStd[i]);
    }
    
    chart->addSeries(avgSeries);
    chart->addSeries(stdSeries);
    chart->createDefaultAxes();
}

void ControlPage::updateEventAnalysis() {
    if (!plotEventAnalysis_ || currentData_.empty()) return;
    
    // 检测事件（简单的阈值检测）
    eventDurations_.clear();
    eventAmplitudes_.clear();
    
    const double threshold = 0.5; // 事件检测阈值
    bool inEvent = false;
    int eventStart = 0;
    double eventMin = 0.0;
    
    for (size_t i = 0; i < currentData_.size(); ++i) {
        if (!inEvent && currentData_[i] < threshold) {
            // 事件开始
            inEvent = true;
            eventStart = i;
            eventMin = currentData_[i];
        } else if (inEvent && currentData_[i] >= threshold) {
            // 事件结束
            inEvent = false;
            double duration = (i - eventStart) * 0.05 * 1000; // 转换为毫秒
            double amplitude = abs(eventMin);
            
            if (duration > 1.0 && amplitude > 0.1) { // 过滤噪声
                eventDurations_.push_back(duration);
                eventAmplitudes_.push_back(amplitude);
            }
        } else if (inEvent && currentData_[i] < eventMin) {
            eventMin = currentData_[i];
        }
    }
    
    // 更新图表
    auto* chart = plotEventAnalysis_->chart();
    chart->removeAllSeries();
    
    auto* eventSeries = new QScatterSeries();
    eventSeries->setName(QString::fromUtf8("检测到的事件"));
    eventSeries->setMarkerSize(6.0);
    eventSeries->setColor(QColor("#9C27B0"));
    
    for (size_t i = 0; i < eventDurations_.size() && i < eventAmplitudes_.size(); ++i) {
        eventSeries->append(eventDurations_[i], eventAmplitudes_[i]);
    }
    
    chart->addSeries(eventSeries);
    chart->createDefaultAxes();
}

void ControlPage::calculateStatistics() {
    if (currentData_.empty()) return;
    
    double mean = std::accumulate(currentData_.begin(), currentData_.end(), 0.0) / currentData_.size();
    double variance = 0.0;
    for (double val : currentData_) {
        variance += (val - mean) * (val - mean);
    }
    variance /= currentData_.size();
    double stdDev = sqrt(variance);
    
    double minVal = *std::min_element(currentData_.begin(), currentData_.end());
    double maxVal = *std::max_element(currentData_.begin(), currentData_.end());
    
    QString statsText = QString::fromUtf8("统计信息: 均值=%.3f, 标准差=%.3f, 最小值=%.3f, 最大值=%.3f, 事件数=%1")
                        .arg(eventDurations_.size()).arg(mean).arg(stdDev).arg(minVal).arg(maxVal);
    
    if (chartStatsLabel_) {
        chartStatsLabel_->setText(statsText);
    }
}

void ControlPage::exportChartData() {
    QString fileName = QFileDialog::getSaveFileName(this, 
        QString::fromUtf8("导出图表数据"), 
        QString("nanopore_data_%1.csv").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")),
        QString::fromUtf8("CSV文件 (*.csv)"));
    
    if (fileName.isEmpty()) return;
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, QString::fromUtf8("错误"), QString::fromUtf8("无法创建文件"));
        return;
    }
    
    QTextStream out(&file);
    out << QString::fromUtf8("时间(s),电流(nA),事件持续时间(ms),事件幅度(nA)\n");
    
    size_t maxSize = std::max({timeData_.size(), currentData_.size(), eventDurations_.size()});
    for (size_t i = 0; i < maxSize; ++i) {
        QString line;
        if (i < timeData_.size()) line += QString::number(timeData_[i], 'f', 3);
        line += ",";
        if (i < currentData_.size()) line += QString::number(currentData_[i], 'f', 3);
        line += ",";
        if (i < eventDurations_.size()) line += QString::number(eventDurations_[i], 'f', 3);
        line += ",";
        if (i < eventAmplitudes_.size()) line += QString::number(eventAmplitudes_[i], 'f', 3);
        line += "\n";
        out << line;
    }
    
    QMessageBox::information(this, QString::fromUtf8("成功"), QString::fromUtf8("数据已导出到: ") + fileName);
}

void ControlPage::resetChartZoom() {
    if (plotHistogram_) plotHistogram_->chart()->zoomReset();
    if (plotScatter_) plotScatter_->chart()->zoomReset();
    if (plotSpectrum_) plotSpectrum_->chart()->zoomReset();
    if (plotStatistics_) plotStatistics_->chart()->zoomReset();
    if (plotEventAnalysis_) plotEventAnalysis_->chart()->zoomReset();
}

void ControlPage::onChartTypeChanged() {
    // 根据选择的图表类型更新显示
    int index = chartTypeCombo_->currentIndex();
    
    // 隐藏所有图表
    if (plotHistogram_) plotHistogram_->setVisible(false);
    if (plotScatter_) plotScatter_->setVisible(false);
    if (plotSpectrum_) plotSpectrum_->setVisible(false);
    if (plotStatistics_) plotStatistics_->setVisible(false);
    if (plotEventAnalysis_) plotEventAnalysis_->setVisible(false);
    
    // 显示选中的图表
    switch (index) {
        case 0: if (plotHistogram_) plotHistogram_->setVisible(true); break;
        case 1: if (plotScatter_) plotScatter_->setVisible(true); break;
        case 2: if (plotSpectrum_) plotSpectrum_->setVisible(true); break;
        case 3: if (plotStatistics_) plotStatistics_->setVisible(true); break;
        case 4: if (plotEventAnalysis_) plotEventAnalysis_->setVisible(true); break;
    }
}

void ControlPage::onZoomIn() {
    // 对当前可见的图表进行放大
    QList<QChartView*> charts = {plotHistogram_, plotScatter_, plotSpectrum_, plotStatistics_, plotEventAnalysis_};
    for (auto* chartView : charts) {
        if (chartView && chartView->isVisible()) {
            chartView->chart()->zoomIn();
        }
    }
}

void ControlPage::onZoomOut() {
    // 对当前可见的图表进行缩小
    QList<QChartView*> charts = {plotHistogram_, plotScatter_, plotSpectrum_, plotStatistics_, plotEventAnalysis_};
    for (auto* chartView : charts) {
        if (chartView && chartView->isVisible()) {
            chartView->chart()->zoomOut();
        }
    }
}

void ControlPage::onPanModeToggle() {
    bool panMode = btnPanMode_->isChecked();
    if (panMode) {
        btnSelectMode_->setChecked(false);
    }
    
    // 设置图表的拖拽模式
    QList<QChartView*> charts = {plotHistogram_, plotScatter_, plotSpectrum_, plotStatistics_, plotEventAnalysis_};
    for (auto* chartView : charts) {
        if (chartView) {
            if (panMode) {
                chartView->setDragMode(QGraphicsView::ScrollHandDrag);
            } else {
                chartView->setDragMode(QGraphicsView::NoDrag);
            }
        }
    }
}

void ControlPage::onSelectModeToggle() {
    bool selectMode = btnSelectMode_->isChecked();
    if (selectMode) {
        btnPanMode_->setChecked(false);
    }
    
    // 设置图表的选择模式
    QList<QChartView*> charts = {plotHistogram_, plotScatter_, plotSpectrum_, plotStatistics_, plotEventAnalysis_};
    for (auto* chartView : charts) {
        if (chartView) {
            if (selectMode) {
                chartView->setDragMode(QGraphicsView::RubberBandDrag);
            } else {
                chartView->setDragMode(QGraphicsView::NoDrag);
            }
        }
    }
}

void ControlPage::onChartMouseMove(const QPointF& position) {
    // 更新鼠标位置显示
    mousePositionLabel_->setText(QString::fromUtf8("鼠标位置: (%.2f, %.2f)").arg(position.x()).arg(position.y()));
}

void ControlPage::onChartMousePress(const QPointF& position) {
    // 更新选择数据显示
    selectedDataLabel_->setText(QString::fromUtf8("选择数据: (%.2f, %.2f)").arg(position.x()).arg(position.y()));
}

void ControlPage::setupChartInteractions(QChartView* chartView) {
    if (!chartView) return;
    
    // 启用鼠标跟踪
    chartView->setMouseTracking(true);
    
    // 设置默认的交互模式
    chartView->setRubberBand(QChartView::RectangleRubberBand);
    
    // 连接鼠标事件（这里使用简化的方式，实际应用中可能需要自定义事件过滤器）
    chartView->chart()->setAcceptHoverEvents(true);
}

// 打开独立窗口：电流分布直方图
void ControlPage::openHistogramWindow() {
    QWidget* win = new QWidget();
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->setWindowTitle(QString::fromUtf8("电流分布直方图"));
    auto* v = new QVBoxLayout(win);
    plotHistogram_ = createAdvancedChart(QString::fromUtf8("电流分布直方图"),
                                         QString::fromUtf8("电流(nA)"), QString::fromUtf8("频次"));
    v->addWidget(plotHistogram_);
    connect(win, &QObject::destroyed, this, [this](QObject*) { plotHistogram_ = nullptr; });
    updateHistogram();
    win->resize(900, 600);
    win->show();
}

// 打开独立窗口：电流-时间散点图
void ControlPage::openScatterWindow() {
    QWidget* win = new QWidget();
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->setWindowTitle(QString::fromUtf8("电流-时间散点图"));
    auto* v = new QVBoxLayout(win);
    plotScatter_ = createAdvancedChart(QString::fromUtf8("电流-时间散点图"),
                                       QString::fromUtf8("时间(s)"), QString::fromUtf8("电流(nA)"));
    v->addWidget(plotScatter_);
    connect(win, &QObject::destroyed, this, [this](QObject*) { plotScatter_ = nullptr; });
    updateScatterPlot();
    win->resize(900, 600);
    win->show();
}

// 打开独立窗口：功率谱密度
void ControlPage::openSpectrumWindow() {
    QWidget* win = new QWidget();
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->setWindowTitle(QString::fromUtf8("功率谱密度"));
    auto* v = new QVBoxLayout(win);
    plotSpectrum_ = createAdvancedChart(QString::fromUtf8("功率谱密度"),
                                        QString::fromUtf8("频率(Hz)"), QString::fromUtf8("功率"));
    v->addWidget(plotSpectrum_);
    connect(win, &QObject::destroyed, this, [this](QObject*) { plotSpectrum_ = nullptr; });
    updateSpectrumPlot();
    win->resize(900, 600);
    win->show();
}

// 打开独立窗口：统计分析
void ControlPage::openStatisticsWindow() {
    QWidget* win = new QWidget();
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->setWindowTitle(QString::fromUtf8("统计分析"));
    auto* v = new QVBoxLayout(win);
    plotStatistics_ = createAdvancedChart(QString::fromUtf8("统计分析"),
                                          QString::fromUtf8("时间(s)"), QString::fromUtf8("值"));
    v->addWidget(plotStatistics_);
    connect(win, &QObject::destroyed, this, [this](QObject*) { plotStatistics_ = nullptr; });
    updateStatisticsPlot();
    win->resize(900, 600);
    win->show();
}

// 打开独立窗口：事件分析
void ControlPage::openEventWindow() {
    QWidget* win = new QWidget();
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->setWindowTitle(QString::fromUtf8("事件分析"));
    auto* v = new QVBoxLayout(win);
    plotEventAnalysis_ = createAdvancedChart(QString::fromUtf8("事件分析"),
                                             QString::fromUtf8("事件持续时间(ms)"), QString::fromUtf8("事件幅度(nA)"));
    v->addWidget(plotEventAnalysis_);
    connect(win, &QObject::destroyed, this, [this](QObject*) { plotEventAnalysis_ = nullptr; });
    updateEventAnalysis();
    win->resize(900, 600);
    win->show();
}