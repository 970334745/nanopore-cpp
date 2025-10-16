#include "ScpiCommunication.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <thread>
#include <chrono>
#include <regex>
#include <cstdlib>

// Qt 模块化包含与条件编译（适配不同安装布局）
#if __has_include(<QtNetwork/QTcpSocket>)
#  include <QtNetwork/QTcpSocket>
#  define HAS_QT_NETWORK 1
#else
#  define HAS_QT_NETWORK 0
#endif

#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QRegularExpression>
#include <QtCore/QElapsedTimer>
#include <QtCore/qglobal.h>
#include <QtCore/Qt>

// PIMPL实现结构
struct ScpiCommunication::Impl {
    enum class Transport { Sim, TCP };
    std::string currentResource;
    bool resourceOpen;
    double timeoutMs;
    ErrorCallback errorCallback;
    ResponseCallback responseCallback;
    Transport transport;
#if HAS_QT_NETWORK
    std::unique_ptr<QTcpSocket> tcpSocket;
#endif
    std::string tcpHost;
    int tcpPort;
    
    // 模拟VISA资源列表
    std::vector<std::string> availableResources;
    
    // 设备状态
    bool outputEnabled;
    double currentVoltage;
    double currentLimit;
    double currentRange;
    bool continuousMeasurement;
    
    Impl() : resourceOpen(false), timeoutMs(5000.0), outputEnabled(false),
             currentVoltage(0.0), currentLimit(0.001), currentRange(0.001),
             continuousMeasurement(false), transport(Transport::Sim), tcpPort(5025) {
        // 初始化可用资源列表
        availableResources = {
            "USB0::0x05E6::0x2450::04123456::INSTR",  // Keithley 2450
            "USB0::0x05E6::0x2460::04123457::INSTR",  // Keithley 2460
            "USB0::0x05E6::0x6487::04123458::INSTR",  // Keithley 6487
            "TCPIP0::192.168.1.100::inst0::INSTR",    // 网络设备
            "ASRL1::INSTR",                           // 串口设备1
            "ASRL2::INSTR"                            // 串口设备2
        };
    }
};

ScpiCommunication::ScpiCommunication() : pImpl_(std::make_unique<Impl>()) {
}

ScpiCommunication::~ScpiCommunication() {
    closeResource();
}

std::vector<std::string> ScpiCommunication::findResources() {
    std::cout << "扫描VISA资源..." << std::endl;
    
    // 模拟资源扫描延迟
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // 从环境变量追加资源列表（以逗号或分号分隔）
    const char* env = std::getenv("SCPI_RESOURCES");
    if (env && *env) {
        QString raw = QString::fromUtf8(env);
        QStringList parts = raw.split(QRegularExpression("[;,]"), Qt::SkipEmptyParts);
        for (const QString& p : parts) {
            std::string s = p.trimmed().toStdString();
            if (!s.empty()) {
                pImpl_->availableResources.push_back(s);
            }
        }
    }

    std::cout << "发现 " << pImpl_->availableResources.size() << " 个VISA资源" << std::endl;
    return pImpl_->availableResources;
}

