import sys
import winsound
import tkinter as tk
from concurrent.futures import ThreadPoolExecutor
from tkinter import ttk
import pandas as pd
import math
import ctypes
import pyabf
import matplotlib
matplotlib.use('TkAgg')
ctypes.CDLL("./libusb-1.0.dll")
# import usb1
import er4_python_wrapper as c4
import dynamic_detection
import dynamic_rms
from collections import defaultdict
from pyvisa.ctwrapper.functions import clear
from scipy import signal
from matplotlib.animation import FuncAnimation
from PIL import Image, ImageTk
from tkinter import filedialog, messagebox, StringVar
import os
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2Tk
from matplotlib.figure import Figure
from scipy.ndimage import gaussian_filter1d
import numpy as np
from matplotlib import rcParams
import threading
from scipy.signal import savgol_filter
from datetime import datetime
rcParams['font.sans-serif'] = ['SimHei']

rcParams['axes.unicode_minus'] = False
import pyvisa
import time
import csv

data_lock = threading.Lock()
data_lock1 = threading.Lock()
class show_page:
    def __init__(self):
        self.root = tk.Tk()
        self.root.title("固态纳米孔生物单分子检测仪软件V1--中北大学")
        self.root.state('zoomed')
        self.page_show_number = 0
        self.start_page_original = tk.Frame(self.root, width=1950, height=1000, bg='#e5e5e5', bd=5, relief=tk.RAISED)
        self.control_page_original = None
        self.collection_eONE_page_original = None
        self.analyse_page_original = None
        '''CBD'''
        self.root.protocol("WM_DELETE_WINDOW", self.on_closing)
        self.rm = None
        self.kei_device = None
        self.kei_device_name = None
        self.combo_device_list = None
        self.kei_device_model = None
        self.make_way = False
        self.manual_check = False
        self.manual_make = False
        self.manual_expand = False
        self.update_completed_check = False
        self.a_save_path = None
        self.check_mean_r = None
        self.check_i = []
        self.check_v = []
        self.check_r = []
        self.check_time = []
        self.make_i = []
        self.make_v = []
        self.make_r = []
        self.make_time = []
        self.make_sigma = []
        self.expand_time = []
        self.expand_v = []
        self.expand_i = []
        self.expand_r = []
        self.expand_R_pore = []
        self.add_control_page()
        self.control_page_original.place_forget()
        self.add_collection_eONE_page()
        self.collection_eONE_page_original.place_forget()
        self.add_analyse_page()
        self.analyse_page_original.place_forget()
        # self.add_start_page()
        self.add_control_page()
        self.menu = tk.Menu(self.root)
        self.add_menu()
        self.channel = 'a'
        '''数据采集(eONE)'''
        self.b_start_detection_param = False
        self.b_start_time_1_number = False
        self.b_detection_DNA_None = 0
        self.b_number = 0
        self.b_frequency = 1250
        self.b_start_time = None
        self.b_devices = []
        self.b_total_channels = None
        self.b_v_channels = None
        self.b_i_data = np.array([])
        self.b_v_data = np.array([])
        self.b_t_data = np.array([])
        self.b_std_data_down = np.array([])
        self.b_std_data_up = np.array([])
        self.b_baseline_data = np.array([])
        self.b_t_data_data_4 = np.array([])
        self.b_i_range = None
        self.b_v_range = None
        self.b_i_data_while = np.array([])
        self.b_v_data_while = np.array([])
        self.b_t_data_while = np.array([])
        self.b_i_data_cache = np.array([])
        self.b_t_data_cache = np.array([])
        self.b_cache_number = False
        self.b_std_data_while_down = np.array([])
        self.b_std_data_while_up = np.array([])
        self.b_baseline_data_while = np.array([])
        self.b_start_time = time.perf_counter()
        self.b_end_time = self.b_start_time
        self.b_fig, (self.b_ax1) = plt.subplots(1, 1, figsize=(14.0, 5.5))
        self.b_fig.patch.set_facecolor('#e5e5e5')
        self.b_fig.subplots_adjust(left=0.05,  right=0.99, top=0.99, bottom=0.16)
        self.b_ax1.set_xlabel( "时间(s)", fontsize=10, fontweight='bold', labelpad=10)
        self.b_ax1.set_ylabel( "电流(nA)",fontsize=10, fontweight='bold', labelpad=10)
        self.b_ax1.set_facecolor('#e5e5e5')
        self.b_line1, = self.b_ax1.plot([], [], lw=0.5)
        # self.b_line2, = self.b_ax2.plot([], [], lw=2)
        self.b_connect_stats = 0
        self.b_save_path = None
        self.b_filter = False
        self.b_filter_parameter = None
        self.b_source_current_range = False
        self.b_source_sampling_range = False
        self.b_source_bandwidth_range = False
        self.b_source_volt_range = False
        self.b_baseline_compensate_param = False
        self.b_detection_param = False
        self.b_i_channels = None
        self.b_default_selection_done_1 = False
        self.b_default_selection_done_2 = False
        self.b_default_selection_done_3 = False
        self.b_default_selection_done_4 = False
        self.b_last_current_range_selected = '200 pA'
        self.b_last_sampling_rate_selected = None
        self.b_last_filter_selected = None
        self.b_data_lock = threading.Lock()
        self.b_event_param = False
        self.b_event_list = None
        self.b_event_number = 0
        self.b_save_number = False
        self.b_event_list_append = False
        self.b_save_txt_head = False
        self.b_save_txt_number = False
        self.b_signal_number = 0
        self.b_event_list_all = []
        self.b_event_list_two = []
        self.b_event_amplitude = []
        self.b_event_duration = []
        self.b_event_amplitude_two = []
        self.b_event_duration_two = []
        self.b_signal_direction = 2
        self.b_save_txt_time_current = False
        self.b_csv_txt_thread = None
        self.b_event_list_1 = []
        self.b_event_list_2 = []
        self.b_event_list_3 = []
        self.b_event_list_4 = []
        self.b_event_list_5 = []
        self.b_event_list_6 = []
        self.b_event_list_7 = []
        self.threads = []
        self.b_frame_scroll = False
        self.b_scroll_param = False
        self.b_event_list_two_len = 0
        self.b_event_1_time = False
        self.b_event_two_number_index = []
        self.b_detection_DNA_sequency = 0
        self.b_detection_DNA_yes1 = 0
        '''数据分析'''
        self.b_probe= '探针EC-K12'
        self.b_event_two_open = False
        self.b_event_two_number = 1
        self.b_event_two_open_csv = False
        self.c1_x = np.array([])
        self.c1_y = np.array([])
        self.c1_y = np.array([])
        self.c_std_down = np.array([])
        self.c_baseline = np.array([])
        self.c_std_up = np.array([])
        self.c1_y_filter = np.array([])
        self.c1_y_filter = np.array([])
        self.c_filter = False
        self.c_detection_three = 2
        self.c_detection_sigma = None
        self.c_detection_mintime = None
        self.c_detection_maxtime = None
        self.c_detection_baseline_std_window = None
        self.executor = ThreadPoolExecutor(max_workers=2)
        self.c_frequency = None
        self.c_detection_param = False
        self.c_event_list = defaultdict(list)
        self.c_event_list_two = []
        self.c_event_list_while = None
        self.c_detection_threshold = False
        self.c_event = False
        self.c_baseline_list = []
        self.c_amplitude_list = []
        self.c_integral_list = []
        self.c_duration_list = []
        self.c_bluntness_list = []
        self.c_asymmetry_list = []
        self.c_x_inertia_list = []
        self.c_y_inertia_list = []
        self.c_save_path = None
        self.c_file_path_list_preview = []
        self.c_file_path_list_statistics = []
        self.c_preview_file_name = None
        self.c_statistics_file_name = None
        self.c_events_list_baseline_2 = []
        self.c_events_list_amplitude = []
        self.c_events_list_integral = []
        self.c_events_list_duration = []
        self.c_events_list_bluntness = []
        self.c_events_list_asymmetry = []
        self.c_events_list_x_inertia = []
        self.c_events_list_y_inertia = []
        self.c_file_folder_choice = False
        self.c_file_abf_list = []

    def on_closing(self):

        c4.disconnect()
        if hasattr(self, 'threads'):
            for t in self.threads:
                if t.is_alive():
                    t.join(timeout=2)
        self.root.quit()
        self.root.destroy()

    def add_start_page(self):
        if self.page_show_number == 0:
            self.start_page_original.place(x=0, y=0, relwidth=1, relheight=1)

            def background_image():

                img = Image.open(r"background.jpg")
                img = img.resize((self.root.winfo_width(), self.root.winfo_height()), Image.Resampling.LANCZOS)
                imgtk = ImageTk.PhotoImage(image=img)

                label = tk.Label(self.start_page_original, image=imgtk)
                label.image = imgtk  # 保存引用
                label.place(x=0, y=0, relwidth=1, relheight=1)

                self.imgtk = imgtk

            self.root.after(100, background_image)
        else:
            self.start_page_original.destroy()

    def add_menu(self):
        self.menu = tk.Menu(self.root)
        self.root.config(menu=self.menu)

        self.menu.add_command(label="CBD打孔", command=lambda: self.function1("CBD打孔"))
        self.menu.add_command(label="数据采集", command=lambda: self.function1("数据采集"))
        self.menu.add_command(label="数据分析", command=lambda: self.function1("数据分析"))
    def on_select(self, number):
        if number == 1:
            self.kei_device_onselect = self.combo1.get()
        else:
            self.channel = self.combo2.get()
    def inquire_device(self):
        self.combo_device_list.place_forget()
        self.rm = pyvisa.ResourceManager()
        self.kei_device_name = self.rm.list_resources()
        self.combo_device_list['values'] = self.kei_device_name
        options1 = [self.kei_device_name]

        self.combo1 = ttk.Combobox(self.control_page_original, values=options1, width=15, state='readonly')
        self.combo1.current(0)
        self.combo1.bind("<<ComboboxSelected>>", lambda event: self.on_select(1))
        self.combo1.place(x=110, y=8)
        return self.kei_device_name
    def connect_device(self, path_entry, kei_device_name):
        self.kei_device = self.rm.open_resource(self.rm.list_resources()[1])
        self.kei_device_model = self.kei_device.query("*IDN?")
        self.update_color_button(1, 'green')
        # self.kei_device.write('beeper.enable = beeper.ON')
        # self.kei_device.write('beeper.beep(0.5, 2400) ')
        # time.sleep(1)
        # self.kei_device.write('beeper.beep(0.5, 2400) ')
        # time.sleep(1)
        # self.kei_device.write('beeper.beep(0.5, 2400) ')
        self.update_event_print(path_entry, self.kei_device_model)
        self.kei_device.write(f'smu{self.channel}.reset()')
        self.kei_device.write(f'smu{self.channel}.source.output = smu{self.channel}.OUTPUT_OFF')
        self.kei_device.write(f'smu{self.channel}.source.func = smu{self.channel}.OUTPUT_DCVOLTS')
        self.kei_device.write(f'smu{self.channel}.source.autorangev = smu{self.channel}.AUTORANGE_ON')
        self.kei_device.write(f'smu{self.channel}.source.levelv = 0')
        self.kei_device.write(f'smu{self.channel}.source.limiti = {self.limit_current_param.get()}')
    def creat_color_button(self, number, control_page, x, y):
        if not hasattr(self, 'canvas_dict'):
            self.canvas_dict = {}
        self.canvas = tk.Canvas(control_page, width=25, height=25, bg='#e5e5e5', bd=0,
                                highlightthickness=0)
        self.canvas.place(x=x, y=y)
        self.canvas_dict[number] = self.canvas
        self.canvas.create_oval(5, 5, 20, 20, fill='red', outline='black')
        self.canvas.config(state='disabled')

    def update_color_button(self, number, color):
        self.canvas = self.canvas_dict.get(number)
        if self.canvas:
            self.canvas.itemconfig(1, fill=color, outline='black')

    def update_event_print(self, path_entry, new_print):
        path_entry.config(state=tk.NORMAL)
        path_entry.delete(0, tk.END)
        path_entry.insert(0, new_print)
        path_entry.config(state=tk.DISABLED)

    def create_toggle_switch(self, master, x, y):
        width, height = 70, 30
        active_color = "green"
        inactive_color = "lightgray"
        circle_color = "white"
        circle_radius = 20
        switch = tk.Canvas(master, width=width, height=height)
        switch.place(x=x, y=y)

        self.make_way = False
        background = switch.create_rectangle(5, 5, width - 5, height - 5, fill=inactive_color, outline="gray", width=2)
        circle = switch.create_oval(5, 5, circle_radius + 5, height - 5, fill=circle_color, outline=inactive_color)
        def toggle(event=None):
            if self.make_way:
                self.make_way = False
                print('guan')
                switch.coords(circle, 5, 5, circle_radius + 5, height - 5)
                switch.itemconfig(background, fill=inactive_color)
                self.canvas_manual_check.tag_bind('shape', '<Button-1>',
                                                   lambda event: self.toggle_shape('check', self.canvas_manual_check))
                self.canvas_manual_make.tag_bind('shape', '<Button-1>',
                                                  lambda event: self.toggle_shape('make', self.canvas_manual_make))
                self.canvas_manual_expand.tag_bind('shape', '<Button-1>',
                                                   lambda event: self.toggle_shape('expand', self.canvas_manual_expand))
                self.canvas_manual_check.itemconfig('shape', fill='green')
                self.canvas_manual_make.itemconfig('shape', fill='green')
                self.canvas_manual_expand.itemconfig('shape', fill='green')
                self.start_make_pore.config(state=tk.DISABLED, bg='lightgray')
            else:
                self.make_way = True
                print('kai')
                switch.coords(circle, width - circle_radius - 5, 5, width - 5, height - 5)
                switch.itemconfig(background, fill=active_color)
                self.canvas_manual_check.tag_bind('shape', '<Button-1>', lambda event: None)
                self.canvas_manual_make.tag_bind('shape', '<Button-1>', lambda event: None)
                self.canvas_manual_expand.tag_bind('shape', '<Button-1>', lambda event: None)
                self.canvas_manual_check.itemconfig('shape', fill='gray')
                self.canvas_manual_make.itemconfig('shape', fill='gray')
                self.canvas_manual_expand.itemconfig('shape', fill='gray')
                self.start_make_pore.config(state=tk.NORMAL, bg='#e5e5e5')

        switch.bind("<Button-1>", toggle)

        return switch

    def draw_triangle(self, process, canvas):

        canvas.delete("shape")
        if process == 'check':
            if self.manual_check == False:
                canvas.create_polygon(0, 1, 31, 16, 0, 31, fill='green', outline='black', tags='shape')
                # canvas.tag_bind('shape', '<Button-1>', lambda event: None)

            else:
                canvas.create_rectangle(0, 0, 31, 31, fill='black', outline='black', tags='shape')
        elif process == 'make':
            if self.manual_make == False:
                canvas.create_polygon(0, 1, 31, 16, 0, 31, fill='green', outline='black', tags='shape')

            else:
                canvas.create_rectangle(0, 0, 31, 31, fill='black', outline='black', tags='shape')
        else:
            if self.manual_expand == False:
                canvas.create_polygon(0, 1, 31, 16, 0, 31, fill='green', outline='black', tags='shape')

            else:
                canvas.create_rectangle(0, 0, 31, 31, fill='black', outline='black', tags='shape')


    def toggle_shape(self, process, canvas_ts):
        if process == 'check':
            self.manual_check = not self.manual_check
            self.draw_triangle('check', canvas_ts)
            if self.manual_check == True:
                self.check_volt_param.config(state=tk.DISABLED)
                self.kei_device.write(f'smu{self.channel}.reset()')
                self.kei_device.write(f'smu{self.channel}.source.output = smu{self.channel}.OUTPUT_ON')
                self.kei_device.write(f'smu{self.channel}.source.limiti = {self.limit_current_param.get()}')
                self.check_volt1 = -1 * float(self.check_volt_param.get())
                self.check_volt2 = float(self.check_volt_param.get())
                step = 0.01
                check_time = 0
                voltage = self.check_volt1
                self.check_i.clear()
                self.check_v.clear()
                self.check_r.clear()
                self.check_time.clear()
                self.ax_check.clear()

                def update_data():
                    nonlocal voltage, check_time
                    if self.manual_check == True:
                        if voltage <= self.check_volt2:
                            self.kei_device.write(f'smu{self.channel}.source.levelv = {voltage}')
                            data_i = float(self.kei_device.query(
                                f'print(smu{self.channel}.measure.i(smu{self.channel}.nvbuffer1))'))
                            self.check_i.append(round(data_i, 12))
                            self.check_v.append(round(voltage, 4))
                            self.check_r.append(abs(voltage / data_i))
                            self.check_time.append(check_time)
                            voltage += step
                            check_time += 0.5
                            self.plot_check_update()
                            self.root.after(500, update_data)
                        else:
                            self.check_mean_r = np.mean(self.check_r)
                            self.check_r_entry.config(state=tk.NORMAL)  # 允许修改文本框
                            self.check_r_entry.delete(0, tk.END)
                            self.check_r_entry.insert(0, self.check_mean_r)
                            self.check_r_entry.config(state=tk.DISABLED)  # 禁止编辑
                            self.kei_device.write(f'smu{self.channel}.source.output = smu{self.channel}.OUTPUT_OFF')
                            self.kei_device.write(f'smu{self.channel}.reset()')
                            self.manual_check = not self.manual_check
                            self.draw_triangle('check', canvas_ts)
                            self.check_volt_param.config(state=tk.NORMAL)
                    else:
                        self.kei_device.write(f'smu{self.channel}.reset()')
                        self.kei_device.write(f'smu{self.channel}.source.output = smu{self.channel}.OUTPUT_OFF')
                        pass
                update_data()
            else:
                self.check_volt_param.config(state=tk.NORMAL)
        elif process == 'make':
            self.manual_make = not self.manual_make
            self.draw_triangle('make', canvas_ts)
            if self.manual_make == True:
                self.kei_device.write(f'smu{self.channel}.reset()')
                self.kei_device.write(f'smu{self.channel}.source.output = smu{self.channel}.OUTPUT_ON')
                self.kei_device.write(f'smu{self.channel}.source.limiti = {self.limit_current_param.get()}')
                time_start = 0
                time_sleep = 0.5
                self.make_v.clear()
                self.make_i.clear()
                self.make_r.clear()
                self.make_time.clear()
                self.make_sigma.clear()
                voltage = float(self.make_volt_param.get())
                self.kei_device.write(f'smu{self.channel}.source.levelv = {voltage}')
                time.sleep(3)
                self.axs_make[0].cla()
                self.axs_make[1].cla()
                self.axs_make[2].cla()
                self.axs_make[3].cla()

                def update_make_data():
                    nonlocal time_start, time_sleep
                    if self.manual_make == True:
                        data = float(self.kei_device.query(f'print(smu{self.channel}.measure.i())'))
                        self.make_volt_param.config(state=tk.DISABLED)
                        self.make_solution_param.config(state=tk.DISABLED)
                        self.make_SiN_param.config(state=tk.DISABLED)
                        self.make_i.append(round(data, 12))
                        self.make_v.append(round(voltage, 4))
                        self.make_r.append(voltage / data)
                        self.make_time.append(time_start)
                        if len(self.make_v) < 10:
                            sigma = (self.make_i[-1] - np.mean(self.make_i[:])) / np.std(self.make_i[:])
                            self.make_sigma.append(sigma)
                        else:
                            sigma = (self.make_i[-1] - np.mean(self.make_i[-10:-1])) / np.std(self.make_i[-10:-1])
                            self.make_sigma.append(sigma)

                        time_start += time_sleep
                        self.plot_VIR_sigma_update()
                        if len(self.make_v) <= 300:
                            if sigma >= 100 and sigma > 0:
                                # self.kei_device.write(f'smua.source.levelv = 0')
                                # print('打孔完成')
                                # df = pd.DataFrame({
                                #     'time': self.make_time,
                                #     'voltage': self.make_v,
                                #     'current': self.make_i,
                                #     'resistor': self.make_r,
                                #     'sigma': self.make_sigma,
                                # })
                                # df.to_csv(f'{self.a_save_path}/pore_make.csv', index=False)
                                # self.kei_device.write(f'smua.source.levelv = 0')
                                # R_pore = self.calculate_d(self.make_r[-1], 11.27, 0.00000003)
                                # self.expand_pore(R_pore, 50)
                                self.kei_device.write(f'smu{self.channel}.reset()')
                                self.kei_device.write(f'smu{self.channel}.source.output = smu{self.channel}.OUTPUT_OFF')
                                pass
                            else:
                                self.root.after(50, update_make_data)
                        else:
                            self.make_v = self.make_v[-30:]
                            print(self.make_v)
                            self.make_i = self.make_i[-30:]
                            self.make_time = self.make_time[-30:]
                            self.make_r = self.make_r[-30:]
                            self.make_sigma = self.make_sigma[-30:]
                            self.axs_make[0].cla()
                            self.axs_make[1].cla()
                            self.axs_make[2].cla()
                            self.axs_make[3].cla()
                            self.root.after(50, update_make_data)
                    else:
                        self.kei_device.write(f'smu{self.channel}.reset()')
                        self.kei_device.write(f'smu{self.channel}.source.output = smu{self.channel}.OUTPUT_OFF')
                        pass
                update_make_data()
            else:
                self.make_volt_param.config(state=tk.NORMAL)
                self.make_solution_param.config(state=tk.NORMAL)
                self.make_SiN_param.config(state=tk.NORMAL)
        else:
            self.manual_expand = not self.manual_expand
            self.draw_triangle('expand', canvas_ts)
            if self.manual_expand == True:
                self.kei_device.write(f'smu{self.channel}.reset()')
                self.kei_device.write(f'smu{self.channel}.source.output = smu{self.channel}.OUTPUT_ON')
                self.kei_device.write(f'smu{self.channel}.source.limiti = {self.limit_current_param.get()}')
                self.expand_volt_param.config(state=tk.DISABLED)
                self.pore_target_param.config(state=tk.DISABLED)
                self.kei_device.write(f'smua.source.levelv = 0')

                voltage = float(self.expand_volt_param.get())
                expand_time = 0
                solution_sigma = float(self.make_solution_param.get())
                pore_h = float(self.make_SiN_param.get())
                pore_d_target = float(self.pore_target_param.get())
                number = 0
                self.axs_expand[0].cla()
                self.axs_expand[1].cla()
                self.expand_i.clear()
                self.expand_v.clear()
                self.expand_r.clear()
                self.expand_time.clear()
                self.expand_R_pore.clear()
                def update_data_expand():
                    if self.manual_expand == True:
                        nonlocal voltage, expand_time, number
                        if number == 0:
                            R_pore = self.calculate_d(1 / 10000, solution_sigma, pore_h)
                            number = 1
                            self.kei_device.write(f'smu{self.channel}.source.levelv = {voltage}')
                            data_i = float(self.kei_device.query(f'print(smu{self.channel}.measure.i())'))
                            self.expand_i.append(round(data_i, 12))
                            self.expand_v.append(round(voltage, 4))
                            self.expand_r.append(abs(voltage / data_i))
                            self.expand_time.append(expand_time)
                            self.expand_R_pore.append(R_pore)
                            voltage = -voltage
                            expand_time += 0.5
                            self.root.after(500, update_data_expand)
                        else:
                            if len(self.expand_v) <= 200:
                                print(len(self.expand_v))
                                R_pore = self.calculate_d(1 / self.expand_r[-1], solution_sigma, pore_h)
                                if R_pore < pore_d_target:
                                    self.kei_device.write(f'smu{self.channel}.source.levelv = {voltage}')
                                    data_i = float(self.kei_device.query(f'print(smu{self.channel}.measure.i())'))
                                    self.expand_i.append(round(data_i, 12))
                                    self.expand_v.append(round(voltage, 4))
                                    self.expand_r.append(abs(voltage / data_i))
                                    self.expand_time.append(expand_time)
                                    self.expand_R_pore.append(R_pore)
                                    voltage = -voltage
                                    expand_time += 0.5

                                    self.plot_expand_update()

                                    self.root.after(500, update_data_expand)
                                else:
                                    self.kei_device.write(f'smu{self.channel}.source.levelv = 0')
                                    # df = pd.DataFrame({
                                    #     'time': self.expand_time,
                                    #     'voltage': self.expand_v,
                                    #     'current': self.expand_i,
                                    #     'resistor': self.expand_r,
                                    #     'R_pore': self.expand_R_pore,
                                    # })
                                    # df.to_csv(f'{self.a_save_path}/pore_expand.csv', index=False)
                                    self.kei_device.write(f'smu{self.channel}.source.levelv = 0')
                                    print('扩孔完成，已达到目标孔径')
                            else:
                                self.expand_v = self.expand_v[-30:]
                                self.expand_i = self.expand_i[-30:]
                                self.expand_time = self.expand_time[-30:]
                                self.expand_r = self.expand_r[-30:]
                                self.expand_R_pore = self.expand_R_pore[-30:]
                                self.axs_expand[0].cla()
                                self.axs_expand[1].cla()
                                self.root.after(500, update_data_expand)
                    else:
                        self.kei_device.write(f'smu{self.channel}.reset()')
                        self.kei_device.write(f'smu{self.channel}.source.output = smu{self.channel}.OUTPUT_OFF')
                        pass

                update_data_expand()
            else:
                self.expand_volt_param.config(state=tk.NORMAL)
                self.pore_target_param.config(state=tk.NORMAL)
    def create_dashed_square(self,canvas, x1, y1, x2, y2, side_length):
        # 虚线
        canvas.create_rectangle(
            x1, y1, x2, y2,
            outline="black", width=2  # dash=(4, 4) 表示虚线的间隔
        )
        # canvas.create_line(
        #     x1, y1 + 200, x1 + side_length, y1 + 200,
        #     dash=(4, 4),
        #     fill="black",
        #     width=2
        # )
    def create_dashed_line(self,canvas, x1, y1, x2, y2, side_length):

        canvas.create_line(
            x1, y1, x2, y2,
            dash=(4, 4),
            fill="black",
            width=side_length
        )
    def a_open_new_window(self):
        new_window = tk.Toplevel(self.root)
        new_window.title("选择路径")
        new_window.geometry("330x120")
        new_window.grab_set()

        path_entry = create_entry(new_window, 30, 40, 30)

        browse_button = tk.Button(new_window, text="浏览", bg='#e5e5e5', font=('', 9),
                                  command=lambda: browse_file(path_entry))
        browse_button.place(x=287, y=30)

        confirm_button = tk.Button(new_window, text="确定", bg='#e5e5e5', font=('', 9),
                                   command=lambda: self.a_on_confirm(new_window, path_entry.get()))
        confirm_button.place(x=140, y=80)

    def a_on_confirm(self, window, folder_path):
        if folder_path:
            self.a_save_path = folder_path

            self.save_path_entry.config(state=tk.NORMAL)  # 允许修改
            self.save_path_entry.delete(0, tk.END)
            self.save_path_entry.insert(0, folder_path)
            self.save_path_entry.config(state=tk.DISABLED)  # 禁止编辑
            window.destroy()

    def b_open_new_window(self):
        new_window = tk.Toplevel(self.root)
        new_window.title("选择路径")
        new_window.geometry("330x120")
        new_window.grab_set()

        path_entry = create_entry(new_window, 30, 40, 30)

        browse_button = tk.Button(new_window, text="浏览", bg='#e5e5e5', font=('', 9),
                                  command=lambda: browse_file(path_entry))
        browse_button.place(x=287, y=30)

        confirm_button = tk.Button(new_window, text="确定", bg='#e5e5e5', font=('', 9),
                                   command=lambda: self.b_on_confirm(new_window, path_entry.get()))
        confirm_button.place(x=140, y=80)

    def b_on_confirm(self, window, folder_path):
        if folder_path:
            self.b_save_path = folder_path
            print(self.b_save_path)
            # self.save_path_entry.config(state=tk.NORMAL)  # 允许修改文本框
            # self.save_path_entry.delete(0, tk.END)
            # self.save_path_entry.insert(0, folder_path)
            # self.save_path_entry.config(state=tk.DISABLED)  # 禁止编辑
            window.destroy()
    def plot_VIR_sigma(self):
        self.fig_make, self.axs_make = plt.subplots(4, 1, figsize=(10, 7), dpi=100)
        self.fig_make.patch.set_facecolor('#e5e5e5')
        # axs[0].plot(x, v, color='white')
        self.axs_make[0].set_title("CBD")
        self.axs_make[0].set_ylabel("Voltage(V)")
        self.axs_make[0].grid(False)
        self.axs_make[0].get_xaxis().set_visible(False)
        self.axs_make[0].set_facecolor('black')

        # axs[1].plot(x, i, color='white')
        self.axs_make[1].set_ylabel("Current(nA)")
        self.axs_make[1].grid(False)
        self.axs_make[1].get_xaxis().set_visible(False)
        self.axs_make[1].set_facecolor('black')

        # axs[2].plot(x, r, color='white')
        self.axs_make[2].set_ylabel("Resistance(MΩ)")
        self.axs_make[2].grid(False)
        self.axs_make[2].get_xaxis().set_visible(False)
        self.axs_make[2].set_facecolor('black')

        # axs[3].plot(x, sigma, color='white')
        self.axs_make[3].set_xlabel("Time(s)")
        self.axs_make[3].set_ylabel("Sigma(I)")
        self.axs_make[3].grid(False)
        self.axs_make[3].set_facecolor('black')
        self.fig_make.tight_layout()
        self.canvas_plotVIR_sigma = FigureCanvasTkAgg(self.fig_make, master=self.control_page_original)
        self.canvas_widget_VIR_sigma = self.canvas_plotVIR_sigma.get_tk_widget()
        self.canvas_widget_VIR_sigma.place(x=0, y=292)

    def plot_VIR_sigma_update(self):
        if len(self.make_v) > 0:
            self.axs_make[0].set_title("CBD")
            self.axs_make[0].set_ylabel("Voltage(V)")
            self.axs_make[1].set_ylabel("Current(A)")
            self.axs_make[2].set_ylabel("Resistance(Ω)")
            self.axs_make[3].set_xlabel("Time(s)")
            self.axs_make[3].set_ylabel("Sigma(I)")
            self.axs_make[0].plot(self.make_time, self.make_v, color='white',linewidth=0.3)
            self.axs_make[1].plot(self.make_time, self.make_i, color='white',linewidth=0.3)
            self.axs_make[2].plot(self.make_time, self.make_r, color='white',linewidth=0.3)
            self.axs_make[3].plot(self.make_time, self.make_sigma, color='white',linewidth=0.3)
            self.axs_make[0].grid(False)
            self.axs_make[1].grid(False)
            self.axs_make[2].grid(False)
            self.axs_make[3].grid(False)
            self.axs_make[0].set_facecolor('black')
            self.axs_make[1].set_facecolor('black')
            self.axs_make[2].set_facecolor('black')
            self.axs_make[3].set_facecolor('black')
            self.fig_make.tight_layout()
            self.canvas_plotVIR_sigma.draw()

    def plot_check(self):
        self.fig_check = Figure(figsize=(9, 3), dpi=100)
        self.fig_check.patch.set_facecolor('#e5e5e5')
        self.ax_check = self.fig_check.add_subplot(111)
        self.ax_check.set_title("芯片检查")
        self.ax_check.set_xlabel("Time(s)")
        self.ax_check.set_ylabel("Current(A)")
        self.ax_check.grid(True)
        # ax_check.plot(x, i, color='blue')
        self.ax_check.set_facecolor('black')
        self.fig_check.tight_layout()
        self.canvas_current_check = FigureCanvasTkAgg(self.fig_check, master=self.control_page_original)
        self.canvas_widget_check = self.canvas_current_check.get_tk_widget()
        self.canvas_widget_check.place(x=1000, y=300)
    def plot_check_update(self):
        if len(self.check_v) > 0:
            self.ax_check.plot(self.check_v, self.check_i, color='blue')
            self.ax_check.set_title("芯片检查")
            self.ax_check.set_xlabel("Time(s)")
            self.ax_check.set_ylabel("Current(A)")
            self.ax_check.grid(True)
            self.ax_check.set_facecolor('black')
            self.fig_check.tight_layout()
            self.canvas_current_check.draw()


    def plot_expand_pore(self):
        self.fig_expand, self.axs_expand = plt.subplots(2, 1, figsize=(9, 3.7), dpi=100)
        self.fig_expand.patch.set_facecolor('#e5e5e5')
        # axs[0].plot(x, i, color='red')
        self.axs_expand[0].set_title("扩孔")
        self.axs_expand[0].set_ylabel("Voltage(V)")
        self.axs_expand[0].grid(True)
        self.axs_expand[0].set_facecolor('black')

        # axs[1].plot(x, r, color='red')
        self.axs_expand[1].set_ylabel("D_pore(nm)")
        self.axs_expand[1].grid(True)
        self.axs_expand[1].set_facecolor('black')

        self.fig_expand.tight_layout()
        self.canvas_plot_expand_pore = FigureCanvasTkAgg(self.fig_expand, master=self.control_page_original)
        self.canvas_widget_expand_pore = self.canvas_plot_expand_pore.get_tk_widget()
        self.canvas_widget_expand_pore.place(x=1000, y=600)

    def plot_expand_update(self):
        if len(self.expand_i) > 0:
            self.axs_expand[0].plot(self.expand_time[1:], self.expand_v[1:], color='red')
            self.axs_expand[1].plot(self.expand_time[1:], self.expand_R_pore[1:], color='red')
            self.ax_check.set_title("扩孔")
            self.fig_expand.tight_layout()
            self.canvas_plot_expand_pore.draw()

    def start_make_pore_process(self):
        self.update_color_button(2, 'yellow')
        self.kei_device.write(f'smu{self.channel}.reset()')
        self.kei_device.write(f'smu{self.channel}.source.output = smu{self.channel}.OUTPUT_OFF')
        self.kei_device.write(f'smu{self.channel}.source.func = smu{self.channel}.OUTPUT_DCVOLTS')
        self.kei_device.write(f'smu{self.channel}.source.autorangev = smu{self.channel}.AUTORANGE_ON')
        self.kei_device.write(f'smu{self.channel}.source.levelv = 0')
        self.kei_device.write(f'smu{self.channel}.source.limiti = {self.limit_current_param.get()}')
        self.kei_device.write(f'smu{self.channel}.source.output = smu{self.channel}.OUTPUT_ON')
        self.check_volt1 = -1 * float(self.check_volt_param.get())
        self.check_volt2 = float(self.check_volt_param.get())
        step = 0.01
        check_time = 0
        voltage = self.check_volt1
        self.check_i.clear()
        self.check_v.clear()
        self.check_r.clear()
        self.check_time.clear()
        self.ax_check.clear()

        def update_data():
            nonlocal voltage, check_time
            if voltage <= self.check_volt2:
                self.kei_device.write(f'smu{self.channel}.source.levelv = {voltage}')
                data_i = float(self.kei_device.query(f'print(smu{self.channel}.measure.i(smu{self.channel}.nvbuffer1))'))
                self.check_i.append(round(data_i, 12))
                self.check_v.append(round(voltage, 4))
                self.check_r.append(abs(voltage / data_i))
                self.check_time.append(check_time)
                voltage += step
                check_time += 0.5
                self.plot_check_update()
                self.root.after(500, update_data)
            else:
                self.check_mean_r = np.mean(self.check_r)
                self.check_r_entry.config(state=tk.NORMAL)  # 允许修改文本框
                self.check_r_entry.delete(0, tk.END)
                self.check_r_entry.insert(0, self.check_mean_r)
                self.check_r_entry.config(state=tk.DISABLED)  # 禁止编辑
                if 1e8 < np.mean(self.check_r) < 1e11:
                    self.update_completed_check = True
                    df = pd.DataFrame({
                        'time': self.check_time,
                        'voltage': self.check_v,
                        'current': self.check_i,
                        'resistor': self.check_r,
                    })
                    df.to_csv(f'{self.a_save_path}/pore_check.csv', index=False)
                    self.kei_device.write(f'smu{self.channel}.reset()')
                    self.kei_device.write(f'smu{self.channel}.source.output = smu{self.channel}.OUTPUT_OFF')
                    self.check_update_finished()
                # elif np.mean(self.check_r) < 1e5:
                #     self.update_color_button(2, 'red')
                #     print(np.mean(self.check_r))
                #     print('流通池漏液')
                # else:
                #     self.update_color_button(2, 'red')
                #     print(np.mean(self.check_r))
                #     print('芯片表面有气泡')

        update_data()

    def check_update_finished(self):
        if self.update_completed_check == True:
            if self.check_i[-1] <= 0.01:
                time_start = 0
                time_sleep = 0.5
                self.update_color_button(2, 'green')
                self.update_color_button(3, 'yellow')
                self.make_v.clear()
                self.make_i.clear()
                self.make_r.clear()
                self.make_time.clear()
                voltage = float(self.make_volt_param.get())
                limit_i = self.limit_current_param.get()
                self.kei_device.write(f'smu{self.channel}.source.levelv = {voltage}')
                self.kei_device.write(f'smu{self.channel}.source.limiti = {limit_i}')
                time.sleep(3)
                def update_make_data():
                    nonlocal time_start, time_sleep
                    data = float(self.kei_device.query(f'print(smu{self.channel}.measure.i())'))
                    self.make_i.append(round(data, 12))
                    self.make_v.append(round(voltage, 4))
                    self.make_r.append(voltage / data)
                    self.make_time.append(time_start)
                    if len(self.make_v)< 10:
                        sigma = (self.make_i[-1] - np.mean(self.make_i[:])) / np.std(self.make_i[:])
                        self.make_sigma.append(sigma)
                    else:
                        sigma = (self.make_i[-1] - np.mean(self.make_i[-10:-1])) / np.std(self.make_i[-10:-1])
                        self.make_sigma.append(sigma)

                    time_start += time_sleep
                    self.plot_VIR_sigma_update()
                    print(self.make_i[-1], self.make_r[-1], self.make_sigma[-1])
                    if self.make_i[-1] < limit_i:
                        if len(self.make_v) <= 300:
                            if sigma >= 100 and sigma > 0:
                                self.kei_device.write(f'smu{self.channel}.source.levelv = 0')
                                print('打孔完成')
                                df = pd.DataFrame({
                                    'time': self.make_time,
                                    'voltage': self.make_v,
                                    'current': self.make_i,
                                    'resistor': self.make_r,
                                    'sigma': self.make_sigma,
                                })
                                df.to_csv(f'{self.a_save_path}/pore_make.csv', index=False)
                                self.kei_device.write(f'smu{self.channel}.source.levelv = 0')
                                R_pore = self.calculate_d(self.make_r[-1], 11.27, 0.00000003)
                                self.expand_pore(R_pore, self.pore_target_param.get())
                            else:
                                self.root.after(500, update_make_data)
                        else:
                            self.make_v = self.make_v[-30:]
                            print(self.make_v)
                            self.make_i = self.make_i[-30:]
                            self.make_time = self.make_time[-30:]
                            self.make_r = self.make_r[-30:]
                            self.make_sigma = self.make_sigma[-30:]
                            self.axs_make[0].cla()
                            self.axs_make[1].cla()
                            self.axs_make[2].cla()
                            self.axs_make[3].cla()
                            self.root.after(500, update_make_data)
                    else:
                        self.update_color_button(3, 'green')
                        print("达到电流限值")

                update_make_data()
            else:
                self.update_completed_check = False
                self.update_color_button(2, 'red')

    def calculate_d(self, pore_sigma, solution_sigma, pore_h):
        return (pore_sigma / (2 * solution_sigma)) * (1 + math.sqrt(1 + (16 * solution_sigma * pore_h) / (math.pi * pore_sigma)))

    def expand_pore(self, R_pore_first, R_pore_last):
        self.kei_device.write(f'smu{self.channel}.source.levelv = 0')
        voltage = float(self.expand_volt_param.get())
        expand_time = 0
        solution_sigma = float(self.make_solution_param.get())
        pore_h = float(self.make_SiN_param.get())
        pore_d_target = float(self.pore_target_param.get())
        self.axs_expand[0].cla()
        self.axs_expand[1].cla()
        self.expand_i.clear()
        self.expand_v.clear()
        self.expand_r.clear()
        self.expand_time.clear()
        self.expand_R_pore.clear()
        def update_data_expand():
            nonlocal voltage, expand_time
            if len(self.expand_v) <= 300:
                R_pore = self.calculate_d(1 / self.make_r[-1], solution_sigma, pore_h)
                if R_pore < pore_d_target:
                    self.kei_device.write(f'smu{self.channel}.source.levelv = {voltage}')
                    data_i = float(self.kei_device.query(f'print(smu{self.channel}.measure.i())'))
                    self.expand_i.append(round(data_i, 12))
                    self.expand_v.append(round(voltage, 4))
                    self.expand_r.append(abs(voltage / data_i))
                    self.expand_time.append(expand_time)
                    self.expand_R_pore.append(R_pore)
                    voltage = -voltage
                    expand_time += 0.5

                    self.plot_expand_update()

                    self.root.after(100, update_data_expand)
                else:
                    self.kei_device.write(f'smu{self.channel}.source.levelv = 0')
                    self.kei_device.write(f'smu{self.channel}.reset()')
                    self.kei_device.write(f'smu{self.channel}.source.output = smu{self.channel}.OUTPUT_OFF')
                    df = pd.DataFrame({
                        'time': self.expand_time,
                        'voltage': self.expand_v,
                        'current': self.expand_i,
                        'resistor': self.expand_r,
                        'R_pore': self.expand_R_pore,
                    })
                    df.to_csv(f'{self.a_save_path}/pore_expand.csv', index=False)
                    self.kei_device.write(f'smu{self.channel}.source.levelv = 0')
                    print('扩孔完成，已达到目标孔径')
            else:
                self.expand_v = self.expand_v[-30:]
                self.expand_i = self.expand_i[-30:]
                self.expand_time = self.expand_time[-30:]
                self.expand_r = self.make_r[-30:]
                self.expand_R_pore = self.expand_R_pore[-30:]
                self.axs_expand[0].cla()
                self.axs_expand[1].cla()
                self.root.after(100, update_data_expand)
        update_data_expand()
    def add_control_page(self):
        self.control_page_original = tk.Frame(self.root, width=1950, height=1000, bg='#e5e5e5', bd=5,
                                              relief=tk.RAISED)
        self.control_page_original.place(x=0, y=0)

        self.canvas_sq_line_model = tk.Canvas(self.control_page_original, width=310, height=210, bg='#e5e5e5', bd=0)
        self.canvas_sq_line_model.place(x=0, y=80)
        self.create_dashed_square(self.canvas_sq_line_model, 3, 3, 310, 210, 1)

        # 模式选择子面板
        self.model_panel = tk.Frame(self.control_page_original, width=304, height=204, bg='#e5e5e5')
        self.model_panel.place(x=3, y=83)
        self.model_panel.grid_columnconfigure(0, weight=1)

        self.creat_color_button(1, self.control_page_original, 300, 8)

        self.device_list_button = create_button(self.control_page_original, '检查连接设备', self.inquire_device, 12, 1,5, 5)

        def on_select_device(kei_device_name):
            self.kei_device_name = kei_device_name

        self.combo_device_list = ttk.Combobox(self.control_page_original, values=['', '', ''], width=15,
                                              state='readonly')
        self.combo_device_list.current(0)
        self.combo_device_list.bind("<<ComboboxSelected>>", lambda event: on_select_device(self.kei_device_name))
        self.combo_device_list.place(x=110, y=8)


        self.device_connect_button = create_button(self.control_page_original, '连接',lambda: self.connect_device(self.device_print,str(self.kei_device_model)[:-1]), 5, 1,250, 5)

        self.device_print = create_entry2(self.control_page_original, 43, 5, 50, default_value="无连接")
        self.device_print.config(state=tk.DISABLED)

        tk.Button(self.control_page_original, text="存储路径", command=self.a_open_new_window,
                  width=8, height=1).place(x=350, y=5)
        self.save_path_entry = create_entry(self.control_page_original, 50, 450, 8)
        self.save_path_entry.config(state=tk.DISABLED)

        # 模式选择内容
        self.model_select = tk.Label(self.model_panel, text='模式选择:', bg='#e5e5e5', font=("SimHei", 13))
        self.model_select.grid(row=0, column=0, sticky='w', padx=6, pady=(6, 2))

        self.start_way = tk.Label(self.model_panel, text='手动打孔   ||   自动打孔', bg='#e5e5e5', font=("SimHei", 13))
        self.start_way.grid(row=1, column=0, sticky='w', padx=6, pady=6)

        self.start_make_pore = tk.Button(self.model_panel, text='开始打孔', command=lambda: self.start_make_pore_process(),
                                         width=10, height=2, bg='#e5e5e5', font=("SimHei", 12, "bold"))
        self.start_make_pore.grid(row=2, column=0, sticky='w', padx=6, pady=(8, 6))
        self.start_make_pore.config(state=tk.DISABLED, bg='lightgray')

        # toggle 开关放入子面板，使用 place 以避免与 grid 冲突
        self.toggle_switch = self.create_toggle_switch(self.model_panel, 220, 4)

        self.canvas_sq_line_check = tk.Canvas(self.control_page_original, width=310, height=210, bg='#e5e5e5', bd=0)
        self.canvas_sq_line_check.place(x=312, y=80)
        self.create_dashed_square(self.canvas_sq_line_check, 3, 3, 310, 210, 1)

        # 检查参数子面板（使用grid，使右侧值始终可见）
        self.check_panel = tk.Frame(self.control_page_original, width=304, height=204, bg='#e5e5e5')
        self.check_panel.place(x=315, y=83)
        self.check_panel.grid_columnconfigure(1, weight=1)

        self.limit_current = create_label(self.control_page_original, '钳位电流(A)', 350, 50)
        self.limit_current_param = create_entry1(self.control_page_original, 10, 470, 50, 1e-4)

        self.channel_index = create_label(self.control_page_original, '通道：', 570, 50)
        options2 = ['a', 'b']

        self.combo2 = ttk.Combobox(self.control_page_original, values=options2, width=5, state='readonly')
        self.combo2.current(0)  # 设置默认选中的项
        self.combo2.bind("<<ComboboxSelected>>", lambda event: self.on_select(2))
        self.combo2.place(x=630, y=50)

        # 标题与手动检查开关
        self.check_param = tk.Label(self.check_panel, text='芯片检查参数', bg='#e5e5e5', font=("SimHei", 13))
        self.check_param.grid(row=0, column=0, sticky='w', padx=4, pady=(4, 2))
        self.canvas_manual_check = tk.Canvas(self.check_panel, width=24, height=24, bg='#e5e5e5', bd=0)
        self.canvas_manual_check.grid(row=0, column=2, sticky='e', padx=2, pady=(4, 2))
        self.draw_triangle('check', self.canvas_manual_check)
        self.canvas_manual_check.tag_bind('shape', '<Button-1>',
                                          lambda event: self.toggle_shape('check', self.canvas_manual_check))

        # 扫描电压
        self.check_volt = tk.Label(self.check_panel, text='扫描电压(V)', bg='#e5e5e5', font=("SimHei", 11))
        self.check_volt.grid(row=1, column=0, sticky='w', padx=4, pady=8)
        self.check_volt_param = tk.Entry(self.check_panel, width=10, justify='center')
        self.check_volt_param.grid(row=1, column=1, sticky='ew', padx=6, pady=8)
        self.check_volt_param.insert(0, 0.1)

        # 平均阻抗
        self.check_r1 = tk.Label(self.check_panel, text='平均阻抗（Ω）', bg='#e5e5e5', font=("SimHei", 11))
        self.check_r1.grid(row=2, column=0, sticky='w', padx=4, pady=8)
        self.check_r_entry = tk.Entry(self.check_panel, width=10, justify='center')
        self.check_r_entry.grid(row=2, column=1, sticky='ew', padx=6, pady=8)
        self.check_r_entry.insert(0, '#')
        self.check_r_entry.config(state=tk.DISABLED)

        self.canvas_sq_line_make = tk.Canvas(self.control_page_original, width=310, height=210, bg='#e5e5e5', bd=0)
        self.canvas_sq_line_make.place(x=624, y=80)
        self.create_dashed_square(self.canvas_sq_line_make, 3, 3, 310, 210, 1)

        # 打孔参数子面板
        self.make_panel = tk.Frame(self.control_page_original, width=304, height=204, bg='#e5e5e5')
        self.make_panel.place(x=627, y=83)
        self.make_panel.grid_columnconfigure(1, weight=1)

        self.make_param = tk.Label(self.make_panel, text='打孔参数', bg='#e5e5e5', font=("SimHei", 13))
        self.make_param.grid(row=0, column=0, sticky='w', padx=6, pady=(6, 2))
        self.canvas_manual_make = tk.Canvas(self.make_panel, width=31, height=31, bg='#e5e5e5', bd=0)
        self.canvas_manual_make.grid(row=0, column=2, sticky='e', padx=2, pady=(4, 2))
        self.draw_triangle('make', self.canvas_manual_make)
        self.canvas_manual_make.tag_bind('shape', '<Button-1>',
                                         lambda event: self.toggle_shape('make', self.canvas_manual_make))

        self.make_volt = tk.Label(self.make_panel, text='制造电压(V)', bg='#e5e5e5', font=("SimHei", 11))
        self.make_volt.grid(row=1, column=0, sticky='w', padx=6, pady=8)
        self.make_volt_param = tk.Entry(self.make_panel, width=10, justify='center')
        self.make_volt_param.grid(row=1, column=1, sticky='ew', padx=6, pady=8)
        self.make_volt_param.insert(0, 19)

        self.make_solution = tk.Label(self.make_panel, text='溶液电导率(S/m)', bg='#e5e5e5', font=("SimHei", 11))
        self.make_solution.grid(row=2, column=0, sticky='w', padx=6, pady=8)
        self.make_solution_param = tk.Entry(self.make_panel, width=10, justify='center')
        self.make_solution_param.grid(row=2, column=1, sticky='ew', padx=6, pady=8)
        self.make_solution_param.insert(0, 11.27)

        self.make_SiN = tk.Label(self.make_panel, text='氮化硅膜厚(nm)', bg='#e5e5e5', font=("SimHei", 11))
        self.make_SiN.grid(row=3, column=0, sticky='w', padx=6, pady=8)
        self.make_SiN_param = tk.Entry(self.make_panel, width=10, justify='center')
        self.make_SiN_param.grid(row=3, column=1, sticky='ew', padx=6, pady=8)
        self.make_SiN_param.insert(0, 30)

        self.canvas_sq_line_expand = tk.Canvas(self.control_page_original, width=310, height=210, bg='#e5e5e5', bd=0)
        self.canvas_sq_line_expand.place(x=936, y=80)
        self.create_dashed_square(self.canvas_sq_line_expand, 3, 3, 310, 210, 1)

        # 扩孔参数子面板
        self.expand_panel = tk.Frame(self.control_page_original, width=304, height=204, bg='#e5e5e5')
        self.expand_panel.place(x=939, y=83)
        self.expand_panel.grid_columnconfigure(1, weight=1)

        self.expand_param = tk.Label(self.expand_panel, text='扩孔参数', bg='#e5e5e5', font=("SimHei", 13))
        self.expand_param.grid(row=0, column=0, sticky='w', padx=6, pady=(6, 2))
        self.canvas_manual_expand = tk.Canvas(self.expand_panel, width=31, height=31, bg='#e5e5e5', bd=0)
        self.canvas_manual_expand.grid(row=0, column=2, sticky='e', padx=2, pady=(4, 2))
        self.draw_triangle('expand', self.canvas_manual_expand)
        self.canvas_manual_expand.tag_bind('shape', '<Button-1>',
                                           lambda event: self.toggle_shape('expand', self.canvas_manual_expand))

        self.expand_volt = tk.Label(self.expand_panel, text='扩孔电压(V)', bg='#e5e5e5', font=("SimHei", 11))
        self.expand_volt.grid(row=1, column=0, sticky='w', padx=6, pady=8)
        self.expand_volt_param = tk.Entry(self.expand_panel, width=10, justify='center')
        self.expand_volt_param.grid(row=1, column=1, sticky='ew', padx=6, pady=8)
        self.expand_volt_param.insert(0, 5)

        self.pore_target = tk.Label(self.expand_panel, text='目标孔径（nm）', bg='#e5e5e5', font=("SimHei", 11))
        self.pore_target.grid(row=2, column=0, sticky='w', padx=6, pady=8)
        self.pore_target_param = tk.Entry(self.expand_panel, width=10, justify='center')
        self.pore_target_param.grid(row=2, column=1, sticky='ew', padx=6, pady=8)
        self.pore_target_param.insert(0, 50)

        self.canvas_sq_line_process = tk.Canvas(self.control_page_original, width=310, height=210, bg='#e5e5e5', bd=0)
        self.canvas_sq_line_process.place(x=1248, y=80)
        self.create_dashed_square(self.canvas_sq_line_process, 3, 3, 310, 210, 1)

        # 流程状态子面板
        self.process_panel = tk.Frame(self.control_page_original, width=304, height=204, bg='#e5e5e5')
        self.process_panel.place(x=1251, y=83)
        self.process_panel.grid_columnconfigure(0, weight=1)

        self.check_process = tk.Label(self.process_panel, text='密封性检查', bg='#e5e5e5', font=("SimHei", 13))
        self.check_process.grid(row=0, column=0, sticky='w', padx=6, pady=(6, 2))
        self.make_process = tk.Label(self.process_panel, text='CBD打孔', bg='#e5e5e5', font=("SimHei", 13))
        self.make_process.grid(row=1, column=0, sticky='w', padx=6, pady=6)
        self.expand_process = tk.Label(self.process_panel, text='电压扩孔', bg='#e5e5e5', font=("SimHei", 13))
        self.expand_process.grid(row=2, column=0, sticky='w', padx=6, pady=6)
        self.edd_process = tk.Label(self.process_panel, text='制造完成', bg='#e5e5e5', font=("SimHei", 13))
        self.edd_process.grid(row=3, column=0, sticky='w', padx=6, pady=6)

        # 右侧流程指示灯放入子面板
        self.creat_color_button(2, self.process_panel, 230, 0)
        self.creat_color_button(3, self.process_panel, 230, 50)
        self.creat_color_button(4, self.process_panel, 230, 100)
        self.creat_color_button(5, self.process_panel, 230, 150)
        self.plot_VIR_sigma()

        self.toolbar_current = NavigationToolbar2Tk(self.canvas_plotVIR_sigma, self.control_page_original)
        self.toolbar_current.update()
        self.toolbar_current.place(x=0, y=960)

        self.plot_check()
        self.plot_expand_pore()

    '''数据采集(eONE)'''

    def bc_data_10_func(self, original_list):
        indexed_data = [(value, idx) for idx, value in enumerate(original_list)]
        data_length = len(indexed_data)
        remove_count = int(data_length * 0.1)

        if remove_count == 0:
            return original_list.copy()

        sorted_data = sorted(indexed_data, key=lambda x: x[0])
        left_removed_indices = [idx for _, idx in sorted_data[:remove_count]]
        middle_valid = sorted_data[remove_count: data_length - remove_count]
        right_removed_indices = [idx for _, idx in sorted_data[data_length - remove_count:]]

        middle_values = [val for val, _ in middle_valid]
        left_fill_values = middle_values[:remove_count]
        right_fill_values = middle_values[-remove_count:]

        result_map = {}

        for val, idx in middle_valid:
            result_map[idx] = val

        for fill_val, original_idx in zip(left_fill_values, left_removed_indices):
            result_map[original_idx] = fill_val

        for fill_val, original_idx in zip(right_fill_values, right_removed_indices):
            result_map[original_idx] = fill_val

        processed_list = [result_map[idx] for idx in range(data_length)]
        return processed_list
    def b_show_table_tk(self):
        self.table_frame = tk.Frame(
            self.collection_eONE_page_original,
            width=800,
            height=600,
            bg='#e5e5e5',
            relief=tk.SUNKEN,
            bd=2
        )
        self.table_frame.place(x=312, y=470)

        style = ttk.Style()

        style.theme_use('default')

        style.configure(
            "Custom.Treeview",
            background="#e5e5e5",
            fieldbackground="#e5e5e5",
        )

        style.configure(
            "Custom.Treeview.Heading",
            background="#e5e5e5",
        )

        style.map(
            "Custom.Treeview",
            background=[('selected', 'skyblue')]
        )

        self.tree = ttk.Treeview(
            self.table_frame,
            columns=[str(i) for i in range(len(self.header_list))],
            show='headings',
            height=14,
            style="Custom.Treeview"
        )

        for i in range(len(self.header_list)):
            self.tree.heading(str(i), text=f'{self.header_list[i]}')
            self.tree.column(str(i), width=137, anchor='center')

        scrollbar = ttk.Scrollbar(self.table_frame, orient=tk.VERTICAL, command=self.tree.yview)
        self.tree.configure(yscrollcommand=scrollbar.set)

        self.tree.grid(row=0, column=0, sticky='nsew')
        scrollbar.grid(row=0, column=1, sticky='ns')

        self.table_frame.grid_rowconfigure(0, weight=1)
        self.table_frame.grid_columnconfigure(0, weight=1)

        self.tree.bind("<ButtonRelease-1>", self.b_on_item_click)

    def b_disconnect_devices(self):
        c4.disconnect()
        print(1)
        self.b_connect_stats = 0
        self.update_color_button(6, 'red')
    def b_connect_devices(self, number):
        err, self.b_devices = c4.detectDevices()
        print(err)
        if err != c4.Success:
            sys.exit()
        err = c4.connect(self.b_devices)
        if err != c4.Success:
            sys.exit()
        else:
            self.update_color_button(6, 'green')
            e, self.b_v_channels, self.b_i_channels, gp_channels = c4.getChannelsNumber()
            c4.setCurrentRange(0, self.b_i_channels)
            c4.setSamplingRate(0)

            e, names, images, voltages, times, slopes, frequencies, adimensionals = c4.getProtocolList()

            e, v_names, v_ranges, default_values = c4.getProtocolVoltage()
            c4.digitalOffsetCompensation(self.b_i_channels, True)
            time.sleep(1)

            c4.digitalOffsetCompensation(self.b_i_channels, False)
            c4.selectVoltageProtocol(0)

            v_hold = c4.Measurement(0, c4.UnitPfxMilli, "V")
            c4.setProtocolVoltage(0, v_hold)

            v_pulse = c4.Measurement(1000.0, c4.UnitPfxMilli, "V")
            c4.setProtocolVoltage(1, v_pulse)

            v_step = c4.Measurement(2000.0, c4.UnitPfxMilli, "V")
            c4.setProtocolVoltage(2, v_step)

            t_hold = c4.Measurement(500.0, c4.UnitPfxMilli, "s")
            c4.setProtocolTime(0, t_hold)

            t_pulse = c4.Measurement(40.0, c4.UnitPfxMilli, "s")
            c4.setProtocolTime(1, t_pulse)

            adim_n = c4.Measurement(10.0, c4.UnitPfxNone, "")
            c4.setProtocolAdimensional(0, adim_n)

            adim_n_r = c4.Measurement(0.0, c4.UnitPfxNone, "")
            c4.setProtocolAdimensional(1, adim_n_r)

            if c4.applyVoltageProtocol() != c4.Success:
                print("无法应用协议")
                c4.disconnect()
                sys.exit()
            _, self.b_i_range = c4.getCurrentRange(0)
            _, self.b_v_range = c4.getVoltageRange()

            self.i_m = self.b_i_range.step
            self.v_m = self.b_v_range.step

            c4.purgeData()

            self.b_total_channels = self.b_v_channels + self.b_i_channels + gp_channels
            ad_rdy_ev = threading.Event()
            ad_rdy_ev.set()  # 线程运行
            t = threading.Thread(target=self.b_start_get_date)
            t.daemon = True
            t.start()

    def b_start_get_date(self):
        # self.b_fig, (self.b_ax1, self.b_ax2) = plt.subplots(2, 1, figsize=(11.0, 5.5))
        # self.b_line1, = self.b_ax1.plot([], [], lw=2)
        # self.b_line2, = self.b_ax2.plot([], [], lw=2)
        self.canvas_current2.destroy()
        self.canvas_current2 = None
        self.canvas_current = None
        self.ax4 = None
        self.b_canvas = FigureCanvasTkAgg(self.b_fig, master=self.collection_picture_page_original)
        self.b_canvas.draw()
        self.b_canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)
        while True:
            # time.sleep(0.001)
            e, q = c4.getQueueStatus()
            available_data = q.availableDataPackets
            if available_data > 0:
                # with self.b_data_lock:  # 锁保护
                    while_time = self.b_end_time
                    read_error, data = c4.readData(available_data)
                    self.data_processing_thread(data, while_time)
                    # self.std(data)
                    if self.b_number == 0:

                        ad_rdy_ev = threading.Event()
                        ad_rdy_ev.set()
                        t = threading.Thread(target=self.b_update_plot,
                                             )
                        t.daemon = True
                        t.start()
                        self.threads.append(t)
                        # self.update_plot()
                        self.b_number += 1
            time.sleep(0.0001)


    def b_update_plot(self):
        global data_lock
        with data_lock:
            self.b_line1.set_xdata(self.b_t_data)
            self.b_line1.set_ydata(self.b_i_data[:len(self.b_t_data)])
            # self.b_line2.set_xdata(self.b_t_data)
            # self.b_line2.set_ydata(self.b_v_data[:len(self.b_t_data)])

            if self.b_detection_param is False:
                for line_name in ['b_line3', 'b_line4', 'b_line5']:
                    if hasattr(self, line_name) and getattr(self, line_name) is not None:
                        getattr(self, line_name).remove()
                        setattr(self, line_name, None)
            else:
                if not hasattr(self, 'b_line3') or self.b_line3 is None:
                    self.b_line3, = self.b_ax1.plot([], [], 'r--')
                if self.b_line3 is not None:
                    self.b_line3.set_xdata(self.b_t_data_data_4)
                    self.b_line3.set_ydata(self.b_std_data_down[:len(self.b_t_data_data_4)])

                if not hasattr(self, 'b_line4') or self.b_line4 is None:
                    self.b_line4, = self.b_ax1.plot([], [], 'r--')
                if self.b_line4 is not None:
                    self.b_line4.set_xdata(self.b_t_data_data_4)
                    self.b_line4.set_ydata(self.b_std_data_up[:len(self.b_t_data_data_4)])

                if not hasattr(self, 'b_line5') or self.b_line5 is None:
                    self.b_line5, = self.b_ax1.plot([], [], 'k--')
                if self.b_line5 is not None:
                    self.b_line5.set_xdata(self.b_t_data_data_4)
                    self.b_line5.set_ydata(self.b_baseline_data[:len(self.b_t_data_data_4)])

        # self.b_ax1.relim()
        # self.b_ax1.autoscale_view()
        self.b_ax1.set_ylim(min(self.b_i_data) -1, max(self.b_i_data)+1)
        self.b_ax1.set_xlim(self.b_t_data[0], self.b_t_data[-1])
        # self.b_ax2.relim()
        # self.b_ax2.autoscale_view()
        self.b_canvas.draw_idle()

        # # self.ax1.set_ylim(1.5 * min(self.i_data), 1.5 * max(self.i_data))
        # self.b_ax2.set_xlim(self.b_t_data[-1] - 3, self.b_t_data[-1] + 1)
        # self.b_ax1.axhline(y=np.mean(self.b_i_data[-100: -1]), linestyle='--', xmin=0.8, xmax=1.0, clip_on=False)
        # self.ax2.axhline(y=np.mean(self.v_data[-100: -1]), linestyle='--', xmin=0.9, xmax=1.0, clip_on=False)
        self.update_baseline_std_vsm()
        self.b_canvas.get_tk_widget().after(500, self.b_update_plot)

    def update_baseline_std_vsm(self):
        global data_lock1
        with data_lock1:
            std_number = dynamic_detection.calculate_dynamic_std_py(self.b_i_data_while, 0.73 * len(self.b_i_data_while))
            baseline_number = dynamic_detection.calculate_dynamic_baseline_fast_trimmed_py(self.b_i_data_while, 0.73 * len(self.b_i_data_while))
            cpp_result = dynamic_rms.calculate_dynamic_rms_py(self.b_i_data_while, 0.1 * len(self.b_i_data_while))
            # self.b_display_std.config(text=f'标准差：{round(float(std_number[-1]), 4)}')
            self.b_display_rms.config(text=f'   {round(float(std_number[-1]), 4)}{self.b_last_current_range_selected[-2:]}')
            self.b_display_mean_baseline.config(text=f'基线：{round(float(baseline_number[-1]), 4)}')
    def integral(self, y, baseline, sampling, direction):
        integral_area = 0
        if direction == 1:
            for i in range(len(y) - 1):
                if i >= 0 and i + 1 < len(y):
                    area = (1 / 2) * (max(y[i], y[i + 1]) - min(y[i], y[i + 1])) * (1 / sampling) + (
                            (min(y[i], y[i + 1]) - baseline) * (1 / sampling))
                    integral_area += area
                else:
                    pass
        else:
            for i in range(len(y) - 1):
                if i >= 0 and i + 1 < len(y):
                    area = (1 / 2) * (max(y[i], y[i + 1]) - min(y[i], y[i + 1])) * (1 / sampling) + (
                            (baseline - min(y[i], y[i + 1])) * (1 / sampling))
                    integral_area += area
                else:
                    pass
        return np.abs(integral_area)
    def event_detection_two(self, input_list_x, input_list_y, th_up_index, th_down_index):
        # return True, True
        integral_area_all = np.sum(np.square(input_list_y))
        th_up_index =  0.00005
        two_decay = 0.000005
        amplitude_one = 0.1
        # coefficient_amplitude = 0.2  #
        # cutoff_frequency = 1000
        # nyquist_frequency = 0.5 * self.c_frequency
        # normal_cutoff = cutoff_frequency / nyquist_frequency
        # b, a = signal.butter(1, normal_cutoff, btype='low', analog=False)
        # input_list_y = signal.filtfilt(b, a, input_list_y)
        y = input_list_y
        # y = savgol_filter(y, 5, 3)
        x = input_list_x
        filter_y = y
        mean_start = np.mean(filter_y)
        start_list = []
        end_list = []
        delta_current = y[0] * 0.005
        down_pulse = False
        down_start_index = 0
        down_end_index = 0
        down_end_number = 0
        down_start_number = 0
        for i in range(len(filter_y)):
            # print(filter_y[i],((mean_start - (delta_current))), mean_start)
            if filter_y[i] < (mean_start - (delta_current)) and not down_pulse:
                # print(333)
                down_pulse = True
                down_start_index = i
                down_start_number += 1
                # print(f'起点{down_start_index}')
                # start_list.append(i)
            elif down_pulse == True:
                delta_current = filter_y[down_start_index] - min(y[down_start_index: i + 1])
                mean_slide = np.mean(filter_y[down_start_index:i + 1])
                # print(filter_y[i], (mean_slide + (th_up_index * delta_current)))
                if filter_y[i] >= (mean_slide + (th_up_index * delta_current)):
                    down_end_index = i
                    down_end_number += 1
                    integral_area_amplitude = np.sum(np.square(filter_y[down_start_index:down_end_index]))

                    # print(f'终点{down_end_index}')
                    down_pulse = False
                    if  1 / 5 * (x[-1] - x[0]) > x[down_end_index] - x[down_start_index] > two_decay and min(
                            y[down_start_index: down_end_index]) < mean_start - (delta_current) and (
                            y[down_start_index] - min(y[down_start_index: down_end_index])) > amplitude_one * (
                            y[0] - y[down_start_index]) and integral_area_amplitude / integral_area_all < 0.13:
                        start_list.append(down_start_index)
                        end_list.append(down_end_index)
                        delta_current = max(y[down_end_index: i + 1]) - filter_y[down_end_index]
                        mean_start = np.mean(filter_y[down_end_index: i + 1])
            # delta_current = filter_y[down_start_index] - min(y[down_start_index: i +1])
        # print(len(start_list), len(end_list),down_end_index)

        if len(end_list) > 0:
            return True, True
        else:
            return False, False
    # def event_detection_two(self, input_list, signal_1, time):
    #     if  50 > time > 0.00001:
    #         # cutoff_frequency = 1000  # 截止频率
    #         # nyquist_frequency = 0.5 * self.c_frequency  # 奈奎斯特频率
    #         # normal_cutoff = cutoff_frequency / nyquist_frequency
    #         # b, a = signal.butter(1, normal_cutoff, btype='low', analog=False)
    #         # input_list = signal.filtfilt(b, a, input_list)
    #         # input_list = savgol_filter(input_list, 20, 3)
    #         down = False
    #         up = False
    #         list_len = len(input_list)
    #         sorted_list = sorted(input_list)
    #         start_idx = int(np.ceil(list_len * 0.40))
    #         end_idx = int(np.ceil(list_len * 0.60))
    #         target_elements = sorted_list[start_idx:end_idx]
    #         mean = [sum(target_elements) / len(target_elements)] * len(input_list)
    #         baseline = [mean] * len(input_list)
    #         std_value = input_list[0] - sum(target_elements) / len(target_elements)
    #         # squared_diff_sum = sum([(x - sum(target_elements) / len(target_elements)) ** 2 for x in target_elements])
    #         # variance = squared_diff_sum / len(target_elements)
    #         # std_value = math.sqrt(variance)
    #         std_value_up = [sum(target_elements) / len(target_elements) + std_value * float(self.b_amplitude_two.get())] * len(input_list)
    #         std_value_down = [sum(target_elements) / len(target_elements) - std_value * float(self.b_amplitude_two.get())] * len(input_list)
    #         if signal_1 == 2:
    #             if min(input_list) <= std_value_down[0]:
    #                 down = True
    #
    #         elif signal_1 == 1:
    #             if max(input_list) <= std_value_up[0]:
    #                 up = True
    #         else:
    #             down = False
    #             up = False
    #         # std = sum(squared_diff_sum) / len(squared_diff_sum)
    #     else:
    #         down = False
    #         up = False
    #     return down, up
    def event_detection(self, data_x, data_y, baseline_data, std_up_data, std_down_data, signal_direction, sampling,
                        window_size, min_amplitude, min_duration, max_duration, event_number):
        x_len = 0
        min_duration  = min_duration / 1000
        max_duration = max_duration / 1000
        event_list = defaultdict(list)
        last_start_index = 0
        current_pos = 0
        data_len = len(data_x)

        while current_pos < data_len - 1:
            rise = 0
            decay = 0
            peak_value = 0
            peak_index = 0
            search_end = min(current_pos + int((window_size / 1000) * sampling), data_len)
            current_window = data_y[current_pos:search_end]
            current_window_x = data_x[current_pos:search_end]
            if signal_direction == 1:
                peak_value = max(current_window)
                peak_idx_in_window = np.where(current_window == peak_value)[0][0]
                peak_index = current_pos + peak_idx_in_window
            if signal_direction == 2:
                peak_value = min(current_window)
                peak_idx_in_window = np.where(current_window == peak_value)[0][0]
                peak_index = current_pos + peak_idx_in_window
            else:
                peak_value_max = max(current_window)
                peak_idx_in_window_max = np.where(current_window == peak_value_max)[0][0]
                peak_index_max = current_pos + peak_idx_in_window_max
                peak_value_min = min(current_window)
                peak_idx_in_window_min = np.where(current_window == peak_value_min)[0][0]
                peak_index_min = current_pos + peak_idx_in_window_min

            if signal_direction == 1:
                if peak_value > std_up_data[peak_index]:
                    event_start_index = None
                    for index_1 in range(peak_index, max(current_pos - 10000, 0), -1):
                        if data_y[index_1] >= baseline_data[index_1] + (peak_value - baseline_data[index_1]) * 0.1:
                            rise = data_x[peak_index] - data_x[index_1]
                            event_start_index = index_1
                            break
                    if event_start_index is None:
                            current_pos = peak_index + 1
                            continue
                    event_end_index = None
                    for index_2 in range(peak_index, min(peak_index + 10000, data_len)):
                        if data_y[index_2] >= baseline_data[index_2] + (peak_value - baseline_data[index_2]) * 0.1:
                            decay = data_x[index_2] - data_x[peak_index]
                            event_end_index = index_2 + 1
                            break
                    if event_end_index is None:
                        current_pos = peak_index + 1
                        continue
                    amplitude = peak_value - baseline_data[event_start_index]
                    total_duration = rise + decay
                    if (amplitude >= min_amplitude and
                            min_duration <= total_duration <= max_duration and
                            event_start_index != last_start_index):
                        x1 = data_x[event_start_index: event_end_index]
                        x_number = [round(num, 6) for num in x1]
                        y1 = data_y[event_start_index: event_end_index]
                        y_number = [round(num, 6) for num in y1]
                        integral_area = self.integral(y_number, baseline_data[event_start_index], sampling, 1)
                        x2 = data_x[event_start_index]
                        y2 = baseline_data[event_start_index]
                        heng = 0
                        zong = 0
                        theta1 = None
                        for i1 in range(event_start_index, event_end_index):
                            if data_y[i1] >= baseline_data[event_start_index] + 0.7 * amplitude:
                                theta1 = data_x[i1]
                                break
                        theta2 = None
                        for i2 in range(event_end_index - 1, event_start_index - 1, -1):
                            if data_y[i2] >= baseline_data[event_start_index] + 0.7 * amplitude:
                                theta2 = data_x[i2]
                                break
                        theta_duration = (theta2 - theta1) * 1000 if (theta1 and theta2) else 0
                        for i3 in range(event_start_index, event_end_index - 1):
                            heng += (data_x[i3] - x2) ** 2 * (data_y[i3] - y2)
                        for i4 in range(event_start_index, peak_index):
                            k = peak_index
                            for j1 in range(peak_index, event_end_index - 1):
                                if data_y[j1] >= data_y[i4]:
                                    k = j1
                                    break
                            zong += (data_y[i4] - y2) ** 2 * (data_x[k] - data_x[i4])
                        for i5 in range(peak_index, event_end_index - 1):
                            k = event_start_index
                            for j2 in range(event_start_index, peak_index):
                                if data_y[j2] >= data_y[i5]:
                                    k = j2
                                    break
                            zong += (data_y[i5] - y2) ** 2 * (data_x[i5] - data_x[k])
                        rise_integral = self.integral(data_y[event_start_index:peak_index],
                                                      baseline_data[event_start_index], sampling, 1)
                        decay_integral = self.integral(data_y[peak_index:event_end_index],
                                                       baseline_data[event_start_index],
                                                       sampling, 1)
                        integral_ratio = rise_integral / decay_integral if decay_integral != 0 else 0
                        event_list[event_number] += [event_number + 1, f'{baseline_data[index_1]:.4e}',
                                                     f'{(peak_value - baseline_data[index_1]):.4e}',
                                                     f'{(integral_area * 1e9):.4e}',
                                                     float(f'{((rise + decay + 1 / sampling) * 1000):.4e}'),
                                                     theta_duration,
                                                     integral_ratio, np.abs(heng), np.abs(zong), x_number, y_number]
                        current_pos = event_end_index
                        last_start_index = event_start_index
                        event_number += 1
                    else:
                        current_pos = peak_index + 1
                else:
                    current_pos = search_end
            elif signal_direction == 2:
                if peak_value < std_down_data[peak_index]:
                    event_start_index = None
                    for index_1 in range(peak_index, max(current_pos - 10000, 0), -1):
                        if data_y[index_1] >= baseline_data[index_1] - (baseline_data[index_1] - peak_value) * 0.1:
                            rise = data_x[peak_index] - data_x[index_1]
                            event_start_index = index_1
                            break
                    if event_start_index is None:
                            current_pos = peak_index + 1
                            continue
                    event_end_index = None
                    for index_2 in range(peak_index, min(peak_index + 10000, data_len)):
                        if data_y[index_2] >= baseline_data[index_2] - (baseline_data[index_2] - peak_value) * 0.1:
                            decay = data_x[index_2] - data_x[peak_index]
                            event_end_index = index_2 + 1
                            break
                    if event_end_index is None:
                        current_pos = peak_index + 1
                        continue
                    amplitude = baseline_data[event_start_index] - peak_value

                    total_duration = rise + decay
                    if (amplitude >= min_amplitude and
                            min_duration <= total_duration <= max_duration and
                            event_start_index != last_start_index):
                        x1 = data_x[event_start_index: event_end_index]
                        x_number = [round(num, 6) for num in x1]
                        y1 = data_y[event_start_index: event_end_index]
                        y_number = [round(num, 6) for num in y1]
                        integral_area = self.integral(y_number, baseline_data[event_start_index], sampling, 0)
                        x2 = data_x[event_start_index]
                        y2 = baseline_data[event_start_index]
                        heng = 0
                        zong = 0
                        theta1 = None
                        for i1 in range(event_start_index, event_end_index):
                            if data_y[i1] <= baseline_data[event_start_index] + 0.7 * amplitude:
                                theta1 = data_x[i1]
                                break
                        theta2 = None
                        for i2 in range(event_end_index - 1, event_start_index - 1, -1):
                            if data_y[i2] <= baseline_data[event_start_index] + 0.7 * amplitude:
                                theta2 = data_x[i2]
                                break
                        theta_duration = (theta2 - theta1) * 1000 if (theta1 and theta2) else 0
                        for i3 in range(event_start_index, event_end_index - 1):
                            heng += (data_x[i3] - x2) ** 2 * (data_y[i3] - y2)
                        for i4 in range(event_start_index, peak_index):
                            k = peak_index
                            for j1 in range(peak_index, event_end_index - 1):
                                if data_y[j1] >= data_y[i4]:
                                    k = j1
                                    break
                            zong += (data_y[i4] - y2) ** 2 * (data_x[k] - data_x[i4])
                        for i5 in range(peak_index, event_end_index - 1):
                            k = event_start_index
                            for j2 in range(event_start_index, peak_index):
                                if data_y[j2] >= data_y[i5]:
                                    k = j2
                                    break
                            zong += (data_y[i5] - y2) ** 2 * (data_x[i5] - data_x[k])
                        rise_integral = self.integral(data_y[event_start_index:peak_index],
                                                      baseline_data[event_start_index], sampling, 0)
                        decay_integral = self.integral(data_y[peak_index:event_end_index],
                                                       baseline_data[event_start_index],
                                                       sampling, 0)
                        integral_ratio = rise_integral / decay_integral if decay_integral != 0 else 0
                        event_list[event_number] += [event_number + 1, f'{baseline_data[index_1]:.4e}',
                                                     f'{(baseline_data[index_1] - peak_value):.4e}',
                                                     f'{(integral_area * 1e9):.4e}',
                                                     float(f'{((rise + decay + 1 / sampling) * 1000):.4e}'),
                                                     theta_duration,
                                                     integral_ratio, np.abs(heng), np.abs(zong), x_number, y_number]
                        current_pos = event_end_index
                        last_start_index = event_start_index
                        event_number += 1
                        x_len += len(x_number)
                    else:
                        current_pos = peak_index + 1
                else:
                    current_pos = search_end

            else:
                if peak_value_max > std_up_data[peak_index_max]:
                    event_start_index = None
                    for index_1 in range(peak_index_max, max(current_pos - 10000, 0), -1):
                        if data_y[index_1] <= baseline_data[index_1] + (peak_value_max - baseline_data[index_1]) * 0.1:
                            rise = data_x[peak_index_max] - data_x[index_1]
                            event_start_index = index_1
                            break
                    if event_start_index is None:
                            current_pos = peak_index_max + 1
                            continue
                    event_end_index = None
                    for index_2 in range(peak_index_max, min(peak_index_max + 10000, data_len)):
                        if data_y[index_2] <= baseline_data[index_2] + (peak_value_max - baseline_data[index_2]) * 0.1:
                            decay = data_x[index_2] - data_x[peak_index_max]
                            event_end_index = index_2 + 1
                            break
                    if event_end_index is None:
                        current_pos = peak_index_max + 1
                        continue
                    amplitude = peak_value_max - baseline_data[event_start_index]
                    total_duration = rise + decay
                    if (amplitude >= min_amplitude and
                            min_duration <= total_duration <= max_duration and
                            event_start_index != last_start_index):
                        x1 = data_x[event_start_index: event_end_index]
                        x_number = [round(num, 6) for num in x1]
                        y1 = data_y[event_start_index: event_end_index]
                        y_number = [round(num, 6) for num in y1]
                        integral_area = self.integral(y_number, baseline_data[event_start_index], sampling, 1)
                        x2 = data_x[event_start_index]
                        y2 = baseline_data[event_start_index]
                        heng = 0
                        zong = 0
                        theta1 = None
                        for i1 in range(event_start_index, event_end_index):
                            if data_y[i1] >= baseline_data[event_start_index] + 0.7 * amplitude:
                                theta1 = data_x[i1]
                                break
                        theta2 = None
                        for i2 in range(event_end_index - 1, event_start_index - 1, -1):
                            if data_y[i2] >= baseline_data[event_start_index] + 0.7 * amplitude:
                                theta2 = data_x[i2]
                                break
                        theta_duration = (theta2 - theta1) * 1000 if (theta1 and theta2) else 0
                        for i3 in range(event_start_index, event_end_index - 1):
                            heng += (data_x[i3] - x2) ** 2 * (data_y[i3] - y2)
                        for i4 in range(event_start_index, peak_index_max):
                            k = peak_index_max
                            for j1 in range(peak_index_max, event_end_index - 1):
                                if data_y[j1] >= data_y[i4]:
                                    k = j1
                                    break
                            zong += (data_y[i4] - y2) ** 2 * (data_x[k] - data_x[i4])
                        for i5 in range(peak_index_max, event_end_index - 1):
                            k = event_start_index
                            for j2 in range(event_start_index, peak_index_max):
                                if data_y[j2] >= data_y[i5]:
                                    k = j2
                                    break
                            zong += (data_y[i5] - y2) ** 2 * (data_x[i5] - data_x[k])
                        rise_integral = self.integral(data_y[event_start_index:peak_index_max],
                                                      baseline_data[event_start_index], sampling, 1)
                        decay_integral = self.integral(data_y[peak_index_max:event_end_index],
                                                       baseline_data[event_start_index],
                                                       sampling, 1)
                        integral_ratio = rise_integral / decay_integral if decay_integral != 0 else 0
                        event_list[event_number] += [event_number + 1, f'{baseline_data[index_1]:.4e}',
                                                     f'{(peak_value_max - baseline_data[index_1]):.4e}',
                                                     f'{(integral_area * 1e9):.4e}',
                                                     float(f'{((rise + decay + 1 / sampling) * 1000):.4e}'),
                                                     theta_duration,
                                                     integral_ratio, np.abs(heng), np.abs(zong), x_number, y_number]
                        current_pos = event_end_index
                        last_start_index = event_start_index
                        event_number += 1
                    else:
                        current_pos = peak_index_max + 1

                if peak_value_min < std_down_data[peak_index_min]:
                    event_start_index = None
                    for index_1 in range(peak_index_min, max(current_pos - 10000, 0), -1):
                        if data_y[index_1] >= baseline_data[index_1] + (baseline_data[index_1] - peak_value_min) * 0.1:
                            rise = data_x[peak_index_min] - data_x[index_1]
                            event_start_index = index_1
                            break
                    if event_start_index is None:
                            current_pos = peak_index_min + 1
                            continue
                    event_end_index = None
                    for index_2 in range(peak_index_min, min(peak_index_min + 10000, data_len)):
                        if data_y[index_2] >= baseline_data[index_2] + (peak_value_min - baseline_data[index_2]) * 0.1:
                            decay = data_x[index_2] - data_x[peak_index_min]
                            event_end_index = index_2 + 1
                            break
                    if event_end_index is None:
                        current_pos = peak_index_min + 1
                        continue
                    amplitude = baseline_data[event_start_index] - peak_value_min
                    total_duration = rise + decay
                    if (amplitude >= min_amplitude and
                            min_duration <= total_duration <= max_duration and
                            event_start_index != last_start_index):
                        x1 = data_x[event_start_index: event_end_index]
                        x_number = [round(num, 6) for num in x1]
                        y1 = data_y[event_start_index: event_end_index]
                        y_number = [round(num, 6) for num in y1]
                        integral_area = self.integral(y_number, baseline_data[event_start_index], sampling, 0)
                        x2 = data_x[event_start_index]
                        y2 = baseline_data[event_start_index]
                        heng = 0
                        zong = 0
                        theta1 = None
                        for i1 in range(event_start_index, event_end_index):
                            if data_y[i1] <= baseline_data[event_start_index] + 0.7 * amplitude:
                                theta1 = data_x[i1]
                                break
                        theta2 = None
                        for i2 in range(event_end_index - 1, event_start_index - 1, -1):
                            if data_y[i2] <= baseline_data[event_start_index] + 0.7 * amplitude:
                                theta2 = data_x[i2]
                                break
                        theta_duration = (theta2 - theta1) * 1000 if (theta1 and theta2) else 0
                        for i3 in range(event_start_index, event_end_index - 1):
                            heng += (data_x[i3] - x2) ** 2 * (data_y[i3] - y2)
                        for i4 in range(event_start_index, peak_index_min):
                            k = peak_index_min
                            for j1 in range(peak_index_min, event_end_index - 1):
                                if data_y[j1] >= data_y[i4]:
                                    k = j1
                                    break
                            zong += (data_y[i4] - y2) ** 2 * (data_x[k] - data_x[i4])
                        for i5 in range(peak_index_min, event_end_index - 1):
                            k = event_start_index
                            for j2 in range(event_start_index, peak_index_min):
                                if data_y[j2] >= data_y[i5]:
                                    k = j2
                                    break
                            zong += (data_y[i5] - y2) ** 2 * (data_x[i5] - data_x[k])
                        rise_integral = self.integral(data_y[event_start_index:peak_index_min],
                                                      baseline_data[event_start_index], sampling, 0)
                        decay_integral = self.integral(data_y[peak_index_min:event_end_index],
                                                       baseline_data[event_start_index],
                                                       sampling, 0)
                        integral_ratio = rise_integral / decay_integral if decay_integral != 0 else 0
                        event_list[event_number] += [event_number + 1, f'{baseline_data[index_1]:.4e}',
                                                     f'{(baseline_data[index_1] - peak_value_min):.4e}',
                                                     f'{(integral_area * 1e9):.4e}',
                                                     float(f'{((rise + decay + 1 / sampling) * 1000):.4e}'),
                                                     theta_duration,
                                                     integral_ratio, np.abs(heng), np.abs(zong), x_number, y_number]
                        current_pos = event_end_index
                        last_start_index = event_start_index
                        event_number += 1
                    else:
                        current_pos = peak_index_min + 1
                else:
                    current_pos = search_end
        return event_list, event_number

    def integral(self, y, baseline, sampling, direction):
        integral_area = 0
        if direction == 1:
            for i in range(len(y) - 1):
                if i >= 0 and i + 1 < len(y):
                    area = (1 / 2) * (max(y[i], y[i + 1]) - min(y[i], y[i + 1])) * (1 / sampling) + (
                            (min(y[i], y[i + 1]) - baseline) * (1 / sampling))
                    integral_area += area
                else:
                    pass
        else:
            for i in range(len(y) - 1):
                if i >= 0 and i + 1 < len(y):
                    area = (1 / 2) * (max(y[i], y[i + 1]) - min(y[i], y[i + 1])) * (1 / sampling) + (
                            (baseline - min(y[i], y[i + 1])) * (1 / sampling))
                    integral_area += area
                else:
                    pass
        return np.abs(integral_area)

    def test_tree(self, data):
        item = self.tree.insert('', tk.END, values=data)
        if self.b_scroll_param == False:
            pass
        else:
            self.tree.see(item)
            # self.tree.update_idletasks()
            # 强制滚动
            # self.tree.yview_moveto(1.0)


    def save_event_list_to_csv(self, number, event_list, csv_file, number1):
        if number1 == 1:
            with open(csv_file, mode='a', newline='') as file:
                event_list_writer = [number, event_list[1], event_list[2],
                                     event_list[3], event_list[4], event_list[5],
                                     event_list[6]
                    , event_list[7], event_list[8], ', '.join(map(str, event_list[9])),
                                     ', '.join(map(str, event_list[10]))
                                     ]
                writer = csv.writer(file)
                writer.writerow(event_list_writer)
        else:
            with open(csv_file, mode='a', newline='') as file:
                event_list_writer = [number, event_list[1],
                                     event_list[2],
                                     event_list[3],
                                     event_list[4],
                                     event_list[5],
                                     event_list[6]
                    , event_list[7], event_list[8],
                                     ', '.join(map(str, event_list[9])),
                                     ', '.join(map(str, event_list[10]))
                                     ]
                writer = csv.writer(file)
                writer.writerow(event_list_writer)
    def data_processing_thread(self, data, while_time):
        global data_lock

        np_buffer = np.array(data, copy=False)
        data_matrix = np_buffer.reshape((-1, self.b_total_channels)).transpose()
        self.b_v_data_while = data_matrix[0].astype(np.int16) * self.v_m
        self.b_i_data_while = data_matrix[self.b_v_channels].astype(np.int16) * self.i_m

        if self.b_last_current_range_selected == '200 pA':
            pass
        if self.b_last_current_range_selected == '2 nA':
            self.b_i_data_while = [(ad / 100) for ad in self.b_i_data_while]
        if self.b_last_current_range_selected == '20 nA':
            self.b_i_data_while = [(ad / 10) for ad in self.b_i_data_while]
        if self.b_last_current_range_selected == '200 nA':
            pass


        if self.b_filter == False:
            pass
        else:
            cutoff_frequency = self.b_filter_parameter  # 截止频率
            nyquist_frequency = 0.5 * self.b_frequency  # 奈奎斯特频率
            normal_cutoff = cutoff_frequency / nyquist_frequency
            b, a = signal.butter(1, normal_cutoff, btype='low', analog=False)

            padding_length = 300
            padded = np.pad(self.b_i_data_while, (padding_length, padding_length), mode='symmetric')  # 镜像延拓

            filtered_padded = signal.filtfilt(b, a, padded)

            self.b_i_data_while = filtered_padded[padding_length:-padding_length]

        self.b_end_time = time.perf_counter()

        step = (self.b_end_time - while_time) / len(self.b_i_data_while)
        self.b_t_data_while = np.arange(while_time - self.b_start_time, self.b_end_time - self.b_start_time,
                                        step)
        if len(self.b_t_data_while) < len(self.b_i_data_while):
            self.b_i_data_while = self.b_i_data_while[:len(self.b_t_data_while)]
        elif len(self.b_t_data_while) > len(self.b_i_data_while):
            self.b_t_data_while = self.b_t_data_while[:len(self.b_i_data_while)]



        if len(self.b_i_data_cache) <= self.b_frequency * 1:
            self.b_i_data_cache = np.concatenate((self.b_i_data_cache, self.b_i_data_while))
            self.b_t_data_cache = np.concatenate((self.b_t_data_cache, self.b_t_data_while))
        else:
            self.b_std_data_while_down, self.b_std_data_while_up = np.array(
                dynamic_detection.detection_y_py(self.b_i_data_cache, (int(self.b_detection_baseline_std_window_param.get()) / 1000) * self.b_frequency,
                                                 self.b_signal_number))
            self.b_baseline_data_while = dynamic_detection.calculate_dynamic_baseline_fast_trimmed_py(
                self.b_i_data_cache,
                (int(self.b_detection_baseline_std_window_param.get()) / 1000) * self.b_frequency)
            with data_lock:
                if len(self.b_std_data_down) <= self.b_frequency * 5:
                    self.b_t_data_data_4 = np.concatenate((self.b_t_data_data_4, self.b_t_data_cache))
                    self.b_std_data_down = np.concatenate((self.b_std_data_down, self.b_std_data_while_down))
                    self.b_std_data_up = np.concatenate((self.b_std_data_up, self.b_std_data_while_up))
                    self.b_baseline_data = np.concatenate((self.b_baseline_data, self.b_baseline_data_while))
                else:
                    self.b_t_data_data_4 = np.concatenate((self.b_t_data_data_4, self.b_t_data_cache))
                    self.b_std_data_down = np.concatenate((self.b_std_data_down, self.b_std_data_while_down))
                    self.b_std_data_up = np.concatenate((self.b_std_data_up, self.b_std_data_while_up))
                    self.b_baseline_data = np.concatenate((self.b_baseline_data, self.b_baseline_data_while))
                    indices2 = list(range(len(self.b_t_data_cache)))
                    self.b_t_data_data_4 = np.delete(self.b_t_data_data_4, indices2)
                    self.b_std_data_down = np.delete(self.b_std_data_down, indices2)
                    self.b_std_data_up = np.delete(self.b_std_data_up, indices2)
                    self.b_baseline_data = np.delete(self.b_baseline_data, indices2)

            if self.b_start_detection_param == True:
                if self.b_start_time_1_number == True:
                    self.b_start_time_1 = time.perf_counter()
                    self.b_start_time_1_number = False
                self.start_csv_txt_thread(self.b_t_data_cache, self.b_i_data_cache, self.b_baseline_data_while,
                                          self.b_std_data_while_up,
                                          self.b_std_data_while_down)
            self.b_cache_number = True
            self.b_i_data_cache = np.array([])
            self.b_t_data_cache = np.array([])
            self.b_i_data_cache = np.concatenate((self.b_i_data_cache, self.b_i_data_while))
            self.b_t_data_cache = np.concatenate((self.b_t_data_cache, self.b_t_data_while))

        with data_lock:
            if len(self.b_i_data) <= self.b_frequency * 5:
                self.b_t_data = np.concatenate((self.b_t_data, self.b_t_data_while))
                # self.b_v_data = np.concatenate((self.b_v_data, self.b_v_data_while))
                self.b_i_data = np.concatenate((self.b_i_data, self.b_i_data_while))

            else:
                self.b_t_data = np.concatenate((self.b_t_data, self.b_t_data_while))
                # self.b_v_data = np.concatenate((self.b_v_data, self.b_v_data_while))
                self.b_i_data = np.concatenate((self.b_i_data, self.b_i_data_while))

                indices1 = list(range(len(self.b_t_data_while)))
                self.b_t_data = np.delete(self.b_t_data, indices1)
                self.b_i_data = np.delete(self.b_i_data, indices1)
                # self.b_v_data = np.delete(self.b_v_data, indices1)

          # ??????????

    def start_csv_txt_thread(self, b_t_data_while, b_i_data_while, b_baseline_data_while, b_std_data_while_up, b_std_data_while_down):
        if not self.b_csv_txt_thread or not self.b_csv_txt_thread.is_alive():
            self.b_csv_txt_thread = threading.Thread(
                target=self.b_csv_and_tree,
                args=(b_t_data_while, b_i_data_while, b_baseline_data_while, b_std_data_while_up, b_std_data_while_down)  # 传递参数
            )
            self.b_csv_txt_thread.daemon = True
            self.b_csv_txt_thread.start()
            self.threads.append(self.b_csv_txt_thread)
    def b_csv_and_tree(self, b_t_data_while, b_i_data_while, b_baseline_data_while, b_std_data_while_up, b_std_data_while_down):
            # self.update_baseline_std_vsm()
                self.b_detection_DNA_sequency = time.perf_counter()
                self.b_event_list, event_number = self.event_detection(b_t_data_while, b_i_data_while,
                                                                       b_baseline_data_while,
                                                                       b_std_data_while_up,
                                                                       b_std_data_while_down, self.b_signal_direction,
                                                                       self.b_frequency,
                                                                       10, 0,
                                                                       float(0), float(1000), 0)



                if self.b_event_two_open_csv == True:
                    if self.b_event_two_open == True:
                        print(1)
                        number = 0
                        for i in range(len(self.b_event_list)):
                            list1 = self.b_event_list[i]
                            y = list1[10]
                            if list1[4] >= 0:
                                # down, up = self.event_detection_two(y, self.c_detection_three, list1[4])
                                down, up = self.event_detection_two(list1[9], list1[10], 0.01, 0.1)
                                if down or up:
                                    self.b_event_list_two.append(list1)
                                    if 2 > len(self.b_event_list_two) > 0:
                                        self.b_detection_DNA_yes1 = time.perf_counter()
                                    self.b_event_amplitude_two.append(float(list1[2]))
                                    self.b_event_duration_two.append(float(list1[4]))
                                    number = i
                                    self.b_event_two_number_index.append(self.b_event_number + number + 1)

                        if len(self.b_event_list_two) > 0:

                            if self.b_event_1_time == True:
                                print(len(self.b_event_list_two))
                                if self.b_probe == '探针EC-K12':
                                    self.b_detection_DNA_yes = time.perf_counter()
                                    self.detection_DNA_None.config(text=f'检测到{self.b_probe}偶联DNA', fg='red')
                                    self.detection_DNA_time.config(
                                        text=f'单次检测时长{np.abs((float(self.b_event_list_all[0][9][0]) - float(self.b_event_list_two[0][9][0]))):.4f}秒',
                                        fg='black')
                                    self.detection_DNA_time_sequency.config(
                                        text=f'单序列检测时长{(self.b_detection_DNA_yes - self.b_detection_DNA_sequency):.4f}秒',
                                        fg='black')
                                if self.b_probe == '探针LA-DSM20079':
                                    self.b_detection_DNA_yes = time.perf_counter()
                                    self.detection_DNA_None.config(text=f'检测到{self.b_probe}偶联DNA', fg='red')
                                    self.detection_DNA_time.config(
                                        text=f'单次检测时长{np.abs((float(self.b_event_list_all[0][9][0]) - float(self.b_event_list_two[0][9][0]))):.4f}秒',
                                        fg='black')
                                    self.detection_DNA_time_sequency.config(
                                        text=f'单序列检测时长{(self.b_detection_DNA_yes - self.b_detection_DNA_sequency):.4f}秒',
                                        fg='black')
                                if self.b_probe == '探针ST-NCTC12958':
                                    self.b_detection_DNA_yes = time.perf_counter()
                                    self.detection_DNA_None.config(text=f'检测到{self.b_probe}偶联DNA', fg='red')
                                    self.detection_DNA_time.config(
                                        text=f'单次检测时长{np.abs((float(self.b_event_list_all[0][9][0]) - float(self.b_event_list_two[0][9][0]))):.4f}秒',
                                        fg='black')
                                    self.detection_DNA_time_sequency.config(
                                        text=f'单序列检测时长{(self.b_detection_DNA_yes - self.b_detection_DNA_sequency):.4f}秒',
                                        fg='black')
                                if self.b_probe == 'EC-K12':
                                    self.b_detection_DNA_yes = time.perf_counter()
                                    self.detection_DNA_None.config(text=f'检测到{self.b_probe}', fg='red')
                                if self.b_probe == 'EC-BL21':
                                    self.b_detection_DNA_yes = time.perf_counter()
                                    self.detection_DNA_None.config(text=f'检测到{self.b_probe}', fg='red')
                                if self.b_probe == 'EC-CFT073':
                                    self.b_detection_DNA_yes = time.perf_counter()
                                    self.detection_DNA_None.config(text=f'检测到{self.b_probe}', fg='red')
                                self.update_plot_event_2D(self.b_event_amplitude, self.b_event_duration)

                                winsound.Beep(1000, 1000)
                                # create_label2(self.collection_eONE_page_original,
                                #               f'检测到{self.b_probe}偶联DNA',
                                #               700, 850)
                                # create_label2(self.collection_eONE_page_original,
                                #               f'单次检测时长{(self.b_detection_DNA_yes - self.b_detection_DNA_None):.4f}秒',
                                #               700, 920)
                                # self.b_display_rms.config(
                                #     text=f'   {round(float(std_number[-1]), 4)}{self.b_last_current_range_selected[-2:]}')
                                self.b_text_display.config(state=tk.NORMAL)
                                line_index = self.b_text_display.index(tk.END)
                                self.b_text_display.delete(1.0, tk.END)
                                self.b_text_display.insert(tk.END, self.b_event_two_number_index)
                                self.b_text_display.yview(tk.END)
                                self.b_text_display.config(state=tk.DISABLED)
                                # self.b_event_1_time = False
                                # self.update_event_plot1(self.b_event_list_two[-1][9],self.b_event_list_two[-1][10])


                self.b_event_number += event_number
                if self.b_save_number == True:
                    print(self.b_event_two_open_csv, self.b_event_two_open)
                    if self.b_probe == '探针EC-K12':
                        self.detection_DNA_None = create_label2(self.collection_eONE_page_original,
                                                                f'未检测到{self.b_probe}偶联DNA',
                                                                700, 800)
                        self.detection_DNA_time = create_label2(self.collection_eONE_page_original,
                                                                '',
                                                                700, 870)
                        self.detection_DNA_time_sequency = create_label2(self.collection_eONE_page_original,
                                                                '',
                                                                700, 940)
                    if self.b_probe == '探针LA-DSM20079':
                        self.detection_DNA_None = create_label2(self.collection_eONE_page_original,
                                                                f'未检测到{self.b_probe}偶联DNA',
                                                                700, 800)
                        self.detection_DNA_time = create_label2(self.collection_eONE_page_original,
                                                                '',
                                                                700, 870)
                        self.detection_DNA_time_sequency = create_label2(self.collection_eONE_page_original,
                                                                '',
                                                                700, 940)
                    if self.b_probe == '探针ST-NCTC12958':
                        self.detection_DNA_None = create_label2(self.collection_eONE_page_original,
                                                                f'未检测到{self.b_probe}偶联DNA',
                                                                700, 800)
                        self.detection_DNA_time = create_label2(self.collection_eONE_page_original,
                                                                '',
                                                                700, 870)
                        self.detection_DNA_time_sequency = create_label2(self.collection_eONE_page_original,
                                                                '',
                                                                700, 940)
                    if self.b_probe == 'EC-K12':
                        self.detection_DNA_None = create_label2(self.collection_eONE_page_original,
                                                                f'未检测到{self.b_probe}',
                                                                700, 800)
                        self.detection_DNA_time = create_label2(self.collection_eONE_page_original,
                                                                '',
                                                                700, 870)
                        self.detection_DNA_time_sequency = create_label2(self.collection_eONE_page_original,
                                                                '',
                                                                700, 940)
                    if self.b_probe == 'EC-BL21':
                        self.detection_DNA_None = create_label2(self.collection_eONE_page_original,
                                                                f'未检测到{self.b_probe}',
                                                                700, 800)
                        self.detection_DNA_time = create_label2(self.collection_eONE_page_original,
                                                                '',
                                                                700, 870)
                        self.detection_DNA_time_sequency = create_label2(self.collection_eONE_page_original,
                                                                '',
                                                                700, 940)
                    if self.b_probe == 'EC-CFT073':
                        self.detection_DNA_None = create_label2(self.collection_eONE_page_original,
                                                                f'未检测到{self.b_probe}',
                                                                700, 800)
                        self.detection_DNA_time = create_label2(self.collection_eONE_page_original,
                                                                '',
                                                                700, 870)
                        self.detection_DNA_time_sequency = create_label2(self.collection_eONE_page_original,
                                                                '',
                                                                700, 940)
                    current_time = datetime.now()
                    self.formatted_time = current_time.strftime("%Y-%m-%d-%H-%M")
                    self.file_path_1 = os.path.join(self.b_save_path, f'{self.formatted_time}特征提取_1.csv')
                    self.file_path_2 = os.path.join(self.b_save_path, f'{self.formatted_time}特征提取_2.csv')
                    print(self.file_path_2)
                    print(self.file_path_1)
                    self.b_save_number = False
                    self.b_event_list_append = True
                    with open(self.file_path_1, mode='w', newline='', encoding='gbk') as file:
                        writer_1 = csv.writer(file)
                        writer_1.writerow([
                            '事件数', '基线(nA)', '振幅(nA)', '积分(10-9C)', '持续时间(ms)',
                            '钝度', '脉冲不对称', '横轴惯性', '纵轴惯性',
                            '事件数据(时间)', '事件数据(电流)'
                        ])
                    with open(self.file_path_2, mode='w', newline='', encoding='gbk') as file:
                        writer_2 = csv.writer(file)
                        writer_2.writerow([
                            '事件数', '基线(nA)', '振幅(nA)', '积分(10-9C)', '持续时间(ms)',
                            '钝度', '脉冲不对称', '横轴惯性', '纵轴惯性',
                            '事件数据(时间)', '事件数据(电流)'
                        ])
                    self.b_event_two_open_csv = True
                    print(self.b_event_two_open)

                if self.b_save_txt_head == True:
                    self.b_save_txt_head = False
                    self.b_save_txt_time_current = True
                    self.file_path_txt = os.path.join(self.b_save_path, f'{self.formatted_time}源数据.txt')
                    print(self.file_path_txt)
                    with open(self.file_path_txt, 'w') as file:
                        file.write(f'{self.formatted_time}' + '      ' + f'采样率:{self.b_frequency}Hz' + '\n')
                        file.write(f'截止频率:{self.b_filter_parameter}Hz' + '\n')
                if self.b_save_txt_time_current == True:
                    time_current_list = [[a, b] for a, b in zip(b_t_data_while, b_i_data_while)]
                    all_content = '\n'.join([' '.join(map(str, row)) for row in time_current_list])
                    with open(self.file_path_txt, 'a', encoding='utf-8') as file:
                        file.write(all_content)
                number = 0
                if self.b_event_list_append == True:
                    for i in range(self.b_event_number - event_number, self.b_event_number, 1):
                        self.save_event_list_to_csv(i + 1, self.b_event_list[number], self.file_path_1, 1)

                        self.test_tree([i + 1, self.b_event_list[number][1], self.b_event_list[number][2],
                                        self.b_event_list[number][3],
                                        self.b_event_list[number][4], self.b_event_list[number][5],
                                        self.b_event_list[number][6],
                                        self.b_event_list[number][7], self.b_event_list[number][8]])
                        self.b_event_list_all.append(self.b_event_list[number])
                        # self.update_event_plot1(self.b_event_list[number][9], self.b_event_list[number][10])
                        self.b_event_amplitude.append(float(self.b_event_list[number][2]))
                        self.b_event_duration.append(self.b_event_list[number][4])
                        self.update_plot_event_2D(self.b_event_amplitude, self.b_event_duration)
                        self.b_event_list_1.append(float(self.b_event_list[number][2]))
                        self.b_event_list_2.append(float(self.b_event_list[number][3]))
                        self.b_event_list_3.append(float(self.b_event_list[number][4]))
                        self.b_event_list_4.append(float(self.b_event_list[number][5]))
                        self.b_event_list_5.append(float(self.b_event_list[number][6]))
                        self.b_event_list_6.append(float(self.b_event_list[number][7]))
                        self.b_event_list_7.append(float(self.b_event_list[number][8]))
                        number += 1

                else:
                    number = 0
                if self.b_event_two_open_csv == True:
                    for i in range(self.b_event_list_two_len, len(self.b_event_list_two)):
                        self.save_event_list_to_csv(self.b_event_list_two_len + 1, self.b_event_list_two[self.b_event_list_two_len], self.file_path_2, 2)
                        self.b_event_list_two_len += 1
                # self.update_seven_features(self.b_event_list_1, self.b_event_list_2, self.b_event_list_3, self.b_event_list_4, self.b_event_list_5,
                #                            self.b_event_list_6, self.b_event_list_7)
    def plot_current(self):
        fig = Figure(figsize=(12.3, 2.9), dpi=100)
        fig.patch.set_facecolor('#e5e5e5')
        self.ax4 = fig.add_subplot(111)
        self.ax4.set_facecolor('#e5e5e5')
        self.ax4.set_xlabel("时间(s)")
        self.ax4.set_ylabel("电流(nA)")
        self.ax4.grid(True)
        fig.tight_layout()
        fig.subplots_adjust(left=0.05)
        self.canvas_current = FigureCanvasTkAgg(fig, master=self.collection_eONE_page_original)
        self.canvas_current2 = self.canvas_current.get_tk_widget()
        self.canvas_current2.place(x=320, y=175)
    def plot_event1(self):
        fig = Figure(figsize=(3.4, 3.4), dpi=100)
        fig.patch.set_facecolor('#e5e5e5')
        self.ax2 = fig.add_subplot(111)
        self.ax2.set_facecolor('#e5e5e5')
        self.ax2.set_title("事件图")
        self.ax2.set_xlabel("时间(s)")
        self.ax2.set_ylabel("电流(nA)")
        self.ax2.grid(True)
        fig.tight_layout()
        fig.subplots_adjust(left=0.17)
        self.canvas_event2 = FigureCanvasTkAgg(fig, master=self.collection_eONE_page_original)
        self.canvas_widget2 = self.canvas_event2.get_tk_widget()
        self.canvas_widget2.place(x=1567, y=0)

    def update_event_plot1(self, x, y):
        self.ax2.cla()
        self.ax2.grid(True)
        self.ax2.set_title("事件图")
        self.ax2.set_xlabel("时间(s)")
        self.ax2.set_ylabel("电流(nA)")
        self.ax2.plot(x, y)
        self.canvas_event2.draw()
    def plot_seven_features(self):
        fig = Figure(figsize=(9.62, 2), dpi=100)
        fig.patch.set_facecolor('#e5e5e5')

        self.seven_axes = []
        self.seven_bar_sets = []
        colors = ['red','orange','yellow','green','blue','purple','pink']
        for i in range(7):
            ax = fig.add_subplot(1, 7, i + 1)
            ax.set_facecolor('#e5e5e5')
            bars = ax.bar([0], [0], color=colors[i])
            self.seven_bar_sets.append(bars)
            ax.grid(True)
            ax.xaxis.set_ticks_position('none')
            ax.yaxis.set_ticks_position('none')
            ax.set_xticklabels([])
            ax.set_yticklabels([])
            self.seven_axes.append(ax)
        fig.subplots_adjust(
            left=0.001,
            right=0.999,
            top=0.999,
            bottom=0.01,
            wspace=0
        )

        self.seven_charts_canvas = FigureCanvasTkAgg(fig, master=self.collection_eONE_page_original)
        self.seven_charts_widget = self.seven_charts_canvas.get_tk_widget()
        self.seven_charts_widget.place(x=587, y=780)
    def update_seven_threading(self, list1):
        for i in range(7):
            ax = self.seven_axes[i]
            data = list1[i]
            ax.clear()

            if not data or len(data) == 0:
                data = [0]

            ax.hist(
                data,
                bins=50,
                density=False,
                alpha=0.6,
            )

            ax.set_facecolor('#e5e5e5')
            ax.set_xticks([])
            ax.set_yticks([])
            ax.grid(True)

            ax.relim()
            ax.autoscale_view()

        self.seven_charts_canvas.draw()
    def update_seven_features(self, data_list1, data_list2, data_list3, data_list4, data_list5, data_list6, data_list7):
        self.threads = [t for t in self.threads if t.is_alive()]
        list1 = [data_list1, data_list2, data_list3, data_list4, data_list5, data_list6, data_list7]

        t = threading.Thread(target=self.update_seven_threading, args=(list1,)
                             )
        t.daemon = True
        t.start()
        self.threads.append(t)

    def b_on_item_click(self, event):
        selected_item = self.tree.selection()
        if selected_item:
            item_values = self.tree.item(selected_item[0])['values']
            number = item_values[0]
            self.update_event_plot1(self.b_event_list_all[number-1][9], self.b_event_list_all[number-1][10])

    def plot_event_2D(self):
        fig = Figure(figsize=(3.4, 3.4), dpi=100)
        fig.patch.set_facecolor('#e5e5e5')
        self.ax3 = fig.add_subplot(111)
        self.ax3.set_facecolor('#e5e5e5')
        self.ax3.set_title("时间VS电流")
        self.ax3.set_xlabel("电流(nA)")
        self.ax3.set_ylabel("时间(ms)")
        self.ax3.grid(True)
        fig.tight_layout()
        fig.subplots_adjust(left=0.17)

        self.canvas_event3 = FigureCanvasTkAgg(fig, master=self.collection_eONE_page_original)
        self.canvas_widget3 = self.canvas_event3.get_tk_widget()
        self.canvas_widget3.place(x=1567, y=340)
    def update_plot_event_2D(self, x, y):
        self.ax3.cla()
        self.ax3.set_title("时间VS电流")
        self.ax3.set_xlabel("电流(nA)")
        self.ax3.set_ylabel("时间(ms)")
        self.ax3.scatter(x, y, edgecolor='red', facecolors='None', s=10)
        if self.b_event_two_open == True:
            if len(self.b_event_amplitude_two) > 0:
                self.ax3.scatter(self.b_event_amplitude_two, self.b_event_duration_two, edgecolor='black', facecolors='None', label='DNA', s=10)
        else:
            # self.ax3.grid(True)
            # self.ax3.set_xticks([])
            # self.ax3.set_yticks([])
            pass
        self.canvas_event3.draw()
    def b_caculate_std(self, data):
        global data_lock
        with data_lock:
            # print(len(data))
            window_size = int(0.79 * len(data))
            if window_size % 2 == 0:
                window_size += 1

            std_values = np.zeros_like(data)

            for i in range(len(data)):
                start_idx = max(0, i - window_size // 2)
                end_idx = min(len(data), i + window_size // 2 + 1)
                window_data = data[start_idx:end_idx]
                std_values[i] = (np.std(window_data))
                print(std_values)
            return std_values

    def add_collection_setting_page(self):
        self.collection_setting_page_original = tk.Frame(self.collection_eONE_page_original,
                                                 width=312,
                                                 height=1000,
                                                 bg='#e5e5e5',
                                                 bd=3,
                                                 relief=tk.RAISED)
        self.collection_setting_page_original.pack_propagate(False)
        self.collection_setting_page_original.place(x=0, y=170)
    def add_collection_picture_page(self):
        self.collection_picture_page_original = tk.Frame(self.collection_eONE_page_original,
                                                 width=1250,
                                                 height=300,
                                                 bg='#e5e5e5',
                                                 bd=0,
                                                 relief=tk.RAISED)
        self.collection_picture_page_original.pack_propagate(False)
        self.collection_picture_page_original.place(x=315, y=170)

    def add_collection_eONE_page(self):
        self.collection_eONE_page_original = tk.Frame(self.root,
                                                 width=1950,
                                                 height=1000,
                                                 bg='#e5e5e5',
                                                 bd=5,
                                                 relief=tk.RAISED)
        self.collection_eONE_page_original.pack_propagate(False)
        self.collection_eONE_page_original.place(x=0, y=0)

        self.add_collection_setting_page()
        self.add_collection_picture_page()
        def get_connect_devices():
            err, devices = c4.detectDevices()
            return devices

        def on_select(i, combo):
            if i == 0:
                list = []
                selected_item = self.b_combo.get()
                list.append(str(selected_item))
                return list
            if i == 1:
                selected_item = combo.get()
                if selected_item == '正向脉冲':
                    self.b_signal_direction = 1
                elif selected_item == '负向脉冲':
                    self.b_signal_direction = 2
                else:
                    self.b_signal_direction = 3
                print(self.b_signal_direction)
            if i == 2:
                selected_item = combo.get()
                if selected_item == '探针EC-K12':
                    self.b_probe = '探针EC-K12'
                if selected_item == '探针LA-DSM20079':
                    self.b_probe = '探针LA-DSM20079'
                if selected_item == '探针ST-NCTC12958':
                    self.b_probe = '探针ST-NCTC12958'
                if selected_item == 'EC-K12':
                    self.b_probe = 'EC-K12'
                if selected_item == 'EC-BL21':
                    self.b_probe = 'EC-BL21'
                if selected_item == 'EC-CFT073':
                    self.b_probe = 'EC-CFT073'
                print(self.b_probe)
            list = []
            selected_item = self.b_combo.get()
            list.append(str(selected_item))
            return list


        self.canvas_b_connect = tk.Canvas(self.collection_eONE_page_original, width=310, height=165, bg='#e5e5e5', bd=0)
        self.canvas_b_connect.place(x=0, y=0)
        self.create_dashed_square(self.canvas_b_connect, 3, 3, 310, 165, 1)

        options = [devices for devices in get_connect_devices()]

        self.b_combo = ttk.Combobox(self.collection_eONE_page_original, values=options, width=10, state='readonly')
        self.b_combo.current()
        self.b_combo.bind("<<ComboboxSelected>>", lambda event: on_select(0, self.b_combo))
        self.b_combo.place(x=125, y=49)

        self.b_connect_state = create_label1(self.collection_eONE_page_original, '连接状态', 10, 15)
        self.creat_color_button(6, self.collection_eONE_page_original, 100, 13)

        self.b_data_state = create_label1(self.collection_eONE_page_original, '数据存储', 140, 15)

        self.b_device_get = create_button(self.collection_eONE_page_original, '获取设备序列号',
                                          lambda: get_connect_devices(), 14, 1, 7, 47)

        self.creat_color_button(7, self.collection_eONE_page_original, 220, 13)
        self.b_connect_device = create_button(self.collection_eONE_page_original, '连接', lambda: self.b_connect_devices(1), 30, 1, 7, 85)
        self.b_dis_connect_device = create_button(self.collection_eONE_page_original, '断开连接', lambda: self.b_disconnect_devices(), 30,1, 7, 121)
        def b_draw_triangle(canvas):
            canvas.delete("shape")
            if self.b_start_detection_param == False:
                canvas.create_polygon(0, 1, 31, 16, 0, 31, fill='green', outline='black', tags='shape')
                # canvas.tag_bind('shape', '<Button-1>', lambda event: None)
            else:
                canvas.create_rectangle(0, 0, 31, 31, fill='black', outline='black', tags='shape')
                print(2)
        self.b_save_file = create_button(self.collection_eONE_page_original, '保存路径', self.b_open_new_window, 7, 1,245, 47)
        '''三角形'''
        self.b_start_detection_param = False
        self.b_save_number = False
        self.b_save_txt_head = False
        self.canvas_b_process_start = tk.Canvas(self.collection_eONE_page_original, width=31, height=31, bg='#e5e5e5', bd=0)
        self.canvas_b_process_start.place(x=260, y=105)
        b_draw_triangle(self.canvas_b_process_start)
        self.canvas_b_process_start.tag_bind('shape', '<Button-1>',
                                          lambda event: b_start_detection( self.canvas_b_process_start))

        def b_start_detection(canvas_ts):
            self.b_start_detection_param = not self.b_start_detection_param
            self.b_start_time_1_number = not self.b_start_time_1_number
            self.b_signal_number = float(self.b_detection_sigma_param.get())
            b_draw_triangle(self.canvas_b_process_start)
            self.b_save_number = not self.b_save_number
            self.b_save_txt_head = not self.b_save_txt_head
            print(self.b_start_detection_param, self.b_save_number , self.b_save_txt_head)
        self.canvas_b_setting = tk.Canvas(self.collection_eONE_page_original, width=1595, height=165, bg='#e5e5e5', bd=0)
        self.canvas_b_setting.place(x=312, y=0)
        self.create_dashed_square(self.canvas_b_setting, 3, 3, 1250, 165, 1)
        self.create_dashed_line(self.canvas_b_setting, 160, 10, 160, 155, 2)
        self.create_dashed_line(self.canvas_b_setting, 590, 10, 590, 155, 2)
        self.create_dashed_line(self.canvas_b_setting, 730, 10, 730, 155, 2)

        def b_apply_voltage(bools, number):
            if bools == True:

                c4.selectVoltageProtocol(0)

                v_hold = c4.Measurement(int(number), c4.UnitPfxMilli, "V")
                c4.setProtocolVoltage(0, v_hold)
                c4.applyVoltageProtocol()

            else:
                c4.selectVoltageProtocol(0)

                v_hold = c4.Measurement(0, c4.UnitPfxMilli, "V")
                c4.setProtocolVoltage(0, v_hold)

                c4.applyVoltageProtocol()
        def b_voltage_up_down(number):
            # def loop_step(i=0, num=1):
            #     if i < 5:
            #         c4.selectVoltageProtocol(0)
            #         v_hold = c4.Measurement(int(number) * num, c4.UnitPfxMilli, "V")
            #         c4.setProtocolVoltage(0, v_hold)
            #         c4.applyVoltageProtocol()
            #
            #         next_num = num * -1
            #         next_i = i + 1
            #         self.root.after(1000, loop_step, next_i, next_num)
            # loop_step()
            c4.selectVoltageProtocol(0)
            v_hold = c4.Measurement(int(number) * -1, c4.UnitPfxMilli, "V")
            c4.setProtocolVoltage(0, v_hold)
            c4.applyVoltageProtocol()
            self.b_bisa_voltage_param.delete(0, "end")
            self.b_bisa_voltage_param.insert(0, int(number) * -1)
        self.b_bisa_voltage = create_label1(self.collection_eONE_page_original, '偏置电压', 350, 15)
        self.b_bisa_voltage_param = create_entry1(self.collection_eONE_page_original, 10, 350, 50, 0)
        self.b_bisa_voltage_mv = create_label(self.collection_eONE_page_original, 'mV', 430, 50)
        self.b_bisa_voltage_apply = create_button(self.collection_eONE_page_original, '应用', lambda: b_apply_voltage(True,  self.b_bisa_voltage_param.get()),
                                              12, 1, 340, 85)
        self.b_bisa_voltage_clear = create_button(self.collection_eONE_page_original, '清除', lambda: b_apply_voltage(False, 1),
                                              12, 1, 340, 121)
        self.b_bisa_voltage_up_down = create_button(self.collection_eONE_page_original, '±', lambda: b_voltage_up_down(self.b_bisa_voltage_param.get()),
                                              2, 1, 440, 85)
        def calculate_d(V ,A, sigma, t):
            G = 1 / ((V * 1e-3) / (A * 1e-9))
            t = round(t * 1e-9, 9)
            d = (G / (2 * sigma)) * (1 + math.sqrt(1 + (16 * sigma * t) / (math.pi * G)))
            self.b_caculate_d_param.config(state=tk.NORMAL)
            self.b_caculate_d_param.delete(0, tk.END)
            self.b_caculate_d_param.insert(0, round(d * 1e9, 4))
            self.b_caculate_d_param.config(state=tk.DISABLED)

        self.b_d_pore_caculate = create_label1(self.collection_eONE_page_original, '纳米孔径计算', 550, 15)
        self.b_caculate_bais = create_label(self.collection_eONE_page_original, '外加电压', 500, 50)
        self.b_caculate_bais_param = create_entry1(self.collection_eONE_page_original, 10, 590, 50, 10)
        self.b_caculate_bais_mv = create_label(self.collection_eONE_page_original, 'mV', 665, 50)
        self.b_caculate_current = create_label(self.collection_eONE_page_original, '响应电流', 500, 85)
        self.b_caculate_current_param = create_entry1(self.collection_eONE_page_original, 10, 590, 85, 60)
        self.b_caculate_current_nA = create_label(self.collection_eONE_page_original, 'nA', 665, 85)
        self.b_caculate_solutiuon = create_label(self.collection_eONE_page_original, '溶液电导率', 500, 121)
        self.b_caculate_solution_param = create_entry1(self.collection_eONE_page_original, 10, 590, 121, 11.27)
        self.b_caculate_solution_sm = create_label(self.collection_eONE_page_original, 's/m', 665, 121)
        self.b_caculate_h = create_label(self.collection_eONE_page_original, '膜厚', 720, 50)
        self.b_caculate_h_param = create_entry1(self.collection_eONE_page_original, 10, 765, 50, 30)
        self.b_caculate_h_nm = create_label(self.collection_eONE_page_original, 'nm', 840, 50)
        self.b_cacilate_d_button = create_button(self.collection_eONE_page_original, '计算', lambda: calculate_d(int(self.b_caculate_bais_param.get()), float(self.b_caculate_current_param.get()),
                                                                                                            float(self.b_caculate_solution_param.get()), float(self.b_caculate_h_param.get())),
                                                  20, 1, 720, 80)
        self.b_caculate_d = create_label(self.collection_eONE_page_original, '孔径', 720, 121)
        self.b_caculate_d_param = create_entry1(self.collection_eONE_page_original, 10, 765, 121, '#')
        self.b_caculate_d_param.config(state=tk.DISABLED)
        self.b_caculate_d_nm = create_label(self.collection_eONE_page_original, 'nm', 840, 121)

        def b_baseline_compensate():
            self.b_baseline_compensate_param = not self.b_baseline_compensate_param
            if self.b_baseline_compensate_param == True:
                c4.digitalOffsetCompensation(self.b_i_channels, True)
            else:
                c4.digitalOffsetCompensation(self.b_i_channels, False)
        self.b_var_baseline_compensate = tk.IntVar()
        self.b_baseline_compensate_choice = tk.Checkbutton(self.collection_eONE_page_original, variable=self.b_var_baseline_compensate,
                                                  font=("Arial", 1), command=lambda: b_baseline_compensate())
        self.b_baseline_compensate_choice.place(x=920, y=15)
        self.b_baseline_compensate = create_label(self.collection_eONE_page_original, '基线补偿', 950, 15)

        self.b_display_std = create_label(self.collection_eONE_page_original, '均方根噪声:', 920, 50)

        self.b_display_rms = create_label(self.collection_eONE_page_original, '###', 920, 85)

        self.b_display_mean_baseline = create_label(self.collection_eONE_page_original, '基线:###', 925, 121)

        self.b_feature_extraction = create_label1(self.collection_eONE_page_original, '过孔事件特征提取', 1100, 15)


        self.b_up_or_down = create_label(self.collection_eONE_page_original, '事件类型:', 1080, 50)
        options1 = ['负向脉冲', '正向脉冲', '双向脉冲']

        self.b_combo1 = ttk.Combobox(self.collection_eONE_page_original, values=options1, width=10, state='readonly')
        self.b_combo1.current(0)
        self.b_combo1.bind("<<ComboboxSelected>>", lambda event: on_select(1, self.b_combo1))
        self.b_combo1.place(x=1160, y=49)

        self.b_detection_sigma = create_label(self.collection_eONE_page_original, '检测阈值（σ）:', 1080, 85)
        self.b_detection_sigma_param = create_entry1(self.collection_eONE_page_original, 10, 1210, 85, 5)
        def b_detection_TF():
            self.b_signal_number = float(self.b_detection_sigma_param.get())
            self.b_detection_param = not self.b_detection_param
            print(self.b_detection_param)
        self.b_var_detection_sigma = tk.IntVar()
        self.b_detection_sigma_choice = tk.Checkbutton(self.collection_eONE_page_original,
                                                           variable=self.b_var_detection_sigma,
                                                           font=("Arial", 1), command=lambda: b_detection_TF())
        self.b_detection_sigma_choice.place(x=1085, y=121)
        self.b_detection_sigma_look = create_label(self.collection_eONE_page_original, '检测阈值预览', 1110, 121)

        # self.b_detection_mintime = create_label(self.collection_eONE_page_original, '最小持续时间:', 1300, 50)
        # self.b_detection_mintime_param = create_entry1(self.collection_eONE_page_original, 10, 1410, 50, 0)
        # self.b_detection_mintime_ms = create_label(self.collection_eONE_page_original, 'ms', 1480, 50)
        #
        # self.b_detection_maxtime = create_label(self.collection_eONE_page_original, '最大持续时间:', 1300, 85)
        # self.b_detection_maxtime_param = create_entry1(self.collection_eONE_page_original, 10, 1410, 85, 1000)
        # self.b_detection_maxtime_ms = create_label(self.collection_eONE_page_original, 'ms', 1480, 85)

        self.b_detection_baseline_std_window = create_label(self.collection_eONE_page_original, '基线/std计算步长:', 1300, 121)
        self.b_detection_baseline_std_window_param = create_entry1(self.collection_eONE_page_original, 10, 1440, 121, 20)
        self.b_detection_baseline_std_window_ms = create_label(self.collection_eONE_page_original, 'ms', 1510, 121)

        # self.b_undate_seven_features = create_button(self.collection_eONE_page_original, '指纹谱更新',
        #                                              lambda: self.update_seven_features(self.b_event_list_1,
        #                                                                                 self.b_event_list_2,
        #                                                                                 self.b_event_list_3,
        #                                                                                 self.b_event_list_4,
        #                                                                                 self.b_event_list_5,
        #                                                                                 self.b_event_list_6,
        #                                                                                 self.b_event_list_7),
        #                                              12, 1, 490, 880)
        # self.update_seven_features(self.b_event_list_1, self.b_event_list_2, self.b_event_list_3, self.b_event_list_4, self.b_event_list_5,
        #                            self.b_event_list_6, self.b_event_list_7)
        def b_clear_table():
            for item in self.tree.get_children():
                self.tree.delete(item)
            self.b_event_number = 0
            self.b_event_amplitude = []
            self.b_event_duration = []
            self.update_plot_event_2D(self.b_event_amplitude, self.b_event_duration)
            self.b_event_list_1 = []
            self.b_event_list_2 = []
            self.b_event_list_3 = []
            self.b_event_list_4 = []
            self.b_event_list_5 = []
            self.b_event_list_6 = []
            self.b_event_list_7 = []
            self.b_event_list_all = []
            self.b_event_list_two = []
            self.b_event_list_two_len = 0
            self.b_event_two_number_index = []
        self.b_clear_frame = create_button(self.collection_eONE_page_original, '清空列表',
                                           lambda: b_clear_table(),
                                           12, 1, 490, 780)
        def b_scroll_choose():
            self.b_scroll_param = not self.b_scroll_param
        self.b_scroll_list = tk.IntVar()
        self.b_scroll_list_choose = tk.Checkbutton(self.collection_eONE_page_original,
                                                       variable=self.b_scroll_list,
                                                       font=("Arial", 1), command=lambda: b_scroll_choose())
        self.b_scroll_list_choose.place(x=330, y=785)
        self.b_scroll_list_look = create_label(self.collection_eONE_page_original, '滚动更新', 350, 785)
        self.probe_label = create_label(self.collection_eONE_page_original, '选择类型:', 1300, 15)

        options2 = ['探针EC-K12', '探针LA-DSM20079', '探针ST-NCTC12958', 'EC-K12', 'EC-BL21', 'EC-CFT073']

        self.b_combo2 = ttk.Combobox(self.collection_eONE_page_original, values=options2, width=15, state='readonly')
        self.b_combo2.current(0)
        self.b_combo2.bind("<<ComboboxSelected>>", lambda event: on_select(2, self.b_combo2))
        self.b_combo2.place(x=1420, y=15)
        # self.
        self.header_list = ['事件数', '基线(nA)', '振幅(nA)', '积分(10-9C)', '持续时间(ms)',
                            '钝度', '脉冲不对称', '横轴惯性', '纵轴惯性']
        self.b_show_table_tk()

        self.b_event_two_display_label = create_label1(self.collection_eONE_page_original, '检测到的事件序号', 1650, 730)
        def b_text_display_func():
            self.b_text_display = tk.Text(self.collection_eONE_page_original, width=30, height=15, wrap=tk.WORD, bg="#f0f0f0",
                                          font=("", 10), state=tk.DISABLED)
            self.b_text_display.place(x=1630, y=760)

            # self.b_scrollbar = tk.Scrollbar(self.collection_eONE_page_original, command=None)
            # self.b_scrollbar.place(x=1385, y=50, height=400)
            # self.b_text_display.config(yscrollcommand=self.b_scrollbar.set)

            self.b_text_display.bind("<Double-1>", None)
        self.plot_event1()
        self.plot_event_2D()
        # self.plot_seven_features()
        self.plot_current()
        b_text_display_func()
        current_selection_var = tk.StringVar(value="200 pA")
        sampling_selection_var = tk.StringVar(value="1.25 KHZ")
        bandwidth_selection_var = tk.StringVar(value="SR/2")
        volt_selection_var = tk.StringVar(value="1")

        def toggle_expand1():
            if self.b_source_current_range:
                self.b_expandable_frame1.pack_forget()
                self.b_extend_current.config(text="▶电流量程")
                self.b_source_current_range = False
                self.b_event_two_open = False
            else:
                self.b_expandable_frame1.pack(fill=tk.X, padx=20, pady=10)
                self.b_extend_current.config(text="▼电流量程")
                self.b_source_current_range = True
                self.b_event_two_open = True
                self.b_event_1_time = True
                if not self.b_default_selection_done_1:
                    self.b_radio1_current.select()
                    self.b_default_selection_done_1 = True
            push_down_main_content()
            print(self.b_event_two_open)
        def toggle_expand2():
            if self.b_source_sampling_range:
                self.b_expandable_frame2.pack_forget()
                self.b_extend_sampling.config(text="▶采样率")
                self.b_source_sampling_range = False
            else:
                self.b_expandable_frame2.pack(fill=tk.X, padx=20, pady=10)
                self.b_extend_sampling.config(text="▼采样率")
                self.b_source_sampling_range = True
                if not self.b_default_selection_done_2:
                    self.b_radio1_sampling.select()
                    self.b_default_selection_done_2 = True
            push_down_main_content()
        def toggle_expand3():
            if self.b_source_bandwidth_range:
                self.b_expandable_frame3.pack_forget()
                self.b_extend_bandwidth.config(text="▶低通滤波")
                self.b_source_bandwidth_range = False
            else:
                self.b_expandable_frame3.pack(fill=tk.X, padx=20, pady=10)
                self.b_extend_bandwidth.config(text="▼低通滤波")
                self.b_source_bandwidth_range = True
                if not self.b_default_selection_done_3:
                    self.b_radio1_bandwidth.select()
                    self.b_default_selection_done_3 = True

            push_down_main_content()

        # def toggle_expand4():
        #     if self.b_source_volt_range:
        #         self.b_expandable_frame1.pack_forget()
        #         self.b_extend_volt.config(text="▶电压")
        #         self.b_source_volt_range = False
        #     else:
        #         self.b_expandable_frame4.pack(fill=tk.X, padx=20, pady=10)
        #         self.b_extend_volt.config(text="▼电压")
        #         self.b_source_volt_range = True
        #         if not self.b_default_selection_done_4:
        #             # 设置默认选中状态为 "200 pA"
        #             self.b_radio1_volt.select()
        #             self.b_default_selection_done_4 = True
        #     push_down_main_content()

        def push_down_main_content():
            self.b_extend_current.pack_forget()
            self.b_extend_sampling.pack_forget()
            self.b_extend_bandwidth.pack_forget()
            # self.b_extend_volt.pack_forget()
            self.b_expandable_frame1.pack_forget()
            self.b_expandable_frame2.pack_forget()
            self.b_expandable_frame3.pack_forget()
            # self.b_expandable_frame4.pack_forget()

            self.b_extend_current.pack(pady=0)

            if self.b_source_current_range:
                self.b_expandable_frame1.pack(fill=tk.X, padx=20, pady=10)

            self.b_extend_sampling.pack(pady=0)

            if self.b_source_sampling_range:
                self.b_expandable_frame2.pack(fill=tk.X, padx=20, pady=10)

            self.b_extend_bandwidth.pack(pady=0)

            if self.b_source_bandwidth_range:
                self.b_expandable_frame3.pack(fill=tk.X, padx=20, pady=10)

            # self.b_extend_volt.pack(pady=0)
            #
            # if self.b_source_volt_range:
            #     self.b_expandable_frame4.pack(fill=tk.X, padx=20, pady=10)

        def return_current_range(value):
            current_range_list = {'200 pA': 0, '2 nA': 1, '20 nA': 2, '200 nA': 3}
            return current_range_list.get(value, 0)
        def current_range(value):

            if self.b_last_current_range_selected != value:
                print(f"选中了: {value}")
                self.b_last_current_range_selected = str(value)

                print(value)
                c4.setCurrentRange(return_current_range(value), 1)
        self.b_extend_current = tk.Button(self.collection_setting_page_original, text="▶电流量程", command=toggle_expand1, width=42, height=1,
                                   anchor='w',bg="#e5e5e5")
        self.b_extend_current.pack(padx=0, pady=0, anchor='w')

        self.b_expandable_frame1 = tk.Frame(self.collection_setting_page_original, borderwidth=0, relief="solid", bg="#e5e5e5")
        self.b_radio1_current = tk.Radiobutton(self.b_expandable_frame1, text="200 pA", variable=current_selection_var,
                                        value="200 pA",
                                        command=lambda: current_range("200 pA"), bg="#e5e5e5")
        self.b_radio1_current.pack(padx=0, pady=0, anchor='w')

        self.b_radio2_current = tk.Radiobutton(self.b_expandable_frame1, text="2 nA", variable=current_selection_var, value="2 nA",
                                        command=lambda: current_range("2 nA"), bg="#e5e5e5")
        self.b_radio2_current.pack(padx=0, pady=0, anchor='w')

        self.b_radio3_current = tk.Radiobutton(self.b_expandable_frame1, text="20 nA", variable=current_selection_var, value="20 nA",
                                        command=lambda: current_range("20 nA"), bg="#e5e5e5")
        self.b_radio3_current.pack(padx=0, pady=0, anchor='w')

        self.b_radio4_current = tk.Radiobutton(self.b_expandable_frame1, text="200 nA", variable=current_selection_var,
                                        value="200 nA",
                                        command=lambda: current_range("200 nA"), bg="#e5e5e5")
        self.b_radio4_current.pack(padx=0, pady=0, anchor='w')

        def return_sampling_rate(value):
            sampling_rate_list = {1250: 0, 5000: 1, 10000: 2, 20000: 3,
                                  50000: 4, 100000: 5, 200000: 6}
            return sampling_rate_list.get(value, 0)

        def sampling_rate(value):
            if self.b_last_sampling_rate_selected != value:
                print(f"选中了: {value}")
                self.b_last_sampling_rate_selected = value
                c4.setSamplingRate(return_sampling_rate(value))
                self.b_frequency = value


        self.b_extend_sampling = tk.Button(self.collection_setting_page_original, text="▶采样率", command=toggle_expand2, width=42, height=1,
                                    anchor='w', bg="#e5e5e5")
        self.b_extend_sampling.pack(padx=0, pady=0, anchor='w')

        self.b_expandable_frame2 = tk.Frame(self.collection_setting_page_original, borderwidth=0, relief="solid", bg="#e5e5e5")
        self.b_radio1_sampling = tk.Radiobutton(self.b_expandable_frame2, text="1.25 KHz", variable=sampling_selection_var,
                                         value="1.25 KHz",
                                         command=lambda: sampling_rate(1250), bg="#e5e5e5")
        self.b_radio1_sampling.pack(padx=0, pady=0, anchor='w')

        self.b_radio2_sampling = tk.Radiobutton(self.b_expandable_frame2, text="5 KHz", variable=sampling_selection_var,
                                         value="5 KHz",
                                         command=lambda: sampling_rate(5000), bg="#e5e5e5")
        self.b_radio2_sampling.pack(padx=0, pady=0, anchor='w')

        self.b_radio3_sampling = tk.Radiobutton(self.b_expandable_frame2, text="10 KHz", variable=sampling_selection_var,
                                         value="10 KHz",
                                         command=lambda: sampling_rate(10000), bg="#e5e5e5")
        self.b_radio3_sampling.pack(padx=0, pady=0, anchor='w')

        self.b_radio4_sampling = tk.Radiobutton(self.b_expandable_frame2, text="20 KHz", variable=sampling_selection_var,
                                         value="20 KHz",
                                         command=lambda: sampling_rate(20000), bg="#e5e5e5")
        self.b_radio4_sampling.pack(padx=0, pady=0, anchor='w')

        self.b_radio5_sampling = tk.Radiobutton(self.b_expandable_frame2, text="50 KHz", variable=sampling_selection_var,
                                         value="50 KHz",
                                         command=lambda: sampling_rate(50000), bg="#e5e5e5")
        self.b_radio5_sampling.pack(padx=0, pady=0, anchor='w')

        self.b_radio6_sampling = tk.Radiobutton(self.b_expandable_frame2, text="100 KHz", variable=sampling_selection_var,
                                         value="100 KHz",
                                         command=lambda: sampling_rate(100000), bg="#e5e5e5")
        self.b_radio6_sampling.pack(padx=0, pady=0, anchor='w')

        self.b_radio7_sampling = tk.Radiobutton(self.b_expandable_frame2, text="200 KHz", variable=sampling_selection_var,
                                         value="200 KHz",
                                         command=lambda: sampling_rate(200000), bg="#e5e5e5")
        self.b_radio7_sampling.pack(padx=0, pady=0, anchor='w')

        def filter_select(value):
            if self.b_last_filter_selected != value:
                print(f"选中了: {value}")
                self.b_last_filter_selected = value
                if value == 0:
                    self.b_filter = False
                else:
                    self.b_filter = True
                    self.b_filter_parameter = value
        self.b_extend_bandwidth = tk.Button(self.collection_setting_page_original, text="▶低通滤波", command=toggle_expand3, width=42, height=1, anchor='w', bg="#e5e5e5")
        self.b_extend_bandwidth.pack(padx=0, pady=0, anchor='w')

        self.b_expandable_frame3 = tk.Frame(self.collection_setting_page_original, borderwidth=0, relief="solid", bg="#e5e5e5")


        self.b_radio1_bandwidth = tk.Radiobutton(self.b_expandable_frame3, text="无滤波", variable=bandwidth_selection_var,
                                          value="无滤波",
                                          command=lambda: filter_select(0), bg="#e5e5e5")
        self.b_radio1_bandwidth.pack(padx=0, pady=0, anchor='w')

        self.b_radio2_bandwidth = tk.Radiobutton(self.b_expandable_frame3, text="10 KHz", variable=bandwidth_selection_var,
                                          value="10KHz",
                                          command=lambda: filter_select(10000), bg="#e5e5e5")
        self.b_radio2_bandwidth.pack(padx=0, pady=0, anchor='w')

        self.b_radio3_bandwidth = tk.Radiobutton(self.b_expandable_frame3, text="5 KHz", variable=bandwidth_selection_var,
                                          value="5KHz",
                                          command=lambda: filter_select(5000), bg="#e5e5e5")
        self.b_radio3_bandwidth.pack(padx=0, pady=0, anchor='w')

        self.b_radio4_bandwidth = tk.Radiobutton(self.b_expandable_frame3, text="2 KHz", variable=bandwidth_selection_var,
                                          value="2KHz",
                                          command=lambda: filter_select(2000), bg="#e5e5e5")
        self.b_radio4_bandwidth.pack(padx=0, pady=0, anchor='w')

        self.b_radio5_bandwidth = tk.Radiobutton(self.b_expandable_frame3, text="1 KHz",
                                                 variable=bandwidth_selection_var,
                                                 value="1KHz",
                                                 command=lambda: filter_select(1000), bg="#e5e5e5")
        self.b_radio5_bandwidth.pack(padx=0, pady=0, anchor='w')

        self.b_radio5_bandwidth = tk.Radiobutton(self.b_expandable_frame3, text="500 Hz",
                                                 variable=bandwidth_selection_var,
                                                 value="500Hz",
                                                 command=lambda: filter_select(500), bg="#e5e5e5")
        self.b_radio5_bandwidth.pack(padx=0, pady=0, anchor='w')

        # self.b_extend_volt = tk.Button(self.collection_setting_page_original, text="▶电压", command=toggle_expand4, width=42, height=1, anchor='w')
        # self.b_extend_volt.pack(padx=0, pady=0, anchor='w')
        #
        # self.b_expandable_frame4 = tk.Frame(self.collection_setting_page_original, borderwidth=0, relief="solid")
        # self.b_radio1_volt = tk.Radiobutton(self.b_expandable_frame4, text="恒压波", variable=volt_selection_var,
        #                              value="恒压波", command=lambda: filter_select("恒压波"))
        # self.b_radio1_volt.pack(padx=0, pady=0, anchor='w')
        #
        # self.b_radio2_volt = tk.Radiobutton(self.b_expandable_frame4, text="三角波", variable=volt_selection_var,
        #                              value="三角波", command=lambda: filter_select("三角波"))
        # self.b_radio2_volt.pack(padx=0, pady=0, anchor='w')
        #
        # self.b_radio3_volt = tk.Radiobutton(self.b_expandable_frame4, text="方波", variable=volt_selection_var,
        #                              value="方波", command=lambda: filter_select("方波"))
        # self.b_radio3_volt.pack(padx=0, pady=0, anchor='w')
        #
        # self.b_radio4_volt = tk.Radiobutton(self.b_expandable_frame4, text="可变幅方波", variable=volt_selection_var,
        #                              value="可变幅方波", command=lambda: filter_select("可变幅方波"))
        # self.b_radio4_volt.pack(padx=0, pady=0, anchor='w')
        #
        # self.b_radio5_volt = tk.Radiobutton(self.b_expandable_frame4, text="可变幅单向方波", variable=volt_selection_var,
        #                              value="可变幅方波", command=lambda: filter_select("可变幅方波"))
        # self.b_radio5_volt.pack(padx=0, pady=0, anchor='w')
        #
        # self.b_radio6_volt = tk.Radiobutton(self.b_expandable_frame4, text="可变持续时间单向方波", variable=volt_selection_var,
        #                              value="可变持续时间单向方波",
        #                              command=lambda: filter_select("可变持续时间单向方波"))
        # self.b_radio6_volt.pack(padx=0, pady=0, anchor='w')
        #
        # self.b_radio7_volt = tk.Radiobutton(self.b_expandable_frame4, text="梯形波", variable=volt_selection_var,
        #                              value="梯形波", command=lambda: filter_select("梯形波"))
        # self.b_radio7_volt.pack(padx=0, pady=0, anchor='w')
        #
        # self.b_radio8_volt = tk.Radiobutton(self.b_expandable_frame4, text="可调周期三角波", variable=volt_selection_var,
        #                              value="可调周期三角波", command=lambda: filter_select("可调周期三角波"))
        # self.b_radio8_volt.pack(padx=0, pady=0, anchor='w')
    '''数据分析'''
    def c_plot_current(self):
        fig = Figure(figsize=(9.4, 2.5), dpi=100)
        fig.patch.set_facecolor('#e5e5e5')
        self.ax_c_current = fig.add_subplot(111)
        self.ax_c_current.set_facecolor('#e5e5e5')
        self.ax_c_current.set_xlabel("时间(s)")
        self.ax_c_current.set_ylabel("电流(nA)")
        self.ax_c_current.grid(True)
        fig.tight_layout()
        fig.subplots_adjust(left=0.06)
        self.canvas_current_c = FigureCanvasTkAgg(fig, master=self.analyse_page_original)
        self.canvas_current_c1 = self.canvas_current_c.get_tk_widget()
        self.canvas_current_c1.place(x=5, y=180)

    def update_c_plot_current(self, x, y):
        self.ax_c_current.cla()
        self.ax_c_current.grid(True)
        self.ax_c_current.set_xlabel("时间(s)")
        self.ax_c_current.set_ylabel("电流(nA)")
        if self.c_filter == True:
            # 绘制主数据线条
            self.ax_c_current.plot(x, self.c1_y_filter)
        else:
            self.ax_c_current.plot(x, y)
        # 处理额外线条（对应原来的b_line3,4,5）
        if self.c_detection_threshold == False:
            pass
        else:
            self.ax_c_current.plot(x, self.c_baseline, color='black')
            self.ax_c_current.plot(x, self.c_std_down, color='green')
            self.ax_c_current.plot(x, self.c_std_up, color='green')
        if self.c_event == True:
            for i in range(len(self.c_event_list)):
                self.ax_c_current.plot(self.c_event_list[i][-2], self.c_event_list[i][-1], color='red')

            for i in range(len(self.c_event_list_two)):
                self.ax_c_current.plot(self.c_event_list_two[i][-2], self.c_event_list_two[i][-1], color='black')
            else:
                pass
        self.canvas_current_c.draw()

    def c_plot_event1(self):
        fig = Figure(figsize=(3.1, 2.8), dpi=100)
        fig.patch.set_facecolor('#e5e5e5')
        self.ax_c_event = fig.add_subplot(111)
        self.ax_c_event.set_facecolor('#e5e5e5')

        self.ax_c_event.set_xlabel("时间(s)")
        self.ax_c_event.set_ylabel("电流(nA)")
        self.ax_c_event.grid(True)
        fig.tight_layout()
        fig.subplots_adjust(left=0.2, bottom=0.15)
        self.canvas_event_c_event = FigureCanvasTkAgg(fig, master=self.analyse_page_original)
        self.canvas_widget2_c_event = self.canvas_event_c_event.get_tk_widget()
        self.canvas_widget2_c_event.place(x=5, y=696)
    def c_update_plot_event1(self, x, y):
        self.ax_c_event.cla()
        self.ax_c_event.grid(True)
        self.ax_c_event.set_xlabel("时间(s)")
        self.ax_c_event.set_ylabel("电流(nA)")
        self.ax_c_event.plot(x, y)
        self.canvas_event_c_event.draw()
    def c_plot_2d(self):
        fig = Figure(figsize=(3.1, 2.8), dpi=100)
        fig.patch.set_facecolor('#e5e5e5')
        self.ax_c_2d = fig.add_subplot(111)
        self.ax_c_2d.set_facecolor('#e5e5e5')
        self.ax_c_2d.set_xlabel("时间(ms)")
        self.ax_c_2d.set_ylabel("振幅(nA)")
        self.ax_c_2d.grid(True)
        fig.tight_layout()
        fig.subplots_adjust(left=0.2, bottom=0.15)
        self.canvas_event_c_2d = FigureCanvasTkAgg(fig, master=self.analyse_page_original)
        self.canvas_widget2_c_2d = self.canvas_event_c_2d.get_tk_widget()
        self.canvas_widget2_c_2d.place(x=320, y=696)
    def c_update_plot_2D(self, x, y):
        self.ax_c_2d.cla()
        # self.ax3.grid(True)
        self.ax_c_2d.set_xlabel("时间(ms)")
        self.ax_c_2d.set_ylabel("振幅(nA)")
        # self.ax3.set_xticks([])
        # self.ax3.set_yticks([])
        self.ax_c_2d.scatter(x, y, edgecolor='red', facecolors='None', s=100)
        self.canvas_event_c_2d.draw()
    def c_plot_histogram(self):
        fig = Figure(figsize=(3.1, 2.8), dpi=100)
        fig.patch.set_facecolor('#e5e5e5')
        self.ax_c_histogram = fig.add_subplot(111)
        self.ax_c_histogram.set_facecolor('#e5e5e5')
        self.ax_c_histogram.set_xlabel("时间(s)")
        self.ax_c_histogram.set_ylabel("电流(nA)")
        self.ax_c_histogram.grid(True)
        fig.tight_layout()
        fig.subplots_adjust(left=0.2, bottom=0.15)
        self.canvas_event_c_histogram = FigureCanvasTkAgg(fig, master=self.analyse_page_original)
        self.canvas_widget2_c_histogram = self.canvas_event_c_histogram.get_tk_widget()
        self.canvas_widget2_c_histogram.place(x=635, y=696)
    def c_undate_plot_histogram(self, data):
        self.ax_c_histogram.cla()
        self.ax_c_histogram.set_xlabel("时间(ms)")
        self.ax_c_histogram.set_ylabel("count")
        self.ax_c_histogram.hist(data,
                bins=50,
                density=False,
                alpha=0.6,)
        self.canvas_event_c_histogram.draw()
    def c_plot_2d_2(self):
        fig = Figure(figsize=(3.0, 2.4), dpi=100)
        fig.patch.set_facecolor('#e5e5e5')
        self.ax_c_2d_2 = fig.add_subplot(111)
        self.ax_c_2d_2.set_facecolor('#e5e5e5')
        self.ax_c_2d_2.set_xlabel("时间(ms)")
        self.ax_c_2d_2.set_ylabel("振幅(nA)")
        self.ax_c_2d_2.grid(True)
        fig.tight_layout()
        fig.subplots_adjust(left=0.2, bottom=0.2)
        self.canvas_event_c_2d_2 = FigureCanvasTkAgg(fig, master=self.analyse_page_original)
        self.canvas_widget2_c_2d_2 = self.canvas_event_c_2d_2.get_tk_widget()
        self.canvas_widget2_c_2d_2.place(x=1430, y=10)
    def c_update_plot_2D_2(self, x, y):
        self.ax_c_2d_2.cla()
        # self.ax3.grid(True)
        self.ax_c_2d_2.set_xlabel("时间(ms)")
        self.ax_c_2d_2.set_ylabel("振幅(nA)")
        # self.ax3.set_xticks([])
        # self.ax3.set_yticks([])
        self.ax_c_2d_2.scatter(x, y, edgecolor='red', facecolors='None', s=100)
        self.canvas_event_c_2d_2.draw()
    def c_plot_histogram_2_2(self):
        fig = Figure(figsize=(3.0, 2.4), dpi=100)
        fig.patch.set_facecolor('#e5e5e5')
        self.ax_c_histogram_2 = fig.add_subplot(111)
        self.ax_c_histogram_2.set_facecolor('#e5e5e5')
        self.ax_c_histogram_2.set_xlabel("时间(s)")
        self.ax_c_histogram_2.set_ylabel("电流(nA)")
        self.ax_c_histogram_2.grid(True)
        fig.tight_layout()
        fig.subplots_adjust(left=0.2, bottom=0.2)
        self.canvas_event_c_histogram_2 = FigureCanvasTkAgg(fig, master=self.analyse_page_original)
        self.canvas_widget2_c_histogram_2 = self.canvas_event_c_histogram_2.get_tk_widget()
        self.canvas_widget2_c_histogram_2.place(x=1430, y=250)
    def c_undate_plot_histogram_2(self, data):
        self.ax_c_histogram_2.cla()
        self.ax_c_histogram_2.set_xlabel("时间(ms)")
        self.ax_c_histogram_2.set_ylabel("count")
        self.ax_c_histogram_2.hist(data,
                bins=50,
                density=False,
                alpha=0.6,)
        self.canvas_event_c_histogram_2.draw()
    def c_plot_2d_3(self):
        fig = Figure(figsize=(5.0, 5.0), dpi=100)
        fig.patch.set_facecolor('#e5e5e5')
        self.ax_c_2d_3 = fig.add_subplot(111)
        self.ax_c_2d_3.set_facecolor('#e5e5e5')
        self.ax_c_2d_3.set_xlabel("时间(ms)")
        self.ax_c_2d_3.set_ylabel("振幅(nA)")
        self.ax_c_2d_3.grid(True)
        fig.tight_layout()
        fig.subplots_adjust(left=0.2, bottom=0.2)
        self.canvas_event_c_2d_3 = FigureCanvasTkAgg(fig, master=self.analyse_page_original)
        self.canvas_widget2_c_2d_3 = self.canvas_event_c_2d_3.get_tk_widget()
        self.canvas_widget2_c_2d_3.place(x=1400, y=450)
    def c_update_plot_2D_3(self, x, y):
        self.ax_c_2d_3.cla()
        # self.ax3.grid(True)
        self.ax_c_2d_3.set_xlabel("时间(ms)")
        self.ax_c_2d_3.set_ylabel("振幅(nA)")
        # self.ax3.set_xticks([])
        # self.ax3.set_yticks([])
        self.ax_c_2d_3.scatter(x, y, edgecolor='red', facecolors='None', s=100)
        self.canvas_event_c_2d_3.draw()
    def c_show_table_tk(self):
        self.c_table_frame = tk.Frame(
            self.analyse_page_original,
            width=600,
            height=600,
            bg='#e5e5e5',
            relief=tk.SUNKEN,
            bd=2
        )
        self.c_table_frame.place(x=8, y=425)

        style = ttk.Style()

        style.theme_use('default')

        style.configure(
            "Custom.Treeview",
            background="#e5e5e5",
            fieldbackground="#e5e5e5",
        )

        style.configure(
            "Custom.Treeview.Heading",
            background="#e5e5e5",
        )

        style.map(
            "Custom.Treeview",
            background=[('selected', 'skyblue')]
        )

        self.c_tree = ttk.Treeview(
            self.c_table_frame,
            columns=[str(i) for i in range(len(self.header_list))],
            show='headings',
            height=10,
            style="Custom.Treeview"
        )

        for i in range(len(self.header_list)):
            self.c_tree.heading(str(i), text=f'{self.header_list[i]}')
            self.c_tree.column(str(i), width=102, anchor='center')

        scrollbar = ttk.Scrollbar(self.c_table_frame, orient=tk.VERTICAL, command=self.c_tree.yview)
        self.c_tree.configure(yscrollcommand=scrollbar.set)

        self.c_tree.grid(row=0, column=0, sticky='nsew')
        scrollbar.grid(row=0, column=1, sticky='ns')

        self.c_table_frame.grid_rowconfigure(0, weight=1)
        self.c_table_frame.grid_columnconfigure(0, weight=1)

        self.c_tree.bind("<ButtonRelease-1>", self.c_on_item_click)
    def c_on_item_click(self, event):
        selected_item = self.c_tree.selection()
        if selected_item:
            item_values = self.c_tree.item(selected_item[0])['values']
            x_str = item_values[-2]
            x_str_list = x_str.split()
            x_float_list = [float(s) for s in x_str_list]
            y_str = item_values[-1]
            y_str_list = y_str.split()
            y_float_list = [float(s) for s in y_str_list]
            self.c_update_plot_event1(x_float_list, y_float_list)

    def c_on_confirm(self, window, folder_path1, folder_path2):
        if folder_path1 and folder_path2:
            self.c_save_path = folder_path1 + '/' + folder_path2 + '.csv'
            print(self.c_save_path)
            window.destroy()
            with open(self.c_save_path, mode='w', newline='', encoding='gbk') as file:
                writer = csv.writer(file)
                writer.writerow([
                    '事件数', '基线(nA)', '振幅(nA)', '积分(10-9C)', '持续时间(ms)',
                    '钝度', '脉冲不对称', '横轴惯性', '纵轴惯性',
                    '事件数据(时间)', '事件数据(电流)'
                ])
                for i in range(len(self.c_event_list)):
                    writer.writerow(self.c_event_list[i])
    def c_text_display_func(self):
        self.c_text_display = tk.Text(self.analyse_page_original, width=60, height=30, wrap=tk.WORD, bg="#f0f0f0",
                                    font=("", 10), state=tk.DISABLED)
        self.c_text_display.place(x=960, y=50)

        self.scrollbar = tk.Scrollbar(self.analyse_page_original, command=self.c_text_display.yview)
        self.scrollbar.place(x=1385, y=50, height=400)
        self.c_text_display.config(yscrollcommand=self.scrollbar.set)

        self.c_text_display.bind("<Double-1>", self.on_text_click)
    def on_text_click(self, event):
        index = self.c_text_display.index(f"@{event.x},{event.y}")
        self.c_line_number = index.split('.')[0]

        line_content = self.c_text_display.get(f"{self.c_line_number}.0", f"{self.c_line_number}.end").strip()
        self.c_preview_file_name = line_content
        if line_content:
            self.c_text_display.tag_remove("highlight", "1.0", "end")

            self.c_text_display.config(state=tk.NORMAL)
            self.c_text_display.tag_add("highlight", f"{self.c_line_number}.0", f"{self.c_line_number}.end")
            self.c_text_display.tag_config("highlight", background="lightblue")

            self.c_text_display.config(state=tk.DISABLED)
            df = pd.read_csv(line_content, encoding='gbk')
            events_baseline = df['基线(nA)']
            self.c_events_list_baseline_2 = events_baseline.tolist()
            events_amplitude = df['振幅(nA)']
            self.c_events_list_amplitude_2 = events_amplitude.tolist()
            events_integral = df['积分(10-9C)']
            self.c_events_list_integral_2 = events_integral.tolist()
            events_duration = df['持续时间(ms)']
            self.c_events_list_duration_2 = events_duration.tolist()
            events_bluntness = df['钝度']
            self.c_events_list_bluntness_2 = events_bluntness.tolist()
            events_asymmetry = df['脉冲不对称']
            self.c_events_list_asymmetry_2 = events_asymmetry.tolist()
            events_x_inertia = df['横轴惯性']
            self.c_events_list_x_inertia_2 = events_x_inertia.tolist()
            events_y_inertia = df['纵轴惯性']
            self.c_events_list_y_inertia_2 = events_y_inertia.tolist()
            self.c_undate_plot_histogram_2(self.c_events_list_amplitude_2)
            self.c_update_plot_2D_2(self.c_events_list_duration_2, self.c_events_list_amplitude_2)
    # def add_file_name(self, filename):
    #     line_index = self.c_text_display.index(tk.END)
    #     self.c_text_display.insert(tk.END, f'{filename}\n')
    #     self.c_text_display.yview(tk.END)
    def c_text_display_func_2(self):
        self.c_text_display_2 = tk.Text(self.analyse_page_original, width=60, height=30, wrap=tk.WORD, bg="#f0f0f0",
                                    font=("", 10), state=tk.DISABLED)
        self.c_text_display_2.place(x=960, y=530)

        self.scrollbar_2 = tk.Scrollbar(self.analyse_page_original, command=self.c_text_display_2.yview)
        self.scrollbar_2.place(x=1385, y=530, height=400)
        self.c_text_display_2.config(yscrollcommand=self.scrollbar_2.set)

        self.c_text_display_2.bind("<Double-1>", self.on_text_click_2)

    def on_text_click_2(self, event):
        index = self.c_text_display_2.index(f"@{event.x},{event.y}")
        self.c_line_number = index.split('.')[0]

        line_content = self.c_text_display_2.get(f"{self.c_line_number}.0", f"{self.c_line_number}.end").strip()
        if line_content:
            content = line_content.strip('()')
            result = [item.strip("'\"") for item in content.split(', ')]
            self.c_statistics_file_name = result
            self.c_text_display_2.tag_remove("highlight", "1.0", "end")

            self.c_text_display_2.config(state=tk.NORMAL)
            self.c_text_display_2.tag_add("highlight", f"{self.c_line_number}.0", f"{self.c_line_number}.end")
            self.c_text_display_2.tag_config("highlight", background="lightblue")
            self.c_text_display_2.config(state=tk.DISABLED)
    def add_analyse_page(self):
        self.analyse_page_original = tk.Frame(self.root,
                                        width=1950,
                                        height=1000,
                                        bg='#e5e5e5',
                                        bd=5,
                                        relief=tk.RAISED)
        self.analyse_page_original.place(x=0, y=0)
        self.canvas_c_picture = tk.Canvas(self.analyse_page_original, width=1905, height=980, bg='#e5e5e5',
                                          bd=0)
        self.canvas_c_picture.place(x=0, y=0)
        self.create_dashed_square(self.canvas_c_picture, 3, 3, 950, 977, 1)
        self.create_dashed_square(self.canvas_c_picture, 950, 3, 1905, 977, 1)

        def open_new_window():
            new_window = tk.Toplevel(self.root)
            new_window.title("打开文件")
            new_window.geometry("330x120")
            new_window.grab_set()

            path_entry = create_entry(new_window, 30, 69, 30)
            create_label(new_window, "文件信息", 3, 30)
            if self.c_file_folder_choice == False:
                browse_button = tk.Button(new_window, text="浏览", bg='#e5e5e5', font=('', 9),
                                          command=lambda: browse_file1(path_entry,
                                                                      [("ABF files", "*.abf"), ("Text files", "*.txt")]))

                browse_button.place(x=287, y=30)
                confirm_button = tk.Button(new_window, text="确定", bg='#e5e5e5', font=('', 9),
                                           command=lambda: on_confirm(new_window, path_entry.get()))
                confirm_button.place(x=140, y=80)
            else:
                browse_button = tk.Button(new_window, text="浏览", bg='#e5e5e5', font=('', 9),
                                          command=lambda: browse_file(path_entry))

                browse_button.place(x=287, y=30)
                confirm_button = tk.Button(new_window, text="确定", bg='#e5e5e5', font=('', 9),
                                           command=lambda: on_confirm(new_window, path_entry.get()))
                confirm_button.place(x=140, y=80)

        def on_confirm(window, folder_path):
            if folder_path:
                self.c1_var_event.set(0)

                self.c2_var_detection_sigma.set(0)

                self.c2_var_filter.set(0)
                self.c_filter = False
                self.c_event = False
                self.c_detection_threshold = False
                save_path = os.path.join(folder_path)
                for item in self.c_tree.get_children():
                    self.c_tree.delete(item)
                self.c_plot_event1()
                self.c_plot_2d()
                self.c_plot_histogram()
                self.c_baseline_list = []
                self.c_amplitude_list = []
                self.c_integral_list = []
                self.c_duration_list = []
                self.c_bluntness_list = []
                self.c_asymmetry_list = []
                self.c_x_inertia_list = []
                self.c_y_inertia_list = []
                self.c_file_abf_list = []
                if self.c_file_folder_choice == False:
                    if folder_path[-3:] == 'abf':
                        self.c1_file_path_entry.config(state=tk.NORMAL)  # 允许修改文本框
                        self.c1_file_path_entry.delete(0, tk.END)
                        self.c1_file_path_entry.insert(0, folder_path)
                        self.c1_file_path_entry.config(state=tk.DISABLED)  # 禁止编辑
                        abf = pyabf.ABF(folder_path)
                        self.c1_x = abf.sweepX[:500000 * 2]
                        self.c1_y = abf.sweepY[:500000 * 2]
                        # self.c1_x = self.c1_x[:len(self.c1_x) // (40 * 8)]
                        # self.c1_y = self.c1_y[:len(self.c1_y) // (40 * 8)]
                        print(abf.dataRate)
                        print(self.c1_x)
                        window.destroy()
                        self.c_frequency = abf.dataRate
                        self.update_c_plot_current(self.c1_x, self.c1_y)
                    else:
                        self.c1_file_path_entry.config(state=tk.NORMAL)  # 允许修改文本框
                        self.c1_file_path_entry.delete(0, tk.END)
                        self.c1_file_path_entry.insert(0, folder_path)
                        self.c1_file_path_entry.config(state=tk.DISABLED)  # 禁止编辑
                        x_list = []
                        y_list = []

                        with open(folder_path, 'r', encoding='gbk') as file:
                            for _ in range(3):
                                next(file)

                            for line in file:
                                parts = line.strip().split()
                                if len(parts) == 2:
                                    x_list.append(float(parts[0]))
                                    y_list.append(float(parts[1]))

                        self.c1_x = np.array(x_list)
                        self.c1_y = np.array(y_list)
                        print(self.c1_x)
                        window.destroy()
                        self.update_c_plot_current(self.c1_x, self.c1_y)
                        self.c_frequency = 100000
                else:
                    self.c1_file_path_entry.config(state=tk.NORMAL)  # 允许修改文本框
                    self.c1_file_path_entry.delete(0, tk.END)
                    self.c1_file_path_entry.insert(0, folder_path)
                    self.c1_file_path_entry.config(state=tk.DISABLED)  # 禁止编辑
                    source_directory = folder_path
                    for filename in os.listdir(source_directory):
                        if filename.endswith(".abf"):
                            file_path = os.path.join(source_directory, filename)
                            self.c_file_abf_list.append(file_path)

                    window.destroy()
            else:
                messagebox.showwarning("警告", "请传入有效路径！")

        def tree_insert(event_list):
            # print(event_list[0])
            for item in self.c_tree.get_children():
                self.c_tree.delete(item)
            for i in range(len(event_list)):
                self.c_tree.insert('', tk.END, values=event_list[i])
        def c_start_detection_func():
            self.c1_var_event.set(0)
            self.c_detection_sigma = float(self.c_detection_sigma_param.get())
            self.c2_var_detection_sigma.set(0)
            self.c_event = False
            self.c_detection_threshold = False
            for item in self.c_tree.get_children():
                self.c_tree.delete(item)
            self.c_plot_event1()
            self.c_plot_2d()
            self.c_plot_histogram()
            self.c_baseline_list = []
            self.c_amplitude_list = []
            self.c_integral_list = []
            self.c_duration_list = []
            self.c_bluntness_list = []
            self.c_asymmetry_list = []
            self.c_x_inertia_list = []
            self.c_y_inertia_list = []

            self.b_event_list_two = []
            self.b_event_amplitude_two= []
            self.b_event_duration_two= []
            self.c_event_list_two = []
            self.c_detection_mintime = float(self.c_detection_mintime_param.get())
            self.c_detection_maxtime = float(self.c_detection_maxtime_param.get())
            if self.c_file_folder_choice == False:
                if self.c_filter == True:
                    self.c_baseline = dynamic_detection.calculate_dynamic_baseline_fast_trimmed_py(self.c1_y_filter,
                                                                                                   (float(
                                                                                                       self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000)
                    self.c_std_down, self.c_std_up = np.array(
                        dynamic_detection.detection_y_py(self.c1_y_filter, (
                                float(self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000,
                                                         float(self.c_detection_sigma)))
                    self.c_event_list, self.c_event_list_number = self.event_detection(self.c1_x, self.c1_y_filter, self.c_baseline, self.c_std_up, self.c_std_down, self.c_detection_three, self.c_frequency, 1, 0,
                                         self.c_detection_mintime, self.c_detection_maxtime, 0)
                else:
                    self.c_baseline = dynamic_detection.calculate_dynamic_baseline_fast_trimmed_py(self.c1_y,
                                                                                                   (float(
                                                                                                       self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000)
                    self.c_std_down, self.c_std_up = np.array(
                        dynamic_detection.detection_y_py(self.c1_y, (
                                float(self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000,
                                                         float(self.c_detection_sigma)))
                    self.c_event_list, self.c_event_list_number = self.event_detection(self.c1_x, self.c1_y,
                                                                                       self.c_baseline, self.c_std_up,
                                                                                       self.c_std_down,
                                                                                       self.c_detection_three,
                                                                                       self.c_frequency, 1, 0,
                                                                                       self.c_detection_mintime,
                                                                                       self.c_detection_maxtime, 0)
                for i in range(len(self.c_event_list)):
                    list1 = self.c_event_list[i]
                    y = list1[10]
                    # down, up = self.event_detection_two(y, self.c_detection_three, list1[4])
                    down, up = self.event_detection_two(list1[9], list1[10], 0.01, 0.1)
                    if down or up:
                        self.c_event_list_two.append(list1)

                    # baseline = dynamic_detection.calculate_dynamic_baseline_fast_trimmed_py(list1[9],
                    #                                                                                (float(
                    #                                                                                    self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000)
                    # c_std_down, c_std_up = np.array(
                    #     dynamic_detection.detection_y_py(list1[9], (
                    #             float(self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000,
                    #                                      float(self.c_detection_sigma)))
                    # list2, number = self.event_detection(list1[8], list1[9],
                    #                                                                    self.c_baseline, self.c_std_up,
                    #                                                                    self.c_std_down,
                    #                                                                    self.c_detection_three,
                    #                                                                    self.c_frequency, 20, 0,
                    #                                                                    self.c_detection_mintime,
                    #                                                                    self.c_detection_maxtime, 0)
                    self.c_baseline_list.append(float(list1[1]))
                    self.c_amplitude_list.append(float(list1[2]))
                    self.c_integral_list.append(float(list1[3]))
                    self.c_duration_list.append(float(list1[4]))
                    self.c_bluntness_list.append(float(list1[5]))
                    self.c_asymmetry_list.append(float(list1[6]))
                    self.c_x_inertia_list.append(float(list1[7]))
                    self.c_y_inertia_list.append(float(list1[8]))
                list_x = []
                list_y = []
                number = len(self.c_event_list_two)
                for i in range(number):
                    list_x.append(self.c_event_list_two[i][9])
                    list_y.append(self.c_event_list_two[i][10])
                valid_number = len(list_x)

                rows = int(np.ceil(np.sqrt(valid_number)))
                cols = int(np.ceil(valid_number / rows))
                fig_size = (cols * 2.5, rows * 2.5)  # 每列2.5英寸宽，每行2.5英寸高
                fig, axes = plt.subplots(nrows=rows, ncols=cols, figsize=fig_size)
                axes_flat = axes.flatten() if (rows > 1 and cols > 1) else [axes]
                for idx, ax in enumerate(axes_flat):
                    if idx < valid_number:
                        ax.plot(list_x[idx], list_y[idx])

                        ax.tick_params(axis='both', labelsize=7)

                        ax.grid(True, alpha=0.2, linestyle='--')
                    else:
                        ax.axis('off')
                # plt.subplots_adjust(hspace=hspace, wspace=wspace)
                plt.show()

                self.update_c_plot_current(self.c1_x, self.c1_y)
                tree_insert(self.c_event_list)
                self.c_update_plot_2D(self.c_duration_list, self.c_amplitude_list)
            else:
                self.c_event_list = defaultdict(list)
                number = 0
                for i in range(len(self.c_file_abf_list)):
                    abf = pyabf.ABF(self.c_file_abf_list[i])
                    self.c1_x = abf.sweepX
                    self.c1_y = abf.sweepY
                    self.c_frequency = abf.dataRate
                    if self.c_filter == True:
                        cutoff_frequency = int(self.c_filter_frequency_param.get())  # 截止频率
                        nyquist_frequency = 0.5 * self.c_frequency  # 奈奎斯特频率
                        normal_cutoff = cutoff_frequency / nyquist_frequency
                        b, a = signal.butter(1, normal_cutoff, btype='low', analog=False)

                        # normal_cutoff = [100 / nyquist_frequency, cutoff_frequency / nyquist_frequency]
                        # b, a = signal.butter(1, normal_cutoff, btype='band', analog=False)

                        self.c1_y_filter = signal.filtfilt(b, a, self.c1_y)

                        self.c_baseline = dynamic_detection.calculate_dynamic_baseline_fast_trimmed_py(self.c1_y_filter,
                                                                                                       (float(
                                                                                                           self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000)
                        self.c_std_down, self.c_std_up = np.array(
                            dynamic_detection.detection_y_py(self.c1_y_filter, (
                                    float(self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000,
                                                             float(self.c_detection_sigma)))
                        self.c_event_list_while, self.c_event_list_number = self.event_detection(self.c1_x, self.c1_y_filter,
                                                                                           self.c_baseline,
                                                                                           self.c_std_up,
                                                                                           self.c_std_down,
                                                                                           self.c_detection_three,
                                                                                           self.c_frequency, 1, 0,
                                                                                           self.c_detection_mintime,
                                                                                           self.c_detection_maxtime, 0)
                    else:

                        self.c_baseline = dynamic_detection.calculate_dynamic_baseline_fast_trimmed_py(self.c1_y,
                                                                                                       (float(
                                                                                                           self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000)
                        self.c_std_down, self.c_std_up = np.array(
                            dynamic_detection.detection_y_py(self.c1_y, (
                                    float(self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000,
                                                             float(self.c_detection_sigma)))
                        self.c_event_list_while, self.c_event_list_number = self.event_detection(self.c1_x, self.c1_y,
                                                                                           self.c_baseline,
                                                                                           self.c_std_up,
                                                                                           self.c_std_down,
                                                                                           self.c_detection_three,
                                                                                           self.c_frequency, 1, 0,
                                                                                           self.c_detection_mintime,
                                                                                           self.c_detection_maxtime, 0)

                    for j in range(len(self.c_event_list_while)):
                        list_while = self.c_event_list_while[j]
                        self.c_event_list[number] += list_while
                        number += 1
                for k in range(len(self.c_event_list)):
                    list1 = self.c_event_list[k]
                    self.c_baseline_list.append(float(list1[1]))
                    self.c_amplitude_list.append(float(list1[2]))
                    self.c_integral_list.append(float(list1[3]))
                    self.c_duration_list.append(float(list1[4]))
                    self.c_bluntness_list.append(float(list1[5]))
                    self.c_asymmetry_list.append(float(list1[6]))
                    self.c_x_inertia_list.append(float(list1[7]))
                    self.c_y_inertia_list.append(float(list1[8]))
                tree_insert(self.c_event_list)

                self.c_update_plot_2D(self.c_duration_list, self.c_amplitude_list)
        def open_new_window_save():
            new_window = tk.Toplevel(self.root)
            new_window.title("数据保存")
            new_window.geometry("330x120")
            new_window.grab_set()
            create_label(new_window, '路径', 10, 20)
            create_label(new_window, '文件名', 10, 50)
            path_entry_1 = create_entry(new_window, 30, 65, 20)
            path_entry_2 = create_entry(new_window, 30, 65, 50)
            browse_button = tk.Button(new_window, text="浏览", bg='#e5e5e5', font=('', 9),
                                      command=lambda: browse_file(path_entry_1))
            browse_button.place(x=287, y=20)

            confirm_button = tk.Button(new_window, text="确定", bg='#e5e5e5', font=('', 9),
                                       command=lambda: self.c_on_confirm(new_window, path_entry_1.get(), path_entry_2.get()))
            confirm_button.place(x=140, y=80)


        self.c_open_file = create_button(self.analyse_page_original, '打开文件/文件夹',
                                                  open_new_window,
                                                  13, 1, 10, 10)
        self.c1_file_path_entry = create_entry(self.analyse_page_original, 95, 120, 12)
        self.c1_file_path_entry.config(state=tk.DISABLED)
        def c_file_folder_choice_func():
            self.c_file_folder_choice = not self.c_file_folder_choice
            print(self.c_file_folder_choice)
        self.c_file_folder_choice_tk = tk.IntVar()
        self.c_file_folder_choice_button = tk.Checkbutton(self.analyse_page_original,
                                                        variable=self.c_file_folder_choice_tk,
                                                        font=("Arial", 1), command=lambda: c_file_folder_choice_func())
        self.c_file_folder_choice_button.place(x=820, y=12)
        self.c_file_folder_choice_look = create_label(self.analyse_page_original, '文件夹模式', 850, 12)


        self.c_start_detection = create_button(self.analyse_page_original, '检测',
                                         c_start_detection_func,
                                         12, 1, 200, 40)

        self.c_save_button = create_button(self.analyse_page_original, '保存至..',
                                               open_new_window_save,
                                               12, 1, 800, 40)
        self.c_feature_extraction = create_label1(self.analyse_page_original, '过孔事件特征提取', 10, 50)

        self.c_up_or_down = create_label(self.analyse_page_original, '事件类型:', 10, 80)
        def on_select1(i, combo):
            if i == 0:
                list = []
                selected_item = self.b_combo.get()
                list.append(str(selected_item))
                return list
            if i == 1:
                selected_item = combo.get()
                if selected_item == '正向脉冲':
                    self.c_detection_three = 1
                elif selected_item == '负向脉冲':
                    self.c_detection_three = 2
                else:
                    self.c_detection_three = 3

        options1 = ['负向脉冲', '正向脉冲', '双向脉冲']
        self.c_combo1 = ttk.Combobox(self.analyse_page_original, values=options1, width=10, state='readonly')
        self.c_combo1.current(0)
        self.c_combo1.bind("<<ComboboxSelected>>", lambda event: on_select1(1, self.c_combo1))
        self.c_combo1.place(x=90, y=80)

        self.c_detection_sigma_label = create_label(self.analyse_page_original, '检测阈值（σ）:', 10, 110)
        self.c_detection_sigma_param = create_entry1(self.analyse_page_original, 10, 1210 - 1070, 110, 5)

        def c_detection_threshold_func():
            self.c_detection_threshold = not self.c_detection_threshold
            self.c_detection_sigma = self.c_detection_sigma_param.get()
            if self.c_filter == True:
                self.c_baseline = dynamic_detection.calculate_dynamic_baseline_fast_trimmed_py(self.c1_y_filter,
                                                                                               (float(
                                                                                                   self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000)
                self.c_std_down, self.c_std_up = np.array(
                    dynamic_detection.detection_y_py(self.c1_y_filter, (
                                float(self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000,
                                                     float(self.c_detection_sigma)))
            else:
                self.c_baseline = dynamic_detection.calculate_dynamic_baseline_fast_trimmed_py(self.c1_y,
                                                                                               (float(
                                                                                                   self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000)
                self.c_std_down, self.c_std_up = np.array(
                    dynamic_detection.detection_y_py(self.c1_y, (
                            float(self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000,
                                                     float(self.c_detection_sigma)))
            self.update_c_plot_current(self.c1_x, self.c1_y)
        def c_filter_func():
            self.c_filter = not self.c_filter
            self.c_detection_sigma = self.c_detection_sigma_param.get()
            if self.c_file_folder_choice == False:
                if self.c_filter == True:
                    cutoff_frequency = int(self.c_filter_frequency_param.get())  # 截止频率
                    nyquist_frequency = 0.5 * self.c_frequency  # 奈奎斯特频率
                    normal_cutoff = cutoff_frequency / nyquist_frequency
                    b, a = signal.butter(1, normal_cutoff, btype='low', analog=False)
                    self.c1_y_filter = signal.filtfilt(b, a, self.c1_y)

                    self.c_baseline = dynamic_detection.calculate_dynamic_baseline_fast_trimmed_py(self.c1_y_filter,
                                                                                                   (float(
                                                                                                       self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000)
                    self.c_std_down, self.c_std_up = np.array(
                        dynamic_detection.detection_y_py(self.c1_y_filter, (
                                float(self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000,
                                                         float(self.c_detection_sigma)))
                    self.update_c_plot_current(self.c1_x, self.c1_y_filter)
                else:
                    self.c_baseline = dynamic_detection.calculate_dynamic_baseline_fast_trimmed_py(self.c1_y,
                                                                                                   (float(
                                                                                                       self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000)
                    self.c_std_down, self.c_std_up = np.array(
                        dynamic_detection.detection_y_py(self.c1_y, (
                                float(self.c_detection_baseline_std_window_param.get()) * self.c_frequency) / 1000,
                                                         float(self.c_detection_sigma)))
                    self.update_c_plot_current(self.c1_x, self.c1_y)
        def c_event_func():
            self.c_event = not self.c_event
            if self.c_event == True:
                self.update_c_plot_current(self.c1_x, self.c1_y)
            else:
                self.update_c_plot_current(self.c1_x, self.c1_y)
        self.c1_var_event = tk.IntVar()
        self.c1_detection_event = tk.Checkbutton(self.analyse_page_original,
                                                       variable=self.c1_var_event,
                                                       font=("Arial", 1), command=lambda: c_event_func())
        self.c1_detection_event.place(x=10, y=140)
        self.c1_detection_sigma_look = create_label(self.analyse_page_original, '事件', 40, 140)

        self.c2_var_detection_sigma = tk.IntVar()
        self.c2_detection_sigma_choice = tk.Checkbutton(self.analyse_page_original,
                                                       variable=self.c2_var_detection_sigma,
                                                       font=("Arial", 1), command=lambda: c_detection_threshold_func())
        self.c2_detection_sigma_choice.place(x=90, y=140)
        self.c2_detection_sigma_look = create_label(self.analyse_page_original, '阈值', 120, 140)

        self.c2_var_filter = tk.IntVar()
        self.c2_var_filter_choice = tk.Checkbutton(self.analyse_page_original,
                                                        variable=self.c2_var_filter,
                                                        font=("Arial", 1), command=lambda: c_filter_func())
        self.c2_var_filter_choice.place(x=330, y=45)
        self.c2_var_filter_look = create_label(self.analyse_page_original, '滤波(截止频率)', 360, 45)
        self.c_filter_frequency_param = create_entry1(self.analyse_page_original, 10, 480, 45, 10000)


        self.c_detection_mintime_label = create_label(self.analyse_page_original, '最小持续时间:', 240, 80)
        self.c_detection_mintime_param = create_entry1(self.analyse_page_original, 10, 360, 80, 0)
        self.c_detection_mintime_ms = create_label(self.analyse_page_original, 'ms', 440, 80)

        self.c_detection_maxtime_label = create_label(self.analyse_page_original, '最大持续时间:', 240, 110)
        self.c_detection_maxtime_param = create_entry1(self.analyse_page_original, 10, 360, 110, 1000)
        self.c_detection_maxtime_ms = create_label(self.analyse_page_original, 'ms', 440, 110)

        self.c_detection_baseline_std_window_label = create_label(self.analyse_page_original, '基线/std计算步长:',
                                                            240, 140)
        self.c_detection_baseline_std_window_param = create_entry1(self.analyse_page_original, 10, 390, 140,
                                                                   200)
        self.c_detection_baseline_std_window_ms = create_label(self.analyse_page_original, 'ms', 470, 140)

        def on_select(number, combo):
            selected_item = combo.get()
            if number == 1:
                if selected_item == '基线':
                    self.c_undate_plot_histogram(self.c_baseline_list)
                elif selected_item == '振幅':
                    self.c_undate_plot_histogram(self.c_amplitude_list)
                elif selected_item == '积分':
                    self.c_undate_plot_histogram(self.c_integral_list)
                elif selected_item == '持续时间':
                    self.c_undate_plot_histogram(self.c_duration_list)
                elif selected_item == '钝度':
                    self.c_undate_plot_histogram(self.c_bluntness_list)
                elif selected_item == '脉冲不对称':
                    self.c_undate_plot_histogram(self.c_asymmetry_list)
                elif selected_item == '横轴惯性':
                    self.c_undate_plot_histogram(self.c_x_inertia_list)
                elif selected_item == '纵轴惯性':
                    self.c_undate_plot_histogram(self.c_y_inertia_list)
            elif number == 2:
                if selected_item == '基线':
                    self.c_undate_plot_histogram_2(self.c_events_list_baseline_2)
                elif selected_item == '振幅':
                    self.c_undate_plot_histogram_2(self.c_events_list_amplitude_2)
                elif selected_item == '积分':
                    self.c_undate_plot_histogram_2(self.c_events_list_integral_2)
                elif selected_item == '持续时间':
                    self.c_undate_plot_histogram_2(self.c_events_list_duration_2)
                elif selected_item == '钝度':
                    self.c_undate_plot_histogram_2(self.c_events_list_bluntness_2)
                elif selected_item == '脉冲不对称':
                    self.c_undate_plot_histogram_2(self.c_events_list_asymmetry_2)
                elif selected_item == '横轴惯性':
                    self.c_undate_plot_histogram_2(self.c_events_list_x_inertia_2)
                elif selected_item == '纵轴惯性':
                    self.c_undate_plot_histogram_2(self.c_events_list_y_inertia_2)
        options1 = ['基线', '振幅', '积分', '持续时间', '钝度', '脉冲不对称', '横轴惯性', '纵轴惯性']

        self.c_combo2 = ttk.Combobox(self.analyse_page_original, values=options1, width=10, state='readonly')
        self.c_combo2.current(0)
        self.c_combo2.bind("<<ComboboxSelected>>", lambda event: on_select(1, self.c_combo2))
        self.c_combo2.place(x=800, y=660)

        self.c_plot_current()
        self.toolbar_current = NavigationToolbar2Tk(self.canvas_current_c, self.analyse_page_original)
        self.toolbar_current.update()
        self.toolbar_current.place(x=500, y=100)
        self.c_show_table_tk()
        self.c_plot_event1()
        self.c_plot_2d()
        self.c_plot_histogram()
        self.c_plot_2d_2()
        self.c_plot_histogram_2_2()

        def open_new_window_2():
            new_window = tk.Toplevel(self.root)
            new_window.title("打开文件")
            new_window.geometry("330x120")
            new_window.grab_set()

            path_entry = create_entry(new_window, 30, 69, 30)
            create_label(new_window, "文件信息", 3, 30)

            browse_button = tk.Button(new_window, text="浏览", bg='#e5e5e5', font=('', 9),
                                      command=lambda: browse_file1(path_entry,
                                                                  [("ABF files", "*.csv")]))

            browse_button.place(x=287, y=30)

            confirm_button = tk.Button(new_window, text="确定", bg='#e5e5e5', font=('', 9),
                                       command=lambda: on_confirm_2(new_window, path_entry.get()))
            confirm_button.place(x=140, y=80)
        def on_confirm_2(window, folder_path):
            if folder_path:
                self.c_preview_file_name = folder_path
                if folder_path not in self.c_file_path_list_preview:
                    add_file_name(folder_path)
                    self.c_file_path_list_preview.append(folder_path)

                pass
            else:
                messagebox.showwarning("警告", "请传入有效路径！")
            window.destroy()

        def add_file_name(filename):
            self.c_text_display.config(state=tk.NORMAL)
            line_index = self.c_text_display.index(tk.END)
            self.c_text_display.insert(tk.END, f'{filename}\n')
            self.c_text_display.yview(tk.END)
            self.c_text_display.config(state=tk.DISABLED)

        def c_add_statistics_text_func(window, category, color):
            file_exists = any(
                item[0] == self.c_preview_file_name
                for item in self.c_file_path_list_statistics
            )
            if not file_exists:
                self.c_file_path_list_statistics.append([self.c_preview_file_name, category, color])
                self.c_text_display_2.config(state=tk.NORMAL)
                line_index = self.c_text_display.index(tk.END)
                self.c_text_display_2.insert(tk.END, f'{self.c_preview_file_name, category, color }\n')
                self.c_text_display_2.yview(tk.END)
                self.c_text_display_2.config(state=tk.DISABLED)
            window.destroy()

        def c_delete_selected_item():

            try:
                self.c_text_display_2.config(state=tk.NORMAL)

                next_line = int(self.c_line_number) + 1
                self.c_text_display_2.delete(f"{self.c_line_number}.0", f"{next_line}.0")
                self.c_text_display_2.config(state=tk.DISABLED)

                if hasattr(self, 'c_file_path_list_statistics') and hasattr(self, 'c_statistics_file_name'):
                    file_name = self.c_statistics_file_name[0] if self.c_statistics_file_name else None
                    if file_name:
                        for i, item in enumerate(self.c_file_path_list_statistics):
                            if item[0] == file_name:
                                del self.c_file_path_list_statistics[i]
                                print(f"已从数据列表中删除: {file_name}")
                                break

                self.c_text_display_2.tag_remove("highlight", "1.0", "end")
                self.c_line_number = None
                self.c_statistics_file_name = None
                print(self.c_file_path_list_statistics)
            except Exception as e:
                print(f"删除失败: {str(e)}")


        self.c_open_file_2 = create_button(self.analyse_page_original, '打开文件',
                                         open_new_window_2,
                                         12, 1, 960, 10)
        def open_new_window_category_color():
            new_window = tk.Toplevel(self.root)
            new_window.title("类目")
            new_window.geometry("330x120")
            new_window.grab_set()

            path_entry = create_entry(new_window, 10, 69, 30)
            create_label(new_window, "类目", 3, 30)

            path_entry1 = create_entry(new_window, 10, 69, 50)
            create_label1(new_window, "颜色", 3, 50)



            confirm_button = tk.Button(new_window, text="确定", bg='#e5e5e5', font=('', 9),
                                       command=lambda: c_add_statistics_text_func(new_window, path_entry.get(), path_entry1.get()))
            confirm_button.place(x=140, y=80)

        self.c_text_display_func()
        self.c_combo2 = ttk.Combobox(self.analyse_page_original, values=options1, width=10, state='readonly')
        self.c_combo2.current(0)
        self.c_combo2.bind("<<ComboboxSelected>>", lambda event: on_select(2, self.c_combo2))
        self.c_combo2.place(x=1750, y=350)
        self.c_add_file_path = create_label(self.analyse_page_original, '↓↓↓↓↓', 1000, 480)
        self.c_add_file_2 = create_button(self.analyse_page_original, '添加',
                                           open_new_window_category_color,
                                           12, 1, 1100, 470)
        self.c_text_display_func_2()
        self.c_delete_file_2 = create_button(self.analyse_page_original, '删除',
                                          c_delete_selected_item,
                                          12, 1, 1300, 470)
        self.c_plot_2d_3()
        def update_2d_3():
            self.ax_c_2d_3.cla()
            self.df_dict = {}
            for item in self.c_file_path_list_statistics:
                file_path, category, color = item
                df = pd.read_csv(file_path, encoding='gbk')
                duration, amplitude = df['持续时间(ms)'], df['振幅(nA)']
                self.ax_c_2d_3.scatter(duration, amplitude, c=f'{color}', label=f'{category}', alpha=0.4, s=10)
            # self.ax3.grid(True)
            self.ax_c_2d_3.set_xlabel("时间(ms)")
            self.ax_c_2d_3.set_ylabel("振幅(nA)")

            # self.ax3.set_xticks([])
            # self.ax3.set_yticks([])
            self.ax_c_2d_3.legend(fontsize=10)
            self.canvas_event_c_2d_3.draw()
        self.c_update_2d_3 = create_button(self.analyse_page_original, '更新',
                                             update_2d_3,
                                             12, 1, 1500, 900)
    def function1(self, command):
        if command == "CBD打孔":
            if self.page_show_number != 1 and self.control_page_original == None:
                self.add_control_page()
                self.page_show_number = 1
            else:
                self.control_page_original.place(x=0, y=0)
                self.collection_eONE_page_original.place_forget()
                self.analyse_page_original.place_forget()

        if command == '数据采集':
            if self.page_show_number != 2 and self.collection_eONE_page_original == None:
                self.add_collection_eONE_page()
                self.page_show_number = 2
            else:
                self.collection_eONE_page_original.place(x=0, y=0)
                self.control_page_original.place_forget()
                self.analyse_page_original.place_forget()
        if command == '数据分析':
            if self.page_show_number != 3 and self.analyse_page_original == None:
                self.add_analyse_page()
                self.page_show_number = 3
            else:
                self.analyse_page_original.place(x=0, y=0)
                self.control_page_original.place_forget()
                self.collection_eONE_page_original.place_forget()


def create_label(window, text, x, y):
    label = tk.Label(window, text=text, bg='#e5e5e5', font=("SimHei", 11))
    label.place(x=x, y=y)
    return label

def create_label1(window, text, x, y):
    label = tk.Label(window, text=text, bg='#e5e5e5', font=("SimHei", 13))
    label.place(x=x, y=y)
    return label
def create_label2(window, text, x, y):
    label = tk.Label(window, text=text, bg='#e5e5e5', font=("SimHei", 30), fg='black')
    label.place(x=x, y=y)
    return label

def create_entry(window, width, x, y):
    entry = tk.Entry(window, width=width)
    entry.place(x=x, y=y)
    return entry
def create_entry1(window, width, x, y, default_value=""):
    entry = tk.Entry(window, width=width, justify='center', disabledforeground='black', disabledbackground='#e5e5e5')
    entry.place(x=x, y=y)
    entry.insert(0, default_value)
    return entry
def create_entry2(window, width, x, y, default_value=""):
    entry = tk.Entry(window, width=width, justify='center', disabledforeground='black', disabledbackground='#e5e5e5')
    entry.place(x=x, y=y)
    entry.insert(0, default_value)
    return entry
def create_button(window, text, command, width, height, x, y):
    button = tk.Button(window, text=text, command=command, width=width, height=height, bg='#e5e5e5').place(x=x, y=y)
    return button
def create_button_left(window, text, command, width, height, x, y):
    button = tk.Button(window, text=text, command=command, width=width, height=height, anchor='w', bg='#e5e5e5').place(x=x, y=y)
    return button
def create_button1(window, text, command, width, height, x, y, number):
    button = tk.Button(window, text=text, command=command, width=width, height=height, bg='#e5e5e5', font=("SimHei", number, "bold"))
    button.place(x=x, y=y)
    return button
def browse_file(entry):
    folder_path = filedialog.askdirectory(title="选择文件夹")
    if folder_path:
        entry.delete(0, tk.END)
        entry.insert(0, folder_path)
def browse_file1(entry, filetypes):
    filename = filedialog.askopenfilename(filetypes=filetypes)
    if filename:
        entry.delete(0, tk.END)
        entry.insert(0, filename)

if __name__ == '__main__':

    app = show_page()

    app.root.mainloop()