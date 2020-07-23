#!/usr/bin/env python3
# encoding: utf-8

from distutils.core import setup, Extension

module = Extension('_streambar', sources = ['module.cpp'], extra_compile_args=['-I../'])

setup(name='streambar',
      version='0.1.0',
      description='Streambar extension for Python',
      ext_modules=[module],
      packages=['streambar'])