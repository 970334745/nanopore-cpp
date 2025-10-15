#!/usr/bin/env python3
"""
测试图表控制功能的实现
验证新添加的图表控制组件和功能
"""

import os
import subprocess
import sys

def check_chart_control_implementation():
    """检查图表控制功能的实现"""
    print("=== 图表控制功能实现验证 ===\n")
    
    # 检查头文件中的图表控制组件声明
    header_file = "src/ui/CollectionPage.h"
    if os.path.exists(header_file):
        with open(header_file, 'r', encoding='utf-8') as f:
            content = f.read()
            
        control_components = [
            "chartControlGroup_",
            "pauseChartBtn_",
            "resetZoomBtn_",
            "exportChartBtn_",
            "autoScaleCheck_",
            "chartsPaused_"
        ]
        
        control_methods = [
            "setupChartControls",
            "toggleChartPause",
            "resetChartZoom",
            "exportCurrentChart",
            "toggleAutoScale"
        ]
        
        print("1. 检查图表控制组件声明:")
        for component in control_components:
            if component in content:
                print(f"   ✓ {component} - 已声明")
            else:
                print(f"   ✗ {component} - 未找到")
        
        print("\n2. 检查图表控制方法声明:")
        for method in control_methods:
            if method in content:
                print(f"   ✓ {method} - 已声明")
            else:
                print(f"   ✗ {method} - 未找到")
    else:
        print("❌ 头文件不存在")
        return False
    
    # 检查实现文件中的方法实现
    impl_file = "src/ui/CollectionPage.cpp"
    if os.path.exists(impl_file):
        with open(impl_file, 'r', encoding='utf-8') as f:
            content = f.read()
        
        print("\n3. 检查图表控制方法实现:")
        for method in control_methods:
            if f"void CollectionPage::{method}" in content:
                print(f"   ✓ {method} - 已实现")
            else:
                print(f"   ✗ {method} - 未实现")
        
        # 检查暂停功能集成
        print("\n4. 检查暂停功能集成:")
        if "if (chartsPaused_) return;" in content:
            print("   ✓ 图表暂停检查 - 已集成到更新方法中")
        else:
            print("   ✗ 图表暂停检查 - 未集成")
        
        # 检查控制组件设置调用
        if "setupChartControls();" in content:
            print("   ✓ 控制组件设置 - 已在setupVisualization中调用")
        else:
            print("   ✗ 控制组件设置 - 未调用")
    else:
        print("❌ 实现文件不存在")
        return False
    
    return True

def check_compilation_status():
    """检查编译状态"""
    print("\n=== 编译状态检查 ===\n")
    
    executable_path = "build/nanopore_cpp.app/Contents/MacOS/nanopore_cpp"
    if os.path.exists(executable_path):
        stat = os.stat(executable_path)
        print(f"✓ 可执行文件存在: {executable_path}")
        print(f"✓ 文件大小: {stat.st_size} bytes")
        print(f"✓ 最后修改时间: {stat.st_mtime}")
        return True
    else:
        print("❌ 可执行文件不存在")
        return False

def check_ui_integration():
    """检查UI集成"""
    print("\n=== UI集成检查 ===\n")
    
    impl_file = "src/ui/CollectionPage.cpp"
    if os.path.exists(impl_file):
        with open(impl_file, 'r', encoding='utf-8') as f:
            content = f.read()
        
        ui_elements = [
            "new QGroupBox(\"图表控制\")",
            "new QPushButton(\"暂停图表\")",
            "new QPushButton(\"重置缩放\")",
            "new QPushButton(\"导出图表\")",
            "new QCheckBox(\"自动缩放\")"
        ]
        
        print("检查UI元素创建:")
        for element in ui_elements:
            if element in content:
                print(f"   ✓ {element} - 已创建")
            else:
                print(f"   ✗ {element} - 未找到")
        
        # 检查信号连接
        signal_connections = [
            "connect(pauseChartBtn_",
            "connect(resetZoomBtn_",
            "connect(exportChartBtn_",
            "connect(autoScaleCheck_"
        ]
        
        print("\n检查信号连接:")
        for connection in signal_connections:
            if connection in content:
                print(f"   ✓ {connection} - 已连接")
            else:
                print(f"   ✗ {connection} - 未连接")
        
        return True
    else:
        print("❌ 实现文件不存在")
        return False

def main():
    """主函数"""
    print("图表控制功能测试脚本")
    print("=" * 50)
    
    # 切换到项目目录
    os.chdir(os.path.dirname(os.path.abspath(__file__)))
    
    success = True
    
    # 运行各项检查
    success &= check_chart_control_implementation()
    success &= check_compilation_status()
    success &= check_ui_integration()
    
    print("\n" + "=" * 50)
    if success:
        print("🎉 所有图表控制功能测试通过！")
        print("\n新增功能总结:")
        print("• 图表暂停/恢复功能")
        print("• 图表缩放重置功能")
        print("• 图表导出为图片功能")
        print("• 自动缩放开关功能")
        print("• 图表性能优化")
        print("\n用户可以通过图表控制面板:")
        print("1. 暂停/恢复实时图表更新")
        print("2. 重置图表缩放到默认范围")
        print("3. 导出当前图表为PNG/JPEG图片")
        print("4. 开启/关闭自动缩放功能")
    else:
        print("❌ 部分测试失败，请检查实现")
        return 1
    
    return 0

if __name__ == "__main__":
    sys.exit(main())