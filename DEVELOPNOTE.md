
# 编译说明

## windows

- 安装Qt、Inno Setup工具，编译opencv lib

- 修改partform_win32.pri文件以下内容

```s
###############################################################################
# 定义opencv lib路径
OPENCV_DIR=D:\Qt\opencv4.2.0
###############################################################################
```

- 修改build_setup.bat文件以下内容

```bat
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: 定义Qt目录
set "QT_DIR=D:/Qt/Qt5.9.2/5.9.2/mingw53_32/bin"
set "QT_TOOLS_DIR=D:/Qt/Qt5.9.2/Tools/mingw530_32/bin"
:: 定义Inno Setup目录
set "INNO_SETUP_DIR=C:/Program Files (x86)/Inno Setup 6"
:: 定义opencv目录
set "OPENCV_DIR=D:/Qt/opencv4.2.0/x64/mingw/bin"
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
```

- 执行build_setup.bat脚本等待编译打包完成。

## linux

- 安装Qt、patchelf工具，编译opencv lib以及linuxdeployqt工具(tools中已编译好适用于ubuntu18.04的linuxdeployqt工具)

- 修改partform_unix.pri文件以下内容

```s
###############################################################################
# 定义opencv lib路径
OPENCV_DIR=/home/xiaoming/Desktop/opencv
###############################################################################
```

- 修改build_deb.sh文件以下内容

```sh
###############################################################################
# 定义Qt目录
QT_DIR=/opt/Qt5.12.2/5.12.2/gcc_64
# 定义opencv目录
OPENCV_DIR=/home/xiaoming/Desktop/opencv
###############################################################################
```

- 执行build_deb.sh脚本等待编译打包完成。

## macos

- 安装Qt工具，编译opencv lib。

- 修改partform_unix.pri文件以下内容

```s
###############################################################################
# 定义opencv lib路径
OPENCV_DIR=/home/xiaoming/Desktop/opencv
###############################################################################
```

- 修改build_dmg.sh文件以下内容

```sh
###############################################################################
# 定义Qt目录
QT_DIR=/opt/Qt5.12.2/5.12.2/gcc_64
# 定义opencv目录
OPENCV_DIR=/home/xiaoming/Desktop/opencv
###############################################################################
```

- 执行build_dmg.sh脚本等待编译打包完成。

## python版本(停止维护)

运行 python ./src/old_code/YUVviewer.py