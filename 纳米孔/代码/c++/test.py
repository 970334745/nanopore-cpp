import dynamic_detection
import numpy as np
import matplotlib.pyplot as plt
import matplotlib
matplotlib.use('TKAgg')
# 生成测试数据
data = np.random.randn(20000).astype(np.float64)
window_size = 501  # 奇数窗口
sigma = 3.0

# 调用C++扩展
baseline = dynamic_detection.calculate_dynamic_baseline_fast_trimmed_py(data, window_size)
std_dev = dynamic_detection.calculate_dynamic_std_py(data, window_size)
down, up = dynamic_detection.detection_y_py(data, window_size, sigma)

# 验证结果
print(f"基线长度: {len(baseline)}, 标准差长度: {len(std_dev)}")
print(f"前5个下限值: {down[:5]}")
print(f"前5个上限值: {up[:5]}")
plt.plot(data)
plt.plot(baseline)
plt.plot(down)
plt.plot(up)
plt.show()