bool ScpiCommunication::openResource(const std::string& resourceString) {
    if (pImpl_->resourceOpen) {
        closeResource();
    }
    
    pImpl_->currentResource = resourceString;
    
    // TCPIP 资源：解析并建立 socket 连接
    if (resourceString.rfind("TCPIP", 0) == 0) {
        // 示例: TCPIP0::192.168.1.100::inst0::INSTR 或 TCPIP0::host::5025::SOCKET
        QString res = QString::fromStdString(resourceString);
        QStringList parts = res.split("::", Qt::SkipEmptyParts);
        if (parts.size() >= 3) {
            // parts[0] = TCPIP0, parts[1] = host, parts[2] = inst0 或 端口
            pImpl_->tcpHost = parts[1].toStdString();
            if (parts[2].startsWith("inst", Qt::CaseInsensitive)) {
                pImpl_->tcpPort = 5025; // 通常的 SCPI 端口
            } else {
                bool ok = false;
                int port = parts[2].toInt(&ok);
                pImpl_->tcpPort = ok ? port : 5025;
            }
        } else {
            handleError("TCPIP资源格式不正确: " + resourceString, -10);
            return false;
        }

        
#if HAS_QT_NETWORK
        pImpl_->tcpSocket = std::make_unique<QTcpSocket>();
        pImpl_->tcpSocket->connectToHost(QString::fromStdString(pImpl_->tcpHost), pImpl_->tcpPort);
        if (!pImpl_->tcpSocket->waitForConnected(static_cast<int>(pImpl_->timeoutMs))) {
            handleError("TCP连接失败: " + pImpl_->tcpHost + ":" + std::to_string(pImpl_->tcpPort), -11);
            pImpl_->tcpSocket.reset();
            return false;
        }

        pImpl_->transport = Impl::Transport::TCP;
        pImpl_->resourceOpen = true;
        std::cout << "已连接 TCPIP 设备: " << pImpl_->tcpHost << ":" << pImpl_->tcpPort << std::endl;
        
        // 初始化设备
        sendCommand("*RST");
        sendCommand("*CLS");
        return true;
#else
        handleError("当前构建缺少 QtNetwork 模块，无法连接 TCPIP 设备", -11);
        return false;
#endif
    }

    // 其他类型暂未实现，仍按模拟处理
    pImpl_->transport = Impl::Transport::Sim;
    pImpl_->resourceOpen = true;
    std::cout << "以模拟模式打开资源: " << resourceString << std::endl;
    sendCommand("*RST");
    sendCommand("*CLS");
    return true;
}

void ScpiCommunication::closeResource() {
    if (pImpl_->resourceOpen) {
        std::cout << "关闭资源: " << pImpl_->currentResource << std::endl;
        if (pImpl_->transport == Impl::Transport::TCP) {
#if HAS_QT_NETWORK
            if (pImpl_->tcpSocket) {
                pImpl_->tcpSocket->disconnectFromHost();
                pImpl_->tcpSocket->close();
                pImpl_->tcpSocket.reset();
            }
#endif
        }
        pImpl_->resourceOpen = false;
        pImpl_->currentResource.clear();
    }
}

bool ScpiCommunication::isResourceOpen() const {
    return pImpl_->resourceOpen;
}

std::string ScpiCommunication::getCurrentResource() const {
    return pImpl_->currentResource;
}

ScpiResult ScpiCommunication::sendCommand(const std::string& command) {
    return executeCommand(command, false);
}

ScpiResult ScpiCommunication::queryCommand(const std::string& command) {
    return executeCommand(command, true);
}

ScpiResult ScpiCommunication::queryCommand(const std::string& command, double timeoutMs) {
    double originalTimeout = pImpl_->timeoutMs;
    pImpl_->timeoutMs = timeoutMs;
    ScpiResult result = executeCommand(command, true);
    pImpl_->timeoutMs = originalTimeout;
    return result;
}

ScpiDeviceInfo ScpiCommunication::getDeviceInfo() {
    ScpiDeviceInfo info;
    info.resourceString = pImpl_->currentResource;
    
    ScpiResult idnResult = queryCommand("*IDN?");
    if (idnResult.success) {
        // 解析IDN响应: "MANUFACTURER,MODEL,SERIAL,FIRMWARE"
        std::istringstream iss(idnResult.response);
        std::string token;
        
        if (std::getline(iss, token, ',')) info.manufacturer = token;
        if (std::getline(iss, token, ',')) info.model = token;
        if (std::getline(iss, token, ',')) info.serialNumber = token;
        if (std::getline(iss, token, ',')) info.firmwareVersion = token;
    }
    
    return info;
}

std::string ScpiCommunication::getIdentification() {
    ScpiResult result = queryCommand("*IDN?");
    return result.success ? result.response : "";
}

bool ScpiCommunication::resetDevice() {
    ScpiResult result = sendCommand("*RST");
    if (result.success) {
        // 重置后恢复默认状态
        pImpl_->outputEnabled = false;
        pImpl_->currentVoltage = 0.0;
        pImpl_->currentLimit = 0.001;
        pImpl_->currentRange = 0.001;
    }
    return result.success;
}

