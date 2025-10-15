#pragma once
#include "DeviceInterface.h"
#include "ScpiCommunication.h"
#include <memory>
#include <string>
#include <vector>

class KeithleyDevice : public DeviceInterface {
public:
    KeithleyDevice();
    ~KeithleyDevice() override;

    // DeviceInterface实现
    std::vector<DeviceInfo> detect() override;
    bool connect(const std::string& id) override;
    void disconnect() override;
    bool isConnected() const override;

    DeviceStatus getStatus() const override;
    std::string getSerialNumber() const override;
    std::string getFirmwareVersion() const override;

    bool setVoltage(double voltage) override;
    double getVoltage() const override;
    VoltageRange getVoltageRange() const override;
    bool setCurrentRange(const CurrentRange& range) override;
    CurrentRange getCurrentRange() const override;

    bool startAcquisition() override;
    bool stopAcquisition() override;
    bool isAcquiring() const override;
    std::vector<double> readData() override;

    bool performSelfTest() override;
    bool calibrate() override;
    std::string getLastError() const override;

    // Keithley特有功能
    bool setSourceFunction(const std::string& function);  // "VOLT" 或 "CURR"
    std::string getSourceFunction() const;
    
    bool setMeasureFunction(const std::string& function);  // "VOLT", "CURR", "RES"
    std::string getMeasureFunction() const;
    
    bool enableAutoRange(bool enable);
    bool isAutoRangeEnabled() const;
    
    bool setComplianceLimit(double limit);
    double getComplianceLimit() const;
    
    bool setNPLC(double nplc);  // Number of Power Line Cycles
    double getNPLC() const;
    
    bool setAveraging(int count);
    int getAveraging() const;
    
    // 高级测量功能
    bool configureSweep(double start, double stop, int points);
    std::vector<double> performSweep();
    
    bool configureList(const std::vector<double>& values);
    std::vector<double> performList();
    
    // 触发和定时
    bool setTriggerSource(const std::string& source);
    bool setTriggerDelay(double delay);
    bool waitForTrigger(double timeout = 10.0);
    
    // 缓冲区管理
    bool clearBuffer();
    int getBufferSize() const;
    std::vector<double> readBuffer();
    
    // 显示和用户界面
    bool setDisplayEnabled(bool enable);
    bool setDisplayText(const std::string& text);
    bool setBeeper(bool enable);

private:
    std::unique_ptr<ScpiCommunication> scpi_;
    std::string deviceId_;
    std::string resourceString_;
    bool connected_;
    bool acquiring_;
    std::string lastError_;
    
    // 设备状态缓存
    mutable DeviceStatus cachedStatus_;
    mutable bool statusCacheValid_;
    
    // 设备配置
    std::string sourceFunction_;
    std::string measureFunction_;
    bool autoRangeEnabled_;
    double complianceLimit_;
    double nplc_;
    int averagingCount_;
    
    // 内部辅助函数
    void updateStatusCache() const;
    void invalidateStatusCache();
    bool sendCommand(const std::string& command);
    std::string queryCommand(const std::string& command);
    std::string queryCommand(const std::string& command) const;
    void handleScpiError(const std::string& error, int code);
    void handleScpiResponse(const std::string& command, const std::string& response);
    
    // 设备型号检测
    std::string detectModel(const std::string& idn);
    VoltageRange getModelVoltageRange(const std::string& model) const;
    CurrentRange getModelCurrentRange(const std::string& model) const;
};