from setuptools import setup, Extension
from Cython.Build import cythonize
import numpy as np

ext_modules = [
    Extension(
        "dynamic_detection",
        sources=["dynamic_detection_wrapper.pyx", "dynamic_detection.cpp"],
        language="c++",
        include_dirs=[np.get_include()],
        extra_compile_args=["-O3"],
    )
]

setup(
    name="dynamic_detection_v2",
    ext_modules=cythonize(ext_modules, language_level=3)
)