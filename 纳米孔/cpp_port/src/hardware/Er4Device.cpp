#include "Er4Device.h"
#include <random>
#include <cmath>

Er4Device::Er4Device() 
    : connected_(false)
    , acquiring_(false)
    , currentVoltage_(0.0)
    , currentRange_{-1e-9, 1e-9, 1e-12}
    , serialNumber_("ER4-SIM-001")
    , firmwareVersion_("v1.2.3")
    , lastError_("")
    , temperature_(25.0) {
}

std::vector<DeviceInfo> Er4Device::detect() {
    DeviceInfo device;
    device.name = "ER4 仿真设备";
    device.id = "sim-001";
    device.serialNumber = serialNumber_;
    device.firmwareVersion = firmwareVersion_;
    device.isConnected = connected_;
    return { device };
}

bool Er4Device::connect(const std::string& id) {
    if (id == "sim-001") {
        connected_ = true;
        lastError_ = "";
        return true;
    }
    lastError_ = "设备ID不匹配";
    return false;
}

void Er4Device::disconnect() {
    connected_ = false;
    acquiring_ = false;
    currentVoltage_ = 0.0;
    lastError_ = "";
}

bool Er4Device::isConnected() const {
    return connected_;
}

DeviceStatus Er4Device::getStatus() const {
    DeviceStatus status;
    status.isConnected = connected_;
    status.temperature = temperature_;
    status.voltage = currentVoltage_;
    status.current = acquiring_ ? (std::sin(clock() * 0.001) * 1e-10) : 0.0;
    status.lastError = lastError_;
    return status;
}

std::string Er4Device::getSerialNumber() const {
    return serialNumber_;
}

std::string Er4Device::getFirmwareVersion() const {
    return firmwareVersion_;
}

bool Er4Device::setVoltage(double voltage) {
    if (!connected_) {
        lastError_ = "设备未连接";
        return false;
    }
    
    VoltageRange range = getVoltageRange();
    if (voltage < range.min || voltage > range.max) {
        lastError_ = "电压超出范围";
        return false;
    }
    
    currentVoltage_ = voltage;
    lastError_ = "";
    return true;
}

double Er4Device::getVoltage() const {
    return currentVoltage_;
}

VoltageRange Er4Device::getVoltageRange() const {
    return {-1.0, 1.0, 0.001};
}

bool Er4Device::setCurrentRange(const CurrentRange& range) {
    if (!connected_) {
        lastError_ = "设备未连接";
        return false;
    }
    
    currentRange_ = range;
    lastError_ = "";
    return true;
}

CurrentRange Er4Device::getCurrentRange() const {
    return currentRange_;
}

bool Er4Device::startAcquisition() {
    if (!connected_) {
        lastError_ = "设备未连接";
        return false;
    }
    
    acquiring_ = true;
    lastError_ = "";
    return true;
}

bool Er4Device::stopAcquisition() {
    acquiring_ = false;
    lastError_ = "";
    return true;
}

bool Er4Device::isAcquiring() const {
    return acquiring_;
}

std::vector<double> Er4Device::readData() {
    if (!acquiring_) {
        return {};
    }
    
    // 生成更真实的纳米孔仿真数据
    std::vector<double> data;
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::normal_distribution<> noise(0.0, 2e-12);  // 增加噪声水平
    static std::uniform_real_distribution<> eventProb(0.0, 1.0);
    static std::exponential_distribution<> eventDuration(0.1);  // 事件持续时间分布
    static std::normal_distribution<> eventAmplitude(-3e-10, 1e-10);  // 事件幅度分布
    static double time = 0.0;
    static double eventEndTime = 0.0;
    static double currentEventAmplitude = 0.0;
    
    const double samplingRate = 10000.0;  // 10 kHz
    const double dt = 1.0 / samplingRate;
    const double baselineCurrent = 1e-10;  // 基线电流
    
    for (int i = 0; i < 100; ++i) {
        time += dt;
        
        double current = baselineCurrent;
        
        // 添加1/f噪声和白噪声
        double whiteNoise = noise(gen);
        double flicker = std::sin(time * 2 * M_PI * 0.1) * 5e-12;  // 低频噪声
        current += whiteNoise + flicker;
        
        // 检查是否在事件中
        if (time < eventEndTime) {
            // 在事件中，应用事件电流
            double eventProgress = (eventEndTime - time) / (eventEndTime - (eventEndTime - eventDuration(gen)));
            current += currentEventAmplitude * std::exp(-eventProgress * 3);  // 指数衰减
        } else if (eventProb(gen) < 0.001) {  // 0.1% 概率产生新事件
            // 开始新事件
            double duration = std::max(0.0001, eventDuration(gen));  // 最小100μs
            eventEndTime = time + duration;
            currentEventAmplitude = eventAmplitude(gen);
        }
        
        // 添加电容耦合效应
        static double lastCurrent = baselineCurrent;
        double coupling = 0.95;
        current = coupling * current + (1 - coupling) * lastCurrent;
        lastCurrent = current;
        
        // 添加量化噪声（模拟ADC）
        const double adcResolution = 1e-13;
        current = std::round(current / adcResolution) * adcResolution;
        
        data.push_back(current);
    }
    
    return data;
}

bool Er4Device::performSelfTest() {
    if (!connected_) {
        lastError_ = "设备未连接";
        return false;
    }
    
    // 仿真自检过程
    lastError_ = "";
    return true;
}

bool Er4Device::calibrate() {
    if (!connected_) {
        lastError_ = "设备未连接";
        return false;
    }
    
    // 仿真校准过程
    lastError_ = "";
    return true;
}

std::string Er4Device::getLastError() const {
    return lastError_;
}