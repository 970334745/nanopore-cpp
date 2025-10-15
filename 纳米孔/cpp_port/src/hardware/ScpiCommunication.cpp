#include "ScpiCommunication.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <thread>
#include <chrono>
#include <regex>

// PIMPL实现结构
struct ScpiCommunication::Impl {
    std::string currentResource;
    bool resourceOpen;
    double timeoutMs;
    ErrorCallback errorCallback;
    ResponseCallback responseCallback;
    
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
             continuousMeasurement(false) {
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
    
    std::cout << "发现 " << pImpl_->availableResources.size() << " 个VISA资源" << std::endl;
    return pImpl_->availableResources;
}

bool ScpiCommunication::openResource(const std::string& resourceString) {
    if (pImpl_->resourceOpen) {
        closeResource();
    }
    
    // 检查资源是否在可用列表中
    auto it = std::find(pImpl_->availableResources.begin(), 
                       pImpl_->availableResources.end(), resourceString);
    
    if (it == pImpl_->availableResources.end()) {
        handleError("资源不存在: " + resourceString, -1);
        return false;
    }
    
    pImpl_->currentResource = resourceString;
    pImpl_->resourceOpen = true;
    
    std::cout << "成功打开VISA资源: " << resourceString << std::endl;
    
    // 发送初始化命令
    sendCommand("*RST");  // 重置设备
    sendCommand("*CLS");  // 清除错误
    
    return true;
}

void ScpiCommunication::closeResource() {
    if (pImpl_->resourceOpen) {
        std::cout << "关闭VISA资源: " << pImpl_->currentResource << std::endl;
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
    
    // 基本的SCPI命令格式验证
    std::regex scpiPattern(R"([A-Z*:?0-9\.\-\s\"]+)", std::regex_constants::icase);
    return std::regex_match(command, scpiPattern);
}

std::string ScpiCommunication::formatCommand(const std::string& command) {
    std::string formatted = command;
    
    // 转换为大写
    std::transform(formatted.begin(), formatted.end(), formatted.begin(), ::toupper);
    
    // 确保命令以换行符结束
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
    
    // 模拟命令执行
    std::cout << "发送SCPI命令: " << command << std::endl;
    
    // 模拟执行延迟
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    
    if (expectResponse) {
        // 生成模拟响应
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
            result.response = "0";  // 自检通过
        } else {
            result.response = "OK";
        }
        
        std::cout << "接收响应: " << result.response << std::endl;
    }
    
    result.success = true;
    
    // 调用响应回调
    if (pImpl_->responseCallback) {
        pImpl_->responseCallback(command, result.response);
    }
    
    logCommunication(command, result.response);
    
    return result;
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