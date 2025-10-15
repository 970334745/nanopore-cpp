#ifndef PARAMETERCONFIGPAGE_H
#define PARAMETERCONFIGPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QSlider>
#include <QTabWidget>
#include <QScrollArea>
#include <QFormLayout>
#include <QSplitter>

class ParameterConfigPage : public QWidget
{
    Q_OBJECT

public:
    explicit ParameterConfigPage(QWidget *parent = nullptr);

private slots:
    void onDeviceParameterChanged();
    void onAnalysisParameterChanged();
    void onDisplayParameterChanged();
    void onSaveParameters();
    void onLoadParameters();
    void onResetToDefaults();
    void onApplyParameters();

private:
    void setupUI();
    void setupDeviceParameters();
    void setupAnalysisParameters();
    void setupDisplayParameters();
    void setupControlButtons();
    void connectSignals();
    void loadDefaultParameters();
    void updateParameterDisplay();

    // Main layout components
    QVBoxLayout* mainLayout_;
    QTabWidget* tabWidget_;
    QHBoxLayout* buttonLayout_;

    // Device parameters tab
    QWidget* deviceTab_;
    QScrollArea* deviceScrollArea_;
    QGroupBox* connectionGroup_;
    QGroupBox* acquisitionGroup_;
    QGroupBox* calibrationGroup_;

    // Device parameter controls
    QComboBox* deviceTypeCombo_;
    QLineEdit* devicePortEdit_;
    QSpinBox* baudRateSpinBox_;
    QDoubleSpinBox* samplingRateSpinBox_;
    QSpinBox* bufferSizeSpinBox_;
    QCheckBox* autoConnectCheckBox_;
    QSlider* gainSlider_;
    QLabel* gainValueLabel_;
    QDoubleSpinBox* offsetSpinBox_;
    QCheckBox* enableFilterCheckBox_;
    QDoubleSpinBox* filterFreqSpinBox_;

    // Analysis parameters tab
    QWidget* analysisTab_;
    QScrollArea* analysisScrollArea_;
    QGroupBox* detectionGroup_;
    QGroupBox* classificationGroup_;
    QGroupBox* statisticsGroup_;

    // Analysis parameter controls
    QDoubleSpinBox* thresholdSpinBox_;
    QSpinBox* windowSizeSpinBox_;
    QDoubleSpinBox* baselineSpinBox_;
    QCheckBox* enableNoiseReductionCheckBox_;
    QComboBox* algorithmCombo_;
    QSpinBox* minEventLengthSpinBox_;
    QSpinBox* maxEventLengthSpinBox_;
    QCheckBox* enableRealTimeAnalysisCheckBox_;
    QDoubleSpinBox* confidenceThresholdSpinBox_;

    // Display parameters tab
    QWidget* displayTab_;
    QScrollArea* displayScrollArea_;
    QGroupBox* chartGroup_;
    QGroupBox* colorGroup_;
    QGroupBox* performanceGroup_;

    // Display parameter controls
    QSpinBox* refreshRateSpinBox_;
    QSpinBox* maxDataPointsSpinBox_;
    QCheckBox* enableAnimationCheckBox_;
    QComboBox* chartThemeCombo_;
    QSlider* transparencySlider_;
    QLabel* transparencyValueLabel_;
    QCheckBox* enableGridCheckBox_;
    QCheckBox* enableLegendCheckBox_;
    QComboBox* colorSchemeCombo_;
    QSpinBox* lineWidthSpinBox_;

    // Control buttons
    QPushButton* saveButton_;
    QPushButton* loadButton_;
    QPushButton* resetButton_;
    QPushButton* applyButton_;
    QPushButton* cancelButton_;

    // Parameter storage
    QVariantMap deviceParameters_;
    QVariantMap analysisParameters_;
    QVariantMap displayParameters_;
};

#endif // PARAMETERCONFIGPAGE_H