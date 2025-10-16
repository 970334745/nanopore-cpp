cmake_minimum_required(VERSION 3.20)
project(NanoporeCpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Qt6 (brew install qt)
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui Charts)

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

target_link_libraries(nanopore_cpp PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui Qt6::Charts)

if(WIN32)
    # MSVC 默认不定义 M_PI 等常量；启用 _USE_MATH_DEFINES 以从 <cmath>/<math.h> 暴露这些宏
    target_compile_definitions(nanopore_cpp PRIVATE _USE_MATH_DEFINES)
endif()

# MSVC-specific stability tweaks for Release builds
if(MSVC)
    # Avoid CL.exe ICE around Type Server PDB by using embedded debug info
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<$<CONFIG:Release>:Embedded>;$<$<CONFIG:Debug>:ProgramDatabase>;$<$<CONFIG:RelWithDebInfo>:ProgramDatabase>")
    # Force synchronized PDB writes when parallel building
    target_compile_options(nanopore_cpp PRIVATE $<$<CONFIG:Release>:/FS> $<$<CONFIG:Release>:/Z7>)
    # Disable IPO/LTCG to prevent "Generating code" stage crashes on CI
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)
endif()

if(APPLE)
    # Ensure Qt frameworks are found at runtime
    set_target_properties(nanopore_cpp PROPERTIES MACOSX_BUNDLE TRUE)
endif()