bool ScpiCommunication::clearErrors() {
    ScpiResult result = sendCommand("*CLS");
    return result.success;
}

std::vector<std::string> ScpiCommunication::getErrorQueue() {
    std::vector<std::string> errors;
    
    // 读取错误队列直到没有错误
    for (int i = 0; i < 10; ++i) {  // 最多读取10个错误
        ScpiResult result = queryCommand("SYST:ERR?");
        if (result.success && result.response != "0,\"No error\"") {
            errors.push_back(result.response);
        } else {
            break;
        }
    }
    
    return errors;
}

bool ScpiCommunication::setVoltage(double voltage) {
    std::ostringstream oss;
    oss << "SOUR:VOLT " << voltage;
    ScpiResult result = sendCommand(oss.str());
    
    if (result.success) {
        pImpl_->currentVoltage = voltage;
    }
    
    return result.success;
}

double ScpiCommunication::getVoltage() {
    ScpiResult result = queryCommand("SOUR:VOLT?");
    if (result.success) {
        try {
            return std::stod(result.response);
        } catch (const std::exception&) {
            return pImpl_->currentVoltage;
        }
    }
    return pImpl_->currentVoltage;
}

bool ScpiCommunication::setCurrentLimit(double current) {
    std::ostringstream oss;
    oss << "SOUR:CURR:LIM " << current;
    ScpiResult result = sendCommand(oss.str());
    
    if (result.success) {
        pImpl_->currentLimit = current;
    }
    
    return result.success;
}

double ScpiCommunication::getCurrentLimit() {
    ScpiResult result = queryCommand("SOUR:CURR:LIM?");
    if (result.success) {
        try {
            return std::stod(result.response);
        } catch (const std::exception&) {
            return pImpl_->currentLimit;
        }
    }
    return pImpl_->currentLimit;
}

bool ScpiCommunication::setCurrentRange(double range) {
    std::ostringstream oss;
    oss << "SENS:CURR:RANG " << range;
    ScpiResult result = sendCommand(oss.str());
    
    if (result.success) {
        pImpl_->currentRange = range;
    }
    
    return result.success;
}

double ScpiCommunication::getCurrentRange() {
    ScpiResult result = queryCommand("SENS:CURR:RANG?");
    if (result.success) {
        try {
            return std::stod(result.response);
        } catch (const std::exception&) {
            return pImpl_->currentRange;
        }
    }
    return pImpl_->currentRange;
}

bool ScpiCommunication::enableOutput(bool enable) {
    std::string command = enable ? "OUTP ON" : "OUTP OFF";
    ScpiResult result = sendCommand(command);
    
    if (result.success) {
        pImpl_->outputEnabled = enable;
    }
    
    return result.success;
}

bool ScpiCommunication::isOutputEnabled() {
    ScpiResult result = queryCommand("OUTP?");
    if (result.success) {
        return (result.response == "1" || result.response == "ON");
    }
    return pImpl_->outputEnabled;
}

double ScpiCommunication::measureCurrent() {
    ScpiResult result = queryCommand("MEAS:CURR?");
    if (result.success) {
        try {
            return std::stod(result.response);
        } catch (const std::exception&) {
            // 返回模拟值
            return pImpl_->outputEnabled ? (pImpl_->currentVoltage * 1e-9) : 0.0;
        }
    }
    return pImpl_->outputEnabled ? (pImpl_->currentVoltage * 1e-9) : 0.0;
}

double ScpiCommunication::measureVoltage() {
    ScpiResult result = queryCommand("MEAS:VOLT?");
    if (result.success) {
        try {
            return std::stod(result.response);
        } catch (const std::exception&) {
            return pImpl_->currentVoltage;
        }
    }
    return pImpl_->currentVoltage;
}

bool ScpiCommunication::startContinuousMeasurement() {
    ScpiResult result = sendCommand("INIT:CONT ON");
    if (result.success) {
        pImpl_->continuousMeasurement = true;
    }
    return result.success;
}

