#include "StyleManager.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>

QString StyleManager::currentTheme_ = "Light";

void StyleManager::applyTheme(Theme theme)
{
    QString styleSheet;
    
    switch (theme) {
        case LightTheme:
            styleSheet = generateLightThemeCSS();
            currentTheme_ = "Light";
            break;
        case DarkTheme:
            styleSheet = generateDarkThemeCSS();
            currentTheme_ = "Dark";
            break;
        case BlueTheme:
            styleSheet = generateBlueThemeCSS();
            currentTheme_ = "Blue";
            break;
        case HighContrastTheme:
            styleSheet = generateHighContrastThemeCSS();
            currentTheme_ = "HighContrast";
            break;
    }
    
    static_cast<QApplication*>(QApplication::instance())->setStyleSheet(styleSheet);
}

void StyleManager::loadStyleSheet(const QString& filePath)
{
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        QString styleSheet = stream.readAll();
        static_cast<QApplication*>(QApplication::instance())->setStyleSheet(styleSheet);
        file.close();
    } else {
        qWarning() << "无法加载样式文件:" << filePath;
    }
}

QString StyleManager::getCurrentTheme()
{
    return currentTheme_;
}

void StyleManager::setCustomColors(const QString& primaryColor, const QString& secondaryColor)
{
    QString customCSS = QString(R"(
        QPushButton {
            background-color: %1;
        }
        QPushButton:hover {
            background-color: %2;
        }
        QTabBar::tab:selected {
            background-color: %1;
        }
        QCheckBox::indicator:checked {
            background-color: %1;
        }
        QSlider::handle:horizontal {
            background-color: %1;
        }
        QProgressBar::chunk {
            background-color: %1;
        }
    )").arg(primaryColor, secondaryColor);
    
    auto app = static_cast<QApplication*>(QApplication::instance());
    app->setStyleSheet(app->styleSheet() + customCSS);
}

QString StyleManager::generateLightThemeCSS()
{
    return R"(
        /* 浅色主题 */
        QMainWindow {
            background-color: #f5f5f5;
            color: #333333;
        }
        
        QMenuBar {
            background-color: #2c3e50;
            color: white;
            border: none;
            padding: 4px;
        }
        
        QMenuBar::item {
            background-color: transparent;
            padding: 8px 16px;
            border-radius: 4px;
            margin: 2px;
        }
        
        QMenuBar::item:selected {
            background-color: #34495e;
        }
        
        QMenuBar::item:pressed {
            background-color: #1abc9c;
        }
        
        QMenu {
            background-color: white;
            border: 1px solid #bdc3c7;
            border-radius: 6px;
            padding: 4px;
        }
        
        QMenu::item {
            padding: 8px 20px;
            border-radius: 4px;
            margin: 1px;
        }
        
        QMenu::item:selected {
            background-color: #3498db;
            color: white;
        }
        
        QToolBar {
            background-color: #ecf0f1;
            border: none;
            spacing: 4px;
            padding: 4px;
        }
        
        QStatusBar {
            background-color: #34495e;
            color: white;
            border-top: 1px solid #2c3e50;
            padding: 4px;
        }
        
        QPushButton {
            background-color: #3498db;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 8px 16px;
            font-weight: bold;
            min-width: 80px;
        }
        
        QPushButton:hover {
            background-color: #2980b9;
        }
        
        QPushButton:pressed {
            background-color: #21618c;
        }
        
        /* 保留exportButton的自定义样式 */
        QPushButton#exportButton {
            background-color: #4CAF50 !important;
            color: white !important;
            border: 1px solid #45a049 !important;
            border-radius: 3px !important;
            font-size: 10px !important;
            font-weight: bold !important;
            padding: 2px !important;
            min-width: 50px !important;
        }
        
        QPushButton#exportButton:hover {
            background-color: #45a049 !important;
            border-color: #3d8b40 !important;
        }
        
        QPushButton#exportButton:pressed {
            background-color: #3d8b40 !important;
        }
        
        QGroupBox {
            font-weight: bold;
            border: 2px solid #bdc3c7;
            border-radius: 8px;
            margin-top: 12px;
            padding-top: 8px;
            background-color: white;
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 8px 0 8px;
            color: #2c3e50;
            background-color: white;
        }
        
        QTabWidget::pane {
            border: 1px solid #bdc3c7;
            border-radius: 6px;
            background-color: white;
            margin-top: -1px;
        }
        
        QTabBar::tab {
            background-color: #ecf0f1;
            border: 1px solid #bdc3c7;
            border-bottom: none;
            border-top-left-radius: 6px;
            border-top-right-radius: 6px;
            padding: 8px 16px;
            margin-right: 2px;
            min-width: 100px;
        }
        
        QTabBar::tab:selected {
            background-color: white;
            border-bottom: 1px solid white;
        }
        
        QLineEdit, QSpinBox, QDoubleSpinBox {
            border: 2px solid #bdc3c7;
            border-radius: 4px;
            padding: 6px;
            background-color: white;
            selection-background-color: #3498db;
        }
        
        QLineEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus {
            border-color: #3498db;
        }
        
        QComboBox {
            border: 2px solid #bdc3c7;
            border-radius: 4px;
            padding: 6px;
            background-color: white;
            min-width: 120px;
        }
        
        QComboBox:focus {
            border-color: #3498db;
        }
        
        QCheckBox::indicator {
            width: 18px;
            height: 18px;
            border: 2px solid #bdc3c7;
            border-radius: 3px;
            background-color: white;
        }
        
        QCheckBox::indicator:checked {
            background-color: #3498db;
            border-color: #3498db;
        }
        
        QSlider::groove:horizontal {
            border: 1px solid #bdc3c7;
            height: 6px;
            background-color: #ecf0f1;
            border-radius: 3px;
        }
        
        QSlider::handle:horizontal {
            background-color: #3498db;
            border: 2px solid #2980b9;
            width: 18px;
            margin: -7px 0;
            border-radius: 9px;
        }
        
        QProgressBar {
            border: 2px solid #bdc3c7;
            border-radius: 6px;
            text-align: center;
            background-color: #ecf0f1;
            color: #2c3e50;
            font-weight: bold;
        }
        
        QProgressBar::chunk {
            background-color: #3498db;
            border-radius: 4px;
        }
        
        QScrollArea {
            border: 1px solid #bdc3c7;
            border-radius: 6px;
            background-color: white;
        }
        
        QChartView {
            border: 2px solid #bdc3c7;
            border-radius: 8px;
            background-color: white;
        }
    )";
}

