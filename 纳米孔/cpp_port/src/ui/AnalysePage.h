#pragma once
#include <QtCore/QObject>
#include <QtWidgets/QWidget>
#include <vector>

class QPushButton;
class QLabel;
class QLineEdit;

class QComboBox;

class AnalysePage : public QWidget {
    Q_OBJECT
public:
    explicit AnalysePage(QWidget* parent = nullptr);

private:
    QPushButton* openBtn_;
    QPushButton* detectBtn_;
    QLabel* infoLabel_;
    QLineEdit* sigmaEdit_;
    QLineEdit* windowEdit_;
    QLineEdit* samplingEdit_;
    QLineEdit* minAmpEdit_;
    QLineEdit* minDurEdit_;
    QLineEdit* maxDurEdit_;
    QComboBox* directionCombo_;

    std::vector<double> dataX_;
    std::vector<double> dataY_;

    void openFile();
    void runDetection();
};