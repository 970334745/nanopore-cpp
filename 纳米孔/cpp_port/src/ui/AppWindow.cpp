#include "AppWindow.h"
#include "ControlPage.h"
#include "CollectionPage.h"
#include "AnalysePage.h"
#include "IdePage.h"
#include "ParameterConfigPage.h"
#include "StyleManager.h"
#include <QMenuBar>
#include <QMenu>
#include <QStackedWidget>
#include <QAction>
#include <QString>
#include <QStatusBar>
#include <QToolBar>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QTimer>
#include <QHBoxLayout>
#include <QWidget>

AppWindow::AppWindow(QWidget* parent) : QMainWindow(parent), stack_(new QStackedWidget(this)) {
    setupPages();
    setupMenu();
    setupStatusBar();
    setupToolBar();
    setCentralWidget(stack_);
    
    // 设置窗口尺寸和自适应
    setMinimumSize(1400, 900);  // 设置最小窗口尺寸，确保内容可见
    resize(1600, 1000);         // 设置默认窗口尺寸
    
    // 应用默认主题
    StyleManager::applyTheme(StyleManager::LightTheme);
    
    // 设置状态更新定时器
    statusTimer_ = new QTimer(this);
    connect(statusTimer_, &QTimer::timeout, this, &AppWindow::updateSystemStatus);
    statusTimer_->start(1000); // 每秒更新一次状态
    
    // 默认显示"CBD打孔"页面，便于与 Python 参考对齐预览
    stack_->setCurrentWidget(controlPage_);
    
    // 初始状态
    updateConnectionStatus("未连接");
    showStatusMessage("应用程序已启动", 3000);
}

void AppWindow::setupPages() {
    controlPage_ = new ControlPage(this);
    collectionPage_ = new CollectionPage(this);
    analysePage_ = new AnalysePage(this);
    idePage_ = new IdePage(this);
    parameterConfigPage_ = new ParameterConfigPage(this);
    stack_->addWidget(controlPage_);
    stack_->addWidget(collectionPage_);
    stack_->addWidget(analysePage_);
    stack_->addWidget(idePage_);
    stack_->addWidget(parameterConfigPage_);
}

void AppWindow::setupMenu() {
    QMenu* menu = menuBar()->addMenu(QStringLiteral("菜单"));
    actionControl_ = menu->addAction(QStringLiteral("CBD打孔"));
    actionCollection_ = menu->addAction(QStringLiteral("数据采集"));
    actionAnalyse_ = menu->addAction(QStringLiteral("数据分析"));
    actionIde_ = menu->addAction(QStringLiteral("代码编辑"));
    actionParameterConfig_ = menu->addAction(QStringLiteral("参数配置"));

    connect(actionControl_, &QAction::triggered, this, [this]{ stack_->setCurrentWidget(controlPage_); });
    connect(actionCollection_, &QAction::triggered, this, [this]{ stack_->setCurrentWidget(collectionPage_); });
    connect(actionAnalyse_, &QAction::triggered, this, [this]{ stack_->setCurrentWidget(analysePage_); });
    connect(actionIde_, &QAction::triggered, this, [this]{ stack_->setCurrentWidget(idePage_); });
    connect(actionParameterConfig_, &QAction::triggered, this, [this]{ stack_->setCurrentWidget(parameterConfigPage_); });
    
    // 主题菜单
    QMenu* themeMenu = menuBar()->addMenu(QStringLiteral("主题"));
    actionLightTheme_ = themeMenu->addAction(QStringLiteral("浅色主题"));
    actionDarkTheme_ = themeMenu->addAction(QStringLiteral("深色主题"));
    actionBlueTheme_ = themeMenu->addAction(QStringLiteral("蓝色主题"));
    actionHighContrastTheme_ = themeMenu->addAction(QStringLiteral("高对比度主题"));
    
    connect(actionLightTheme_, &QAction::triggered, this, &AppWindow::applyLightTheme);
    connect(actionDarkTheme_, &QAction::triggered, this, &AppWindow::applyDarkTheme);
    connect(actionBlueTheme_, &QAction::triggered, this, &AppWindow::applyBlueTheme);
    connect(actionHighContrastTheme_, &QAction::triggered, this, &AppWindow::applyHighContrastTheme);
}

