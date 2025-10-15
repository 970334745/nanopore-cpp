#ifndef DATAEXPORTDIALOG_H
#define DATAEXPORTDIALOG_H

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
#include <vector>

class DataExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DataExportDialog(QWidget *parent = nullptr);
    ~DataExportDialog();
    
    void setExportData(const std::vector<double>& timeData, const std::vector<double>& currentData);

private slots:
    void selectExportPath();
    void selectTemplateFile();
    void onFormatChanged();
    void onCustomFormatToggled(bool enabled);
    void onDataRangeChanged();
    void onTimeRangeChanged();
    void startExport();
    void cancelExport();
    void previewData();
    void saveTemplate();
    void loadTemplate();
    void deleteTemplate();
    void addBatchFile();
    void removeBatchFile();
    void startBatchExport();

private:
    void setupUI();
    void setupFilePanel();
    void setupFormatPanel();
    void setupDataRangePanel();
    void setupOptionsPanel();
    void setupPreviewPanel();
    void setupTemplatePanel();
    void setupBatchPanel();
    void setupProgressPanel();
    void updatePreview();
    void updateFileExtension();
    void validateInputs();

    // UI组件
    QTabWidget* tabWidget_;
    
    // 文件设置
    QLineEdit* exportPathEdit_;
    QPushButton* browsePathBtn_;
    QLineEdit* fileNameEdit_;
    QComboBox* formatCombo_;
    QCheckBox* customFormatCheck_;
    QLineEdit* customFormatEdit_;
    
    // 数据范围
    QGroupBox* rangeGroup_;
    QButtonGroup* rangeButtonGroup_;
    QRadioButton* allDataRadio_;
    QRadioButton* timeRangeRadio_;
    QRadioButton* indexRangeRadio_;
    QDoubleSpinBox* startTimeSpin_;
    QDoubleSpinBox* endTimeSpin_;
    QSpinBox* startIndexSpin_;
    QSpinBox* endIndexSpin_;
    QLabel* dataCountLabel_;
    
    // 导出选项
    QCheckBox* includeHeaderCheck_;
    QCheckBox* includeTimestampCheck_;
    QCheckBox* includeUnitsCheck_;
    QComboBox* separatorCombo_;
    QComboBox* encodingCombo_;
    QSpinBox* precisionSpin_;
    QCheckBox* compressCheck_;
    
    // 预览
    QTableWidget* previewTable_;
    QPushButton* previewBtn_;
    QLabel* previewInfoLabel_;
    
    // 模板管理
    QComboBox* templateCombo_;
    QLineEdit* templateNameEdit_;
    QPushButton* saveTemplateBtn_;
    QPushButton* loadTemplateBtn_;
    QPushButton* deleteTemplateBtn_;
    
    // 批量导出
    QTableWidget* batchTable_;
    QPushButton* addBatchBtn_;
    QPushButton* removeBatchBtn_;
    QPushButton* batchExportBtn_;
    
    // 进度控制
    QProgressBar* progressBar_;
    QLabel* progressLabel_;
    QTextEdit* logText_;
    QPushButton* exportBtn_;
    QPushButton* cancelBtn_;
    QPushButton* closeBtn_;
    
    // 数据
    std::vector<double> timeData_;
    std::vector<double> currentData_;
    bool isExporting_;
};

#endif // DATAEXPORTDIALOG_H