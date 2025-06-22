#!/usr/bin/env python
# coding=utf-8
def FlagsForFile(filename, **kwargs):
    return {
        'flags': [
            '-Wall',
            '-Wextra',
            '-Werror',
            '-std=c17',              # C 标准
            # 头文件路径
            '-I.',
            '-I../server',
            '-I/usr/include/',
            # 如果你用 C++
            # '-std=c++17'
        ],
        'do_cache': True
    }
