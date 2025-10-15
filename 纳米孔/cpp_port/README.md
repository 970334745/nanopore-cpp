# Nanopore C++ 项目说明

> 路径：`/Users/wangyi/Desktop/project/project/纳米孔/cpp_port`

本项目是使用 Qt6 与 C++ 编写的纳米孔实验控制与数据分析桌面应用。应用将设备控制、数据采集、图表展示、统计/谱分析、事件检测与报告生成整合到一个统一的 GUI 中。

## 目录结构

```
cpp_port/
├── CHART_FEATURES.md              # 图表功能清单与设计说明
├── CMakeLists.txt                 # CMake 构建配置（Qt6 Widgets/Core/Gui/Charts）
├── build/                         # 构建输出目录（含 .app 包与 CMake 临时文件）
├── src/
│   ├── main.cpp                   # 应用入口（QApplication + AppWindow）
│   ├── ui/                        # 界面层（Qt Widgets/Charts）
│   │   ├── AppWindow.{h,cpp}      # 主窗口，菜单/工具栏/状态栏与页面切换
│   │   ├── ControlPage.{h,cpp}    # 控制页：设备连接、芯片检查、打孔/扩孔控制与图表
│   │   ├── CollectionPage.{h,cpp} # 采集页：实时采样、图表显示、导出/报告/高级分析入口
│   │   ├── AnalysePage.{h,cpp}    # 分析页：文件级事件检测参数设置与执行
│   │   ├── IdePage.{h,cpp}        # 简易 IDE：项目树浏览、文本编辑与构建工具条
│   │   ├── ParameterConfigPage.{h,cpp} # 参数配置：设备/分析/显示参数分标签管理
│   │   ├── DataAnalysisDialog.{h,cpp}  # 高级数据分析对话框
│   │   ├── DeviceManagementDialog.{h,cpp} # 设备管理对话框
│   │   ├── DataExportDialog.{h,cpp} # 数据导出对话框
│   │   ├── StyleManager.{h,cpp}     # 主题与统一样式管理
│   │   └── styles.qss               # 统一样式表（可选）
│   ├── algorithms/                  # 算法层：预处理、事件检测、积分等
│   │   ├── DataProcessor.{h,cpp}    # 预处理流水线框架（滤波/基线/去噪/离群）
│   │   ├── DataProcessorImpl.cpp    # 预处理流水线实现
│   │   ├── DynamicDetectionWrapper.{h,cpp} # 动态事件检测包装器
│   │   ├── DynamicRms.{h,cpp}       # 动态 RMS 计算
│   │   ├── EventDetection.{h,cpp}   # 事件检测核心逻辑
│   │   ├── Integral.{h,cpp}         # 数值积分相关
│   ├── analysis/                     # 分析层：统计/谱/分类/报告
│   │   ├── StatisticalAnalysis.{h,cpp} # 统计分析
│   │   ├── SpectralAnalysis.{h,cpp}    # 功率谱密度等谱分析
│   │   ├── EventClassification.{h,cpp} # 事件分类
│   │   ├── ReportGenerator.{h,cpp}     # 报告生成（统计数据、图表快照）
│   ├── hardware/                     # 硬件层：设备抽象与驱动
│   │   ├── DeviceInterface.h         # 统一设备接口
│   │   ├── DeviceManager.{h,cpp}     # 设备管理器（选择/连接/状态）
│   │   ├── Er4Device.{h,cpp}         # ER4 设备驱动
│   │   ├── KeithleyDevice.{h,cpp}    # Keithley 设备驱动
│   │   ├── ScpiCommunication.{h,cpp} # SCPI 通信封装
│   ├── utils/                        # 工具层：文件/校验/路径等
│   │   ├── FileIO.{h,cpp}            # 数据文件读写
│   │   ├── FileManager.{h,cpp}       # 文件管理与路径工具
│   │   ├── FileSystemUtils.cpp       # 文件系统辅助方法
│   │   └── DataValidation.cpp        # 数据校验
│   └── ui/之外的依赖
│       └── ../代码/c++/dynamic_detection.cpp # 复用的动态检测实现（外部子目录）
├── test_chart_controls.py            # 图表控件的 Python 测试（示范/验证）
└── test_charts.py                    # 图表行为的 Python 测试（示范/验证）
```

## 模块说明与职责

- `AppWindow`：主窗口，包含菜单（页面切换、主题）、工具栏（保存/加载/导出/设置）、状态栏（连接状态/系统状态/进度），用 `QStackedWidget` 管理五个页面。
- `ControlPage`：
  - 设备与存储路径区：设备枚举与连接、数据保存路径设置。
  - 模式选择与流程控制：开始/停止；芯片检查、打孔、电压扩孔参数与执行按钮。
  - 图表区：打孔/扩孔的实时电压/电流/孔径等子图；高级图（直方图、散点、功率谱、统计、事件分析）以绿色按钮方式打开独立窗口（便于对比与交互）。
