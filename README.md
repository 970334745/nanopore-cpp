# nanopore-cpp（纳米孔 C++ 端口）

一个面向纳米孔平台的 C++ 项目，基于 Qt 6 构建，提供经由 TCP/IP 的 SCPI/TSP 通信能力，并在没有物理设备的环境下仍可通过模拟服务器与冒烟测试进行可靠验证。仓库集成了 Windows 自动构建与打包流程，推送后会生成可分发的 `.exe`。

## 项目特性

- 使用 `Qt6::Network` 实现 TCP/IP SCPI/TSP 传输（`纳米孔/cpp_port/src/hardware/ScpiCommunication.cpp`）。
- 兼容 Qt 模块化头文件路径（如 `QtCore/QByteArray`），并提供回退以适配不同环境的编译器索引。
- 提供 Python 模拟服务器（`纳米孔/cpp_port/scripts/scpi_mock_server.py`），支持核心 SCPI 指令如 `*IDN?`、`SOUR:VOLT`、`MEAS:CURR?`、`OUTP ON/OFF` 等。
- 提供 C++ 控制台冒烟测试（`scpi_smoke_test`），在本地或 CI 中验证网络通信与指令往返。
- Windows CI（`.github/workflows/windows-exe.yml`）在打包前自动运行冒烟测试，通过后使用 `windeployqt` 打包依赖（包含 `Qt6Network.dll`）。

## 快速开始（本地构建）

先确保安装：

- Qt 6（建议 6.6.x，至少包含 `QtBase` 与 `QtNetwork`）
- CMake ≥ 3.24、Ninja（可选）
- Python 3.8+

构建：

```
cd 纳米孔/cpp_port
cmake -B build -S . -G Ninja
cmake --build build -j
```

## 无设备验证（模拟服务器 + 冒烟测试）

1) 启动模拟服务器（默认 `127.0.0.1:5025`）：

```
python3 纳米孔/cpp_port/scripts/scpi_mock_server.py --host 127.0.0.1 --port 5025
```

2) 在另一终端运行冒烟测试：

```
cd 纳米孔/cpp_port/build
./scpi_smoke_test TCPIP0::127.0.0.1::5025::SOCKET
```

预期输出包含：

- `*IDN?` 返回设备身份（示例：`KEITHLEY INSTRUMENTS INC.,MODEL 2450,...`）
- `SOUR:VOLT 0.1` 后 `SOUR:VOLT?` 返回 `0.1`
- `OUTP ON` 后 `MEAS:CURR?` 返回一个近零电流值
- 控制台末尾显示 `SCPI smoke test passed.`

## Windows 构建与打包（GitHub Actions）

- 推送至 `main` 分支会自动触发工作流：
  - 安装 Qt（含 `QtBase` 与 `qtcharts`）
  - 配置 CMake + Ninja 构建
  - 启动模拟服务器并运行 `scpi_smoke_test`
  - 通过后执行 `windeployqt` 打包并上传工件 `nanopore_cpp_windows.zip`
- 在仓库的 Actions 页面下载工件即可获得 `.exe` 与所需 DLL。

## 常见问题

- 编译时提示找不到 `QtCore/QByteArray` 或 `QString` 等：
  - 请确认 Qt 安装路径与 CMake `find_package(Qt6 ...)` 正确。
  - 本项目在 `ScpiCommunication.cpp` 中包含了多路径 `#include` 回退策略以适配不同环境。
- 运行时找不到 `Qt6Network.dll`：
  - Windows 下须使用 `windeployqt` 或在同级目录放置网络模块 DLL。

## 目录结构（节选）

- `纳米孔/cpp_port/src/hardware/ScpiCommunication.cpp`：SCPI/TSP 通信层实现。
- `纳米孔/cpp_port/scripts/scpi_mock_server.py`：SCPI 模拟服务器。
- `纳米孔/cpp_port/src/tools/ScpiSmokeTest.cpp`：控制台冒烟测试源文件。
- `.github/workflows/windows-exe.yml`：Windows 构建与打包工作流。

## 反馈与问题

请使用 GitHub Issues 提交问题与建议，并附上：

- 复现步骤、期望行为与实际行为
- 操作系统、Qt 版本、构建日志或屏幕截图

感谢你的使用与反馈！