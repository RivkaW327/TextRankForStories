# -*- coding: utf-8 -*-

from setuptools import setup, Extension
import pybind11
import os

ext_modules = [
    Extension(
        'textranker',
        sources=[
            "bindings.cpp",
            'text_ranker.cpp',
            'Paragraph.cpp',
            'IntervalTree.cpp'
        ],
        include_dirs=[
            pybind11.get_include(),
            'C:\\Users\\user\\Documents\\year2\\project\\TextRank\\TextRank', 
        ],
        language='c++',
        extra_compile_args=['-g'],  # ����� ��� ������
    ),
]
setup(
    name='textranker',
    version='0.1',
    ext_modules=ext_modules,
)