QString StyleManager::generateDarkThemeCSS()
{
    return R"(
        /* 深色主题 */
        QMainWindow {
            background-color: #2b2b2b;
            color: #ffffff;
        }
        
        QMenuBar {
            background-color: #1e1e1e;
            color: white;
            border: none;
            padding: 4px;
        }
        
        QMenuBar::item {
            background-color: transparent;
            padding: 8px 16px;
            border-radius: 4px;
            margin: 2px;
        }
        
        QMenuBar::item:selected {
            background-color: #404040;
        }
        
        QMenu {
            background-color: #3c3c3c;
            border: 1px solid #555555;
            border-radius: 6px;
            padding: 4px;
            color: white;
        }
        
        QMenu::item:selected {
            background-color: #0078d4;
            color: white;
        }
        
        QToolBar {
            background-color: #404040;
            border: none;
            spacing: 4px;
            padding: 4px;
        }
        
        QStatusBar {
            background-color: #1e1e1e;
            color: white;
            border-top: 1px solid #404040;
            padding: 4px;
        }
        
        QPushButton {
            background-color: #0078d4;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 8px 16px;
            font-weight: bold;
            min-width: 80px;
        }
        
        QPushButton:hover {
            background-color: #106ebe;
        }
        
        QGroupBox {
            font-weight: bold;
            border: 2px solid #555555;
            border-radius: 8px;
            margin-top: 12px;
            padding-top: 8px;
            background-color: #3c3c3c;
            color: white;
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 8px 0 8px;
            color: white;
            background-color: #3c3c3c;
        }
        
        QTabWidget::pane {
            border: 1px solid #555555;
            border-radius: 6px;
            background-color: #3c3c3c;
            margin-top: -1px;
        }
        
        QTabBar::tab {
            background-color: #2b2b2b;
            border: 1px solid #555555;
            border-bottom: none;
            border-top-left-radius: 6px;
            border-top-right-radius: 6px;
            padding: 8px 16px;
            margin-right: 2px;
            min-width: 100px;
            color: white;
        }
        
        QTabBar::tab:selected {
            background-color: #3c3c3c;
            border-bottom: 1px solid #3c3c3c;
        }
        
        QLineEdit, QSpinBox, QDoubleSpinBox {
            border: 2px solid #555555;
            border-radius: 4px;
            padding: 6px;
            background-color: #3c3c3c;
            color: white;
            selection-background-color: #0078d4;
        }
        
        QLineEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus {
            border-color: #0078d4;
        }
        
        QComboBox {
            border: 2px solid #555555;
            border-radius: 4px;
            padding: 6px;
            background-color: #3c3c3c;
            color: white;
            min-width: 120px;
        }
        
        QComboBox:focus {
            border-color: #0078d4;
        }
        
        QCheckBox {
            color: white;
        }
        
        QCheckBox::indicator {
            width: 18px;
            height: 18px;
            border: 2px solid #555555;
            border-radius: 3px;
            background-color: #3c3c3c;
        }
        
        QCheckBox::indicator:checked {
            background-color: #0078d4;
            border-color: #0078d4;
        }
        
        QSlider::groove:horizontal {
            border: 1px solid #555555;
            height: 6px;
            background-color: #2b2b2b;
            border-radius: 3px;
        }
        
        QSlider::handle:horizontal {
            background-color: #0078d4;
            border: 2px solid #106ebe;
            width: 18px;
            margin: -7px 0;
            border-radius: 9px;
        }
        
        QProgressBar {
            border: 2px solid #555555;
            border-radius: 6px;
            text-align: center;
            background-color: #2b2b2b;
            color: white;
            font-weight: bold;
        }
        
        QProgressBar::chunk {
            background-color: #0078d4;
            border-radius: 4px;
        }
        
        QScrollArea {
            border: 1px solid #555555;
            border-radius: 6px;
            background-color: #3c3c3c;
        }
        
        QChartView {
            border: 2px solid #555555;
            border-radius: 8px;
            background-color: #3c3c3c;
        }
        
        QLabel {
            color: white;
        }
    )";
}