- `CollectionPage`：
  - 采集参数与控制：采样率/通道选择、开始/停止、文件保存。
  - 实时图表：时间-电流/电压等曲线，支持轴/网格样式。
  - 快速分析与报告：一键分析、生成报告（调用 `ReportGenerator`），进入 `DataAnalysisDialog` 进行高级配置。
- `AnalysePage`：
  - 面向离线数据文件的事件检测；配置 `窗口(ms)`、`sigma`、采样率与方向等参数；调用算法层进行检测并在界面显示结果摘要。
- `ParameterConfigPage`：
  - 设备、分析、显示三类参数分标签管理；支持默认参数加载/保存，滚动区域保证复杂参数组可视。
- `StyleManager`/`styles.qss`：统一颜色/字体/主题切换（亮/暗/蓝色/高对比）。
- `algorithms/*`：数据预处理（滤波、基线、去噪、离群）、动态 RMS、事件检测、积分工具；`dynamic_detection.cpp` 复用外部子目录的既有实现。
- `analysis/*`：统计分析、功率谱密度、事件分类、报告生成（可嵌入图表快照和统计表）。
- `hardware/*`：设备抽象与具体实现（ER4、Keithley），统一通过 `DeviceInterface` 提供读写/配置接口，`ScpiCommunication` 封装指令通讯。
- `utils/*`：文件读写与路径工具、数据校验。

## 整体功能概述

- 设备管理与连接：选择并连接实验设备，查看连接状态、系统状态与进度。
- 芯片检查与打孔/扩孔：设置电压/阈值/脉冲时间等参数，执行流程并实时查看图表。
- 数据采集与实时显示：按设定采样率采集数据，绘制趋势曲线并保存。
- 数据分析：
  - 快速分析：常用统计与谱分析；
  - 高级分析：进入对话框配置并生成报告；
  - 离线事件检测：针对历史文件进行事件检测与摘要展示。
- 报告生成与数据导出：生成包含统计数据与图表快照的报告，导出原始或处理后的数据文件。
- 主题与样式：提供亮/暗/蓝色/高对比主题切换，统一风格管理。

## 运行流程（高层）

1. 启动应用：`main.cpp` 创建 `QApplication` 与 `AppWindow`，显示主窗体。
2. 初始化页面：`AppWindow::setupPages()` 创建并注册 `ControlPage`、`CollectionPage`、`AnalysePage`、`IdePage`、`ParameterConfigPage`，默认显示“CBD打孔”（ControlPage）。
3. 菜单与工具栏：通过菜单切换页面、切换主题；工具栏支持保存/加载/导出/设置。
4. 状态更新：`QTimer` 定时刷新系统状态与连接状态，状态栏显示当前进度与消息。
5. 扩孔/打孔图表：实时刷新；扩孔区子图已增大高度并开启坐标轴标签（白色数字，便于深色背景下查看）。
6. 高级图按钮：点击绿色按钮打开独立窗口，分别显示直方图、散点图、功率谱密度、统计分析、事件分析。
7. 采集与分析：在 `CollectionPage` 采集数据，进行快速/高级分析与报告生成；在 `AnalysePage` 对离线文件执行事件检测。

## 构建与启动（macOS）

### 依赖

- CMake ≥ 3.20
- Qt6（Widgets、Core、Gui、Charts 组件）
- C++17 编译器（Apple Clang 兼容）

### 安装 Qt6（Homebrew）

```bash
brew install qt
# 可选：查看 Qt 的 cmake 路径
brew --prefix qt
# 一般为：/opt/homebrew/opt/qt
```

### 配置与构建

```bash
cd /Users/wangyi/Desktop/project/project/纳米孔/cpp_port

# 配置：指定 Qt 的 CMake 包路径
cmake -S . -B build -DCMAKE_PREFIX_PATH="$(brew --prefix qt)/lib/cmake"

# 构建
cmake --build build -j
```

### 启动应用

```bash
# 启动 .app 包（推荐）
open build/nanopore_cpp.app

# 或直接运行二进制
./build/nanopore_cpp.app/Contents/MacOS/nanopore_cpp
```

> 项目输出（应用包）位于：`/Users/wangyi/Desktop/project/project/纳米孔/cpp_port/build/nanopore_cpp.app`

## 构建与启动（Windows）

### 依赖

