cmake_minimum_required(VERSION 3.20)
project(NanoporeCpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Qt6 (brew install qt)
# 增加 Network 以支持基于 QTcpSocket 的 SCPI/TSP 传输
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui Charts Network)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(nanopore_cpp
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/ui/AppWindow.cpp
    ${SRC_DIR}/ui/ControlPage.cpp
    ${SRC_DIR}/ui/CollectionPage.cpp
    ${SRC_DIR}/ui/AnalysePage.cpp
    ${SRC_DIR}/ui/IdePage.cpp
    ${SRC_DIR}/ui/ParameterConfigPage.cpp
    ${SRC_DIR}/ui/StyleManager.cpp
    ${SRC_DIR}/ui/DataAnalysisDialog.cpp
    ${SRC_DIR}/ui/DeviceManagementDialog.cpp
    ${SRC_DIR}/ui/DataExportDialog.cpp
    ${SRC_DIR}/algorithms/Integral.cpp
    ${SRC_DIR}/algorithms/EventDetection.cpp
    ${SRC_DIR}/algorithms/DynamicRms.cpp
    ${SRC_DIR}/algorithms/DynamicDetectionWrapper.cpp
    ${SRC_DIR}/algorithms/DataProcessor.cpp
    ${SRC_DIR}/algorithms/DataProcessorImpl.cpp
    ${SRC_DIR}/utils/FileManager.cpp
    ${SRC_DIR}/utils/FileSystemUtils.cpp
    ${SRC_DIR}/utils/DataValidation.cpp
    ${SRC_DIR}/utils/FileIO.cpp
    ${SRC_DIR}/analysis/StatisticalAnalysis.cpp
    ${SRC_DIR}/analysis/SpectralAnalysis.cpp
    ${SRC_DIR}/analysis/EventClassification.cpp
    ${SRC_DIR}/analysis/ReportGenerator.cpp
    ${SRC_DIR}/hardware/Er4Device.cpp
    ${SRC_DIR}/hardware/DeviceManager.cpp
    ${SRC_DIR}/hardware/ScpiCommunication.cpp
    ${SRC_DIR}/hardware/KeithleyDevice.cpp
    
    # Reuse existing C++ dynamic detection implementation
    ${CMAKE_CURRENT_SOURCE_DIR}/../代码/c++/dynamic_detection.cpp
)

target_include_directories(nanopore_cpp PRIVATE
    ${SRC_DIR}
    ${SRC_DIR}/ui
    ${SRC_DIR}/algorithms
    ${SRC_DIR}/utils
    ${SRC_DIR}/hardware
    ${SRC_DIR}/analysis
    ${CMAKE_CURRENT_SOURCE_DIR}/../代码/c++
)

target_link_libraries(nanopore_cpp PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui Qt6::Charts Qt6::Network)

if(WIN32)
    # MSVC 默认不定义 M_PI 等常量；启用 _USE_MATH_DEFINES 以从 <cmath>/<math.h> 暴露这些宏
    target_compile_definitions(nanopore_cpp PRIVATE _USE_MATH_DEFINES)
endif()

# MSVC-specific stability tweaks for Release builds
if(MSVC)
    # Use embedded debug info in Release to avoid Type Server PDB
    set_target_properties(nanopore_cpp PROPERTIES MSVC_DEBUG_INFORMATION_FORMAT "$<$<CONFIG:Release>:Embedded>$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:ProgramDatabase>")
    # Force synchronized PDB writes when parallel building
    target_compile_options(nanopore_cpp PRIVATE $<$<CONFIG:Release>:/FS> $<$<CONFIG:Release>:/Z7>)
    # Disable IPO/LTCG explicitly on the target to prevent CL.exe invoked at link time
    set_property(TARGET nanopore_cpp PROPERTY INTERPROCEDURAL_OPTIMIZATION OFF)
    # Disable Visual Studio Whole Program Optimization property (redundant safety)
    set_target_properties(nanopore_cpp PROPERTIES VS_GLOBAL_EnableWholeProgramOptimization "false")
endif()

if(APPLE)
    # Ensure Qt frameworks are found at runtime
    set_target_properties(nanopore_cpp PROPERTIES MACOSX_BUNDLE TRUE)
endif()

# 控制台 SCPI 冒烟测试，可用于 CI 与本地验证（无需真实设备）
add_executable(scpi_smoke_test
    ${SRC_DIR}/tools/ScpiSmokeTest.cpp
    ${SRC_DIR}/hardware/ScpiCommunication.cpp
)

target_include_directories(scpi_smoke_test PRIVATE
    ${SRC_DIR}
    ${SRC_DIR}/hardware
)

target_link_libraries(scpi_smoke_test PRIVATE Qt6::Core Qt6::Network)