#include "KeithleyDevice.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <thread>
#include <chrono>

KeithleyDevice::KeithleyDevice() 
    : scpi_(std::make_unique<ScpiCommunication>())
    , connected_(false)
    , acquiring_(false)
    , statusCacheValid_(false)
    , sourceFunction_("VOLT")
    , measureFunction_("CURR")
    , autoRangeEnabled_(true)
    , complianceLimit_(0.001)
    , nplc_(1.0)
    , averagingCount_(1)
{
    // 设置SCPI回调
    scpi_->setErrorCallback([this](const std::string& error, int code) {
        handleScpiError(error, code);
    });
    
    scpi_->setResponseCallback([this](const std::string& command, const std::string& response) {
        handleScpiResponse(command, response);
    });
}

KeithleyDevice::~KeithleyDevice() {
    disconnect();
}

std::vector<DeviceInfo> KeithleyDevice::detect() {
    std::vector<DeviceInfo> devices;
    
    std::vector<std::string> resources = scpi_->findResources();
    
    for (const auto& resource : resources) {
        // 只处理Keithley设备
        if (resource.find("0x05E6") != std::string::npos || 
            resource.find("KEITHLEY") != std::string::npos) {
            
            DeviceInfo info;
            info.id = resource;
            info.name = "Keithley Device";
            info.isConnected = false;
            
            // 尝试获取设备信息
            ScpiCommunication tempScpi;
            if (tempScpi.openResource(resource)) {
                ScpiDeviceInfo scpiInfo = tempScpi.getDeviceInfo();
                info.name = scpiInfo.manufacturer + " " + scpiInfo.model;
                info.serialNumber = scpiInfo.serialNumber;
                info.firmwareVersion = scpiInfo.firmwareVersion;
                tempScpi.closeResource();
            }
            
            devices.push_back(info);
        }
    }
    
    std::cout << "检测到 " << devices.size() << " 个Keithley设备" << std::endl;
    return devices;
}

bool KeithleyDevice::connect(const std::string& id) {
    if (connected_) {
        disconnect();
    }
    
    deviceId_ = id;
    resourceString_ = id;  // 对于Keithley设备，ID就是资源字符串
    
    if (!scpi_->openResource(resourceString_)) {
        lastError_ = "无法打开VISA资源: " + resourceString_;
        return false;
    }
    
    // 获取设备信息
    ScpiDeviceInfo info = scpi_->getDeviceInfo();
    std::cout << "连接到设备: " << info.manufacturer << " " << info.model 
              << " (S/N: " << info.serialNumber << ")" << std::endl;
    
    // 初始化设备
    if (!scpi_->resetDevice()) {
        lastError_ = "设备重置失败";
        scpi_->closeResource();
        return false;
    }
    
    // 配置基本设置
    setSourceFunction(sourceFunction_);
    setMeasureFunction(measureFunction_);
    enableAutoRange(autoRangeEnabled_);
    scpi_->setCurrentLimit(complianceLimit_);
    
    connected_ = true;
    invalidateStatusCache();
    
    std::cout << "Keithley设备连接成功" << std::endl;
    return true;
}

void KeithleyDevice::disconnect() {
    if (connected_) {
        if (acquiring_) {
            stopAcquisition();
        }
        
        // 关闭输出
        scpi_->enableOutput(false);
        scpi_->closeResource();
        
        connected_ = false;
        acquiring_ = false;
        invalidateStatusCache();
        
        std::cout << "Keithley设备断开连接" << std::endl;
    }
}

bool KeithleyDevice::isConnected() const {
    return connected_ && scpi_->isResourceOpen();
}

DeviceStatus KeithleyDevice::getStatus() const {
    if (!connected_) {
        DeviceStatus status;
        status.isConnected = false;
        status.temperature = 0.0;
        status.voltage = 0.0;
        status.current = 0.0;
        status.lastError = lastError_;
        return status;
    }
    
    if (!statusCacheValid_) {
        updateStatusCache();
    }
    
    return cachedStatus_;
}

std::string KeithleyDevice::getSerialNumber() const {
    if (!connected_) return "";
    
    ScpiDeviceInfo info = scpi_->getDeviceInfo();
    return info.serialNumber;
}

std::string KeithleyDevice::getFirmwareVersion() const {
    if (!connected_) return "";
    
    ScpiDeviceInfo info = scpi_->getDeviceInfo();
    return info.firmwareVersion;
}

bool KeithleyDevice::setVoltage(double voltage) {
    if (!connected_) {
        lastError_ = "设备未连接";
        return false;
    }
    
    bool success = scpi_->setVoltage(voltage);
    if (success) {
        invalidateStatusCache();
    } else {
        lastError_ = "设置电压失败";
    }
    
    return success;
}

double KeithleyDevice::getVoltage() const {
    if (!connected_) return 0.0;
    
    return scpi_->getVoltage();
}

