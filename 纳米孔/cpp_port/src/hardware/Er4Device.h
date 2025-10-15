#pragma once
#include "DeviceInterface.h"

class Er4Device : public DeviceInterface {
public:
    Er4Device();
    
    // 设备发现和连接
    std::vector<DeviceInfo> detect() override;
    bool connect(const std::string& id) override;
    void disconnect() override;
    bool isConnected() const override;
    
    // 设备状态和信息
    DeviceStatus getStatus() const override;
    std::string getSerialNumber() const override;
    std::string getFirmwareVersion() const override;
    
    // 电压和电流控制
    bool setVoltage(double voltage) override;
    double getVoltage() const override;
    VoltageRange getVoltageRange() const override;
    bool setCurrentRange(const CurrentRange& range) override;
    CurrentRange getCurrentRange() const override;
    
    // 数据采集
    bool startAcquisition() override;
    bool stopAcquisition() override;
    bool isAcquiring() const override;
    std::vector<double> readData() override;
    
    // 设备校准和测试
    bool performSelfTest() override;
    bool calibrate() override;
    std::string getLastError() const override;

private:
    bool connected_;
    bool acquiring_;
    double currentVoltage_;
    CurrentRange currentRange_;
    std::string serialNumber_;
    std::string firmwareVersion_;
    std::string lastError_;
    double temperature_;
};