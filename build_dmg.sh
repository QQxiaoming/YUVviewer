#!/bin/sh

###############################################################################
# 定义QT目录
QT_DIR=/opt/Qt5.12.2/5.12.2/gcc_64
# 定义opencv目录
OPENCV_DIR=/home/xiaoming/Desktop/opencv

# 定义版本号
YVYVIEWER_MAJARVERSION="0"
YVYVIEWER_SUBVERSION="4"
YVYVIEWER_REVISION="3"
###############################################################################


###############################################################################
# 合成版本号
YVYVIEWER_VERSION="V"$YVYVIEWER_MAJARVERSION""$YVYVIEWER_SUBVERSION""$YVYVIEWER_REVISION""
# 编译
rm -rf .qmake.stash Makefile
qmake -makefile
make
cd ./build_release/out
# 打包
macdeployqt YUVviewer.app -dmg -verbose=2
echo build success!
###############################################################################
