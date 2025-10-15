#pragma once
#include <memory>
#include <vector>
#include <string>
#include <map>
#include <functional>
#include "DeviceInterface.h"
#include "Er4Device.h"

class DeviceManager
{
public:
    using DeviceConnectedCallback = std::function<void(const std::string&)>;
    using DeviceDisconnectedCallback = std::function<void(const std::string&)>;
    using DeviceErrorCallback = std::function<void(const std::string&, const std::string&)>;

    DeviceManager();
    ~DeviceManager();

    // 设备扫描和检测
    std::vector<std::string> scanDevices();
    std::vector<std::string> getAvailableDevices() const;
    
    // 设备连接管理
    bool connectDevice(const std::string &deviceId);
    bool disconnectDevice(const std::string &deviceId);
    bool disconnectAllDevices();
    bool isDeviceConnected(const std::string &deviceId) const;
    
    // 设备状态管理
    DeviceStatus getDeviceStatus(const std::string &deviceId) const;
    std::vector<std::string> getConnectedDevices() const;
    int getConnectedDeviceCount() const;
    
    // 设备操作
    DeviceInterface* getDevice(const std::string &deviceId) const;
    bool setDeviceVoltage(const std::string &deviceId, double voltage);
    bool setDeviceCurrentRange(const std::string &deviceId, double range);
    bool startDataAcquisition(const std::string &deviceId);
    bool stopDataAcquisition(const std::string &deviceId);
    
    // VISA资源管理（模拟）
    std::vector<std::string> getVisaResources();
    bool connectVisaDevice(const std::string &resource);
    bool sendScpiCommand(const std::string &deviceId, const std::string &command);
    std::string queryScpiCommand(const std::string &deviceId, const std::string &command);
    
    // 设备配置
    bool configureDevice(const std::string &deviceId, const std::map<std::string, std::string> &config);
    std::map<std::string, std::string> getDeviceConfiguration(const std::string &deviceId) const;
    
    // 回调函数设置
    void setDeviceConnectedCallback(DeviceConnectedCallback callback);
    void setDeviceDisconnectedCallback(DeviceDisconnectedCallback callback);
    void setDeviceErrorCallback(DeviceErrorCallback callback);
    
    // 状态检查
    void checkDeviceStatus();

private:
    struct DeviceEntry {
        std::unique_ptr<DeviceInterface> device;
        std::string deviceType;
        bool isConnected;
        std::map<std::string, std::string> configuration;
    };

    std::map<std::string, DeviceEntry> devices_;
    std::vector<std::string> availableDevices_;
    
    // VISA模拟相关
    std::vector<std::string> visaResources_;
    std::map<std::string, std::string> visaDeviceMap_;
    
    // 回调函数
    DeviceConnectedCallback deviceConnectedCallback_;
    DeviceDisconnectedCallback deviceDisconnectedCallback_;
    DeviceErrorCallback deviceErrorCallback_;
    
    void initializeDeviceTypes();
    std::string generateDeviceId(const std::string &deviceType, int index);
    bool validateDeviceId(const std::string &deviceId) const;
    void updateDeviceStatus(const std::string &deviceId);
    void handleDeviceError(const std::string &deviceId, const std::string &error);
};