VoltageRange KeithleyDevice::getVoltageRange() const {
    if (!connected_) {
        return {0.0, 0.0, 0.0};
    }
    
    // 根据设备型号返回电压范围
    ScpiDeviceInfo info = scpi_->getDeviceInfo();
    return getModelVoltageRange(info.model);
}

bool KeithleyDevice::setCurrentRange(const CurrentRange& range) {
    if (!connected_) {
        lastError_ = "设备未连接";
        return false;
    }
    
    bool success = scpi_->setCurrentRange(range.max);
    if (success) {
        invalidateStatusCache();
    } else {
        lastError_ = "设置电流范围失败";
    }
    
    return success;
}

CurrentRange KeithleyDevice::getCurrentRange() const {
    if (!connected_) {
        return {0.0, 0.0, 0.0};
    }
    
    double range = scpi_->getCurrentRange();
    return {0.0, range, range / 1000.0};
}

bool KeithleyDevice::startAcquisition() {
    if (!connected_) {
        lastError_ = "设备未连接";
        return false;
    }
    
    if (acquiring_) {
        return true;  // 已经在采集
    }
    
    // 启用输出
    if (!scpi_->enableOutput(true)) {
        lastError_ = "启用输出失败";
        return false;
    }
    
    // 开始连续测量
    if (!scpi_->startContinuousMeasurement()) {
        lastError_ = "启动连续测量失败";
        scpi_->enableOutput(false);
        return false;
    }
    
    acquiring_ = true;
    invalidateStatusCache();
    
    std::cout << "开始数据采集" << std::endl;
    return true;
}

bool KeithleyDevice::stopAcquisition() {
    if (!connected_) {
        lastError_ = "设备未连接";
        return false;
    }
    
    if (!acquiring_) {
        return true;  // 已经停止
    }
    
    // 停止连续测量
    scpi_->stopContinuousMeasurement();
    
    // 关闭输出
    scpi_->enableOutput(false);
    
    acquiring_ = false;
    invalidateStatusCache();
    
    std::cout << "停止数据采集" << std::endl;
    return true;
}

bool KeithleyDevice::isAcquiring() const {
    return acquiring_;
}

std::vector<double> KeithleyDevice::readData() {
    if (!connected_ || !acquiring_) {
        return {};
    }
    
    return scpi_->readMeasurementData();
}

bool KeithleyDevice::performSelfTest() {
    if (!connected_) {
        lastError_ = "设备未连接";
        return false;
    }
    
    std::cout << "执行自检..." << std::endl;
    bool result = scpi_->performSelfTest();
    
    if (!result) {
        lastError_ = "自检失败";
    }
    
    return result;
}

bool KeithleyDevice::calibrate() {
    if (!connected_) {
        lastError_ = "设备未连接";
        return false;
    }
    
    std::cout << "执行校准..." << std::endl;
    bool result = scpi_->calibrate();
    
    if (!result) {
        lastError_ = "校准失败";
    }
    
    return result;
}

std::string KeithleyDevice::getLastError() const {
    return lastError_;
}

// Keithley特有功能实现

bool KeithleyDevice::setSourceFunction(const std::string& function) {
    if (!connected_) {
        lastError_ = "设备未连接";
        return false;
    }
    
    std::string command = "SOUR:FUNC " + function;
    if (sendCommand(command)) {
        sourceFunction_ = function;
        invalidateStatusCache();
        return true;
    }
    
    lastError_ = "设置源功能失败";
    return false;
}

std::string KeithleyDevice::getSourceFunction() const {
    if (!connected_) return "";
    
    std::string response = queryCommand("SOUR:FUNC?");
    return response.empty() ? sourceFunction_ : response;
}

bool KeithleyDevice::setMeasureFunction(const std::string& function) {
    if (!connected_) {
        lastError_ = "设备未连接";
        return false;
    }
    
    std::string command = "SENS:FUNC \"" + function + "\"";
    if (sendCommand(command)) {
        measureFunction_ = function;
        invalidateStatusCache();
        return true;
    }
    
    lastError_ = "设置测量功能失败";
    return false;
}

std::string KeithleyDevice::getMeasureFunction() const {
    if (!connected_) return "";
    
    std::string response = queryCommand("SENS:FUNC?");
    return response.empty() ? measureFunction_ : response;
}

bool KeithleyDevice::enableAutoRange(bool enable) {
    if (!connected_) {
        lastError_ = "设备未连接";
        return false;
    }
    
    std::string command = enable ? "SENS:CURR:RANG:AUTO ON" : "SENS:CURR:RANG:AUTO OFF";
    if (sendCommand(command)) {
        autoRangeEnabled_ = enable;
        return true;
    }
    
    lastError_ = "设置自动量程失败";
    return false;
}

bool KeithleyDevice::isAutoRangeEnabled() const {
    if (!connected_) return autoRangeEnabled_;
    
    std::string response = queryCommand("SENS:CURR:RANG:AUTO?");
    if (!response.empty()) {
        return (response == "1" || response == "ON");
    }
    
    return autoRangeEnabled_;
}

