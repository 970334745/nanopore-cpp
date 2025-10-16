#include <iostream>
#include <string>

// 兼容不同索引器/编译器的头文件查找路径
#if __has_include("../hardware/ScpiCommunication.h")
#  include "../hardware/ScpiCommunication.h"
#elif __has_include("hardware/ScpiCommunication.h")
#  include "hardware/ScpiCommunication.h"
#elif __has_include("ScpiCommunication.h")
#  include "ScpiCommunication.h"
#else
#  error "ScpiCommunication.h not found"
#endif

int main(int argc, char** argv) {
    std::string resource = "TCPIP0::127.0.0.1::5025::SOCKET";
    if (argc > 1) {
        resource = argv[1];
    }
    ScpiCommunication scpi;
    if (!scpi.openResource(resource)) {
        std::cerr << "Failed to open resource: " << resource << std::endl;
        return 1;
    }
    auto idn = scpi.queryCommand("*IDN?");
    if (!idn.success) {
        std::cerr << "*IDN? failed: " << idn.errorMessage << std::endl;
        return 2;
    }
    std::cout << "IDN: " << idn.response << std::endl;

    if (!scpi.sendCommand("SOUR:VOLT 0.1").success) {
        std::cerr << "Set voltage failed" << std::endl;
        return 3;
    }
    auto volt = scpi.queryCommand("SOUR:VOLT?");
    if (!volt.success) {
        std::cerr << "Query voltage failed" << std::endl;
        return 4;
    }
    std::cout << "Voltage: " << volt.response << std::endl;

    if (!scpi.sendCommand("OUTP ON").success) {
        std::cerr << "Enable output failed" << std::endl;
        return 5;
    }
    auto curr = scpi.queryCommand("MEAS:CURR?");
    if (!curr.success) {
        std::cerr << "Measure current failed" << std::endl;
        return 6;
    }
    std::cout << "Current: " << curr.response << std::endl;

    std::cout << "SCPI smoke test passed." << std::endl;
    return 0;
}