# 行 1：导入模块 sys。
import sys
# 行 2：导入模块 winsound。
import winsound
# 行 3：导入模块 tkinter，并起别名 tk
import tkinter as tk
# 行 4：从模块 concurrent.futures 导入 ThreadPoolExecutor。
from concurrent.futures import ThreadPoolExecutor
# 行 5：从模块 tkinter 导入 ttk。
from tkinter import ttk
# 行 6：导入模块 pandas，并起别名 pd
import pandas as pd
# 行 7：导入模块 math。
import math
# 行 8：导入模块 ctypes。
import ctypes
# 行 9：导入模块 pyabf。
import pyabf
# 行 10：导入模块 matplotlib。
import matplotlib
# 行 11：设置 Matplotlib 使用的后端。
matplotlib.use('TkAgg')
# 行 12：普通语句：执行一步操作或表达式。
ctypes.CDLL("./libusb-1.0.dll")
# 行 13：原始注释行，提供作者或用途说明。
# import usb1
# 行 14：导入模块 er4_python_wrapper，并起别名 c4
import er4_python_wrapper as c4
# 行 15：导入模块 dynamic_detection。
import dynamic_detection
# 行 16：导入模块 dynamic_rms。
import dynamic_rms
# 行 17：从模块 collections 导入 defaultdict。
from collections import defaultdict
# 行 18：从模块 pyvisa.ctwrapper.functions 导入 clear。
from pyvisa.ctwrapper.functions import clear
# 行 19：从模块 scipy 导入 signal。
from scipy import signal
# 行 20：从模块 matplotlib.animation 导入 FuncAnimation。
from matplotlib.animation import FuncAnimation
# 行 21：从模块 PIL 导入 Image, ImageTk。
from PIL import Image, ImageTk
# 行 22：从模块 tkinter 导入 filedialog, messagebox, StringVar。
from tkinter import filedialog, messagebox, StringVar
# 行 23：导入模块 os。
import os
# 行 24：导入模块 matplotlib.pyplot，并起别名 plt
import matplotlib.pyplot as plt
# 行 25：从模块 matplotlib.backends.backend_tkagg 导入 FigureCanvasTkAgg, NavigationToolbar2Tk。
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2Tk
# 行 26：从模块 matplotlib.figure 导入 Figure。
from matplotlib.figure import Figure
# 行 27：从模块 scipy.ndimage 导入 gaussian_filter1d。
from scipy.ndimage import gaussian_filter1d
# 行 28：导入模块 numpy，并起别名 np
import numpy as np
# 行 29：从模块 matplotlib 导入 rcParams。
from matplotlib import rcParams
# 行 30：导入模块 threading。
import threading
# 行 31：从模块 scipy.signal 导入 savgol_filter。
from scipy.signal import savgol_filter
# 行 32：从模块 datetime 导入 datetime。
from datetime import datetime
# 行 33：普通语句：执行一步操作或表达式。
rcParams['font.sans-serif'] = ['SimHei']
# 行 34：空行，用于分隔代码块，提高可读性。

# 行 35：普通语句：执行一步操作或表达式。
rcParams['axes.unicode_minus'] = False
# 行 36：导入模块 pyvisa。
import pyvisa
# 行 37：导入模块 time。
import time
# 行 38：导入模块 csv。
import csv
# 行 39：空行，用于分隔代码块，提高可读性。

# 行 40：变量赋值：保存配置、状态或计算结果。
data_lock = threading.Lock()
# 行 41：变量赋值：保存配置、状态或计算结果。
data_lock1 = threading.Lock()
# 行 42：定义类 show_page，用于封装相关属性与方法。
class show_page:
# 行 43：定义函数/方法 __init__(self)。
    def __init__(self):
# 行 44：创建 Tkinter 根窗口。
        self.root = tk.Tk()
# 行 45：普通语句：执行一步操作或表达式。
        self.root.title("固态纳米孔生物单分子检测仪软件V1--中北大学")
# 行 46：普通语句：执行一步操作或表达式。
        self.root.state('zoomed')
# 行 47：变量赋值：保存配置、状态或计算结果。
        self.page_show_number = 0
# 行 48：变量赋值：保存配置、状态或计算结果。
        self.start_page_original = tk.Frame(self.root, width=1950, height=1000, bg='#e5e5e5', bd=5, relief=tk.RAISED)
# 行 49：变量赋值：保存配置、状态或计算结果。
        self.control_page_original = None
# 行 50：变量赋值：保存配置、状态或计算结果。
        self.collection_eONE_page_original = None
# 行 51：变量赋值：保存配置、状态或计算结果。
        self.analyse_page_original = None
# 行 52：普通语句：执行一步操作或表达式。
        '''CBD'''
# 行 53：普通语句：执行一步操作或表达式。
        self.root.protocol("WM_DELETE_WINDOW", self.on_closing)
# 行 54：变量赋值：保存配置、状态或计算结果。
        self.rm = None
# 行 55：变量赋值：保存配置、状态或计算结果。
        self.kei_device = None
# 行 56：变量赋值：保存配置、状态或计算结果。
        self.kei_device_name = None
# 行 57：变量赋值：保存配置、状态或计算结果。
        self.combo_device_list = None
# 行 58：变量赋值：保存配置、状态或计算结果。
        self.kei_device_model = None
# 行 59：变量赋值：保存配置、状态或计算结果。
        self.make_way = False
# 行 60：变量赋值：保存配置、状态或计算结果。
        self.manual_check = False
# 行 61：变量赋值：保存配置、状态或计算结果。
        self.manual_make = False
# 行 62：变量赋值：保存配置、状态或计算结果。
        self.manual_expand = False
# 行 63：变量赋值：保存配置、状态或计算结果。
        self.update_completed_check = False
# 行 64：变量赋值：保存配置、状态或计算结果。
        self.a_save_path = None
# 行 65：变量赋值：保存配置、状态或计算结果。
        self.check_mean_r = None
# 行 66：变量赋值：保存配置、状态或计算结果。
        self.check_i = []
# 行 67：变量赋值：保存配置、状态或计算结果。
        self.check_v = []
# 行 68：变量赋值：保存配置、状态或计算结果。
        self.check_r = []
# 行 69：变量赋值：保存配置、状态或计算结果。
        self.check_time = []
# 行 70：变量赋值：保存配置、状态或计算结果。
        self.make_i = []
# 行 71：变量赋值：保存配置、状态或计算结果。
        self.make_v = []
# 行 72：变量赋值：保存配置、状态或计算结果。
        self.make_r = []
# 行 73：变量赋值：保存配置、状态或计算结果。
        self.make_time = []
# 行 74：变量赋值：保存配置、状态或计算结果。
        self.make_sigma = []
# 行 75：变量赋值：保存配置、状态或计算结果。
        self.expand_time = []
# 行 76：变量赋值：保存配置、状态或计算结果。
        self.expand_v = []
# 行 77：变量赋值：保存配置、状态或计算结果。
        self.expand_i = []
# 行 78：变量赋值：保存配置、状态或计算结果。
        self.expand_r = []
# 行 79：变量赋值：保存配置、状态或计算结果。
        self.expand_R_pore = []
# 行 80：普通语句：执行一步操作或表达式。
        self.add_control_page()
# 行 81：普通语句：执行一步操作或表达式。
        self.control_page_original.place_forget()
# 行 82：普通语句：执行一步操作或表达式。
        self.add_collection_eONE_page()
# 行 83：普通语句：执行一步操作或表达式。
        self.collection_eONE_page_original.place_forget()
# 行 84：普通语句：执行一步操作或表达式。
        self.add_analyse_page()
# 行 85：普通语句：执行一步操作或表达式。
        self.analyse_page_original.place_forget()
# 行 86：原始注释行，提供作者或用途说明。
        # self.add_start_page()
# 行 87：普通语句：执行一步操作或表达式。
        self.add_control_page()
# 行 88：变量赋值：保存配置、状态或计算结果。
        self.menu = tk.Menu(self.root)
# 行 89：普通语句：执行一步操作或表达式。
        self.add_menu()
# 行 90：变量赋值：保存配置、状态或计算结果。
        self.channel = 'a'
# 行 91：普通语句：执行一步操作或表达式。
        '''数据采集(eONE)'''
# 行 92：变量赋值：保存配置、状态或计算结果。
        self.b_start_detection_param = False
# 行 93：变量赋值：保存配置、状态或计算结果。
        self.b_start_time_1_number = False
# 行 94：变量赋值：保存配置、状态或计算结果。
        self.b_detection_DNA_None = 0
# 行 95：变量赋值：保存配置、状态或计算结果。
        self.b_number = 0
# 行 96：变量赋值：保存配置、状态或计算结果。
        self.b_frequency = 1250
# 行 97：变量赋值：保存配置、状态或计算结果。
        self.b_start_time = None
# 行 98：变量赋值：保存配置、状态或计算结果。
        self.b_devices = []
# 行 99：变量赋值：保存配置、状态或计算结果。
        self.b_total_channels = None
# 行 100：变量赋值：保存配置、状态或计算结果。
        self.b_v_channels = None
# 行 101：变量赋值：保存配置、状态或计算结果。
        self.b_i_data = np.array([])
# 行 102：变量赋值：保存配置、状态或计算结果。
        self.b_v_data = np.array([])
# 行 103：变量赋值：保存配置、状态或计算结果。
        self.b_t_data = np.array([])
# 行 104：变量赋值：保存配置、状态或计算结果。
        self.b_std_data_down = np.array([])
# 行 105：变量赋值：保存配置、状态或计算结果。
        self.b_std_data_up = np.array([])
# 行 106：变量赋值：保存配置、状态或计算结果。
        self.b_baseline_data = np.array([])
# 行 107：变量赋值：保存配置、状态或计算结果。
        self.b_t_data_data_4 = np.array([])
# 行 108：变量赋值：保存配置、状态或计算结果。
        self.b_i_range = None
# 行 109：变量赋值：保存配置、状态或计算结果。
        self.b_v_range = None
# 行 110：变量赋值：保存配置、状态或计算结果。
        self.b_i_data_while = np.array([])
# 行 111：变量赋值：保存配置、状态或计算结果。
        self.b_v_data_while = np.array([])
# 行 112：变量赋值：保存配置、状态或计算结果。
        self.b_t_data_while = np.array([])
# 行 113：变量赋值：保存配置、状态或计算结果。
        self.b_i_data_cache = np.array([])
# 行 114：变量赋值：保存配置、状态或计算结果。
        self.b_t_data_cache = np.array([])
# 行 115：变量赋值：保存配置、状态或计算结果。
        self.b_cache_number = False
# 行 116：变量赋值：保存配置、状态或计算结果。
        self.b_std_data_while_down = np.array([])
# 行 117：变量赋值：保存配置、状态或计算结果。
        self.b_std_data_while_up = np.array([])
# 行 118：变量赋值：保存配置、状态或计算结果。
        self.b_baseline_data_while = np.array([])
# 行 119：变量赋值：保存配置、状态或计算结果。
        self.b_start_time = time.perf_counter()
# 行 120：变量赋值：保存配置、状态或计算结果。
        self.b_end_time = self.b_start_time
# 行 121：普通语句：执行一步操作或表达式。
        self.b_fig, (self.b_ax1) = plt.subplots(1, 1, figsize=(14.0, 5.5))
# 行 122：普通语句：执行一步操作或表达式。
        self.b_fig.patch.set_facecolor('#e5e5e5')
# 行 123：普通语句：执行一步操作或表达式。
        self.b_fig.subplots_adjust(left=0.05,  right=0.99, top=0.99, bottom=0.16)
# 行 124：普通语句：执行一步操作或表达式。
        self.b_ax1.set_xlabel( "时间(s)", fontsize=10, fontweight='bold', labelpad=10)
# 行 125：普通语句：执行一步操作或表达式。
        self.b_ax1.set_ylabel( "电流(nA)",fontsize=10, fontweight='bold', labelpad=10)
# 行 126：普通语句：执行一步操作或表达式。
        self.b_ax1.set_facecolor('#e5e5e5')
# 行 127：普通语句：执行一步操作或表达式。
        self.b_line1, = self.b_ax1.plot([], [], lw=0.5)
# 行 128：原始注释行，提供作者或用途说明。
        # self.b_line2, = self.b_ax2.plot([], [], lw=2)
# 行 129：变量赋值：保存配置、状态或计算结果。
        self.b_connect_stats = 0
# 行 130：变量赋值：保存配置、状态或计算结果。
        self.b_save_path = None
# 行 131：变量赋值：保存配置、状态或计算结果。
        self.b_filter = False
# 行 132：变量赋值：保存配置、状态或计算结果。
        self.b_filter_parameter = None
# 行 133：变量赋值：保存配置、状态或计算结果。
        self.b_source_current_range = False
# 行 134：变量赋值：保存配置、状态或计算结果。
        self.b_source_sampling_range = False
# 行 135：变量赋值：保存配置、状态或计算结果。
        self.b_source_bandwidth_range = False
# 行 136：变量赋值：保存配置、状态或计算结果。
        self.b_source_volt_range = False
# 行 137：变量赋值：保存配置、状态或计算结果。
        self.b_baseline_compensate_param = False
# 行 138：变量赋值：保存配置、状态或计算结果。
        self.b_detection_param = False
# 行 139：变量赋值：保存配置、状态或计算结果。
        self.b_i_channels = None
# 行 140：变量赋值：保存配置、状态或计算结果。
        self.b_default_selection_done_1 = False
# 行 141：变量赋值：保存配置、状态或计算结果。
        self.b_default_selection_done_2 = False
# 行 142：变量赋值：保存配置、状态或计算结果。
        self.b_default_selection_done_3 = False
# 行 143：变量赋值：保存配置、状态或计算结果。
        self.b_default_selection_done_4 = False
# 行 144：变量赋值：保存配置、状态或计算结果。
        self.b_last_current_range_selected = '200 pA'
# 行 145：变量赋值：保存配置、状态或计算结果。
        self.b_last_sampling_rate_selected = None
# 行 146：变量赋值：保存配置、状态或计算结果。
        self.b_last_filter_selected = None
# 行 147：变量赋值：保存配置、状态或计算结果。
        self.b_data_lock = threading.Lock()
# 行 148：变量赋值：保存配置、状态或计算结果。
        self.b_event_param = False
# 行 149：变量赋值：保存配置、状态或计算结果。
        self.b_event_list = None
# 行 150：变量赋值：保存配置、状态或计算结果。
        self.b_event_number = 0
# 行 151：变量赋值：保存配置、状态或计算结果。
        self.b_save_number = False
# 行 152：变量赋值：保存配置、状态或计算结果。
        self.b_event_list_append = False
# 行 153：变量赋值：保存配置、状态或计算结果。
        self.b_save_txt_head = False
# 行 154：变量赋值：保存配置、状态或计算结果。
        self.b_save_txt_number = False
# 行 155：变量赋值：保存配置、状态或计算结果。
        self.b_signal_number = 0
# 行 156：变量赋值：保存配置、状态或计算结果。
        self.b_event_list_all = []
# 行 157：变量赋值：保存配置、状态或计算结果。
        self.b_event_list_two = []
# 行 158：变量赋值：保存配置、状态或计算结果。
        self.b_event_amplitude = []
# 行 159：变量赋值：保存配置、状态或计算结果。
        self.b_event_duration = []
# 行 160：变量赋值：保存配置、状态或计算结果。
        self.b_event_amplitude_two = []
# 行 161：变量赋值：保存配置、状态或计算结果。
        self.b_event_duration_two = []
# 行 162：变量赋值：保存配置、状态或计算结果。
        self.b_signal_direction = 2
# 行 163：变量赋值：保存配置、状态或计算结果。
        self.b_save_txt_time_current = False
# 行 164：变量赋值：保存配置、状态或计算结果。
        self.b_csv_txt_thread = None
# 行 165：变量赋值：保存配置、状态或计算结果。
        self.b_event_list_1 = []
# 行 166：变量赋值：保存配置、状态或计算结果。
        self.b_event_list_2 = []
# 行 167：变量赋值：保存配置、状态或计算结果。
        self.b_event_list_3 = []
# 行 168：变量赋值：保存配置、状态或计算结果。
        self.b_event_list_4 = []
# 行 169：变量赋值：保存配置、状态或计算结果。
        self.b_event_list_5 = []
# 行 170：变量赋值：保存配置、状态或计算结果。
        self.b_event_list_6 = []
# 行 171：变量赋值：保存配置、状态或计算结果。
        self.b_event_list_7 = []
# 行 172：变量赋值：保存配置、状态或计算结果。
        self.threads = []
# 行 173：变量赋值：保存配置、状态或计算结果。
        self.b_frame_scroll = False
# 行 174：变量赋值：保存配置、状态或计算结果。
        self.b_scroll_param = False
# 行 175：变量赋值：保存配置、状态或计算结果。
        self.b_event_list_two_len = 0
# 行 176：变量赋值：保存配置、状态或计算结果。
        self.b_event_1_time = False
# 行 177：变量赋值：保存配置、状态或计算结果。
        self.b_event_two_number_index = []
# 行 178：变量赋值：保存配置、状态或计算结果。
        self.b_detection_DNA_sequency = 0
# 行 179：变量赋值：保存配置、状态或计算结果。
        self.b_detection_DNA_yes1 = 0
# 行 180：普通语句：执行一步操作或表达式。
        '''数据分析'''
# 行 181：变量赋值：保存配置、状态或计算结果。
        self.b_probe= '探针EC-K12'
# 行 182：变量赋值：保存配置、状态或计算结果。
        self.b_event_two_open = False
# 行 183：变量赋值：保存配置、状态或计算结果。
        self.b_event_two_number = 1
# 行 184：变量赋值：保存配置、状态或计算结果。
        self.b_event_two_open_csv = False
# 行 185：变量赋值：保存配置、状态或计算结果。
        self.c1_x = np.array([])
# 行 186：变量赋值：保存配置、状态或计算结果。
        self.c1_y = np.array([])
# 行 187：变量赋值：保存配置、状态或计算结果。
        self.c1_y = np.array([])
# 行 188：变量赋值：保存配置、状态或计算结果。
        self.c_std_down = np.array([])
# 行 189：变量赋值：保存配置、状态或计算结果。
        self.c_baseline = np.array([])
# 行 190：变量赋值：保存配置、状态或计算结果。
        self.c_std_up = np.array([])
# 行 191：变量赋值：保存配置、状态或计算结果。
        self.c1_y_filter = np.array([])
# 行 192：变量赋值：保存配置、状态或计算结果。
        self.c1_y_filter = np.array([])
# 行 193：变量赋值：保存配置、状态或计算结果。
        self.c_filter = False
# 行 194：变量赋值：保存配置、状态或计算结果。
        self.c_detection_three = 2
# 行 195：变量赋值：保存配置、状态或计算结果。
        self.c_detection_sigma = None
# 行 196：变量赋值：保存配置、状态或计算结果。
        self.c_detection_mintime = None
# 行 197：变量赋值：保存配置、状态或计算结果。
        self.c_detection_maxtime = None
# 行 198：变量赋值：保存配置、状态或计算结果。
        self.c_detection_baseline_std_window = None
# 行 199：变量赋值：保存配置、状态或计算结果。
        self.executor = ThreadPoolExecutor(max_workers=2)
# 行 200：变量赋值：保存配置、状态或计算结果。
        self.c_frequency = None
# 行 201：变量赋值：保存配置、状态或计算结果。
        self.c_detection_param = False
# 行 202：变量赋值：保存配置、状态或计算结果。
        self.c_event_list = defaultdict(list)
# 行 203：变量赋值：保存配置、状态或计算结果。
        self.c_event_list_two = []
# 行 204：变量赋值：保存配置、状态或计算结果。
        self.c_event_list_while = None
# 行 205：变量赋值：保存配置、状态或计算结果。
        self.c_detection_threshold = False
# 行 206：变量赋值：保存配置、状态或计算结果。
        self.c_event = False
# 行 207：变量赋值：保存配置、状态或计算结果。
        self.c_baseline_list = []
# 行 208：变量赋值：保存配置、状态或计算结果。
        self.c_amplitude_list = []
# 行 209：变量赋值：保存配置、状态或计算结果。
        self.c_integral_list = []
# 行 210：变量赋值：保存配置、状态或计算结果。
        self.c_duration_list = []
# 行 211：变量赋值：保存配置、状态或计算结果。
        self.c_bluntness_list = []
# 行 212：变量赋值：保存配置、状态或计算结果。
        self.c_asymmetry_list = []
# 行 213：变量赋值：保存配置、状态或计算结果。
        self.c_x_inertia_list = []
# 行 214：变量赋值：保存配置、状态或计算结果。
        self.c_y_inertia_list = []
# 行 215：变量赋值：保存配置、状态或计算结果。
        self.c_save_path = None
# 行 216：变量赋值：保存配置、状态或计算结果。
        self.c_file_path_list_preview = []
# 行 217：变量赋值：保存配置、状态或计算结果。
        self.c_file_path_list_statistics = []
# 行 218：变量赋值：保存配置、状态或计算结果。
        self.c_preview_file_name = None
# 行 219：变量赋值：保存配置、状态或计算结果。
        self.c_statistics_file_name = None
# 行 220：变量赋值：保存配置、状态或计算结果。
        self.c_events_list_baseline_2 = []
# 行 221：变量赋值：保存配置、状态或计算结果。
        self.c_events_list_amplitude = []
# 行 222：变量赋值：保存配置、状态或计算结果。
        self.c_events_list_integral = []
# 行 223：变量赋值：保存配置、状态或计算结果。
        self.c_events_list_duration = []
# 行 224：变量赋值：保存配置、状态或计算结果。
        self.c_events_list_bluntness = []
# 行 225：变量赋值：保存配置、状态或计算结果。
        self.c_events_list_asymmetry = []
# 行 226：变量赋值：保存配置、状态或计算结果。
        self.c_events_list_x_inertia = []
# 行 227：变量赋值：保存配置、状态或计算结果。
        self.c_events_list_y_inertia = []
# 行 228：变量赋值：保存配置、状态或计算结果。
        self.c_file_folder_choice = False
# 行 229：变量赋值：保存配置、状态或计算结果。
        self.c_file_abf_list = []
# 行 230：空行，用于分隔代码块，提高可读性。

# 行 231：定义函数/方法 on_closing(self)。
    def on_closing(self):
# 行 232：空行，用于分隔代码块，提高可读性。

# 行 233：普通语句：执行一步操作或表达式。
        c4.disconnect()
# 行 234：条件判断，根据不同条件执行不同分支。
        if hasattr(self, 'threads'):
# 行 235：for 循环遍历序列或可迭代对象。
            for t in self.threads:
# 行 236：条件判断，根据不同条件执行不同分支。
                if t.is_alive():
# 行 237：普通语句：执行一步操作或表达式。
                    t.join(timeout=2)
# 行 238：普通语句：执行一步操作或表达式。
        self.root.quit()
# 行 239：普通语句：执行一步操作或表达式。
        self.root.destroy()
# 行 240：空行，用于分隔代码块，提高可读性。

# 行 241：定义函数/方法 add_start_page(self)。
    def add_start_page(self):
# 行 242：条件判断，根据不同条件执行不同分支。
        if self.page_show_number == 0:
# 行 243：普通语句：执行一步操作或表达式。
            self.start_page_original.place(x=0, y=0, relwidth=1, relheight=1)
# 行 244：空行，用于分隔代码块，提高可读性。

# 行 245：定义函数/方法 background_image()。
            def background_image():
# 行 246：空行，用于分隔代码块，提高可读性。

# 行 247：变量赋值：保存配置、状态或计算结果。
                img = Image.open(r"background.jpg")
# 行 248：变量赋值：保存配置、状态或计算结果。
                img = img.resize((self.root.winfo_width(), self.root.winfo_height()), Image.Resampling.LANCZOS)
# 行 249：变量赋值：保存配置、状态或计算结果。
                imgtk = ImageTk.PhotoImage(image=img)
# 行 250：空行，用于分隔代码块，提高可读性。

# 行 251：变量赋值：保存配置、状态或计算结果。
                label = tk.Label(self.start_page_original, image=imgtk)
# 行 252：变量赋值：保存配置、状态或计算结果。
                label.image = imgtk  # 保存引用
# 行 253：普通语句：执行一步操作或表达式。
                label.place(x=0, y=0, relwidth=1, relheight=1)
# 行 254：空行，用于分隔代码块，提高可读性。

# 行 255：变量赋值：保存配置、状态或计算结果。
                self.imgtk = imgtk
# 行 256：空行，用于分隔代码块，提高可读性。

# 行 257：普通语句：执行一步操作或表达式。
            self.root.after(100, background_image)
# 行 258：条件判断，根据不同条件执行不同分支。
        else:
# 行 259：普通语句：执行一步操作或表达式。
            self.start_page_original.destroy()
# 行 260：空行，用于分隔代码块，提高可读性。

# 行 261：定义函数/方法 add_menu(self)。
    def add_menu(self):
# 行 262：变量赋值：保存配置、状态或计算结果。
        self.menu = tk.Menu(self.root)
# 行 263：普通语句：执行一步操作或表达式。
        self.root.config(menu=self.menu)
# 行 264：空行，用于分隔代码块，提高可读性。

# 行 265：普通语句：执行一步操作或表达式。
        self.menu.add_command(label="CBD打孔", command=lambda: self.function1("CBD打孔"))
# 行 266：普通语句：执行一步操作或表达式。
        self.menu.add_command(label="数据采集", command=lambda: self.function1("数据采集"))
# 行 267：普通语句：执行一步操作或表达式。
        self.menu.add_command(label="数据分析", command=lambda: self.function1("数据分析"))
# 行 268：定义函数/方法 on_select(self, number)。
    def on_select(self, number):
# 行 269：条件判断，根据不同条件执行不同分支。
        if number == 1:
# 行 270：变量赋值：保存配置、状态或计算结果。
            self.kei_device_onselect = self.combo1.get()
# 行 271：条件判断，根据不同条件执行不同分支。
        else:
# 行 272：变量赋值：保存配置、状态或计算结果。
            self.channel = self.combo2.get()
# 行 273：定义函数/方法 inquire_device(self)。
    def inquire_device(self):
# 行 274：普通语句：执行一步操作或表达式。
        self.combo_device_list.place_forget()
# 行 275：变量赋值：保存配置、状态或计算结果。
        self.rm = pyvisa.ResourceManager()
# 行 276：变量赋值：保存配置、状态或计算结果。
        self.kei_device_name = self.rm.list_resources()
# 行 277：普通语句：执行一步操作或表达式。
        self.combo_device_list['values'] = self.kei_device_name
# 行 278：变量赋值：保存配置、状态或计算结果。
        options1 = [self.kei_device_name]
# 行 279：空行，用于分隔代码块，提高可读性。

# 行 280：变量赋值：保存配置、状态或计算结果。
        self.combo1 = ttk.Combobox(self.control_page_original, values=options1, width=15, state='readonly')
# 行 281：普通语句：执行一步操作或表达式。
        self.combo1.current(0)
# 行 282：普通语句：执行一步操作或表达式。
        self.combo1.bind("<<ComboboxSelected>>", lambda event: self.on_select(1))
# 行 283：普通语句：执行一步操作或表达式。
        self.combo1.place(x=110, y=8)
# 行 284：返回函数结果。
        return self.kei_device_name
# 行 285：定义函数/方法 connect_device(self, path_entry, kei_device_name)。
    def connect_device(self, path_entry, kei_device_name):
# 行 286：变量赋值：保存配置、状态或计算结果。
        self.kei_device = self.rm.open_resource(self.rm.list_resources()[1])
# 行 287：变量赋值：保存配置、状态或计算结果。
        self.kei_device_model = self.kei_device.query("*IDN?")
# 行 288：普通语句：执行一步操作或表达式。
        self.update_color_button(1, 'green')
# 行 289：原始注释行，提供作者或用途说明。
        # self.kei_device.write('beeper.enable = beeper.ON')
# 行 290：原始注释行，提供作者或用途说明。
        # self.kei_device.write('beeper.beep(0.5, 2400) ')
# 行 291：原始注释行，提供作者或用途说明。
        # time.sleep(1)
# 行 292：原始注释行，提供作者或用途说明。
        # self.kei_device.write('beeper.beep(0.5, 2400) ')
# 行 293：原始注释行，提供作者或用途说明。
        # time.sleep(1)
# 行 294：原始注释行，提供作者或用途说明。
        # self.kei_device.write('beeper.beep(0.5, 2400) ')
# 行 295：普通语句：执行一步操作或表达式。
        self.update_event_print(path_entry, self.kei_device_model)
# 行 296：向源表/仪器发送 SCPI 指令以设置或查询参数。
        self.kei_device.write(f'smu{self.channel}.reset()')
# 行 297：向源表/仪器发送 SCPI 指令以设置或查询参数。
        self.kei_device.write(f'smu{self.channel}.source.output = smu{self.channel}.OUTPUT_OFF')
# 行 298：向源表/仪器发送 SCPI 指令以设置或查询参数。
        self.kei_device.write(f'smu{self.channel}.source.func = smu{self.channel}.OUTPUT_DCVOLTS')
# 行 299：向源表/仪器发送 SCPI 指令以设置或查询参数。
        self.kei_device.write(f'smu{self.channel}.source.autorangev = smu{self.channel}.AUTORANGE_ON')
# 行 300：向源表/仪器发送 SCPI 指令以设置或查询参数。
        self.kei_device.write(f'smu{self.channel}.source.levelv = 0')
# 行 301：向源表/仪器发送 SCPI 指令以设置或查询参数。
        self.kei_device.write(f'smu{self.channel}.source.limiti = {self.limit_current_param.get()}')
# 行 302：定义函数/方法 creat_color_button(self, number, control_page, x, y)。
    def creat_color_button(self, number, control_page, x, y):
# 行 303：条件判断，根据不同条件执行不同分支。
        if not hasattr(self, 'canvas_dict'):
# 行 304：变量赋值：保存配置、状态或计算结果。
            self.canvas_dict = {}
# 行 305：变量赋值：保存配置、状态或计算结果。
        self.canvas = tk.Canvas(control_page, width=25, height=25, bg='#e5e5e5', bd=0,
# 行 306：变量赋值：保存配置、状态或计算结果。
                                highlightthickness=0)
# 行 307：普通语句：执行一步操作或表达式。
        self.canvas.place(x=x, y=y)
# 行 308：普通语句：执行一步操作或表达式。
        self.canvas_dict[number] = self.canvas
# 行 309：普通语句：执行一步操作或表达式。
        self.canvas.create_oval(5, 5, 20, 20, fill='red', outline='black')
# 行 310：普通语句：执行一步操作或表达式。
        self.canvas.config(state='disabled')
# 行 311：空行，用于分隔代码块，提高可读性。

# 行 312：定义函数/方法 update_color_button(self, number, color)。
    def update_color_button(self, number, color):
# 行 313：变量赋值：保存配置、状态或计算结果。
        self.canvas = self.canvas_dict.get(number)
# 行 314：条件判断，根据不同条件执行不同分支。
        if self.canvas:
# 行 315：普通语句：执行一步操作或表达式。
            self.canvas.itemconfig(1, fill=color, outline='black')
# 行 316：空行，用于分隔代码块，提高可读性。

# 行 317：定义函数/方法 update_event_print(self, path_entry, new_print)。
    def update_event_print(self, path_entry, new_print):
# 行 318：普通语句：执行一步操作或表达式。
        path_entry.config(state=tk.NORMAL)
# 行 319：普通语句：执行一步操作或表达式。
        path_entry.delete(0, tk.END)
# 行 320：普通语句：执行一步操作或表达式。
        path_entry.insert(0, new_print)
# 行 321：普通语句：执行一步操作或表达式。
        path_entry.config(state=tk.DISABLED)
# 行 322：空行，用于分隔代码块，提高可读性。

# 行 323：定义函数/方法 create_toggle_switch(self, master, x, y)。
    def create_toggle_switch(self, master, x, y):
# 行 324：普通语句：执行一步操作或表达式。
        width, height = 70, 30
# 行 325：变量赋值：保存配置、状态或计算结果。
        active_color = "green"
# 行 326：变量赋值：保存配置、状态或计算结果。
        inactive_color = "lightgray"
# 行 327：变量赋值：保存配置、状态或计算结果。
        circle_color = "white"
# 行 328：变量赋值：保存配置、状态或计算结果。
        circle_radius = 20
# 行 329：变量赋值：保存配置、状态或计算结果。
        switch = tk.Canvas(master, width=width, height=height)
# 行 330：普通语句：执行一步操作或表达式。
        switch.place(x=x, y=y)
# 行 331：空行，用于分隔代码块，提高可读性。

# 行 332：变量赋值：保存配置、状态或计算结果。
        self.make_way = False
# 行 333：变量赋值：保存配置、状态或计算结果。
        background = switch.create_rectangle(5, 5, width - 5, height - 5, fill=inactive_color, outline="gray", width=2)
# 行 334：变量赋值：保存配置、状态或计算结果。
        circle = switch.create_oval(5, 5, circle_radius + 5, height - 5, fill=circle_color, outline=inactive_color)
# 行 335：定义函数/方法 toggle(event=None)。
        def toggle(event=None):
# 行 336：条件判断，根据不同条件执行不同分支。
            if self.make_way:
# 行 337：变量赋值：保存配置、状态或计算结果。
                self.make_way = False
# 行 338：打印调试或状态信息到控制台。
                print('guan')
# 行 339：普通语句：执行一步操作或表达式。
                switch.coords(circle, 5, 5, circle_radius + 5, height - 5)
# 行 340：普通语句：执行一步操作或表达式。
                switch.itemconfig(background, fill=inactive_color)
# 行 341：普通语句：执行一步操作或表达式。
                self.canvas_manual_check.tag_bind('shape', '<Button-1>',
# 行 342：普通语句：执行一步操作或表达式。
                                                   lambda event: self.toggle_shape('check', self.canvas_manual_check))
# 行 343：普通语句：执行一步操作或表达式。
                self.canvas_manual_make.tag_bind('shape', '<Button-1>',
# 行 344：普通语句：执行一步操作或表达式。
                                                  lambda event: self.toggle_shape('make', self.canvas_manual_make))
# 行 345：普通语句：执行一步操作或表达式。
                self.canvas_manual_expand.tag_bind('shape', '<Button-1>',
# 行 346：普通语句：执行一步操作或表达式。
                                                   lambda event: self.toggle_shape('expand', self.canvas_manual_expand))
# 行 347：普通语句：执行一步操作或表达式。
                self.canvas_manual_check.itemconfig('shape', fill='green')
# 行 348：普通语句：执行一步操作或表达式。
                self.canvas_manual_make.itemconfig('shape', fill='green')
# 行 349：普通语句：执行一步操作或表达式。
                self.canvas_manual_expand.itemconfig('shape', fill='green')
# 行 350：普通语句：执行一步操作或表达式。
                self.start_make_pore.config(state=tk.DISABLED, bg='lightgray')
# 行 351：条件判断，根据不同条件执行不同分支。
            else:
# 行 352：变量赋值：保存配置、状态或计算结果。
                self.make_way = True
# 行 353：打印调试或状态信息到控制台。
                print('kai')
# 行 354：普通语句：执行一步操作或表达式。
                switch.coords(circle, width - circle_radius - 5, 5, width - 5, height - 5)
# 行 355：普通语句：执行一步操作或表达式。
                switch.itemconfig(background, fill=active_color)
# 行 356：普通语句：执行一步操作或表达式。
                self.canvas_manual_check.tag_bind('shape', '<Button-1>', lambda event: None)
# 行 357：普通语句：执行一步操作或表达式。
                self.canvas_manual_make.tag_bind('shape', '<Button-1>', lambda event: None)
# 行 358：普通语句：执行一步操作或表达式。
                self.canvas_manual_expand.tag_bind('shape', '<Button-1>', lambda event: None)
# 行 359：普通语句：执行一步操作或表达式。
                self.canvas_manual_check.itemconfig('shape', fill='gray')
# 行 360：普通语句：执行一步操作或表达式。
                self.canvas_manual_make.itemconfig('shape', fill='gray')
# 行 361：普通语句：执行一步操作或表达式。
                self.canvas_manual_expand.itemconfig('shape', fill='gray')
# 行 362：普通语句：执行一步操作或表达式。
                self.start_make_pore.config(state=tk.NORMAL, bg='#e5e5e5')
# 行 363：空行，用于分隔代码块，提高可读性。

# 行 364：普通语句：执行一步操作或表达式。
        switch.bind("<Button-1>", toggle)
# 行 365：空行，用于分隔代码块，提高可读性。

# 行 366：返回函数结果。
        return switch
# 行 367：空行，用于分隔代码块，提高可读性。

# 行 368：定义函数/方法 draw_triangle(self, process, canvas)。
    def draw_triangle(self, process, canvas):
# 行 369：空行，用于分隔代码块，提高可读性。

# 行 370：普通语句：执行一步操作或表达式。
        canvas.delete("shape")
# 行 371：条件判断，根据不同条件执行不同分支。
        if process == 'check':
# 行 372：条件判断，根据不同条件执行不同分支。
            if self.manual_check == False:
# 行 373：普通语句：执行一步操作或表达式。
                canvas.create_polygon(0, 1, 31, 16, 0, 31, fill='green', outline='black', tags='shape')
# 行 374：原始注释行，提供作者或用途说明。
                # canvas.tag_bind('shape', '<Button-1>', lambda event: None)
# 行 375：空行，用于分隔代码块，提高可读性。

# 行 376：条件判断，根据不同条件执行不同分支。
            else:
# 行 377：普通语句：执行一步操作或表达式。
                canvas.create_rectangle(0, 0, 31, 31, fill='black', outline='black', tags='shape')
# 行 378：条件判断，根据不同条件执行不同分支。
        elif process == 'make':
# 行 379：条件判断，根据不同条件执行不同分支。
            if self.manual_make == False:
# 行 380：普通语句：执行一步操作或表达式。
                canvas.create_polygon(0, 1, 31, 16, 0, 31, fill='green', outline='black', tags='shape')
# 行 381：空行，用于分隔代码块，提高可读性。

# 行 382：条件判断，根据不同条件执行不同分支。
            else:
# 行 383：普通语句：执行一步操作或表达式。
                canvas.create_rectangle(0, 0, 31, 31, fill='black', outline='black', tags='shape')
# 行 384：条件判断，根据不同条件执行不同分支。
        else:
# 行 385：条件判断，根据不同条件执行不同分支。
            if self.manual_expand == False:
# 行 386：普通语句：执行一步操作或表达式。
                canvas.create_polygon(0, 1, 31, 16, 0, 31, fill='green', outline='black', tags='shape')
# 行 387：空行，用于分隔代码块，提高可读性。

# 行 388：条件判断，根据不同条件执行不同分支。
            else:
# 行 389：普通语句：执行一步操作或表达式。
                canvas.create_rectangle(0, 0, 31, 31, fill='black', outline='black', tags='shape')
# 行 390：空行，用于分隔代码块，提高可读性。

# 行 391：空行，用于分隔代码块，提高可读性。

# 行 392：定义函数/方法 toggle_shape(self, process, canvas_ts)。
    def toggle_shape(self, process, canvas_ts):
# 行 393：条件判断，根据不同条件执行不同分支。
        if process == 'check':
# 行 394：变量赋值：保存配置、状态或计算结果。
            self.manual_check = not self.manual_check
# 行 395：普通语句：执行一步操作或表达式。
            self.draw_triangle('check', canvas_ts)
# 行 396：条件判断，根据不同条件执行不同分支。
            if self.manual_check == True:
# 行 397：普通语句：执行一步操作或表达式。
                self.check_volt_param.config(state=tk.DISABLED)
# 行 398：向源表/仪器发送 SCPI 指令以设置或查询参数。
                self.kei_device.write(f'smu{self.channel}.reset()')
# 行 399：向源表/仪器发送 SCPI 指令以设置或查询参数。
                self.kei_device.write(f'smu{self.channel}.source.output = smu{self.channel}.OUTPUT_ON')
# 行 400：向源表/仪器发送 SCPI 指令以设置或查询参数。
                self.kei_device.write(f'smu{self.channel}.source.limiti = {self.limit_current_param.get()}')
# 行 401：变量赋值：保存配置、状态或计算结果。
                self.check_volt1 = -1 * float(self.check_volt_param.get())
# 行 402：变量赋值：保存配置、状态或计算结果。
                self.check_volt2 = float(self.check_volt_param.get())
# 行 403：变量赋值：保存配置、状态或计算结果。
                step = 0.01
# 行 404：变量赋值：保存配置、状态或计算结果。
                check_time = 0
# 行 405：变量赋值：保存配置、状态或计算结果。
                voltage = self.check_volt1
# 行 406：普通语句：执行一步操作或表达式。
                self.check_i.clear()
# 行 407：普通语句：执行一步操作或表达式。
                self.check_v.clear()
# 行 408：普通语句：执行一步操作或表达式。
                self.check_r.clear()
# 行 409：普通语句：执行一步操作或表达式。
                self.check_time.clear()
# 行 410：普通语句：执行一步操作或表达式。
                self.ax_check.clear()
# 行 411：空行，用于分隔代码块，提高可读性。

# 行 412：定义函数/方法 update_data()。
                def update_data():
# 行 413：普通语句：执行一步操作或表达式。
                    nonlocal voltage, check_time
# 行 414：条件判断，根据不同条件执行不同分支。
                    if self.manual_check == True:
# 行 415：条件判断，根据不同条件执行不同分支。
                        if voltage <= self.check_volt2:
# 行 416：向源表/仪器发送 SCPI 指令以设置或查询参数。
                            self.kei_device.write(f'smu{self.channel}.source.levelv = {voltage}')
# 行 417：变量赋值：保存配置、状态或计算结果。
                            data_i = float(self.kei_device.query(
# 行 418：普通语句：执行一步操作或表达式。
                                f'print(smu{self.channel}.measure.i(smu{self.channel}.nvbuffer1))'))
# 行 419：普通语句：执行一步操作或表达式。
                            self.check_i.append(round(data_i, 12))
# 行 420：普通语句：执行一步操作或表达式。
                            self.check_v.append(round(voltage, 4))
# 行 421：普通语句：执行一步操作或表达式。
                            self.check_r.append(abs(voltage / data_i))
# 行 422：普通语句：执行一步操作或表达式。
                            self.check_time.append(check_time)
# 行 423：普通语句：执行一步操作或表达式。
                            voltage += step
# 行 424：普通语句：执行一步操作或表达式。
                            check_time += 0.5
# 行 425：普通语句：执行一步操作或表达式。
                            self.plot_check_update()
# 行 426：普通语句：执行一步操作或表达式。
                            self.root.after(500, update_data)
# 行 427：条件判断，根据不同条件执行不同分支。
                        else:
# 行 428：变量赋值：保存配置、状态或计算结果。
                            self.check_mean_r = np.mean(self.check_r)
# 行 429：普通语句：执行一步操作或表达式。
                            self.check_r_entry.config(state=tk.NORMAL)  # 允许修改文本框
# 行 430：普通语句：执行一步操作或表达式。
                            self.check_r_entry.delete(0, tk.END)
# 行 431：普通语句：执行一步操作或表达式。
                            self.check_r_entry.insert(0, self.check_mean_r)
# 行 432：普通语句：执行一步操作或表达式。
                            self.check_r_entry.config(state=tk.DISABLED)  # 禁止编辑
# 行 433：向源表/仪器发送 SCPI 指令以设置或查询参数。
                            self.kei_device.write(f'smu{self.channel}.source.output = smu{self.channel}.OUTPUT_OFF')
# 行 434：向源表/仪器发送 SCPI 指令以设置或查询参数。
                            self.kei_device.write(f'smu{self.channel}.reset()')
# 行 435：变量赋值：保存配置、状态或计算结果。
                            self.manual_check = not self.manual_check
# 行 436：普通语句：执行一步操作或表达式。
                            self.draw_triangle('check', canvas_ts)
# 行 437：普通语句：执行一步操作或表达式。
                            self.check_volt_param.config(state=tk.NORMAL)
# 行 438：条件判断，根据不同条件执行不同分支。
                    else:
# 行 439：向源表/仪器发送 SCPI 指令以设置或查询参数。
                        self.kei_device.write(f'smu{self.channel}.reset()')
# 行 440：向源表/仪器发送 SCPI 指令以设置或查询参数。
                        self.kei_device.write(f'smu{self.channel}.source.output = smu{self.channel}.OUTPUT_OFF')
# 行 441：占位语句：暂不执行任何操作。
                        pass
# 行 442：普通语句：执行一步操作或表达式。
                update_data()
# 行 443：条件判断，根据不同条件执行不同分支。
            else:
# 行 444：普通语句：执行一步操作或表达式。
                self.check_volt_param.config(state=tk.NORMAL)
# 行 445：条件判断，根据不同条件执行不同分支。
        elif process == 'make':
# 行 446：变量赋值：保存配置、状态或计算结果。
            self.manual_make = not self.manual_make
# 行 447：普通语句：执行一步操作或表达式。
            self.draw_triangle('make', canvas_ts)
# 行 448：条件判断，根据不同条件执行不同分支。
            if self.manual_make == True:
# 行 449：向源表/仪器发送 SCPI 指令以设置或查询参数。
                self.kei_device.write(f'smu{self.channel}.reset()')
# 行 450：向源表/仪器发送 SCPI 指令以设置或查询参数。
                self.kei_device.write(f'smu{self.channel}.source.output = smu{self.channel}.OUTPUT_ON')
# 行 451：向源表/仪器发送 SCPI 指令以设置或查询参数。
                self.kei_device.write(f'smu{self.channel}.source.limiti = {self.limit_current_param.get()}')
# 行 452：变量赋值：保存配置、状态或计算结果。
                time_start = 0
# 行 453：变量赋值：保存配置、状态或计算结果。
                time_sleep = 0.5
# 行 454：普通语句：执行一步操作或表达式。
                self.make_v.clear()
# 行 455：普通语句：执行一步操作或表达式。
                self.make_i.clear()
# 行 456：普通语句：执行一步操作或表达式。
                self.make_r.clear()
# 行 457：普通语句：执行一步操作或表达式。
                self.make_time.clear()
# 行 458：普通语句：执行一步操作或表达式。
                self.make_sigma.clear()
# 行 459：变量赋值：保存配置、状态或计算结果。
                voltage = float(self.make_volt_param.get())
# 行 460：向源表/仪器发送 SCPI 指令以设置或查询参数。
                self.kei_device.write(f'smu{self.channel}.source.levelv = {voltage}')
# 行 461：普通语句：执行一步操作或表达式。
                time.sleep(3)
# 行 462：普通语句：执行一步操作或表达式。
                self.axs_make[0].cla()
# 行 463：普通语句：执行一步操作或表达式。
                self.axs_make[1].cla()
# 行 464：普通语句：执行一步操作或表达式。
                self.axs_make[2].cla()
# 行 465：普通语句：执行一步操作或表达式。
                self.axs_make[3].cla()
# 行 466：空行，用于分隔代码块，提高可读性。

# 行 467：定义函数/方法 update_make_data()。
                def update_make_data():
# 行 468：普通语句：执行一步操作或表达式。
                    nonlocal time_start, time_sleep
# 行 469：条件判断，根据不同条件执行不同分支。
                    if self.manual_make == True:
# 行 470：变量赋值：保存配置、状态或计算结果。
                        data = float(self.kei_device.query(f'print(smu{self.channel}.measure.i())'))
# 行 471：普通语句：执行一步操作或表达式。
                        self.make_volt_param.config(state=tk.DISABLED)
# 行 472：普通语句：执行一步操作或表达式。
                        self.make_solution_param.config(state=tk.DISABLED)
# 行 473：普通语句：执行一步操作或表达式。
                        self.make_SiN_param.config(state=tk.DISABLED)
# 行 474：普通语句：执行一步操作或表达式。
                        self.make_i.append(round(data, 12))
# 行 475：普通语句：执行一步操作或表达式。
                        self.make_v.append(round(voltage, 4))
# 行 476：普通语句：执行一步操作或表达式。
                        self.make_r.append(voltage / data)
# 行 477：普通语句：执行一步操作或表达式。
                        self.make_time.append(time_start)
# 行 478：条件判断，根据不同条件执行不同分支。
                        if len(self.make_v) < 10:
# 行 479：变量赋值：保存配置、状态或计算结果。
                            sigma = (self.make_i[-1] - np.mean(self.make_i[:])) / np.std(self.make_i[:])
# 行 480：普通语句：执行一步操作或表达式。
                            self.make_sigma.append(sigma)
# 行 481：条件判断，根据不同条件执行不同分支。
                        else:
# 行 482：变量赋值：保存配置、状态或计算结果。
                            sigma = (self.make_i[-1] - np.mean(self.make_i[-10:-1])) / np.std(self.make_i[-10:-1])
# 行 483：普通语句：执行一步操作或表达式。
                            self.make_sigma.append(sigma)
# 行 484：空行，用于分隔代码块，提高可读性。

# 行 485：普通语句：执行一步操作或表达式。
                        time_start += time_sleep
# 行 486：普通语句：执行一步操作或表达式。
                        self.plot_VIR_sigma_update()
# 行 487：条件判断，根据不同条件执行不同分支。
                        if len(self.make_v) <= 300:
# 行 488：条件判断，根据不同条件执行不同分支。
                            if sigma >= 100 and sigma > 0:
# 行 489：原始注释行，提供作者或用途说明。
                                # self.kei_device.write(f'smua.source.levelv = 0')
# 行 490：原始注释行，提供作者或用途说明。
                                # print('打孔完成')
# 行 491：原始注释行，提供作者或用途说明。
                                # df = pd.DataFrame({
# 行 492：原始注释行，提供作者或用途说明。
                                #     'time': self.make_time,
# 行 493：原始注释行，提供作者或用途说明。
                                #     'voltage': self.make_v,
# 行 494：原始注释行，提供作者或用途说明。
                                #     'current': self.make_i,
# 行 495：原始注释行，提供作者或用途说明。
                                #     'resistor': self.make_r,
# 行 496：原始注释行，提供作者或用途说明。
                                #     'sigma': self.make_sigma,
# 行 497：原始注释行，提供作者或用途说明。
                                # })
# 行 498：原始注释行，提供作者或用途说明。
                                # df.to_csv(f'{self.a_save_path}/pore_make.csv', index=False)
# 行 499：原始注释行，提供作者或用途说明。
                                # self.kei_device.write(f'smua.source.levelv = 0')
# 行 500：原始注释行，提供作者或用途说明。
                                # R_pore = self.calculate_d(self.make_r[-1], 11.27, 0.00000003)
# 行 501：原始注释行，提供作者或用途说明。
                                # self.expand_pore(R_pore, 50)
# 行 502：向源表/仪器发送 SCPI 指令以设置或查询参数。
                                self.kei_device.write(f'smu{self.channel}.reset()')
# 行 503：向源表/仪器发送 SCPI 指令以设置或查询参数。
                                self.kei_device.write(f'smu{self.channel}.source.output = smu{self.channel}.OUTPUT_OFF')
# 行 504：占位语句：暂不执行任何操作。
                                pass
# 行 505：条件判断，根据不同条件执行不同分支。
                            else:
# 行 506：普通语句：执行一步操作或表达式。
                                self.root.after(50, update_make_data)
# 行 507：条件判断，根据不同条件执行不同分支。
                        else:
# 行 508：变量赋值：保存配置、状态或计算结果。
                            self.make_v = self.make_v[-30:]
# 行 509：打印调试或状态信息到控制台。
                            print(self.make_v)
# 行 510：变量赋值：保存配置、状态或计算结果。
                            self.make_i = self.make_i[-30:]
# 行 511：变量赋值：保存配置、状态或计算结果。
                            self.make_time = self.make_time[-30:]
# 行 512：变量赋值：保存配置、状态或计算结果。
                            self.make_r = self.make_r[-30:]
# 行 513：变量赋值：保存配置、状态或计算结果。
                            self.make_sigma = self.make_sigma[-30:]
# 行 514：普通语句：执行一步操作或表达式。
                            self.axs_make[0].cla()
# 行 515：普通语句：执行一步操作或表达式。
                            self.axs_make[1].cla()
# 行 516：普通语句：执行一步操作或表达式。
                            self.axs_make[2].cla()
# 行 517：普通语句：执行一步操作或表达式。
                            self.axs_make[3].cla()
# 行 518：普通语句：执行一步操作或表达式。
                            self.root.after(50, update_make_data)
# 行 519：条件判断，根据不同条件执行不同分支。
                    else:
# 行 520：向源表/仪器发送 SCPI 指令以设置或查询参数。
                        self.kei_device.write(f'smu{self.channel}.reset()')
# 行 521：向源表/仪器发送 SCPI 指令以设置或查询参数。
                        self.kei_device.write(f'smu{self.channel}.source.output = smu{self.channel}.OUTPUT_OFF')
# 行 522：占位语句：暂不执行任何操作。
                        pass
# 行 523：普通语句：执行一步操作或表达式。
                update_make_data()
# 行 524：条件判断，根据不同条件执行不同分支。
            else:
# 行 525：普通语句：执行一步操作或表达式。
                self.make_volt_param.config(state=tk.NORMAL)
# 行 526：普通语句：执行一步操作或表达式。
                self.make_solution_param.config(state=tk.NORMAL)
# 行 527：普通语句：执行一步操作或表达式。
                self.make_SiN_param.config(state=tk.NORMAL)
# 行 528：条件判断，根据不同条件执行不同分支。
        else:
# 行 529：变量赋值：保存配置、状态或计算结果。
            self.manual_expand = not self.manual_expand
# 行 530：普通语句：执行一步操作或表达式。
            self.draw_triangle('expand', canvas_ts)
# 行 531：条件判断，根据不同条件执行不同分支。
            if self.manual_expand == True:
# 行 532：向源表/仪器发送 SCPI 指令以设置或查询参数。
                self.kei_device.write(f'smu{self.channel}.reset()')
# 行 533：向源表/仪器发送 SCPI 指令以设置或查询参数。
                self.kei_device.write(f'smu{self.channel}.source.output = smu{self.channel}.OUTPUT_ON')
# 行 534：向源表/仪器发送 SCPI 指令以设置或查询参数。
                self.kei_device.write(f'smu{self.channel}.source.limiti = {self.limit_current_param.get()}')
# 行 535：普通语句：执行一步操作或表达式。
                self.expand_volt_param.config(state=tk.DISABLED)
# 行 536：普通语句：执行一步操作或表达式。
                self.pore_target_param.config(state=tk.DISABLED)
# 行 537：向源表/仪器发送 SCPI 指令以设置或查询参数。
                self.kei_device.write(f'smua.source.levelv = 0')
# 行 538：空行，用于分隔代码块，提高可读性。

# 行 539：变量赋值：保存配置、状态或计算结果。
                voltage = float(self.expand_volt_param.get())
# 行 540：变量赋值：保存配置、状态或计算结果。
                expand_time = 0
# 行 541：变量赋值：保存配置、状态或计算结果。
                solution_sigma = float(self.make_solution_param.get())
# 行 542：变量赋值：保存配置、状态或计算结果。
                pore_h = float(self.make_SiN_param.get())
# 行 543：变量赋值：保存配置、状态或计算结果。
                pore_d_target = float(self.pore_target_param.get())
# 行 544：变量赋值：保存配置、状态或计算结果。
                number = 0
# 行 545：普通语句：执行一步操作或表达式。
                self.axs_expand[0].cla()
# 行 546：普通语句：执行一步操作或表达式。
                self.axs_expand[1].cla()
# 行 547：普通语句：执行一步操作或表达式。
                self.expand_i.clear()
# 行 548：普通语句：执行一步操作或表达式。
                self.expand_v.clear()
# 行 549：普通语句：执行一步操作或表达式。
                self.expand_r.clear()
# 行 550：普通语句：执行一步操作或表达式。
                self.expand_time.clear()
# 行 551：普通语句：执行一步操作或表达式。
                self.expand_R_pore.clear()
# 行 552：定义函数/方法 update_data_expand()。
                def update_data_expand():
# 行 553：条件判断，根据不同条件执行不同分支。
                    if self.manual_expand == True:
# 行 554：普通语句：执行一步操作或表达式。
                        nonlocal voltage, expand_time, number
# 行 555：条件判断，根据不同条件执行不同分支。
                        if number == 0:
# 行 556：变量赋值：保存配置、状态或计算结果。
                            R_pore = self.calculate_d(1 / 10000, solution_sigma, pore_h)
# 行 557：变量赋值：保存配置、状态或计算结果。
                            number = 1
# 行 558：向源表/仪器发送 SCPI 指令以设置或查询参数。
                            self.kei_device.write(f'smu{self.channel}.source.levelv = {voltage}')
# 行 559：变量赋值：保存配置、状态或计算结果。
                            data_i = float(self.kei_device.query(f'print(smu{self.channel}.measure.i())'))
# 行 560：普通语句：执行一步操作或表达式。
                            self.expand_i.append(round(data_i, 12))
# 行 561：普通语句：执行一步操作或表达式。
                            self.expand_v.append(round(voltage, 4))
# 行 562：普通语句：执行一步操作或表达式。
                            self.expand_r.append(abs(voltage / data_i))
# 行 563：普通语句：执行一步操作或表达式。
                            self.expand_time.append(expand_time)
# 行 564：普通语句：执行一步操作或表达式。
                            self.expand_R_pore.append(R_pore)
# 行 565：变量赋值：保存配置、状态或计算结果。
                            voltage = -voltage
# 行 566：普通语句：执行一步操作或表达式。
                            expand_time += 0.5
# 行 567：普通语句：执行一步操作或表达式。
                            self.root.after(500, update_data_expand)
# 行 568：条件判断，根据不同条件执行不同分支。
                        else:
# 行 569：条件判断，根据不同条件执行不同分支。
                            if len(self.expand_v) <= 200:
# 行 570：打印调试或状态信息到控制台。
                                print(len(self.expand_v))
# 行 571：变量赋值：保存配置、状态或计算结果。
                                R_pore = self.calculate_d(1 / self.expand_r[-1], solution_sigma, pore_h)
# 行 572：条件判断，根据不同条件执行不同分支。
                                if R_pore < pore_d_target:
# 行 573：向源表/仪器发送 SCPI 指令以设置或查询参数。
                                    self.kei_device.write(f'smu{self.channel}.source.levelv = {voltage}')
# 行 574：变量赋值：保存配置、状态或计算结果。
                                    data_i = float(self.kei_device.query(f'print(smu{self.channel}.measure.i())'))
# 行 575：普通语句：执行一步操作或表达式。
                                    self.expand_i.append(round(data_i, 12))
# 行 576：普通语句：执行一步操作或表达式。
                                    self.expand_v.append(round(voltage, 4))
# 行 577：普通语句：执行一步操作或表达式。
                                    self.expand_r.append(abs(voltage / data_i))
# 行 578：普通语句：执行一步操作或表达式。
                                    self.expand_time.append(expand_time)
# 行 579：普通语句：执行一步操作或表达式。
                                    self.expand_R_pore.append(R_pore)
# 行 580：变量赋值：保存配置、状态或计算结果。
                                    voltage = -voltage
# 行 581：普通语句：执行一步操作或表达式。
                                    expand_time += 0.5
# 行 582：空行，用于分隔代码块，提高可读性。

# 行 583：普通语句：执行一步操作或表达式。
                                    self.plot_expand_update()
# 行 584：空行，用于分隔代码块，提高可读性。

# 行 585：普通语句：执行一步操作或表达式。
                                    self.root.after(500, update_data_expand)
# 行 586：条件判断，根据不同条件执行不同分支。
                                else:
# 行 587：向源表/仪器发送 SCPI 指令以设置或查询参数。
                                    self.kei_device.write(f'smu{self.channel}.source.levelv = 0')
# 行 588：原始注释行，提供作者或用途说明。
                                    # df = pd.DataFrame({
# 行 589：原始注释行，提供作者或用途说明。
                                    #     'time': self.expand_time,
# 行 590：原始注释行，提供作者或用途说明。
                                    #     'voltage': self.expand_v,
# 行 591：原始注释行，提供作者或用途说明。
                                    #     'current': self.expand_i,
# 行 592：原始注释行，提供作者或用途说明。
                                    #     'resistor': self.expand_r,
# 行 593：原始注释行，提供作者或用途说明。
                                    #     'R_pore': self.expand_R_pore,
# 行 594：原始注释行，提供作者或用途说明。
                                    # })
# 行 595：原始注释行，提供作者或用途说明。
                                    # df.to_csv(f'{self.a_save_path}/pore_expand.csv', index=False)
# 行 596：向源表/仪器发送 SCPI 指令以设置或查询参数。
                                    self.kei_device.write(f'smu{self.channel}.source.levelv = 0')
# 行 597：打印调试或状态信息到控制台。
                                    print('扩孔完成，已达到目标孔径')
# 行 598：条件判断，根据不同条件执行不同分支。
                            else:
# 行 599：变量赋值：保存配置、状态或计算结果。
                                self.expand_v = self.expand_v[-30:]
# 行 600：变量赋值：保存配置、状态或计算结果。
                                self.expand_i = self.expand_i[-30:]
# 行 601：变量赋值：保存配置、状态或计算结果。
                                self.expand_time = self.expand_time[-30:]
# 行 602：变量赋值：保存配置、状态或计算结果。
                                self.expand_r = self.expand_r[-30:]
# 行 603：变量赋值：保存配置、状态或计算结果。
                                self.expand_R_pore = self.expand_R_pore[-30:]
# 行 604：普通语句：执行一步操作或表达式。
                                self.axs_expand[0].cla()
# 行 605：普通语句：执行一步操作或表达式。
                                self.axs_expand[1].cla()
# 行 606：普通语句：执行一步操作或表达式。
                                self.root.after(500, update_data_expand)
# 行 607：条件判断，根据不同条件执行不同分支。
                    else:
# 行 608：向源表/仪器发送 SCPI 指令以设置或查询参数。
                        self.kei_device.write(f'smu{self.channel}.reset()')
# 行 609：向源表/仪器发送 SCPI 指令以设置或查询参数。
                        self.kei_device.write(f'smu{self.channel}.source.output = smu{self.channel}.OUTPUT_OFF')
# 行 610：占位语句：暂不执行任何操作。
                        pass
# 行 611：空行，用于分隔代码块，提高可读性。

# 行 612：普通语句：执行一步操作或表达式。
                update_data_expand()
# 行 613：条件判断，根据不同条件执行不同分支。
            else:
# 行 614：普通语句：执行一步操作或表达式。
                self.expand_volt_param.config(state=tk.NORMAL)
# 行 615：普通语句：执行一步操作或表达式。
                self.pore_target_param.config(state=tk.NORMAL)
# 行 616：定义函数/方法 create_dashed_square(self,canvas, x1, y1, x2, y2, side_length)。
    def create_dashed_square(self,canvas, x1, y1, x2, y2, side_length):
# 行 617：原始注释行，提供作者或用途说明。
        # 虚线
# 行 618：普通语句：执行一步操作或表达式。
        canvas.create_rectangle(
# 行 619：普通语句：执行一步操作或表达式。
            x1, y1, x2, y2,
# 行 620：变量赋值：保存配置、状态或计算结果。
            outline="black", width=2  # dash=(4, 4) 表示虚线的间隔
# 行 621：普通语句：执行一步操作或表达式。
        )
# 行 622：原始注释行，提供作者或用途说明。
        # canvas.create_line(
# 行 623：原始注释行，提供作者或用途说明。
        #     x1, y1 + 200, x1 + side_length, y1 + 200,
# 行 624：原始注释行，提供作者或用途说明。
        #     dash=(4, 4),
# 行 625：原始注释行，提供作者或用途说明。
        #     fill="black",
# 行 626：原始注释行，提供作者或用途说明。
        #     width=2
# 行 627：原始注释行，提供作者或用途说明。
        # )
# 行 628：定义函数/方法 create_dashed_line(self,canvas, x1, y1, x2, y2, side_length)。
    def create_dashed_line(self,canvas, x1, y1, x2, y2, side_length):
# 行 629：空行，用于分隔代码块，提高可读性。

# 行 630：普通语句：执行一步操作或表达式。
        canvas.create_line(
# 行 631：普通语句：执行一步操作或表达式。
            x1, y1, x2, y2,
# 行 632：变量赋值：保存配置、状态或计算结果。
            dash=(4, 4),
# 行 633：变量赋值：保存配置、状态或计算结果。
            fill="black",
# 行 634：变量赋值：保存配置、状态或计算结果。
            width=side_length
# 行 635：普通语句：执行一步操作或表达式。
        )
# 行 636：定义函数/方法 a_open_new_window(self)。
    def a_open_new_window(self):
# 行 637：变量赋值：保存配置、状态或计算结果。
        new_window = tk.Toplevel(self.root)
# 行 638：普通语句：执行一步操作或表达式。
        new_window.title("选择路径")
# 行 639：普通语句：执行一步操作或表达式。
        new_window.geometry("330x120")
# 行 640：普通语句：执行一步操作或表达式。
        new_window.grab_set()
# 行 641：空行，用于分隔代码块，提高可读性。

# 行 642：变量赋值：保存配置、状态或计算结果。
        path_entry = create_entry(new_window, 30, 40, 30)
# 行 643：空行，用于分隔代码块，提高可读性。

# 行 644：变量赋值：保存配置、状态或计算结果。
        browse_button = tk.Button(new_window, text="浏览", bg='#e5e5e5', font=('', 9),
# 行 645：变量赋值：保存配置、状态或计算结果。
                                  command=lambda: browse_file(path_entry))
# 行 646：普通语句：执行一步操作或表达式。
        browse_button.place(x=287, y=30)
# 行 647：空行，用于分隔代码块，提高可读性。

# 行 648：变量赋值：保存配置、状态或计算结果。
        confirm_button = tk.Button(new_window, text="确定", bg='#e5e5e5', font=('', 9),
# 行 649：变量赋值：保存配置、状态或计算结果。
                                   command=lambda: self.a_on_confirm(new_window, path_entry.get()))
# 行 650：普通语句：执行一步操作或表达式。
        confirm_button.place(x=140, y=80)
# 行 651：空行，用于分隔代码块，提高可读性。

# 行 652：定义函数/方法 a_on_confirm(self, window, folder_path)。
    def a_on_confirm(self, window, folder_path):
# 行 653：条件判断，根据不同条件执行不同分支。
        if folder_path:
# 行 654：变量赋值：保存配置、状态或计算结果。
            self.a_save_path = folder_path
# 行 655：空行，用于分隔代码块，提高可读性。

# 行 656：普通语句：执行一步操作或表达式。
            self.save_path_entry.config(state=tk.NORMAL)  # 允许修改
# 行 657：普通语句：执行一步操作或表达式。
            self.save_path_entry.delete(0, tk.END)
# 行 658：普通语句：执行一步操作或表达式。
            self.save_path_entry.insert(0, folder_path)
# 行 659：普通语句：执行一步操作或表达式。
            self.save_path_entry.config(state=tk.DISABLED)  # 禁止编辑
# 行 660：普通语句：执行一步操作或表达式。
            window.destroy()
# 行 661：空行，用于分隔代码块，提高可读性。

# 行 662：定义函数/方法 b_open_new_window(self)。
    def b_open_new_window(self):
# 行 663：变量赋值：保存配置、状态或计算结果。
        new_window = tk.Toplevel(self.root)
# 行 664：普通语句：执行一步操作或表达式。
        new_window.title("选择路径")
# 行 665：普通语句：执行一步操作或表达式。
        new_window.geometry("330x120")
# 行 666：普通语句：执行一步操作或表达式。
        new_window.grab_set()
# 行 667：空行，用于分隔代码块，提高可读性。

# 行 668：变量赋值：保存配置、状态或计算结果。
        path_entry = create_entry(new_window, 30, 40, 30)
# 行 669：空行，用于分隔代码块，提高可读性。

# 行 670：变量赋值：保存配置、状态或计算结果。
        browse_button = tk.Button(new_window, text="浏览", bg='#e5e5e5', font=('', 9),
# 行 671：变量赋值：保存配置、状态或计算结果。
                                  command=lambda: browse_file(path_entry))
# 行 672：普通语句：执行一步操作或表达式。
        browse_button.place(x=287, y=30)
# 行 673：空行，用于分隔代码块，提高可读性。

# 行 674：变量赋值：保存配置、状态或计算结果。
        confirm_button = tk.Button(new_window, text="确定", bg='#e5e5e5', font=('', 9),
# 行 675：变量赋值：保存配置、状态或计算结果。
                                   command=lambda: self.b_on_confirm(new_window, path_entry.get()))
# 行 676：普通语句：执行一步操作或表达式。
        confirm_button.place(x=140, y=80)
# 行 677：空行，用于分隔代码块，提高可读性。

# 行 678：定义函数/方法 b_on_confirm(self, window, folder_path)。
    def b_on_confirm(self, window, folder_path):
# 行 679：条件判断，根据不同条件执行不同分支。
        if folder_path:
# 行 680：变量赋值：保存配置、状态或计算结果。
            self.b_save_path = folder_path
# 行 681：打印调试或状态信息到控制台。
            print(self.b_save_path)
# 行 682：原始注释行，提供作者或用途说明。
            # self.save_path_entry.config(state=tk.NORMAL)  # 允许修改文本框
# 行 683：原始注释行，提供作者或用途说明。
            # self.save_path_entry.delete(0, tk.END)
# 行 684：原始注释行，提供作者或用途说明。
            # self.save_path_entry.insert(0, folder_path)
# 行 685：原始注释行，提供作者或用途说明。
            # self.save_path_entry.config(state=tk.DISABLED)  # 禁止编辑
# 行 686：普通语句：执行一步操作或表达式。
            window.destroy()
# 行 687：定义函数/方法 plot_VIR_sigma(self)。
    def plot_VIR_sigma(self):
# 行 688：普通语句：执行一步操作或表达式。
        self.fig_make, self.axs_make = plt.subplots(4, 1, figsize=(10, 7), dpi=100)
# 行 689：普通语句：执行一步操作或表达式。
        self.fig_make.patch.set_facecolor('#e5e5e5')
# 行 690：原始注释行，提供作者或用途说明。
        # axs[0].plot(x, v, color='white')
# 行 691：普通语句：执行一步操作或表达式。
        self.axs_make[0].set_title("CBD")
# 行 692：普通语句：执行一步操作或表达式。
        self.axs_make[0].set_ylabel("Voltage(V)")
# 行 693：普通语句：执行一步操作或表达式。
        self.axs_make[0].grid(False)
# 行 694：普通语句：执行一步操作或表达式。
        self.axs_make[0].get_xaxis().set_visible(False)
# 行 695：普通语句：执行一步操作或表达式。
        self.axs_make[0].set_facecolor('black')
# 行 696：空行，用于分隔代码块，提高可读性。

# 行 697：原始注释行，提供作者或用途说明。
        # axs[1].plot(x, i, color='white')
# 行 698：普通语句：执行一步操作或表达式。
        self.axs_make[1].set_ylabel("Current(nA)")
# 行 699：普通语句：执行一步操作或表达式。
        self.axs_make[1].grid(False)
# 行 700：普通语句：执行一步操作或表达式。
        self.axs_make[1].get_xaxis().set_visible(False)
# 行 701：普通语句：执行一步操作或表达式。
        self.axs_make[1].set_facecolor('black')
# 行 702：空行，用于分隔代码块，提高可读性。

# 行 703：原始注释行，提供作者或用途说明。
        # axs[2].plot(x, r, color='white')
# 行 704：普通语句：执行一步操作或表达式。
        self.axs_make[2].set_ylabel("Resistance(MΩ)")
# 行 705：普通语句：执行一步操作或表达式。
        self.axs_make[2].grid(False)
# 行 706：普通语句：执行一步操作或表达式。
        self.axs_make[2].get_xaxis().set_visible(False)
# 行 707：普通语句：执行一步操作或表达式。
        self.axs_make[2].set_facecolor('black')
# 行 708：空行，用于分隔代码块，提高可读性。

# 行 709：原始注释行，提供作者或用途说明。
        # axs[3].plot(x, sigma, color='white')
# 行 710：普通语句：执行一步操作或表达式。
        self.axs_make[3].set_xlabel("Time(s)")
# 行 711：普通语句：执行一步操作或表达式。
        self.axs_make[3].set_ylabel("Sigma(I)")
# 行 712：普通语句：执行一步操作或表达式。
        self.axs_make[3].grid(False)
# 行 713：普通语句：执行一步操作或表达式。
        self.axs_make[3].set_facecolor('black')
# 行 714：普通语句：执行一步操作或表达式。
        self.fig_make.tight_layout()
# 行 715：变量赋值：保存配置、状态或计算结果。
        self.canvas_plotVIR_sigma = FigureCanvasTkAgg(self.fig_make, master=self.control_page_original)
# 行 716：变量赋值：保存配置、状态或计算结果。
        self.canvas_widget_VIR_sigma = self.canvas_plotVIR_sigma.get_tk_widget()
# 行 717：普通语句：执行一步操作或表达式。
        self.canvas_widget_VIR_sigma.place(x=0, y=292)
# 行 718：空行，用于分隔代码块，提高可读性。

# 行 719：定义函数/方法 plot_VIR_sigma_update(self)。
    def plot_VIR_sigma_update(self):
# 行 720：条件判断，根据不同条件执行不同分支。
        if len(self.make_v) > 0:
# 行 721：普通语句：执行一步操作或表达式。
            self.axs_make[0].set_title("CBD")
# 行 722：普通语句：执行一步操作或表达式。
            self.axs_make[0].set_ylabel("Voltage(V)")
# 行 723：普通语句：执行一步操作或表达式。
            self.axs_make[1].set_ylabel("Current(A)")
# 行 724：普通语句：执行一步操作或表达式。
            self.axs_make[2].set_ylabel("Resistance(Ω)")
# 行 725：普通语句：执行一步操作或表达式。
            self.axs_make[3].set_xlabel("Time(s)")
# 行 726：普通语句：执行一步操作或表达式。
            self.axs_make[3].set_ylabel("Sigma(I)")
# 行 727：普通语句：执行一步操作或表达式。
            self.axs_make[0].plot(self.make_time, self.make_v, color='white',linewidth=0.3)
# 行 728：普通语句：执行一步操作或表达式。
            self.axs_make[1].plot(self.make_time, self.make_i, color='white',linewidth=0.3)
# 行 729：普通语句：执行一步操作或表达式。
            self.axs_make[2].plot(self.make_time, self.make_r, color='white',linewidth=0.3)
# 行 730：普通语句：执行一步操作或表达式。
            self.axs_make[3].plot(self.make_time, self.make_sigma, color='white',linewidth=0.3)
# 行 731：普通语句：执行一步操作或表达式。
            self.axs_make[0].grid(False)
# 行 732：普通语句：执行一步操作或表达式。
            self.axs_make[1].grid(False)
# 行 733：普通语句：执行一步操作或表达式。
            self.axs_make[2].grid(False)
# 行 734：普通语句：执行一步操作或表达式。
            self.axs_make[3].grid(False)
# 行 735：普通语句：执行一步操作或表达式。
            self.axs_make[0].set_facecolor('black')
# 行 736：普通语句：执行一步操作或表达式。
            self.axs_make[1].set_facecolor('black')
# 行 737：普通语句：执行一步操作或表达式。
            self.axs_make[2].set_facecolor('black')
# 行 738：普通语句：执行一步操作或表达式。
            self.axs_make[3].set_facecolor('black')
# 行 739：普通语句：执行一步操作或表达式。
            self.fig_make.tight_layout()
# 行 740：普通语句：执行一步操作或表达式。
            self.canvas_plotVIR_sigma.draw()
# 行 741：空行，用于分隔代码块，提高可读性。

# 行 742：定义函数/方法 plot_check(self)。
    def plot_check(self):
# 行 743：变量赋值：保存配置、状态或计算结果。
        self.fig_check = Figure(figsize=(9, 3), dpi=100)
# 行 744：普通语句：执行一步操作或表达式。
        self.fig_check.patch.set_facecolor('#e5e5e5')
# 行 745：变量赋值：保存配置、状态或计算结果。
        self.ax_check = self.fig_check.add_subplot(111)
# 行 746：普通语句：执行一步操作或表达式。
        self.ax_check.set_title("芯片检查")
# 行 747：普通语句：执行一步操作或表达式。
        self.ax_check.set_xlabel("Time(s)")
# 行 748：普通语句：执行一步操作或表达式。
        self.ax_check.set_ylabel("Current(A)")
# 行 749：普通语句：执行一步操作或表达式。
        self.ax_check.grid(True)
# 行 750：原始注释行，提供作者或用途说明。
        # ax_check.plot(x, i, color='blue')
# 行 751：普通语句：执行一步操作或表达式。
        self.ax_check.set_facecolor('black')
# 行 752：普通语句：执行一步操作或表达式。
        self.fig_check.tight_layout()
# 行 753：变量赋值：保存配置、状态或计算结果。
        self.canvas_current_check = FigureCanvasTkAgg(self.fig_check, master=self.control_page_original)
# 行 754：变量赋值：保存配置、状态或计算结果。
        self.canvas_widget_check = self.canvas_current_check.get_tk_widget()
# 行 755：普通语句：执行一步操作或表达式。
        self.canvas_widget_check.place(x=1000, y=300)
# 行 756：定义函数/方法 plot_check_update(self)。
    def plot_check_update(self):
# 行 757：条件判断，根据不同条件执行不同分支。
        if len(self.check_v) > 0:
# 行 758：普通语句：执行一步操作或表达式。
            self.ax_check.plot(self.check_v, self.check_i, color='blue')
# 行 759：普通语句：执行一步操作或表达式。
            self.ax_check.set_title("芯片检查")
# 行 760：普通语句：执行一步操作或表达式。
            self.ax_check.set_xlabel("Time(s)")
# 行 761：普通语句：执行一步操作或表达式。
            self.ax_check.set_ylabel("Current(A)")
# 行 762：普通语句：执行一步操作或表达式。
            self.ax_check.grid(True)
# 行 763：普通语句：执行一步操作或表达式。
            self.ax_check.set_facecolor('black')
# 行 764：普通语句：执行一步操作或表达式。
            self.fig_check.tight_layout()
# 行 765：普通语句：执行一步操作或表达式。
            self.canvas_current_check.draw()
# 行 766：空行，用于分隔代码块，提高可读性。

# 行 767：空行，用于分隔代码块，提高可读性。

# 行 768：定义函数/方法 plot_expand_pore(self)。
    def plot_expand_pore(self):
# 行 769：普通语句：执行一步操作或表达式。
        self.fig_expand, self.axs_expand = plt.subplots(2, 1, figsize=(9, 3.7), dpi=100)
# 行 770：普通语句：执行一步操作或表达式。
        self.fig_expand.patch.set_facecolor('#e5e5e5')
# 行 771：原始注释行，提供作者或用途说明。
        # axs[0].plot(x, i, color='red')
# 行 772：普通语句：执行一步操作或表达式。
        self.axs_expand[0].set_title("扩孔")
# 行 773：普通语句：执行一步操作或表达式。
        self.axs_expand[0].set_ylabel("Voltage(V)")
# 行 774：普通语句：执行一步操作或表达式。
        self.axs_expand[0].grid(True)
# 行 775：普通语句：执行一步操作或表达式。
        self.axs_expand[0].set_facecolor('black')
# 行 776：空行，用于分隔代码块，提高可读性。

# 行 777：原始注释行，提供作者或用途说明。
        # axs[1].plot(x, r, color='red')
# 行 778：普通语句：执行一步操作或表达式。
        self.axs_expand[1].set_ylabel("D_pore(nm)")
# 行 779：普通语句：执行一步操作或表达式。
        self.axs_expand[1].grid(True)
# 行 780：普通语句：执行一步操作或表达式。
        self.axs_expand[1].set_facecolor('black')
# 行 781：空行，用于分隔代码块，提高可读性。

# 行 782：普通语句：执行一步操作或表达式。
        self.fig_expand.tight_layout()
# 行 783：变量赋值：保存配置、状态或计算结果。
        self.canvas_plot_expand_pore = FigureCanvasTkAgg(self.fig_expand, master=self.control_page_original)
# 行 784：变量赋值：保存配置、状态或计算结果。
        self.canvas_widget_expand_pore = self.canvas_plot_expand_pore.get_tk_widget()
# 行 785：普通语句：执行一步操作或表达式。
        self.canvas_widget_expand_pore.place(x=1000, y=600)
# 行 786：空行，用于分隔代码块，提高可读性。

# 行 787：定义函数/方法 plot_expand_update(self)。
    def plot_expand_update(self):
# 行 788：条件判断，根据不同条件执行不同分支。
        if len(self.expand_i) > 0:
# 行 789：普通语句：执行一步操作或表达式。
            self.axs_expand[0].plot(self.expand_time[1:], self.expand_v[1:], color='red')
# 行 790：普通语句：执行一步操作或表达式。
            self.axs_expand[1].plot(self.expand_time[1:], self.expand_R_pore[1:], color='red')
# 行 791：普通语句：执行一步操作或表达式。
            self.ax_check.set_title("扩孔")
# 行 792：普通语句：执行一步操作或表达式。
            self.fig_expand.tight_layout()
# 行 793：普通语句：执行一步操作或表达式。
            self.canvas_plot_expand_pore.draw()
# 行 794：空行，用于分隔代码块，提高可读性。

# 行 795：定义函数/方法 start_make_pore_process(self)。
    def start_make_pore_process(self):
# 行 796：普通语句：执行一步操作或表达式。
        self.update_color_button(2, 'yellow')
# 行 797：向源表/仪器发送 SCPI 指令以设置或查询参数。
        self.kei_device.write(f'smu{self.channel}.reset()')
# 行 798：向源表/仪器发送 SCPI 指令以设置或查询参数。
        self.kei_device.write(f'smu{self.channel}.source.output = smu{self.channel}.OUTPUT_OFF')
# 行 799：向源表/仪器发送 SCPI 指令以设置或查询参数。
        self.kei_device.write(f'smu{self.channel}.source.func = smu{self.channel}.OUTPUT_DCVOLTS')
# 行 800：向源表/仪器发送 SCPI 指令以设置或查询参数。
        self.kei_device.write(f'smu{self.channel}.source.autorangev = smu{self.channel}.AUTORANGE_ON')
# 行 801：向源表/仪器发送 SCPI 指令以设置或查询参数。
        self.kei_device.write(f'smu{self.channel}.source.levelv = 0')
# 行 802：向源表/仪器发送 SCPI 指令以设置或查询参数。
        self.kei_device.write(f'smu{self.channel}.source.limiti = {self.limit_current_param.get()}')
# 行 803：向源表/仪器发送 SCPI 指令以设置或查询参数。
        self.kei_device.write(f'smu{self.channel}.source.output = smu{self.channel}.OUTPUT_ON')
# 行 804：变量赋值：保存配置、状态或计算结果。
        self.check_volt1 = -1 * float(self.check_volt_param.get())
# 行 805：变量赋值：保存配置、状态或计算结果。
        self.check_volt2 = float(self.check_volt_param.get())
# 行 806：变量赋值：保存配置、状态或计算结果。
        step = 0.01
# 行 807：变量赋值：保存配置、状态或计算结果。
        check_time = 0
# 行 808：变量赋值：保存配置、状态或计算结果。
        voltage = self.check_volt1
# 行 809：普通语句：执行一步操作或表达式。
        self.check_i.clear()
# 行 810：普通语句：执行一步操作或表达式。
        self.check_v.clear()
# 行 811：普通语句：执行一步操作或表达式。
        self.check_r.clear()
# 行 812：普通语句：执行一步操作或表达式。
        self.check_time.clear()
# 行 813：普通语句：执行一步操作或表达式。
        self.ax_check.clear()
# 行 814：空行，用于分隔代码块，提高可读性。

# 行 815：定义函数/方法 update_data()。
        def update_data():
# 行 816：普通语句：执行一步操作或表达式。
            nonlocal voltage, check_time
# 行 817：条件判断，根据不同条件执行不同分支。
            if voltage <= self.check_volt2:
# 行 818：向源表/仪器发送 SCPI 指令以设置或查询参数。
                self.kei_device.write(f'smu{self.channel}.source.levelv = {voltage}')
# 行 819：变量赋值：保存配置、状态或计算结果。
                data_i = float(self.kei_device.query(f'print(smu{self.channel}.measure.i(smu{self.channel}.nvbuffer1))'))
# 行 820：普通语句：执行一步操作或表达式。
                self.check_i.append(round(data_i, 12))
# 行 821：普通语句：执行一步操作或表达式。
                self.check_v.append(round(voltage, 4))
# 行 822：普通语句：执行一步操作或表达式。
                self.check_r.append(abs(voltage / data_i))
# 行 823：普通语句：执行一步操作或表达式。
                self.check_time.append(check_time)
# 行 824：普通语句：执行一步操作或表达式。
                voltage += step
# 行 825：普通语句：执行一步操作或表达式。
                check_time += 0.5
# 行 826：普通语句：执行一步操作或表达式。
                self.plot_check_update()
# 行 827：普通语句：执行一步操作或表达式。
                self.root.after(500, update_data)
# 行 828：条件判断，根据不同条件执行不同分支。
            else:
# 行 829：变量赋值：保存配置、状态或计算结果。
                self.check_mean_r = np.mean(self.check_r)
# 行 830：普通语句：执行一步操作或表达式。
                self.check_r_entry.config(state=tk.NORMAL)  # 允许修改文本框
# 行 831：普通语句：执行一步操作或表达式。
                self.check_r_entry.delete(0, tk.END)
# 行 832：普通语句：执行一步操作或表达式。
                self.check_r_entry.insert(0, self.check_mean_r)
# 行 833：普通语句：执行一步操作或表达式。
                self.check_r_entry.config(state=tk.DISABLED)  # 禁止编辑
# 行 834：条件判断，根据不同条件执行不同分支。
                if 1e8 < np.mean(self.check_r) < 1e11:
# 行 835：变量赋值：保存配置、状态或计算结果。
                    self.update_completed_check = True
# 行 836：变量赋值：保存配置、状态或计算结果。
                    df = pd.DataFrame({
# 行 837：普通语句：执行一步操作或表达式。
                        'time': self.check_time,
# 行 838：普通语句：执行一步操作或表达式。
                        'voltage': self.check_v,
# 行 839：普通语句：执行一步操作或表达式。
                        'current': self.check_i,
# 行 840：普通语句：执行一步操作或表达式。
                        'resistor': self.check_r,
# 行 841：普通语句：执行一步操作或表达式。
                    })
# 行 842：普通语句：执行一步操作或表达式。
                    df.to_csv(f'{self.a_save_path}/pore_check.csv', index=False)
# 行 843：向源表/仪器发送 SCPI 指令以设置或查询参数。
                    self.kei_device.write(f'smu{self.channel}.reset()')
# 行 844：向源表/仪器发送 SCPI 指令以设置或查询参数。
                    self.kei_device.write(f'smu{self.channel}.source.output = smu{self.channel}.OUTPUT_OFF')
# 行 845：普通语句：执行一步操作或表达式。
                    self.check_update_finished()
# 行 846：原始注释行，提供作者或用途说明。
                # elif np.mean(self.check_r) < 1e5:
# 行 847：原始注释行，提供作者或用途说明。
                #     self.update_color_button(2, 'red')
# 行 848：原始注释行，提供作者或用途说明。
                #     print(np.mean(self.check_r))
# 行 849：原始注释行，提供作者或用途说明。
                #     print('流通池漏液')
# 行 850：原始注释行，提供作者或用途说明。
                # else:
# 行 851：原始注释行，提供作者或用途说明。
                #     self.update_color_button(2, 'red')
# 行 852：原始注释行，提供作者或用途说明。
                #     print(np.mean(self.check_r))
# 行 853：原始注释行，提供作者或用途说明。
                #     print('芯片表面有气泡')
# 行 854：空行，用于分隔代码块，提高可读性。

# 行 855：普通语句：执行一步操作或表达式。
        update_data()
# 行 856：空行，用于分隔代码块，提高可读性。

# 行 857：定义函数/方法 check_update_finished(self)。
    def check_update_finished(self):
# 行 858：条件判断，根据不同条件执行不同分支。
        if self.update_completed_check == True:
# 行 859：条件判断，根据不同条件执行不同分支。
            if self.check_i[-1] <= 0.01:
# 行 860：变量赋值：保存配置、状态或计算结果。
                time_start = 0
# 行 861：变量赋值：保存配置、状态或计算结果。
                time_sleep = 0.5
# 行 862：普通语句：执行一步操作或表达式。
                self.update_color_button(2, 'green')
# 行 863：普通语句：执行一步操作或表达式。
                self.update_color_button(3, 'yellow')
# 行 864：普通语句：执行一步操作或表达式。
                self.make_v.clear()
# 行 865：普通语句：执行一步操作或表达式。
                self.make_i.clear()
# 行 866：普通语句：执行一步操作或表达式。
                self.make_r.clear()
# 行 867：普通语句：执行一步操作或表达式。
                self.make_time.clear()
# 行 868：变量赋值：保存配置、状态或计算结果。
                voltage = float(self.make_volt_param.get())
# 行 869：变量赋值：保存配置、状态或计算结果。
                limit_i = self.limit_current_param.get()
# 行 870：向源表/仪器发送 SCPI 指令以设置或查询参数。
                self.kei_device.write(f'smu{self.channel}.source.levelv = {voltage}')
# 行 871：向源表/仪器发送 SCPI 指令以设置或查询参数。
                self.kei_device.write(f'smu{self.channel}.source.limiti = {limit_i}')
# 行 872：普通语句：执行一步操作或表达式。
                time.sleep(3)
# 行 873：定义函数/方法 update_make_data()。
                def update_make_data():
# 行 874：普通语句：执行一步操作或表达式。
                    nonlocal time_start, time_sleep
# 行 875：变量赋值：保存配置、状态或计算结果。
                    data = float(self.kei_device.query(f'print(smu{self.channel}.measure.i())'))
# 行 876：普通语句：执行一步操作或表达式。
                    self.make_i.append(round(data, 12))
# 行 877：普通语句：执行一步操作或表达式。
                    self.make_v.append(round(voltage, 4))
# 行 878：普通语句：执行一步操作或表达式。
                    self.make_r.append(voltage / data)
# 行 879：普通语句：执行一步操作或表达式。
                    self.make_time.append(time_start)
# 行 880：条件判断，根据不同条件执行不同分支。
                    if len(self.make_v)< 10:
# 行 881：变量赋值：保存配置、状态或计算结果。
                        sigma = (self.make_i[-1] - np.mean(self.make_i[:])) / np.std(self.make_i[:])
# 行 882：普通语句：执行一步操作或表达式。
                        self.make_sigma.append(sigma)
# 行 883：条件判断，根据不同条件执行不同分支。
                    else:
# 行 884：变量赋值：保存配置、状态或计算结果。
                        sigma = (self.make_i[-1] - np.mean(self.make_i[-10:-1])) / np.std(self.make_i[-10:-1])
# 行 885：普通语句：执行一步操作或表达式。
                        self.make_sigma.append(sigma)
# 行 886：空行，用于分隔代码块，提高可读性。

# 行 887：普通语句：执行一步操作或表达式。
                    time_start += time_sleep
# 行 888：普通语句：执行一步操作或表达式。
                    self.plot_VIR_sigma_update()
# 行 889：打印调试或状态信息到控制台。
                    print(self.make_i[-1], self.make_r[-1], self.make_sigma[-1])
# 行 890：条件判断，根据不同条件执行不同分支。
                    if self.make_i[-1] < limit_i:
# 行 891：条件判断，根据不同条件执行不同分支。
                        if len(self.make_v) <= 300:
# 行 892：条件判断，根据不同条件执行不同分支。
                            if sigma >= 100 and sigma > 0:
# 行 893：向源表/仪器发送 SCPI 指令以设置或查询参数。
                                self.kei_device.write(f'smu{self.channel}.source.levelv = 0')
# 行 894：打印调试或状态信息到控制台。
                                print('打孔完成')
# 行 895：变量赋值：保存配置、状态或计算结果。
                                df = pd.DataFrame({
# 行 896：普通语句：执行一步操作或表达式。
                                    'time': self.make_time,
# 行 897：普通语句：执行一步操作或表达式。
                                    'voltage': self.make_v,
# 行 898：普通语句：执行一步操作或表达式。
                                    'current': self.make_i,
# 行 899：普通语句：执行一步操作或表达式。
                                    'resistor': self.make_r,
# 行 900：普通语句：执行一步操作或表达式。
                                    'sigma': self.make_sigma,
# 行 901：普通语句：执行一步操作或表达式。
                                })
# 行 902：普通语句：执行一步操作或表达式。
                                df.to_csv(f'{self.a_save_path}/pore_make.csv', index=False)
# 行 903：向源表/仪器发送 SCPI 指令以设置或查询参数。
                                self.kei_device.write(f'smu{self.channel}.source.levelv = 0')
# 行 904：变量赋值：保存配置、状态或计算结果。
                                R_pore = self.calculate_d(self.make_r[-1], 11.27, 0.00000003)
# 行 905：普通语句：执行一步操作或表达式。
                                self.expand_pore(R_pore, self.pore_target_param.get())
# 行 906：条件判断，根据不同条件执行不同分支。
                            else:
# 行 907：普通语句：执行一步操作或表达式。
                                self.root.after(500, update_make_data)
# 行 908：条件判断，根据不同条件执行不同分支。
                        else:
# 行 909：变量赋值：保存配置、状态或计算结果。
                            self.make_v = self.make_v[-30:]
# 行 910：打印调试或状态信息到控制台。
                            print(self.make_v)
# 行 911：变量赋值：保存配置、状态或计算结果。
                            self.make_i = self.make_i[-30:]
# 行 912：变量赋值：保存配置、状态或计算结果。
                            self.make_time = self.make_time[-30:]
# 行 913：变量赋值：保存配置、状态或计算结果。
                            self.make_r = self.make_r[-30:]
# 行 914：变量赋值：保存配置、状态或计算结果。
                            self.make_sigma = self.make_sigma[-30:]
# 行 915：普通语句：执行一步操作或表达式。
                            self.axs_make[0].cla()
# 行 916：普通语句：执行一步操作或表达式。
                            self.axs_make[1].cla()
# 行 917：普通语句：执行一步操作或表达式。
                            self.axs_make[2].cla()
# 行 918：普通语句：执行一步操作或表达式。
                            self.axs_make[3].cla()
# 行 919：普通语句：执行一步操作或表达式。
                            self.root.after(500, update_make_data)
# 行 920：条件判断，根据不同条件执行不同分支。
                    else:
# 行 921：普通语句：执行一步操作或表达式。
                        self.update_color_button(3, 'green')
# 行 922：打印调试或状态信息到控制台。
                        print("达到电流限值")
# 行 923：空行，用于分隔代码块，提高可读性。

# 行 924：普通语句：执行一步操作或表达式。
                update_make_data()
# 行 925：条件判断，根据不同条件执行不同分支。
            else:
# 行 926：变量赋值：保存配置、状态或计算结果。
                self.update_completed_check = False
# 行 927：普通语句：执行一步操作或表达式。
                self.update_color_button(2, 'red')
# 行 928：空行，用于分隔代码块，提高可读性。

# 行 929：定义函数/方法 calculate_d(self, pore_sigma, solution_sigma, pore_h)。
    def calculate_d(self, pore_sigma, solution_sigma, pore_h):
# 行 930：返回函数结果。
        return (pore_sigma / (2 * solution_sigma)) * (1 + math.sqrt(1 + (16 * solution_sigma * pore_h) / (math.pi * pore_sigma)))
# 行 931：空行，用于分隔代码块，提高可读性。

# 行 932：定义函数/方法 expand_pore(self, R_pore_first, R_pore_last)。
    def expand_pore(self, R_pore_first, R_pore_last):
# 行 933：向源表/仪器发送 SCPI 指令以设置或查询参数。
        self.kei_device.write(f'smu{self.channel}.source.levelv = 0')
# 行 934：变量赋值：保存配置、状态或计算结果。
        voltage = float(self.expand_volt_param.get())
# 行 935：变量赋值：保存配置、状态或计算结果。
        expand_time = 0
# 行 936：变量赋值：保存配置、状态或计算结果。
        solution_sigma = float(self.make_solution_param.get())
# 行 937：变量赋值：保存配置、状态或计算结果。
        pore_h = float(self.make_SiN_param.get())
# 行 938：变量赋值：保存配置、状态或计算结果。
        pore_d_target = float(self.pore_target_param.get())
# 行 939：普通语句：执行一步操作或表达式。
        self.axs_expand[0].cla()
# 行 940：普通语句：执行一步操作或表达式。
        self.axs_expand[1].cla()
# 行 941：普通语句：执行一步操作或表达式。
        self.expand_i.clear()
# 行 942：普通语句：执行一步操作或表达式。
        self.expand_v.clear()
# 行 943：普通语句：执行一步操作或表达式。
        self.expand_r.clear()
# 行 944：普通语句：执行一步操作或表达式。
        self.expand_time.clear()
# 行 945：普通语句：执行一步操作或表达式。
        self.expand_R_pore.clear()
# 行 946：定义函数/方法 update_data_expand()。
        def update_data_expand():
# 行 947：普通语句：执行一步操作或表达式。
            nonlocal voltage, expand_time
# 行 948：条件判断，根据不同条件执行不同分支。
            if len(self.expand_v) <= 300:
# 行 949：变量赋值：保存配置、状态或计算结果。
                R_pore = self.calculate_d(1 / self.make_r[-1], solution_sigma, pore_h)
# 行 950：条件判断，根据不同条件执行不同分支。
                if R_pore < pore_d_target:
# 行 951：向源表/仪器发送 SCPI 指令以设置或查询参数。
                    self.kei_device.write(f'smu{self.channel}.source.levelv = {voltage}')
# 行 952：变量赋值：保存配置、状态或计算结果。
                    data_i = float(self.kei_device.query(f'print(smu{self.channel}.measure.i())'))
# 行 953：普通语句：执行一步操作或表达式。
                    self.expand_i.append(round(data_i, 12))
# 行 954：普通语句：执行一步操作或表达式。
                    self.expand_v.append(round(voltage, 4))
# 行 955：普通语句：执行一步操作或表达式。
                    self.expand_r.append(abs(voltage / data_i))
# 行 956：普通语句：执行一步操作或表达式。
                    self.expand_time.append(expand_time)
# 行 957：普通语句：执行一步操作或表达式。
                    self.expand_R_pore.append(R_pore)
# 行 958：变量赋值：保存配置、状态或计算结果。
                    voltage = -voltage
# 行 959：普通语句：执行一步操作或表达式。
                    expand_time += 0.5
# 行 960：空行，用于分隔代码块，提高可读性。

# 行 961：普通语句：执行一步操作或表达式。
                    self.plot_expand_update()
# 行 962：空行，用于分隔代码块，提高可读性。

# 行 963：普通语句：执行一步操作或表达式。
                    self.root.after(100, update_data_expand)
# 行 964：条件判断，根据不同条件执行不同分支。
                else:
# 行 965：向源表/仪器发送 SCPI 指令以设置或查询参数。
                    self.kei_device.write(f'smu{self.channel}.source.levelv = 0')
# 行 966：向源表/仪器发送 SCPI 指令以设置或查询参数。
                    self.kei_device.write(f'smu{self.channel}.reset()')
# 行 967：向源表/仪器发送 SCPI 指令以设置或查询参数。
                    self.kei_device.write(f'smu{self.channel}.source.output = smu{self.channel}.OUTPUT_OFF')
# 行 968：变量赋值：保存配置、状态或计算结果。
                    df = pd.DataFrame({
# 行 969：普通语句：执行一步操作或表达式。
                        'time': self.expand_time,
# 行 970：普通语句：执行一步操作或表达式。
                        'voltage': self.expand_v,
# 行 971：普通语句：执行一步操作或表达式。
                        'current': self.expand_i,
# 行 972：普通语句：执行一步操作或表达式。
                        'resistor': self.expand_r,
# 行 973：普通语句：执行一步操作或表达式。
                        'R_pore': self.expand_R_pore,
# 行 974：普通语句：执行一步操作或表达式。
                    })
# 行 975：普通语句：执行一步操作或表达式。
                    df.to_csv(f'{self.a_save_path}/pore_expand.csv', index=False)
# 行 976：向源表/仪器发送 SCPI 指令以设置或查询参数。
                    self.kei_device.write(f'smu{self.channel}.source.levelv = 0')
# 行 977：打印调试或状态信息到控制台。
                    print('扩孔完成，已达到目标孔径')
# 行 978：条件判断，根据不同条件执行不同分支。
            else:
# 行 979：变量赋值：保存配置、状态或计算结果。
                self.expand_v = self.expand_v[-30:]
# 行 980：变量赋值：保存配置、状态或计算结果。
                self.expand_i = self.expand_i[-30:]
# 行 981：变量赋值：保存配置、状态或计算结果。
                self.expand_time = self.expand_time[-30:]
# 行 982：变量赋值：保存配置、状态或计算结果。
                self.expand_r = self.make_r[-30:]
# 行 983：变量赋值：保存配置、状态或计算结果。
                self.expand_R_pore = self.expand_R_pore[-30:]
# 行 984：普通语句：执行一步操作或表达式。
                self.axs_expand[0].cla()
# 行 985：普通语句：执行一步操作或表达式。
                self.axs_expand[1].cla()
# 行 986：普通语句：执行一步操作或表达式。
                self.root.after(100, update_data_expand)
# 行 987：普通语句：执行一步操作或表达式。
        update_data_expand()
# 行 988：定义函数/方法 add_control_page(self)。
    def add_control_page(self):
# 行 989：变量赋值：保存配置、状态或计算结果。
        self.control_page_original = tk.Frame(self.root, width=1950, height=1000, bg='#e5e5e5', bd=5,
# 行 990：变量赋值：保存配置、状态或计算结果。
                                              relief=tk.RAISED)
# 行 991：普通语句：执行一步操作或表达式。
        self.control_page_original.place(x=0, y=0)
# 行 992：空行，用于分隔代码块，提高可读性。

# 行 993：变量赋值：保存配置、状态或计算结果。
        self.canvas_sq_line_model = tk.Canvas(self.control_page_original, width=310, height=210, bg='#e5e5e5', bd=0)
# 行 994：普通语句：执行一步操作或表达式。
        self.canvas_sq_line_model.place(x=0, y=80)
# 行 995：普通语句：执行一步操作或表达式。
        self.create_dashed_square(self.canvas_sq_line_model, 3, 3, 310, 210, 1)
# 行 996：空行，用于分隔代码块，提高可读性。

# 行 997：普通语句：执行一步操作或表达式。
        self.creat_color_button(1, self.control_page_original, 300, 8)
# 行 998：空行，用于分隔代码块，提高可读性。

# 行 999：变量赋值：保存配置、状态或计算结果。
        self.device_list_button = create_button(self.control_page_original, '检查连接设备', self.inquire_device, 12, 1,5, 5)
# 行 1000：空行，用于分隔代码块，提高可读性。

# 行 1001：定义函数/方法 on_select_device(kei_device_name)。
        def on_select_device(kei_device_name):
# 行 1002：变量赋值：保存配置、状态或计算结果。
            self.kei_device_name = kei_device_name
# 行 1003：空行，用于分隔代码块，提高可读性。

# 行 1004：变量赋值：保存配置、状态或计算结果。
        self.combo_device_list = ttk.Combobox(self.control_page_original, values=['', '', ''], width=15,
# 行 1005：变量赋值：保存配置、状态或计算结果。
                                              state='readonly')
# 行 1006：普通语句：执行一步操作或表达式。
        self.combo_device_list.current(0)
# 行 1007：普通语句：执行一步操作或表达式。
        self.combo_device_list.bind("<<ComboboxSelected>>", lambda event: on_select_device(self.kei_device_name))
# 行 1008：普通语句：执行一步操作或表达式。
        self.combo_device_list.place(x=110, y=8)
# 行 1009：空行，用于分隔代码块，提高可读性。

# 行 1010：空行，用于分隔代码块，提高可读性。

# 行 1011：变量赋值：保存配置、状态或计算结果。
        self.device_connect_button = create_button(self.control_page_original, '连接',lambda: self.connect_device(self.device_print,str(self.kei_device_model)[:-1]), 5, 1,250, 5)
# 行 1012：空行，用于分隔代码块，提高可读性。

# 行 1013：变量赋值：保存配置、状态或计算结果。
        self.device_print = create_entry2(self.control_page_original, 43, 5, 50, default_value="无连接")
# 行 1014：普通语句：执行一步操作或表达式。
        self.device_print.config(state=tk.DISABLED)
# 行 1015：空行，用于分隔代码块，提高可读性。

# 行 1016：普通语句：执行一步操作或表达式。
        tk.Button(self.control_page_original, text="存储路径", command=self.a_open_new_window,
# 行 1017：变量赋值：保存配置、状态或计算结果。
                  width=8, height=1).place(x=350, y=5)
# 行 1018：变量赋值：保存配置、状态或计算结果。
        self.save_path_entry = create_entry(self.control_page_original, 50, 450, 8)
# 行 1019：普通语句：执行一步操作或表达式。
        self.save_path_entry.config(state=tk.DISABLED)
# 行 1020：空行，用于分隔代码块，提高可读性。

# 行 1021：变量赋值：保存配置、状态或计算结果。
        self.model_select = create_label1(self.control_page_original, '模式选择:', 20, 90)
# 行 1022：空行，用于分隔代码块，提高可读性。

# 行 1023：变量赋值：保存配置、状态或计算结果。
        self.start_make_pore = create_button1(self.control_page_original, '开始打孔',
# 行 1024：普通语句：执行一步操作或表达式。
                                              lambda: self.start_make_pore_process(), 10, 2, 190, 225, 12)
# 行 1025：普通语句：执行一步操作或表达式。
        self.start_make_pore.config(state=tk.DISABLED, bg='lightgray')
# 行 1026：变量赋值：保存配置、状态或计算结果。
        self.toggle_switch = self.create_toggle_switch(self.control_page_original, 130, 130)
# 行 1027：空行，用于分隔代码块，提高可读性。

# 行 1028：变量赋值：保存配置、状态或计算结果。
        self.start_way = create_label1(self.control_page_original, '手动打孔   ||   自动打孔', 45, 185)
# 行 1029：空行，用于分隔代码块，提高可读性。

# 行 1030：变量赋值：保存配置、状态或计算结果。
        self.canvas_sq_line_check = tk.Canvas(self.control_page_original, width=310, height=210, bg='#e5e5e5', bd=0)
# 行 1031：普通语句：执行一步操作或表达式。
        self.canvas_sq_line_check.place(x=312, y=80)
# 行 1032：普通语句：执行一步操作或表达式。
        self.create_dashed_square(self.canvas_sq_line_check, 3, 3, 310, 210, 1)
# 行 1033：空行，用于分隔代码块，提高可读性。

# 行 1034：变量赋值：保存配置、状态或计算结果。
        self.limit_current = create_label(self.control_page_original, '钳位电流(A)', 350, 50)
# 行 1035：变量赋值：保存配置、状态或计算结果。
        self.limit_current_param = create_entry1(self.control_page_original, 10, 470, 50, 1e-4)
# 行 1036：空行，用于分隔代码块，提高可读性。

# 行 1037：变量赋值：保存配置、状态或计算结果。
        self.channel_index = create_label(self.control_page_original, '通道：', 570, 50)
# 行 1038：变量赋值：保存配置、状态或计算结果。
        options2 = ['a', 'b']
# 行 1039：空行，用于分隔代码块，提高可读性。

# 行 1040：变量赋值：保存配置、状态或计算结果。
        self.combo2 = ttk.Combobox(self.control_page_original, values=options2, width=5, state='readonly')
# 行 1041：普通语句：执行一步操作或表达式。
        self.combo2.current(0)  # 设置默认选中的项
# 行 1042：普通语句：执行一步操作或表达式。
        self.combo2.bind("<<ComboboxSelected>>", lambda event: self.on_select(2))
# 行 1043：普通语句：执行一步操作或表达式。
        self.combo2.place(x=630, y=50)
# 行 1044：空行，用于分隔代码块，提高可读性。

# 行 1045：变量赋值：保存配置、状态或计算结果。
        self.check_param = create_label1(self.control_page_original, '芯片检查参数', 330, 90)
# 行 1046：变量赋值：保存配置、状态或计算结果。
        self.check_volt = create_label(self.control_page_original, '扫描电压(V)', 350, 140)
# 行 1047：变量赋值：保存配置、状态或计算结果。
        self.check_volt_param = create_entry1(self.control_page_original, 10, 470, 140, 0.1)
# 行 1048：变量赋值：保存配置、状态或计算结果。
        self.check_r1 = create_label(self.control_page_original, '平均阻抗（Ω）', 350, 190)
# 行 1049：变量赋值：保存配置、状态或计算结果。
        self.check_r_entry = create_entry1(self.control_page_original, 10, 470, 190, '#')
# 行 1050：普通语句：执行一步操作或表达式。
        self.check_r_entry.config(state=tk.DISABLED)
# 行 1051：空行，用于分隔代码块，提高可读性。

# 行 1052：变量赋值：保存配置、状态或计算结果。
        self.canvas_manual_check = tk.Canvas(self.control_page_original, width=31, height=31, bg='#e5e5e5', bd=0)
# 行 1053：普通语句：执行一步操作或表达式。
        self.canvas_manual_check.place(x=585, y=85)
# 行 1054：普通语句：执行一步操作或表达式。
        self.draw_triangle('check', self.canvas_manual_check)
# 行 1055：普通语句：执行一步操作或表达式。
        self.canvas_manual_check.tag_bind('shape', '<Button-1>',
# 行 1056：普通语句：执行一步操作或表达式。
                                          lambda event: self.toggle_shape('check', self.canvas_manual_check))
# 行 1057：空行，用于分隔代码块，提高可读性。

# 行 1058：变量赋值：保存配置、状态或计算结果。
        self.canvas_sq_line_make = tk.Canvas(self.control_page_original, width=310, height=210, bg='#e5e5e5', bd=0)
# 行 1059：普通语句：执行一步操作或表达式。
        self.canvas_sq_line_make.place(x=624, y=80)
# 行 1060：普通语句：执行一步操作或表达式。
        self.create_dashed_square(self.canvas_sq_line_make, 3, 3, 310, 210, 1)
# 行 1061：空行，用于分隔代码块，提高可读性。

# 行 1062：变量赋值：保存配置、状态或计算结果。
        self.make_param = create_label1(self.control_page_original, '打孔参数', 642, 90)
# 行 1063：变量赋值：保存配置、状态或计算结果。
        self.make_volt = create_label(self.control_page_original, '制造电压(V)', 624 + 38, 140)
# 行 1064：变量赋值：保存配置、状态或计算结果。
        self.make_volt_param = create_entry1(self.control_page_original, 10, 624 + 38 + 120, 140, 19)
# 行 1065：变量赋值：保存配置、状态或计算结果。
        self.make_solution = create_label(self.control_page_original, '溶液电导率(S/m)', 624 + 38, 140 + 50)
# 行 1066：变量赋值：保存配置、状态或计算结果。
        self.make_solution_param = create_entry1(self.control_page_original, 10, 624 + 38 + 120, 140 + 50, 11.27)
# 行 1067：变量赋值：保存配置、状态或计算结果。
        self.make_SiN = create_label(self.control_page_original, '氮化硅膜厚(nm)', 624 + 38, 140 + 50 + 50)
# 行 1068：变量赋值：保存配置、状态或计算结果。
        self.make_SiN_param = create_entry1(self.control_page_original, 10, 624 + 38 + 120, 140 + 50 + 50, 30)
# 行 1069：变量赋值：保存配置、状态或计算结果。
        self.canvas_manual_make = tk.Canvas(self.control_page_original, width=31, height=31, bg='#e5e5e5', bd=0)
# 行 1070：普通语句：执行一步操作或表达式。
        self.canvas_manual_make.place(x=585 + 312, y=85)
# 行 1071：普通语句：执行一步操作或表达式。
        self.draw_triangle('make', self.canvas_manual_make)
# 行 1072：普通语句：执行一步操作或表达式。
        self.canvas_manual_make.tag_bind('shape', '<Button-1>',
# 行 1073：普通语句：执行一步操作或表达式。
                                         lambda event: self.toggle_shape('make', self.canvas_manual_make))
# 行 1074：空行，用于分隔代码块，提高可读性。

# 行 1075：变量赋值：保存配置、状态或计算结果。
        self.canvas_sq_line_expand = tk.Canvas(self.control_page_original, width=310, height=210, bg='#e5e5e5', bd=0)
# 行 1076：普通语句：执行一步操作或表达式。
        self.canvas_sq_line_expand.place(x=936, y=80)
# 行 1077：普通语句：执行一步操作或表达式。
        self.create_dashed_square(self.canvas_sq_line_expand, 3, 3, 310, 210, 1)
# 行 1078：空行，用于分隔代码块，提高可读性。

# 行 1079：变量赋值：保存配置、状态或计算结果。
        self.expand_param = create_label1(self.control_page_original, '扩孔参数', 954, 90)
# 行 1080：变量赋值：保存配置、状态或计算结果。
        self.expand_volt = create_label(self.control_page_original, '扩孔电压(V)', 954 + 38, 140)
# 行 1081：变量赋值：保存配置、状态或计算结果。
        self.expand_volt_param = create_entry1(self.control_page_original, 10, 954 + 38 + 120, 140, 5)
# 行 1082：变量赋值：保存配置、状态或计算结果。
        self.pore_target = create_label(self.control_page_original, '目标孔径（nm）', 954 + 38, 140 + 50)
# 行 1083：变量赋值：保存配置、状态或计算结果。
        self.pore_target_param = create_entry1(self.control_page_original, 10, 954 + 38 + 120, 140 + 50, 50)
# 行 1084：空行，用于分隔代码块，提高可读性。

# 行 1085：变量赋值：保存配置、状态或计算结果。
        self.canvas_sq_line_process = tk.Canvas(self.control_page_original, width=310, height=210, bg='#e5e5e5', bd=0)
# 行 1086：普通语句：执行一步操作或表达式。
        self.canvas_sq_line_process.place(x=1248, y=80)
# 行 1087：普通语句：执行一步操作或表达式。
        self.create_dashed_square(self.canvas_sq_line_process, 3, 3, 310, 210, 1)
# 行 1088：变量赋值：保存配置、状态或计算结果。
        self.check_process = create_label1(self.control_page_original, '密封性检查', 1330, 95)
# 行 1089：变量赋值：保存配置、状态或计算结果。
        self.make_process = create_label1(self.control_page_original, 'CBD打孔', 1345, 95 + 50)
# 行 1090：变量赋值：保存配置、状态或计算结果。
        self.expand_process = create_label1(self.control_page_original, '电压扩孔', 1340, 95 + 100)
# 行 1091：变量赋值：保存配置、状态或计算结果。
        self.edd_process = create_label1(self.control_page_original, '制造完成', 1340, 95 + 150)
# 行 1092：变量赋值：保存配置、状态或计算结果。
        self.canvas_manual_expand = tk.Canvas(self.control_page_original, width=31, height=31, bg='#e5e5e5', bd=0)
# 行 1093：普通语句：执行一步操作或表达式。
        self.canvas_manual_expand.place(x=585 + 312 + 312, y=85)
# 行 1094：普通语句：执行一步操作或表达式。
        self.draw_triangle('expand', self.canvas_manual_expand)
# 行 1095：普通语句：执行一步操作或表达式。
        self.canvas_manual_expand.tag_bind('shape', '<Button-1>',
# 行 1096：普通语句：执行一步操作或表达式。
                                           lambda event: self.toggle_shape('expand', self.canvas_manual_expand))
# 行 1097：空行，用于分隔代码块，提高可读性。

# 行 1098：空行，用于分隔代码块，提高可读性。

# 行 1099：普通语句：执行一步操作或表达式。
        self.creat_color_button(2, self.control_page_original, 1460, 94)
# 行 1100：普通语句：执行一步操作或表达式。
        self.creat_color_button(3, self.control_page_original, 1460, 94 + 50)
# 行 1101：普通语句：执行一步操作或表达式。
        self.creat_color_button(4, self.control_page_original, 1460, 94 + 100)
# 行 1102：普通语句：执行一步操作或表达式。
        self.creat_color_button(5, self.control_page_original, 1460, 94 + 150)
# 行 1103：普通语句：执行一步操作或表达式。
        self.plot_VIR_sigma()
# 行 1104：空行，用于分隔代码块，提高可读性。

# 行 1105：变量赋值：保存配置、状态或计算结果。
        self.toolbar_current = NavigationToolbar2Tk(self.canvas_plotVIR_sigma, self.control_page_original)
# 行 1106：普通语句：执行一步操作或表达式。
        self.toolbar_current.update()
# 行 1107：普通语句：执行一步操作或表达式。
        self.toolbar_current.place(x=0, y=960)
# 行 1108：空行，用于分隔代码块，提高可读性。

# 行 1109：普通语句：执行一步操作或表达式。
        self.plot_check()
# 行 1110：普通语句：执行一步操作或表达式。
        self.plot_expand_pore()
# 行 1111：空行，用于分隔代码块，提高可读性。

# 行 1112：普通语句：执行一步操作或表达式。
    '''数据采集(eONE)'''
# 行 1113：空行，用于分隔代码块，提高可读性。

# 行 1114：定义函数/方法 bc_data_10_func(self, original_list)。
    def bc_data_10_func(self, original_list):
# 行 1115：变量赋值：保存配置、状态或计算结果。
        indexed_data = [(value, idx) for idx, value in enumerate(original_list)]
# 行 1116：变量赋值：保存配置、状态或计算结果。
        data_length = len(indexed_data)
# 行 1117：变量赋值：保存配置、状态或计算结果。
        remove_count = int(data_length * 0.1)
# 行 1118：空行，用于分隔代码块，提高可读性。

# 行 1119：条件判断，根据不同条件执行不同分支。
        if remove_count == 0:
# 行 1120：返回函数结果。
            return original_list.copy()
# 行 1121：空行，用于分隔代码块，提高可读性。

# 行 1122：变量赋值：保存配置、状态或计算结果。
        sorted_data = sorted(indexed_data, key=lambda x: x[0])
# 行 1123：变量赋值：保存配置、状态或计算结果。
        left_removed_indices = [idx for _, idx in sorted_data[:remove_count]]
# 行 1124：变量赋值：保存配置、状态或计算结果。
        middle_valid = sorted_data[remove_count: data_length - remove_count]
# 行 1125：变量赋值：保存配置、状态或计算结果。
        right_removed_indices = [idx for _, idx in sorted_data[data_length - remove_count:]]
# 行 1126：空行，用于分隔代码块，提高可读性。

# 行 1127：变量赋值：保存配置、状态或计算结果。
        middle_values = [val for val, _ in middle_valid]
# 行 1128：变量赋值：保存配置、状态或计算结果。
        left_fill_values = middle_values[:remove_count]
# 行 1129：变量赋值：保存配置、状态或计算结果。
        right_fill_values = middle_values[-remove_count:]
# 行 1130：空行，用于分隔代码块，提高可读性。

# 行 1131：变量赋值：保存配置、状态或计算结果。
        result_map = {}
# 行 1132：空行，用于分隔代码块，提高可读性。

# 行 1133：for 循环遍历序列或可迭代对象。
        for val, idx in middle_valid:
# 行 1134：普通语句：执行一步操作或表达式。
            result_map[idx] = val
# 行 1135：空行，用于分隔代码块，提高可读性。

# 行 1136：for 循环遍历序列或可迭代对象。
        for fill_val, original_idx in zip(left_fill_values, left_removed_indices):
# 行 1137：普通语句：执行一步操作或表达式。
            result_map[original_idx] = fill_val
# 行 1138：空行，用于分隔代码块，提高可读性。

# 行 1139：for 循环遍历序列或可迭代对象。
        for fill_val, original_idx in zip(right_fill_values, right_removed_indices):
# 行 1140：普通语句：执行一步操作或表达式。
            result_map[original_idx] = fill_val
# 行 1141：空行，用于分隔代码块，提高可读性。

# 行 1142：变量赋值：保存配置、状态或计算结果。
        processed_list = [result_map[idx] for idx in range(data_length)]
# 行 1143：返回函数结果。
        return processed_list
# 行 1144：定义函数/方法 b_show_table_tk(self)。
    def b_show_table_tk(self):
# 行 1145：变量赋值：保存配置、状态或计算结果。
        self.table_frame = tk.Frame(
# 行 1146：普通语句：执行一步操作或表达式。
            self.collection_eONE_page_original,
# 行 1147：变量赋值：保存配置、状态或计算结果。
            width=800,
# 行 1148：变量赋值：保存配置、状态或计算结果。
            height=600,
# 行 1149：变量赋值：保存配置、状态或计算结果。
            bg='#e5e5e5',
# 行 1150：变量赋值：保存配置、状态或计算结果。
            relief=tk.SUNKEN,
# 行 1151：变量赋值：保存配置、状态或计算结果。
            bd=2
# 行 1152：普通语句：执行一步操作或表达式。
        )
# 行 1153：普通语句：执行一步操作或表达式。
        self.table_frame.place(x=312, y=470)
# 行 1154：空行，用于分隔代码块，提高可读性。

# 行 1155：变量赋值：保存配置、状态或计算结果。
        style = ttk.Style()
# 行 1156：空行，用于分隔代码块，提高可读性。

# 行 1157：普通语句：执行一步操作或表达式。
        style.theme_use('default')
# 行 1158：空行，用于分隔代码块，提高可读性。

# 行 1159：普通语句：执行一步操作或表达式。
        style.configure(
# 行 1160：普通语句：执行一步操作或表达式。
            "Custom.Treeview",
# 行 1161：变量赋值：保存配置、状态或计算结果。
            background="#e5e5e5",
# 行 1162：变量赋值：保存配置、状态或计算结果。
            fieldbackground="#e5e5e5",
# 行 1163：普通语句：执行一步操作或表达式。
        )
# 行 1164：空行，用于分隔代码块，提高可读性。

# 行 1165：普通语句：执行一步操作或表达式。
        style.configure(
# 行 1166：普通语句：执行一步操作或表达式。
            "Custom.Treeview.Heading",
# 行 1167：变量赋值：保存配置、状态或计算结果。
            background="#e5e5e5",
# 行 1168：普通语句：执行一步操作或表达式。
        )
# 行 1169：空行，用于分隔代码块，提高可读性。

# 行 1170：普通语句：执行一步操作或表达式。
        style.map(
# 行 1171：普通语句：执行一步操作或表达式。
            "Custom.Treeview",
# 行 1172：变量赋值：保存配置、状态或计算结果。
            background=[('selected', 'skyblue')]
# 行 1173：普通语句：执行一步操作或表达式。
        )
# 行 1174：空行，用于分隔代码块，提高可读性。

# 行 1175：变量赋值：保存配置、状态或计算结果。
        self.tree = ttk.Treeview(
# 行 1176：普通语句：执行一步操作或表达式。
            self.table_frame,
# 行 1177：变量赋值：保存配置、状态或计算结果。
            columns=[str(i) for i in range(len(self.header_list))],
# 行 1178：变量赋值：保存配置、状态或计算结果。
            show='headings',
# 行 1179：变量赋值：保存配置、状态或计算结果。
            height=14,
# 行 1180：变量赋值：保存配置、状态或计算结果。
            style="Custom.Treeview"
# 行 1181：普通语句：执行一步操作或表达式。
        )
# 行 1182：空行，用于分隔代码块，提高可读性。

# 行 1183：for 循环遍历序列或可迭代对象。
        for i in range(len(self.header_list)):
# 行 1184：普通语句：执行一步操作或表达式。
            self.tree.heading(str(i), text=f'{self.header_list[i]}')
# 行 1185：普通语句：执行一步操作或表达式。
            self.tree.column(str(i), width=137, anchor='center')
# 行 1186：空行，用于分隔代码块，提高可读性。

# 行 1187：变量赋值：保存配置、状态或计算结果。
        scrollbar = ttk.Scrollbar(self.table_frame, orient=tk.VERTICAL, command=self.tree.yview)
# 行 1188：普通语句：执行一步操作或表达式。
        self.tree.configure(yscrollcommand=scrollbar.set)
# 行 1189：空行，用于分隔代码块，提高可读性。

# 行 1190：普通语句：执行一步操作或表达式。
        self.tree.grid(row=0, column=0, sticky='nsew')
# 行 1191：普通语句：执行一步操作或表达式。
        scrollbar.grid(row=0, column=1, sticky='ns')
# 行 1192：空行，用于分隔代码块，提高可读性。

# 行 1193：普通语句：执行一步操作或表达式。
        self.table_frame.grid_rowconfigure(0, weight=1)
# 行 1194：普通语句：执行一步操作或表达式。
        self.table_frame.grid_columnconfigure(0, weight=1)
# 行 1195：空行，用于分隔代码块，提高可读性。

# 行 1196：普通语句：执行一步操作或表达式。
        self.tree.bind("<ButtonRelease-1>", self.b_on_item_click)
# 行 1197：空行，用于分隔代码块，提高可读性。

# 行 1198：定义函数/方法 b_disconnect_devices(self)。
    def b_disconnect_devices(self):
# 行 1199：普通语句：执行一步操作或表达式。
        c4.disconnect()
# 行 1200：打印调试或状态信息到控制台。
        print(1)
# 行 1201：变量赋值：保存配置、状态或计算结果。
        self.b_connect_stats = 0
# 行 1202：普通语句：执行一步操作或表达式。
        self.update_color_button(6, 'red')
# 行 1203：定义函数/方法 b_connect_devices(self, number)。
    def b_connect_devices(self, number):
# 行 1204：普通语句：执行一步操作或表达式。
        err, self.b_devices = c4.detectDevices()
# 行 1205：打印调试或状态信息到控制台。
        print(err)
# 行 1206：条件判断，根据不同条件执行不同分支。
        if err != c4.Success:
# 行 1207：普通语句：执行一步操作或表达式。
            sys.exit()
# 行 1208：变量赋值：保存配置、状态或计算结果。
        err = c4.connect(self.b_devices)
# 行 1209：条件判断，根据不同条件执行不同分支。
        if err != c4.Success:
# 行 1210：普通语句：执行一步操作或表达式。
            sys.exit()
# 行 1211：条件判断，根据不同条件执行不同分支。
        else:
# 行 1212：普通语句：执行一步操作或表达式。
            self.update_color_button(6, 'green')
# 行 1213：普通语句：执行一步操作或表达式。
            e, self.b_v_channels, self.b_i_channels, gp_channels = c4.getChannelsNumber()
# 行 1214：普通语句：执行一步操作或表达式。
            c4.setCurrentRange(0, self.b_i_channels)
# 行 1215：普通语句：执行一步操作或表达式。
            c4.setSamplingRate(0)
# 行 1216：空行，用于分隔代码块，提高可读性。

# 行 1217：普通语句：执行一步操作或表达式。
            e, names, images, voltages, times, slopes, frequencies, adimensionals = c4.getProtocolList()
# 行 1218：空行，用于分隔代码块，提高可读性。

# 行 1219：普通语句：执行一步操作或表达式。
            e, v_names, v_ranges, default_values = c4.getProtocolVoltage()
# 行 1220：普通语句：执行一步操作或表达式。
            c4.digitalOffsetCompensation(self.b_i_channels, True)
# 行 1221：普通语句：执行一步操作或表达式。
            time.sleep(1)
# 行 1222：空行，用于分隔代码块，提高可读性。

# 行 1223：普通语句：执行一步操作或表达式。
            c4.digitalOffsetCompensation(self.b_i_channels, False)
# 行 1224：普通语句：执行一步操作或表达式。
            c4.selectVoltageProtocol(0)
# 行 1225：空行，用于分隔代码块，提高可读性。

# 行 1226：变量赋值：保存配置、状态或计算结果。
            v_hold = c4.Measurement(0, c4.UnitPfxMilli, "V")
# 行 1227：普通语句：执行一步操作或表达式。
            c4.setProtocolVoltage(0, v_hold)
# 行 1228：空行，用于分隔代码块，提高可读性。

# 行 1229：变量赋值：保存配置、状态或计算结果。
            v_pulse = c4.Measurement(1000.0, c4.UnitPfxMilli, "V")
# 行 1230：普通语句：执行一步操作或表达式。
            c4.setProtocolVoltage(1, v_pulse)
# 行 1231：空行，用于分隔代码块，提高可读性。

# 行 1232：变量赋值：保存配置、状态或计算结果。
            v_step = c4.Measurement(2000.0, c4.UnitPfxMilli, "V")
# 行 1233：普通语句：执行一步操作或表达式。
            c4.setProtocolVoltage(2, v_step)
# 行 1234：空行，用于分隔代码块，提高可读性。

# 行 1235：变量赋值：保存配置、状态或计算结果。
            t_hold = c4.Measurement(500.0, c4.UnitPfxMilli, "s")
# 行 1236：普通语句：执行一步操作或表达式。
            c4.setProtocolTime(0, t_hold)
# 行 1237：空行，用于分隔代码块，提高可读性。

# 行 1238：变量赋值：保存配置、状态或计算结果。
            t_pulse = c4.Measurement(40.0, c4.UnitPfxMilli, "s")
# 行 1239：普通语句：执行一步操作或表达式。
            c4.setProtocolTime(1, t_pulse)
# 行 1240：空行，用于分隔代码块，提高可读性。

# 行 1241：变量赋值：保存配置、状态或计算结果。
            adim_n = c4.Measurement(10.0, c4.UnitPfxNone, "")
# 行 1242：普通语句：执行一步操作或表达式。
            c4.setProtocolAdimensional(0, adim_n)
# 行 1243：空行，用于分隔代码块，提高可读性。

# 行 1244：变量赋值：保存配置、状态或计算结果。
            adim_n_r = c4.Measurement(0.0, c4.UnitPfxNone, "")
# 行 1245：普通语句：执行一步操作或表达式。
            c4.setProtocolAdimensional(1, adim_n_r)
# 行 1246：空行，用于分隔代码块，提高可读性。

# 行 1247：条件判断，根据不同条件执行不同分支。
            if c4.applyVoltageProtocol() != c4.Success:
# 行 1248：打印调试或状态信息到控制台。
                print("无法应用协议")
# 行 1249：普通语句：执行一步操作或表达式。
                c4.disconnect()
# 行 1250：普通语句：执行一步操作或表达式。
                sys.exit()
# 行 1251：普通语句：执行一步操作或表达式。
            _, self.b_i_range = c4.getCurrentRange(0)
# 行 1252：普通语句：执行一步操作或表达式。
            _, self.b_v_range = c4.getVoltageRange()
# 行 1253：空行，用于分隔代码块，提高可读性。

# 行 1254：变量赋值：保存配置、状态或计算结果。
            self.i_m = self.b_i_range.step
# 行 1255：变量赋值：保存配置、状态或计算结果。
            self.v_m = self.b_v_range.step
# 行 1256：空行，用于分隔代码块，提高可读性。

# 行 1257：普通语句：执行一步操作或表达式。
            c4.purgeData()
# 行 1258：空行，用于分隔代码块，提高可读性。

# 行 1259：变量赋值：保存配置、状态或计算结果。
            self.b_total_channels = self.b_v_channels + self.b_i_channels + gp_channels
# 行 1260：变量赋值：保存配置、状态或计算结果。
            ad_rdy_ev = threading.Event()
# 行 1261：普通语句：执行一步操作或表达式。
            ad_rdy_ev.set()  # 线程运行
# 行 1262：变量赋值：保存配置、状态或计算结果。
            t = threading.Thread(target=self.b_start_get_date)
# 行 1263：变量赋值：保存配置、状态或计算结果。
            t.daemon = True
# 行 1264：普通语句：执行一步操作或表达式。
            t.start()
# 行 1265：空行，用于分隔代码块，提高可读性。

# 行 1266：定义函数/方法 b_start_get_date(self)。
    def b_start_get_date(self):
# 行 1267：原始注释行，提供作者或用途说明。
        # self.b_fig, (self.b_ax1, self.b_ax2) = plt.subplots(2, 1, figsize=(11.0, 5.5))
# 行 1268：原始注释行，提供作者或用途说明。
        # self.b_line1, = self.b_ax1.plot([], [], lw=2)
# 行 1269：原始注释行，提供作者或用途说明。
        # self.b_line2, = self.b_ax2.plot([], [], lw=2)
# 行 1270：普通语句：执行一步操作或表达式。
        self.canvas_current2.destroy()
# 行 1271：变量赋值：保存配置、状态或计算结果。
        self.canvas_current2 = None
# 行 1272：变量赋值：保存配置、状态或计算结果。
        self.canvas_current = None
# 行 1273：变量赋值：保存配置、状态或计算结果。
        self.ax4 = None
# 行 1274：变量赋值：保存配置、状态或计算结果。
        self.b_canvas = FigureCanvasTkAgg(self.b_fig, master=self.collection_picture_page_original)
# 行 1275：普通语句：执行一步操作或表达式。
        self.b_canvas.draw()
# 行 1276：普通语句：执行一步操作或表达式。
        self.b_canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)
# 行 1277：while 循环，满足条件时持续执行。
        while True:
# 行 1278：原始注释行，提供作者或用途说明。
            # time.sleep(0.001)
# 行 1279：普通语句：执行一步操作或表达式。
            e, q = c4.getQueueStatus()
# 行 1280：变量赋值：保存配置、状态或计算结果。
            available_data = q.availableDataPackets
# 行 1281：条件判断，根据不同条件执行不同分支。
            if available_data > 0:
# 行 1282：原始注释行，提供作者或用途说明。
                # with self.b_data_lock:  # 锁保护
# 行 1283：变量赋值：保存配置、状态或计算结果。
                    while_time = self.b_end_time
# 行 1284：普通语句：执行一步操作或表达式。
                    read_error, data = c4.readData(available_data)
# 行 1285：普通语句：执行一步操作或表达式。
                    self.data_processing_thread(data, while_time)
# 行 1286：原始注释行，提供作者或用途说明。
                    # self.std(data)
# 行 1287：条件判断，根据不同条件执行不同分支。
                    if self.b_number == 0:
# 行 1288：空行，用于分隔代码块，提高可读性。

# 行 1289：变量赋值：保存配置、状态或计算结果。
                        ad_rdy_ev = threading.Event()
# 行 1290：普通语句：执行一步操作或表达式。
                        ad_rdy_ev.set()
# 行 1291：变量赋值：保存配置、状态或计算结果。
                        t = threading.Thread(target=self.b_update_plot,
# 行 1292：普通语句：执行一步操作或表达式。
                                             )
# 行 1293：变量赋值：保存配置、状态或计算结果。
                        t.daemon = True
# 行 1294：普通语句：执行一步操作或表达式。
                        t.start()
# 行 1295：普通语句：执行一步操作或表达式。
                        self.threads.append(t)
# 行 1296：原始注释行，提供作者或用途说明。
                        # self.update_plot()
# 行 1297：普通语句：执行一步操作或表达式。
                        self.b_number += 1
# 行 1298：普通语句：执行一步操作或表达式。
            time.sleep(0.0001)
# 行 1299：空行，用于分隔代码块，提高可读性。

# 行 1300：空行，用于分隔代码块，提高可读性。

# 行 1301：定义函数/方法 b_update_plot(self)。
    def b_update_plot(self):
# 行 1302：普通语句：执行一步操作或表达式。
        global data_lock
# 行 1303：代码块开始（如函数、类、条件或循环体）。
        with data_lock:
# 行 1304：普通语句：执行一步操作或表达式。
            self.b_line1.set_xdata(self.b_t_data)
# 行 1305：普通语句：执行一步操作或表达式。
            self.b_line1.set_ydata(self.b_i_data[:len(self.b_t_data)])
# 行 1306：原始注释行，提供作者或用途说明。
            # self.b_line2.set_xdata(self.b_t_data)
# 行 1307：原始注释行，提供作者或用途说明。
            # self.b_line2.set_ydata(self.b_v_data[:len(self.b_t_data)])
# 行 1308：空行，用于分隔代码块，提高可读性。

# 行 1309：条件判断，根据不同条件执行不同分支。
            if self.b_detection_param is False:
# 行 1310：for 循环遍历序列或可迭代对象。
                for line_name in ['b_line3', 'b_line4', 'b_line5']:
# 行 1311：条件判断，根据不同条件执行不同分支。
                    if hasattr(self, line_name) and getattr(self, line_name) is not None:
# 行 1312：普通语句：执行一步操作或表达式。
                        getattr(self, line_name).remove()
# 行 1313：普通语句：执行一步操作或表达式。
                        setattr(self, line_name, None)
# 行 1314：条件判断，根据不同条件执行不同分支。
            else:
# 行 1315：条件判断，根据不同条件执行不同分支。
                if not hasattr(self, 'b_line3') or self.b_line3 is None:
# 行 1316：普通语句：执行一步操作或表达式。
                    self.b_line3, = self.b_ax1.plot([], [], 'r--')
# 行 1317：条件判断，根据不同条件执行不同分支。
                if self.b_line3 is not None:
# 行 1318：普通语句：执行一步操作或表达式。
                    self.b_line3.set_xdata(self.b_t_data_data_4)
# 行 1319：普通语句：执行一步操作或表达式。
                    self.b_line3.set_ydata(self.b_std_data_down[:len(self.b_t_data_data_4)])
# 行 1320：空行，用于分隔代码块，提高可读性。

# 行 1321：条件判断，根据不同条件执行不同分支。
                if not hasattr(self, 'b_line4') or self.b_line4 is None:
# 行 1322：普通语句：执行一步操作或表达式。
                    self.b_line4, = self.b_ax1.plot([], [], 'r--')
# 行 1323：条件判断，根据不同条件执行不同分支。
                if self.b_line4 is not None:
# 行 1324：普通语句：执行一步操作或表达式。
                    self.b_line4.set_xdata(self.b_t_data_data_4)
# 行 1325：普通语句：执行一步操作或表达式。
                    self.b_line4.set_ydata(self.b_std_data_up[:len(self.b_t_data_data_4)])
# 行 1326：空行，用于分隔代码块，提高可读性。

# 行 1327：条件判断，根据不同条件执行不同分支。
                if not hasattr(self, 'b_line5') or self.b_line5 is None:
# 行 1328：普通语句：执行一步操作或表达式。
                    self.b_line5, = self.b_ax1.plot([], [], 'k--')
# 行 1329：条件判断，根据不同条件执行不同分支。
                if self.b_line5 is not None:
# 行 1330：普通语句：执行一步操作或表达式。
                    self.b_line5.set_xdata(self.b_t_data_data_4)
# 行 1331：普通语句：执行一步操作或表达式。
                    self.b_line5.set_ydata(self.b_baseline_data[:len(self.b_t_data_data_4)])
# 行 1332：空行，用于分隔代码块，提高可读性。

# 行 1333：原始注释行，提供作者或用途说明。
        # self.b_ax1.relim()
# 行 1334：原始注释行，提供作者或用途说明。
        # self.b_ax1.autoscale_view()
# 行 1335：普通语句：执行一步操作或表达式。
        self.b_ax1.set_ylim(min(self.b_i_data) -1, max(self.b_i_data)+1)
# 行 1336：普通语句：执行一步操作或表达式。
        self.b_ax1.set_xlim(self.b_t_data[0], self.b_t_data[-1])
# 行 1337：原始注释行，提供作者或用途说明。
        # self.b_ax2.relim()
# 行 1338：原始注释行，提供作者或用途说明。
        # self.b_ax2.autoscale_view()
# 行 1339：普通语句：执行一步操作或表达式。
        self.b_canvas.draw_idle()
# 行 1340：空行，用于分隔代码块，提高可读性。

# 行 1341：原始注释行，提供作者或用途说明。
        # # self.ax1.set_ylim(1.5 * min(self.i_data), 1.5 * max(self.i_data))
# 行 1342：原始注释行，提供作者或用途说明。
        # self.b_ax2.set_xlim(self.b_t_data[-1] - 3, self.b_t_data[-1] + 1)
# 行 1343：原始注释行，提供作者或用途说明。
        # self.b_ax1.axhline(y=np.mean(self.b_i_data[-100: -1]), linestyle='--', xmin=0.8, xmax=1.0, clip_on=False)
# 行 1344：原始注释行，提供作者或用途说明。
        # self.ax2.axhline(y=np.mean(self.v_data[-100: -1]), linestyle='--', xmin=0.9, xmax=1.0, clip_on=False)
# 行 1345：普通语句：执行一步操作或表达式。
        self.update_baseline_std_vsm()
# 行 1346：普通语句：执行一步操作或表达式。
        self.b_canvas.get_tk_widget().after(500, self.b_update_plot)
# 行 1347：空行，用于分隔代码块，提高可读性。

# 行 1348：定义函数/方法 update_baseline_std_vsm(self)。
    def update_baseline_std_vsm(self):
# 行 1349：普通语句：执行一步操作或表达式。
        global data_lock1
# 行 1350：代码块开始（如函数、类、条件或循环体）。
        with data_lock1:
# 行 1351：变量赋值：保存配置、状态或计算结果。
            std_number = dynamic_detection.calculate_dynamic_std_py(self.b_i_data_while, 0.73 * len(self.b_i_data_while))
# 行 1352：变量赋值：保存配置、状态或计算结果。
            baseline_number = dynamic_detection.calculate_dynamic_baseline_fast_trimmed_py(self.b_i_data_while, 0.73 * len(self.b_i_data_while))
# 行 1353：变量赋值：保存配置、状态或计算结果。
            cpp_result = dynamic_rms.calculate_dynamic_rms_py(self.b_i_data_while, 0.1 * len(self.b_i_data_while))
# 行 1354：原始注释行，提供作者或用途说明。
            # self.b_display_std.config(text=f'标准差：{round(float(std_number[-1]), 4)}')
# 行 1355：普通语句：执行一步操作或表达式。
            self.b_display_rms.config(text=f'   {round(float(std_number[-1]), 4)}{self.b_last_current_range_selected[-2:]}')
# 行 1356：普通语句：执行一步操作或表达式。
            self.b_display_mean_baseline.config(text=f'基线：{round(float(baseline_number[-1]), 4)}')
# 行 1357：定义函数/方法 integral(self, y, baseline, sampling, direction)。
    def integral(self, y, baseline, sampling, direction):
# 行 1358：变量赋值：保存配置、状态或计算结果。
        integral_area = 0
# 行 1359：条件判断，根据不同条件执行不同分支。
        if direction == 1:
# 行 1360：for 循环遍历序列或可迭代对象。
            for i in range(len(y) - 1):
# 行 1361：条件判断，根据不同条件执行不同分支。
                if i >= 0 and i + 1 < len(y):
# 行 1362：变量赋值：保存配置、状态或计算结果。
                    area = (1 / 2) * (max(y[i], y[i + 1]) - min(y[i], y[i + 1])) * (1 / sampling) + (
# 行 1363：普通语句：执行一步操作或表达式。
                            (min(y[i], y[i + 1]) - baseline) * (1 / sampling))
# 行 1364：普通语句：执行一步操作或表达式。
                    integral_area += area
# 行 1365：条件判断，根据不同条件执行不同分支。
                else:
# 行 1366：占位语句：暂不执行任何操作。
                    pass
# 行 1367：条件判断，根据不同条件执行不同分支。
        else:
# 行 1368：for 循环遍历序列或可迭代对象。
            for i in range(len(y) - 1):
# 行 1369：条件判断，根据不同条件执行不同分支。
                if i >= 0 and i + 1 < len(y):
# 行 1370：变量赋值：保存配置、状态或计算结果。
                    area = (1 / 2) * (max(y[i], y[i + 1]) - min(y[i], y[i + 1])) * (1 / sampling) + (
# 行 1371：普通语句：执行一步操作或表达式。
                            (baseline - min(y[i], y[i + 1])) * (1 / sampling))
# 行 1372：普通语句：执行一步操作或表达式。
                    integral_area += area
# 行 1373：条件判断，根据不同条件执行不同分支。
                else:
# 行 1374：占位语句：暂不执行任何操作。
                    pass
# 行 1375：返回函数结果。
        return np.abs(integral_area)
# 行 1376：定义函数/方法 event_detection_two(self, input_list_x, input_list_y, th_up_index, th_down_index)。
    def event_detection_two(self, input_list_x, input_list_y, th_up_index, th_down_index):
# 行 1377：原始注释行，提供作者或用途说明。
        # return True, True
# 行 1378：变量赋值：保存配置、状态或计算结果。
        integral_area_all = np.sum(np.square(input_list_y))
# 行 1379：变量赋值：保存配置、状态或计算结果。
        th_up_index =  0.00005
# 行 1380：变量赋值：保存配置、状态或计算结果。
        two_decay = 0.000005
# 行 1381：变量赋值：保存配置、状态或计算结果。
        amplitude_one = 0.1
# 行 1382：原始注释行，提供作者或用途说明。
        # coefficient_amplitude = 0.2  #
# 行 1383：原始注释行，提供作者或用途说明。
        # cutoff_frequency = 1000
# 行 1384：原始注释行，提供作者或用途说明。
        # nyquist_frequency = 0.5 * self.c_frequency
# 行 1385：原始注释行，提供作者或用途说明。
        # normal_cutoff = cutoff_frequency / nyquist_frequency
# 行 1386：原始注释行，提供作者或用途说明。
        # b, a = signal.butter(1, normal_cutoff, btype='low', analog=False)
# 行 1387：原始注释行，提供作者或用途说明。
        # input_list_y = signal.filtfilt(b, a, input_list_y)
# 行 1388：变量赋值：保存配置、状态或计算结果。
        y = input_list_y
# 行 1389：原始注释行，提供作者或用途说明。
        # y = savgol_filter(y, 5, 3)
# 行 1390：变量赋值：保存配置、状态或计算结果。
        x = input_list_x
# 行 1391：变量赋值：保存配置、状态或计算结果。
        filter_y = y
# 行 1392：变量赋值：保存配置、状态或计算结果。
        mean_start = np.mean(filter_y)
# 行 1393：变量赋值：保存配置、状态或计算结果。
        start_list = []
# 行 1394：变量赋值：保存配置、状态或计算结果。
        end_list = []
# 行 1395：变量赋值：保存配置、状态或计算结果。
        delta_current = y[0] * 0.005
# 行 1396：变量赋值：保存配置、状态或计算结果。
        down_pulse = False
# 行 1397：变量赋值：保存配置、状态或计算结果。
        down_start_index = 0
# 行 1398：变量赋值：保存配置、状态或计算结果。
        down_end_index = 0
# 行 1399：变量赋值：保存配置、状态或计算结果。
        down_end_number = 0
# 行 1400：变量赋值：保存配置、状态或计算结果。
        down_start_number = 0
# 行 1401：for 循环遍历序列或可迭代对象。
        for i in range(len(filter_y)):
# 行 1402：原始注释行，提供作者或用途说明。
            # print(filter_y[i],((mean_start - (delta_current))), mean_start)
# 行 1403：条件判断，根据不同条件执行不同分支。
            if filter_y[i] < (mean_start - (delta_current)) and not down_pulse:
# 行 1404：原始注释行，提供作者或用途说明。
                # print(333)
# 行 1405：变量赋值：保存配置、状态或计算结果。
                down_pulse = True
# 行 1406：变量赋值：保存配置、状态或计算结果。
                down_start_index = i
# 行 1407：普通语句：执行一步操作或表达式。
                down_start_number += 1
# 行 1408：原始注释行，提供作者或用途说明。
                # print(f'起点{down_start_index}')
# 行 1409：原始注释行，提供作者或用途说明。
                # start_list.append(i)
# 行 1410：条件判断，根据不同条件执行不同分支。
            elif down_pulse == True:
# 行 1411：变量赋值：保存配置、状态或计算结果。
                delta_current = filter_y[down_start_index] - min(y[down_start_index: i + 1])
# 行 1412：变量赋值：保存配置、状态或计算结果。
                mean_slide = np.mean(filter_y[down_start_index:i + 1])
# 行 1413：原始注释行，提供作者或用途说明。
                # print(filter_y[i], (mean_slide + (th_up_index * delta_current)))
# 行 1414：条件判断，根据不同条件执行不同分支。
                if filter_y[i] >= (mean_slide + (th_up_index * delta_current)):
# 行 1415：变量赋值：保存配置、状态或计算结果。
                    down_end_index = i
# 行 1416：普通语句：执行一步操作或表达式。
                    down_end_number += 1
# 行 1417：变量赋值：保存配置、状态或计算结果。
                    integral_area_amplitude = np.sum(np.square(filter_y[down_start_index:down_end_index]))
# 行 1418：空行，用于分隔代码块，提高可读性。

# 行 1419：原始注释行，提供作者或用途说明。
                    # print(f'终点{down_end_index}')
# 行 1420：变量赋值：保存配置、状态或计算结果。
                    down_pulse = False
# 行 1421：条件判断，根据不同条件执行不同分支。
                    if  1 / 5 * (x[-1] - x[0]) > x[down_end_index] - x[down_start_index] > two_decay and min(
# 行 1422：普通语句：执行一步操作或表达式。
                            y[down_start_index: down_end_index]) < mean_start - (delta_current) and (
# 行 1423：普通语句：执行一步操作或表达式。
                            y[down_start_index] - min(y[down_start_index: down_end_index])) > amplitude_one * (
# 行 1424：代码块开始（如函数、类、条件或循环体）。
                            y[0] - y[down_start_index]) and integral_area_amplitude / integral_area_all < 0.13:
# 行 1425：普通语句：执行一步操作或表达式。
                        start_list.append(down_start_index)
# 行 1426：普通语句：执行一步操作或表达式。
                        end_list.append(down_end_index)
# 行 1427：变量赋值：保存配置、状态或计算结果。
                        delta_current = max(y[down_end_index: i + 1]) - filter_y[down_end_index]
# 行 1428：变量赋值：保存配置、状态或计算结果。
                        mean_start = np.mean(filter_y[down_end_index: i + 1])
# 行 1429：原始注释行，提供作者或用途说明。
            # delta_current = filter_y[down_start_index] - min(y[down_start_index: i +1])
# 行 1430：原始注释行，提供作者或用途说明。
        # print(len(start_list), len(end_list),down_end_index)
# 行 1431：空行，用于分隔代码块，提高可读性。

# 行 1432：条件判断，根据不同条件执行不同分支。
        if len(end_list) > 0:
# 行 1433：返回函数结果。
            return True, True
# 行 1434：条件判断，根据不同条件执行不同分支。
        else:
# 行 1435：返回函数结果。
            return False, False
# 行 1436：原始注释行，提供作者或用途说明。
    # def event_detection_two(self, input_list, signal_1, time):
# 行 1437：原始注释行，提供作者或用途说明。
    #     if  50 > time > 0.00001:
# 行 1438：原始注释行，提供作者或用途说明。
    #         # cutoff_frequency = 1000  # 截止频率
# 行 1439：原始注释行，提供作者或用途说明。
    #         # nyquist_frequency = 0.5 * self.c_frequency  # 奈奎斯特频率
# 行 1440：原始注释行，提供作者或用途说明。
    #         # normal_cutoff = cutoff_frequency / nyquist_frequency
# 行 1441：原始注释行，提供作者或用途说明。
    #         # b, a = signal.butter(1, normal_cutoff, btype='low', analog=False)
# 行 1442：原始注释行，提供作者或用途说明。
    #         # input_list = signal.filtfilt(b, a, input_list)
# 行 1443：原始注释行，提供作者或用途说明。
    #         # input_list = savgol_filter(input_list, 20, 3)
# 行 1444：原始注释行，提供作者或用途说明。
    #         down = False
# 行 1445：原始注释行，提供作者或用途说明。
    #         up = False
# 行 1446：原始注释行，提供作者或用途说明。
    #         list_len = len(input_list)
# 行 1447：原始注释行，提供作者或用途说明。
    #         sorted_list = sorted(input_list)
# 行 1448：原始注释行，提供作者或用途说明。
    #         start_idx = int(np.ceil(list_len * 0.40))
# 行 1449：原始注释行，提供作者或用途说明。
    #         end_idx = int(np.ceil(list_len * 0.60))
# 行 1450：原始注释行，提供作者或用途说明。
    #         target_elements = sorted_list[start_idx:end_idx]
# 行 1451：原始注释行，提供作者或用途说明。
    #         mean = [sum(target_elements) / len(target_elements)] * len(input_list)
# 行 1452：原始注释行，提供作者或用途说明。
    #         baseline = [mean] * len(input_list)
# 行 1453：原始注释行，提供作者或用途说明。
    #         std_value = input_list[0] - sum(target_elements) / len(target_elements)
# 行 1454：原始注释行，提供作者或用途说明。
    #         # squared_diff_sum = sum([(x - sum(target_elements) / len(target_elements)) ** 2 for x in target_elements])
# 行 1455：原始注释行，提供作者或用途说明。
    #         # variance = squared_diff_sum / len(target_elements)
# 行 1456：原始注释行，提供作者或用途说明。
    #         # std_value = math.sqrt(variance)
# 行 1457：原始注释行，提供作者或用途说明。
    #         std_value_up = [sum(target_elements) / len(target_elements) + std_value * float(self.b_amplitude_two.get())] * len(input_list)
# 行 1458：原始注释行，提供作者或用途说明。
    #         std_value_down = [sum(target_elements) / len(target_elements) - std_value * float(self.b_amplitude_two.get())] * len(input_list)
# 行 1459：原始注释行，提供作者或用途说明。
    #         if signal_1 == 2:
# 行 1460：原始注释行，提供作者或用途说明。
    #             if min(input_list) <= std_value_down[0]:
# 行 1461：原始注释行，提供作者或用途说明。
    #                 down = True
# 行 1462：原始注释行，提供作者或用途说明。
    #
# 行 1463：原始注释行，提供作者或用途说明。
    #         elif signal_1 == 1:
# 行 1464：原始注释行，提供作者或用途说明。
    #             if max(input_list) <= std_value_up[0]:
# 行 1465：原始注释行，提供作者或用途说明。
    #                 up = True
# 行 1466：原始注释行，提供作者或用途说明。
    #         else:
# 行 1467：原始注释行，提供作者或用途说明。
    #             down = False
# 行 1468：原始注释行，提供作者或用途说明。
    #             up = False
# 行 1469：原始注释行，提供作者或用途说明。
    #         # std = sum(squared_diff_sum) / len(squared_diff_sum)
# 行 1470：原始注释行，提供作者或用途说明。
    #     else:
# 行 1471：原始注释行，提供作者或用途说明。
    #         down = False
# 行 1472：原始注释行，提供作者或用途说明。
    #         up = False
# 行 1473：原始注释行，提供作者或用途说明。
    #     return down, up
# 行 1474：普通语句：执行一步操作或表达式。
    def event_detection(self, data_x, data_y, baseline_data, std_up_data, std_down_data, signal_direction, sampling,
# 行 1475：代码块开始（如函数、类、条件或循环体）。
                        window_size, min_amplitude, min_duration, max_duration, event_number):
# 行 1476：变量赋值：保存配置、状态或计算结果。
        x_len = 0
# 行 1477：变量赋值：保存配置、状态或计算结果。
        min_duration  = min_duration / 1000
# 行 1478：变量赋值：保存配置、状态或计算结果。
        max_duration = max_duration / 1000
# 行 1479：变量赋值：保存配置、状态或计算结果。
        event_list = defaultdict(list)
# 行 1480：变量赋值：保存配置、状态或计算结果。
        last_start_index = 0
# 行 1481：变量赋值：保存配置、状态或计算结果。
        current_pos = 0
# 行 1482：变量赋值：保存配置、状态或计算结果。
        data_len = len(data_x)
# 行 1483：空行，用于分隔代码块，提高可读性。

# 行 1484：while 循环，满足条件时持续执行。
        while current_pos < data_len - 1:
# 行 1485：变量赋值：保存配置、状态或计算结果。
            rise = 0
# 行 1486：变量赋值：保存配置、状态或计算结果。
            decay = 0
# 行 1487：变量赋值：保存配置、状态或计算结果。
            peak_value = 0
# 行 1488：变量赋值：保存配置、状态或计算结果。
            peak_index = 0
# 行 1489：变量赋值：保存配置、状态或计算结果。
            search_end = min(current_pos + int((window_size / 1000) * sampling), data_len)
# 行 1490：变量赋值：保存配置、状态或计算结果。
            current_window = data_y[current_pos:search_end]
# 行 1491：变量赋值：保存配置、状态或计算结果。
            current_window_x = data_x[current_pos:search_end]
# 行 1492：条件判断，根据不同条件执行不同分支。
            if signal_direction == 1:
# 行 1493：变量赋值：保存配置、状态或计算结果。
                peak_value = max(current_window)
# 行 1494：变量赋值：保存配置、状态或计算结果。
                peak_idx_in_window = np.where(current_window == peak_value)[0][0]
# 行 1495：变量赋值：保存配置、状态或计算结果。
                peak_index = current_pos + peak_idx_in_window
# 行 1496：条件判断，根据不同条件执行不同分支。
            if signal_direction == 2:
# 行 1497：变量赋值：保存配置、状态或计算结果。
                peak_value = min(current_window)
# 行 1498：变量赋值：保存配置、状态或计算结果。
                peak_idx_in_window = np.where(current_window == peak_value)[0][0]
# 行 1499：变量赋值：保存配置、状态或计算结果。
                peak_index = current_pos + peak_idx_in_window
# 行 1500：条件判断，根据不同条件执行不同分支。
            else:
# 行 1501：变量赋值：保存配置、状态或计算结果。
                peak_value_max = max(current_window)
# 行 1502：变量赋值：保存配置、状态或计算结果。
                peak_idx_in_window_max = np.where(current_window == peak_value_max)[0][0]
# 行 1503：变量赋值：保存配置、状态或计算结果。
                peak_index_max = current_pos + peak_idx_in_window_max
# 行 1504：变量赋值：保存配置、状态或计算结果。
                peak_value_min = min(current_window)
# 行 1505：变量赋值：保存配置、状态或计算结果。
                peak_idx_in_window_min = np.where(current_window == peak_value_min)[0][0]
# 行 1506：变量赋值：保存配置、状态或计算结果。
                peak_index_min = current_pos + peak_idx_in_window_min
# 行 1507：空行，用于分隔代码块，提高可读性。

# 行 1508：条件判断，根据不同条件执行不同分支。
            if signal_direction == 1:
# 行 1509：条件判断，根据不同条件执行不同分支。
                if peak_value > std_up_data[peak_index]:
# 行 1510：变量赋值：保存配置、状态或计算结果。
                    event_start_index = None
# 行 1511：for 循环遍历序列或可迭代对象。
                    for index_1 in range(peak_index, max(current_pos - 10000, 0), -1):
# 行 1512：条件判断，根据不同条件执行不同分支。
                        if data_y[index_1] >= baseline_data[index_1] + (peak_value - baseline_data[index_1]) * 0.1:
# 行 1513：变量赋值：保存配置、状态或计算结果。
                            rise = data_x[peak_index] - data_x[index_1]
# 行 1514：变量赋值：保存配置、状态或计算结果。
                            event_start_index = index_1
# 行 1515：普通语句：执行一步操作或表达式。
                            break
# 行 1516：条件判断，根据不同条件执行不同分支。
                    if event_start_index is None:
# 行 1517：变量赋值：保存配置、状态或计算结果。
                            current_pos = peak_index + 1
# 行 1518：普通语句：执行一步操作或表达式。
                            continue
# 行 1519：变量赋值：保存配置、状态或计算结果。
                    event_end_index = None
# 行 1520：for 循环遍历序列或可迭代对象。
                    for index_2 in range(peak_index, min(peak_index + 10000, data_len)):
# 行 1521：条件判断，根据不同条件执行不同分支。
                        if data_y[index_2] >= baseline_data[index_2] + (peak_value - baseline_data[index_2]) * 0.1:
# 行 1522：变量赋值：保存配置、状态或计算结果。
                            decay = data_x[index_2] - data_x[peak_index]
# 行 1523：变量赋值：保存配置、状态或计算结果。
                            event_end_index = index_2 + 1
# 行 1524：普通语句：执行一步操作或表达式。
                            break
# 行 1525：条件判断，根据不同条件执行不同分支。
                    if event_end_index is None:
# 行 1526：变量赋值：保存配置、状态或计算结果。
                        current_pos = peak_index + 1
# 行 1527：普通语句：执行一步操作或表达式。
                        continue
# 行 1528：变量赋值：保存配置、状态或计算结果。
                    amplitude = peak_value - baseline_data[event_start_index]
# 行 1529：变量赋值：保存配置、状态或计算结果。
                    total_duration = rise + decay
# 行 1530：条件判断，根据不同条件执行不同分支。
                    if (amplitude >= min_amplitude and
# 行 1531：普通语句：执行一步操作或表达式。
                            min_duration <= total_duration <= max_duration and
# 行 1532：代码块开始（如函数、类、条件或循环体）。
                            event_start_index != last_start_index):
# 行 1533：变量赋值：保存配置、状态或计算结果。
                        x1 = data_x[event_start_index: event_end_index]
# 行 1534：变量赋值：保存配置、状态或计算结果。
                        x_number = [round(num, 6) for num in x1]
# 行 1535：变量赋值：保存配置、状态或计算结果。
                        y1 = data_y[event_start_index: event_end_index]
# 行 1536：变量赋值：保存配置、状态或计算结果。
                        y_number = [round(num, 6) for num in y1]
# 行 1537：变量赋值：保存配置、状态或计算结果。
                        integral_area = self.integral(y_number, baseline_data[event_start_index], sampling, 1)
# 行 1538：变量赋值：保存配置、状态或计算结果。
                        x2 = data_x[event_start_index]
# 行 1539：变量赋值：保存配置、状态或计算结果。
                        y2 = baseline_data[event_start_index]
# 行 1540：变量赋值：保存配置、状态或计算结果。
                        heng = 0
# 行 1541：变量赋值：保存配置、状态或计算结果。
                        zong = 0
# 行 1542：变量赋值：保存配置、状态或计算结果。
                        theta1 = None
# 行 1543：for 循环遍历序列或可迭代对象。
                        for i1 in range(event_start_index, event_end_index):
# 行 1544：条件判断，根据不同条件执行不同分支。
                            if data_y[i1] >= baseline_data[event_start_index] + 0.7 * amplitude:
# 行 1545：变量赋值：保存配置、状态或计算结果。
                                theta1 = data_x[i1]
# 行 1546：普通语句：执行一步操作或表达式。
                                break
# 行 1547：变量赋值：保存配置、状态或计算结果。
                        theta2 = None
# 行 1548：for 循环遍历序列或可迭代对象。
                        for i2 in range(event_end_index - 1, event_start_index - 1, -1):
# 行 1549：条件判断，根据不同条件执行不同分支。
                            if data_y[i2] >= baseline_data[event_start_index] + 0.7 * amplitude:
# 行 1550：变量赋值：保存配置、状态或计算结果。
                                theta2 = data_x[i2]
# 行 1551：普通语句：执行一步操作或表达式。
                                break
# 行 1552：变量赋值：保存配置、状态或计算结果。
                        theta_duration = (theta2 - theta1) * 1000 if (theta1 and theta2) else 0
# 行 1553：for 循环遍历序列或可迭代对象。
                        for i3 in range(event_start_index, event_end_index - 1):
# 行 1554：普通语句：执行一步操作或表达式。
                            heng += (data_x[i3] - x2) ** 2 * (data_y[i3] - y2)
# 行 1555：for 循环遍历序列或可迭代对象。
                        for i4 in range(event_start_index, peak_index):
# 行 1556：变量赋值：保存配置、状态或计算结果。
                            k = peak_index
# 行 1557：for 循环遍历序列或可迭代对象。
                            for j1 in range(peak_index, event_end_index - 1):
# 行 1558：条件判断，根据不同条件执行不同分支。
                                if data_y[j1] >= data_y[i4]:
# 行 1559：变量赋值：保存配置、状态或计算结果。
                                    k = j1
# 行 1560：普通语句：执行一步操作或表达式。
                                    break
# 行 1561：普通语句：执行一步操作或表达式。
                            zong += (data_y[i4] - y2) ** 2 * (data_x[k] - data_x[i4])
# 行 1562：for 循环遍历序列或可迭代对象。
                        for i5 in range(peak_index, event_end_index - 1):
# 行 1563：变量赋值：保存配置、状态或计算结果。
                            k = event_start_index
# 行 1564：for 循环遍历序列或可迭代对象。
                            for j2 in range(event_start_index, peak_index):
# 行 1565：条件判断，根据不同条件执行不同分支。
                                if data_y[j2] >= data_y[i5]:
# 行 1566：变量赋值：保存配置、状态或计算结果。
                                    k = j2
# 行 1567：普通语句：执行一步操作或表达式。
                                    break
# 行 1568：普通语句：执行一步操作或表达式。
                            zong += (data_y[i5] - y2) ** 2 * (data_x[i5] - data_x[k])
# 行 1569：变量赋值：保存配置、状态或计算结果。
                        rise_integral = self.integral(data_y[event_start_index:peak_index],
# 行 1570：普通语句：执行一步操作或表达式。
                                                      baseline_data[event_start_index], sampling, 1)
# 行 1571：变量赋值：保存配置、状态或计算结果。
                        decay_integral = self.integral(data_y[peak_index:event_end_index],
# 行 1572：普通语句：执行一步操作或表达式。
                                                       baseline_data[event_start_index],
# 行 1573：普通语句：执行一步操作或表达式。
                                                       sampling, 1)
# 行 1574：变量赋值：保存配置、状态或计算结果。
                        integral_ratio = rise_integral / decay_integral if decay_integral != 0 else 0
# 行 1575：普通语句：执行一步操作或表达式。
                        event_list[event_number] += [event_number + 1, f'{baseline_data[index_1]:.4e}',
# 行 1576：普通语句：执行一步操作或表达式。
                                                     f'{(peak_value - baseline_data[index_1]):.4e}',
# 行 1577：普通语句：执行一步操作或表达式。
                                                     f'{(integral_area * 1e9):.4e}',
# 行 1578：普通语句：执行一步操作或表达式。
                                                     float(f'{((rise + decay + 1 / sampling) * 1000):.4e}'),
# 行 1579：普通语句：执行一步操作或表达式。
                                                     theta_duration,
# 行 1580：普通语句：执行一步操作或表达式。
                                                     integral_ratio, np.abs(heng), np.abs(zong), x_number, y_number]
# 行 1581：变量赋值：保存配置、状态或计算结果。
                        current_pos = event_end_index
# 行 1582：变量赋值：保存配置、状态或计算结果。
                        last_start_index = event_start_index
# 行 1583：普通语句：执行一步操作或表达式。
                        event_number += 1
# 行 1584：条件判断，根据不同条件执行不同分支。
                    else:
# 行 1585：变量赋值：保存配置、状态或计算结果。
                        current_pos = peak_index + 1
# 行 1586：条件判断，根据不同条件执行不同分支。
                else:
# 行 1587：变量赋值：保存配置、状态或计算结果。
                    current_pos = search_end
# 行 1588：条件判断，根据不同条件执行不同分支。
            elif signal_direction == 2:
# 行 1589：条件判断，根据不同条件执行不同分支。
                if peak_value < std_down_data[peak_index]:
# 行 1590：变量赋值：保存配置、状态或计算结果。
                    event_start_index = None
# 行 1591：for 循环遍历序列或可迭代对象。
                    for index_1 in range(peak_index, max(current_pos - 10000, 0), -1):
# 行 1592：条件判断，根据不同条件执行不同分支。
                        if data_y[index_1] >= baseline_data[index_1] - (baseline_data[index_1] - peak_value) * 0.1:
# 行 1593：变量赋值：保存配置、状态或计算结果。
                            rise = data_x[peak_index] - data_x[index_1]
# 行 1594：变量赋值：保存配置、状态或计算结果。
                            event_start_index = index_1
# 行 1595：普通语句：执行一步操作或表达式。
                            break
# 行 1596：条件判断，根据不同条件执行不同分支。
                    if event_start_index is None:
# 行 1597：变量赋值：保存配置、状态或计算结果。
                            current_pos = peak_index + 1
# 行 1598：普通语句：执行一步操作或表达式。
                            continue
# 行 1599：变量赋值：保存配置、状态或计算结果。
                    event_end_index = None
# 行 1600：for 循环遍历序列或可迭代对象。
                    for index_2 in range(peak_index, min(peak_index + 10000, data_len)):
# 行 1601：条件判断，根据不同条件执行不同分支。
                        if data_y[index_2] >= baseline_data[index_2] - (baseline_data[index_2] - peak_value) * 0.1:
# 行 1602：变量赋值：保存配置、状态或计算结果。
                            decay = data_x[index_2] - data_x[peak_index]
# 行 1603：变量赋值：保存配置、状态或计算结果。
                            event_end_index = index_2 + 1
# 行 1604：普通语句：执行一步操作或表达式。
                            break
# 行 1605：条件判断，根据不同条件执行不同分支。
                    if event_end_index is None:
# 行 1606：变量赋值：保存配置、状态或计算结果。
                        current_pos = peak_index + 1
# 行 1607：普通语句：执行一步操作或表达式。
                        continue
# 行 1608：变量赋值：保存配置、状态或计算结果。
                    amplitude = baseline_data[event_start_index] - peak_value
# 行 1609：空行，用于分隔代码块，提高可读性。

# 行 1610：变量赋值：保存配置、状态或计算结果。
                    total_duration = rise + decay
# 行 1611：条件判断，根据不同条件执行不同分支。
                    if (amplitude >= min_amplitude and
# 行 1612：普通语句：执行一步操作或表达式。
                            min_duration <= total_duration <= max_duration and
# 行 1613：代码块开始（如函数、类、条件或循环体）。
                            event_start_index != last_start_index):
# 行 1614：变量赋值：保存配置、状态或计算结果。
                        x1 = data_x[event_start_index: event_end_index]
# 行 1615：变量赋值：保存配置、状态或计算结果。
                        x_number = [round(num, 6) for num in x1]
# 行 1616：变量赋值：保存配置、状态或计算结果。
                        y1 = data_y[event_start_index: event_end_index]
# 行 1617：变量赋值：保存配置、状态或计算结果。
                        y_number = [round(num, 6) for num in y1]
# 行 1618：变量赋值：保存配置、状态或计算结果。
                        integral_area = self.integral(y_number, baseline_data[event_start_index], sampling, 0)
# 行 1619：变量赋值：保存配置、状态或计算结果。
                        x2 = data_x[event_start_index]
# 行 1620：变量赋值：保存配置、状态或计算结果。
                        y2 = baseline_data[event_start_index]
# 行 1621：变量赋值：保存配置、状态或计算结果。
                        heng = 0
# 行 1622：变量赋值：保存配置、状态或计算结果。
                        zong = 0
# 行 1623：变量赋值：保存配置、状态或计算结果。
                        theta1 = None
# 行 1624：for 循环遍历序列或可迭代对象。
                        for i1 in range(event_start_index, event_end_index):
# 行 1625：条件判断，根据不同条件执行不同分支。
                            if data_y[i1] <= baseline_data[event_start_index] + 0.7 * amplitude:
# 行 1626：变量赋值：保存配置、状态或计算结果。
                                theta1 = data_x[i1]
# 行 1627：普通语句：执行一步操作或表达式。
                                break
# 行 1628：变量赋值：保存配置、状态或计算结果。
                        theta2 = None
# 行 1629：for 循环遍历序列或可迭代对象。
                        for i2 in range(event_end_index - 1, event_start_index - 1, -1):
# 行 1630：条件判断，根据不同条件执行不同分支。
                            if data_y[i2] <= baseline_data[event_start_index] + 0.7 * amplitude:
# 行 1631：变量赋值：保存配置、状态或计算结果。
                                theta2 = data_x[i2]
# 行 1632：普通语句：执行一步操作或表达式。
                                break
# 行 1633：变量赋值：保存配置、状态或计算结果。
                        theta_duration = (theta2 - theta1) * 1000 if (theta1 and theta2) else 0
# 行 1634：for 循环遍历序列或可迭代对象。
                        for i3 in range(event_start_index, event_end_index - 1):
# 行 1635：普通语句：执行一步操作或表达式。
                            heng += (data_x[i3] - x2) ** 2 * (data_y[i3] - y2)
# 行 1636：for 循环遍历序列或可迭代对象。
                        for i4 in range(event_start_index, peak_index):
# 行 1637：变量赋值：保存配置、状态或计算结果。
                            k = peak_index
# 行 1638：for 循环遍历序列或可迭代对象。
                            for j1 in range(peak_index, event_end_index - 1):
# 行 1639：条件判断，根据不同条件执行不同分支。
                                if data_y[j1] >= data_y[i4]:
# 行 1640：变量赋值：保存配置、状态或计算结果。
                                    k = j1
# 行 1641：普通语句：执行一步操作或表达式。
                                    break
# 行 1642：普通语句：执行一步操作或表达式。
                            zong += (data_y[i4] - y2) ** 2 * (data_x[k] - data_x[i4])
# 行 1643：for 循环遍历序列或可迭代对象。
                        for i5 in range(peak_index, event_end_index - 1):
# 行 1644：变量赋值：保存配置、状态或计算结果。
                            k = event_start_index
# 行 1645：for 循环遍历序列或可迭代对象。
                            for j2 in range(event_start_index, peak_index):
# 行 1646：条件判断，根据不同条件执行不同分支。
                                if data_y[j2] >= data_y[i5]:
# 行 1647：变量赋值：保存配置、状态或计算结果。
                                    k = j2
# 行 1648：普通语句：执行一步操作或表达式。
                                    break
# 行 1649：普通语句：执行一步操作或表达式。
                            zong += (data_y[i5] - y2) ** 2 * (data_x[i5] - data_x[k])
# 行 1650：变量赋值：保存配置、状态或计算结果。
                        rise_integral = self.integral(data_y[event_start_index:peak_index],
# 行 1651：普通语句：执行一步操作或表达式。
                                                      baseline_data[event_start_index], sampling, 0)
# 行 1652：变量赋值：保存配置、状态或计算结果。
                        decay_integral = self.integral(data_y[peak_index:event_end_index],
# 行 1653：普通语句：执行一步操作或表达式。
                                                       baseline_data[event_start_index],
# 行 1654：普通语句：执行一步操作或表达式。
                                                       sampling, 0)
# 行 1655：变量赋值：保存配置、状态或计算结果。
                        integral_ratio = rise_integral / decay_integral if decay_integral != 0 else 0
# 行 1656：普通语句：执行一步操作或表达式。
                        event_list[event_number] += [event_number + 1, f'{baseline_data[index_1]:.4e}',
# 行 1657：普通语句：执行一步操作或表达式。
                                                     f'{(baseline_data[index_1] - peak_value):.4e}',
# 行 1658：普通语句：执行一步操作或表达式。
                                                     f'{(integral_area * 1e9):.4e}',
# 行 1659：普通语句：执行一步操作或表达式。
                                                     float(f'{((rise + decay + 1 / sampling) * 1000):.4e}'),
# 行 1660：普通语句：执行一步操作或表达式。
                                                     theta_duration,
# 行 1661：普通语句：执行一步操作或表达式。
                                                     integral_ratio, np.abs(heng), np.abs(zong), x_number, y_number]
# 行 1662：变量赋值：保存配置、状态或计算结果。
                        current_pos = event_end_index
# 行 1663：变量赋值：保存配置、状态或计算结果。
                        last_start_index = event_start_index
# 行 1664：普通语句：执行一步操作或表达式。
                        event_number += 1
# 行 1665：普通语句：执行一步操作或表达式。
                        x_len += len(x_number)
# 行 1666：条件判断，根据不同条件执行不同分支。
                    else:
# 行 1667：变量赋值：保存配置、状态或计算结果。
                        current_pos = peak_index + 1
# 行 1668：条件判断，根据不同条件执行不同分支。
                else:
# 行 1669：变量赋值：保存配置、状态或计算结果。
                    current_pos = search_end
# 行 1670：空行，用于分隔代码块，提高可读性。

# 行 1671：条件判断，根据不同条件执行不同分支。
            else:
# 行 1672：条件判断，根据不同条件执行不同分支。
                if peak_value_max > std_up_data[peak_index_max]:
# 行 1673：变量赋值：保存配置、状态或计算结果。
                    event_start_index = None
# 行 1674：for 循环遍历序列或可迭代对象。
                    for index_1 in range(peak_index_max, max(current_pos - 10000, 0), -1):
# 行 1675：条件判断，根据不同条件执行不同分支。
                        if data_y[index_1] <= baseline_data[index_1] + (peak_value_max - baseline_data[index_1]) * 0.1:
# 行 1676：变量赋值：保存配置、状态或计算结果。
                            rise = data_x[peak_index_max] - data_x[index_1]
# 行 1677：变量赋值：保存配置、状态或计算结果。
                            event_start_index = index_1
# 行 1678：普通语句：执行一步操作或表达式。
                            break
# 行 1679：条件判断，根据不同条件执行不同分支。
                    if event_start_index is None:
# 行 1680：变量赋值：保存配置、状态或计算结果。
                            current_pos = peak_index_max + 1
# 行 1681：普通语句：执行一步操作或表达式。
                            continue
# 行 1682：变量赋值：保存配置、状态或计算结果。
                    event_end_index = None
# 行 1683：for 循环遍历序列或可迭代对象。
                    for index_2 in range(peak_index_max, min(peak_index_max + 10000, data_len)):
# 行 1684：条件判断，根据不同条件执行不同分支。
                        if data_y[index_2] <= baseline_data[index_2] + (peak_value_max - baseline_data[index_2]) * 0.1:
# 行 1685：变量赋值：保存配置、状态或计算结果。
                            decay = data_x[index_2] - data_x[peak_index_max]
# 行 1686：变量赋值：保存配置、状态或计算结果。
                            event_end_index = index_2 + 1
# 行 1687：普通语句：执行一步操作或表达式。
                            break
# 行 1688：条件判断，根据不同条件执行不同分支。
                    if event_end_index is None:
# 行 1689：变量赋值：保存配置、状态或计算结果。
                        current_pos = peak_index_max + 1
# 行 1690：普通语句：执行一步操作或表达式。
                        continue
# 行 1691：变量赋值：保存配置、状态或计算结果。
                    amplitude = peak_value_max - baseline_data[event_start_index]
# 行 1692：变量赋值：保存配置、状态或计算结果。
                    total_duration = rise + decay
# 行 1693：条件判断，根据不同条件执行不同分支。
                    if (amplitude >= min_amplitude and
# 行 1694：普通语句：执行一步操作或表达式。
                            min_duration <= total_duration <= max_duration and
# 行 1695：代码块开始（如函数、类、条件或循环体）。
                            event_start_index != last_start_index):
# 行 1696：变量赋值：保存配置、状态或计算结果。
                        x1 = data_x[event_start_index: event_end_index]
# 行 1697：变量赋值：保存配置、状态或计算结果。
                        x_number = [round(num, 6) for num in x1]
# 行 1698：变量赋值：保存配置、状态或计算结果。
                        y1 = data_y[event_start_index: event_end_index]
# 行 1699：变量赋值：保存配置、状态或计算结果。
                        y_number = [round(num, 6) for num in y1]
# 行 1700：变量赋值：保存配置、状态或计算结果。
                        integral_area = self.integral(y_number, baseline_data[event_start_index], sampling, 1)
# 行 1701：变量赋值：保存配置、状态或计算结果。
                        x2 = data_x[event_start_index]
# 行 1702：变量赋值：保存配置、状态或计算结果。
                        y2 = baseline_data[event_start_index]
# 行 1703：变量赋值：保存配置、状态或计算结果。
                        heng = 0
# 行 1704：变量赋值：保存配置、状态或计算结果。
                        zong = 0
# 行 1705：变量赋值：保存配置、状态或计算结果。
                        theta1 = None
# 行 1706：for 循环遍历序列或可迭代对象。
                        for i1 in range(event_start_index, event_end_index):
# 行 1707：条件判断，根据不同条件执行不同分支。
                            if data_y[i1] >= baseline_data[event_start_index] + 0.7 * amplitude:
# 行 1708：变量赋值：保存配置、状态或计算结果。
                                theta1 = data_x[i1]
# 行 1709：普通语句：执行一步操作或表达式。
                                break
# 行 1710：变量赋值：保存配置、状态或计算结果。
                        theta2 = None
# 行 1711：for 循环遍历序列或可迭代对象。
                        for i2 in range(event_end_index - 1, event_start_index - 1, -1):
# 行 1712：条件判断，根据不同条件执行不同分支。
                            if data_y[i2] >= baseline_data[event_start_index] + 0.7 * amplitude:
# 行 1713：变量赋值：保存配置、状态或计算结果。
                                theta2 = data_x[i2]
# 行 1714：普通语句：执行一步操作或表达式。
                                break
# 行 1715：变量赋值：保存配置、状态或计算结果。
                        theta_duration = (theta2 - theta1) * 1000 if (theta1 and theta2) else 0
# 行 1716：for 循环遍历序列或可迭代对象。
                        for i3 in range(event_start_index, event_end_index - 1):
# 行 1717：普通语句：执行一步操作或表达式。
                            heng += (data_x[i3] - x2) ** 2 * (data_y[i3] - y2)
# 行 1718：for 循环遍历序列或可迭代对象。
                        for i4 in range(event_start_index, peak_index_max):
# 行 1719：变量赋值：保存配置、状态或计算结果。
                            k = peak_index_max
# 行 1720：for 循环遍历序列或可迭代对象。
                            for j1 in range(peak_index_max, event_end_index - 1):
# 行 1721：条件判断，根据不同条件执行不同分支。
                                if data_y[j1] >= data_y[i4]:
# 行 1722：变量赋值：保存配置、状态或计算结果。
                                    k = j1
# 行 1723：普通语句：执行一步操作或表达式。
                                    break
# 行 1724：普通语句：执行一步操作或表达式。
                            zong += (data_y[i4] - y2) ** 2 * (data_x[k] - data_x[i4])
# 行 1725：for 循环遍历序列或可迭代对象。
                        for i5 in range(peak_index_max, event_end_index - 1):
# 行 1726：变量赋值：保存配置、状态或计算结果。
                            k = event_start_index
# 行 1727：for 循环遍历序列或可迭代对象。
                            for j2 in range(event_start_index, peak_index_max):
# 行 1728：条件判断，根据不同条件执行不同分支。
                                if data_y[j2] >= data_y[i5]:
# 行 1729：变量赋值：保存配置、状态或计算结果。
                                    k = j2
# 行 1730：普通语句：执行一步操作或表达式。
                                    break
# 行 1731：普通语句：执行一步操作或表达式。
                            zong += (data_y[i5] - y2) ** 2 * (data_x[i5] - data_x[k])
# 行 1732：变量赋值：保存配置、状态或计算结果。
                        rise_integral = self.integral(data_y[event_start_index:peak_index_max],
# 行 1733：普通语句：执行一步操作或表达式。
                                                      baseline_data[event_start_index], sampling, 1)
# 行 1734：变量赋值：保存配置、状态或计算结果。
                        decay_integral = self.integral(data_y[peak_index_max:event_end_index],
# 行 1735：普通语句：执行一步操作或表达式。
                                                       baseline_data[event_start_index],
# 行 1736：普通语句：执行一步操作或表达式。
                                                       sampling, 1)
# 行 1737：变量赋值：保存配置、状态或计算结果。
                        integral_ratio = rise_integral / decay_integral if decay_integral != 0 else 0
# 行 1738：普通语句：执行一步操作或表达式。
                        event_list[event_number] += [event_number + 1, f'{baseline_data[index_1]:.4e}',
# 行 1739：普通语句：执行一步操作或表达式。
                                                     f'{(peak_value_max - baseline_data[index_1]):.4e}',
# 行 1740：普通语句：执行一步操作或表达式。
                                                     f'{(integral_area * 1e9):.4e}',
# 行 1741：普通语句：执行一步操作或表达式。
                                                     float(f'{((rise + decay + 1 / sampling) * 1000):.4e}'),
# 行 1742：普通语句：执行一步操作或表达式。
                                                     theta_duration,
# 行 1743：普通语句：执行一步操作或表达式。
                                                     integral_ratio, np.abs(heng), np.abs(zong), x_number, y_number]
# 行 1744：变量赋值：保存配置、状态或计算结果。
                        current_pos = event_end_index
# 行 1745：变量赋值：保存配置、状态或计算结果。
                        last_start_index = event_start_index
# 行 1746：普通语句：执行一步操作或表达式。
                        event_number += 1
# 行 1747：条件判断，根据不同条件执行不同分支。
                    else:
# 行 1748：变量赋值：保存配置、状态或计算结果。
                        current_pos = peak_index_max + 1
# 行 1749：空行，用于分隔代码块，提高可读性。

# 行 1750：条件判断，根据不同条件执行不同分支。
                if peak_value_min < std_down_data[peak_index_min]:
# 行 1751：变量赋值：保存配置、状态或计算结果。
                    event_start_index = None
# 行 1752：for 循环遍历序列或可迭代对象。
                    for index_1 in range(peak_index_min, max(current_pos - 10000, 0), -1):
# 行 1753：条件判断，根据不同条件执行不同分支。
                        if data_y[index_1] >= baseline_data[index_1] + (baseline_data[index_1] - peak_value_min) * 0.1:
# 行 1754：变量赋值：保存配置、状态或计算结果。
                            rise = data_x[peak_index_min] - data_x[index_1]
# 行 1755：变量赋值：保存配置、状态或计算结果。
                            event_start_index = index_1
# 行 1756：普通语句：执行一步操作或表达式。
                            break
# 行 1757：条件判断，根据不同条件执行不同分支。
                    if event_start_index is None:
# 行 1758：变量赋值：保存配置、状态或计算结果。
                            current_pos = peak_index_min + 1
# 行 1759：普通语句：执行一步操作或表达式。
                            continue
# 行 1760：变量赋值：保存配置、状态或计算结果。
                    event_end_index = None
# 行 1761：for 循环遍历序列或可迭代对象。
                    for index_2 in range(peak_index_min, min(peak_index_min + 10000, data_len)):
# 行 1762：条件判断，根据不同条件执行不同分支。
                        if data_y[index_2] >= baseline_data[index_2] + (peak_value_min - baseline_data[index_2]) * 0.1:
# 行 1763：变量赋值：保存配置、状态或计算结果。
                            decay = data_x[index_2] - data_x[peak_index_min]
# 行 1764：变量赋值：保存配置、状态或计算结果。
                            event_end_index = index_2 + 1
# 行 1765：普通语句：执行一步操作或表达式。
                            break
# 行 1766：条件判断，根据不同条件执行不同分支。
                    if event_end_index is None:
# 行 1767：变量赋值：保存配置、状态或计算结果。
                        current_pos = peak_index_min + 1
# 行 1768：普通语句：执行一步操作或表达式。
                        continue
# 行 1769：变量赋值：保存配置、状态或计算结果。
                    amplitude = baseline_data[event_start_index] - peak_value_min
# 行 1770：变量赋值：保存配置、状态或计算结果。
                    total_duration = rise + decay
# 行 1771：条件判断，根据不同条件执行不同分支。
                    if (amplitude >= min_amplitude and
# 行 1772：普通语句：执行一步操作或表达式。
                            min_duration <= total_duration <= max_duration and
# 行 1773：代码块开始（如函数、类、条件或循环体）。
                            event_start_index != last_start_index):
# 行 1774：变量赋值：保存配置、状态或计算结果。
                        x1 = data_x[event_start_index: event_end_index]
# 行 1775：变量赋值：保存配置、状态或计算结果。
                        x_number = [round(num, 6) for num in x1]
# 行 1776：变量赋值：保存配置、状态或计算结果。
                        y1 = data_y[event_start_index: event_end_index]
# 行 1777：变量赋值：保存配置、状态或计算结果。
                        y_number = [round(num, 6) for num in y1]
# 行 1778：变量赋值：保存配置、状态或计算结果。
                        integral_area = self.integral(y_number, baseline_data[event_start_index], sampling, 0)
# 行 1779：变量赋值：保存配置、状态或计算结果。
                        x2 = data_x[event_start_index]
# 行 1780：变量赋值：保存配置、状态或计算结果。
                        y2 = baseline_data[event_start_index]
# 行 1781：变量赋值：保存配置、状态或计算结果。
                        heng = 0
# 行 1782：变量赋值：保存配置、状态或计算结果。
                        zong = 0
# 行 1783：变量赋值：保存配置、状态或计算结果。
                        theta1 = None
# 行 1784：for 循环遍历序列或可迭代对象。
                        for i1 in range(event_start_index, event_end_index):
# 行 1785：条件判断，根据不同条件执行不同分支。
                            if data_y[i1] <= baseline_data[event_start_index] + 0.7 * amplitude:
# 行 1786：变量赋值：保存配置、状态或计算结果。
                                theta1 = data_x[i1]
# 行 1787：普通语句：执行一步操作或表达式。
                                break
# 行 1788：变量赋值：保存配置、状态或计算结果。
                        theta2 = None
# 行 1789：for 循环遍历序列或可迭代对象。
                        for i2 in range(event_end_index - 1, event_start_index - 1, -1):
# 行 1790：条件判断，根据不同条件执行不同分支。
                            if data_y[i2] <= baseline_data[event_start_index] + 0.7 * amplitude:
# 行 1791：变量赋值：保存配置、状态或计算结果。
                                theta2 = data_x[i2]
# 行 1792：普通语句：执行一步操作或表达式。
                                break
# 行 1793：变量赋值：保存配置、状态或计算结果。
                        theta_duration = (theta2 - theta1) * 1000 if (theta1 and theta2) else 0
# 行 1794：for 循环遍历序列或可迭代对象。
                        for i3 in range(event_start_index, event_end_index - 1):
# 行 1795：普通语句：执行一步操作或表达式。
                            heng += (data_x[i3] - x2) ** 2 * (data_y[i3] - y2)
# 行 1796：for 循环遍历序列或可迭代对象。
                        for i4 in range(event_start_index, peak_index_min):
# 行 1797：变量赋值：保存配置、状态或计算结果。
                            k = peak_index_min
# 行 1798：for 循环遍历序列或可迭代对象。
                            for j1 in range(peak_index_min, event_end_index - 1):
# 行 1799：条件判断，根据不同条件执行不同分支。
                                if data_y[j1] >= data_y[i4]:
# 行 1800：变量赋值：保存配置、状态或计算结果。
                                    k = j1
# 行 1801：普通语句：执行一步操作或表达式。
                                    break
# 行 1802：普通语句：执行一步操作或表达式。
                            zong += (data_y[i4] - y2) ** 2 * (data_x[k] - data_x[i4])
# 行 1803：for 循环遍历序列或可迭代对象。
                        for i5 in range(peak_index_min, event_end_index - 1):
# 行 1804：变量赋值：保存配置、状态或计算结果。
                            k = event_start_index
# 行 1805：for 循环遍历序列或可迭代对象。
                            for j2 in range(event_start_index, peak_index_min):
# 行 1806：条件判断，根据不同条件执行不同分支。
                                if data_y[j2] >= data_y[i5]:
# 行 1807：变量赋值：保存配置、状态或计算结果。
                                    k = j2
# 行 1808：普通语句：执行一步操作或表达式。
                                    break
# 行 1809：普通语句：执行一步操作或表达式。
                            zong += (data_y[i5] - y2) ** 2 * (data_x[i5] - data_x[k])
# 行 1810：变量赋值：保存配置、状态或计算结果。
                        rise_integral = self.integral(data_y[event_start_index:peak_index_min],
# 行 1811：普通语句：执行一步操作或表达式。
                                                      baseline_data[event_start_index], sampling, 0)
# 行 1812：变量赋值：保存配置、状态或计算结果。
                        decay_integral = self.integral(data_y[peak_index_min:event_end_index],
# 行 1813：普通语句：执行一步操作或表达式。
                                                       baseline_data[event_start_index],
# 行 1814：普通语句：执行一步操作或表达式。
                                                       sampling, 0)
# 行 1815：变量赋值：保存配置、状态或计算结果。
                        integral_ratio = rise_integral / decay_integral if decay_integral != 0 else 0
# 行 1816：普通语句：执行一步操作或表达式。
                        event_list[event_number] += [event_number + 1, f'{baseline_data[index_1]:.4e}',
# 行 1817：普通语句：执行一步操作或表达式。
                                                     f'{(baseline_data[index_1] - peak_value_min):.4e}',
# 行 1818：普通语句：执行一步操作或表达式。
                                                     f'{(integral_area * 1e9):.4e}',
# 行 1819：普通语句：执行一步操作或表达式。
                                                     float(f'{((rise + decay + 1 / sampling) * 1000):.4e}'),
# 行 1820：普通语句：执行一步操作或表达式。
                                                     theta_duration,
# 行 1821：普通语句：执行一步操作或表达式。
                                                     integral_ratio, np.abs(heng), np.abs(zong), x_number, y_number]
# 行 1822：变量赋值：保存配置、状态或计算结果。
                        current_pos = event_end_index
# 行 1823：变量赋值：保存配置、状态或计算结果。
                        last_start_index = event_start_index
# 行 1824：普通语句：执行一步操作或表达式。
                        event_number += 1
# 行 1825：条件判断，根据不同条件执行不同分支。
                    else:
# 行 1826：变量赋值：保存配置、状态或计算结果。
                        current_pos = peak_index_min + 1
# 行 1827：条件判断，根据不同条件执行不同分支。
                else:
# 行 1828：变量赋值：保存配置、状态或计算结果。
                    current_pos = search_end
# 行 1829：返回函数结果。
        return event_list, event_number
# 行 1830：空行，用于分隔代码块，提高可读性。

# 行 1831：定义函数/方法 integral(self, y, baseline, sampling, direction)。
    def integral(self, y, baseline, sampling, direction):
# 行 1832：变量赋值：保存配置、状态或计算结果。
        integral_area = 0
# 行 1833：条件判断，根据不同条件执行不同分支。
        if direction == 1:
# 行 1834：for 循环遍历序列或可迭代对象。
            for i in range(len(y) - 1):
# 行 1835：条件判断，根据不同条件执行不同分支。
                if i >= 0 and i + 1 < len(y):
# 行 1836：变量赋值：保存配置、状态或计算结果。
                    area = (1 / 2) * (max(y[i], y[i + 1]) - min(y[i], y[i + 1])) * (1 / sampling) + (
# 行 1837：普通语句：执行一步操作或表达式。
                            (min(y[i], y[i + 1]) - baseline) * (1 / sampling))
# 行 1838：普通语句：执行一步操作或表达式。
                    integral_area += area
# 行 1839：条件判断，根据不同条件执行不同分支。
                else:
# 行 1840：占位语句：暂不执行任何操作。
                    pass
# 行 1841：条件判断，根据不同条件执行不同分支。
        else:
# 行 1842：for 循环遍历序列或可迭代对象。
            for i in range(len(y) - 1):
# 行 1843：条件判断，根据不同条件执行不同分支。
                if i >= 0 and i + 1 < len(y):
# 行 1844：变量赋值：保存配置、状态或计算结果。
                    area = (1 / 2) * (max(y[i], y[i + 1]) - min(y[i], y[i + 1])) * (1 / sampling) + (
# 行 1845：普通语句：执行一步操作或表达式。
                            (baseline - min(y[i], y[i + 1])) * (1 / sampling))
# 行 1846：普通语句：执行一步操作或表达式。
                    integral_area += area
# 行 1847：条件判断，根据不同条件执行不同分支。
                else:
# 行 1848：占位语句：暂不执行任何操作。
                    pass
# 行 1849：返回函数结果。
        return np.abs(integral_area)
# 行 1850：空行，用于分隔代码块，提高可读性。

# 行 1851：定义函数/方法 test_tree(self, data)。
    def test_tree(self, data):
# 行 1852：变量赋值：保存配置、状态或计算结果。
        item = self.tree.insert('', tk.END, values=data)
# 行 1853：条件判断，根据不同条件执行不同分支。
        if self.b_scroll_param == False:
# 行 1854：占位语句：暂不执行任何操作。
            pass
# 行 1855：条件判断，根据不同条件执行不同分支。
        else:
# 行 1856：普通语句：执行一步操作或表达式。
            self.tree.see(item)
# 行 1857：原始注释行，提供作者或用途说明。
            # self.tree.update_idletasks()
# 行 1858：原始注释行，提供作者或用途说明。
            # 强制滚动
# 行 1859：原始注释行，提供作者或用途说明。
            # self.tree.yview_moveto(1.0)
# 行 1860：空行，用于分隔代码块，提高可读性。

# 行 1861：空行，用于分隔代码块，提高可读性。

# 行 1862：定义函数/方法 save_event_list_to_csv(self, number, event_list, csv_file, number1)。
    def save_event_list_to_csv(self, number, event_list, csv_file, number1):
# 行 1863：条件判断，根据不同条件执行不同分支。
        if number1 == 1:
# 行 1864：使用 with 上下文安全地打开文件，结束时自动关闭。
            with open(csv_file, mode='a', newline='') as file:
# 行 1865：变量赋值：保存配置、状态或计算结果。
                event_list_writer = [number, event_list[1], event_list[2],
# 行 1866：普通语句：执行一步操作或表达式。
                                     event_list[3], event_list[4], event_list[5],
# 行 1867：普通语句：执行一步操作或表达式。
                                     event_list[6]
# 行 1868：普通语句：执行一步操作或表达式。
                    , event_list[7], event_list[8], ', '.join(map(str, event_list[9])),
# 行 1869：普通语句：执行一步操作或表达式。
                                     ', '.join(map(str, event_list[10]))
# 行 1870：普通语句：执行一步操作或表达式。
                                     ]
# 行 1871：变量赋值：保存配置、状态或计算结果。
                writer = csv.writer(file)
# 行 1872：普通语句：执行一步操作或表达式。
                writer.writerow(event_list_writer)
# 行 1873：条件判断，根据不同条件执行不同分支。
        else:
# 行 1874：使用 with 上下文安全地打开文件，结束时自动关闭。
            with open(csv_file, mode='a', newline='') as file:
# 行 1875：变量赋值：保存配置、状态或计算结果。
                event_list_writer = [number, event_list[1],
# 行 1876：普通语句：执行一步操作或表达式。
                                     event_list[2],
# 行 1877：普通语句：执行一步操作或表达式。
                                     event_list[3],
# 行 1878：普通语句：执行一步操作或表达式。
                                     event_list[4],
# 行 1879：普通语句：执行一步操作或表达式。
                                     event_list[5],
# 行 1880：普通语句：执行一步操作或表达式。
                                     event_list[6]
# 行 1881：普通语句：执行一步操作或表达式。
                    , event_list[7], event_list[8],
# 行 1882：普通语句：执行一步操作或表达式。
                                     ', '.join(map(str, event_list[9])),
# 行 1883：普通语句：执行一步操作或表达式。
                                     ', '.join(map(str, event_list[10]))
# 行 1884：普通语句：执行一步操作或表达式。
                                     ]
# 行 1885：变量赋值：保存配置、状态或计算结果。
                writer = csv.writer(file)
# 行 1886：普通语句：执行一步操作或表达式。
                writer.writerow(event_list_writer)
# 行 1887：定义函数/方法 data_processing_thread(self, data, while_time)。
    def data_processing_thread(self, data, while_time):
# 行 1888：普通语句：执行一步操作或表达式。
        global data_lock
# 行 1889：空行，用于分隔代码块，提高可读性。

# 行 1890：变量赋值：保存配置、状态或计算结果。
        np_buffer = np.array(data, copy=False)
# 行 1891：变量赋值：保存配置、状态或计算结果。
        data_matrix = np_buffer.reshape((-1, self.b_total_channels)).transpose()
# 行 1892：变量赋值：保存配置、状态或计算结果。
        self.b_v_data_while = data_matrix[0].astype(np.int16) * self.v_m
# 行 1893：变量赋值：保存配置、状态或计算结果。
        self.b_i_data_while = data_matrix[self.b_v_channels].astype(np.int16) * self.i_m
# 行 1894：空行，用于分隔代码块，提高可读性。

# 行 1895：条件判断，根据不同条件执行不同分支。
        if self.b_last_current_range_selected == '200 pA':
# 行 1896：占位语句：暂不执行任何操作。
            pass
# 行 1897：条件判断，根据不同条件执行不同分支。
        if self.b_last_current_range_selected == '2 nA':
# 行 1898：变量赋值：保存配置、状态或计算结果。
            self.b_i_data_while = [(ad / 100) for ad in self.b_i_data_while]
# 行 1899：条件判断，根据不同条件执行不同分支。
        if self.b_last_current_range_selected == '20 nA':
# 行 1900：变量赋值：保存配置、状态或计算结果。
            self.b_i_data_while = [(ad / 10) for ad in self.b_i_data_while]
# 行 1901：条件判断，根据不同条件执行不同分支。
        if self.b_last_current_range_selected == '200 nA':
# 行 1902：占位语句：暂不执行任何操作。
            pass
# 行 1903：空行，用于分隔代码块，提高可读性。

# 行 1904：空行，用于分隔代码块，提高可读性。

# 行 1905：条件判断，根据不同条件执行不同分支。
        if self.b_filter == False:
# 行 1906：占位语句：暂不执行任何操作。
            pass
# 行 1907：条件判断，根据不同条件执行不同分支。
        else:
# 行 1908：变量赋值：保存配置、状态或计算结果。
            cutoff_frequency = self.b_filter_parameter  # 截止频率
# 行 1909：变量赋值：保存配置、状态或计算结果。
            nyquist_frequency = 0.5 * self.b_frequency  # 奈奎斯特频率
# 行 1910：变量赋值：保存配置、状态或计算结果。
            normal_cutoff = cutoff_frequency / nyquist_frequency
# 行 1911：普通语句：执行一步操作或表达式。
            b, a = signal.butter(1, normal_cutoff, btype='low', analog=False)
# 行 1912：空行，用于分隔代码块，提高可读性。

# 行 1913：变量赋值：保存配置、状态或计算结果。
            padding_length = 300
# 行 1914：变量赋值：保存配置、状态或计算结果。
            padded = np.pad(self.b_i_data_while, (padding_length, padding_length), mode='symmetric')  # 镜像延拓
# 行 1915：空行，用于分隔代码块，提高可读性。

# 行 1916：变量赋值：保存配置、状态或计算结果。
            filtered_padded = signal.filtfilt(b, a, padded)
# 行 1917：空行，用于分隔代码块，提高可读性。

# 行 1918：变量赋值：保存配置、状态或计算结果。
            self.b_i_data_while = filtered_padded[padding_length:-padding_length]
# 行 1919：空行，用于分隔代码块，提高可读性。

# 行 1920：变量赋值：保存配置、状态或计算结果。
        self.b_end_time = time.perf_counter()
# 行 1921：空行，用于分隔代码块，提高可读性。

# 行 1922：变量赋值：保存配置、状态或计算结果。
        step = (self.b_end_time - while_time) / len(self.b_i_data_while)
# 行 1923：变量赋值：保存配置、状态或计算结果。
        self.b_t_data_while = np.arange(while_time - self.b_start_time, self.b_end_time - self.b_start_time,
# 行 1924：普通语句：执行一步操作或表达式。
                                        step)
# 行 1925：条件判断，根据不同条件执行不同分支。
        if len(self.b_t_data_while) < len(self.b_i_data_while):
# 行 1926：变量赋值：保存配置、状态或计算结果。
            self.b_i_data_while = self.b_i_data_while[:len(self.b_t_data_while)]
# 行 1927：条件判断，根据不同条件执行不同分支。
        elif len(self.b_t_data_while) > len(self.b_i_data_while):
# 行 1928：变量赋值：保存配置、状态或计算结果。
            self.b_t_data_while = self.b_t_data_while[:len(self.b_i_data_while)]
# 行 1929：空行，用于分隔代码块，提高可读性。

# 行 1930：空行，用于分隔代码块，提高可读性。

# 行 1931：空行，用于分隔代码块，提高可读性。

# 行 1932：条件判断，根据不同条件执行不同分支。
        if len(self.b_i_data_cache) <= self.b_frequency * 1:
# 行 1933：变量赋值：保存配置、状态或计算结果。
            self.b_i_data_cache = np.concatenate((self.b_i_data_cache, self.b_i_data_while))
# 行 1934：变量赋值：保存配置、状态或计算结果。
            self.b_t_data_cache = np.concatenate((self.b_t_data_cache, self.b_t_data_while))
# 行 1935：条件判断，根据不同条件执行不同分支。
        else:
# 行 1936：普通语句：执行一步操作或表达式。
            self.b_std_data_while_down, self.b_std_data_while_up = np.array(
# 行 1937：普通语句：执行一步操作或表达式。
                dynamic_detection.detection_y_py(self.b_i_data_cache, (int(self.b_detection_baseline_std_window_param.get()) / 1000) * self.b_frequency,
# 行 1938：普通语句：执行一步操作或表达式。
                                                 self.b_signal_number))
# 行 1939：变量赋值：保存配置、状态或计算结果。
            self.b_baseline_data_while = dynamic_detection.calculate_dynamic_baseline_fast_trimmed_py(
# 行 1940：普通语句：执行一步操作或表达式。
                self.b_i_data_cache,
# 行 1941：普通语句：执行一步操作或表达式。
                (int(self.b_detection_baseline_std_window_param.get()) / 1000) * self.b_frequency)
# 行 1942：代码块开始（如函数、类、条件或循环体）。
            with data_lock:
# 行 1943：条件判断，根据不同条件执行不同分支。
                if len(self.b_std_data_down) <= self.b_frequency * 5:
# 行 1944：变量赋值：保存配置、状态或计算结果。
                    self.b_t_data_data_4 = np.concatenate((self.b_t_data_data_4, self.b_t_data_cache))
# 行 1945：变量赋值：保存配置、状态或计算结果。
                    self.b_std_data_down = np.concatenate((self.b_std_data_down, self.b_std_data_while_down))
# 行 1946：变量赋值：保存配置、状态或计算结果。
                    self.b_std_data_up = np.concatenate((self.b_std_data_up, self.b_std_data_while_up))
# 行 1947：变量赋值：保存配置、状态或计算结果。
                    self.b_baseline_data = np.concatenate((self.b_baseline_data, self.b_baseline_data_while))
# 行 1948：条件判断，根据不同条件执行不同分支。
                else:
# 行 1949：变量赋值：保存配置、状态或计算结果。
                    self.b_t_data_data_4 = np.concatenate((self.b_t_data_data_4, self.b_t_data_cache))
# 行 1950：变量赋值：保存配置、状态或计算结果。
                    self.b_std_data_down = np.concatenate((self.b_std_data_down, self.b_std_data_while_down))
# 行 1951：变量赋值：保存配置、状态或计算结果。
                    self.b_std_data_up = np.concatenate((self.b_std_data_up, self.b_std_data_while_up))
# 行 1952：变量赋值：保存配置、状态或计算结果。
                    self.b_baseline_data = np.concatenate((self.b_baseline_data, self.b_baseline_data_while))
# 行 1953：变量赋值：保存配置、状态或计算结果。
                    indices2 = list(range(len(self.b_t_data_cache)))
# 行 1954：变量赋值：保存配置、状态或计算结果。
                    self.b_t_data_data_4 = np.delete(self.b_t_data_data_4, indices2)
# 行 1955：变量赋值：保存配置、状态或计算结果。
                    self.b_std_data_down = np.delete(self.b_std_data_down, indices2)
# 行 1956：变量赋值：保存配置、状态或计算结果。
                    self.b_std_data_up = np.delete(self.b_std_data_up, indices2)
# 行 1957：变量赋值：保存配置、状态或计算结果。
                    self.b_baseline_data = np.delete(self.b_baseline_data, indices2)
# 行 1958：空行，用于分隔代码块，提高可读性。

# 行 1959：条件判断，根据不同条件执行不同分支。
            if self.b_start_detection_param == True:
# 行 1960：条件判断，根据不同条件执行不同分支。
                if self.b_start_time_1_number == True:
# 行 1961：变量赋值：保存配置、状态或计算结果。
                    self.b_start_time_1 = time.perf_counter()
# 行 1962：变量赋值：保存配置、状态或计算结果。
                    self.b_start_time_1_number = False
# 行 1963：普通语句：执行一步操作或表达式。
                self.start_csv_txt_thread(self.b_t_data_cache, self.b_i_data_cache, self.b_baseline_data_while,
# 行 1964：普通语句：执行一步操作或表达式。
                                          self.b_std_data_while_up,
# 行 1965：普通语句：执行一步操作或表达式。
                                          self.b_std_data_while_down)
# 行 1966：变量赋值：保存配置、状态或计算结果。
            self.b_cache_number = True
# 行 1967：变量赋值：保存配置、状态或计算结果。
            self.b_i_data_cache = np.array([])
# 行 1968：变量赋值：保存配置、状态或计算结果。
            self.b_t_data_cache = np.array([])
# 行 1969：变量赋值：保存配置、状态或计算结果。
            self.b_i_data_cache = np.concatenate((self.b_i_data_cache, self.b_i_data_while))
# 行 1970：变量赋值：保存配置、状态或计算结果。
            self.b_t_data_cache = np.concatenate((self.b_t_data_cache, self.b_t_data_while))
# 行 1971：空行，用于分隔代码块，提高可读性。

# 行 1972：代码块开始（如函数、类、条件或循环体）。
        with data_lock:
# 行 1973：条件判断，根据不同条件执行不同分支。
            if len(self.b_i_data) <= self.b_frequency * 5:
# 行 1974：变量赋值：保存配置、状态或计算结果。
                self.b_t_data = np.concatenate((self.b_t_data, self.b_t_data_while))
# 行 1975：原始注释行，提供作者或用途说明。
                # self.b_v_data = np.concatenate((self.b_v_data, self.b_v_data_while))
# 行 1976：变量赋值：保存配置、状态或计算结果。
                self.b_i_data = np.concatenate((self.b_i_data, self.b_i_data_while))
# 行 1977：空行，用于分隔代码块，提高可读性。

# 行 1978：条件判断，根据不同条件执行不同分支。
            else:
# 行 1979：变量赋值：保存配置、状态或计算结果。
                self.b_t_data = np.concatenate((self.b_t_data, self.b_t_data_while))
# 行 1980：原始注释行，提供作者或用途说明。
                # self.b_v_data = np.concatenate((self.b_v_data, self.b_v_data_while))
# 行 1981：变量赋值：保存配置、状态或计算结果。
                self.b_i_data = np.concatenate((self.b_i_data, self.b_i_data_while))
# 行 1982：空行，用于分隔代码块，提高可读性。

# 行 1983：变量赋值：保存配置、状态或计算结果。
                indices1 = list(range(len(self.b_t_data_while)))
# 行 1984：变量赋值：保存配置、状态或计算结果。
                self.b_t_data = np.delete(self.b_t_data, indices1)
# 行 1985：变量赋值：保存配置、状态或计算结果。
                self.b_i_data = np.delete(self.b_i_data, indices1)
# 行 1986：原始注释行，提供作者或用途说明。
                # self.b_v_data = np.delete(self.b_v_data, indices1)
# 行 1987：空行，用于分隔代码块，提高可读性。

# 行 1988：原始注释行，提供作者或用途说明。
          # ??????????
# 行 1989：空行，用于分隔代码块，提高可读性。

# 行 1990：定义函数/方法 start_csv_txt_thread(self, b_t_data_while, b_i_data_while, b_baseline_data_while, b_std_data_while_up, b_std_data_while_down)。
    def start_csv_txt_thread(self, b_t_data_while, b_i_data_while, b_baseline_data_while, b_std_data_while_up, b_std_data_while_down):
# 行 1991：条件判断，根据不同条件执行不同分支。
        if not self.b_csv_txt_thread or not self.b_csv_txt_thread.is_alive():
# 行 1992：变量赋值：保存配置、状态或计算结果。
            self.b_csv_txt_thread = threading.Thread(
# 行 1993：变量赋值：保存配置、状态或计算结果。
                target=self.b_csv_and_tree,
# 行 1994：变量赋值：保存配置、状态或计算结果。
                args=(b_t_data_while, b_i_data_while, b_baseline_data_while, b_std_data_while_up, b_std_data_while_down)  # 传递参数
# 行 1995：普通语句：执行一步操作或表达式。
            )
# 行 1996：变量赋值：保存配置、状态或计算结果。
            self.b_csv_txt_thread.daemon = True
# 行 1997：普通语句：执行一步操作或表达式。
            self.b_csv_txt_thread.start()
# 行 1998：普通语句：执行一步操作或表达式。
            self.threads.append(self.b_csv_txt_thread)
# 行 1999：定义函数/方法 b_csv_and_tree(self, b_t_data_while, b_i_data_while, b_baseline_data_while, b_std_data_while_up, b_std_data_while_down)。
    def b_csv_and_tree(self, b_t_data_while, b_i_data_while, b_baseline_data_while, b_std_data_while_up, b_std_data_while_down):
# 行 2000：原始注释行，提供作者或用途说明。
            # self.update_baseline_std_vsm()
# 行 2001：变量赋值：保存配置、状态或计算结果。
                self.b_detection_DNA_sequency = time.perf_counter()
# 行 2002：普通语句：执行一步操作或表达式。
                self.b_event_list, event_number = self.event_detection(b_t_data_while, b_i_data_while,
# 行 2003：普通语句：执行一步操作或表达式。
                                                                       b_baseline_data_while,
# 行 2004：普通语句：执行一步操作或表达式。
                                                                       b_std_data_while_up,
# 行 2005：普通语句：执行一步操作或表达式。
                                                                       b_std_data_while_down, self.b_signal_direction,
# 行 2006：普通语句：执行一步操作或表达式。
                                                                       self.b_frequency,
# 行 2007：普通语句：执行一步操作或表达式。
                                                                       10, 0,
# 行 2008：普通语句：执行一步操作或表达式。
                                                                       float(0), float(1000), 0)
# 行 2009：空行，用于分隔代码块，提高可读性。

# 行 2010：空行，用于分隔代码块，提高可读性。

# 行 2011：空行，用于分隔代码块，提高可读性。

# 行 2012：条件判断，根据不同条件执行不同分支。
                if self.b_event_two_open_csv == True:
# 行 2013：条件判断，根据不同条件执行不同分支。
                    if self.b_event_two_open == True:
# 行 2014：打印调试或状态信息到控制台。
                        print(1)
# 行 2015：变量赋值：保存配置、状态或计算结果。
                        number = 0
# 行 2016：for 循环遍历序列或可迭代对象。
                        for i in range(len(self.b_event_list)):
# 行 2017：变量赋值：保存配置、状态或计算结果。
                            list1 = self.b_event_list[i]
# 行 2018：变量赋值：保存配置、状态或计算结果。
                            y = list1[10]
# 行 2019：条件判断，根据不同条件执行不同分支。
                            if list1[4] >= 0:
# 行 2020：原始注释行，提供作者或用途说明。
                                # down, up = self.event_detection_two(y, self.c_detection_three, list1[4])
# 行 2021：普通语句：执行一步操作或表达式。
                                down, up = self.event_detection_two(list1[9], list1[10], 0.01, 0.1)
# 行 2022：条件判断，根据不同条件执行不同分支。
                                if down or up:
# 行 2023：普通语句：执行一步操作或表达式。
                                    self.b_event_list_two.append(list1)
# 行 2024：条件判断，根据不同条件执行不同分支。
                                    if 2 > len(self.b_event_list_two) > 0:
# 行 2025：变量赋值：保存配置、状态或计算结果。
                                        self.b_detection_DNA_yes1 = time.perf_counter()
# 行 2026：普通语句：执行一步操作或表达式。
                                    self.b_event_amplitude_two.append(float(list1[2]))
# 行 2027：普通语句：执行一步操作或表达式。
                                    self.b_event_duration_two.append(float(list1[4]))
# 行 2028：变量赋值：保存配置、状态或计算结果。
                                    number = i
# 行 2029：普通语句：执行一步操作或表达式。
                                    self.b_event_two_number_index.append(self.b_event_number + number + 1)
# 行 2030：空行，用于分隔代码块，提高可读性。

# 行 2031：条件判断，根据不同条件执行不同分支。
                        if len(self.b_event_list_two) > 0:
# 行 2032：空行，用于分隔代码块，提高可读性。

# 行 2033：条件判断，根据不同条件执行不同分支。
                            if self.b_event_1_time == True:
# 行 2034：打印调试或状态信息到控制台。
                                print(len(self.b_event_list_two))
# 行 2035：条件判断，根据不同条件执行不同分支。
                                if self.b_probe == '探针EC-K12':
# 行 2036：变量赋值：保存配置、状态或计算结果。
                                    self.b_detection_DNA_yes = time.perf_counter()
# 行 2037：普通语句：执行一步操作或表达式。
                                    self.detection_DNA_None.config(text=f'检测到{self.b_probe}偶联DNA', fg='red')
# 行 2038：普通语句：执行一步操作或表达式。
                                    self.detection_DNA_time.config(
# 行 2039：变量赋值：保存配置、状态或计算结果。
                                        text=f'单次检测时长{np.abs((float(self.b_event_list_all[0][9][0]) - float(self.b_event_list_two[0][9][0]))):.4f}秒',
# 行 2040：变量赋值：保存配置、状态或计算结果。
                                        fg='black')
# 行 2041：普通语句：执行一步操作或表达式。
                                    self.detection_DNA_time_sequency.config(
# 行 2042：变量赋值：保存配置、状态或计算结果。
                                        text=f'单序列检测时长{(self.b_detection_DNA_yes - self.b_detection_DNA_sequency):.4f}秒',
# 行 2043：变量赋值：保存配置、状态或计算结果。
                                        fg='black')
# 行 2044：条件判断，根据不同条件执行不同分支。
                                if self.b_probe == '探针LA-DSM20079':
# 行 2045：变量赋值：保存配置、状态或计算结果。
                                    self.b_detection_DNA_yes = time.perf_counter()
# 行 2046：普通语句：执行一步操作或表达式。
                                    self.detection_DNA_None.config(text=f'检测到{self.b_probe}偶联DNA', fg='red')
# 行 2047：普通语句：执行一步操作或表达式。
                                    self.detection_DNA_time.config(
# 行 2048：变量赋值：保存配置、状态或计算结果。
                                        text=f'单次检测时长{np.abs((float(self.b_event_list_all[0][9][0]) - float(self.b_event_list_two[0][9][0]))):.4f}秒',
# 行 2049：变量赋值：保存配置、状态或计算结果。
                                        fg='black')
# 行 2050：普通语句：执行一步操作或表达式。
                                    self.detection_DNA_time_sequency.config(
# 行 2051：变量赋值：保存配置、状态或计算结果。
                                        text=f'单序列检测时长{(self.b_detection_DNA_yes - self.b_detection_DNA_sequency):.4f}秒',
# 行 2052：变量赋值：保存配置、状态或计算结果。
                                        fg='black')
# 行 2053：条件判断，根据不同条件执行不同分支。
                                if self.b_probe == '探针ST-NCTC12958':
# 行 2054：变量赋值：保存配置、状态或计算结果。
                                    self.b_detection_DNA_yes = time.perf_counter()
# 行 2055：普通语句：执行一步操作或表达式。
                                    self.detection_DNA_None.config(text=f'检测到{self.b_probe}偶联DNA', fg='red')
# 行 2056：普通语句：执行一步操作或表达式。
                                    self.detection_DNA_time.config(
# 行 2057：变量赋值：保存配置、状态或计算结果。
                                        text=f'单次检测时长{np.abs((float(self.b_event_list_all[0][9][0]) - float(self.b_event_list_two[0][9][0]))):.4f}秒',
# 行 2058：变量赋值：保存配置、状态或计算结果。
                                        fg='black')
# 行 2059：普通语句：执行一步操作或表达式。
                                    self.detection_DNA_time_sequency.config(
# 行 2060：变量赋值：保存配置、状态或计算结果。
                                        text=f'单序列检测时长{(self.b_detection_DNA_yes - self.b_detection_DNA_sequency):.4f}秒',
# 行 2061：变量赋值：保存配置、状态或计算结果。
                                        fg='black')
# 行 2062：条件判断，根据不同条件执行不同分支。
                                if self.b_probe == 'EC-K12':
# 行 2063：变量赋值：保存配置、状态或计算结果。
                                    self.b_detection_DNA_yes = time.perf_counter()
# 行 2064：普通语句：执行一步操作或表达式。
                                    self.detection_DNA_None.config(text=f'检测到{self.b_probe}', fg='red')
# 行 2065：条件判断，根据不同条件执行不同分支。
                                if self.b_probe == 'EC-BL21':
# 行 2066：变量赋值：保存配置、状态或计算结果。
                                    self.b_detection_DNA_yes = time.perf_counter()
# 行 2067：普通语句：执行一步操作或表达式。
                                    self.detection_DNA_None.config(text=f'检测到{self.b_probe}', fg='red')
# 行 2068：条件判断，根据不同条件执行不同分支。
                                if self.b_probe == 'EC-CFT073':
# 行 2069：变量赋值：保存配置、状态或计算结果。
                                    self.b_detection_DNA_yes = time.perf_counter()
# 行 2070：普通语句：执行一步操作或表达式。
                                    self.detection_DNA_None.config(text=f'检测到{self.b_probe}', fg='red')
# 行 2071：普通语句：执行一步操作或表达式。
                                self.update_plot_event_2D(self.b_event_amplitude, self.b_event_duration)
# 行 2072：空行，用于分隔代码块，提高可读性。

# 行 2073：普通语句：执行一步操作或表达式。
                                winsound.Beep(1000, 1000)
# 行 2074：原始注释行，提供作者或用途说明。
                                # create_label2(self.collection_eONE_page_original,
# 行 2075：原始注释行，提供作者或用途说明。
                                #               f'检测到{self.b_probe}偶联DNA',
# 行 2076：原始注释行，提供作者或用途说明。
                                #               700, 850)
# 行 2077：原始注释行，提供作者或用途说明。
                                # create_label2(self.collection_eONE_page_original,
# 行 2078：原始注释行，提供作者或用途说明。
                                #               f'单次检测时长{(self.b_detection_DNA_yes - self.b_detection_DNA_None):.4f}秒',
# 行 2079：原始注释行，提供作者或用途说明。
                                #               700, 920)
# 行 2080：原始注释行，提供作者或用途说明。
                                # self.b_display_rms.config(
# 行 2081：原始注释行，提供作者或用途说明。
                                #     text=f'   {round(float(std_number[-1]), 4)}{self.b_last_current_range_selected[-2:]}')
# 行 2082：普通语句：执行一步操作或表达式。
                                self.b_text_display.config(state=tk.NORMAL)
# 行 2083：变量赋值：保存配置、状态或计算结果。
                                line_index = self.b_text_display.index(tk.END)
# 行 2084：普通语句：执行一步操作或表达式。
                                self.b_text_display.delete(1.0, tk.END)
# 行 2085：普通语句：执行一步操作或表达式。
                                self.b_text_display.insert(tk.END, self.b_event_two_number_index)
# 行 2086：普通语句：执行一步操作或表达式。
                                self.b_text_display.yview(tk.END)
# 行 2087：普通语句：执行一步操作或表达式。
                                self.b_text_display.config(state=tk.DISABLED)
# 行 2088：原始注释行，提供作者或用途说明。
                                # self.b_event_1_time = False
# 行 2089：原始注释行，提供作者或用途说明。
                                # self.update_event_plot1(self.b_event_list_two[-1][9],self.b_event_list_two[-1][10])
# 行 2090：空行，用于分隔代码块，提高可读性。

# 行 2091：空行，用于分隔代码块，提高可读性。

# 行 2092：普通语句：执行一步操作或表达式。
                self.b_event_number += event_number
# 行 2093：条件判断，根据不同条件执行不同分支。
                if self.b_save_number == True:
# 行 2094：打印调试或状态信息到控制台。
                    print(self.b_event_two_open_csv, self.b_event_two_open)
# 行 2095：条件判断，根据不同条件执行不同分支。
                    if self.b_probe == '探针EC-K12':
# 行 2096：变量赋值：保存配置、状态或计算结果。
                        self.detection_DNA_None = create_label2(self.collection_eONE_page_original,
# 行 2097：普通语句：执行一步操作或表达式。
                                                                f'未检测到{self.b_probe}偶联DNA',
# 行 2098：普通语句：执行一步操作或表达式。
                                                                700, 800)
# 行 2099：变量赋值：保存配置、状态或计算结果。
                        self.detection_DNA_time = create_label2(self.collection_eONE_page_original,
# 行 2100：普通语句：执行一步操作或表达式。
                                                                '',
# 行 2101：普通语句：执行一步操作或表达式。
                                                                700, 870)
# 行 2102：变量赋值：保存配置、状态或计算结果。
                        self.detection_DNA_time_sequency = create_label2(self.collection_eONE_page_original,
# 行 2103：普通语句：执行一步操作或表达式。
                                                                '',
# 行 2104：普通语句：执行一步操作或表达式。
                                                                700, 940)
# 行 2105：条件判断，根据不同条件执行不同分支。
                    if self.b_probe == '探针LA-DSM20079':
# 行 2106：变量赋值：保存配置、状态或计算结果。
                        self.detection_DNA_None = create_label2(self.collection_eONE_page_original,
# 行 2107：普通语句：执行一步操作或表达式。
                                                                f'未检测到{self.b_probe}偶联DNA',
# 行 2108：普通语句：执行一步操作或表达式。
                                                                700, 800)
# 行 2109：变量赋值：保存配置、状态或计算结果。
                        self.detection_DNA_time = create_label2(self.collection_eONE_page_original,
# 行 2110：普通语句：执行一步操作或表达式。
                                                                '',
# 行 2111：普通语句：执行一步操作或表达式。
                                                                700, 870)
# 行 2112：变量赋值：保存配置、状态或计算结果。
                        self.detection_DNA_time_sequency = create_label2(self.collection_eONE_page_original,
# 行 2113：普通语句：执行一步操作或表达式。
                                                                '',
# 行 2114：普通语句：执行一步操作或表达式。
                                                                700, 940)
# 行 2115：条件判断，根据不同条件执行不同分支。
                    if self.b_probe == '探针ST-NCTC12958':
# 行 2116：变量赋值：保存配置、状态或计算结果。
                        self.detection_DNA_None = create_label2(self.collection_eONE_page_original,
# 行 2117：普通语句：执行一步操作或表达式。
                                                                f'未检测到{self.b_probe}偶联DNA',
# 行 2118：普通语句：执行一步操作或表达式。
                                                                700, 800)
# 行 2119：变量赋值：保存配置、状态或计算结果。
                        self.detection_DNA_time = create_label2(self.collection_eONE_page_original,
# 行 2120：普通语句：执行一步操作或表达式。
                                                                '',
# 行 2121：普通语句：执行一步操作或表达式。
                                                                700, 870)
# 行 2122：变量赋值：保存配置、状态或计算结果。
                        self.detection_DNA_time_sequency = create_label2(self.collection_eONE_page_original,
# 行 2123：普通语句：执行一步操作或表达式。
                                                                '',
# 行 2124：普通语句：执行一步操作或表达式。
                                                                700, 940)
# 行 2125：条件判断，根据不同条件执行不同分支。
                    if self.b_probe == 'EC-K12':
# 行 2126：变量赋值：保存配置、状态或计算结果。
                        self.detection_DNA_None = create_label2(self.collection_eONE_page_original,
# 行 2127：普通语句：执行一步操作或表达式。
                                                                f'未检测到{self.b_probe}',
# 行 2128：普通语句：执行一步操作或表达式。
                                                                700, 800)
# 行 2129：变量赋值：保存配置、状态或计算结果。
                        self.detection_DNA_time = create_label2(self.collection_eONE_page_original,
# 行 2130：普通语句：执行一步操作或表达式。
                                                                '',
# 行 2131：普通语句：执行一步操作或表达式。
                                                                700, 870)
# 行 2132：变量赋值：保存配置、状态或计算结果。
                        self.detection_DNA_time_sequency = create_label2(self.collection_eONE_page_original,
# 行 2133：普通语句：执行一步操作或表达式。
                                                                '',
# 行 2134：普通语句：执行一步操作或表达式。
                                                                700, 940)
# 行 2135：条件判断，根据不同条件执行不同分支。
                    if self.b_probe == 'EC-BL21':
# 行 2136：变量赋值：保存配置、状态或计算结果。
                        self.detection_DNA_None = create_label2(self.collection_eONE_page_original,
# 行 2137：普通语句：执行一步操作或表达式。
                                                                f'未检测到{self.b_probe}',
# 行 2138：普通语句：执行一步操作或表达式。
                                                                700, 800)
# 行 2139：变量赋值：保存配置、状态或计算结果。
                        self.detection_DNA_time = create_label2(self.collection_eONE_page_original,
# 行 2140：普通语句：执行一步操作或表达式。
                                                                '',
# 行 2141：普通语句：执行一步操作或表达式。
                                                                700, 870)
# 行 2142：变量赋值：保存配置、状态或计算结果。
                        self.detection_DNA_time_sequency = create_label2(self.collection_eONE_page_original,
# 行 2143：普通语句：执行一步操作或表达式。
                                                                '',
# 行 2144：普通语句：执行一步操作或表达式。
                                                                700, 940)
# 行 2145：条件判断，根据不同条件执行不同分支。
                    if self.b_probe == 'EC-CFT073':
# 行 2146：变量赋值：保存配置、状态或计算结果。
                        self.detection_DNA_None = create_label2(self.collection_eONE_page_original,
# 行 2147：普通语句：执行一步操作或表达式。
                                                                f'未检测到{self.b_probe}',
# 行 2148：普通语句：执行一步操作或表达式。
                                                                700, 800)
# 行 2149：变量赋值：保存配置、状态或计算结果。
                        self.detection_DNA_time = create_label2(self.collection_eONE_page_original,
# 行 2150：普通语句：执行一步操作或表达式。
                                                                '',
# 行 2151：普通语句：执行一步操作或表达式。
                                                                700, 870)
# 行 2152：变量赋值：保存配置、状态或计算结果。
                        self.detection_DNA_time_sequency = create_label2(self.collection_eONE_page_original,
# 行 2153：普通语句：执行一步操作或表达式。
                                                                '',
# 行 2154：普通语句：执行一步操作或表达式。
                                                                700, 940)
# 行 2155：变量赋值：保存配置、状态或计算结果。
                    current_time = datetime.now()
# 行 2156：变量赋值：保存配置、状态或计算结果。
                    self.formatted_time = current_time.strftime("%Y-%m-%d-%H-%M")
# 行 2157：变量赋值：保存配置、状态或计算结果。
                    self.file_path_1 = os.path.join(self.b_save_path, f'{self.formatted_time}特征提取_1.csv')
# 行 2158：变量赋值：保存配置、状态或计算结果。
                    self.file_path_2 = os.path.join(self.b_save_path, f'{self.formatted_time}特征提取_2.csv')
# 行 2159：打印调试或状态信息到控制台。
                    print(self.file_path_2)
# 行 2160：打印调试或状态信息到控制台。
                    print(self.file_path_1)
# 行 2161：变量赋值：保存配置、状态或计算结果。
                    self.b_save_number = False
# 行 2162：变量赋值：保存配置、状态或计算结果。
                    self.b_event_list_append = True
# 行 2163：使用 with 上下文安全地打开文件，结束时自动关闭。
                    with open(self.file_path_1, mode='w', newline='', encoding='gbk') as file:
# 行 2164：变量赋值：保存配置、状态或计算结果。
                        writer_1 = csv.writer(file)
# 行 2165：普通语句：执行一步操作或表达式。
                        writer_1.writerow([
# 行 2166：普通语句：执行一步操作或表达式。
                            '事件数', '基线(nA)', '振幅(nA)', '积分(10-9C)', '持续时间(ms)',
# 行 2167：普通语句：执行一步操作或表达式。
                            '钝度', '脉冲不对称', '横轴惯性', '纵轴惯性',
# 行 2168：普通语句：执行一步操作或表达式。
                            '事件数据(时间)', '事件数据(电流)'
# 行 2169：普通语句：执行一步操作或表达式。
                        ])
# 行 2170：使用 with 上下文安全地打开文件，结束时自动关闭。
                    with open(self.file_path_2, mode='w', newline='', encoding='gbk') as file:
# 行 2171：变量赋值：保存配置、状态或计算结果。
                        writer_2 = csv.writer(file)
# 行 2172：普通语句：执行一步操作或表达式。
                        writer_2.writerow([
# 行 2173：普通语句：执行一步操作或表达式。
                            '事件数', '基线(nA)', '振幅(nA)', '积分(10-9C)', '持续时间(ms)',
# 行 2174：普通语句：执行一步操作或表达式。
                            '钝度', '脉冲不对称', '横轴惯性', '纵轴惯性',
# 行 2175：普通语句：执行一步操作或表达式。
                            '事件数据(时间)', '事件数据(电流)'
# 行 2176：普通语句：执行一步操作或表达式。
                        ])
# 行 2177：变量赋值：保存配置、状态或计算结果。
                    self.b_event_two_open_csv = True
# 行 2178：打印调试或状态信息到控制台。
                    print(self.b_event_two_open)
# 行 2179：空行，用于分隔代码块，提高可读性。

# 行 2180：条件判断，根据不同条件执行不同分支。
                if self.b_save_txt_head == True:
# 行 2181：变量赋值：保存配置、状态或计算结果。
                    self.b_save_txt_head = False
# 行 2182：变量赋值：保存配置、状态或计算结果。
                    self.b_save_txt_time_current = True
# 行 2183：变量赋值：保存配置、状态或计算结果。
                    self.file_path_txt = os.path.join(self.b_save_path, f'{self.formatted_time}源数据.txt')
# 行 2184：打印调试或状态信息到控制台。
                    print(self.file_path_txt)
# 行 2185：使用 with 上下文安全地打开文件，结束时自动关闭。
                    with open(self.file_path_txt, 'w') as file:
# 行 2186：普通语句：执行一步操作或表达式。
                        file.write(f'{self.formatted_time}' + '      ' + f'采样率:{self.b_frequency}Hz' + '\n')
# 行 2187：普通语句：执行一步操作或表达式。
                        file.write(f'截止频率:{self.b_filter_parameter}Hz' + '\n')
# 行 2188：条件判断，根据不同条件执行不同分支。
                if self.b_save_txt_time_current == True:
# 行 2189：变量赋值：保存配置、状态或计算结果。
                    time_current_list = [[a, b] for a, b in zip(b_t_data_while, b_i_data_while)]
# 行 2190：变量赋值：保存配置、状态或计算结果。
                    all_content = '\n'.join([' '.join(map(str, row)) for row in time_current_list])
# 行 2191：使用 with 上下文安全地打开文件，结束时自动关闭。
                    with open(self.file_path_txt, 'a', encoding='utf-8') as file:
# 行 2192：普通语句：执行一步操作或表达式。
                        file.write(all_content)
# 行 2193：变量赋值：保存配置、状态或计算结果。
                number = 0
# 行 2194：条件判断，根据不同条件执行不同分支。
                if self.b_event_list_append == True:
# 行 2195：for 循环遍历序列或可迭代对象。
                    for i in range(self.b_event_number - event_number, self.b_event_number, 1):
# 行 2196：普通语句：执行一步操作或表达式。
                        self.save_event_list_to_csv(i + 1, self.b_event_list[number], self.file_path_1, 1)
# 行 2197：空行，用于分隔代码块，提高可读性。

# 行 2198：普通语句：执行一步操作或表达式。
                        self.test_tree([i + 1, self.b_event_list[number][1], self.b_event_list[number][2],
# 行 2199：普通语句：执行一步操作或表达式。
                                        self.b_event_list[number][3],
# 行 2200：普通语句：执行一步操作或表达式。
                                        self.b_event_list[number][4], self.b_event_list[number][5],
# 行 2201：普通语句：执行一步操作或表达式。
                                        self.b_event_list[number][6],
# 行 2202：普通语句：执行一步操作或表达式。
                                        self.b_event_list[number][7], self.b_event_list[number][8]])
# 行 2203：普通语句：执行一步操作或表达式。
                        self.b_event_list_all.append(self.b_event_list[number])
# 行 2204：原始注释行，提供作者或用途说明。
                        # self.update_event_plot1(self.b_event_list[number][9], self.b_event_list[number][10])
# 行 2205：普通语句：执行一步操作或表达式。
                        self.b_event_amplitude.append(float(self.b_event_list[number][2]))
# 行 2206：普通语句：执行一步操作或表达式。
                        self.b_event_duration.append(self.b_event_list[number][4])
# 行 2207：普通语句：执行一步操作或表达式。
                        self.update_plot_event_2D(self.b_event_amplitude, self.b_event_duration)
# 行 2208：普通语句：执行一步操作或表达式。
                        self.b_event_list_1.append(float(self.b_event_list[number][2]))
# 行 2209：普通语句：执行一步操作或表达式。
                        self.b_event_list_2.append(float(self.b_event_list[number][3]))
# 行 2210：普通语句：执行一步操作或表达式。
                        self.b_event_list_3.append(float(self.b_event_list[number][4]))
# 行 2211：普通语句：执行一步操作或表达式。
                        self.b_event_list_4.append(float(self.b_event_list[number][5]))
# 行 2212：普通语句：执行一步操作或表达式。
                        self.b_event_list_5.append(float(self.b_event_list[number][6]))
# 行 2213：普通语句：执行一步操作或表达式。
                        self.b_event_list_6.append(float(self.b_event_list[number][7]))
# 行 2214：普通语句：执行一步操作或表达式。
                        self.b_event_list_7.append(float(self.b_event_list[number][8]))
# 行 2215：普通语句：执行一步操作或表达式。
                        number += 1
# 行 2216：空行，用于分隔代码块，提高可读性。

# 行 2217：条件判断，根据不同条件执行不同分支。
                else:
# 行 2218：变量赋值：保存配置、状态或计算结果。
                    number = 0
# 行 2219：条件判断，根据不同条件执行不同分支。
                if self.b_event_two_open_csv == True:
# 行 2220：for 循环遍历序列或可迭代对象。
                    for i in range(self.b_event_list_two_len, len(self.b_event_list_two)):
# 行 2221：普通语句：执行一步操作或表达式。
                        self.save_event_list_to_csv(self.b_event_list_two_len + 1, self.b_event_list_two[self.b_event_list_two_len], self.file_path_2, 2)
# 行 2222：普通语句：执行一步操作或表达式。
                        self.b_event_list_two_len += 1
# 行 2223：原始注释行，提供作者或用途说明。
                # self.update_seven_features(self.b_event_list_1, self.b_event_list_2, self.b_event_list_3, self.b_event_list_4, self.b_event_list_5,
# 行 2224：原始注释行，提供作者或用途说明。
                #                            self.b_event_list_6, self.b_event_list_7)
# 行 2225：定义函数/方法 plot_current(self)。
    def plot_current(self):
# 行 2226：变量赋值：保存配置、状态或计算结果。
        fig = Figure(figsize=(12.3, 2.9), dpi=100)
# 行 2227：普通语句：执行一步操作或表达式。
        fig.patch.set_facecolor('#e5e5e5')
# 行 2228：变量赋值：保存配置、状态或计算结果。
        self.ax4 = fig.add_subplot(111)
# 行 2229：普通语句：执行一步操作或表达式。
        self.ax4.set_facecolor('#e5e5e5')
# 行 2230：普通语句：执行一步操作或表达式。
        self.ax4.set_xlabel("时间(s)")
# 行 2231：普通语句：执行一步操作或表达式。
        self.ax4.set_ylabel("电流(nA)")
# 行 2232：普通语句：执行一步操作或表达式。
        self.ax4.grid(True)
# 行 2233：普通语句：执行一步操作或表达式。
        fig.tight_layout()
# 行 2234：普通语句：执行一步操作或表达式。
        fig.subplots_adjust(left=0.05)
# 行 2235：变量赋值：保存配置、状态或计算结果。
        self.canvas_current = FigureCanvasTkAgg(fig, master=self.collection_eONE_page_original)
# 行 2236：变量赋值：保存配置、状态或计算结果。
        self.canvas_current2 = self.canvas_current.get_tk_widget()
# 行 2237：普通语句：执行一步操作或表达式。
        self.canvas_current2.place(x=320, y=175)
# 行 2238：定义函数/方法 plot_event1(self)。
    def plot_event1(self):
# 行 2239：变量赋值：保存配置、状态或计算结果。
        fig = Figure(figsize=(3.4, 3.4), dpi=100)
# 行 2240：普通语句：执行一步操作或表达式。
        fig.patch.set_facecolor('#e5e5e5')
# 行 2241：变量赋值：保存配置、状态或计算结果。
        self.ax2 = fig.add_subplot(111)
# 行 2242：普通语句：执行一步操作或表达式。
        self.ax2.set_facecolor('#e5e5e5')
# 行 2243：普通语句：执行一步操作或表达式。
        self.ax2.set_title("事件图")
# 行 2244：普通语句：执行一步操作或表达式。
        self.ax2.set_xlabel("时间(s)")
# 行 2245：普通语句：执行一步操作或表达式。
        self.ax2.set_ylabel("电流(nA)")
# 行 2246：普通语句：执行一步操作或表达式。
        self.ax2.grid(True)
# 行 2247：普通语句：执行一步操作或表达式。
        fig.tight_layout()
# 行 2248：普通语句：执行一步操作或表达式。
        fig.subplots_adjust(left=0.17)
# 行 2249：变量赋值：保存配置、状态或计算结果。
        self.canvas_event2 = FigureCanvasTkAgg(fig, master=self.collection_eONE_page_original)
# 行 2250：变量赋值：保存配置、状态或计算结果。
        self.canvas_widget2 = self.canvas_event2.get_tk_widget()
# 行 2251：普通语句：执行一步操作或表达式。
        self.canvas_widget2.place(x=1567, y=0)
# 行 2252：空行，用于分隔代码块，提高可读性。

# 行 2253：定义函数/方法 update_event_plot1(self, x, y)。
    def update_event_plot1(self, x, y):
# 行 2254：普通语句：执行一步操作或表达式。
        self.ax2.cla()
# 行 2255：普通语句：执行一步操作或表达式。
        self.ax2.grid(True)
# 行 2256：普通语句：执行一步操作或表达式。
        self.ax2.set_title("事件图")
# 行 2257：普通语句：执行一步操作或表达式。
        self.ax2.set_xlabel("时间(s)")
# 行 2258：普通语句：执行一步操作或表达式。
        self.ax2.set_ylabel("电流(nA)")
# 行 2259：普通语句：执行一步操作或表达式。
        self.ax2.plot(x, y)
# 行 2260：普通语句：执行一步操作或表达式。
        self.canvas_event2.draw()
# 行 2261：定义函数/方法 plot_seven_features(self)。
    def plot_seven_features(self):
# 行 2262：变量赋值：保存配置、状态或计算结果。
        fig = Figure(figsize=(9.62, 2), dpi=100)
# 行 2263：普通语句：执行一步操作或表达式。
        fig.patch.set_facecolor('#e5e5e5')
# 行 2264：空行，用于分隔代码块，提高可读性。

# 行 2265：变量赋值：保存配置、状态或计算结果。
        self.seven_axes = []
# 行 2266：变量赋值：保存配置、状态或计算结果。
        self.seven_bar_sets = []
# 行 2267：变量赋值：保存配置、状态或计算结果。
        colors = ['red','orange','yellow','green','blue','purple','pink']
# 行 2268：for 循环遍历序列或可迭代对象。
        for i in range(7):
# 行 2269：变量赋值：保存配置、状态或计算结果。
            ax = fig.add_subplot(1, 7, i + 1)
# 行 2270：普通语句：执行一步操作或表达式。
            ax.set_facecolor('#e5e5e5')
# 行 2271：变量赋值：保存配置、状态或计算结果。
            bars = ax.bar([0], [0], color=colors[i])
# 行 2272：普通语句：执行一步操作或表达式。
            self.seven_bar_sets.append(bars)
# 行 2273：普通语句：执行一步操作或表达式。
            ax.grid(True)
# 行 2274：普通语句：执行一步操作或表达式。
            ax.xaxis.set_ticks_position('none')
# 行 2275：普通语句：执行一步操作或表达式。
            ax.yaxis.set_ticks_position('none')
# 行 2276：普通语句：执行一步操作或表达式。
            ax.set_xticklabels([])
# 行 2277：普通语句：执行一步操作或表达式。
            ax.set_yticklabels([])
# 行 2278：普通语句：执行一步操作或表达式。
            self.seven_axes.append(ax)
# 行 2279：普通语句：执行一步操作或表达式。
        fig.subplots_adjust(
# 行 2280：变量赋值：保存配置、状态或计算结果。
            left=0.001,
# 行 2281：变量赋值：保存配置、状态或计算结果。
            right=0.999,
# 行 2282：变量赋值：保存配置、状态或计算结果。
            top=0.999,
# 行 2283：变量赋值：保存配置、状态或计算结果。
            bottom=0.01,
# 行 2284：变量赋值：保存配置、状态或计算结果。
            wspace=0
# 行 2285：普通语句：执行一步操作或表达式。
        )
# 行 2286：空行，用于分隔代码块，提高可读性。

# 行 2287：变量赋值：保存配置、状态或计算结果。
        self.seven_charts_canvas = FigureCanvasTkAgg(fig, master=self.collection_eONE_page_original)
# 行 2288：变量赋值：保存配置、状态或计算结果。
        self.seven_charts_widget = self.seven_charts_canvas.get_tk_widget()
# 行 2289：普通语句：执行一步操作或表达式。
        self.seven_charts_widget.place(x=587, y=780)
# 行 2290：定义函数/方法 update_seven_threading(self, list1)。
    def update_seven_threading(self, list1):
# 行 2291：for 循环遍历序列或可迭代对象。
        for i in range(7):
# 行 2292：变量赋值：保存配置、状态或计算结果。
            ax = self.seven_axes[i]
# 行 2293：变量赋值：保存配置、状态或计算结果。
            data = list1[i]
# 行 2294：普通语句：执行一步操作或表达式。
            ax.clear()
# 行 2295：空行，用于分隔代码块，提高可读性。

# 行 2296：条件判断，根据不同条件执行不同分支。
            if not data or len(data) == 0:
# 行 2297：变量赋值：保存配置、状态或计算结果。
                data = [0]
# 行 2298：空行，用于分隔代码块，提高可读性。

# 行 2299：普通语句：执行一步操作或表达式。
            ax.hist(
# 行 2300：普通语句：执行一步操作或表达式。
                data,
# 行 2301：变量赋值：保存配置、状态或计算结果。
                bins=50,
# 行 2302：变量赋值：保存配置、状态或计算结果。
                density=False,
# 行 2303：变量赋值：保存配置、状态或计算结果。
                alpha=0.6,
# 行 2304：普通语句：执行一步操作或表达式。
            )
# 行 2305：空行，用于分隔代码块，提高可读性。

# 行 2306：普通语句：执行一步操作或表达式。
            ax.set_facecolor('#e5e5e5')
# 行 2307：普通语句：执行一步操作或表达式。
            ax.set_xticks([])
# 行 2308：普通语句：执行一步操作或表达式。
            ax.set_yticks([])
# 行 2309：普通语句：执行一步操作或表达式。
            ax.grid(True)
# 行 2310：空行，用于分隔代码块，提高可读性。

# 行 2311：普通语句：执行一步操作或表达式。
            ax.relim()
# 行 2312：普通语句：执行一步操作或表达式。
            ax.autoscale_view()
# 行 2313：空行，用于分隔代码块，提高可读性。

# 行 2314：普通语句：执行一步操作或表达式。
        self.seven_charts_canvas.draw()
# 行 2315：定义函数/方法 update_seven_features(self, data_list1, data_list2, data_list3, data_list4, data_list5, data_list6, data_list7)。
    def update_seven_features(self, data_list1, data_list2, data_list3, data_list4, data_list5, data_list6, data_list7):
# 行 2316：变量赋值：保存配置、状态或计算结果。
        self.threads = [t for t in self.threads if t.is_alive()]
# 行 2317：变量赋值：保存配置、状态或计算结果。
        list1 = [data_list1, data_list2, data_list3, data_list4, data_list5, data_list6, data_list7]
# 行 2318：空行，用于分隔代码块，提高可读性。

# 行 2319：变量赋值：保存配置、状态或计算结果。
        t = threading.Thread(target=self.update_seven_threading, args=(list1,)
# 行 2320：普通语句：执行一步操作或表达式。
                             )
# 行 2321：变量赋值：保存配置、状态或计算结果。
        t.daemon = True
# 行 2322：普通语句：执行一步操作或表达式。
        t.start()
# 行 2323：普通语句：执行一步操作或表达式。
        self.threads.append(t)
# 行 2324：空行，用于分隔代码块，提高可读性。

# 行 2325：定义函数/方法 b_on_item_click(self, event)。
    def b_on_item_click(self, event):
# 行 2326：变量赋值：保存配置、状态或计算结果。
        selected_item = self.tree.selection()
# 行 2327：条件判断，根据不同条件执行不同分支。
        if selected_item:
# 行 2328：变量赋值：保存配置、状态或计算结果。
            item_values = self.tree.item(selected_item[0])['values']
# 行 2329：变量赋值：保存配置、状态或计算结果。
            number = item_values[0]
# 行 2330：普通语句：执行一步操作或表达式。
            self.update_event_plot1(self.b_event_list_all[number-1][9], self.b_event_list_all[number-1][10])
# 行 2331：空行，用于分隔代码块，提高可读性。

# 行 2332：定义函数/方法 plot_event_2D(self)。
    def plot_event_2D(self):
# 行 2333：变量赋值：保存配置、状态或计算结果。
        fig = Figure(figsize=(3.4, 3.4), dpi=100)
# 行 2334：普通语句：执行一步操作或表达式。
        fig.patch.set_facecolor('#e5e5e5')
# 行 2335：变量赋值：保存配置、状态或计算结果。
        self.ax3 = fig.add_subplot(111)
# 行 2336：普通语句：执行一步操作或表达式。
        self.ax3.set_facecolor('#e5e5e5')
# 行 2337：普通语句：执行一步操作或表达式。
        self.ax3.set_title("时间VS电流")
# 行 2338：普通语句：执行一步操作或表达式。
        self.ax3.set_xlabel("电流(nA)")
# 行 2339：普通语句：执行一步操作或表达式。
        self.ax3.set_ylabel("时间(ms)")
# 行 2340：普通语句：执行一步操作或表达式。
        self.ax3.grid(True)
# 行 2341：普通语句：执行一步操作或表达式。
        fig.tight_layout()
# 行 2342：普通语句：执行一步操作或表达式。
        fig.subplots_adjust(left=0.17)
# 行 2343：空行，用于分隔代码块，提高可读性。

# 行 2344：变量赋值：保存配置、状态或计算结果。
        self.canvas_event3 = FigureCanvasTkAgg(fig, master=self.collection_eONE_page_original)
# 行 2345：变量赋值：保存配置、状态或计算结果。
        self.canvas_widget3 = self.canvas_event3.get_tk_widget()
# 行 2346：普通语句：执行一步操作或表达式。
        self.canvas_widget3.place(x=1567, y=340)
# 行 2347：定义函数/方法 update_plot_event_2D(self, x, y)。
    def update_plot_event_2D(self, x, y):
# 行 2348：普通语句：执行一步操作或表达式。
        self.ax3.cla()
# 行 2349：普通语句：执行一步操作或表达式。
        self.ax3.set_title("时间VS电流")
# 行 2350：普通语句：执行一步操作或表达式。
        self.ax3.set_xlabel("电流(nA)")
# 行 2351：普通语句：执行一步操作或表达式。
        self.ax3.set_ylabel("时间(ms)")
# 行 2352：普通语句：执行一步操作或表达式。
        self.ax3.scatter(x, y, edgecolor='red', facecolors='None', s=10)
# 行 2353：条件判断，根据不同条件执行不同分支。
        if self.b_event_two_open == True:
# 行 2354：条件判断，根据不同条件执行不同分支。
            if len(self.b_event_amplitude_two) > 0:
# 行 2355：普通语句：执行一步操作或表达式。
                self.ax3.scatter(self.b_event_amplitude_two, self.b_event_duration_two, edgecolor='black', facecolors='None', label='DNA', s=10)
# 行 2356：条件判断，根据不同条件执行不同分支。
        else:
# 行 2357：原始注释行，提供作者或用途说明。
            # self.ax3.grid(True)
# 行 2358：原始注释行，提供作者或用途说明。
            # self.ax3.set_xticks([])
# 行 2359：原始注释行，提供作者或用途说明。
            # self.ax3.set_yticks([])
# 行 2360：占位语句：暂不执行任何操作。
            pass
# 行 2361：普通语句：执行一步操作或表达式。
        self.canvas_event3.draw()
# 行 2362：定义函数/方法 b_caculate_std(self, data)。
    def b_caculate_std(self, data):
# 行 2363：普通语句：执行一步操作或表达式。
        global data_lock
# 行 2364：代码块开始（如函数、类、条件或循环体）。
        with data_lock:
# 行 2365：原始注释行，提供作者或用途说明。
            # print(len(data))
# 行 2366：变量赋值：保存配置、状态或计算结果。
            window_size = int(0.79 * len(data))
# 行 2367：条件判断，根据不同条件执行不同分支。
            if window_size % 2 == 0:
# 行 2368：普通语句：执行一步操作或表达式。
                window_size += 1
# 行 2369：空行，用于分隔代码块，提高可读性。

# 行 2370：变量赋值：保存配置、状态或计算结果。
            std_values = np.zeros_like(data)
# 行 2371：空行，用于分隔代码块，提高可读性。

# 行 2372：for 循环遍历序列或可迭代对象。
            for i in range(len(data)):
# 行 2373：变量赋值：保存配置、状态或计算结果。
                start_idx = max(0, i - window_size // 2)
# 行 2374：变量赋值：保存配置、状态或计算结果。
                end_idx = min(len(data), i + window_size // 2 + 1)
# 行 2375：变量赋值：保存配置、状态或计算结果。
                window_data = data[start_idx:end_idx]
# 行 2376：普通语句：执行一步操作或表达式。
                std_values[i] = (np.std(window_data))
# 行 2377：打印调试或状态信息到控制台。
                print(std_values)
# 行 2378：返回函数结果。
            return std_values
# 行 2379：空行，用于分隔代码块，提高可读性。

# 行 2380：定义函数/方法 add_collection_setting_page(self)。
    def add_collection_setting_page(self):
# 行 2381：变量赋值：保存配置、状态或计算结果。
        self.collection_setting_page_original = tk.Frame(self.collection_eONE_page_original,
# 行 2382：变量赋值：保存配置、状态或计算结果。
                                                 width=312,
# 行 2383：变量赋值：保存配置、状态或计算结果。
                                                 height=1000,
# 行 2384：变量赋值：保存配置、状态或计算结果。
                                                 bg='#e5e5e5',
# 行 2385：变量赋值：保存配置、状态或计算结果。
                                                 bd=3,
# 行 2386：变量赋值：保存配置、状态或计算结果。
                                                 relief=tk.RAISED)
# 行 2387：普通语句：执行一步操作或表达式。
        self.collection_setting_page_original.pack_propagate(False)
# 行 2388：普通语句：执行一步操作或表达式。
        self.collection_setting_page_original.place(x=0, y=170)
# 行 2389：定义函数/方法 add_collection_picture_page(self)。
    def add_collection_picture_page(self):
# 行 2390：变量赋值：保存配置、状态或计算结果。
        self.collection_picture_page_original = tk.Frame(self.collection_eONE_page_original,
# 行 2391：变量赋值：保存配置、状态或计算结果。
                                                 width=1250,
# 行 2392：变量赋值：保存配置、状态或计算结果。
                                                 height=300,
# 行 2393：变量赋值：保存配置、状态或计算结果。
                                                 bg='#e5e5e5',
# 行 2394：变量赋值：保存配置、状态或计算结果。
                                                 bd=0,
# 行 2395：变量赋值：保存配置、状态或计算结果。
                                                 relief=tk.RAISED)
# 行 2396：普通语句：执行一步操作或表达式。
        self.collection_picture_page_original.pack_propagate(False)
# 行 2397：普通语句：执行一步操作或表达式。
        self.collection_picture_page_original.place(x=315, y=170)
# 行 2398：空行，用于分隔代码块，提高可读性。

# 行 2399：定义函数/方法 add_collection_eONE_page(self)。
    def add_collection_eONE_page(self):
# 行 2400：变量赋值：保存配置、状态或计算结果。
        self.collection_eONE_page_original = tk.Frame(self.root,
# 行 2401：变量赋值：保存配置、状态或计算结果。
                                                 width=1950,
# 行 2402：变量赋值：保存配置、状态或计算结果。
                                                 height=1000,
# 行 2403：变量赋值：保存配置、状态或计算结果。
                                                 bg='#e5e5e5',
# 行 2404：变量赋值：保存配置、状态或计算结果。
                                                 bd=5,
# 行 2405：变量赋值：保存配置、状态或计算结果。
                                                 relief=tk.RAISED)
# 行 2406：普通语句：执行一步操作或表达式。
        self.collection_eONE_page_original.pack_propagate(False)
# 行 2407：普通语句：执行一步操作或表达式。
        self.collection_eONE_page_original.place(x=0, y=0)
# 行 2408：空行，用于分隔代码块，提高可读性。

# 行 2409：普通语句：执行一步操作或表达式。
        self.add_collection_setting_page()
# 行 2410：普通语句：执行一步操作或表达式。
        self.add_collection_picture_page()
# 行 2411：定义函数/方法 get_connect_devices()。
        def get_connect_devices():
# 行 2412：普通语句：执行一步操作或表达式。
            err, devices = c4.detectDevices()
# 行 2413：返回函数结果。
            return devices
# 行 2414：空行，用于分隔代码块，提高可读性。

# 行 2415：定义函数/方法 on_select(i, combo)。
        def on_select(i, combo):
# 行 2416：条件判断，根据不同条件执行不同分支。
            if i == 0:
# 行 2417：变量赋值：保存配置、状态或计算结果。
                list = []
# 行 2418：变量赋值：保存配置、状态或计算结果。
                selected_item = self.b_combo.get()
# 行 2419：普通语句：执行一步操作或表达式。
                list.append(str(selected_item))
# 行 2420：返回函数结果。
                return list
# 行 2421：条件判断，根据不同条件执行不同分支。
            if i == 1:
# 行 2422：变量赋值：保存配置、状态或计算结果。
                selected_item = combo.get()
# 行 2423：条件判断，根据不同条件执行不同分支。
                if selected_item == '正向脉冲':
# 行 2424：变量赋值：保存配置、状态或计算结果。
                    self.b_signal_direction = 1
# 行 2425：条件判断，根据不同条件执行不同分支。
                elif selected_item == '负向脉冲':
# 行 2426：变量赋值：保存配置、状态或计算结果。
                    self.b_signal_direction = 2
# 行 2427：条件判断，根据不同条件执行不同分支。
                else:
# 行 2428：变量赋值：保存配置、状态或计算结果。
                    self.b_signal_direction = 3
# 行 2429：打印调试或状态信息到控制台。
                print(self.b_signal_direction)
# 行 2430：条件判断，根据不同条件执行不同分支。
            if i == 2:
# 行 2431：变量赋值：保存配置、状态或计算结果。
                selected_item = combo.get()
# 行 2432：条件判断，根据不同条件执行不同分支。
                if selected_item == '探针EC-K12':
# 行 2433：变量赋值：保存配置、状态或计算结果。
                    self.b_probe = '探针EC-K12'
# 行 2434：条件判断，根据不同条件执行不同分支。
                if selected_item == '探针LA-DSM20079':
# 行 2435：变量赋值：保存配置、状态或计算结果。
                    self.b_probe = '探针LA-DSM20079'
# 行 2436：条件判断，根据不同条件执行不同分支。
                if selected_item == '探针ST-NCTC12958':
# 行 2437：变量赋值：保存配置、状态或计算结果。
                    self.b_probe = '探针ST-NCTC12958'
# 行 2438：条件判断，根据不同条件执行不同分支。
                if selected_item == 'EC-K12':
# 行 2439：变量赋值：保存配置、状态或计算结果。
                    self.b_probe = 'EC-K12'
# 行 2440：条件判断，根据不同条件执行不同分支。
                if selected_item == 'EC-BL21':
# 行 2441：变量赋值：保存配置、状态或计算结果。
                    self.b_probe = 'EC-BL21'
# 行 2442：条件判断，根据不同条件执行不同分支。
                if selected_item == 'EC-CFT073':
# 行 2443：变量赋值：保存配置、状态或计算结果。
                    self.b_probe = 'EC-CFT073'
# 行 2444：打印调试或状态信息到控制台。
                print(self.b_probe)
# 行 2445：变量赋值：保存配置、状态或计算结果。
            list = []
# 行 2446：变量赋值：保存配置、状态或计算结果。
            selected_item = self.b_combo.get()
# 行 2447：普通语句：执行一步操作或表达式。
            list.append(str(selected_item))
# 行 2448：返回函数结果。
            return list
# 行 2449：空行，用于分隔代码块，提高可读性。

# 行 2450：空行，用于分隔代码块，提高可读性。

# 行 2451：变量赋值：保存配置、状态或计算结果。
        self.canvas_b_connect = tk.Canvas(self.collection_eONE_page_original, width=310, height=165, bg='#e5e5e5', bd=0)
# 行 2452：普通语句：执行一步操作或表达式。
        self.canvas_b_connect.place(x=0, y=0)
# 行 2453：普通语句：执行一步操作或表达式。
        self.create_dashed_square(self.canvas_b_connect, 3, 3, 310, 165, 1)
# 行 2454：空行，用于分隔代码块，提高可读性。

# 行 2455：变量赋值：保存配置、状态或计算结果。
        options = [devices for devices in get_connect_devices()]
# 行 2456：空行，用于分隔代码块，提高可读性。

# 行 2457：变量赋值：保存配置、状态或计算结果。
        self.b_combo = ttk.Combobox(self.collection_eONE_page_original, values=options, width=10, state='readonly')
# 行 2458：普通语句：执行一步操作或表达式。
        self.b_combo.current()
# 行 2459：普通语句：执行一步操作或表达式。
        self.b_combo.bind("<<ComboboxSelected>>", lambda event: on_select(0, self.b_combo))
# 行 2460：普通语句：执行一步操作或表达式。
        self.b_combo.place(x=125, y=49)
# 行 2461：空行，用于分隔代码块，提高可读性。

# 行 2462：变量赋值：保存配置、状态或计算结果。
        self.b_connect_state = create_label1(self.collection_eONE_page_original, '连接状态', 10, 15)
# 行 2463：普通语句：执行一步操作或表达式。
        self.creat_color_button(6, self.collection_eONE_page_original, 100, 13)
# 行 2464：空行，用于分隔代码块，提高可读性。

# 行 2465：变量赋值：保存配置、状态或计算结果。
        self.b_data_state = create_label1(self.collection_eONE_page_original, '数据存储', 140, 15)
# 行 2466：空行，用于分隔代码块，提高可读性。

# 行 2467：变量赋值：保存配置、状态或计算结果。
        self.b_device_get = create_button(self.collection_eONE_page_original, '获取设备序列号',
# 行 2468：普通语句：执行一步操作或表达式。
                                          lambda: get_connect_devices(), 14, 1, 7, 47)
# 行 2469：空行，用于分隔代码块，提高可读性。

# 行 2470：普通语句：执行一步操作或表达式。
        self.creat_color_button(7, self.collection_eONE_page_original, 220, 13)
# 行 2471：变量赋值：保存配置、状态或计算结果。
        self.b_connect_device = create_button(self.collection_eONE_page_original, '连接', lambda: self.b_connect_devices(1), 30, 1, 7, 85)
# 行 2472：变量赋值：保存配置、状态或计算结果。
        self.b_dis_connect_device = create_button(self.collection_eONE_page_original, '断开连接', lambda: self.b_disconnect_devices(), 30,1, 7, 121)
# 行 2473：定义函数/方法 b_draw_triangle(canvas)。
        def b_draw_triangle(canvas):
# 行 2474：普通语句：执行一步操作或表达式。
            canvas.delete("shape")
# 行 2475：条件判断，根据不同条件执行不同分支。
            if self.b_start_detection_param == False:
# 行 2476：普通语句：执行一步操作或表达式。
                canvas.create_polygon(0, 1, 31, 16, 0, 31, fill='green', outline='black', tags='shape')
# 行 2477：原始注释行，提供作者或用途说明。
                # canvas.tag_bind('shape', '<Button-1>', lambda event: None)
# 行 2478：条件判断，根据不同条件执行不同分支。
            else:
# 行 2479：普通语句：执行一步操作或表达式。
                canvas.create_rectangle(0, 0, 31, 31, fill='black', outline='black', tags='shape')
# 行 2480：打印调试或状态信息到控制台。
                print(2)
# 行 2481：变量赋值：保存配置、状态或计算结果。
        self.b_save_file = create_button(self.collection_eONE_page_original, '保存路径', self.b_open_new_window, 7, 1,245, 47)
# 行 2482：普通语句：执行一步操作或表达式。
        '''三角形'''
# 行 2483：变量赋值：保存配置、状态或计算结果。
        self.b_start_detection_param = False
# 行 2484：变量赋值：保存配置、状态或计算结果。
        self.b_save_number = False
# 行 2485：变量赋值：保存配置、状态或计算结果。
        self.b_save_txt_head = False
# 行 2486：变量赋值：保存配置、状态或计算结果。
        self.canvas_b_process_start = tk.Canvas(self.collection_eONE_page_original, width=31, height=31, bg='#e5e5e5', bd=0)
# 行 2487：普通语句：执行一步操作或表达式。
        self.canvas_b_process_start.place(x=260, y=105)
# 行 2488：普通语句：执行一步操作或表达式。
        b_draw_triangle(self.canvas_b_process_start)
# 行 2489：普通语句：执行一步操作或表达式。
        self.canvas_b_process_start.tag_bind('shape', '<Button-1>',
# 行 2490：普通语句：执行一步操作或表达式。
                                          lambda event: b_start_detection( self.canvas_b_process_start))
# 行 2491：空行，用于分隔代码块，提高可读性。

# 行 2492：定义函数/方法 b_start_detection(canvas_ts)。
        def b_start_detection(canvas_ts):
# 行 2493：变量赋值：保存配置、状态或计算结果。
            self.b_start_detection_param = not self.b_start_detection_param
# 行 2494：变量赋值：保存配置、状态或计算结果。
            self.b_start_time_1_number = not self.b_start_time_1_number
# 行 2495：变量赋值：保存配置、状态或计算结果。
            self.b_signal_number = float(self.b_detection_sigma_param.get())
# 行 2496：普通语句：执行一步操作或表达式。
            b_draw_triangle(self.canvas_b_process_start)
# 行 2497：变量赋值：保存配置、状态或计算结果。
            self.b_save_number = not self.b_save_number
# 行 2498：变量赋值：保存配置、状态或计算结果。
            self.b_save_txt_head = not self.b_save_txt_head
# 行 2499：打印调试或状态信息到控制台。
            print(self.b_start_detection_param, self.b_save_number , self.b_save_txt_head)
# 行 2500：变量赋值：保存配置、状态或计算结果。
        self.canvas_b_setting = tk.Canvas(self.collection_eONE_page_original, width=1595, height=165, bg='#e5e5e5', bd=0)
# 行 2501：普通语句：执行一步操作或表达式。
        self.canvas_b_setting.place(x=312, y=0)
# 行 2502：普通语句：执行一步操作或表达式。
        self.create_dashed_square(self.canvas_b_setting, 3, 3, 1250, 165, 1)
# 行 2503：普通语句：执行一步操作或表达式。
        self.create_dashed_line(self.canvas_b_setting, 160, 10, 160, 155, 2)
# 行 2504：普通语句：执行一步操作或表达式。
        self.create_dashed_line(self.canvas_b_setting, 590, 10, 590, 155, 2)
# 行 2505：普通语句：执行一步操作或表达式。
        self.create_dashed_line(self.canvas_b_setting, 730, 10, 730, 155, 2)
# 行 2506：空行，用于分隔代码块，提高可读性。

# 行 2507：定义函数/方法 b_apply_voltage(bools, number)。
        def b_apply_voltage(bools, number):
# 行 2508：条件判断，根据不同条件执行不同分支。
            if bools == True:
# 行 2509：空行，用于分隔代码块，提高可读性。

# 行 2510：普通语句：执行一步操作或表达式。
                c4.selectVoltageProtocol(0)
# 行 2511：空行，用于分隔代码块，提高可读性。

# 行 2512：变量赋值：保存配置、状态或计算结果。
                v_hold = c4.Measurement(int(number), c4.UnitPfxMilli, "V")
# 行 2513：普通语句：执行一步操作或表达式。
                c4.setProtocolVoltage(0, v_hold)
# 行 2514：普通语句：执行一步操作或表达式。
                c4.applyVoltageProtocol()
# 行 2515：空行，用于分隔代码块，提高可读性。

# 行 2516：条件判断，根据不同条件执行不同分支。
            else:
# 行 2517：普通语句：执行一步操作或表达式。
                c4.selectVoltageProtocol(0)
# 行 2518：空行，用于分隔代码块，提高可读性。

# 行 2519：变量赋值：保存配置、状态或计算结果。
                v_hold = c4.Measurement(0, c4.UnitPfxMilli, "V")
# 行 2520：普通语句：执行一步操作或表达式。
                c4.setProtocolVoltage(0, v_hold)
# 行 2521：空行，用于分隔代码块，提高可读性。

# 行 2522：普通语句：执行一步操作或表达式。
                c4.applyVoltageProtocol()
# 行 2523：定义函数/方法 b_voltage_up_down(number)。
        def b_voltage_up_down(number):
# 行 2524：原始注释行，提供作者或用途说明。
            # def loop_step(i=0, num=1):
# 行 2525：原始注释行，提供作者或用途说明。
            #     if i < 5:
# 行 2526：原始注释行，提供作者或用途说明。
            #         c4.selectVoltageProtocol(0)
# 行 2527：原始注释行，提供作者或用途说明。
            #         v_hold = c4.Measurement(int(number) * num, c4.UnitPfxMilli, "V")
# 行 2528：原始注释行，提供作者或用途说明。
            #         c4.setProtocolVoltage(0, v_hold)
# 行 2529：原始注释行，提供作者或用途说明。
            #         c4.applyVoltageProtocol()
# 行 2530：原始注释行，提供作者或用途说明。
            #
# 行 2531：原始注释行，提供作者或用途说明。
            #         next_num = num * -1
# 行 2532：原始注释行，提供作者或用途说明。
            #         next_i = i + 1
# 行 2533：原始注释行，提供作者或用途说明。
            #         self.root.after(1000, loop_step, next_i, next_num)
# 行 2534：原始注释行，提供作者或用途说明。
            # loop_step()
# 行 2535：普通语句：执行一步操作或表达式。
            c4.selectVoltageProtocol(0)
# 行 2536：变量赋值：保存配置、状态或计算结果。
            v_hold = c4.Measurement(int(number) * -1, c4.UnitPfxMilli, "V")
# 行 2537：普通语句：执行一步操作或表达式。
            c4.setProtocolVoltage(0, v_hold)
# 行 2538：普通语句：执行一步操作或表达式。
            c4.applyVoltageProtocol()
# 行 2539：普通语句：执行一步操作或表达式。
            self.b_bisa_voltage_param.delete(0, "end")
# 行 2540：普通语句：执行一步操作或表达式。
            self.b_bisa_voltage_param.insert(0, int(number) * -1)
# 行 2541：变量赋值：保存配置、状态或计算结果。
        self.b_bisa_voltage = create_label1(self.collection_eONE_page_original, '偏置电压', 350, 15)
# 行 2542：变量赋值：保存配置、状态或计算结果。
        self.b_bisa_voltage_param = create_entry1(self.collection_eONE_page_original, 10, 350, 50, 0)
# 行 2543：变量赋值：保存配置、状态或计算结果。
        self.b_bisa_voltage_mv = create_label(self.collection_eONE_page_original, 'mV', 430, 50)
# 行 2544：变量赋值：保存配置、状态或计算结果。
        self.b_bisa_voltage_apply = create_button(self.collection_eONE_page_original, '应用', lambda: b_apply_voltage(True,  self.b_bisa_voltage_param.get()),
# 行 2545：普通语句：执行一步操作或表达式。
                                              12, 1, 340, 85)
# 行 2546：变量赋值：保存配置、状态或计算结果。
        self.b_bisa_voltage_clear = create_button(self.collection_eONE_page_original, '清除', lambda: b_apply_voltage(False, 1),
# 行 2547：普通语句：执行一步操作或表达式。
                                              12, 1, 340, 121)
# 行 2548：变量赋值：保存配置、状态或计算结果。
        self.b_bisa_voltage_up_down = create_button(self.collection_eONE_page_original, '±', lambda: b_voltage_up_down(self.b_bisa_voltage_param.get()),
# 行 2549：普通语句：执行一步操作或表达式。
                                              2, 1, 440, 85)
# 行 2550：定义函数/方法 calculate_d(V ,A, sigma, t)。
        def calculate_d(V ,A, sigma, t):
# 行 2551：变量赋值：保存配置、状态或计算结果。
            G = 1 / ((V * 1e-3) / (A * 1e-9))
# 行 2552：变量赋值：保存配置、状态或计算结果。
            t = round(t * 1e-9, 9)
# 行 2553：变量赋值：保存配置、状态或计算结果。
            d = (G / (2 * sigma)) * (1 + math.sqrt(1 + (16 * sigma * t) / (math.pi * G)))
# 行 2554：普通语句：执行一步操作或表达式。
            self.b_caculate_d_param.config(state=tk.NORMAL)
# 行 2555：普通语句：执行一步操作或表达式。
            self.b_caculate_d_param.delete(0, tk.END)
# 行 2556：普通语句：执行一步操作或表达式。
            self.b_caculate_d_param.insert(0, round(d * 1e9, 4))
# 行 2557：普通语句：执行一步操作或表达式。
            self.b_caculate_d_param.config(state=tk.DISABLED)
# 行 2558：空行，用于分隔代码块，提高可读性。

# 行 2559：变量赋值：保存配置、状态或计算结果。
        self.b_d_pore_caculate = create_label1(self.collection_eONE_page_original, '纳米孔径计算', 550, 15)
# 行 2560：变量赋值：保存配置、状态或计算结果。
        self.b_caculate_bais = create_label(self.collection_eONE_page_original, '外加电压', 500, 50)
# 行 2561：变量赋值：保存配置、状态或计算结果。
        self.b_caculate_bais_param = create_entry1(self.collection_eONE_page_original, 10, 590, 50, 10)
# 行 2562：变量赋值：保存配置、状态或计算结果。
        self.b_caculate_bais_mv = create_label(self.collection_eONE_page_original, 'mV', 665, 50)
# 行 2563：变量赋值：保存配置、状态或计算结果。
        self.b_caculate_current = create_label(self.collection_eONE_page_original, '响应电流', 500, 85)
# 行 2564：变量赋值：保存配置、状态或计算结果。
        self.b_caculate_current_param = create_entry1(self.collection_eONE_page_original, 10, 590, 85, 60)
# 行 2565：变量赋值：保存配置、状态或计算结果。
        self.b_caculate_current_nA = create_label(self.collection_eONE_page_original, 'nA', 665, 85)
# 行 2566：变量赋值：保存配置、状态或计算结果。
        self.b_caculate_solutiuon = create_label(self.collection_eONE_page_original, '溶液电导率', 500, 121)
# 行 2567：变量赋值：保存配置、状态或计算结果。
        self.b_caculate_solution_param = create_entry1(self.collection_eONE_page_original, 10, 590, 121, 11.27)
# 行 2568：变量赋值：保存配置、状态或计算结果。
        self.b_caculate_solution_sm = create_label(self.collection_eONE_page_original, 's/m', 665, 121)
# 行 2569：变量赋值：保存配置、状态或计算结果。
        self.b_caculate_h = create_label(self.collection_eONE_page_original, '膜厚', 720, 50)
# 行 2570：变量赋值：保存配置、状态或计算结果。
        self.b_caculate_h_param = create_entry1(self.collection_eONE_page_original, 10, 765, 50, 30)
# 行 2571：变量赋值：保存配置、状态或计算结果。
        self.b_caculate_h_nm = create_label(self.collection_eONE_page_original, 'nm', 840, 50)
# 行 2572：变量赋值：保存配置、状态或计算结果。
        self.b_cacilate_d_button = create_button(self.collection_eONE_page_original, '计算', lambda: calculate_d(int(self.b_caculate_bais_param.get()), float(self.b_caculate_current_param.get()),
# 行 2573：普通语句：执行一步操作或表达式。
                                                                                                            float(self.b_caculate_solution_param.get()), float(self.b_caculate_h_param.get())),
# 行 2574：普通语句：执行一步操作或表达式。
                                                  20, 1, 720, 80)
# 行 2575：变量赋值：保存配置、状态或计算结果。
        self.b_caculate_d = create_label(self.collection_eONE_page_original, '孔径', 720, 121)
# 行 2576：变量赋值：保存配置、状态或计算结果。
        self.b_caculate_d_param = create_entry1(self.collection_eONE_page_original, 10, 765, 121, '#')
# 行 2577：普通语句：执行一步操作或表达式。
        self.b_caculate_d_param.config(state=tk.DISABLED)
# 行 2578：变量赋值：保存配置、状态或计算结果。
        self.b_caculate_d_nm = create_label(self.collection_eONE_page_original, 'nm', 840, 121)
# 行 2579：空行，用于分隔代码块，提高可读性。

# 行 2580：定义函数/方法 b_baseline_compensate()。
        def b_baseline_compensate():
# 行 2581：变量赋值：保存配置、状态或计算结果。
            self.b_baseline_compensate_param = not self.b_baseline_compensate_param
# 行 2582：条件判断，根据不同条件执行不同分支。
            if self.b_baseline_compensate_param == True:
# 行 2583：普通语句：执行一步操作或表达式。
                c4.digitalOffsetCompensation(self.b_i_channels, True)
# 行 2584：条件判断，根据不同条件执行不同分支。
            else:
# 行 2585：普通语句：执行一步操作或表达式。
                c4.digitalOffsetCompensation(self.b_i_channels, False)
# 行 2586：变量赋值：保存配置、状态或计算结果。
        self.b_var_baseline_compensate = tk.IntVar()
# 行 2587：变量赋值：保存配置、状态或计算结果。
        self.b_baseline_compensate_choice = tk.Checkbutton(self.collection_eONE_page_original, variable=self.b_var_baseline_compensate,
# 行 2588：变量赋值：保存配置、状态或计算结果。
                                                  font=("Arial", 1), command=lambda: b_baseline_compensate())
# 行 2589：普通语句：执行一步操作或表达式。
        self.b_baseline_compensate_choice.place(x=920, y=15)
# 行 2590：变量赋值：保存配置、状态或计算结果。
        self.b_baseline_compensate = create_label(self.collection_eONE_page_original, '基线补偿', 950, 15)
# 行 2591：空行，用于分隔代码块，提高可读性。

# 行 2592：变量赋值：保存配置、状态或计算结果。
        self.b_display_std = create_label(self.collection_eONE_page_original, '均方根噪声:', 920, 50)
# 行 2593：空行，用于分隔代码块，提高可读性。

# 行 2594：变量赋值：保存配置、状态或计算结果。
        self.b_display_rms = create_label(self.collection_eONE_page_original, '###', 920, 85)
# 行 2595：空行，用于分隔代码块，提高可读性。

# 行 2596：变量赋值：保存配置、状态或计算结果。
        self.b_display_mean_baseline = create_label(self.collection_eONE_page_original, '基线:###', 925, 121)
# 行 2597：空行，用于分隔代码块，提高可读性。

# 行 2598：变量赋值：保存配置、状态或计算结果。
        self.b_feature_extraction = create_label1(self.collection_eONE_page_original, '过孔事件特征提取', 1100, 15)
# 行 2599：空行，用于分隔代码块，提高可读性。

# 行 2600：空行，用于分隔代码块，提高可读性。

# 行 2601：变量赋值：保存配置、状态或计算结果。
        self.b_up_or_down = create_label(self.collection_eONE_page_original, '事件类型:', 1080, 50)
# 行 2602：变量赋值：保存配置、状态或计算结果。
        options1 = ['负向脉冲', '正向脉冲', '双向脉冲']
# 行 2603：空行，用于分隔代码块，提高可读性。

# 行 2604：变量赋值：保存配置、状态或计算结果。
        self.b_combo1 = ttk.Combobox(self.collection_eONE_page_original, values=options1, width=10, state='readonly')
# 行 2605：普通语句：执行一步操作或表达式。
        self.b_combo1.current(0)
# 行 2606：普通语句：执行一步操作或表达式。
        self.b_combo1.bind("<<ComboboxSelected>>", lambda event: on_select(1, self.b_combo1))
# 行 2607：普通语句：执行一步操作或表达式。
        self.b_combo1.place(x=1160, y=49)
# 行 2608：空行，用于分隔代码块，提高可读性。

# 行 2609：变量赋值：保存配置、状态或计算结果。
        self.b_detection_sigma = create_label(self.collection_eONE_page_original, '检测阈值（σ）:', 1080, 85)
# 行 2610：变量赋值：保存配置、状态或计算结果。
        self.b_detection_sigma_param = create_entry1(self.collection_eONE_page_original, 10, 1210, 85, 5)
# 行 2611：定义函数/方法 b_detection_TF()。
        def b_detection_TF():
# 行 2612：变量赋值：保存配置、状态或计算结果。
            self.b_signal_number = float(self.b_detection_sigma_param.get())
# 行 2613：变量赋值：保存配置、状态或计算结果。
            self.b_detection_param = not self.b_detection_param
# 行 2614：打印调试或状态信息到控制台。
            print(self.b_detection_param)
# 行 2615：变量赋值：保存配置、状态或计算结果。
        self.b_var_detection_sigma = tk.IntVar()
# 行 2616：变量赋值：保存配置、状态或计算结果。
        self.b_detection_sigma_choice = tk.Checkbutton(self.collection_eONE_page_original,
# 行 2617：变量赋值：保存配置、状态或计算结果。
                                                           variable=self.b_var_detection_sigma,
# 行 2618：变量赋值：保存配置、状态或计算结果。
                                                           font=("Arial", 1), command=lambda: b_detection_TF())
# 行 2619：普通语句：执行一步操作或表达式。
        self.b_detection_sigma_choice.place(x=1085, y=121)
# 行 2620：变量赋值：保存配置、状态或计算结果。
        self.b_detection_sigma_look = create_label(self.collection_eONE_page_original, '检测阈值预览', 1110, 121)
# 行 2621：空行，用于分隔代码块，提高可读性。

# 行 2622：原始注释行，提供作者或用途说明。
        # self.b_detection_mintime = create_label(self.collection_eONE_page_original, '最小持续时间:', 1300, 50)
# 行 2623：原始注释行，提供作者或用途说明。
        # self.b_detection_mintime_param = create_entry1(self.collection_eONE_page_original, 10, 1410, 50, 0)
# 行 2624：原始注释行，提供作者或用途说明。
        # self.b_detection_mintime_ms = create_label(self.collection_eONE_page_original, 'ms', 1480, 50)
# 行 2625：原始注释行，提供作者或用途说明。
        #
# 行 2626：原始注释行，提供作者或用途说明。
        # self.b_detection_maxtime = create_label(self.collection_eONE_page_original, '最大持续时间:', 1300, 85)
# 行 2627：原始注释行，提供作者或用途说明。
        # self.b_detection_maxtime_param = create_entry1(self.collection_eONE_page_original, 10, 1410, 85, 1000)
# 行 2628：原始注释行，提供作者或用途说明。
        # self.b_detection_maxtime_ms = create_label(self.collection_eONE_page_original, 'ms', 1480, 85)
# 行 2629：空行，用于分隔代码块，提高可读性。

# 行 2630：变量赋值：保存配置、状态或计算结果。
        self.b_detection_baseline_std_window = create_label(self.collection_eONE_page_original, '基线/std计算步长:', 1300, 121)
# 行 2631：变量赋值：保存配置、状态或计算结果。
        self.b_detection_baseline_std_window_param = create_entry1(self.collection_eONE_page_original, 10, 1440, 121, 20)
# 行 2632：变量赋值：保存配置、状态或计算结果。
        self.b_detection_baseline_std_window_ms = create_label(self.collection_eONE_page_original, 'ms', 1510, 121)
# 行 2633：空行，用于分隔代码块，提高可读性。

# 行 2634：原始注释行，提供作者或用途说明。
        # self.b_undate_seven_features = create_button(self.collection_eONE_page_original, '指纹谱更新',
# 行 2635：原始注释行，提供作者或用途说明。
        #                                              lambda: self.update_seven_features(self.b_event_list_1,
# 行 2636：原始注释行，提供作者或用途说明。
        #                                                                                 self.b_event_list_2,
# 行 2637：原始注释行，提供作者或用途说明。
        #                                                                                 self.b_event_list_3,
# 行 2638：原始注释行，提供作者或用途说明。
        #                                                                                 self.b_event_list_4,
# 行 2639：原始注释行，提供作者或用途说明。
        #                                                                                 self.b_event_list_5,
# 行 2640：原始注释行，提供作者或用途说明。
        #                                                                                 self.b_event_list_6,
# 行 2641：原始注释行，提供作者或用途说明。
        #                                                                                 self.b_event_list_7),
# 行 2642：原始注释行，提供作者或用途说明。
        #                                              12, 1, 490, 880)
# 行 2643：原始注释行，提供作者或用途说明。
        # self.update_seven_features(self.b_event_list_1, self.b_event_list_2, self.b_event_list_3, self.b_event_list_4, self.b_event_list_5,
# 行 2644：原始注释行，提供作者或用途说明。
        #                            self.b_event_list_6, self.b_event_list_7)
# 行 2645：定义函数/方法 b_clear_table()。
        def b_clear_table():
# 行 2646：for 循环遍历序列或可迭代对象。
            for item in self.tree.get_children():
# 行 2647：普通语句：执行一步操作或表达式。
                self.tree.delete(item)
# 行 2648：变量赋值：保存配置、状态或计算结果。
            self.b_event_number = 0
# 行 2649：变量赋值：保存配置、状态或计算结果。
            self.b_event_amplitude = []
# 行 2650：变量赋值：保存配置、状态或计算结果。
            self.b_event_duration = []
# 行 2651：普通语句：执行一步操作或表达式。
            self.update_plot_event_2D(self.b_event_amplitude, self.b_event_duration)
# 行 2652：变量赋值：保存配置、状态或计算结果。
            self.b_event_list_1 = []
# 行 2653：变量赋值：保存配置、状态或计算结果。
            self.b_event_list_2 = []
# 行 2654：变量赋值：保存配置、状态或计算结果。
            self.b_event_list_3 = []
# 行 2655：变量赋值：保存配置、状态或计算结果。
            self.b_event_list_4 = []
# 行 2656：变量赋值：保存配置、状态或计算结果。
            self.b_event_list_5 = []
# 行 2657：变量赋值：保存配置、状态或计算结果。
            self.b_event_list_6 = []
# 行 2658：变量赋值：保存配置、状态或计算结果。
            self.b_event_list_7 = []
# 行 2659：变量赋值：保存配置、状态或计算结果。
            self.b_event_list_all = []
# 行 2660：变量赋值：保存配置、状态或计算结果。
            self.b_event_list_two = []
# 行 2661：变量赋值：保存配置、状态或计算结果。
            self.b_event_list_two_len = 0
# 行 2662：变量赋值：保存配置、状态或计算结果。
            self.b_event_two_number_index = []
# 行 2663：变量赋值：保存配置、状态或计算结果。
        self.b_clear_frame = create_button(self.collection_eONE_page_original, '清空列表',
# 行 2664：普通语句：执行一步操作或表达式。
                                           lambda: b_clear_table(),
# 行 2665：普通语句：执行一步操作或表达式。
                                           12, 1, 490, 780)
# 行 2666：定义函数/方法 b_scroll_choose()。
        def b_scroll_choose():
# 行 2667：变量赋值：保存配置、状态或计算结果。
            self.b_scroll_param = not self.b_scroll_param
# 行 2668：变量赋值：保存配置、状态或计算结果。
        self.b_scroll_list = tk.IntVar()
# 行 2669：变量赋值：保存配置、状态或计算结果。
        self.b_scroll_list_choose = tk.Checkbutton(self.collection_eONE_page_original,
# 行 2670：变量赋值：保存配置、状态或计算结果。
                                                       variable=self.b_scroll_list,
# 行 2671：变量赋值：保存配置、状态或计算结果。
                                                       font=("Arial", 1), command=lambda: b_scroll_choose())
# 行 2672：普通语句：执行一步操作或表达式。
        self.b_scroll_list_choose.place(x=330, y=785)
# 行 2673：变量赋值：保存配置、状态或计算结果。
        self.b_scroll_list_look = create_label(self.collection_eONE_page_original, '滚动更新', 350, 785)
# 行 2674：变量赋值：保存配置、状态或计算结果。
        self.probe_label = create_label(self.collection_eONE_page_original, '选择类型:', 1300, 15)
# 行 2675：空行，用于分隔代码块，提高可读性。

# 行 2676：变量赋值：保存配置、状态或计算结果。
        options2 = ['探针EC-K12', '探针LA-DSM20079', '探针ST-NCTC12958', 'EC-K12', 'EC-BL21', 'EC-CFT073']
# 行 2677：空行，用于分隔代码块，提高可读性。

# 行 2678：变量赋值：保存配置、状态或计算结果。
        self.b_combo2 = ttk.Combobox(self.collection_eONE_page_original, values=options2, width=15, state='readonly')
# 行 2679：普通语句：执行一步操作或表达式。
        self.b_combo2.current(0)
# 行 2680：普通语句：执行一步操作或表达式。
        self.b_combo2.bind("<<ComboboxSelected>>", lambda event: on_select(2, self.b_combo2))
# 行 2681：普通语句：执行一步操作或表达式。
        self.b_combo2.place(x=1420, y=15)
# 行 2682：原始注释行，提供作者或用途说明。
        # self.
# 行 2683：变量赋值：保存配置、状态或计算结果。
        self.header_list = ['事件数', '基线(nA)', '振幅(nA)', '积分(10-9C)', '持续时间(ms)',
# 行 2684：普通语句：执行一步操作或表达式。
                            '钝度', '脉冲不对称', '横轴惯性', '纵轴惯性']
# 行 2685：普通语句：执行一步操作或表达式。
        self.b_show_table_tk()
# 行 2686：空行，用于分隔代码块，提高可读性。

# 行 2687：变量赋值：保存配置、状态或计算结果。
        self.b_event_two_display_label = create_label1(self.collection_eONE_page_original, '检测到的事件序号', 1650, 730)
# 行 2688：定义函数/方法 b_text_display_func()。
        def b_text_display_func():
# 行 2689：变量赋值：保存配置、状态或计算结果。
            self.b_text_display = tk.Text(self.collection_eONE_page_original, width=30, height=15, wrap=tk.WORD, bg="#f0f0f0",
# 行 2690：变量赋值：保存配置、状态或计算结果。
                                          font=("", 10), state=tk.DISABLED)
# 行 2691：普通语句：执行一步操作或表达式。
            self.b_text_display.place(x=1630, y=760)
# 行 2692：空行，用于分隔代码块，提高可读性。

# 行 2693：原始注释行，提供作者或用途说明。
            # self.b_scrollbar = tk.Scrollbar(self.collection_eONE_page_original, command=None)
# 行 2694：原始注释行，提供作者或用途说明。
            # self.b_scrollbar.place(x=1385, y=50, height=400)
# 行 2695：原始注释行，提供作者或用途说明。
            # self.b_text_display.config(yscrollcommand=self.b_scrollbar.set)
# 行 2696：空行，用于分隔代码块，提高可读性。

# 行 2697：普通语句：执行一步操作或表达式。
            self.b_text_display.bind("<Double-1>", None)
# 行 2698：普通语句：执行一步操作或表达式。
        self.plot_event1()
# 行 2699：普通语句：执行一步操作或表达式。
        self.plot_event_2D()
# 行 2700：原始注释行，提供作者或用途说明。
        # self.plot_seven_features()
# 行 2701：普通语句：执行一步操作或表达式。
        self.plot_current()
# 行 2702：普通语句：执行一步操作或表达式。
        b_text_display_func()
# 行 2703：变量赋值：保存配置、状态或计算结果。
        current_selection_var = tk.StringVar(value="200 pA")
# 行 2704：变量赋值：保存配置、状态或计算结果。
        sampling_selection_var = tk.StringVar(value="1.25 KHZ")
# 行 2705：变量赋值：保存配置、状态或计算结果。
        bandwidth_selection_var = tk.StringVar(value="SR/2")
# 行 2706：变量赋值：保存配置、状态或计算结果。
        volt_selection_var = tk.StringVar(value="1")
# 行 2707：空行，用于分隔代码块，提高可读性。

# 行 2708：定义函数/方法 toggle_expand1()。
        def toggle_expand1():
# 行 2709：条件判断，根据不同条件执行不同分支。
            if self.b_source_current_range:
# 行 2710：普通语句：执行一步操作或表达式。
                self.b_expandable_frame1.pack_forget()
# 行 2711：普通语句：执行一步操作或表达式。
                self.b_extend_current.config(text="▶电流量程")
# 行 2712：变量赋值：保存配置、状态或计算结果。
                self.b_source_current_range = False
# 行 2713：变量赋值：保存配置、状态或计算结果。
                self.b_event_two_open = False
# 行 2714：条件判断，根据不同条件执行不同分支。
            else:
# 行 2715：普通语句：执行一步操作或表达式。
                self.b_expandable_frame1.pack(fill=tk.X, padx=20, pady=10)
# 行 2716：普通语句：执行一步操作或表达式。
                self.b_extend_current.config(text="▼电流量程")
# 行 2717：变量赋值：保存配置、状态或计算结果。
                self.b_source_current_range = True
# 行 2718：变量赋值：保存配置、状态或计算结果。
                self.b_event_two_open = True
# 行 2719：变量赋值：保存配置、状态或计算结果。
                self.b_event_1_time = True
# 行 2720：条件判断，根据不同条件执行不同分支。
                if not self.b_default_selection_done_1:
# 行 2721：普通语句：执行一步操作或表达式。
                    self.b_radio1_current.select()
# 行 2722：变量赋值：保存配置、状态或计算结果。
                    self.b_default_selection_done_1 = True
# 行 2723：普通语句：执行一步操作或表达式。
            push_down_main_content()
# 行 2724：打印调试或状态信息到控制台。
            print(self.b_event_two_open)
# 行 2725：定义函数/方法 toggle_expand2()。
        def toggle_expand2():
# 行 2726：条件判断，根据不同条件执行不同分支。
            if self.b_source_sampling_range:
# 行 2727：普通语句：执行一步操作或表达式。
                self.b_expandable_frame2.pack_forget()
# 行 2728：普通语句：执行一步操作或表达式。
                self.b_extend_sampling.config(text="▶采样率")
# 行 2729：变量赋值：保存配置、状态或计算结果。
                self.b_source_sampling_range = False
# 行 2730：条件判断，根据不同条件执行不同分支。
            else:
# 行 2731：普通语句：执行一步操作或表达式。
                self.b_expandable_frame2.pack(fill=tk.X, padx=20, pady=10)
# 行 2732：普通语句：执行一步操作或表达式。
                self.b_extend_sampling.config(text="▼采样率")
# 行 2733：变量赋值：保存配置、状态或计算结果。
                self.b_source_sampling_range = True
# 行 2734：条件判断，根据不同条件执行不同分支。
                if not self.b_default_selection_done_2:
# 行 2735：普通语句：执行一步操作或表达式。
                    self.b_radio1_sampling.select()
# 行 2736：变量赋值：保存配置、状态或计算结果。
                    self.b_default_selection_done_2 = True
# 行 2737：普通语句：执行一步操作或表达式。
            push_down_main_content()
# 行 2738：定义函数/方法 toggle_expand3()。
        def toggle_expand3():
# 行 2739：条件判断，根据不同条件执行不同分支。
            if self.b_source_bandwidth_range:
# 行 2740：普通语句：执行一步操作或表达式。
                self.b_expandable_frame3.pack_forget()
# 行 2741：普通语句：执行一步操作或表达式。
                self.b_extend_bandwidth.config(text="▶低通滤波")
# 行 2742：变量赋值：保存配置、状态或计算结果。
                self.b_source_bandwidth_range = False
# 行 2743：条件判断，根据不同条件执行不同分支。
            else:
# 行 2744：普通语句：执行一步操作或表达式。
                self.b_expandable_frame3.pack(fill=tk.X, padx=20, pady=10)
# 行 2745：普通语句：执行一步操作或表达式。
                self.b_extend_bandwidth.config(text="▼低通滤波")
# 行 2746：变量赋值：保存配置、状态或计算结果。
                self.b_source_bandwidth_range = True
# 行 2747：条件判断，根据不同条件执行不同分支。
                if not self.b_default_selection_done_3:
# 行 2748：普通语句：执行一步操作或表达式。
                    self.b_radio1_bandwidth.select()
# 行 2749：变量赋值：保存配置、状态或计算结果。
                    self.b_default_selection_done_3 = True
# 行 2750：空行，用于分隔代码块，提高可读性。

# 行 2751：普通语句：执行一步操作或表达式。
            push_down_main_content()
# 行 2752：空行，用于分隔代码块，提高可读性。

# 行 2753：原始注释行，提供作者或用途说明。
        # def toggle_expand4():
# 行 2754：原始注释行，提供作者或用途说明。
        #     if self.b_source_volt_range:
# 行 2755：原始注释行，提供作者或用途说明。
        #         self.b_expandable_frame1.pack_forget()
# 行 2756：原始注释行，提供作者或用途说明。
        #         self.b_extend_volt.config(text="▶电压")
# 行 2757：原始注释行，提供作者或用途说明。
        #         self.b_source_volt_range = False
# 行 2758：原始注释行，提供作者或用途说明。
        #     else:
# 行 2759：原始注释行，提供作者或用途说明。
        #         self.b_expandable_frame4.pack(fill=tk.X, padx=20, pady=10)
# 行 2760：原始注释行，提供作者或用途说明。
        #         self.b_extend_volt.config(text="▼电压")
# 行 2761：原始注释行，提供作者或用途说明。
        #         self.b_source_volt_range = True
# 行 2762：原始注释行，提供作者或用途说明。
        #         if not self.b_default_selection_done_4:
# 行 2763：原始注释行，提供作者或用途说明。
        #             # 设置默认选中状态为 "200 pA"
# 行 2764：原始注释行，提供作者或用途说明。
        #             self.b_radio1_volt.select()
# 行 2765：原始注释行，提供作者或用途说明。
        #             self.b_default_selection_done_4 = True
# 行 2766：原始注释行，提供作者或用途说明。
        #     push_down_main_content()
# 行 2767：空行，用于分隔代码块，提高可读性。

# 行 2768：定义函数/方法 push_down_main_content()。
        def push_down_main_content():
# 行 2769：普通语句：执行一步操作或表达式。
            self.b_extend_current.pack_forget()
# 行 2770：普通语句：执行一步操作或表达式。
            self.b_extend_sampling.pack_forget()
# 行 2771：普通语句：执行一步操作或表达式。
            self.b_extend_bandwidth.pack_forget()
# 行 2772：原始注释行，提供作者或用途说明。
            # self.b_extend_volt.pack_forget()
# 行 2773：普通语句：执行一步操作或表达式。
            self.b_expandable_frame1.pack_forget()
# 行 2774：普通语句：执行一步操作或表达式。
            self.b_expandable_frame2.pack_forget()
# 行 2775：普通语句：执行一步操作或表达式。
            self.b_expandable_frame3.pack_forget()
# 行 2776：原始注释行，提供作者或用途说明。
            # self.b_expandable_frame4.pack_forget()
# 行 2777：空行，用于分隔代码块，提高可读性。

# 行 2778：普通语句：执行一步操作或表达式。
            self.b_extend_current.pack(pady=0)
# 行 2779：空行，用于分隔代码块，提高可读性。

# 行 2780：条件判断，根据不同条件执行不同分支。
            if self.b_source_current_range:
# 行 2781：普通语句：执行一步操作或表达式。
                self.b_expandable_frame1.pack(fill=tk.X, padx=20, pady=10)
# 行 2782：空行，用于分隔代码块，提高可读性。

# 行 2783：普通语句：执行一步操作或表达式。
            self.b_extend_sampling.pack(pady=0)
# 行 2784：空行，用于分隔代码块，提高可读性。

# 行 2785：条件判断，根据不同条件执行不同分支。
            if self.b_source_sampling_range:
# 行 2786：普通语句：执行一步操作或表达式。
                self.b_expandable_frame2.pack(fill=tk.X, padx=20, pady=10)
# 行 2787：空行，用于分隔代码块，提高可读性。

# 行 2788：普通语句：执行一步操作或表达式。
            self.b_extend_bandwidth.pack(pady=0)
# 行 2789：空行，用于分隔代码块，提高可读性。

# 行 2790：条件判断，根据不同条件执行不同分支。
            if self.b_source_bandwidth_range:
# 行 2791：普通语句：执行一步操作或表达式。
                self.b_expandable_frame3.pack(fill=tk.X, padx=20, pady=10)
# 行 2792：空行，用于分隔代码块，提高可读性。

# 行 2793：原始注释行，提供作者或用途说明。
            # self.b_extend_volt.pack(pady=0)
# 行 2794：原始注释行，提供作者或用途说明。
            #
# 行 2795：原始注释行，提供作者或用途说明。
            # if self.b_source_volt_range:
# 行 2796：原始注释行，提供作者或用途说明。
            #     self.b_expandable_frame4.pack(fill=tk.X, padx=20, pady=10)
# 行 2797：空行，用于分隔代码块，提高可读性。

# 行 2798：定义函数/方法 return_current_range(value)。
        def return_current_range(value):
# 行 2799：变量赋值：保存配置、状态或计算结果。
            current_range_list = {'200 pA': 0, '2 nA': 1, '20 nA': 2, '200 nA': 3}
# 行 2800：返回函数结果。
            return current_range_list.get(value, 0)
# 行 2801：定义函数/方法 current_range(value)。
        def current_range(value):
# 行 2802：空行，用于分隔代码块，提高可读性。

# 行 2803：条件判断，根据不同条件执行不同分支。
            if self.b_last_current_range_selected != value:
# 行 2804：打印调试或状态信息到控制台。
                print(f"选中了: {value}")
# 行 2805：变量赋值：保存配置、状态或计算结果。
                self.b_last_current_range_selected = str(value)
# 行 2806：空行，用于分隔代码块，提高可读性。

# 行 2807：打印调试或状态信息到控制台。
                print(value)
# 行 2808：普通语句：执行一步操作或表达式。
                c4.setCurrentRange(return_current_range(value), 1)
# 行 2809：变量赋值：保存配置、状态或计算结果。
        self.b_extend_current = tk.Button(self.collection_setting_page_original, text="▶电流量程", command=toggle_expand1, width=42, height=1,
# 行 2810：变量赋值：保存配置、状态或计算结果。
                                   anchor='w',bg="#e5e5e5")
# 行 2811：普通语句：执行一步操作或表达式。
        self.b_extend_current.pack(padx=0, pady=0, anchor='w')
# 行 2812：空行，用于分隔代码块，提高可读性。

# 行 2813：变量赋值：保存配置、状态或计算结果。
        self.b_expandable_frame1 = tk.Frame(self.collection_setting_page_original, borderwidth=0, relief="solid", bg="#e5e5e5")
# 行 2814：变量赋值：保存配置、状态或计算结果。
        self.b_radio1_current = tk.Radiobutton(self.b_expandable_frame1, text="200 pA", variable=current_selection_var,
# 行 2815：变量赋值：保存配置、状态或计算结果。
                                        value="200 pA",
# 行 2816：变量赋值：保存配置、状态或计算结果。
                                        command=lambda: current_range("200 pA"), bg="#e5e5e5")
# 行 2817：普通语句：执行一步操作或表达式。
        self.b_radio1_current.pack(padx=0, pady=0, anchor='w')
# 行 2818：空行，用于分隔代码块，提高可读性。

# 行 2819：变量赋值：保存配置、状态或计算结果。
        self.b_radio2_current = tk.Radiobutton(self.b_expandable_frame1, text="2 nA", variable=current_selection_var, value="2 nA",
# 行 2820：变量赋值：保存配置、状态或计算结果。
                                        command=lambda: current_range("2 nA"), bg="#e5e5e5")
# 行 2821：普通语句：执行一步操作或表达式。
        self.b_radio2_current.pack(padx=0, pady=0, anchor='w')
# 行 2822：空行，用于分隔代码块，提高可读性。

# 行 2823：变量赋值：保存配置、状态或计算结果。
        self.b_radio3_current = tk.Radiobutton(self.b_expandable_frame1, text="20 nA", variable=current_selection_var, value="20 nA",
# 行 2824：变量赋值：保存配置、状态或计算结果。
                                        command=lambda: current_range("20 nA"), bg="#e5e5e5")
# 行 2825：普通语句：执行一步操作或表达式。
        self.b_radio3_current.pack(padx=0, pady=0, anchor='w')
# 行 2826：空行，用于分隔代码块，提高可读性。

# 行 2827：变量赋值：保存配置、状态或计算结果。
        self.b_radio4_current = tk.Radiobutton(self.b_expandable_frame1, text="200 nA", variable=current_selection_var,
# 行 2828：变量赋值：保存配置、状态或计算结果。
                                        value="200 nA",
# 行 2829：变量赋值：保存配置、状态或计算结果。
                                        command=lambda: current_range("200 nA"), bg="#e5e5e5")
# 行 2830：普通语句：执行一步操作或表达式。
        self.b_radio4_current.pack(padx=0, pady=0, anchor='w')
# 行 2831：空行，用于分隔代码块，提高可读性。

# 行 2832：定义函数/方法 return_sampling_rate(value)。
        def return_sampling_rate(value):
# 行 2833：变量赋值：保存配置、状态或计算结果。
            sampling_rate_list = {1250: 0, 5000: 1, 10000: 2, 20000: 3,
# 行 2834：普通语句：执行一步操作或表达式。
                                  50000: 4, 100000: 5, 200000: 6}
# 行 2835：返回函数结果。
            return sampling_rate_list.get(value, 0)
# 行 2836：空行，用于分隔代码块，提高可读性。

# 行 2837：定义函数/方法 sampling_rate(value)。
        def sampling_rate(value):
# 行 2838：条件判断，根据不同条件执行不同分支。
            if self.b_last_sampling_rate_selected != value:
# 行 2839：打印调试或状态信息到控制台。
                print(f"选中了: {value}")
# 行 2840：变量赋值：保存配置、状态或计算结果。
                self.b_last_sampling_rate_selected = value
# 行 2841：普通语句：执行一步操作或表达式。
                c4.setSamplingRate(return_sampling_rate(value))
# 行 2842：变量赋值：保存配置、状态或计算结果。
                self.b_frequency = value
# 行 2843：空行，用于分隔代码块，提高可读性。

# 行 2844：空行，用于分隔代码块，提高可读性。

# 行 2845：变量赋值：保存配置、状态或计算结果。
        self.b_extend_sampling = tk.Button(self.collection_setting_page_original, text="▶采样率", command=toggle_expand2, width=42, height=1,
# 行 2846：变量赋值：保存配置、状态或计算结果。
                                    anchor='w', bg="#e5e5e5")
# 行 2847：普通语句：执行一步操作或表达式。
        self.b_extend_sampling.pack(padx=0, pady=0, anchor='w')
# 行 2848：空行，用于分隔代码块，提高可读性。

# 行 2849：变量赋值：保存配置、状态或计算结果。
        self.b_expandable_frame2 = tk.Frame(self.collection_setting_page_original, borderwidth=0, relief="solid", bg="#e5e5e5")
# 行 2850：变量赋值：保存配置、状态或计算结果。
        self.b_radio1_sampling = tk.Radiobutton(self.b_expandable_frame2, text="1.25 KHz", variable=sampling_selection_var,
# 行 2851：变量赋值：保存配置、状态或计算结果。
                                         value="1.25 KHz",
# 行 2852：变量赋值：保存配置、状态或计算结果。
                                         command=lambda: sampling_rate(1250), bg="#e5e5e5")
# 行 2853：普通语句：执行一步操作或表达式。
        self.b_radio1_sampling.pack(padx=0, pady=0, anchor='w')
# 行 2854：空行，用于分隔代码块，提高可读性。

# 行 2855：变量赋值：保存配置、状态或计算结果。
        self.b_radio2_sampling = tk.Radiobutton(self.b_expandable_frame2, text="5 KHz", variable=sampling_selection_var,
# 行 2856：变量赋值：保存配置、状态或计算结果。
                                         value="5 KHz",
# 行 2857：变量赋值：保存配置、状态或计算结果。
                                         command=lambda: sampling_rate(5000), bg="#e5e5e5")
# 行 2858：普通语句：执行一步操作或表达式。
        self.b_radio2_sampling.pack(padx=0, pady=0, anchor='w')
# 行 2859：空行，用于分隔代码块，提高可读性。

# 行 2860：变量赋值：保存配置、状态或计算结果。
        self.b_radio3_sampling = tk.Radiobutton(self.b_expandable_frame2, text="10 KHz", variable=sampling_selection_var,
# 行 2861：变量赋值：保存配置、状态或计算结果。
                                         value="10 KHz",
# 行 2862：变量赋值：保存配置、状态或计算结果。
                                         command=lambda: sampling_rate(10000), bg="#e5e5e5")
# 行 2863：普通语句：执行一步操作或表达式。
        self.b_radio3_sampling.pack(padx=0, pady=0, anchor='w')
# 行 2864：空行，用于分隔代码块，提高可读性。

# 行 2865：变量赋值：保存配置、状态或计算结果。
        self.b_radio4_sampling = tk.Radiobutton(self.b_expandable_frame2, text="20 KHz", variable=sampling_selection_var,
# 行 2866：变量赋值：保存配置、状态或计算结果。
                                         value="20 KHz",
# 行 2867：变量赋值：保存配置、状态或计算结果。
                                         command=lambda: sampling_rate(20000), bg="#e5e5e5")
# 行 2868：普通语句：执行一步操作或表达式。
        self.b_radio4_sampling.pack(padx=0, pady=0, anchor='w')
# 行 2869：空行，用于分隔代码块，提高可读性。

# 行 2870：变量赋值：保存配置、状态或计算结果。
        self.b_radio5_sampling = tk.Radiobutton(self.b_expandable_frame2, text="50 KHz", variable=sampling_selection_var,
# 行 2871：变量赋值：保存配置、状态或计算结果。
                                         value="50 KHz",
# 行 2872：变量赋值：保存配置、状态或计算结果。
                                         command=lambda: sampling_rate(50000), bg="#e5e5e5")
# 行 2873：普通语句：执行一步操作或表达式。
        self.b_radio5_sampling.pack(padx=0, pady=0, anchor='w')
# 行 2874：空行，用于分隔代码块，提高可读性。

# 行 2875：变量赋值：保存配置、状态或计算结果。
        self.b_radio6_sampling = tk.Radiobutton(self.b_expandable_frame2, text="100 KHz", variable=sampling_selection_var,
# 行 2876：变量赋值：保存配置、状态或计算结果。
                                         value="100 KHz",
# 行 2877：变量赋值：保存配置、状态或计算结果。
                                         command=lambda: sampling_rate(100000), bg="#e5e5e5")
# 行 2878：普通语句：执行一步操作或表达式。
        self.b_radio6_sampling.pack(padx=0, pady=0, anchor='w')
# 行 2879：空行，用于分隔代码块，提高可读性。

# 行 2880：变量赋值：保存配置、状态或计算结果。
        self.b_radio7_sampling = tk.Radiobutton(self.b_expandable_frame2, text="200 KHz", variable=sampling_selection_var,
# 行 2881：变量赋值：保存配置、状态或计算结果。
                                         value="200 KHz",
# 行 2882：变量赋值：保存配置、状态或计算结果。
                                         command=lambda: sampling_rate(200000), bg="#e5e5e5")
# 行 2883：普通语句：执行一步操作或表达式。
        self.b_radio7_sampling.pack(padx=0, pady=0, anchor='w')
# 行 2884：空行，用于分隔代码块，提高可读性。

# 行 2885：定义函数/方法 filter_select(value)。
        def filter_select(value):
# 行 2886：条件判断，根据不同条件执行不同分支。
            if self.b_last_filter_selected != value:
# 行 2887：打印调试或状态信息到控制台。
                print(f"选中了: {value}")
# 行 2888：变量赋值：保存配置、状态或计算结果。
                self.b_last_filter_selected = value
# 行 2889：条件判断，根据不同条件执行不同分支。
                if value == 0:
# 行 2890：变量赋值：保存配置、状态或计算结果。
                    self.b_filter = False
# 行 2891：条件判断，根据不同条件执行不同分支。
                else:
# 行 2892：变量赋值：保存配置、状态或计算结果。
                    self.b_filter = True
# 行 2893：变量赋值：保存配置、状态或计算结果。
                    self.b_filter_parameter = value
# 行 2894：变量赋值：保存配置、状态或计算结果。
        self.b_extend_bandwidth = tk.Button(self.collection_setting_page_original, text="▶低通滤波", command=toggle_expand3, width=42, height=1, anchor='w', bg="#e5e5e5")
# 行 2895：普通语句：执行一步操作或表达式。
        self.b_extend_bandwidth.pack(padx=0, pady=0, anchor='w')
# 行 2896：空行，用于分隔代码块，提高可读性。

# 行 2897：变量赋值：保存配置、状态或计算结果。
        self.b_expandable_frame3 = tk.Frame(self.collection_setting_page_original, borderwidth=0, relief="solid", bg="#e5e5e5")
# 行 2898：空行，用于分隔代码块，提高可读性。

# 行 2899：空行，用于分隔代码块，提高可读性。

# 行 2900：变量赋值：保存配置、状态或计算结果。
        self.b_radio1_bandwidth = tk.Radiobutton(self.b_expandable_frame3, text="无滤波", variable=bandwidth_selection_var,
# 行 2901：变量赋值：保存配置、状态或计算结果。
                                          value="无滤波",
# 行 2902：变量赋值：保存配置、状态或计算结果。
                                          command=lambda: filter_select(0), bg="#e5e5e5")
# 行 2903：普通语句：执行一步操作或表达式。
        self.b_radio1_bandwidth.pack(padx=0, pady=0, anchor='w')
# 行 2904：空行，用于分隔代码块，提高可读性。

# 行 2905：变量赋值：保存配置、状态或计算结果。
        self.b_radio2_bandwidth = tk.Radiobutton(self.b_expandable_frame3, text="10 KHz", variable=bandwidth_selection_var,
# 行 2906：变量赋值：保存配置、状态或计算结果。
                                          value="10KHz",
# 行 2907：变量赋值：保存配置、状态或计算结果。
                                          command=lambda: filter_select(10000), bg="#e5e5e5")
# 行 2908：普通语句：执行一步操作或表达式。
        self.b_radio2_bandwidth.pack(padx=0, pady=0, anchor='w')
# 行 2909：空行，用于分隔代码块，提高可读性。

# 行 2910：变量赋值：保存配置、状态或计算结果。
        self.b_radio3_bandwidth = tk.Radiobutton(self.b_expandable_frame3, text="5 KHz", variable=bandwidth_selection_var,
# 行 2911：变量赋值：保存配置、状态或计算结果。
                                          value="5KHz",
# 行 2912：变量赋值：保存配置、状态或计算结果。
                                          command=lambda: filter_select(5000), bg="#e5e5e5")
# 行 2913：普通语句：执行一步操作或表达式。
        self.b_radio3_bandwidth.pack(padx=0, pady=0, anchor='w')
# 行 2914：空行，用于分隔代码块，提高可读性。

# 行 2915：变量赋值：保存配置、状态或计算结果。
        self.b_radio4_bandwidth = tk.Radiobutton(self.b_expandable_frame3, text="2 KHz", variable=bandwidth_selection_var,
# 行 2916：变量赋值：保存配置、状态或计算结果。
                                          value="2KHz",
# 行 2917：变量赋值：保存配置、状态或计算结果。
                                          command=lambda: filter_select(2000), bg="#e5e5e5")
# 行 2918：普通语句：执行一步操作或表达式。
        self.b_radio4_bandwidth.pack(padx=0, pady=0, anchor='w')
# 行 2919：空行，用于分隔代码块，提高可读性。

# 行 2920：变量赋值：保存配置、状态或计算结果。
        self.b_radio5_bandwidth = tk.Radiobutton(self.b_expandable_frame3, text="1 KHz",
# 行 2921：变量赋值：保存配置、状态或计算结果。
                                                 variable=bandwidth_selection_var,
# 行 2922：变量赋值：保存配置、状态或计算结果。
                                                 value="1KHz",
# 行 2923：变量赋值：保存配置、状态或计算结果。
                                                 command=lambda: filter_select(1000), bg="#e5e5e5")
# 行 2924：普通语句：执行一步操作或表达式。
        self.b_radio5_bandwidth.pack(padx=0, pady=0, anchor='w')
# 行 2925：空行，用于分隔代码块，提高可读性。

# 行 2926：变量赋值：保存配置、状态或计算结果。
        self.b_radio5_bandwidth = tk.Radiobutton(self.b_expandable_frame3, text="500 Hz",
# 行 2927：变量赋值：保存配置、状态或计算结果。
                                                 variable=bandwidth_selection_var,
# 行 2928：变量赋值：保存配置、状态或计算结果。
                                                 value="500Hz",
# 行 2929：变量赋值：保存配置、状态或计算结果。
                                                 command=lambda: filter_select(500), bg="#e5e5e5")
# 行 2930：普通语句：执行一步操作或表达式。
        self.b_radio5_bandwidth.pack(padx=0, pady=0, anchor='w')
# 行 2931：空行，用于分隔代码块，提高可读性。

# 行 2932：原始注释行，提供作者或用途说明。
        # self.b_extend_volt = tk.Button(self.collection_setting_page_original, text="▶电压", command=toggle_expand4, width=42, height=1, anchor='w')
# 行 2933：原始注释行，提供作者或用途说明。
        # self.b_extend_volt.pack(padx=0, pady=0, anchor='w')
# 行 2934：原始注释行，提供作者或用途说明。
        #
# 行 2935：原始注释行，提供作者或用途说明。
        # self.b_expandable_frame4 = tk.Frame(self.collection_setting_page_original, borderwidth=0, relief="solid")
# 行 2936：原始注释行，提供作者或用途说明。
        # self.b_radio1_volt = tk.Radiobutton(self.b_expandable_frame4, text="恒压波", variable=volt_selection_var,
# 行 2937：原始注释行，提供作者或用途说明。
        #                              value="恒压波", command=lambda: filter_select("恒压波"))
# 行 2938：原始注释行，提供作者或用途说明。
        # self.b_radio1_volt.pack(padx=0, pady=0, anchor='w')
# 行 2939：原始注释行，提供作者或用途说明。
        #
# 行 2940：原始注释行，提供作者或用途说明。
        # self.b_radio2_volt = tk.Radiobutton(self.b_expandable_frame4, text="三角波", variable=volt_selection_var,
# 行 2941：原始注释行，提供作者或用途说明。
        #                              value="三角波", command=lambda: filter_select("三角波"))
# 行 2942：原始注释行，提供作者或用途说明。
        # self.b_radio2_volt.pack(padx=0, pady=0, anchor='w')
# 行 2943：原始注释行，提供作者或用途说明。
        #
# 行 2944：原始注释行，提供作者或用途说明。
        # self.b_radio3_volt = tk.Radiobutton(self.b_expandable_frame4, text="方波", variable=volt_selection_var,
# 行 2945：原始注释行，提供作者或用途说明。
        #                              value="方波", command=lambda: filter_select("方波"))
# 行 2946：原始注释行，提供作者或用途说明。
        # self.b_radio3_volt.pack(padx=0, pady=0, anchor='w')
# 行 2947：原始注释行，提供作者或用途说明。
        #
# 行 2948：原始注释行，提供作者或用途说明。
        # self.b_radio4_volt = tk.Radiobutton(self.b_expandable_frame4, text="可变幅方波", variable=volt_selection_var,
# 行 2949：原始注释行，提供作者或用途说明。
        #                              value="可变幅方波", command=lambda: filter_select("可变幅方波"))
# 行 2950：原始注释行，提供作者或用途说明。
        # self.b_radio4_volt.pack(padx=0, pady=0, anchor='w')
# 行 2951：原始注释行，提供作者或用途说明。
        #
# 行 2952：原始注释行，提供作者或用途说明。
        # self.b_radio5_volt = tk.Radiobutton(self.b_expandable_frame4, text="可变幅单向方波", variable=volt_selection_var,
# 行 2953：原始注释行，提供作者或用途说明。
        #                              value="可变幅方波", command=lambda: filter_select("可变幅方波"))
# 行 2954：原始注释行，提供作者或用途说明。
        # self.b_radio5_volt.pack(padx=0, pady=0, anchor='w')
# 行 2955：原始注释行，提供作者或用途说明。
        #
# 行 2956：原始注释行，提供作者或用途说明。
        # self.b_radio6_volt = tk.Radiobutton(self.b_expandable_frame4, text="可变持续时间单向方波", variable=volt_selection_var,
# 行 2957：原始注释行，提供作者或用途说明。
        #                              value="可变持续时间单向方波",
# 行 2958：原始注释行，提供作者或用途说明。
        #                              command=lambda: filter_select("可变持续时间单向方波"))
# 行 2959：原始注释行，提供作者或用途说明。
        # self.b_radio6_volt.pack(padx=0, pady=0, anchor='w')
# 行 2960：原始注释行，提供作者或用途说明。
        #
# 行 2961：原始注释行，提供作者或用途说明。
        # self.b_radio7_volt = tk.Radiobutton(self.b_expandable_frame4, text="梯形波", variable=volt_selection_var,
# 行 2962：原始注释行，提供作者或用途说明。
        #                              value="梯形波", command=lambda: filter_select("梯形波"))
# 行 2963：原始注释行，提供作者或用途说明。
        # self.b_radio7_volt.pack(padx=0, pady=0, anchor='w')
# 行 2964：原始注释行，提供作者或用途说明。
        #
# 行 2965：原始注释行，提供作者或用途说明。
        # self.b_radio8_volt = tk.Radiobutton(self.b_expandable_frame4, text="可调周期三角波", variable=volt_selection_var,
# 行 2966：原始注释行，提供作者或用途说明。
        #                              value="可调周期三角波", command=lambda: filter_select("可调周期三角波"))
# 行 2967：原始注释行，提供作者或用途说明。
        # self.b_radio8_volt.pack(padx=0, pady=0, anchor='w')
# 行 2968：普通语句：执行一步操作或表达式。
    '''数据分析'''
# 行 2969：定义函数/方法 c_plot_current(self)。
    def c_plot_current(self):
# 行 2970：变量赋值：保存配置、状态或计算结果。
        fig = Figure(figsize=(9.4, 2.5), dpi=100)
# 行 2971：普通语句：执行一步操作或表达式。
        fig.patch.set_facecolor('#e5e5e5')
# 行 2972：变量赋值：保存配置、状态或计算结果。
        self.ax_c_current = fig.add_subplot(111)
# 行 2973：普通语句：执行一步操作或表达式。
        self.ax_c_current.set_facecolor('#e5e5e5')
# 行 2974：普通语句：执行一步操作或表达式。
        self.ax_c_current.set_xlabel("时间(s)")
# 行 2975：普通语句：执行一步操作或表达式。
        self.ax_c_current.set_ylabel("电流(nA)")
# 行 2976：普通语句：执行一步操作或表达式。
        self.ax_c_current.grid(True)
# 行 2977：普通语句：执行一步操作或表达式。
        fig.tight_layout()
# 行 2978：普通语句：执行一步操作或表达式。
        fig.subplots_adjust(left=0.06)
# 行 2979：变量赋值：保存配置、状态或计算结果。
        self.canvas_current_c = FigureCanvasTkAgg(fig, master=self.analyse_page_original)
# 行 2980：变量赋值：保存配置、状态或计算结果。
        self.canvas_current_c1 = self.canvas_current_c.get_tk_widget()
# 行 2981：普通语句：执行一步操作或表达式。
        self.canvas_current_c1.place(x=5, y=180)
# 行 2982：空行，用于分隔代码块，提高可读性。

# 行 2983：定义函数/方法 update_c_plot_current(self, x, y)。
    def update_c_plot_current(self, x, y):
# 行 2984：普通语句：执行一步操作或表达式。
        self.ax_c_current.cla()
# 行 2985：普通语句：执行一步操作或表达式。
        self.ax_c_current.grid(True)
# 行 2986：普通语句：执行一步操作或表达式。
        self.ax_c_current.set_xlabel("时间(s)")
# 行 2987：普通语句：执行一步操作或表达式。
        self.ax_c_current.set_ylabel("电流(nA)")
# 行 2988：条件判断，根据不同条件执行不同分支。
        if self.c_filter == True:
# 行 2989：原始注释行，提供作者或用途说明。
            # 绘制主数据线条
# 行 2990：普通语句：执行一步操作或表达式。
            self.ax_c_current.plot(x, self.c1_y_filter)
# 行 2991：条件判断，根据不同条件执行不同分支。
        else:
# 行 2992：普通语句：执行一步操作或表达式。
            self.ax_c_current.plot(x, y)
# 行 2993：原始注释行，提供作者或用途说明。
        # 处理额外线条（对应原来的b_line3,4,5）
# 行 2994：条件判断，根据不同条件执行不同分支。
        if self.c_detection_threshold == False:
# 行 2995：占位语句：暂不执行任何操作。
            pass
# 行 2996：条件判断，根据不同条件执行不同分支。
        else:
# 行 2997：普通语句：执行一步操作或表达式。
            self.ax_c_current.plot(x, self.c_baseline, color='black')
# 行 2998：普通语句：执行一步操作或表达式。
            self.ax_c_current.plot(x, self.c_std_down, color='green')
# 行 2999：普通语句：执行一步操作或表达式。
            self.ax_c_current.plot(x, self.c_std_up, color='green')
# 行 3000：条件判断，根据不同条件执行不同分支。
        if self.c_event == True:
# 行 3001：for 循环遍历序列或可迭代对象。
            for i in range(len(self.c_event_list)):
# 行 3002：普通语句：执行一步操作或表达式。
                self.ax_c_current.plot(self.c_event_list[i][-2], self.c_event_list[i][-1], color='red')
# 行 3003：空行，用于分隔代码块，提高可读性。

# 行 3004：for 循环遍历序列或可迭代对象。
            for i in range(len(self.c_event_list_two)):
# 行 3005：普通语句：执行一步操作或表达式。
                self.ax_c_current.plot(self.c_event_list_two[i][-2], self.c_event_list_two[i][-1], color='black')
# 行 3006：条件判断，根据不同条件执行不同分支。
            else:
# 行 3007：占位语句：暂不执行任何操作。
                pass
# 行 3008：普通语句：执行一步操作或表达式。
        self.canvas_current_c.draw()
# 行 3009：空行，用于分隔代码块，提高可读性。

# 行 3010：定义函数/方法 c_plot_event1(self)。
    def c_plot_event1(self):
# 行 3011：变量赋值：保存配置、状态或计算结果。
        fig = Figure(figsize=(3.1, 2.8), dpi=100)
# 行 3012：普通语句：执行一步操作或表达式。
        fig.patch.set_facecolor('#e5e5e5')
# 行 3013：变量赋值：保存配置、状态或计算结果。
        self.ax_c_event = fig.add_subplot(111)
# 行 3014：普通语句：执行一步操作或表达式。
        self.ax_c_event.set_facecolor('#e5e5e5')
# 行 3015：空行，用于分隔代码块，提高可读性。

# 行 3016：普通语句：执行一步操作或表达式。
        self.ax_c_event.set_xlabel("时间(s)")
# 行 3017：普通语句：执行一步操作或表达式。
        self.ax_c_event.set_ylabel("电流(nA)")
# 行 3018：普通语句：执行一步操作或表达式。
        self.ax_c_event.grid(True)
# 行 3019：普通语句：执行一步操作或表达式。
        fig.tight_layout()
# 行 3020：普通语句：执行一步操作或表达式。
        fig.subplots_adjust(left=0.2, bottom=0.15)
# 行 3021：变量赋值：保存配置、状态或计算结果。
        self.canvas_event_c_event = FigureCanvasTkAgg(fig, master=self.analyse_page_original)
# 行 3022：变量赋值：保存配置、状态或计算结果。
        self.canvas_widget2_c_event = self.canvas_event_c_event.get_tk_widget()
# 行 3023：普通语句：执行一步操作或表达式。
        self.canvas_widget2_c_event.place(x=5, y=696)
# 行 3024：定义函数/方法 c_update_plot_event1(self, x, y)。
    def c_update_plot_event1(self, x, y):
# 行 3025：普通语句：执行一步操作或表达式。
        self.ax_c_event.cla()
# 行 3026：普通语句：执行一步操作或表达式。
        self.ax_c_event.grid(True)
# 行 3027：普通语句：执行一步操作或表达式。
        self.ax_c_event.set_xlabel("时间(s)")
# 行 3028：普通语句：执行一步操作或表达式。
        self.ax_c_event.set_ylabel("电流(nA)")
# 行 3029：普通语句：执行一步操作或表达式。
        self.ax_c_event.plot(x, y)
# 行 3030：普通语句：执行一步操作或表达式。
        self.canvas_event_c_event.draw()
# 行 3031：定义函数/方法 c_plot_2d(self)。
    def c_plot_2d(self):
# 行 3032：变量赋值：保存配置、状态或计算结果。
        fig = Figure(figsize=(3.1, 2.8), dpi=100)
# 行 3033：普通语句：执行一步操作或表达式。
        fig.patch.set_facecolor('#e5e5e5')
# 行 3034：变量赋值：保存配置、状态或计算结果。
        self.ax_c_2d = fig.add_subplot(111)
# 行 3035：普通语句：执行一步操作或表达式。
        self.ax_c_2d.set_facecolor('#e5e5e5')
# 行 3036：普通语句：执行一步操作或表达式。
        self.ax_c_2d.set_xlabel("时间(ms)")
# 行 3037：普通语句：执行一步操作或表达式。
        self.ax_c_2d.set_ylabel("振幅(nA)")
# 行 3038：普通语句：执行一步操作或表达式。
        self.ax_c_2d.grid(True)
# 行 3039：普通语句：执行一步操作或表达式。
        fig.tight_layout()
# 行 3040：普通语句：执行一步操作或表达式。
        fig.subplots_adjust(left=0.2, bottom=0.15)
# 行 3041：变量赋值：保存配置、状态或计算结果。
        self.canvas_event_c_2d = FigureCanvasTkAgg(fig, master=self.analyse_page_original)
# 行 3042：变量赋值：保存配置、状态或计算结果。
        self.canvas_widget2_c_2d = self.canvas_event_c_2d.get_tk_widget()
# 行 3043：普通语句：执行一步操作或表达式。
        self.canvas_widget2_c_2d.place(x=320, y=696)
# 行 3044：定义函数/方法 c_update_plot_2D(self, x, y)。
    def c_update_plot_2D(self, x, y):
# 行 3045：普通语句：执行一步操作或表达式。
        self.ax_c_2d.cla()
# 行 3046：原始注释行，提供作者或用途说明。
        # self.ax3.grid(True)
# 行 3047：普通语句：执行一步操作或表达式。
        self.ax_c_2d.set_xlabel("时间(ms)")
# 行 3048：普通语句：执行一步操作或表达式。
        self.ax_c_2d.set_ylabel("振幅(nA)")
# 行 3049：原始注释行，提供作者或用途说明。
        # self.ax3.set_xticks([])
# 行 3050：原始注释行，提供作者或用途说明。
        # self.ax3.set_yticks([])
# 行 3051：普通语句：执行一步操作或表达式。
        self.ax_c_2d.scatter(x, y, edgecolor='red', facecolors='None', s=100)
# 行 3052：普通语句：执行一步操作或表达式。
        self.canvas_event_c_2d.draw()
# 行 3053：定义函数/方法 c_plot_histogram(self)。
    def c_plot_histogram(self):
# 行 3054：变量赋值：保存配置、状态或计算结果。
        fig = Figure(figsize=(3.1, 2.8), dpi=100)
# 行 3055：普通语句：执行一步操作或表达式。
        fig.patch.set_facecolor('#e5e5e5')
# 行 3056：变量赋值：保存配置、状态或计算结果。
        self.ax_c_histogram = fig.add_subplot(111)
# 行 3057：普通语句：执行一步操作或表达式。
        self.ax_c_histogram.set_facecolor('#e5e5e5')
# 行 3058：普通语句：执行一步操作或表达式。
        self.ax_c_histogram.set_xlabel("时间(s)")
# 行 3059：普通语句：执行一步操作或表达式。
        self.ax_c_histogram.set_ylabel("电流(nA)")
# 行 3060：普通语句：执行一步操作或表达式。
        self.ax_c_histogram.grid(True)
# 行 3061：普通语句：执行一步操作或表达式。
        fig.tight_layout()
# 行 3062：普通语句：执行一步操作或表达式。
        fig.subplots_adjust(left=0.2, bottom=0.15)
# 行 3063：变量赋值：保存配置、状态或计算结果。
        self.canvas_event_c_histogram = FigureCanvasTkAgg(fig, master=self.analyse_page_original)
# 行 3064：变量赋值：保存配置、状态或计算结果。
        self.canvas_widget2_c_histogram = self.canvas_event_c_histogram.get_tk_widget()
# 行 3065：普通语句：执行一步操作或表达式。
        self.canvas_widget2_c_histogram.place(x=635, y=696)
# 行 3066：定义函数/方法 c_undate_plot_histogram(self, data)。
    def c_undate_plot_histogram(self, data):
# 行 3067：普通语句：执行一步操作或表达式。
        self.ax_c_histogram.cla()
# 行 3068：普通语句：执行一步操作或表达式。
        self.ax_c_histogram.set_xlabel("时间(ms)")
# 行 3069：普通语句：执行一步操作或表达式。
        self.ax_c_histogram.set_ylabel("count")
# 行 3070：普通语句：执行一步操作或表达式。
        self.ax_c_histogram.hist(data,
# 行 3071：变量赋值：保存配置、状态或计算结果。
                bins=50,
# 行 3072：变量赋值：保存配置、状态或计算结果。
                density=False,
# 行 3073：变量赋值：保存配置、状态或计算结果。
                alpha=0.6,)
# 行 3074：普通语句：执行一步操作或表达式。
        self.canvas_event_c_histogram.draw()
# 行 3075：定义函数/方法 c_plot_2d_2(self)。
    def c_plot_2d_2(self):
# 行 3076：变量赋值：保存配置、状态或计算结果。
        fig = Figure(figsize=(3.0, 2.4), dpi=100)
# 行 3077：普通语句：执行一步操作或表达式。
        fig.patch.set_facecolor('#e5e5e5')
# 行 3078：变量赋值：保存配置、状态或计算结果。
        self.ax_c_2d_2 = fig.add_subplot(111)
# 行 3079：普通语句：执行一步操作或表达式。
        self.ax_c_2d_2.set_facecolor('#e5e5e5')
# 行 3080：普通语句：执行一步操作或表达式。
        self.ax_c_2d_2.set_xlabel("时间(ms)")
# 行 3081：普通语句：执行一步操作或表达式。
        self.ax_c_2d_2.set_ylabel("振幅(nA)")
# 行 3082：普通语句：执行一步操作或表达式。
        self.ax_c_2d_2.grid(True)
# 行 3083：普通语句：执行一步操作或表达式。
        fig.tight_layout()
# 行 3084：普通语句：执行一步操作或表达式。
        fig.subplots_adjust(left=0.2, bottom=0.2)
# 行 3085：变量赋值：保存配置、状态或计算结果。
        self.canvas_event_c_2d_2 = FigureCanvasTkAgg(fig, master=self.analyse_page_original)
# 行 3086：变量赋值：保存配置、状态或计算结果。
        self.canvas_widget2_c_2d_2 = self.canvas_event_c_2d_2.get_tk_widget()
# 行 3087：普通语句：执行一步操作或表达式。
        self.canvas_widget2_c_2d_2.place(x=1430, y=10)
# 行 3088：定义函数/方法 c_update_plot_2D_2(self, x, y)。
    def c_update_plot_2D_2(self, x, y):
# 行 3089：普通语句：执行一步操作或表达式。
        self.ax_c_2d_2.cla()
# 行 3090：原始注释行，提供作者或用途说明。
        # self.ax3.grid(True)
# 行 3091：普通语句：执行一步操作或表达式。
        self.ax_c_2d_2.set_xlabel("时间(ms)")
# 行 3092：普通语句：执行一步操作或表达式。
        self.ax_c_2d_2.set_ylabel("振幅(nA)")
# 行 3093：原始注释行，提供作者或用途说明。
        # self.ax3.set_xticks([])
# 行 3094：原始注释行，提供作者或用途说明。
        # self.ax3.set_yticks([])
# 行 3095：普通语句：执行一步操作或表达式。
        self.ax_c_2d_2.scatter(x, y, edgecolor='red', facecolors='None', s=100)
# 行 3096：普通语句：执行一步操作或表达式。
        self.canvas_event_c_2d_2.draw()
# 行 3097：定义函数/方法 c_plot_histogram_2_2(self)。
    def c_plot_histogram_2_2(self):
# 行 3098：变量赋值：保存配置、状态或计算结果。
        fig = Figure(figsize=(3.0, 2.4), dpi=100)
# 行 3099：普通语句：执行一步操作或表达式。
        fig.patch.set_facecolor('#e5e5e5')
# 行 3100：变量赋值：保存配置、状态或计算结果。
        self.ax_c_histogram_2 = fig.add_subplot(111)
# 行 3101：普通语句：执行一步操作或表达式。
        self.ax_c_histogram_2.set_facecolor('#e5e5e5')
# 行 3102：普通语句：执行一步操作或表达式。
        self.ax_c_histogram_2.set_xlabel("时间(s)")
# 行 3103：普通语句：执行一步操作或表达式。
        self.ax_c_histogram_2.set_ylabel("电流(nA)")
# 行 3104：普通语句：执行一步操作或表达式。
        self.ax_c_histogram_2.grid(True)
# 行 3105：普通语句：执行一步操作或表达式。
        fig.tight_layout()
# 行 3106：普通语句：执行一步操作或表达式。
        fig.subplots_adjust(left=0.2, bottom=0.2)
# 行 3107：变量赋值：保存配置、状态或计算结果。
        self.canvas_event_c_histogram_2 = FigureCanvasTkAgg(fig, master=self.analyse_page_original)
# 行 3108：变量赋值：保存配置、状态或计算结果。
        self.canvas_widget2_c_histogram_2 = self.canvas_event_c_histogram_2.get_tk_widget()
# 行 3109：普通语句：执行一步操作或表达式。
        self.canvas_widget2_c_histogram_2.place(x=1430, y=250)
# 行 3110：定义函数/方法 c_undate_plot_histogram_2(self, data)。
    def c_undate_plot_histogram_2(self, data):
# 行 3111：普通语句：执行一步操作或表达式。
        self.ax_c_histogram_2.cla()
# 行 3112：普通语句：执行一步操作或表达式。
        self.ax_c_histogram_2.set_xlabel("时间(ms)")
# 行 3113：普通语句：执行一步操作或表达式。
        self.ax_c_histogram_2.set_ylabel("count")
# 行 3114：普通语句：执行一步操作或表达式。
        self.ax_c_histogram_2.hist(data,
# 行 3115：变量赋值：保存配置、状态或计算结果。
                bins=50,
# 行 3116：变量赋值：保存配置、状态或计算结果。
                density=False,
# 行 3117：变量赋值：保存配置、状态或计算结果。
                alpha=0.6,)
# 行 3118：普通语句：执行一步操作或表达式。
        self.canvas_event_c_histogram_2.draw()
# 行 3119：定义函数/方法 c_plot_2d_3(self)。
    def c_plot_2d_3(self):
# 行 3120：变量赋值：保存配置、状态或计算结果。
        fig = Figure(figsize=(5.0, 5.0), dpi=100)
# 行 3121：普通语句：执行一步操作或表达式。
        fig.patch.set_facecolor('#e5e5e5')
# 行 3122：变量赋值：保存配置、状态或计算结果。
        self.ax_c_2d_3 = fig.add_subplot(111)
# 行 3123：普通语句：执行一步操作或表达式。
        self.ax_c_2d_3.set_facecolor('#e5e5e5')
# 行 3124：普通语句：执行一步操作或表达式。
        self.ax_c_2d_3.set_xlabel("时间(ms)")
# 行 3125：普通语句：执行一步操作或表达式。
        self.ax_c_2d_3.set_ylabel("振幅(nA)")
# 行 3126：普通语句：执行一步操作或表达式。
        self.ax_c_2d_3.grid(True)
# 行 3127：普通语句：执行一步操作或表达式。
        fig.tight_layout()
# 行 3128：普通语句：执行一步操作或表达式。
        fig.subplots_adjust(left=0.2, bottom=0.2)
# 行 3129：变量赋值：保存配置、状态或计算结果。
        self.canvas_event_c_2d_3 = FigureCanvasTkAgg(fig, master=self.analyse_page_original)
# 行 3130：变量赋值：保存配置、状态或计算结果。
        self.canvas_widget2_c_2d_3 = self.canvas_event_c_2d_3.get_tk_widget()
# 行 3131：普通语句：执行一步操作或表达式。
        self.canvas_widget2_c_2d_3.place(x=1400, y=450)
# 行 3132：定义函数/方法 c_update_plot_2D_3(self, x, y)。
    def c_update_plot_2D_3(self, x, y):
# 行 3133：普通语句：执行一步操作或表达式。
        self.ax_c_2d_3.cla()
# 行 3134：原始注释行，提供作者或用途说明。
        # self.ax3.grid(True)
# 行 3135：普通语句：执行一步操作或表达式。
        self.ax_c_2d_3.set_xlabel("时间(ms)")
# 行 3136：普通语句：执行一步操作或表达式。
        self.ax_c_2d_3.set_ylabel("振幅(nA)")
# 行 3137：原始注释行，提供作者或用途说明。
        # self.ax3.set_xticks([])
# 行 3138：原始注释行，提供作者或用途说明。
        # self.ax3.set_yticks([])
# 行 3139：普通语句：执行一步操作或表达式。
        self.ax_c_2d_3.scatter(x, y, edgecolor='red', facecolors='None', s=100)
# 行 3140：普通语句：执行一步操作或表达式。
        self.canvas_event_c_2d_3.draw()
# 行 3141：定义函数/方法 c_show_table_tk(self)。
    def c_show_table_tk(self):
# 行 3142：变量赋值：保存配置、状态或计算结果。
        self.c_table_frame = tk.Frame(
# 行 3143：普通语句：执行一步操作或表达式。
            self.analyse_page_original,
# 行 3144：变量赋值：保存配置、状态或计算结果。
            width=600,
# 行 3145：变量赋值：保存配置、状态或计算结果。
            height=600,
# 行 3146：变量赋值：保存配置、状态或计算结果。
            bg='#e5e5e5',
# 行 3147：变量赋值：保存配置、状态或计算结果。
            relief=tk.SUNKEN,
# 行 3148：变量赋值：保存配置、状态或计算结果。
            bd=2
# 行 3149：普通语句：执行一步操作或表达式。
        )
# 行 3150：普通语句：执行一步操作或表达式。
        self.c_table_frame.place(x=8, y=425)
# 行 3151：空行，用于分隔代码块，提高可读性。

# 行 3152：变量赋值：保存配置、状态或计算结果。
        style = ttk.Style()
# 行 3153：空行，用于分隔代码块，提高可读性。

# 行 3154：普通语句：执行一步操作或表达式。
        style.theme_use('default')
# 行 3155：空行，用于分隔代码块，提高可读性。

# 行 3156：普通语句：执行一步操作或表达式。
        style.configure(
# 行 3157：普通语句：执行一步操作或表达式。
            "Custom.Treeview",
# 行 3158：变量赋值：保存配置、状态或计算结果。
            background="#e5e5e5",
# 行 3159：变量赋值：保存配置、状态或计算结果。
            fieldbackground="#e5e5e5",
# 行 3160：普通语句：执行一步操作或表达式。
        )
# 行 3161：空行，用于分隔代码块，提高可读性。

# 行 3162：普通语句：执行一步操作或表达式。
        style.configure(
# 行 3163：普通语句：执行一步操作或表达式。
            "Custom.Treeview.Heading",
# 行 3164：变量赋值：保存配置、状态或计算结果。
            background="#e5e5e5",
# 行 3165：普通语句：执行一步操作或表达式。
        )
# 行 3166：空行，用于分隔代码块，提高可读性。

# 行 3167：普通语句：执行一步操作或表达式。
        style.map(
# 行 3168：普通语句：执行一步操作或表达式。
            "Custom.Treeview",
# 行 3169：变量赋值：保存配置、状态或计算结果。
            background=[('selected', 'skyblue')]
# 行 3170：普通语句：执行一步操作或表达式。
        )
# 行 3171：空行，用于分隔代码块，提高可读性。

# 行 3172：变量赋值：保存配置、状态或计算结果。
        self.c_tree = ttk.Treeview(
# 行 3173：普通语句：执行一步操作或表达式。
            self.c_table_frame,
# 行 3174：变量赋值：保存配置、状态或计算结果。
            columns=[str(i) for i in range(len(self.header_list))],
# 行 3175：变量赋值：保存配置、状态或计算结果。
            show='headings',
# 行 3176：变量赋值：保存配置、状态或计算结果。
            height=10,
# 行 3177：变量赋值：保存配置、状态或计算结果。
            style="Custom.Treeview"
# 行 3178：普通语句：执行一步操作或表达式。
        )
# 行 3179：空行，用于分隔代码块，提高可读性。

# 行 3180：for 循环遍历序列或可迭代对象。
        for i in range(len(self.header_list)):
# 行 3181：普通语句：执行一步操作或表达式。
            self.c_tree.heading(str(i), text=f'{self.header_list[i]}')
# 行 3182：普通语句：执行一步操作或表达式。
            self.c_tree.column(str(i), width=102, anchor='center')
# 行 3183：空行，用于分隔代码块，提高可读性。

# 行 3184：变量赋值：保存配置、状态或计算结果。
        scrollbar = ttk.Scrollbar(self.c_table_frame, orient=tk.VERTICAL, command=self.c_tree.yview)
# 行 3185：普通语句：执行一步操作或表达式。
        self.c_tree.configure(yscrollcommand=scrollbar.set)
# 行 3186：空行，用于分隔代码块，提高可读性。

# 行 3187：普通语句：执行一步操作或表达式。
        self.c_tree.grid(row=0, column=0, sticky='nsew')
# 行 3188：普通语句：执行一步操作或表达式。
        scrollbar.grid(row=0, column=1, sticky='ns')
# 行 3189：空行，用于分隔代码块，提高可读性。

# 行 3190：普通语句：执行一步操作或表达式。
        self.c_table_frame.grid_rowconfigure(0, weight=1)
# 行 3191：普通语句：执行一步操作或表达式。
        self.c_table_frame.grid_columnconfigure(0, weight=1)
# 行 3192：空行，用于分隔代码块，提高可读性。

# 行 3193：普通语句：执行一步操作或表达式。
        self.c_tree.bind("<ButtonRelease-1>", self.c_on_item_click)
# 行 3194：定义函数/方法 c_on_item_click(self, event)。
    def c_on_item_click(self, event):
# 行 3195：变量赋值：保存配置、状态或计算结果。
        selected_item = self.c_tree.selection()
# 行 3196：条件判断，根据不同条件执行不同分支。
        if selected_item:
# 行 3197：变量赋值：保存配置、状态或计算结果。
            item_values = self.c_tree.item(selected_item[0])['values']
# 行 3198：变量赋值：保存配置、状态或计算结果。
            x_str = item_values[-2]
# 行 3199：变量赋值：保存配置、状态或计算结果。
            x_str_list = x_str.split()
# 行 3200：变量赋值：保存配置、状态或计算结果。
            x_float_list = [float(s) for s in x_str_list]
# 行 3201：变量赋值：保存配置、状态或计算结果。
            y_str = item_values[-1]
# 行 3202：变量赋值：保存配置、状态或计算结果。
            y_str_list = y_str.split()
# 行 3203：变量赋值：保存配置、状态或计算结果。
            y_float_list = [float(s) for s in y_str_list]
# 行 3204：普通语句：执行一步操作或表达式。
            self.c_update_plot_event1(x_float_list, y_float_list)
# 行 3205：空行，用于分隔代码块，提高可读性。

# 行 3206：定义函数/方法 c_on_confirm(self, window, folder_path1, folder_path2)。
    def c_on_confirm(self, window, folder_path1, folder_path2):
# 行 3207：条件判断，根据不同条件执行不同分支。
        if folder_path1 and folder_path2:
# 行 3208：变量赋值：保存配置、状态或计算结果。
            self.c_save_path = folder_path1 + '/' + folder_path2 + '.csv'
# 行 3209：打印调试或状态信息到控制台。
            print(self.c_save_path)
# 行 3210：普通语句：执行一步操作或表达式。
            window.destroy()
# 行 3211：使用 with 上下文安全地打开文件，结束时自动关闭。
            with open(self.c_save_path, mode='w', newline='', encoding='gbk') as file:
# 行 3212：变量赋值：保存配置、状态或计算结果。
                writer = csv.writer(file)
# 行 3213：普通语句：执行一步操作或表达式。
                writer.writerow([
# 行 3214：普通语句：执行一步操作或表达式。
                    '事件数', '基线(nA)', '振幅(nA)', '积分(10-9C)', '持续时间(ms)',
# 行 3215：普通语句：执行一步操作或表达式。
                    '钝度', '脉冲不对称', '横轴惯性', '纵轴惯性',
# 行 3216：普通语句：执行一步操作或表达式。
                    '事件数据(时间)', '事件数据(电流)'
# 行 3217：普通语句：执行一步操作或表达式。
                ])
# 行 3218：for 循环遍历序列或可迭代对象。
                for i in range(len(self.c_event_list)):
# 行 3219：普通语句：执行一步操作或表达式。
                    writer.writerow(self.c_event_list[i])
# 行 3220：定义函数/方法 c_text_display_func(self)。
    def c_text_display_func(self):
# 行 3221：变量赋值：保存配置、状态或计算结果。
        self.c_text_display = tk.Text(self.analyse_page_original, width=60, height=30, wrap=tk.WORD, bg="#f0f0f0",
# 行 3222：变量赋值：保存配置、状态或计算结果。
                                    font=("", 10), state=tk.DISABLED)
# 行 3223：普通语句：执行一步操作或表达式。
        self.c_text_display.place(x=960, y=50)
# 行 3224：空行，用于分隔代码块，提高可读性。

# 行 3225：变量赋值：保存配置、状态或计算结果。
        self.scrollbar = tk.Scrollbar(self.analyse_page_original, command=self.c_text_display.yview)
# 行 3226：普通语句：执行一步操作或表达式。
        self.scrollbar.place(x=1385, y=50, height=400)
# 行 3227：普通语句：执行一步操作或表达式。
        self.c_text_display.config(yscrollcommand=self.scrollbar.set)
# 行 3228：空行，用于分隔代码块，提高可读性。

# 行 3229：普通语句：执行一步操作或表达式。
        self.c_text_display.bind("<Double-1>", self.on_text_click)
# 行 3230：定义函数/方法 on_text_click(self, event)。
    def on_text_click(self, event):
# 行 3231：变量赋值：保存配置、状态或计算结果。
        index = self.c_text_display.index(f"@{event.x},{event.y}")
# 行 3232：变量赋值：保存配置、状态或计算结果。
        self.c_line_number = index.split('.')[0]
# 行 3233：空行，用于分隔代码块，提高可读性。

# 行 3234：变量赋值：保存配置、状态或计算结果。
        line_content = self.c_text_display.get(f"{self.c_line_number}.0", f"{self.c_line_number}.end").strip()
# 行 3235：变量赋值：保存配置、状态或计算结果。
        self.c_preview_file_name = line_content
# 行 3236：条件判断，根据不同条件执行不同分支。
        if line_content:
# 行 3237：普通语句：执行一步操作或表达式。
            self.c_text_display.tag_remove("highlight", "1.0", "end")
# 行 3238：空行，用于分隔代码块，提高可读性。

# 行 3239：普通语句：执行一步操作或表达式。
            self.c_text_display.config(state=tk.NORMAL)
# 行 3240：普通语句：执行一步操作或表达式。
            self.c_text_display.tag_add("highlight", f"{self.c_line_number}.0", f"{self.c_line_number}.end")
# 行 3241：普通语句：执行一步操作或表达式。
            self.c_text_display.tag_config("highlight", background="lightblue")
# 行 3242：空行，用于分隔代码块，提高可读性。

# 行 3243：普通语句：执行一步操作或表达式。
            self.c_text_display.config(state=tk.DISABLED)
# 行 3244：变量赋值：保存配置、状态或计算结果。
            df = pd.read_csv(line_content, encoding='gbk')
# 行 3245：变量赋值：保存配置、状态或计算结果。
            events_baseline = df['基线(nA)']
# 行 3246：变量赋值：保存配置、状态或计算结果。
            self.c_events_list_baseline_2 = events_baseline.tolist()
# 行 3247：变量赋值：保存配置、状态或计算结果。
            events_amplitude = df['振幅(nA)']
# 行 3248：变量赋值：保存配置、状态或计算结果。
            self.c_events_list_amplitude_2 = events_amplitude.tolist()
# 行 3249：变量赋值：保存配置、状态或计算结果。
            events_integral = df['积分(10-9C)']
# 行 3250：变量赋值：保存配置、状态或计算结果。
            self.c_events_list_integral_2 = events_integral.tolist()
# 行 3251：变量赋值：保存配置、状态或计算结果。
            events_duration = df['持续时间(ms)']
# 行 3252：变量赋值：保存配置、状态或计算结果。
            self.c_events_list_duration_2 = events_duration.tolist()
# 行 3253：变量赋值：保存配置、状态或计算结果。
            events_bluntness = df['钝度']
# 行 3254：变量赋值：保存配置、状态或计算结果。
            self.c_events_list_bluntness_2 = events_bluntness.tolist()
# 行 3255：变量赋值：保存配置、状态或计算结果。
            events_asymmetry = df['脉冲不对称']
# 行 3256：变量赋值：保存配置、状态或计算结果。
            self.c_events_list_asymmetry_2 = events_asymmetry.tolist()
# 行 3257：变量赋值：保存配置、状态或计算结果。
            events_x_inertia = df['横轴惯性']
# 行 3258：变量赋值：保存配置、状态或计算结果。
            self.c_events_list_x_inertia_2 = events_x_inertia.tolist()
# 行 3259：变量赋值：保存配置、状态或计算结果。
            events_y_inertia = df['纵轴惯性']
# 行 3260：变量赋值：保存配置、状态或计算结果。
            self.c_events_list_y_inertia_2 = events_y_inertia.tolist()
# 行 3261：普通语句：执行一步操作或表达式。
            self.c_undate_plot_histogram_2(self.c_events_list_amplitude_2)
# 行 3262：普通语句：执行一步操作或表达式。
            self.c_update_plot_2D_2(self.c_events_list_duration_2, self.c_events_list_amplitude_2)
# 行 3263：原始注释行，提供作者或用途说明。
    # def add_file_name(self, filename):
# 行 3264：原始注释行，提供作者或用途说明。
    #     line_index = self.c_text_display.index(tk.END)
# 行 3265：原始注释行，提供作者或用途说明。
    #     self.c_text_display.insert(tk.END, f'{filename}\n')
# 行 3266：原始注释行，提供作者或用途说明。
    #     self.c_text_display.yview(tk.END)
# 行 3267：定义函数/方法 c_text_display_func_2(self)。
    def c_text_display_func_2(self):
# 行 3268：变量赋值：保存配置、状态或计算结果。
        self.c_text_display_2 = tk.Text(self.analyse_page_original, width=60, height=30, wrap=tk.WORD, bg="#f0f0f0",
# 行 3269：变量赋值：保存配置、状态或计算结果。
                                    font=("", 10), state=tk.DISABLED)
# 行 3270：普通语句：执行一步操作或表达式。
        self.c_text_display_2.place(x=960, y=530)
# 行 3271：空行，用于分隔代码块，提高可读性。

# 行 3272：变量赋值：保存配置、状态或计算结果。
        self.scrollbar_2 = tk.Scrollbar(self.analyse_page_original, command=self.c_text_display_2.yview)
# 行 3273：普通语句：执行一步操作或表达式。
        self.scrollbar_2.place(x=1385, y=530, height=400)
# 行 3274：普通语句：执行一步操作或表达式。
        self.c_text_display_2.config(yscrollcommand=self.scrollbar_2.set)
# 行 3275：空行，用于分隔代码块，提高可读性。

# 行 3276：普通语句：执行一步操作或表达式。
        self.c_text_display_2.bind("<Double-1>", self.on_text_click_2)
# 行 3277：空行，用于分隔代码块，提高可读性。

# 行 3278：定义函数/方法 on_text_click_2(self, event)。
    def on_text_click_2(self, event):
# 行 3279：变量赋值：保存配置、状态或计算结果。
        index = self.c_text_display_2.index(f"@{event.x},{event.y}")
# 行 3280：变量赋值：保存配置、状态或计算结果。
        self.c_line_number = index.split('.')[0]
# 行 3281：空行，用于分隔代码块，提高可读性。

# 行 3282：变量赋值：保存配置、状态或计算结果。
        line_content = self.c_text_display_2.get(f"{self.c_line_number}.0", f"{self.c_line_number}.end").strip()
# 行 3283：条件判断，根据不同条件执行不同分支。
        if line_content:
# 行 3284：变量赋值：保存配置、状态或计算结果。
            content = line_content.strip('()')
# 行 3285：变量赋值：保存配置、状态或计算结果。
            result = [item.strip("'\"") for item in content.split(', ')]
# 行 3286：变量赋值：保存配置、状态或计算结果。
            self.c_statistics_file_name = result
# 行 3287：普通语句：执行一步操作或表达式。
            self.c_text_display_2.tag_remove("highlight", "1.0", "end")
# 行 3288：空行，用于分隔代码块，提高可读性。

# 行 3289：普通语句：执行一步操作或表达式。
            self.c_text_display_2.config(state=tk.NORMAL)
# 行 3290：普通语句：执行一步操作或表达式。
            self.c_text_display_2.tag_add("highlight", f"{self.c_line_number}.0", f"{self.c_line_number}.end")
# 行 3291：普通语句：执行一步操作或表达式。
            self.c_text_display_2.tag_config("highlight", background="lightblue")
# 行 3292：普通语句：执行一步操作或表达式。
            self.c_text_display_2.config(state=tk.DISABLED)
# 行 3293：定义函数/方法 add_analyse_page(self)。
    def add_analyse_page(self):
# 行 3294：变量赋值：保存配置、状态或计算结果。
        self.analyse_page_original = tk.Frame(self.root,
# 行 3295：变量赋值：保存配置、状态或计算结果。
                                        width=1950,
# 行 3296：变量赋值：保存配置、状态或计算结果。
                                        height=1000,
# 行 3297：变量赋值：保存配置、状态或计算结果。
                                        bg='#e5e5e5',
# 行 3298：变量赋值：保存配置、状态或计算结果。
                                        bd=5,
# 行 3299：变量赋值：保存配置、状态或计算结果。
                                        relief=tk.RAISED)
# 行 3300：普通语句：执行一步操作或表达式。
        self.analyse_page_original.place(x=0, y=0)
# 行 3301：变量赋值：保存配置、状态或计算结果。
        self.canvas_c_picture = tk.Canvas(self.analyse_page_original, width=1905, height=980, bg='#e5e5e5',
# 行 3302：变量赋值：保存配置、状态或计算结果。
                                          bd=0)
# 行 3303：普通语句：执行一步操作或表达式。
        self.canvas_c_picture.place(x=0, y=0)
# 行 3304：普通语句：执行一步操作或表达式。
        self.create_dashed_square(self.canvas_c_picture, 3, 3, 950, 977, 1)
# 行 3305：普通语句：执行一步操作或表达式。
        self.create_dashed_square(self.canvas_c_picture, 950, 3, 1905, 977, 1)
# 行 3306：空行，用于分隔代码块，提高可读性。

# 行 3307：定义函数/方法 open_new_window()。
        def open_new_window():
# 行 3308：变量赋值：保存配置、状态或计算结果。
            new_window = tk.Toplevel(self.root)
# 行 3309：普通语句：执行一步操作或表达式。
            new_window.title("打开文件")
# 行 3310：普通语句：执行一步操作或表达式。
            new_window.geometry("330x120")
# 行 3311：普通语句：执行一步操作或表达式。
            new_window.grab_set()
# 行 3312：空行，用于分隔代码块，提高可读性。

# 行 3313：变量赋值：保存配置、状态或计算结果。
            path_entry = create_entry(new_window, 30, 69, 30)
# 行 3314：普通语句：执行一步操作或表达式。
            create_label(new_window, "文件信息", 3, 30)
# 行 3315：条件判断，根据不同条件执行不同分支。
            if self.c_file_folder_choice == False:
# 行 3316：变量赋值：保存配置、状态或计算结果。
                browse_button = tk.Button(new_window, text="浏览", bg='#e5e5e5', font=('', 9),
# 行 3317：变量赋值：保存配置、状态或计算结果。
                                          command=lambda: browse_file1(path_entry,
# 行 3318：普通语句：执行一步操作或表达式。
                                                                      [("ABF files", "*.abf"), ("Text files", "*.txt")]))
# 行 3319：空行，用于分隔代码块，提高可读性。

# 行 3320：普通语句：执行一步操作或表达式。
                browse_button.place(x=287, y=30)
# 行 3321：变量赋值：保存配置、状态或计算结果。
                confirm_button = tk.Button(new_window, text="确定", bg='#e5e5e5', font=('', 9),
# 行 3322：变量赋值：保存配置、状态或计算结果。
                                           command=lambda: on_confirm(new_window, path_entry.get()))
# 行 3323：普通语句：执行一步操作或表达式。
                confirm_button.place(x=140, y=80)
# 行 3324：条件判断，根据不同条件执行不同分支。
            else:
# 行 3325：变量赋值：保存配置、状态或计算结果。
                browse_button = tk.Button(new_window, text="浏览", bg='#e5e5e5', font=('', 9),
# 行 3326：变量赋值：保存配置、状态或计算结果。
                                          command=lambda: browse_file(path_entry))
# 行 3327：空行，用于分隔代码块，提高可读性。

# 行 3328：普通语句：执行一步操作或表达式。
                browse_button.place(x=287, y=30)
# 行 3329：变量赋值：保存配置、状态或计算结果。
                confirm_button = tk.Button(new_window, text="确定", bg='#e5e5e5', font=('', 9),
# 行 3330：变量赋值：保存配置、状态或计算结果。
                                           command=lambda: on_confirm(new_window, path_entry.get()))
# 行 3331：普通语句：执行一步操作或表达式。
                confirm_button.place(x=140, y=80)
# 行 3332：空行，用于分隔代码块，提高可读性。

# 行 3333：定义函数/方法 on_confirm(window, folder_path)。
        def on_confirm(window, folder_path):
# 行 3334：条件判断，根据不同条件执行不同分支。
            if folder_path:
# 行 3335：普通语句：执行一步操作或表达式。
                self.c1_var_event.set(0)
# 行 3336：空行，用于分隔代码块，提高可读性。

# 行 3337：普通语句：执行一步操作或表达式。
                self.c2_var_detection_sigma.set(0)
# 行 3338：空行，用于分隔代码块，提高可读性。

# 行 3339：普通语句：执行一步操作或表达式。
                self.c2_var_filter.set(0)
# 行 3340：变量赋值：保存配置、状态或计算结果。
                self.c_filter = False
# 行 3341：变量赋值：保存配置、状态或计算结果。
                self.c_event = False
# 行 3342：变量赋值：保存配置、状态或计算结果。
                self.c_detection_threshold = False
# 行 3343：变量赋值：保存配置、状态或计算结果。
                save_path = os.path.join(folder_path)
# 行 3344：for 循环遍历序列或可迭代对象。
                for item in self.c_tree.get_children():
# 行 3345：普通语句：执行一步操作或表达式。
                    self.c_tree.delete(item)
# 行 3346：普通语句：执行一步操作或表达式。
                self.c_plot_event1()
# 行 3347：普通语句：执行一步操作或表达式。
                self.c_plot_2d()
# 行 3348：普通语句：执行一步操作或表达式。
                self.c_plot_histogram()
# 行 3349：变量赋值：保存配置、状态或计算结果。
                self.c_baseline_list = []
# 行 3350：变量赋值：保存配置、状态或计算结果。
                self.c_amplitude_list = []
# 行 3351：变量赋值：保存配置、状态或计算结果。
                self.c_integral_list = []
# 行 3352：变量赋值：保存配置、状态或计算结果。
                self.c_duration_list = []
# 行 3353：变量赋值：保存配置、状态或计算结果。
                self.c_bluntness_list = []
# 行 3354：变量赋值：保存配置、状态或计算结果。
                self.c_asymmetry_list = []
# 行 3355：变量赋值：保存配置、状态或计算结果。
                self.c_x_inertia_list = []
# 行 3356：变量赋值：保存配置、状态或计算结果。
                self.c_y_inertia_list = []
# 行 3357：变量赋值：保存配置、状态或计算结果。
                self.c_file_abf_list = []
# 行 3358：条件判断，根据不同条件执行不同分支。
                if self.c_file_folder_choice == False:
# 行 3359：条件判断，根据不同条件执行不同分支。
                    if folder_path[-3:] == 'abf':
# 行 3360：普通语句：执行一步操作或表达式。
                        self.c1_file_path_entry.config(state=tk.NORMAL)  # 允许修改文本框
# 行 3361：普通语句：执行一步操作或表达式。
                        self.c1_file_path_entry.delete(0, tk.END)
# 行 3362：普通语句：执行一步操作或表达式。
                        self.c1_file_path_entry.insert(0, folder_path)
# 行 3363：普通语句：执行一步操作或表达式。
                        self.c1_file_path_entry.config(state=tk.DISABLED)  # 禁止编辑
# 行 3364：变量赋值：保存配置、状态或计算结果。
                        abf = pyabf.ABF(folder_path)
# 行 3365：变量赋值：保存配置、状态或计算结果。
                        self.c1_x = abf.sweepX[:500000 * 2]
# 行 3366：变量赋值：保存配置、状态或计算结果。
                        self.c1_y = abf.sweepY[:500000 * 2]
# 行 3367：原始注释行，提供作者或用途说明。
                        # self.c1_x = self.c1_x[:len(self.c1_x) // (40 * 8)]
# 行 3368：原始注释行，提供作者或用途说明。
                        # self.c1_y = self.c1_y[:len(self.c1_y) // (40 * 8)]
# 行 3369：打印调试或状态信息到控制台。
                        print(abf.dataRate)
# 行 3370：打印调试或状态信息到控制台。
                        print(self.c1_x)
# 行 3371：普通语句：执行一步操作或表达式。
                        window.destroy()
# 行 3372：变量赋值：保存配置、状态或计算结果。
                        self.c_frequency = abf.dataRate
# 行 3373：普通语句：执行一步操作或表达式。
                        self.update_c_plot_current(self.c1_x, self.c1_y)
# 行 3374：条件判断，根据不同条件执行不同分支。
                    else:
# 行 3375：普通语句：执行一步操作或表达式。
                        self.c1_file_path_entry.config(state=tk.NORMAL)  # 允许修改文本框
# 行 3376：普通语句：执行一步操作或表达式。
                        self.c1_file_path_entry.delete(0, tk.END)
# 行 3377：普通语句：执行一步操作或表达式。
                        self.c1_file_path_entry.insert(0, folder_path)
# 行 3378：普通语句：执行一步操作或表达式。
                        self.c1_file_path_entry.config(state=tk.DISABLED)  # 禁止编辑
# 行 3379：变量赋值：保存配置、状态或计算结果。
                        x_list = []
# 行 3380：变量赋值：保存配置、状态或计算结果。
                        y_list = []
# 行 3381：空行，用于分隔代码块，提高可读性。

# 行 3382：使用 with 上下文安全地打开文件，结束时自动关闭。
                        with open(folder_path, 'r', encoding='gbk') as file:
# 行 3383：for 循环遍历序列或可迭代对象。
                            for _ in range(3):
# 行 3384：普通语句：执行一步操作或表达式。
                                next(file)
# 行 3385：空行，用于分隔代码块，提高可读性。

# 行 3386：for 循环遍历序列或可迭代对象。
                            for line in file:
# 行 3387：变量赋值：保存配置、状态或计算结果。
                                parts = line.strip().split()
# 行 3388：条件判断，根据不同条件执行不同分支。
                                if len(parts) == 2:
# 行 3389：普通语句：执行一步操作或表达式。
                                    x_list.append(float(parts[0]))
# 行 3390：普通语句：执行一步操作或表达式。
                                    y_list.append(float(parts[1]))
# 行 3391：空行，用于分隔代码块，提高可读性。

# 行 3392：变量赋值：保存配置、状态或计算结果。
                        self.c1_x = np.array(x_list)
# 行 3393：变量赋值：保存配置、状态或计算结果。
                        self.c1_y = np.array(y_list)
# 行 3394：打印调试或状态信息到控制台。
                        print(self.c1_x)
# 行 3395：普通语句：执行一步操作或表达式。
                        window.destroy()
# 行 3396：普通语句：执行一步操作或表达式。
                        self.update_c_plot_current(self.c1_x, self.c1_y)
# 行 3397：变量赋值：保存配置、状态或计算结果。
                        self.c_frequency = 100000
# 行 3398：条件判断，根据不同条件执行不同分支。
                else:
# 行 3399：普通语句：执行一步操作或表达式。
                    self.c1_file_path_entry.config(state=tk.NORMAL)  # 允许修改文本框
# 行 3400：普通语句：执行一步操作或表达式。
                    self.c1_file_path_entry.delete(0, tk.END)
# 行 3401：普通语句：执行一步操作或表达式。
                    self.c1_file_path_entry.insert(0, folder_path)
# 行 3402：普通语句：执行一步操作或表达式。
                    self.c1_file_path_entry.config(state=tk.DISABLED)  # 禁止编辑
# 行 3403：变量赋值：保存配置、状态或计算结果。
                    source_directory = folder_path
# 行 3404：for 循环遍历序列或可迭代对象。
                    for filename in os.listdir(source_directory):
# 行 3405：条件判断，根据不同条件执行不同分支。
                        if filename.endswith(".abf"):
# 行 3406：变量赋值：保存配置、状态或计算结果。
                            file_path = os.path.join(source_directory, filename)
# 行 3407：普通语句：执行一步操作或表达式。
                            self.c_file_abf_list.append(file_path)
# 行 3408：空行，用于分隔代码块，提高可读性。

# 行 3409：普通语句：执行一步操作或表达式。
                    window.destroy()
# 行 3410：条件判断，根据不同条件执行不同分支。
            else:
# 行 3411：普通语句：执行一步操作或表达式。
                messagebox.showwarning("警告", "请传入有效路径！")
# 行 3412：空行，用于分隔代码块，提高可读性。

# 行 3413：定义函数/方法 tree_insert(event_list)。
        def tree_insert(event_list):
# 行 3414：原始注释行，提供作者或用途说明。
            # print(event_list[0])
# 行 3415：for 循环遍历序列或可迭代对象。
            for item in self.c_tree.get_children():
# 行 3416：普通语句：执行一步操作或表达式。
                self.c_tree.delete(item)
# 行 3417：for 循环遍历序列或可迭代对象。
            for i in range(len(event_list)):
# 行 3418：普通语句：执行一步操作或表达式。
                self.c_tree.insert('', tk.END, values=event_list[i])
# 行 3419：定义函数/方法 c_start_detection_func()。
        def c_start_detection_func():
# 行 3420：普通语句：执行一步操作或表达式。
            self.c1_var_event.set(0)
# 行 3421：变量赋值：保存配置、状态或计算结果。
            self.c_detection_sigma = float(self.c_detection_sigma_param.get())
# 行 3422：普通语句：执行一步操作或表达式。
            self.c2_var_detection_sigma.set(0)
# 行 3423：变量赋值：保存配置、状态或计算结果。
            self.c_event = False
# 行 3424：变量赋值：保存配置、状态或计算结果。
            self.c_detection_threshold = False
# 行 3425：for 循环遍历序列或可迭代对象。
            for item in self.c_tree.get_children():
# 行 3426：普通语句：执行一步操作或表达式。
                self.c_tree.delete(item)
# 行 3427：普通语句：执行一步操作或表达式。
            self.c_plot_event1()
# 行 3428：普通语句：执行一步操作或表达式。
            self.c_plot_2d()
# 行 3429：普通语句：执行一步操作或表达式。
            self.c_plot_histogram()
# 行 3430：变量赋值：保存配置、状态或计算结果。
            self.c_baseline_list = []
# 行 3431：变量赋值：保存配置、状态或计算结果。
            self.c_amplitude_list = []
# 行 3432：变量赋值：保存配置、状态或计算结果。
            self.c_integral_list = []
# 行 3433：变量赋值：保存配置、状态或计算结果。
            self.c_duration_list = []
# 行 3434：变量赋值：保存配置、状态或计算结果。
            self.c_bluntness_list = []
# 行 3435：变量赋值：保存配置、状态或计算结果。
            self.c_asymmetry_list = []
# 行 3436：变量赋值：保存配置、状态或计算结果。
            self.c_x_inertia_list = []
# 行 3437：变量赋值：保存配置、状态或计算结果。
            self.c_y_inertia_list = []
# 行 3438：空行，用于分隔代码块，提高可读性。

# 行 3439：变量赋值：保存配置、状态或计算结果。
            self.b_event_list_two = []
# 行 3440：变量赋值：保存配置、状态或计算结果。
            self.b_event_amplitude_two= []
# 行 3441：变量赋值：保存配置、状态或计算结果。
            self.b_event_duration_two= []
# 行 3442：变量赋值：保存配置、状态或计算结果。
            self.c_event_list_two = []
# 行 3443：变量赋值：保存配置、状态或计算结果。
            self.c_detection_mintime = float(self.c_detection_mintime_param.get())
# 行 3444：变量赋值：保存配置、状态或计算结果。
            self.c_detection_maxtime = float(self.c_detection_maxtime_param.get())
# 行 3445：条件判断，根据不同条件执行不同分支。
            if self.c_file_folder_choice == False:
# 行 3446：条件判断，根据不同条件执行不同分支。
                if self.c_filter == True:
# 行 3447：变量赋值：保存配置、状态或计算结果。
                    self.c_baseline = dynamic_detection.calculate_dynamic_baseline_fast_trimmed_py(self.c1_y_filter,
# 行 3448：普通语句：执行一步操作或表达式。
                                                                                                   (float(
# 行 3449：普通语句：执行一步操作或表达式。
                                                                                                       self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000)
# 行 3450：普通语句：执行一步操作或表达式。
                    self.c_std_down, self.c_std_up = np.array(
# 行 3451：普通语句：执行一步操作或表达式。
                        dynamic_detection.detection_y_py(self.c1_y_filter, (
# 行 3452：普通语句：执行一步操作或表达式。
                                float(self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000,
# 行 3453：普通语句：执行一步操作或表达式。
                                                         float(self.c_detection_sigma)))
# 行 3454：普通语句：执行一步操作或表达式。
                    self.c_event_list, self.c_event_list_number = self.event_detection(self.c1_x, self.c1_y_filter, self.c_baseline, self.c_std_up, self.c_std_down, self.c_detection_three, self.c_frequency, 1, 0,
# 行 3455：普通语句：执行一步操作或表达式。
                                         self.c_detection_mintime, self.c_detection_maxtime, 0)
# 行 3456：条件判断，根据不同条件执行不同分支。
                else:
# 行 3457：变量赋值：保存配置、状态或计算结果。
                    self.c_baseline = dynamic_detection.calculate_dynamic_baseline_fast_trimmed_py(self.c1_y,
# 行 3458：普通语句：执行一步操作或表达式。
                                                                                                   (float(
# 行 3459：普通语句：执行一步操作或表达式。
                                                                                                       self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000)
# 行 3460：普通语句：执行一步操作或表达式。
                    self.c_std_down, self.c_std_up = np.array(
# 行 3461：普通语句：执行一步操作或表达式。
                        dynamic_detection.detection_y_py(self.c1_y, (
# 行 3462：普通语句：执行一步操作或表达式。
                                float(self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000,
# 行 3463：普通语句：执行一步操作或表达式。
                                                         float(self.c_detection_sigma)))
# 行 3464：普通语句：执行一步操作或表达式。
                    self.c_event_list, self.c_event_list_number = self.event_detection(self.c1_x, self.c1_y,
# 行 3465：普通语句：执行一步操作或表达式。
                                                                                       self.c_baseline, self.c_std_up,
# 行 3466：普通语句：执行一步操作或表达式。
                                                                                       self.c_std_down,
# 行 3467：普通语句：执行一步操作或表达式。
                                                                                       self.c_detection_three,
# 行 3468：普通语句：执行一步操作或表达式。
                                                                                       self.c_frequency, 1, 0,
# 行 3469：普通语句：执行一步操作或表达式。
                                                                                       self.c_detection_mintime,
# 行 3470：普通语句：执行一步操作或表达式。
                                                                                       self.c_detection_maxtime, 0)
# 行 3471：for 循环遍历序列或可迭代对象。
                for i in range(len(self.c_event_list)):
# 行 3472：变量赋值：保存配置、状态或计算结果。
                    list1 = self.c_event_list[i]
# 行 3473：变量赋值：保存配置、状态或计算结果。
                    y = list1[10]
# 行 3474：原始注释行，提供作者或用途说明。
                    # down, up = self.event_detection_two(y, self.c_detection_three, list1[4])
# 行 3475：普通语句：执行一步操作或表达式。
                    down, up = self.event_detection_two(list1[9], list1[10], 0.01, 0.1)
# 行 3476：条件判断，根据不同条件执行不同分支。
                    if down or up:
# 行 3477：普通语句：执行一步操作或表达式。
                        self.c_event_list_two.append(list1)
# 行 3478：空行，用于分隔代码块，提高可读性。

# 行 3479：原始注释行，提供作者或用途说明。
                    # baseline = dynamic_detection.calculate_dynamic_baseline_fast_trimmed_py(list1[9],
# 行 3480：原始注释行，提供作者或用途说明。
                    #                                                                                (float(
# 行 3481：原始注释行，提供作者或用途说明。
                    #                                                                                    self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000)
# 行 3482：原始注释行，提供作者或用途说明。
                    # c_std_down, c_std_up = np.array(
# 行 3483：原始注释行，提供作者或用途说明。
                    #     dynamic_detection.detection_y_py(list1[9], (
# 行 3484：原始注释行，提供作者或用途说明。
                    #             float(self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000,
# 行 3485：原始注释行，提供作者或用途说明。
                    #                                      float(self.c_detection_sigma)))
# 行 3486：原始注释行，提供作者或用途说明。
                    # list2, number = self.event_detection(list1[8], list1[9],
# 行 3487：原始注释行，提供作者或用途说明。
                    #                                                                    self.c_baseline, self.c_std_up,
# 行 3488：原始注释行，提供作者或用途说明。
                    #                                                                    self.c_std_down,
# 行 3489：原始注释行，提供作者或用途说明。
                    #                                                                    self.c_detection_three,
# 行 3490：原始注释行，提供作者或用途说明。
                    #                                                                    self.c_frequency, 20, 0,
# 行 3491：原始注释行，提供作者或用途说明。
                    #                                                                    self.c_detection_mintime,
# 行 3492：原始注释行，提供作者或用途说明。
                    #                                                                    self.c_detection_maxtime, 0)
# 行 3493：普通语句：执行一步操作或表达式。
                    self.c_baseline_list.append(float(list1[1]))
# 行 3494：普通语句：执行一步操作或表达式。
                    self.c_amplitude_list.append(float(list1[2]))
# 行 3495：普通语句：执行一步操作或表达式。
                    self.c_integral_list.append(float(list1[3]))
# 行 3496：普通语句：执行一步操作或表达式。
                    self.c_duration_list.append(float(list1[4]))
# 行 3497：普通语句：执行一步操作或表达式。
                    self.c_bluntness_list.append(float(list1[5]))
# 行 3498：普通语句：执行一步操作或表达式。
                    self.c_asymmetry_list.append(float(list1[6]))
# 行 3499：普通语句：执行一步操作或表达式。
                    self.c_x_inertia_list.append(float(list1[7]))
# 行 3500：普通语句：执行一步操作或表达式。
                    self.c_y_inertia_list.append(float(list1[8]))
# 行 3501：变量赋值：保存配置、状态或计算结果。
                list_x = []
# 行 3502：变量赋值：保存配置、状态或计算结果。
                list_y = []
# 行 3503：变量赋值：保存配置、状态或计算结果。
                number = len(self.c_event_list_two)
# 行 3504：for 循环遍历序列或可迭代对象。
                for i in range(number):
# 行 3505：普通语句：执行一步操作或表达式。
                    list_x.append(self.c_event_list_two[i][9])
# 行 3506：普通语句：执行一步操作或表达式。
                    list_y.append(self.c_event_list_two[i][10])
# 行 3507：变量赋值：保存配置、状态或计算结果。
                valid_number = len(list_x)
# 行 3508：空行，用于分隔代码块，提高可读性。

# 行 3509：变量赋值：保存配置、状态或计算结果。
                rows = int(np.ceil(np.sqrt(valid_number)))
# 行 3510：变量赋值：保存配置、状态或计算结果。
                cols = int(np.ceil(valid_number / rows))
# 行 3511：变量赋值：保存配置、状态或计算结果。
                fig_size = (cols * 2.5, rows * 2.5)  # 每列2.5英寸宽，每行2.5英寸高
# 行 3512：普通语句：执行一步操作或表达式。
                fig, axes = plt.subplots(nrows=rows, ncols=cols, figsize=fig_size)
# 行 3513：变量赋值：保存配置、状态或计算结果。
                axes_flat = axes.flatten() if (rows > 1 and cols > 1) else [axes]
# 行 3514：for 循环遍历序列或可迭代对象。
                for idx, ax in enumerate(axes_flat):
# 行 3515：条件判断，根据不同条件执行不同分支。
                    if idx < valid_number:
# 行 3516：普通语句：执行一步操作或表达式。
                        ax.plot(list_x[idx], list_y[idx])
# 行 3517：空行，用于分隔代码块，提高可读性。

# 行 3518：普通语句：执行一步操作或表达式。
                        ax.tick_params(axis='both', labelsize=7)
# 行 3519：空行，用于分隔代码块，提高可读性。

# 行 3520：普通语句：执行一步操作或表达式。
                        ax.grid(True, alpha=0.2, linestyle='--')
# 行 3521：条件判断，根据不同条件执行不同分支。
                    else:
# 行 3522：普通语句：执行一步操作或表达式。
                        ax.axis('off')
# 行 3523：原始注释行，提供作者或用途说明。
                # plt.subplots_adjust(hspace=hspace, wspace=wspace)
# 行 3524：普通语句：执行一步操作或表达式。
                plt.show()
# 行 3525：空行，用于分隔代码块，提高可读性。

# 行 3526：普通语句：执行一步操作或表达式。
                self.update_c_plot_current(self.c1_x, self.c1_y)
# 行 3527：普通语句：执行一步操作或表达式。
                tree_insert(self.c_event_list)
# 行 3528：普通语句：执行一步操作或表达式。
                self.c_update_plot_2D(self.c_duration_list, self.c_amplitude_list)
# 行 3529：条件判断，根据不同条件执行不同分支。
            else:
# 行 3530：变量赋值：保存配置、状态或计算结果。
                self.c_event_list = defaultdict(list)
# 行 3531：变量赋值：保存配置、状态或计算结果。
                number = 0
# 行 3532：for 循环遍历序列或可迭代对象。
                for i in range(len(self.c_file_abf_list)):
# 行 3533：变量赋值：保存配置、状态或计算结果。
                    abf = pyabf.ABF(self.c_file_abf_list[i])
# 行 3534：变量赋值：保存配置、状态或计算结果。
                    self.c1_x = abf.sweepX
# 行 3535：变量赋值：保存配置、状态或计算结果。
                    self.c1_y = abf.sweepY
# 行 3536：变量赋值：保存配置、状态或计算结果。
                    self.c_frequency = abf.dataRate
# 行 3537：条件判断，根据不同条件执行不同分支。
                    if self.c_filter == True:
# 行 3538：变量赋值：保存配置、状态或计算结果。
                        cutoff_frequency = int(self.c_filter_frequency_param.get())  # 截止频率
# 行 3539：变量赋值：保存配置、状态或计算结果。
                        nyquist_frequency = 0.5 * self.c_frequency  # 奈奎斯特频率
# 行 3540：变量赋值：保存配置、状态或计算结果。
                        normal_cutoff = cutoff_frequency / nyquist_frequency
# 行 3541：普通语句：执行一步操作或表达式。
                        b, a = signal.butter(1, normal_cutoff, btype='low', analog=False)
# 行 3542：空行，用于分隔代码块，提高可读性。

# 行 3543：原始注释行，提供作者或用途说明。
                        # normal_cutoff = [100 / nyquist_frequency, cutoff_frequency / nyquist_frequency]
# 行 3544：原始注释行，提供作者或用途说明。
                        # b, a = signal.butter(1, normal_cutoff, btype='band', analog=False)
# 行 3545：空行，用于分隔代码块，提高可读性。

# 行 3546：变量赋值：保存配置、状态或计算结果。
                        self.c1_y_filter = signal.filtfilt(b, a, self.c1_y)
# 行 3547：空行，用于分隔代码块，提高可读性。

# 行 3548：变量赋值：保存配置、状态或计算结果。
                        self.c_baseline = dynamic_detection.calculate_dynamic_baseline_fast_trimmed_py(self.c1_y_filter,
# 行 3549：普通语句：执行一步操作或表达式。
                                                                                                       (float(
# 行 3550：普通语句：执行一步操作或表达式。
                                                                                                           self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000)
# 行 3551：普通语句：执行一步操作或表达式。
                        self.c_std_down, self.c_std_up = np.array(
# 行 3552：普通语句：执行一步操作或表达式。
                            dynamic_detection.detection_y_py(self.c1_y_filter, (
# 行 3553：普通语句：执行一步操作或表达式。
                                    float(self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000,
# 行 3554：普通语句：执行一步操作或表达式。
                                                             float(self.c_detection_sigma)))
# 行 3555：普通语句：执行一步操作或表达式。
                        self.c_event_list_while, self.c_event_list_number = self.event_detection(self.c1_x, self.c1_y_filter,
# 行 3556：普通语句：执行一步操作或表达式。
                                                                                           self.c_baseline,
# 行 3557：普通语句：执行一步操作或表达式。
                                                                                           self.c_std_up,
# 行 3558：普通语句：执行一步操作或表达式。
                                                                                           self.c_std_down,
# 行 3559：普通语句：执行一步操作或表达式。
                                                                                           self.c_detection_three,
# 行 3560：普通语句：执行一步操作或表达式。
                                                                                           self.c_frequency, 1, 0,
# 行 3561：普通语句：执行一步操作或表达式。
                                                                                           self.c_detection_mintime,
# 行 3562：普通语句：执行一步操作或表达式。
                                                                                           self.c_detection_maxtime, 0)
# 行 3563：条件判断，根据不同条件执行不同分支。
                    else:
# 行 3564：空行，用于分隔代码块，提高可读性。

# 行 3565：变量赋值：保存配置、状态或计算结果。
                        self.c_baseline = dynamic_detection.calculate_dynamic_baseline_fast_trimmed_py(self.c1_y,
# 行 3566：普通语句：执行一步操作或表达式。
                                                                                                       (float(
# 行 3567：普通语句：执行一步操作或表达式。
                                                                                                           self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000)
# 行 3568：普通语句：执行一步操作或表达式。
                        self.c_std_down, self.c_std_up = np.array(
# 行 3569：普通语句：执行一步操作或表达式。
                            dynamic_detection.detection_y_py(self.c1_y, (
# 行 3570：普通语句：执行一步操作或表达式。
                                    float(self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000,
# 行 3571：普通语句：执行一步操作或表达式。
                                                             float(self.c_detection_sigma)))
# 行 3572：普通语句：执行一步操作或表达式。
                        self.c_event_list_while, self.c_event_list_number = self.event_detection(self.c1_x, self.c1_y,
# 行 3573：普通语句：执行一步操作或表达式。
                                                                                           self.c_baseline,
# 行 3574：普通语句：执行一步操作或表达式。
                                                                                           self.c_std_up,
# 行 3575：普通语句：执行一步操作或表达式。
                                                                                           self.c_std_down,
# 行 3576：普通语句：执行一步操作或表达式。
                                                                                           self.c_detection_three,
# 行 3577：普通语句：执行一步操作或表达式。
                                                                                           self.c_frequency, 1, 0,
# 行 3578：普通语句：执行一步操作或表达式。
                                                                                           self.c_detection_mintime,
# 行 3579：普通语句：执行一步操作或表达式。
                                                                                           self.c_detection_maxtime, 0)
# 行 3580：空行，用于分隔代码块，提高可读性。

# 行 3581：for 循环遍历序列或可迭代对象。
                    for j in range(len(self.c_event_list_while)):
# 行 3582：变量赋值：保存配置、状态或计算结果。
                        list_while = self.c_event_list_while[j]
# 行 3583：普通语句：执行一步操作或表达式。
                        self.c_event_list[number] += list_while
# 行 3584：普通语句：执行一步操作或表达式。
                        number += 1
# 行 3585：for 循环遍历序列或可迭代对象。
                for k in range(len(self.c_event_list)):
# 行 3586：变量赋值：保存配置、状态或计算结果。
                    list1 = self.c_event_list[k]
# 行 3587：普通语句：执行一步操作或表达式。
                    self.c_baseline_list.append(float(list1[1]))
# 行 3588：普通语句：执行一步操作或表达式。
                    self.c_amplitude_list.append(float(list1[2]))
# 行 3589：普通语句：执行一步操作或表达式。
                    self.c_integral_list.append(float(list1[3]))
# 行 3590：普通语句：执行一步操作或表达式。
                    self.c_duration_list.append(float(list1[4]))
# 行 3591：普通语句：执行一步操作或表达式。
                    self.c_bluntness_list.append(float(list1[5]))
# 行 3592：普通语句：执行一步操作或表达式。
                    self.c_asymmetry_list.append(float(list1[6]))
# 行 3593：普通语句：执行一步操作或表达式。
                    self.c_x_inertia_list.append(float(list1[7]))
# 行 3594：普通语句：执行一步操作或表达式。
                    self.c_y_inertia_list.append(float(list1[8]))
# 行 3595：普通语句：执行一步操作或表达式。
                tree_insert(self.c_event_list)
# 行 3596：空行，用于分隔代码块，提高可读性。

# 行 3597：普通语句：执行一步操作或表达式。
                self.c_update_plot_2D(self.c_duration_list, self.c_amplitude_list)
# 行 3598：定义函数/方法 open_new_window_save()。
        def open_new_window_save():
# 行 3599：变量赋值：保存配置、状态或计算结果。
            new_window = tk.Toplevel(self.root)
# 行 3600：普通语句：执行一步操作或表达式。
            new_window.title("数据保存")
# 行 3601：普通语句：执行一步操作或表达式。
            new_window.geometry("330x120")
# 行 3602：普通语句：执行一步操作或表达式。
            new_window.grab_set()
# 行 3603：普通语句：执行一步操作或表达式。
            create_label(new_window, '路径', 10, 20)
# 行 3604：普通语句：执行一步操作或表达式。
            create_label(new_window, '文件名', 10, 50)
# 行 3605：变量赋值：保存配置、状态或计算结果。
            path_entry_1 = create_entry(new_window, 30, 65, 20)
# 行 3606：变量赋值：保存配置、状态或计算结果。
            path_entry_2 = create_entry(new_window, 30, 65, 50)
# 行 3607：变量赋值：保存配置、状态或计算结果。
            browse_button = tk.Button(new_window, text="浏览", bg='#e5e5e5', font=('', 9),
# 行 3608：变量赋值：保存配置、状态或计算结果。
                                      command=lambda: browse_file(path_entry_1))
# 行 3609：普通语句：执行一步操作或表达式。
            browse_button.place(x=287, y=20)
# 行 3610：空行，用于分隔代码块，提高可读性。

# 行 3611：变量赋值：保存配置、状态或计算结果。
            confirm_button = tk.Button(new_window, text="确定", bg='#e5e5e5', font=('', 9),
# 行 3612：变量赋值：保存配置、状态或计算结果。
                                       command=lambda: self.c_on_confirm(new_window, path_entry_1.get(), path_entry_2.get()))
# 行 3613：普通语句：执行一步操作或表达式。
            confirm_button.place(x=140, y=80)
# 行 3614：空行，用于分隔代码块，提高可读性。

# 行 3615：空行，用于分隔代码块，提高可读性。

# 行 3616：变量赋值：保存配置、状态或计算结果。
        self.c_open_file = create_button(self.analyse_page_original, '打开文件/文件夹',
# 行 3617：普通语句：执行一步操作或表达式。
                                                  open_new_window,
# 行 3618：普通语句：执行一步操作或表达式。
                                                  13, 1, 10, 10)
# 行 3619：变量赋值：保存配置、状态或计算结果。
        self.c1_file_path_entry = create_entry(self.analyse_page_original, 95, 120, 12)
# 行 3620：普通语句：执行一步操作或表达式。
        self.c1_file_path_entry.config(state=tk.DISABLED)
# 行 3621：定义函数/方法 c_file_folder_choice_func()。
        def c_file_folder_choice_func():
# 行 3622：变量赋值：保存配置、状态或计算结果。
            self.c_file_folder_choice = not self.c_file_folder_choice
# 行 3623：打印调试或状态信息到控制台。
            print(self.c_file_folder_choice)
# 行 3624：变量赋值：保存配置、状态或计算结果。
        self.c_file_folder_choice_tk = tk.IntVar()
# 行 3625：变量赋值：保存配置、状态或计算结果。
        self.c_file_folder_choice_button = tk.Checkbutton(self.analyse_page_original,
# 行 3626：变量赋值：保存配置、状态或计算结果。
                                                        variable=self.c_file_folder_choice_tk,
# 行 3627：变量赋值：保存配置、状态或计算结果。
                                                        font=("Arial", 1), command=lambda: c_file_folder_choice_func())
# 行 3628：普通语句：执行一步操作或表达式。
        self.c_file_folder_choice_button.place(x=820, y=12)
# 行 3629：变量赋值：保存配置、状态或计算结果。
        self.c_file_folder_choice_look = create_label(self.analyse_page_original, '文件夹模式', 850, 12)
# 行 3630：空行，用于分隔代码块，提高可读性。

# 行 3631：空行，用于分隔代码块，提高可读性。

# 行 3632：变量赋值：保存配置、状态或计算结果。
        self.c_start_detection = create_button(self.analyse_page_original, '检测',
# 行 3633：普通语句：执行一步操作或表达式。
                                         c_start_detection_func,
# 行 3634：普通语句：执行一步操作或表达式。
                                         12, 1, 200, 40)
# 行 3635：空行，用于分隔代码块，提高可读性。

# 行 3636：变量赋值：保存配置、状态或计算结果。
        self.c_save_button = create_button(self.analyse_page_original, '保存至..',
# 行 3637：普通语句：执行一步操作或表达式。
                                               open_new_window_save,
# 行 3638：普通语句：执行一步操作或表达式。
                                               12, 1, 800, 40)
# 行 3639：变量赋值：保存配置、状态或计算结果。
        self.c_feature_extraction = create_label1(self.analyse_page_original, '过孔事件特征提取', 10, 50)
# 行 3640：空行，用于分隔代码块，提高可读性。

# 行 3641：变量赋值：保存配置、状态或计算结果。
        self.c_up_or_down = create_label(self.analyse_page_original, '事件类型:', 10, 80)
# 行 3642：定义函数/方法 on_select1(i, combo)。
        def on_select1(i, combo):
# 行 3643：条件判断，根据不同条件执行不同分支。
            if i == 0:
# 行 3644：变量赋值：保存配置、状态或计算结果。
                list = []
# 行 3645：变量赋值：保存配置、状态或计算结果。
                selected_item = self.b_combo.get()
# 行 3646：普通语句：执行一步操作或表达式。
                list.append(str(selected_item))
# 行 3647：返回函数结果。
                return list
# 行 3648：条件判断，根据不同条件执行不同分支。
            if i == 1:
# 行 3649：变量赋值：保存配置、状态或计算结果。
                selected_item = combo.get()
# 行 3650：条件判断，根据不同条件执行不同分支。
                if selected_item == '正向脉冲':
# 行 3651：变量赋值：保存配置、状态或计算结果。
                    self.c_detection_three = 1
# 行 3652：条件判断，根据不同条件执行不同分支。
                elif selected_item == '负向脉冲':
# 行 3653：变量赋值：保存配置、状态或计算结果。
                    self.c_detection_three = 2
# 行 3654：条件判断，根据不同条件执行不同分支。
                else:
# 行 3655：变量赋值：保存配置、状态或计算结果。
                    self.c_detection_three = 3
# 行 3656：空行，用于分隔代码块，提高可读性。

# 行 3657：变量赋值：保存配置、状态或计算结果。
        options1 = ['负向脉冲', '正向脉冲', '双向脉冲']
# 行 3658：变量赋值：保存配置、状态或计算结果。
        self.c_combo1 = ttk.Combobox(self.analyse_page_original, values=options1, width=10, state='readonly')
# 行 3659：普通语句：执行一步操作或表达式。
        self.c_combo1.current(0)
# 行 3660：普通语句：执行一步操作或表达式。
        self.c_combo1.bind("<<ComboboxSelected>>", lambda event: on_select1(1, self.c_combo1))
# 行 3661：普通语句：执行一步操作或表达式。
        self.c_combo1.place(x=90, y=80)
# 行 3662：空行，用于分隔代码块，提高可读性。

# 行 3663：变量赋值：保存配置、状态或计算结果。
        self.c_detection_sigma_label = create_label(self.analyse_page_original, '检测阈值（σ）:', 10, 110)
# 行 3664：变量赋值：保存配置、状态或计算结果。
        self.c_detection_sigma_param = create_entry1(self.analyse_page_original, 10, 1210 - 1070, 110, 5)
# 行 3665：空行，用于分隔代码块，提高可读性。

# 行 3666：定义函数/方法 c_detection_threshold_func()。
        def c_detection_threshold_func():
# 行 3667：变量赋值：保存配置、状态或计算结果。
            self.c_detection_threshold = not self.c_detection_threshold
# 行 3668：变量赋值：保存配置、状态或计算结果。
            self.c_detection_sigma = self.c_detection_sigma_param.get()
# 行 3669：条件判断，根据不同条件执行不同分支。
            if self.c_filter == True:
# 行 3670：变量赋值：保存配置、状态或计算结果。
                self.c_baseline = dynamic_detection.calculate_dynamic_baseline_fast_trimmed_py(self.c1_y_filter,
# 行 3671：普通语句：执行一步操作或表达式。
                                                                                               (float(
# 行 3672：普通语句：执行一步操作或表达式。
                                                                                                   self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000)
# 行 3673：普通语句：执行一步操作或表达式。
                self.c_std_down, self.c_std_up = np.array(
# 行 3674：普通语句：执行一步操作或表达式。
                    dynamic_detection.detection_y_py(self.c1_y_filter, (
# 行 3675：普通语句：执行一步操作或表达式。
                                float(self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000,
# 行 3676：普通语句：执行一步操作或表达式。
                                                     float(self.c_detection_sigma)))
# 行 3677：条件判断，根据不同条件执行不同分支。
            else:
# 行 3678：变量赋值：保存配置、状态或计算结果。
                self.c_baseline = dynamic_detection.calculate_dynamic_baseline_fast_trimmed_py(self.c1_y,
# 行 3679：普通语句：执行一步操作或表达式。
                                                                                               (float(
# 行 3680：普通语句：执行一步操作或表达式。
                                                                                                   self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000)
# 行 3681：普通语句：执行一步操作或表达式。
                self.c_std_down, self.c_std_up = np.array(
# 行 3682：普通语句：执行一步操作或表达式。
                    dynamic_detection.detection_y_py(self.c1_y, (
# 行 3683：普通语句：执行一步操作或表达式。
                            float(self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000,
# 行 3684：普通语句：执行一步操作或表达式。
                                                     float(self.c_detection_sigma)))
# 行 3685：普通语句：执行一步操作或表达式。
            self.update_c_plot_current(self.c1_x, self.c1_y)
# 行 3686：定义函数/方法 c_filter_func()。
        def c_filter_func():
# 行 3687：变量赋值：保存配置、状态或计算结果。
            self.c_filter = not self.c_filter
# 行 3688：变量赋值：保存配置、状态或计算结果。
            self.c_detection_sigma = self.c_detection_sigma_param.get()
# 行 3689：条件判断，根据不同条件执行不同分支。
            if self.c_file_folder_choice == False:
# 行 3690：条件判断，根据不同条件执行不同分支。
                if self.c_filter == True:
# 行 3691：变量赋值：保存配置、状态或计算结果。
                    cutoff_frequency = int(self.c_filter_frequency_param.get())  # 截止频率
# 行 3692：变量赋值：保存配置、状态或计算结果。
                    nyquist_frequency = 0.5 * self.c_frequency  # 奈奎斯特频率
# 行 3693：变量赋值：保存配置、状态或计算结果。
                    normal_cutoff = cutoff_frequency / nyquist_frequency
# 行 3694：普通语句：执行一步操作或表达式。
                    b, a = signal.butter(1, normal_cutoff, btype='low', analog=False)
# 行 3695：变量赋值：保存配置、状态或计算结果。
                    self.c1_y_filter = signal.filtfilt(b, a, self.c1_y)
# 行 3696：空行，用于分隔代码块，提高可读性。

# 行 3697：变量赋值：保存配置、状态或计算结果。
                    self.c_baseline = dynamic_detection.calculate_dynamic_baseline_fast_trimmed_py(self.c1_y_filter,
# 行 3698：普通语句：执行一步操作或表达式。
                                                                                                   (float(
# 行 3699：普通语句：执行一步操作或表达式。
                                                                                                       self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000)
# 行 3700：普通语句：执行一步操作或表达式。
                    self.c_std_down, self.c_std_up = np.array(
# 行 3701：普通语句：执行一步操作或表达式。
                        dynamic_detection.detection_y_py(self.c1_y_filter, (
# 行 3702：普通语句：执行一步操作或表达式。
                                float(self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000,
# 行 3703：普通语句：执行一步操作或表达式。
                                                         float(self.c_detection_sigma)))
# 行 3704：普通语句：执行一步操作或表达式。
                    self.update_c_plot_current(self.c1_x, self.c1_y_filter)
# 行 3705：条件判断，根据不同条件执行不同分支。
                else:
# 行 3706：变量赋值：保存配置、状态或计算结果。
                    self.c_baseline = dynamic_detection.calculate_dynamic_baseline_fast_trimmed_py(self.c1_y,
# 行 3707：普通语句：执行一步操作或表达式。
                                                                                                   (float(
# 行 3708：普通语句：执行一步操作或表达式。
                                                                                                       self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000)
# 行 3709：普通语句：执行一步操作或表达式。
                    self.c_std_down, self.c_std_up = np.array(
# 行 3710：普通语句：执行一步操作或表达式。
                        dynamic_detection.detection_y_py(self.c1_y, (
# 行 3711：普通语句：执行一步操作或表达式。
                                float(self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000,
# 行 3712：普通语句：执行一步操作或表达式。
                                                         float(self.c_detection_sigma)))
# 行 3713：普通语句：执行一步操作或表达式。
                    self.update_c_plot_current(self.c1_x, self.c1_y)
# 行 3714：定义函数/方法 c_event_func()。
        def c_event_func():
# 行 3715：变量赋值：保存配置、状态或计算结果。
            self.c_event = not self.c_event
# 行 3716：条件判断，根据不同条件执行不同分支。
            if self.c_event == True:
# 行 3717：普通语句：执行一步操作或表达式。
                self.update_c_plot_current(self.c1_x, self.c1_y)
# 行 3718：条件判断，根据不同条件执行不同分支。
            else:
# 行 3719：普通语句：执行一步操作或表达式。
                self.update_c_plot_current(self.c1_x, self.c1_y)
# 行 3720：变量赋值：保存配置、状态或计算结果。
        self.c1_var_event = tk.IntVar()
# 行 3721：变量赋值：保存配置、状态或计算结果。
        self.c1_detection_event = tk.Checkbutton(self.analyse_page_original,
# 行 3722：变量赋值：保存配置、状态或计算结果。
                                                       variable=self.c1_var_event,
# 行 3723：变量赋值：保存配置、状态或计算结果。
                                                       font=("Arial", 1), command=lambda: c_event_func())
# 行 3724：普通语句：执行一步操作或表达式。
        self.c1_detection_event.place(x=10, y=140)
# 行 3725：变量赋值：保存配置、状态或计算结果。
        self.c1_detection_sigma_look = create_label(self.analyse_page_original, '事件', 40, 140)
# 行 3726：空行，用于分隔代码块，提高可读性。

# 行 3727：变量赋值：保存配置、状态或计算结果。
        self.c2_var_detection_sigma = tk.IntVar()
# 行 3728：变量赋值：保存配置、状态或计算结果。
        self.c2_detection_sigma_choice = tk.Checkbutton(self.analyse_page_original,
# 行 3729：变量赋值：保存配置、状态或计算结果。
                                                       variable=self.c2_var_detection_sigma,
# 行 3730：变量赋值：保存配置、状态或计算结果。
                                                       font=("Arial", 1), command=lambda: c_detection_threshold_func())
# 行 3731：普通语句：执行一步操作或表达式。
        self.c2_detection_sigma_choice.place(x=90, y=140)
# 行 3732：变量赋值：保存配置、状态或计算结果。
        self.c2_detection_sigma_look = create_label(self.analyse_page_original, '阈值', 120, 140)
# 行 3733：空行，用于分隔代码块，提高可读性。

# 行 3734：变量赋值：保存配置、状态或计算结果。
        self.c2_var_filter = tk.IntVar()
# 行 3735：变量赋值：保存配置、状态或计算结果。
        self.c2_var_filter_choice = tk.Checkbutton(self.analyse_page_original,
# 行 3736：变量赋值：保存配置、状态或计算结果。
                                                        variable=self.c2_var_filter,
# 行 3737：变量赋值：保存配置、状态或计算结果。
                                                        font=("Arial", 1), command=lambda: c_filter_func())
# 行 3738：普通语句：执行一步操作或表达式。
        self.c2_var_filter_choice.place(x=330, y=45)
# 行 3739：变量赋值：保存配置、状态或计算结果。
        self.c2_var_filter_look = create_label(self.analyse_page_original, '滤波(截止频率)', 360, 45)
# 行 3740：变量赋值：保存配置、状态或计算结果。
        self.c_filter_frequency_param = create_entry1(self.analyse_page_original, 10, 480, 45, 10000)
# 行 3741：空行，用于分隔代码块，提高可读性。

# 行 3742：空行，用于分隔代码块，提高可读性。

# 行 3743：变量赋值：保存配置、状态或计算结果。
        self.c_detection_mintime_label = create_label(self.analyse_page_original, '最小持续时间:', 240, 80)
# 行 3744：变量赋值：保存配置、状态或计算结果。
        self.c_detection_mintime_param = create_entry1(self.analyse_page_original, 10, 360, 80, 0)
# 行 3745：变量赋值：保存配置、状态或计算结果。
        self.c_detection_mintime_ms = create_label(self.analyse_page_original, 'ms', 440, 80)
# 行 3746：空行，用于分隔代码块，提高可读性。

# 行 3747：变量赋值：保存配置、状态或计算结果。
        self.c_detection_maxtime_label = create_label(self.analyse_page_original, '最大持续时间:', 240, 110)
# 行 3748：变量赋值：保存配置、状态或计算结果。
        self.c_detection_maxtime_param = create_entry1(self.analyse_page_original, 10, 360, 110, 1000)
# 行 3749：变量赋值：保存配置、状态或计算结果。
        self.c_detection_maxtime_ms = create_label(self.analyse_page_original, 'ms', 440, 110)
# 行 3750：空行，用于分隔代码块，提高可读性。

# 行 3751：变量赋值：保存配置、状态或计算结果。
        self.c_detection_baseline_std_window_label = create_label(self.analyse_page_original, '基线/std计算步长:',
# 行 3752：普通语句：执行一步操作或表达式。
                                                            240, 140)
# 行 3753：变量赋值：保存配置、状态或计算结果。
        self.c_detection_baseline_std_window_param = create_entry1(self.analyse_page_original, 10, 390, 140,
# 行 3754：普通语句：执行一步操作或表达式。
                                                                   200)
# 行 3755：变量赋值：保存配置、状态或计算结果。
        self.c_detection_baseline_std_window_ms = create_label(self.analyse_page_original, 'ms', 470, 140)
# 行 3756：空行，用于分隔代码块，提高可读性。

# 行 3757：定义函数/方法 on_select(number, combo)。
        def on_select(number, combo):
# 行 3758：变量赋值：保存配置、状态或计算结果。
            selected_item = combo.get()
# 行 3759：条件判断，根据不同条件执行不同分支。
            if number == 1:
# 行 3760：条件判断，根据不同条件执行不同分支。
                if selected_item == '基线':
# 行 3761：普通语句：执行一步操作或表达式。
                    self.c_undate_plot_histogram(self.c_baseline_list)
# 行 3762：条件判断，根据不同条件执行不同分支。
                elif selected_item == '振幅':
# 行 3763：普通语句：执行一步操作或表达式。
                    self.c_undate_plot_histogram(self.c_amplitude_list)
# 行 3764：条件判断，根据不同条件执行不同分支。
                elif selected_item == '积分':
# 行 3765：普通语句：执行一步操作或表达式。
                    self.c_undate_plot_histogram(self.c_integral_list)
# 行 3766：条件判断，根据不同条件执行不同分支。
                elif selected_item == '持续时间':
# 行 3767：普通语句：执行一步操作或表达式。
                    self.c_undate_plot_histogram(self.c_duration_list)
# 行 3768：条件判断，根据不同条件执行不同分支。
                elif selected_item == '钝度':
# 行 3769：普通语句：执行一步操作或表达式。
                    self.c_undate_plot_histogram(self.c_bluntness_list)
# 行 3770：条件判断，根据不同条件执行不同分支。
                elif selected_item == '脉冲不对称':
# 行 3771：普通语句：执行一步操作或表达式。
                    self.c_undate_plot_histogram(self.c_asymmetry_list)
# 行 3772：条件判断，根据不同条件执行不同分支。
                elif selected_item == '横轴惯性':
# 行 3773：普通语句：执行一步操作或表达式。
                    self.c_undate_plot_histogram(self.c_x_inertia_list)
# 行 3774：条件判断，根据不同条件执行不同分支。
                elif selected_item == '纵轴惯性':
# 行 3775：普通语句：执行一步操作或表达式。
                    self.c_undate_plot_histogram(self.c_y_inertia_list)
# 行 3776：条件判断，根据不同条件执行不同分支。
            elif number == 2:
# 行 3777：条件判断，根据不同条件执行不同分支。
                if selected_item == '基线':
# 行 3778：普通语句：执行一步操作或表达式。
                    self.c_undate_plot_histogram_2(self.c_events_list_baseline_2)
# 行 3779：条件判断，根据不同条件执行不同分支。
                elif selected_item == '振幅':
# 行 3780：普通语句：执行一步操作或表达式。
                    self.c_undate_plot_histogram_2(self.c_events_list_amplitude_2)
# 行 3781：条件判断，根据不同条件执行不同分支。
                elif selected_item == '积分':
# 行 3782：普通语句：执行一步操作或表达式。
                    self.c_undate_plot_histogram_2(self.c_events_list_integral_2)
# 行 3783：条件判断，根据不同条件执行不同分支。
                elif selected_item == '持续时间':
# 行 3784：普通语句：执行一步操作或表达式。
                    self.c_undate_plot_histogram_2(self.c_events_list_duration_2)
# 行 3785：条件判断，根据不同条件执行不同分支。
                elif selected_item == '钝度':
# 行 3786：普通语句：执行一步操作或表达式。
                    self.c_undate_plot_histogram_2(self.c_events_list_bluntness_2)
# 行 3787：条件判断，根据不同条件执行不同分支。
                elif selected_item == '脉冲不对称':
# 行 3788：普通语句：执行一步操作或表达式。
                    self.c_undate_plot_histogram_2(self.c_events_list_asymmetry_2)
# 行 3789：条件判断，根据不同条件执行不同分支。
                elif selected_item == '横轴惯性':
# 行 3790：普通语句：执行一步操作或表达式。
                    self.c_undate_plot_histogram_2(self.c_events_list_x_inertia_2)
# 行 3791：条件判断，根据不同条件执行不同分支。
                elif selected_item == '纵轴惯性':
# 行 3792：普通语句：执行一步操作或表达式。
                    self.c_undate_plot_histogram_2(self.c_events_list_y_inertia_2)
# 行 3793：变量赋值：保存配置、状态或计算结果。
        options1 = ['基线', '振幅', '积分', '持续时间', '钝度', '脉冲不对称', '横轴惯性', '纵轴惯性']
# 行 3794：空行，用于分隔代码块，提高可读性。

# 行 3795：变量赋值：保存配置、状态或计算结果。
        self.c_combo2 = ttk.Combobox(self.analyse_page_original, values=options1, width=10, state='readonly')
# 行 3796：普通语句：执行一步操作或表达式。
        self.c_combo2.current(0)
# 行 3797：普通语句：执行一步操作或表达式。
        self.c_combo2.bind("<<ComboboxSelected>>", lambda event: on_select(1, self.c_combo2))
# 行 3798：普通语句：执行一步操作或表达式。
        self.c_combo2.place(x=800, y=660)
# 行 3799：空行，用于分隔代码块，提高可读性。

# 行 3800：普通语句：执行一步操作或表达式。
        self.c_plot_current()
# 行 3801：变量赋值：保存配置、状态或计算结果。
        self.toolbar_current = NavigationToolbar2Tk(self.canvas_current_c, self.analyse_page_original)
# 行 3802：普通语句：执行一步操作或表达式。
        self.toolbar_current.update()
# 行 3803：普通语句：执行一步操作或表达式。
        self.toolbar_current.place(x=500, y=100)
# 行 3804：普通语句：执行一步操作或表达式。
        self.c_show_table_tk()
# 行 3805：普通语句：执行一步操作或表达式。
        self.c_plot_event1()
# 行 3806：普通语句：执行一步操作或表达式。
        self.c_plot_2d()
# 行 3807：普通语句：执行一步操作或表达式。
        self.c_plot_histogram()
# 行 3808：普通语句：执行一步操作或表达式。
        self.c_plot_2d_2()
# 行 3809：普通语句：执行一步操作或表达式。
        self.c_plot_histogram_2_2()
# 行 3810：空行，用于分隔代码块，提高可读性。

# 行 3811：定义函数/方法 open_new_window_2()。
        def open_new_window_2():
# 行 3812：变量赋值：保存配置、状态或计算结果。
            new_window = tk.Toplevel(self.root)
# 行 3813：普通语句：执行一步操作或表达式。
            new_window.title("打开文件")
# 行 3814：普通语句：执行一步操作或表达式。
            new_window.geometry("330x120")
# 行 3815：普通语句：执行一步操作或表达式。
            new_window.grab_set()
# 行 3816：空行，用于分隔代码块，提高可读性。

# 行 3817：变量赋值：保存配置、状态或计算结果。
            path_entry = create_entry(new_window, 30, 69, 30)
# 行 3818：普通语句：执行一步操作或表达式。
            create_label(new_window, "文件信息", 3, 30)
# 行 3819：空行，用于分隔代码块，提高可读性。

# 行 3820：变量赋值：保存配置、状态或计算结果。
            browse_button = tk.Button(new_window, text="浏览", bg='#e5e5e5', font=('', 9),
# 行 3821：变量赋值：保存配置、状态或计算结果。
                                      command=lambda: browse_file1(path_entry,
# 行 3822：普通语句：执行一步操作或表达式。
                                                                  [("ABF files", "*.csv")]))
# 行 3823：空行，用于分隔代码块，提高可读性。

# 行 3824：普通语句：执行一步操作或表达式。
            browse_button.place(x=287, y=30)
# 行 3825：空行，用于分隔代码块，提高可读性。

# 行 3826：变量赋值：保存配置、状态或计算结果。
            confirm_button = tk.Button(new_window, text="确定", bg='#e5e5e5', font=('', 9),
# 行 3827：变量赋值：保存配置、状态或计算结果。
                                       command=lambda: on_confirm_2(new_window, path_entry.get()))
# 行 3828：普通语句：执行一步操作或表达式。
            confirm_button.place(x=140, y=80)
# 行 3829：定义函数/方法 on_confirm_2(window, folder_path)。
        def on_confirm_2(window, folder_path):
# 行 3830：条件判断，根据不同条件执行不同分支。
            if folder_path:
# 行 3831：变量赋值：保存配置、状态或计算结果。
                self.c_preview_file_name = folder_path
# 行 3832：条件判断，根据不同条件执行不同分支。
                if folder_path not in self.c_file_path_list_preview:
# 行 3833：普通语句：执行一步操作或表达式。
                    add_file_name(folder_path)
# 行 3834：普通语句：执行一步操作或表达式。
                    self.c_file_path_list_preview.append(folder_path)
# 行 3835：空行，用于分隔代码块，提高可读性。

# 行 3836：占位语句：暂不执行任何操作。
                pass
# 行 3837：条件判断，根据不同条件执行不同分支。
            else:
# 行 3838：普通语句：执行一步操作或表达式。
                messagebox.showwarning("警告", "请传入有效路径！")
# 行 3839：普通语句：执行一步操作或表达式。
            window.destroy()
# 行 3840：空行，用于分隔代码块，提高可读性。

# 行 3841：定义函数/方法 add_file_name(filename)。
        def add_file_name(filename):
# 行 3842：普通语句：执行一步操作或表达式。
            self.c_text_display.config(state=tk.NORMAL)
# 行 3843：变量赋值：保存配置、状态或计算结果。
            line_index = self.c_text_display.index(tk.END)
# 行 3844：普通语句：执行一步操作或表达式。
            self.c_text_display.insert(tk.END, f'{filename}\n')
# 行 3845：普通语句：执行一步操作或表达式。
            self.c_text_display.yview(tk.END)
# 行 3846：普通语句：执行一步操作或表达式。
            self.c_text_display.config(state=tk.DISABLED)
# 行 3847：空行，用于分隔代码块，提高可读性。

# 行 3848：定义函数/方法 c_add_statistics_text_func(window, category, color)。
        def c_add_statistics_text_func(window, category, color):
# 行 3849：变量赋值：保存配置、状态或计算结果。
            file_exists = any(
# 行 3850：普通语句：执行一步操作或表达式。
                item[0] == self.c_preview_file_name
# 行 3851：普通语句：执行一步操作或表达式。
                for item in self.c_file_path_list_statistics
# 行 3852：普通语句：执行一步操作或表达式。
            )
# 行 3853：条件判断，根据不同条件执行不同分支。
            if not file_exists:
# 行 3854：普通语句：执行一步操作或表达式。
                self.c_file_path_list_statistics.append([self.c_preview_file_name, category, color])
# 行 3855：普通语句：执行一步操作或表达式。
                self.c_text_display_2.config(state=tk.NORMAL)
# 行 3856：变量赋值：保存配置、状态或计算结果。
                line_index = self.c_text_display.index(tk.END)
# 行 3857：普通语句：执行一步操作或表达式。
                self.c_text_display_2.insert(tk.END, f'{self.c_preview_file_name, category, color }\n')
# 行 3858：普通语句：执行一步操作或表达式。
                self.c_text_display_2.yview(tk.END)
# 行 3859：普通语句：执行一步操作或表达式。
                self.c_text_display_2.config(state=tk.DISABLED)
# 行 3860：普通语句：执行一步操作或表达式。
            window.destroy()
# 行 3861：空行，用于分隔代码块，提高可读性。

# 行 3862：定义函数/方法 c_delete_selected_item()。
        def c_delete_selected_item():
# 行 3863：空行，用于分隔代码块，提高可读性。

# 行 3864：try 块：尝试执行，捕获潜在异常。
            try:
# 行 3865：普通语句：执行一步操作或表达式。
                self.c_text_display_2.config(state=tk.NORMAL)
# 行 3866：空行，用于分隔代码块，提高可读性。

# 行 3867：变量赋值：保存配置、状态或计算结果。
                next_line = int(self.c_line_number) + 1
# 行 3868：普通语句：执行一步操作或表达式。
                self.c_text_display_2.delete(f"{self.c_line_number}.0", f"{next_line}.0")
# 行 3869：普通语句：执行一步操作或表达式。
                self.c_text_display_2.config(state=tk.DISABLED)
# 行 3870：空行，用于分隔代码块，提高可读性。

# 行 3871：条件判断，根据不同条件执行不同分支。
                if hasattr(self, 'c_file_path_list_statistics') and hasattr(self, 'c_statistics_file_name'):
# 行 3872：变量赋值：保存配置、状态或计算结果。
                    file_name = self.c_statistics_file_name[0] if self.c_statistics_file_name else None
# 行 3873：条件判断，根据不同条件执行不同分支。
                    if file_name:
# 行 3874：for 循环遍历序列或可迭代对象。
                        for i, item in enumerate(self.c_file_path_list_statistics):
# 行 3875：条件判断，根据不同条件执行不同分支。
                            if item[0] == file_name:
# 行 3876：普通语句：执行一步操作或表达式。
                                del self.c_file_path_list_statistics[i]
# 行 3877：打印调试或状态信息到控制台。
                                print(f"已从数据列表中删除: {file_name}")
# 行 3878：普通语句：执行一步操作或表达式。
                                break
# 行 3879：空行，用于分隔代码块，提高可读性。

# 行 3880：普通语句：执行一步操作或表达式。
                self.c_text_display_2.tag_remove("highlight", "1.0", "end")
# 行 3881：变量赋值：保存配置、状态或计算结果。
                self.c_line_number = None
# 行 3882：变量赋值：保存配置、状态或计算结果。
                self.c_statistics_file_name = None
# 行 3883：打印调试或状态信息到控制台。
                print(self.c_file_path_list_statistics)
# 行 3884：except 块：处理异常情况，防止程序崩溃。
            except Exception as e:
# 行 3885：打印调试或状态信息到控制台。
                print(f"删除失败: {str(e)}")
# 行 3886：空行，用于分隔代码块，提高可读性。

# 行 3887：空行，用于分隔代码块，提高可读性。

# 行 3888：变量赋值：保存配置、状态或计算结果。
        self.c_open_file_2 = create_button(self.analyse_page_original, '打开文件',
# 行 3889：普通语句：执行一步操作或表达式。
                                         open_new_window_2,
# 行 3890：普通语句：执行一步操作或表达式。
                                         12, 1, 960, 10)
# 行 3891：定义函数/方法 open_new_window_category_color()。
        def open_new_window_category_color():
# 行 3892：变量赋值：保存配置、状态或计算结果。
            new_window = tk.Toplevel(self.root)
# 行 3893：普通语句：执行一步操作或表达式。
            new_window.title("类目")
# 行 3894：普通语句：执行一步操作或表达式。
            new_window.geometry("330x120")
# 行 3895：普通语句：执行一步操作或表达式。
            new_window.grab_set()
# 行 3896：空行，用于分隔代码块，提高可读性。

# 行 3897：变量赋值：保存配置、状态或计算结果。
            path_entry = create_entry(new_window, 10, 69, 30)
# 行 3898：普通语句：执行一步操作或表达式。
            create_label(new_window, "类目", 3, 30)
# 行 3899：空行，用于分隔代码块，提高可读性。

# 行 3900：变量赋值：保存配置、状态或计算结果。
            path_entry1 = create_entry(new_window, 10, 69, 50)
# 行 3901：普通语句：执行一步操作或表达式。
            create_label1(new_window, "颜色", 3, 50)
# 行 3902：空行，用于分隔代码块，提高可读性。

# 行 3903：空行，用于分隔代码块，提高可读性。

# 行 3904：空行，用于分隔代码块，提高可读性。

# 行 3905：变量赋值：保存配置、状态或计算结果。
            confirm_button = tk.Button(new_window, text="确定", bg='#e5e5e5', font=('', 9),
# 行 3906：变量赋值：保存配置、状态或计算结果。
                                       command=lambda: c_add_statistics_text_func(new_window, path_entry.get(), path_entry1.get()))
# 行 3907：普通语句：执行一步操作或表达式。
            confirm_button.place(x=140, y=80)
# 行 3908：空行，用于分隔代码块，提高可读性。

# 行 3909：普通语句：执行一步操作或表达式。
        self.c_text_display_func()
# 行 3910：变量赋值：保存配置、状态或计算结果。
        self.c_combo2 = ttk.Combobox(self.analyse_page_original, values=options1, width=10, state='readonly')
# 行 3911：普通语句：执行一步操作或表达式。
        self.c_combo2.current(0)
# 行 3912：普通语句：执行一步操作或表达式。
        self.c_combo2.bind("<<ComboboxSelected>>", lambda event: on_select(2, self.c_combo2))
# 行 3913：普通语句：执行一步操作或表达式。
        self.c_combo2.place(x=1750, y=350)
# 行 3914：变量赋值：保存配置、状态或计算结果。
        self.c_add_file_path = create_label(self.analyse_page_original, '↓↓↓↓↓', 1000, 480)
# 行 3915：变量赋值：保存配置、状态或计算结果。
        self.c_add_file_2 = create_button(self.analyse_page_original, '添加',
# 行 3916：普通语句：执行一步操作或表达式。
                                           open_new_window_category_color,
# 行 3917：普通语句：执行一步操作或表达式。
                                           12, 1, 1100, 470)
# 行 3918：普通语句：执行一步操作或表达式。
        self.c_text_display_func_2()
# 行 3919：变量赋值：保存配置、状态或计算结果。
        self.c_delete_file_2 = create_button(self.analyse_page_original, '删除',
# 行 3920：普通语句：执行一步操作或表达式。
                                          c_delete_selected_item,
# 行 3921：普通语句：执行一步操作或表达式。
                                          12, 1, 1300, 470)
# 行 3922：普通语句：执行一步操作或表达式。
        self.c_plot_2d_3()
# 行 3923：定义函数/方法 update_2d_3()。
        def update_2d_3():
# 行 3924：普通语句：执行一步操作或表达式。
            self.ax_c_2d_3.cla()
# 行 3925：变量赋值：保存配置、状态或计算结果。
            self.df_dict = {}
# 行 3926：for 循环遍历序列或可迭代对象。
            for item in self.c_file_path_list_statistics:
# 行 3927：普通语句：执行一步操作或表达式。
                file_path, category, color = item
# 行 3928：变量赋值：保存配置、状态或计算结果。
                df = pd.read_csv(file_path, encoding='gbk')
# 行 3929：普通语句：执行一步操作或表达式。
                duration, amplitude = df['持续时间(ms)'], df['振幅(nA)']
# 行 3930：普通语句：执行一步操作或表达式。
                self.ax_c_2d_3.scatter(duration, amplitude, c=f'{color}', label=f'{category}', alpha=0.4, s=10)
# 行 3931：原始注释行，提供作者或用途说明。
            # self.ax3.grid(True)
# 行 3932：普通语句：执行一步操作或表达式。
            self.ax_c_2d_3.set_xlabel("时间(ms)")
# 行 3933：普通语句：执行一步操作或表达式。
            self.ax_c_2d_3.set_ylabel("振幅(nA)")
# 行 3934：空行，用于分隔代码块，提高可读性。

# 行 3935：原始注释行，提供作者或用途说明。
            # self.ax3.set_xticks([])
# 行 3936：原始注释行，提供作者或用途说明。
            # self.ax3.set_yticks([])
# 行 3937：普通语句：执行一步操作或表达式。
            self.ax_c_2d_3.legend(fontsize=10)
# 行 3938：普通语句：执行一步操作或表达式。
            self.canvas_event_c_2d_3.draw()
# 行 3939：变量赋值：保存配置、状态或计算结果。
        self.c_update_2d_3 = create_button(self.analyse_page_original, '更新',
# 行 3940：普通语句：执行一步操作或表达式。
                                             update_2d_3,
# 行 3941：普通语句：执行一步操作或表达式。
                                             12, 1, 1500, 900)
# 行 3942：定义函数/方法 function1(self, command)。
    def function1(self, command):
# 行 3943：条件判断，根据不同条件执行不同分支。
        if command == "CBD打孔":
# 行 3944：条件判断，根据不同条件执行不同分支。
            if self.page_show_number != 1 and self.control_page_original == None:
# 行 3945：普通语句：执行一步操作或表达式。
                self.add_control_page()
# 行 3946：变量赋值：保存配置、状态或计算结果。
                self.page_show_number = 1
# 行 3947：条件判断，根据不同条件执行不同分支。
            else:
# 行 3948：普通语句：执行一步操作或表达式。
                self.control_page_original.place(x=0, y=0)
# 行 3949：普通语句：执行一步操作或表达式。
                self.collection_eONE_page_original.place_forget()
# 行 3950：普通语句：执行一步操作或表达式。
                self.analyse_page_original.place_forget()
# 行 3951：空行，用于分隔代码块，提高可读性。

# 行 3952：条件判断，根据不同条件执行不同分支。
        if command == '数据采集':
# 行 3953：条件判断，根据不同条件执行不同分支。
            if self.page_show_number != 2 and self.collection_eONE_page_original == None:
# 行 3954：普通语句：执行一步操作或表达式。
                self.add_collection_eONE_page()
# 行 3955：变量赋值：保存配置、状态或计算结果。
                self.page_show_number = 2
# 行 3956：条件判断，根据不同条件执行不同分支。
            else:
# 行 3957：普通语句：执行一步操作或表达式。
                self.collection_eONE_page_original.place(x=0, y=0)
# 行 3958：普通语句：执行一步操作或表达式。
                self.control_page_original.place_forget()
# 行 3959：普通语句：执行一步操作或表达式。
                self.analyse_page_original.place_forget()
# 行 3960：条件判断，根据不同条件执行不同分支。
        if command == '数据分析':
# 行 3961：条件判断，根据不同条件执行不同分支。
            if self.page_show_number != 3 and self.analyse_page_original == None:
# 行 3962：普通语句：执行一步操作或表达式。
                self.add_analyse_page()
# 行 3963：变量赋值：保存配置、状态或计算结果。
                self.page_show_number = 3
# 行 3964：条件判断，根据不同条件执行不同分支。
            else:
# 行 3965：普通语句：执行一步操作或表达式。
                self.analyse_page_original.place(x=0, y=0)
# 行 3966：普通语句：执行一步操作或表达式。
                self.control_page_original.place_forget()
# 行 3967：普通语句：执行一步操作或表达式。
                self.collection_eONE_page_original.place_forget()
# 行 3968：空行，用于分隔代码块，提高可读性。

# 行 3969：空行，用于分隔代码块，提高可读性。

# 行 3970：定义函数/方法 create_label(window, text, x, y)。
def create_label(window, text, x, y):
# 行 3971：变量赋值：保存配置、状态或计算结果。
    label = tk.Label(window, text=text, bg='#e5e5e5', font=("SimHei", 11))
# 行 3972：普通语句：执行一步操作或表达式。
    label.place(x=x, y=y)
# 行 3973：返回函数结果。
    return label
# 行 3974：空行，用于分隔代码块，提高可读性。

# 行 3975：定义函数/方法 create_label1(window, text, x, y)。
def create_label1(window, text, x, y):
# 行 3976：变量赋值：保存配置、状态或计算结果。
    label = tk.Label(window, text=text, bg='#e5e5e5', font=("SimHei", 13))
# 行 3977：普通语句：执行一步操作或表达式。
    label.place(x=x, y=y)
# 行 3978：返回函数结果。
    return label
# 行 3979：定义函数/方法 create_label2(window, text, x, y)。
def create_label2(window, text, x, y):
# 行 3980：变量赋值：保存配置、状态或计算结果。
    label = tk.Label(window, text=text, bg='#e5e5e5', font=("SimHei", 30), fg='black')
# 行 3981：普通语句：执行一步操作或表达式。
    label.place(x=x, y=y)
# 行 3982：返回函数结果。
    return label
# 行 3983：空行，用于分隔代码块，提高可读性。

# 行 3984：定义函数/方法 create_entry(window, width, x, y)。
def create_entry(window, width, x, y):
# 行 3985：变量赋值：保存配置、状态或计算结果。
    entry = tk.Entry(window, width=width)
# 行 3986：普通语句：执行一步操作或表达式。
    entry.place(x=x, y=y)
# 行 3987：返回函数结果。
    return entry
# 行 3988：定义函数/方法 create_entry1(window, width, x, y, default_value="")。
def create_entry1(window, width, x, y, default_value=""):
# 行 3989：变量赋值：保存配置、状态或计算结果。
    entry = tk.Entry(window, width=width, justify='center')
# 行 3990：普通语句：执行一步操作或表达式。
    entry.place(x=x, y=y)
# 行 3991：普通语句：执行一步操作或表达式。
    entry.insert(0, default_value)
# 行 3992：返回函数结果。
    return entry
# 行 3993：定义函数/方法 create_entry2(window, width, x, y, default_value="")。
def create_entry2(window, width, x, y, default_value=""):
# 行 3994：变量赋值：保存配置、状态或计算结果。
    entry = tk.Entry(window, width=width, justify='center')
# 行 3995：普通语句：执行一步操作或表达式。
    entry.place(x=x, y=y)
# 行 3996：普通语句：执行一步操作或表达式。
    entry.insert(0, default_value)
# 行 3997：返回函数结果。
    return entry
# 行 3998：定义函数/方法 create_button(window, text, command, width, height, x, y)。
def create_button(window, text, command, width, height, x, y):
# 行 3999：变量赋值：保存配置、状态或计算结果。
    button = tk.Button(window, text=text, command=command, width=width, height=height, bg='#e5e5e5').place(x=x, y=y)
# 行 4000：返回函数结果。
    return button
# 行 4001：定义函数/方法 create_button_left(window, text, command, width, height, x, y)。
def create_button_left(window, text, command, width, height, x, y):
# 行 4002：变量赋值：保存配置、状态或计算结果。
    button = tk.Button(window, text=text, command=command, width=width, height=height, anchor='w', bg='#e5e5e5').place(x=x, y=y)
# 行 4003：返回函数结果。
    return button
# 行 4004：定义函数/方法 create_button1(window, text, command, width, height, x, y, number)。
def create_button1(window, text, command, width, height, x, y, number):
# 行 4005：变量赋值：保存配置、状态或计算结果。
    button = tk.Button(window, text=text, command=command, width=width, height=height, bg='#e5e5e5', font=("SimHei", number, "bold"))
# 行 4006：普通语句：执行一步操作或表达式。
    button.place(x=x, y=y)
# 行 4007：返回函数结果。
    return button
# 行 4008：定义函数/方法 browse_file(entry)。
def browse_file(entry):
# 行 4009：变量赋值：保存配置、状态或计算结果。
    folder_path = filedialog.askdirectory(title="选择文件夹")
# 行 4010：条件判断，根据不同条件执行不同分支。
    if folder_path:
# 行 4011：普通语句：执行一步操作或表达式。
        entry.delete(0, tk.END)
# 行 4012：普通语句：执行一步操作或表达式。
        entry.insert(0, folder_path)
# 行 4013：定义函数/方法 browse_file1(entry, filetypes)。
def browse_file1(entry, filetypes):
# 行 4014：变量赋值：保存配置、状态或计算结果。
    filename = filedialog.askopenfilename(filetypes=filetypes)
# 行 4015：条件判断，根据不同条件执行不同分支。
    if filename:
# 行 4016：普通语句：执行一步操作或表达式。
        entry.delete(0, tk.END)
# 行 4017：普通语句：执行一步操作或表达式。
        entry.insert(0, filename)
# 行 4018：空行，用于分隔代码块，提高可读性。

# 行 4019：程序入口判断：仅在直接运行本文件时执行下面的代码。
if __name__ == '__main__':
# 行 4020：空行，用于分隔代码块，提高可读性。

# 行 4021：变量赋值：保存配置、状态或计算结果。
    app = show_page()
# 行 4022：空行，用于分隔代码块，提高可读性。

# 行 4023：普通语句：执行一步操作或表达式。
    app.root.mainloop()