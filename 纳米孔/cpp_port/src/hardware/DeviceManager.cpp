#include "DeviceManager.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>

DeviceManager::DeviceManager()
{
    initializeDeviceTypes();
}

DeviceManager::~DeviceManager()
{
    disconnectAllDevices();
}

void DeviceManager::initializeDeviceTypes()
{
    // 初始化VISA资源列表（模拟）
    visaResources_ = {
        "USB0::0x05E6::0x2450::04123456::INSTR",  // Keithley 2450
        "USB0::0x05E6::0x2460::04123457::INSTR",  // Keithley 2460
        "TCPIP0::192.168.1.100::inst0::INSTR",    // 网络设备
        "ASRL1::INSTR",                           // 串口设备
        "ASRL2::INSTR"                            // 串口设备
    };
    
    // 初始化可用设备列表
    availableDevices_ = {
        "ER4_SIM_001",
        "ER4_SIM_002",
        "KEITHLEY_2450_001",
        "KEITHLEY_2460_001"
    };
}

std::vector<std::string> DeviceManager::scanDevices()
{
    std::cout << "扫描设备中..." << std::endl;
    
    // 模拟设备扫描过程
    availableDevices_.clear();
    
    // 扫描ER4设备
    for (int i = 1; i <= 2; ++i) {
        std::string deviceId = generateDeviceId("ER4_SIM", i);
        availableDevices_.push_back(deviceId);
    }
    
    // 扫描VISA设备
    for (const auto& resource : visaResources_) {
        if (resource.find("2450") != std::string::npos) {
            availableDevices_.push_back("KEITHLEY_2450_001");
        } else if (resource.find("2460") != std::string::npos) {
            availableDevices_.push_back("KEITHLEY_2460_001");
        }
    }
    
    std::cout << "发现 " << availableDevices_.size() << " 个设备" << std::endl;
    return availableDevices_;
}

std::vector<std::string> DeviceManager::getAvailableDevices() const
{
    return availableDevices_;
}

bool DeviceManager::connectDevice(const std::string &deviceId)
{
    if (!validateDeviceId(deviceId)) {
        handleDeviceError(deviceId, "无效的设备ID");
        return false;
    }
    
    if (isDeviceConnected(deviceId)) {
        std::cout << "设备 " << deviceId << " 已经连接" << std::endl;
        return true;
    }
    
    try {
        DeviceEntry entry;
        
        // 根据设备类型创建设备实例
        if (deviceId.find("ER4") != std::string::npos) {
            entry.device = std::make_unique<Er4Device>();
            entry.deviceType = "ER4";
        } else {
            // 其他设备类型可以在这里添加
            handleDeviceError(deviceId, "不支持的设备类型");
            return false;
        }
        
        // 尝试连接设备
        if (entry.device->connect(deviceId)) {
            entry.isConnected = true;
            devices_[deviceId] = std::move(entry);
            
            std::cout << "设备 " << deviceId << " 连接成功" << std::endl;
            
            // 调用连接回调
            if (deviceConnectedCallback_) {
                deviceConnectedCallback_(deviceId);
            }
            
            return true;
        } else {
            handleDeviceError(deviceId, "设备连接失败");
            return false;
        }
    } catch (const std::exception& e) {
        handleDeviceError(deviceId, std::string("连接异常: ") + e.what());
        return false;
    }
}

bool DeviceManager::disconnectDevice(const std::string &deviceId)
{
    auto it = devices_.find(deviceId);
    if (it == devices_.end()) {
        return false;
    }
    
    try {
        if (it->second.device) {
            it->second.device->disconnect();
        }
        
        devices_.erase(it);
        
        std::cout << "设备 " << deviceId << " 断开连接" << std::endl;
        
        // 调用断开连接回调
        if (deviceDisconnectedCallback_) {
            deviceDisconnectedCallback_(deviceId);
        }
        
        return true;
    } catch (const std::exception& e) {
        handleDeviceError(deviceId, std::string("断开连接异常: ") + e.what());
        return false;
    }
}

bool DeviceManager::disconnectAllDevices()
{
    bool allSuccess = true;
    
    for (auto& pair : devices_) {
        if (!disconnectDevice(pair.first)) {
            allSuccess = false;
        }
    }
    
    devices_.clear();
    return allSuccess;
}

bool DeviceManager::isDeviceConnected(const std::string &deviceId) const
{
    auto it = devices_.find(deviceId);
    return it != devices_.end() && it->second.isConnected;
}

DeviceStatus DeviceManager::getDeviceStatus(const std::string &deviceId) const
{
    auto it = devices_.find(deviceId);
    if (it != devices_.end() && it->second.device) {
        return it->second.device->getStatus();
    }
    DeviceStatus status;
    status.isConnected = false;
    status.temperature = 0.0;
    status.voltage = 0.0;
    status.current = 0.0;
    status.lastError = "Device not found";
    return status;
}

std::vector<std::string> DeviceManager::getConnectedDevices() const
{
    std::vector<std::string> connected;
    for (const auto& pair : devices_) {
        if (pair.second.isConnected) {
            connected.push_back(pair.first);
        }
    }
    return connected;
}

int DeviceManager::getConnectedDeviceCount() const
{
    return static_cast<int>(getConnectedDevices().size());
}