bool KeithleyDevice::setComplianceLimit(double limit) {
    if (!connected_) {
        lastError_ = "设备未连接";
        return false;
    }
    
    if (scpi_->setCurrentLimit(limit)) {
        complianceLimit_ = limit;
        return true;
    }
    
    lastError_ = "设置合规限制失败";
    return false;
}

double KeithleyDevice::getComplianceLimit() const {
    if (!connected_) return complianceLimit_;
    
    return scpi_->getCurrentLimit();
}

bool KeithleyDevice::setNPLC(double nplc) {
    if (!connected_) {
        lastError_ = "设备未连接";
        return false;
    }
    
    std::ostringstream oss;
    oss << "SENS:CURR:NPLC " << nplc;
    
    if (sendCommand(oss.str())) {
        nplc_ = nplc;
        return true;
    }
    
    lastError_ = "设置NPLC失败";
    return false;
}

double KeithleyDevice::getNPLC() const {
    if (!connected_) return nplc_;
    
    std::string response = queryCommand("SENS:CURR:NPLC?");
    if (!response.empty()) {
        try {
            return std::stod(response);
        } catch (const std::exception&) {
            return nplc_;
        }
    }
    
    return nplc_;
}

bool KeithleyDevice::setAveraging(int count) {
    if (!connected_) {
        lastError_ = "设备未连接";
        return false;
    }
    
    std::ostringstream oss;
    oss << "SENS:AVER:COUN " << count;
    
    if (sendCommand(oss.str()) && sendCommand("SENS:AVER ON")) {
        averagingCount_ = count;
        return true;
    }
    
    lastError_ = "设置平均次数失败";
    return false;
}

int KeithleyDevice::getAveraging() const {
    if (!connected_) return averagingCount_;
    
    std::string response = queryCommand("SENS:AVER:COUN?");
    if (!response.empty()) {
        try {
            return std::stoi(response);
        } catch (const std::exception&) {
            return averagingCount_;
        }
    }
    
    return averagingCount_;
}

// 私有辅助函数实现

void KeithleyDevice::updateStatusCache() const {
    cachedStatus_.isConnected = connected_;
    cachedStatus_.temperature = 25.0;  // 模拟温度
    cachedStatus_.voltage = scpi_->measureVoltage();
    cachedStatus_.current = scpi_->measureCurrent();
    cachedStatus_.lastError = lastError_;
    statusCacheValid_ = true;
}

void KeithleyDevice::invalidateStatusCache() {
    statusCacheValid_ = false;
}

bool KeithleyDevice::sendCommand(const std::string& command) {
    ScpiResult result = scpi_->sendCommand(command);
    if (!result.success) {
        lastError_ = result.errorMessage;
    }
    return result.success;
}

std::string KeithleyDevice::queryCommand(const std::string& command) {
    ScpiResult result = scpi_->queryCommand(command);
    if (!result.success) {
        lastError_ = result.errorMessage;
        return "";
    }
    return result.response;
}

std::string KeithleyDevice::queryCommand(const std::string& command) const {
    ScpiResult result = scpi_->queryCommand(command);
    return result.success ? result.response : "";
}

void KeithleyDevice::handleScpiError(const std::string& error, int code) {
    lastError_ = error;
    std::cerr << "SCPI错误 [" << code << "]: " << error << std::endl;
}

void KeithleyDevice::handleScpiResponse(const std::string& command, const std::string& response) {
    // 可以在这里添加响应处理逻辑
    // 例如更新缓存或触发事件
}

std::string KeithleyDevice::detectModel(const std::string& idn) {
    if (idn.find("2450") != std::string::npos) return "2450";
    if (idn.find("2460") != std::string::npos) return "2460";
    if (idn.find("6487") != std::string::npos) return "6487";
    if (idn.find("2400") != std::string::npos) return "2400";
    return "Unknown";
}

VoltageRange KeithleyDevice::getModelVoltageRange(const std::string& model) const {
    if (model == "2450" || model == "2460") {
        return {-200.0, 200.0, 0.001};
    } else if (model == "6487") {
        return {-500.0, 500.0, 0.001};
    } else if (model == "2400") {
        return {-210.0, 210.0, 0.001};
    }
    
    // 默认范围
    return {-10.0, 10.0, 0.001};
}

CurrentRange KeithleyDevice::getModelCurrentRange(const std::string& model) const {
    if (model == "2450") {
        return {-1.0, 1.0, 1e-9};
    } else if (model == "2460") {
        return {-7.35, 7.35, 1e-9};
    } else if (model == "6487") {
        return {-20e-3, 20e-3, 1e-15};
    } else if (model == "2400") {
        return {-1.05, 1.05, 1e-9};
    }
    
    // 默认范围
    return {-0.001, 0.001, 1e-9};
}