#include "DataExportDialog.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QProgressBar>
#include <QTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QTabWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QDateTimeEdit>
#include <QRadioButton>
#include <QButtonGroup>
#include <QAbstractButton>
#include <QString>
#include <QTableWidgetItem>
#include <QAbstractItemView>
#include <QThread>
#include <QTimer>
#include <algorithm>
#include <cmath>

DataExportDialog::DataExportDialog(QWidget *parent)
    : QDialog(parent)
    , isExporting_(false)
{
    setWindowTitle(QString::fromUtf8("数据导出"));
    setMinimumSize(800, 600);
    resize(1000, 700);
    
    setupUI();
    
    exportBtn_ = new QPushButton(QString::fromUtf8("开始导出"));
    cancelBtn_ = new QPushButton(QString::fromUtf8("取消"));
    closeBtn_ = new QPushButton(QString::fromUtf8("关闭"), this);
}

DataExportDialog::~DataExportDialog() {
    // 析构函数
}

void DataExportDialog::setExportData(const std::vector<double>& timeData, const std::vector<double>& currentData) {
    timeData_ = timeData;
    currentData_ = currentData;
    updatePreview();
}

void DataExportDialog::selectExportPath() {
    QString path = QFileDialog::getExistingDirectory(this, QString::fromUtf8("选择导出路径"));
    if (!path.isEmpty()) {
        exportPathEdit_->setText(path);
    }
}

void DataExportDialog::selectTemplateFile() {
    QString fileName = QFileDialog::getOpenFileName(this, QString::fromUtf8("选择模板文件"), "", "JSON Files (*.json)");
    if (!fileName.isEmpty()) {
        // 加载模板文件
    }
}

void DataExportDialog::setupUI() {
    auto* mainLayout = new QVBoxLayout(this);
    
    tabWidget_ = new QTabWidget();
    
    // 创建各个面板
    setupFilePanel();
    setupFormatPanel();
    setupDataRangePanel();
    setupOptionsPanel();
    setupPreviewPanel();
    setupTemplatePanel();
    setupBatchPanel();
    setupProgressPanel();
    
    mainLayout->addWidget(tabWidget_);
}

void DataExportDialog::setupFilePanel() {
    auto* fileWidget = new QWidget();
    auto* fileLayout = new QVBoxLayout(fileWidget);
    
    tabWidget_->addTab(fileWidget, QString::fromUtf8("文件设置"));
}

void DataExportDialog::setupFormatPanel() {
    auto* formatWidget = new QWidget();
    auto* formatLayout = new QVBoxLayout(formatWidget);
    
    tabWidget_->addTab(formatWidget, QString::fromUtf8("格式选项"));
}

void DataExportDialog::setupDataRangePanel() {
    auto* rangeWidget = new QWidget();
    auto* rangeLayout = new QVBoxLayout(rangeWidget);
    
    // 数据范围选择
    rangeGroup_ = new QGroupBox(QString::fromUtf8("数据范围"));
    auto* rangeGroupLayout = new QGridLayout(rangeGroup_);
    
    allDataRadio_ = new QRadioButton(QString::fromUtf8("全部数据"));
    timeRangeRadio_ = new QRadioButton(QString::fromUtf8("时间范围"));
    indexRangeRadio_ = new QRadioButton(QString::fromUtf8("索引范围"));
    
    rangeButtonGroup_ = new QButtonGroup(this);
    rangeButtonGroup_->addButton(allDataRadio_, 0);
    rangeButtonGroup_->addButton(timeRangeRadio_, 1);
    rangeButtonGroup_->addButton(indexRangeRadio_, 2);
    
    allDataRadio_->setChecked(true);
    
    rangeGroupLayout->addWidget(allDataRadio_, 0, 0);
    rangeGroupLayout->addWidget(timeRangeRadio_, 1, 0);
    rangeGroupLayout->addWidget(indexRangeRadio_, 2, 0);
    
    // 时间范围控件
    rangeGroupLayout->addWidget(new QLabel(QString::fromUtf8("开始时间:")), 1, 1);
    startTimeSpin_ = new QDoubleSpinBox();
    startTimeSpin_->setRange(0.0, 1000000.0);
    startTimeSpin_->setSuffix(" s");
    rangeGroupLayout->addWidget(startTimeSpin_, 1, 2);
    
    rangeGroupLayout->addWidget(new QLabel(QString::fromUtf8("结束时间:")), 1, 3);
    endTimeSpin_ = new QDoubleSpinBox();
    endTimeSpin_->setRange(0.0, 1000000.0);
    endTimeSpin_->setSuffix(" s");
    rangeGroupLayout->addWidget(endTimeSpin_, 1, 4);
    
    // 索引范围控件
    rangeGroupLayout->addWidget(new QLabel(QString::fromUtf8("开始索引:")), 2, 1);
    startIndexSpin_ = new QSpinBox();
    startIndexSpin_->setRange(0, 1000000);
    rangeGroupLayout->addWidget(startIndexSpin_, 2, 2);
    
    rangeGroupLayout->addWidget(new QLabel(QString::fromUtf8("结束索引:")), 2, 3);
    endIndexSpin_ = new QSpinBox();
    endIndexSpin_->setRange(0, 1000000);
    rangeGroupLayout->addWidget(endIndexSpin_, 2, 4);
    
    // 数据计数标签
    dataCountLabel_ = new QLabel(QString::fromUtf8("数据点数: 0"));
    rangeGroupLayout->addWidget(dataCountLabel_, 3, 0, 1, 5);
    
    rangeLayout->addWidget(rangeGroup_);
    
    // 连接信号
    connect(rangeButtonGroup_, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, [this](QAbstractButton*) { onDataRangeChanged(); });
    connect(startTimeSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &DataExportDialog::onTimeRangeChanged);
    connect(endTimeSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &DataExportDialog::onTimeRangeChanged);
    connect(startIndexSpin_, QOverload<int>::of(&QSpinBox::valueChanged), this, &DataExportDialog::onTimeRangeChanged);
    connect(endIndexSpin_, QOverload<int>::of(&QSpinBox::valueChanged), this, &DataExportDialog::onTimeRangeChanged);
    
    tabWidget_->addTab(rangeWidget, QString::fromUtf8("数据范围"));
}