QString StyleManager::generateBlueThemeCSS()
{
    return R"(
        /* 蓝色主题 */
        QMainWindow {
            background-color: #e3f2fd;
            color: #0d47a1;
        }
        
        QMenuBar {
            background-color: #1976d2;
            color: white;
            border: none;
            padding: 4px;
        }
        
        QMenuBar::item:selected {
            background-color: #1565c0;
        }
        
        QPushButton {
            background-color: #2196f3;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 8px 16px;
            font-weight: bold;
            min-width: 80px;
        }
        
        QPushButton:hover {
            background-color: #1976d2;
        }
        
        QGroupBox {
            font-weight: bold;
            border: 2px solid #90caf9;
            border-radius: 8px;
            margin-top: 12px;
            padding-top: 8px;
            background-color: white;
            color: #0d47a1;
        }
        
        QTabBar::tab:selected {
            background-color: #bbdefb;
        }
        
        QLineEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus {
            border-color: #2196f3;
        }
        
        QCheckBox::indicator:checked {
            background-color: #2196f3;
            border-color: #2196f3;
        }
        
        QSlider::handle:horizontal {
            background-color: #2196f3;
            border: 2px solid #1976d2;
        }
        
        QProgressBar::chunk {
            background-color: #2196f3;
        }
    )";
}

QString StyleManager::generateHighContrastThemeCSS()
{
    return R"(
        /* 高对比度主题 */
        QMainWindow {
            background-color: #000000;
            color: #ffffff;
        }
        
        QMenuBar {
            background-color: #000000;
            color: #ffffff;
            border: 2px solid #ffffff;
        }
        
        QMenuBar::item:selected {
            background-color: #ffffff;
            color: #000000;
        }
        
        QPushButton {
            background-color: #ffffff;
            color: #000000;
            border: 2px solid #ffffff;
            border-radius: 0px;
            padding: 8px 16px;
            font-weight: bold;
            min-width: 80px;
        }
        
        QPushButton:hover {
            background-color: #ffff00;
            color: #000000;
        }
        
        QGroupBox {
            font-weight: bold;
            border: 2px solid #ffffff;
            border-radius: 0px;
            margin-top: 12px;
            padding-top: 8px;
            background-color: #000000;
            color: #ffffff;
        }
        
        QLineEdit, QSpinBox, QDoubleSpinBox {
            border: 2px solid #ffffff;
            border-radius: 0px;
            padding: 6px;
            background-color: #000000;
            color: #ffffff;
        }
        
        QLineEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus {
            border-color: #ffff00;
        }
        
        QCheckBox {
            color: #ffffff;
        }
        
        QCheckBox::indicator {
            border: 2px solid #ffffff;
            border-radius: 0px;
            background-color: #000000;
        }
        
        QCheckBox::indicator:checked {
            background-color: #ffffff;
        }
        
        QLabel {
            color: #ffffff;
        }
    )";
}