DeviceInterface* DeviceManager::getDevice(const std::string &deviceId) const
{
    auto it = devices_.find(deviceId);
    if (it != devices_.end()) {
        return it->second.device.get();
    }
    return nullptr;
}

bool DeviceManager::setDeviceVoltage(const std::string &deviceId, double voltage)
{
    auto device = getDevice(deviceId);
    if (device) {
        return device->setVoltage(voltage);
    }
    return false;
}

bool DeviceManager::setDeviceCurrentRange(const std::string &deviceId, double range)
{
    auto device = getDevice(deviceId);
    if (device) {
        CurrentRange currentRange;
        currentRange.min = 0.0;
        currentRange.max = range;
        currentRange.step = range / 1000.0;
        return device->setCurrentRange(currentRange);
    }
    return false;
}

bool DeviceManager::startDataAcquisition(const std::string &deviceId)
{
    auto device = getDevice(deviceId);
    if (device) {
        return device->startAcquisition();
    }
    return false;
}

bool DeviceManager::stopDataAcquisition(const std::string &deviceId)
{
    auto device = getDevice(deviceId);
    if (device) {
        return device->stopAcquisition();
    }
    return false;
}

std::vector<std::string> DeviceManager::getVisaResources()
{
    return visaResources_;
}

bool DeviceManager::connectVisaDevice(const std::string &resource)
{
    // 模拟VISA设备连接
    std::cout << "连接VISA资源: " << resource << std::endl;
    
    // 生成设备ID并映射到VISA资源
    std::string deviceId;
    if (resource.find("2450") != std::string::npos) {
        deviceId = "KEITHLEY_2450_001";
    } else if (resource.find("2460") != std::string::npos) {
        deviceId = "KEITHLEY_2460_001";
    } else {
        deviceId = "VISA_DEVICE_" + std::to_string(visaDeviceMap_.size() + 1);
    }
    
    visaDeviceMap_[deviceId] = resource;
    return true;
}

bool DeviceManager::sendScpiCommand(const std::string &deviceId, const std::string &command)
{
    auto it = visaDeviceMap_.find(deviceId);
    if (it != visaDeviceMap_.end()) {
        std::cout << "发送SCPI命令到 " << deviceId << ": " << command << std::endl;
        return true;
    }
    return false;
}

std::string DeviceManager::queryScpiCommand(const std::string &deviceId, const std::string &command)
{
    auto it = visaDeviceMap_.find(deviceId);
    if (it != visaDeviceMap_.end()) {
        std::cout << "查询SCPI命令到 " << deviceId << ": " << command << std::endl;
        
        // 模拟一些常见的SCPI响应
        if (command == "*IDN?") {
            if (deviceId.find("2450") != std::string::npos) {
                return "KEITHLEY INSTRUMENTS INC.,MODEL 2450,04123456,1.0.0";
            } else if (deviceId.find("2460") != std::string::npos) {
                return "KEITHLEY INSTRUMENTS INC.,MODEL 2460,04123457,1.0.0";
            }
        }
        
        return "OK";
    }
    return "";
}

bool DeviceManager::configureDevice(const std::string &deviceId, const std::map<std::string, std::string> &config)
{
    auto it = devices_.find(deviceId);
    if (it != devices_.end()) {
        it->second.configuration = config;
        std::cout << "配置设备 " << deviceId << " 完成" << std::endl;
        return true;
    }
    return false;
}

std::map<std::string, std::string> DeviceManager::getDeviceConfiguration(const std::string &deviceId) const
{
    auto it = devices_.find(deviceId);
    if (it != devices_.end()) {
        return it->second.configuration;
    }
    return {};
}

void DeviceManager::setDeviceConnectedCallback(DeviceConnectedCallback callback)
{
    deviceConnectedCallback_ = callback;
}

void DeviceManager::setDeviceDisconnectedCallback(DeviceDisconnectedCallback callback)
{
    deviceDisconnectedCallback_ = callback;
}

void DeviceManager::setDeviceErrorCallback(DeviceErrorCallback callback)
{
    deviceErrorCallback_ = callback;
}

void DeviceManager::checkDeviceStatus()
{
    for (auto& pair : devices_) {
        updateDeviceStatus(pair.first);
    }
}

std::string DeviceManager::generateDeviceId(const std::string &deviceType, int index)
{
    std::ostringstream oss;
    oss << deviceType << "_" << std::setfill('0') << std::setw(3) << index;
    return oss.str();
}

bool DeviceManager::validateDeviceId(const std::string &deviceId) const
{
    return std::find(availableDevices_.begin(), availableDevices_.end(), deviceId) != availableDevices_.end();
}

void DeviceManager::updateDeviceStatus(const std::string &deviceId)
{
    auto it = devices_.find(deviceId);
    if (it != devices_.end() && it->second.device) {
        DeviceStatus status = it->second.device->getStatus();
        it->second.isConnected = status.isConnected;
    }
}

void DeviceManager::handleDeviceError(const std::string &deviceId, const std::string &error)
{
    std::cerr << "设备错误 [" << deviceId << "]: " << error << std::endl;
    
    if (deviceErrorCallback_) {
        deviceErrorCallback_(deviceId, error);
    }
}