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

if(APPLE)
    # Ensure Qt frameworks are found at runtime
    set_target_properties(nanopore_cpp PROPERTIES MACOSX_BUNDLE TRUE)
endif()