- CMake ≥ 3.20（Windows Installer）
- Visual Studio 2022（或 Visual Studio Build Tools），安装“Desktop development with C++”工作负载与最新 Windows 10/11 SDK
- Qt6（选择对应工具链的套件，并勾选 `Qt Charts` 模块）

> 推荐使用 MSVC 2022 64-bit 套件：`Qt 6.x -> MSVC 2022 64-bit`，路径类似 `C:\Qt\6.6.3\msvc2022_64`。如使用 MinGW，请选择 `Qt 6.x -> MinGW 64-bit`，路径类似 `C:\Qt\6.6.3\mingw_64`。

### 方案 A：MSVC 2022（Visual Studio）

1) 配置与生成 VS 工程

```bat
cd /d C:\Users\<你的用户名>\Desktop\project\project\纳米孔\cpp_port

cmake -S . -B build ^
  -G "Visual Studio 17 2022" -A x64 ^
  -DCMAKE_PREFIX_PATH="C:\Qt\6.6.3\msvc2022_64\lib\cmake"
```

2) 构建 Release 可执行文件（命令行或在 VS 中打开 `build/NanoporeCpp.sln`）

```bat
cmake --build build --config Release -j
```

3) 运行（未打包时需要 Qt 运行时）

```bat
build\Release\nanopore_cpp.exe
```

4) 部署（拷贝所需 Qt DLL 到同一目录）

使用 `windeployqt`（位于 `C:\Qt\6.6.3\msvc2022_64\bin\windeployqt.exe`）：

```bat
"C:\Qt\6.6.3\msvc2022_64\bin\windeployqt.exe" --release build\Release\nanopore_cpp.exe
```

执行后，`Qt6Core.dll`、`Qt6Widgets.dll`、`Qt6Gui.dll`、`Qt6Charts.dll` 及相关插件会被复制到同目录，可直接分发该目录（建议打包为 zip）。

### 方案 B：MinGW 64-bit

1) 安装带 MinGW 套件的 Qt6，并确保 `mingw32-make` 在 PATH 中。

2) 配置与构建

```bat
cmake -S . -B build -G "MinGW Makefiles" ^
  -DCMAKE_PREFIX_PATH="C:\Qt\6.6.3\mingw_64\lib\cmake" ^
  -DCMAKE_BUILD_TYPE=Release

cmake --build build -j
```

3) 运行

```bat
build\nanopore_cpp.exe
```

4) 部署

```bat
"C:\Qt\6.6.3\mingw_64\bin\windeployqt.exe" --release build\nanopore_cpp.exe
```