void AppWindow::setupStatusBar() {
    statusBar_ = statusBar();
    
    // 连接状态标签
    connectionStatusLabel_ = new QLabel("连接状态: 未连接");
    connectionStatusLabel_->setStyleSheet("QLabel { color: red; font-weight: bold; }");
    statusBar_->addWidget(connectionStatusLabel_);
    
    // 添加分隔符
    statusBar_->addWidget(new QLabel(" | "));
    
    // 系统状态标签
    systemStatusLabel_ = new QLabel("系统状态: 就绪");
    systemStatusLabel_->setStyleSheet("QLabel { color: green; }");
    statusBar_->addWidget(systemStatusLabel_);
    
    // 进度条
    progressBar_ = new QProgressBar();
    progressBar_->setVisible(false);
    progressBar_->setMaximumWidth(200);
    statusBar_->addPermanentWidget(progressBar_);
    
    // 状态详情按钮
    statusDetailsButton_ = new QPushButton("详情");
    statusDetailsButton_->setMaximumWidth(60);
    statusBar_->addPermanentWidget(statusDetailsButton_);
}

void AppWindow::setupToolBar() {
    mainToolBar_ = addToolBar("主工具栏");
    
    // 保存操作
    actionSave_ = mainToolBar_->addAction("保存");
    actionSave_->setToolTip("保存当前配置");
    
    // 加载操作
    actionLoad_ = mainToolBar_->addAction("加载");
    actionLoad_->setToolTip("加载配置文件");
    
    mainToolBar_->addSeparator();
    
    // 导出操作
    actionExport_ = mainToolBar_->addAction("导出");
    actionExport_->setToolTip("导出数据");
    
    mainToolBar_->addSeparator();
    
    // 设置操作
    actionSettings_ = mainToolBar_->addAction("设置");
    actionSettings_->setToolTip("应用程序设置");
}

void AppWindow::updateConnectionStatus(const QString& status) {
    connectionStatusLabel_->setText("连接状态: " + status);
    if (status == "已连接") {
        connectionStatusLabel_->setStyleSheet("QLabel { color: green; font-weight: bold; }");
    } else {
        connectionStatusLabel_->setStyleSheet("QLabel { color: red; font-weight: bold; }");
    }
}

void AppWindow::updateProgress(int value) {
    if (value >= 0 && value <= 100) {
        progressBar_->setVisible(true);
        progressBar_->setValue(value);
    } else {
        progressBar_->setVisible(false);
    }
}

void AppWindow::showStatusMessage(const QString& message, int timeout) {
    statusBar_->showMessage(message, timeout);
}

void AppWindow::updateSystemStatus() {
    // 这里可以添加实际的系统状态检查逻辑
    static int counter = 0;
    counter++;
    
    if (counter % 10 == 0) { // 每10秒更新一次系统状态
        systemStatusLabel_->setText("系统状态: 运行中 (" + QString::number(counter) + "s)");
    }
}

void AppWindow::applyLightTheme() {
    StyleManager::applyTheme(StyleManager::LightTheme);
    showStatusMessage("已切换到浅色主题", 2000);
}

void AppWindow::applyDarkTheme() {
    StyleManager::applyTheme(StyleManager::DarkTheme);
    showStatusMessage("已切换到深色主题", 2000);
}

void AppWindow::applyBlueTheme() {
    StyleManager::applyTheme(StyleManager::BlueTheme);
    showStatusMessage("已切换到蓝色主题", 2000);
}

void AppWindow::applyHighContrastTheme() {
    StyleManager::applyTheme(StyleManager::HighContrastTheme);
    showStatusMessage("已切换到高对比度主题", 2000);
}