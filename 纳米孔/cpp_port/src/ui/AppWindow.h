#pragma once

#include <QMainWindow>

class QStackedWidget;
class QAction;
class QStatusBar;
class QToolBar;
class QLabel;
class QProgressBar;
class QPushButton;
class QTimer;
class ControlPage;
class CollectionPage;
class AnalysePage;
class IdePage;
class ParameterConfigPage;

class AppWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit AppWindow(QWidget* parent = nullptr);

public slots:
    void updateConnectionStatus(const QString& status);
    void updateProgress(int value);
    void showStatusMessage(const QString& message, int timeout = 0);

private slots:
    void updateSystemStatus();
    void applyLightTheme();
    void applyDarkTheme();
    void applyBlueTheme();
    void applyHighContrastTheme();

private:
    void setupPages();
    void setupMenu();
    void setupStatusBar();
    void setupToolBar();

    QStackedWidget* stack_{};
    ControlPage* controlPage_{};
    CollectionPage* collectionPage_{};
    AnalysePage* analysePage_{};
    IdePage* idePage_{};
    ParameterConfigPage* parameterConfigPage_{};
    QAction* actionControl_{};
    QAction* actionCollection_{};
    QAction* actionAnalyse_{};
    QAction* actionIde_{};
    QAction* actionParameterConfig_{};
    
    // 主题菜单项
    QAction* actionLightTheme_{};
    QAction* actionDarkTheme_{};
    QAction* actionBlueTheme_{};
    QAction* actionHighContrastTheme_{};
    
    // 状态栏组件
    QStatusBar* statusBar_{};
    QLabel* connectionStatusLabel_{};
    QLabel* systemStatusLabel_{};
    QProgressBar* progressBar_{};
    QPushButton* statusDetailsButton_{};
    
    // 工具栏
    QToolBar* mainToolBar_{};
    QAction* actionSave_{};
    QAction* actionLoad_{};
    QAction* actionExport_{};
    QAction* actionSettings_{};
    
    // 状态更新定时器
    QTimer* statusTimer_{};
};