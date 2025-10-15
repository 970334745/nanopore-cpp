#pragma once
#include <string>
#include <vector>

struct DeviceInfo {
    std::string name;
    std::string id;
    std::string serialNumber;
    std::string firmwareVersion;
    bool isConnected;
};

struct DeviceStatus {
    bool isConnected;
    double temperature;
    double voltage;
    double current;
    std::string lastError;
};

struct VoltageRange {
    double min;
    double max;
    double step;
};

struct CurrentRange {
    double min;
    double max;
    double step;
};

class DeviceInterface {
public:
    virtual ~DeviceInterface() = default;
    
    // 设备发现和连接
    virtual std::vector<DeviceInfo> detect() = 0;
    virtual bool connect(const std::string& id) = 0;
    virtual void disconnect() = 0;
    virtual bool isConnected() const = 0;
    
    // 设备状态和信息
    virtual DeviceStatus getStatus() const = 0;
    virtual std::string getSerialNumber() const = 0;
    virtual std::string getFirmwareVersion() const = 0;
    
    // 电压和电流控制
    virtual bool setVoltage(double voltage) = 0;
    virtual double getVoltage() const = 0;
    virtual VoltageRange getVoltageRange() const = 0;
    virtual bool setCurrentRange(const CurrentRange& range) = 0;
    virtual CurrentRange getCurrentRange() const = 0;
    
    // 数据采集
    virtual bool startAcquisition() = 0;
    virtual bool stopAcquisition() = 0;
    virtual bool isAcquiring() const = 0;
    virtual std::vector<double> readData() = 0;
    
    // 设备校准和测试
    virtual bool performSelfTest() = 0;
    virtual bool calibrate() = 0;
    virtual std::string getLastError() const = 0;
};