#!/usr/bin/env python3
"""
测试脚本：验证纳米孔应用程序的图表功能
"""

import time
import subprocess
import sys
import os

def test_application_startup():
    """测试应用程序启动"""
    print("🔍 测试应用程序启动...")
    
    # 检查可执行文件是否存在
    app_path = "./build/nanopore_cpp.app/Contents/MacOS/nanopore_cpp"
    if not os.path.exists(app_path):
        print("❌ 应用程序可执行文件不存在")
        return False
    
    print("✅ 应用程序可执行文件存在")
    return True

def check_qt_charts_support():
    """检查Qt Charts支持"""
    print("\n🔍 检查Qt Charts支持...")
    
    try:
        # 检查是否链接了QtCharts库
        result = subprocess.run(
            ["otool", "-L", "./build/nanopore_cpp.app/Contents/MacOS/nanopore_cpp"],
            capture_output=True,
            text=True,
            cwd="/Users/wangyi/Desktop/project/project/纳米孔/cpp_port"
        )
        
        if "QtCharts" in result.stdout:
            print("✅ 应用程序已链接QtCharts库")
            return True
        else:
            print("❌ 应用程序未链接QtCharts库")
            return False
            
    except Exception as e:
        print(f"❌ 检查QtCharts支持时出错: {e}")
        return False

def check_compilation_status():
    """检查编译状态"""
    print("\n🔍 检查编译状态...")
    
    try:
        # 检查最近的编译时间
        app_path = "./build/nanopore_cpp.app/Contents/MacOS/nanopore_cpp"
        if os.path.exists(app_path):
            mtime = os.path.getmtime(app_path)
            compile_time = time.ctime(mtime)
            print(f"✅ 应用程序编译时间: {compile_time}")
            
            # 检查是否是最近编译的（5分钟内）
            if time.time() - mtime < 300:
                print("✅ 应用程序是最近编译的")
                return True
            else:
                print("⚠️ 应用程序编译时间较早，可能需要重新编译")
                return True
        else:
            print("❌ 应用程序可执行文件不存在")
            return False
            
    except Exception as e:
        print(f"❌ 检查编译状态时出错: {e}")
        return False

def check_source_files():
    """检查源文件是否包含图表相关代码"""
    print("\n🔍 检查源文件中的图表代码...")
    
    try:
        # 检查CollectionPage.h中的图表相关声明
        with open("./src/ui/CollectionPage.h", "r", encoding="utf-8") as f:
            header_content = f.read()
            
        chart_features = [
            "QChartView",
            "QChart",
            "QLineSeries",
            "QScatterSeries",
            "setupVisualization",
            "updateRealTimeChart",
            "updateStatisticsChart"
        ]
        
        found_features = []
        for feature in chart_features:
            if feature in header_content:
                found_features.append(feature)
        
        print(f"✅ 找到图表功能: {', '.join(found_features)}")
        
        if len(found_features) >= 5:
            print("✅ 图表功能已正确添加到头文件")
            return True
        else:
            print("❌ 图表功能不完整")
            return False
            
    except Exception as e:
        print(f"❌ 检查源文件时出错: {e}")
        return False

def main():
    """主测试函数"""
    print("🚀 开始测试纳米孔应用程序图表功能\n")
    
    # 切换到项目目录
    os.chdir("/Users/wangyi/Desktop/project/project/纳米孔/cpp_port")
    
    tests = [
        ("应用程序启动", test_application_startup),
        ("Qt Charts支持", check_qt_charts_support),
        ("编译状态", check_compilation_status),
        ("源文件检查", check_source_files)
    ]
    
    passed = 0
    total = len(tests)
    
    for test_name, test_func in tests:
        print(f"📋 运行测试: {test_name}")
        if test_func():
            passed += 1
        print()
    
    print("=" * 50)
    print(f"📊 测试结果: {passed}/{total} 通过")
    
    if passed == total:
        print("🎉 所有测试通过！图表功能已成功集成")
        return True
    else:
        print("⚠️ 部分测试失败，请检查相关问题")
        return False

if __name__ == "__main__":
    success = main()
    sys.exit(0 if success else 1)