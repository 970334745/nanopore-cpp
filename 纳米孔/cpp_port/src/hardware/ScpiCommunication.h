#pragma once
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <memory>

// SCPI命令结果结构
struct ScpiResult {
    bool success;
    std::string response;
    std::string errorMessage;
    int errorCode;
};

// SCPI设备信息
struct ScpiDeviceInfo {
    std::string manufacturer;
    std::string model;
    std::string serialNumber;
    std::string firmwareVersion;
    std::string resourceString;
};

// SCPI通信接口类
class ScpiCommunication {
public:
    using ErrorCallback = std::function<void(const std::string&, int)>;
    using ResponseCallback = std::function<void(const std::string&, const std::string&)>;

    ScpiCommunication();
    ~ScpiCommunication();

    // 资源管理
    std::vector<std::string> findResources();
    bool openResource(const std::string& resourceString);
    void closeResource();
    bool isResourceOpen() const;
    std::string getCurrentResource() const;

    // 基本SCPI通信
    ScpiResult sendCommand(const std::string& command);
    ScpiResult queryCommand(const std::string& command);
    ScpiResult queryCommand(const std::string& command, double timeoutMs);

    // 设备识别和信息
    ScpiDeviceInfo getDeviceInfo();
    std::string getIdentification();
    bool resetDevice();
    bool clearErrors();
    std::vector<std::string> getErrorQueue();

    // 电压和电流控制
    bool setVoltage(double voltage);
    double getVoltage();
    bool setCurrentLimit(double current);
    double getCurrentLimit();
    bool setCurrentRange(double range);
    double getCurrentRange();

    // 输出控制
    bool enableOutput(bool enable);
    bool isOutputEnabled();
    double measureCurrent();
    double measureVoltage();

    // 数据采集
    bool startContinuousMeasurement();
    bool stopContinuousMeasurement();
    std::vector<double> readMeasurementData();
    bool configureTrigger(const std::string& source, double level = 0.0);

    // 系统配置
    bool setRemoteMode(bool remote);
    bool setDisplayText(const std::string& text);
    bool beep();
    
    // 高级功能
    bool performSelfTest();
    bool calibrate();
    std::string getSystemStatus();
    
    // 回调设置
    void setErrorCallback(ErrorCallback callback);
    void setResponseCallback(ResponseCallback callback);
    
    // 超时设置
    void setTimeout(double timeoutMs);
    double getTimeout() const;

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl_;
    
    // 内部辅助函数
    bool validateCommand(const std::string& command);
    std::string formatCommand(const std::string& command);
    ScpiResult executeCommand(const std::string& command, bool expectResponse);
    void handleError(const std::string& message, int code);
    void logCommunication(const std::string& command, const std::string& response);
};