#!/usr/bin/env python

import os
import sys
from setuptools import setup, Extension

if not hasattr(sys, "hexversion") or sys.hexversion < 0x02040000:
    raise Error("Python 2.4 or newer is required")

__version__ = "0.1.0"

setup(
    name="_attachsql",
    description="A Python wrapper for the libAttachSQL connector for MySQL servers",
    version=__version__,
    author="Andrew Hutchings <andrew@linuxjedi.co.uk>",
    ext_modules=[
                Extension("_attachsql",
                    sources=['src/module.c',
                             'src/util.c'
                            ],
                    libraries=['attachsql']),
                ]
)