bool ScpiCommunication::stopContinuousMeasurement() {
    ScpiResult result = sendCommand("INIT:CONT OFF");
    if (result.success) {
        pImpl_->continuousMeasurement = false;
    }
    return result.success;
}

std::vector<double> ScpiCommunication::readMeasurementData() {
    std::vector<double> data;
    
    if (!pImpl_->continuousMeasurement) {
        return data;
    }
    
    // 模拟读取多个数据点
    for (int i = 0; i < 10; ++i) {
        double current = measureCurrent();
        data.push_back(current);
    }
    
    return data;
}

bool ScpiCommunication::configureTrigger(const std::string& source, double level) {
    std::ostringstream oss;
    oss << "TRIG:SOUR " << source;
    ScpiResult result1 = sendCommand(oss.str());
    
    if (level != 0.0) {
        oss.str("");
        oss << "TRIG:LEV " << level;
        ScpiResult result2 = sendCommand(oss.str());
        return result1.success && result2.success;
    }
    
    return result1.success;
}

bool ScpiCommunication::setRemoteMode(bool remote) {
    std::string command = remote ? "SYST:REM" : "SYST:LOC";
    ScpiResult result = sendCommand(command);
    return result.success;
}

bool ScpiCommunication::setDisplayText(const std::string& text) {
    std::ostringstream oss;
    oss << "DISP:TEXT \"" << text << "\"";
    ScpiResult result = sendCommand(oss.str());
    return result.success;
}

bool ScpiCommunication::beep() {
    ScpiResult result = sendCommand("SYST:BEEP");
    return result.success;
}

bool ScpiCommunication::performSelfTest() {
    ScpiResult result = queryCommand("*TST?");
    if (result.success) {
        return (result.response == "0");  // 0表示测试通过
    }
    return false;
}

bool ScpiCommunication::calibrate() {
    ScpiResult result = sendCommand("CAL:ALL");
    return result.success;
}

std::string ScpiCommunication::getSystemStatus() {
    ScpiResult result = queryCommand("STAT:OPER?");
    return result.success ? result.response : "Unknown";
}

void ScpiCommunication::setErrorCallback(ErrorCallback callback) {
    pImpl_->errorCallback = callback;
}

void ScpiCommunication::setResponseCallback(ResponseCallback callback) {
    pImpl_->responseCallback = callback;
}

void ScpiCommunication::setTimeout(double timeoutMs) {
    pImpl_->timeoutMs = timeoutMs;
}

double ScpiCommunication::getTimeout() const {
    return pImpl_->timeoutMs;
}

bool ScpiCommunication::validateCommand(const std::string& command) {
    if (command.empty()) {
        return false;
    }
    
    // 宽松的命令验证：允许可打印 ASCII（兼容 SCPI 与 TSP/Lua 表达式）
    std::regex printableAscii(R"([^\x00-\x1F\x7F]+)");
    return std::regex_match(command, printableAscii);
}

std::string ScpiCommunication::formatCommand(const std::string& command) {
    std::string formatted = command;
    // 不改变大小写，避免破坏 TSP/Lua 变量名（如 smua）
    if (!formatted.empty() && formatted.back() != '\n') {
        formatted += '\n';
    }
    return formatted;
}

