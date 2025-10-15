#include "AnalysePage.h"
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFileDialog>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtGui/QDoubleValidator>
#include <QtGui/QIntValidator>

#include "utils/FileIO.h"
#include "algorithms/DynamicDetectionWrapper.h"
#include "algorithms/EventDetection.h"

AnalysePage::AnalysePage(QWidget* parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);

    auto* row1 = new QHBoxLayout();
    openBtn_ = new QPushButton(QStringLiteral("打开数据文件"), this);
    row1->addWidget(openBtn_);
    layout->addLayout(row1);

    auto* row2 = new QHBoxLayout();
    row2->addWidget(new QLabel(QStringLiteral("窗口(ms):"), this));
    windowEdit_ = new QLineEdit(this);
    windowEdit_->setValidator(new QIntValidator(1, 100000, this));
    windowEdit_->setText("100");
    row2->addWidget(windowEdit_);

    row2->addWidget(new QLabel(QStringLiteral("sigma:"), this));
    sigmaEdit_ = new QLineEdit(this);
    sigmaEdit_->setValidator(new QDoubleValidator(0.0, 10.0, 2, this));
    sigmaEdit_->setText("3.0");
    row2->addWidget(sigmaEdit_);

    row2->addWidget(new QLabel(QStringLiteral("采样率(Hz):"), this));
    samplingEdit_ = new QLineEdit(this);
    samplingEdit_->setValidator(new QIntValidator(1, 10000000, this));
    samplingEdit_->setText("10000");
    row2->addWidget(samplingEdit_);
    layout->addLayout(row2);

    auto* row3 = new QHBoxLayout();
    row3->addWidget(new QLabel(QStringLiteral("最小幅度:"), this));
    minAmpEdit_ = new QLineEdit(this);
    minAmpEdit_->setValidator(new QDoubleValidator(0.0, 1e9, 6, this));
    minAmpEdit_->setText("0.0");
    row3->addWidget(minAmpEdit_);

    row3->addWidget(new QLabel(QStringLiteral("最小时长(ms):"), this));
    minDurEdit_ = new QLineEdit(this);
    minDurEdit_->setValidator(new QDoubleValidator(0.0, 1e9, 3, this));
    minDurEdit_->setText("0.0");
    row3->addWidget(minDurEdit_);

    row3->addWidget(new QLabel(QStringLiteral("最大时长(ms):"), this));
    maxDurEdit_ = new QLineEdit(this);
    maxDurEdit_->setValidator(new QDoubleValidator(0.0, 1e9, 3, this));
    maxDurEdit_->setText("100000.0");
    row3->addWidget(maxDurEdit_);
    layout->addLayout(row3);

    auto* row4 = new QHBoxLayout();
    row4->addWidget(new QLabel(QStringLiteral("检测方向:"), this));
    directionCombo_ = new QComboBox(this);
    directionCombo_->addItem(QStringLiteral("双向"), 3);
    directionCombo_->addItem(QStringLiteral("正向"), 1);
    directionCombo_->addItem(QStringLiteral("负向"), 2);
    directionCombo_->setCurrentIndex(0);
    row4->addWidget(directionCombo_);
    detectBtn_ = new QPushButton(QStringLiteral("事件检测"), this);
    row4->addWidget(detectBtn_);
    layout->addLayout(row4);

    infoLabel_ = new QLabel(QStringLiteral("未加载数据"), this);
    layout->addWidget(infoLabel_);

    connect(openBtn_, &QPushButton::clicked, this, [this]{ openFile(); });
    connect(detectBtn_, &QPushButton::clicked, this, [this]{ runDetection(); });
    setLayout(layout);
}

void AnalysePage::openFile() {
    QString file = QFileDialog::getOpenFileName(this, QStringLiteral("选择数据文件"), QString(), QStringLiteral("文本文件 (*.txt);;所有文件 (*.*)"));
    if (file.isEmpty()) return;
    std::vector<double> x, y;
    if (!FileIO::readTxt(file.toStdString(), x, y)) {
        infoLabel_->setText(QStringLiteral("读取失败: ") + file);
        return;
    }
    dataX_ = std::move(x);
    dataY_ = std::move(y);
    infoLabel_->setText(QStringLiteral("加载数据点数: ") + QString::number((int)dataY_.size()));
}

void AnalysePage::runDetection() {
    if (dataY_.empty()) {
        infoLabel_->setText(QStringLiteral("未加载数据"));
        return;
    }
    int window_ms = windowEdit_->text().toInt();
    double sigma = sigmaEdit_->text().toDouble();
    int sampling = samplingEdit_->text().toInt();
    int window_samples = std::max(1, (int)(sampling * (window_ms / 1000.0)));

    auto baseline = DynamicDetectionWrapper::baseline(dataY_, window_samples);
    auto std_dev = DynamicDetectionWrapper::stddev(dataY_, window_samples);
    auto thresholds = DynamicDetectionWrapper::thresholds(dataY_, window_samples, sigma);
    auto down = thresholds.first;
    auto up = thresholds.second;

    // 使用与Python一致的事件检测逻辑（C++移植）
    EventDetection::Params p;
    p.signal_direction = directionCombo_->currentData().toInt();
    p.sampling = sampling;
    p.window_size_ms = window_ms;
    p.min_amplitude = minAmpEdit_->text().toDouble();
    p.min_duration_ms = minDurEdit_->text().toDouble();
    p.max_duration_ms = maxDurEdit_->text().toDouble();
    p.event_number = 10000;

    auto result = EventDetection::detect(dataX_, dataY_, baseline, up, down, p);
    infoLabel_->setText(QStringLiteral("检测事件个数: ") + QString::number((int)result.events.size()));
}