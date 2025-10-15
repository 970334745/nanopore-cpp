# -*- coding: utf-8 -*-
import numpy as np
cimport numpy as np
from libcpp.vector cimport vector
from libcpp.pair cimport pair

cdef extern from "dynamic_detection.h":
    vector[double] calculate_dynamic_baseline_fast_trimmed(const vector[double]& y, int window_size)
    vector[double] calculate_dynamic_std(const vector[double]& y, int window_size)
    pair[vector[double], vector[double]] detection_y(
        const vector[double]& y, int window_size, double sigma
    )

cdef vector[double] to_cpp_vector(py_obj):
    cdef vector[double] vec
    if isinstance(py_obj, np.ndarray):
        arr = np.asarray(py_obj, dtype=np.float64)
        vec.reserve(arr.size)
        for x in arr:
            vec.push_back(x)
    else:
        vec.reserve(len(py_obj))
        for x in py_obj:
            vec.push_back(x)
    return vec

cdef np.ndarray[np.float64_t, ndim=1] from_cpp_vector(const vector[double]& vec):
    cdef np.ndarray[np.float64_t, ndim=1] arr = np.empty(vec.size(), dtype=np.float64)
    for i in range(vec.size()):
        arr[i] = vec[i]
    return arr

def calculate_dynamic_baseline_fast_trimmed_py(py_y, int window_size):
    return from_cpp_vector(calculate_dynamic_baseline_fast_trimmed(to_cpp_vector(py_y), window_size))

def calculate_dynamic_std_py(py_y, int window_size):
    return from_cpp_vector(calculate_dynamic_std(to_cpp_vector(py_y), window_size))

def detection_y_py(py_y, int window_size, double sigma):
    cdef pair[vector[double], vector[double]] result = detection_y(to_cpp_vector(py_y), window_size, sigma)
    return (from_cpp_vector(result.first), from_cpp_vector(result.second))