ScpiResult ScpiCommunication::executeCommand(const std::string& command, bool expectResponse) {
    ScpiResult result;
    result.success = false;
    result.errorCode = 0;
    
    if (!pImpl_->resourceOpen) {
        result.errorMessage = "VISA资源未打开";
        handleError(result.errorMessage, -1);
        return result;
    }
    
    if (!validateCommand(command)) {
        result.errorMessage = "无效的SCPI命令格式";
        handleError(result.errorMessage, -2);
        return result;
    }
    
    std::string formattedCommand = formatCommand(command);
    
    // 真实 TCP 传输路径
    if (pImpl_->transport == Impl::Transport::TCP) {
#if HAS_QT_NETWORK
        if (!pImpl_->tcpSocket) {
            result.errorMessage = "TCP 连接未建立";
            result.errorCode = -12;
            handleError(result.errorMessage, result.errorCode);
            return result;
        }
        std::cout << "发送SCPI/TSP: " << command << std::endl;
        QByteArray out = QByteArray::fromStdString(formattedCommand);
        qint64 written = pImpl_->tcpSocket->write(out);
        if (written != out.size() && !pImpl_->tcpSocket->waitForBytesWritten(static_cast<int>(pImpl_->timeoutMs))) {
            result.errorMessage = "写入失败或超时";
            result.errorCode = -12;
            handleError(result.errorMessage, result.errorCode);
            return result;
        }
        
        if (expectResponse) {
            QByteArray buf;
            QElapsedTimer timer; timer.start();
            while (timer.elapsed() < static_cast<qint64>(pImpl_->timeoutMs)) {
                if (pImpl_->tcpSocket->waitForReadyRead(100)) {
                    buf.append(pImpl_->tcpSocket->readAll());
                    if (buf.contains('\n')) break; // 简单以换行终止
                }
            }
            if (buf.isEmpty()) {
                result.errorMessage = "读取响应超时或为空";
                result.errorCode = -13;
                handleError(result.errorMessage, result.errorCode);
                return result;
            }
            // 去掉末尾换行/回车
            std::string resp = buf.toStdString();
            while (!resp.empty() && (resp.back() == '\n' || resp.back() == '\r')) resp.pop_back();
            result.response = resp;
            std::cout << "接收响应: " << result.response << std::endl;
        }
        result.success = true;
        if (pImpl_->responseCallback) {
            pImpl_->responseCallback(command, result.response);
        }
        logCommunication(command, result.response);
        return result;
#else
        result.errorMessage = "QtNetwork 不可用，无法进行 TCP 传输";
        result.errorCode = -15;
        handleError(result.errorMessage, result.errorCode);
        return result;
#endif
    }

    // 回退到模拟路径
    {
        // 模拟命令执行
        std::cout << "发送SCPI命令(模拟): " << command << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (expectResponse) {
            if (command.find("*IDN?") != std::string::npos) {
                if (pImpl_->currentResource.find("2450") != std::string::npos) {
                    result.response = "KEITHLEY INSTRUMENTS INC.,MODEL 2450,04123456,1.0.0";
                } else if (pImpl_->currentResource.find("2460") != std::string::npos) {
                    result.response = "KEITHLEY INSTRUMENTS INC.,MODEL 2460,04123457,1.0.0";
                } else if (pImpl_->currentResource.find("6487") != std::string::npos) {
                    result.response = "KEITHLEY INSTRUMENTS INC.,MODEL 6487,04123458,1.0.0";
                } else {
                    result.response = "UNKNOWN MANUFACTURER,UNKNOWN MODEL,00000000,1.0.0";
                }
            } else if (command.find("SOUR:VOLT?") != std::string::npos) {
                result.response = std::to_string(pImpl_->currentVoltage);
            } else if (command.find("MEAS:CURR?") != std::string::npos) {
                double current = pImpl_->outputEnabled ? (pImpl_->currentVoltage * 1e-9) : 0.0;
                result.response = std::to_string(current);
            } else if (command.find("OUTP?") != std::string::npos) {
                result.response = pImpl_->outputEnabled ? "1" : "0";
            } else if (command.find("SYST:ERR?") != std::string::npos) {
                result.response = "0,\"No error\"";
            } else if (command.find("*TST?") != std::string::npos) {
                result.response = "0";
            } else {
                result.response = "OK";
            }
            std::cout << "接收响应(模拟): " << result.response << std::endl;
        }
        result.success = true;
        if (pImpl_->responseCallback) {
            pImpl_->responseCallback(command, result.response);
        }
        logCommunication(command, result.response);
        return result;
    }
}

void ScpiCommunication::handleError(const std::string& message, int code) {
    std::cerr << "SCPI错误 [" << code << "]: " << message << std::endl;
    
    if (pImpl_->errorCallback) {
        pImpl_->errorCallback(message, code);
    }
}

void ScpiCommunication::logCommunication(const std::string& command, const std::string& response) {
    // 这里可以添加日志记录功能
    // 例如写入文件或发送到日志系统
}