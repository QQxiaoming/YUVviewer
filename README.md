[![CI](https://github.com/QQxiaoming/YUVviewer/actions/workflows/ci.yml/badge.svg?branch=master)](https://github.com/QQxiaoming/YUVviewer/actions/workflows/ci.yml)
[![GitHub tag (latest SemVer)](https://img.shields.io/github/tag/QQxiaoming/YUVviewer.svg)](https://github.com/QQxiaoming/YUVviewer/releases)
[![GitHub All Releases](https://img.shields.io/github/downloads/QQxiaoming/YUVviewer/total.svg)](https://github.com/QQxiaoming/YUVviewer/releases)

# YUVviewer

本项目初期为基于PyQt、opencv-python的YUV文件查看浏览工具，即V0.3.2以及更低版本。

从V0.3.3，基于Qt、OpenCV重构YUV文件查看浏览工具，原python版本同步更新.

## 编译说明

### windows

- 安装Qt、Inno Setup工具，编译opencv lib

- 修改partform_win32.pri文件以下内容

```s
###############################################################################
# 定义opencv lib路径
OPENCV_DIR=D:\Qt\opencv4.2.0
# 定义版本号路径
YVYVIEWER_VERSION=0.3.4
###############################################################################
```

- 修改build_setup.bat文件以下内容

```bat
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: 定义QT目录
set "QT_DIR=D:/Qt/Qt5.9.2/5.9.2/mingw53_32/bin"
set "QT_TOOLS_DIR=D:/Qt/Qt5.9.2/Tools/mingw530_32/bin"
:: 定义Inno Setup目录
set "INNO_SETUP_DIR=C:/Program Files (x86)/Inno Setup 6"
:: 定义opencv目录
set "OPENCV_DIR=D:/Qt/opencv4.2.0/x64/mingw/bin"
:: 定义版本号
set "YVYVIEWER_VERSION=0.3.4"
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
```

- 执行build_setup.bat脚本等待编译打包完成。

### linux

- 安装Qt、patchelf工具，编译opencv lib以及linuxdeployqt工具(tools中已编译好适用于ubuntu18.04的linuxdeployqt工具)

- 修改partform_unix.pri文件以下内容

```s
###############################################################################
# 定义opencv lib路径
OPENCV_DIR=/home/xiaoming/Desktop/opencv
# 定义版本号路径
YVYVIEWER_VERSION=0.3.4
###############################################################################
```

- 修改build_deb.sh文件以下内容

```sh
###############################################################################
# 定义QT目录
QT_DIR=/opt/Qt5.12.2/5.12.2/gcc_64
# 定义opencv目录
OPENCV_DIR=/home/xiaoming/Desktop/opencv

# 定义版本号
YVYVIEWER_MAJARVERSION="0"
YVYVIEWER_SUBVERSION="3"
YVYVIEWER_REVISION="4"
###############################################################################
```

- 执行build_deb.sh脚本等待编译打包完成。

## python版本

运行 python ./src/YUVviewer.py