void DataExportDialog::setupOptionsPanel() {
    auto* optionsWidget = new QWidget();
    auto* optionsLayout = new QVBoxLayout(optionsWidget);
    
    tabWidget_->addTab(optionsWidget, QString::fromUtf8("导出选项"));
}

void DataExportDialog::setupPreviewPanel() {
    auto* previewWidget = new QWidget();
    auto* previewLayout = new QVBoxLayout(previewWidget);
    
    tabWidget_->addTab(previewWidget, QString::fromUtf8("数据预览"));
}

void DataExportDialog::setupTemplatePanel() {
    auto* templateWidget = new QWidget();
    auto* templateLayout = new QVBoxLayout(templateWidget);
    
    tabWidget_->addTab(templateWidget, QString::fromUtf8("模板管理"));
}

void DataExportDialog::setupBatchPanel() {
    auto* batchWidget = new QWidget();
    auto* batchLayout = new QVBoxLayout(batchWidget);
    
    tabWidget_->addTab(batchWidget, QString::fromUtf8("批量导出"));
}

void DataExportDialog::setupProgressPanel() {
    auto* progressWidget = new QWidget();
    auto* progressLayout = new QVBoxLayout(progressWidget);
    
    tabWidget_->addTab(progressWidget, QString::fromUtf8("导出进度"));
}

// 槽函数实现
void DataExportDialog::onFormatChanged() {
    updateFileExtension();
}

void DataExportDialog::onCustomFormatToggled(bool enabled) {
    // 处理自定义格式切换
}

void DataExportDialog::onDataRangeChanged() {
    updatePreview();
}

void DataExportDialog::onTimeRangeChanged() {
    updatePreview();
}

void DataExportDialog::startExport() {
    if (isExporting_) return;
    
    isExporting_ = true;
    // 开始导出逻辑
    QThread::msleep(100); // 模拟导出过程
    isExporting_ = false;
}

void DataExportDialog::cancelExport() {
    isExporting_ = false;
}

void DataExportDialog::previewData() {
    updatePreview();
}

void DataExportDialog::saveTemplate() {
    // 保存模板逻辑
}

void DataExportDialog::loadTemplate() {
    // 加载模板逻辑
}

void DataExportDialog::deleteTemplate() {
    // 删除模板逻辑
}

void DataExportDialog::addBatchFile() {
    // 添加批量文件逻辑
}

void DataExportDialog::removeBatchFile() {
    // 移除批量文件逻辑
}

void DataExportDialog::startBatchExport() {
    // 开始批量导出逻辑
}

// 私有函数实现
void DataExportDialog::updatePreview() {
    if (dataCountLabel_) {
        dataCountLabel_->setText(QString::fromUtf8("数据点数: %1").arg(timeData_.size()));
    }
}

void DataExportDialog::updateFileExtension() {
    // 更新文件扩展名逻辑
}

void DataExportDialog::validateInputs() {
    // 验证输入逻辑
}