> MinGW 构建默认生成到 `build\` 根目录（非 VS 的 `Release/Debug` 子目录）。

### 在 Visual Studio 中运行

- 用上述 MSVC 配置后，打开 `build\NanoporeCpp.sln`。
- 将启动项设置为 `nanopore_cpp`，选择 `Release|x64` 构建并运行。
- 如需独立分发，先执行 `windeployqt` 将依赖打包到可执行文件同目录。

### 常见问题（Windows）

- `QWidget file not found` 或 `cannot find Qt6`：
  - 检查是否正确设置 `-DCMAKE_PREFIX_PATH=C:\Qt\<版本>\<工具链>\lib\cmake`。
  - 选择的 Qt 版本与工具链需匹配（MSVC 2022 vs MinGW）。
- 链接错误 `LNK1104: cannot open file 'Qt6Charts.lib'` 或 MinGW 的 `cannot find -lQt6Charts`：
  - 在 Qt 安装器中勾选并安装 `Qt Charts` 模块，确保该模块与所选工具链一致。
- 运行时报错 `Could not find the Qt platform plugin "windows"`：
  - 未正确部署依赖。请使用 `windeployqt --release`，确认 `platforms\qwindows.dll` 已被复制。
- `windeployqt` 未识别：
  - 使用该工具的绝对路径，或将 `C:\Qt\<版本>\<工具链>\bin` 加入 PATH。

## 云端自动打包 EXE（GitHub Actions）

无需在本地安装 Qt/VS，也可在 GitHub 云端生成可分发的 EXE：

1) 将本项目推送到 GitHub 仓库（保持目录结构）。
2) 仓库根包含工作流：`.github/workflows/windows-exe.yml`。
3) 在仓库的 Actions 页面运行该工作流（或推送后自动触发）。
   - 流水线会：安装 Qt6（含 Charts）→ 用 MSVC 2022 构建 → 执行 `windeployqt` → 打包为 `nanopore_cpp_windows.zip`。
4) 在 Actions 的构建结果中下载 Artifact：`nanopore_cpp_windows.zip`。
5) 将解压后的目录发给学生，双击其中的 `nanopore_cpp.exe` 即可运行。

注意：
- 工作流使用路径 `纳米孔/cpp_port` 作为工程根目录；如你移动目录，请同步修改 workflow 中 `cmake -S` 的路径。
- 若仓库名称或路径包含中文，GitHub Actions 仍可正常处理；如遇编码问题，可改为英文路径并更新工作流。

## 云端自动打包 EXE（GitLab CI）

GitLab 也可以。需要一个 Windows Runner（自托管）：

1) 在一台 Windows 10/11 机器上预装：
   - Visual Studio 2022 Build Tools（C++ 工作负载与 Windows SDK）
   - Python 3.11、CMake（将其加入 PATH）
2) 安装并注册 GitLab Runner（shell 执行器），为 Runner 设置标签，如 `windows`。
   - 安装：`choco install gitlab-runner`
   - 注册：`gitlab-runner register` → 输入 GitLab 地址与注册令牌 → 选择 `shell` → 设置 `tags=windows`
3) 在仓库根目录放置 `.gitlab-ci.yml`（本项目已提供于 `纳米孔/cpp_port/.gitlab-ci.yml`，若你的仓库根不是该目录，请把该文件移到仓库根）。
4) 推送后，GitLab CI 会在带有 `windows` 标签的 Runner 上执行：安装 Qt6 → 构建 → `windeployqt` → 打包 `nanopore_cpp_windows.zip` 为工件（Artifacts）。
5) 在 Job 的 Artifacts 中下载 `nanopore_cpp_windows.zip`，发给学生即可运行。

> 如使用 MinGW，可将 `.gitlab-ci.yml` 中的 `msvc2022_64` 改为 `mingw_64` 并把生成器改为 `MinGW Makefiles`，其余步骤一致。


## 常用操作

- 页面切换：菜单“CBD打孔 / 数据采集 / 数据分析 / 代码编辑 / 参数配置”。
- 主题切换：菜单“主题”中选择“亮色/暗色/蓝色/高对比”。
- 设备与路径：在 `ControlPage` 顶部选择设备、设置数据保存路径。
- 扩孔与打孔：设置参数后点击对应绿色按钮执行；图表自动更新。
- 高级图表：点击“电流分布直方图 / 电流-时间散点图 / 功率谱密度 / 统计分析 / 事件分析”绿色按钮，打开独立窗口查看。
- 数据采集：在 `CollectionPage` 设置采样率等参数，开始采集并查看实时曲线。
- 快速/高级分析与报告：在 `CollectionPage` 进行一键分析、生成报告或打开高级分析对话框。
- 离线事件检测：在 `AnalysePage` 打开数据文件并配置参数后运行检测。

## 重要设计与交互细节

- 图表统一使用 Qt Charts，并配置网格、轴、图例风格以匹配主题；深色背景下轴标签为白色。
- 扩孔区子图（电压与孔径）已增大高度（约 190–230）并显示坐标刻度（6 级），便于观察细节。
- 高级图使用独立窗口展示，避免在主页面挤压布局，提升交互体验。
- 状态栏持续显示连接状态与系统信息，进度条在需要时可见。

## 故障排查（编译/运行）

- “`QWidget` file not found”等头文件错误：
  - 请确认使用 Qt6 并在配置时设置 `CMAKE_PREFIX_PATH` 指向 Qt 的 `lib/cmake` 路径（见上文）。
  - 头文件建议使用模块前缀（如 `#include <QtWidgets/QWidget>`、`#include <QtCharts/QChartView>`），已在 `ControlPage.cpp` 中调整。
- 运行时找不到 Qt 依赖：
  - 建议通过 `open build/nanopore_cpp.app` 启动，由 macOS `.app` 带框架路径解析。
  - 若直接运行可执行文件，请确保环境变量能找到 Qt Framework。

## 测试与验证

- `test_chart_controls.py` 与 `test_charts.py` 为图表控件行为的示例/测试脚本（Python），可用于快速验证数据与图表映射逻辑。示例：

```bash
python3 test_chart_controls.py
```

## 开发建议

- 统一通过 `StyleManager` 控制主题与样式，避免页面各自配置不一致。
- 算法与分析层尽量保持无 UI 依赖，便于在测试或服务端复用。
- 新增设备遵循 `DeviceInterface`，在 `DeviceManager` 中注册并在 `ControlPage`/`CollectionPage` 暴露选择项。

---

如需进一步自定义（例如增加新的图表、调整扩孔图尺寸、添加新的报告版式），请直接告知具体目标，我会在相应模块中进行优化并说明修改点。