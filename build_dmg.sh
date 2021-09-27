#!/bin/sh

###############################################################################
# 定义QT目录
QT_DIR=/opt/Qt5.12.2/5.12.2/gcc_64
# 定义opencv目录
OPENCV_DIR=/home/xiaoming/Desktop/opencv

# 定义版本号
YVYVIEWER_MAJARVERSION="0"
YVYVIEWER_SUBVERSION="4"
YVYVIEWER_REVISION="6"
###############################################################################


###############################################################################
export PATH=$QT_DIR/bin:$PATH
export LD_LIBRARY_PATH=$OPENCV_DIR/lib:$QT_DIR/lib:$LD_LIBRARY_PATH
export QT_PLUGIN_PATH=$QT_DIR/plugins:$QT_PLUGIN_PATH
export QML2_IMPORT_PATH=$QT_DIR/qml:$QML2_IMPORT_PATH
# 合成版本号
YVYVIEWER_VERSION="V"$YVYVIEWER_MAJARVERSION""$YVYVIEWER_SUBVERSION""$YVYVIEWER_REVISION""
# 编译
rm -rf .qmake.stash Makefile
qmake -makefile
make
cp -R ./test ./build_release/out/YUVviewer.app/contents/resources/test
cd ./build_release/out
# 打包
macdeployqt YUVviewer.app
cp /usr/local/lib/libopencv_imgproc.4.0.dylib ./YUVviewer.app/Contents/Frameworks/libopencv_imgproc.4.0.dylib
cp /usr/local/lib/libopencv_core.4.0.dylib ./YUVviewer.app/Contents/Frameworks/libopencv_core.4.0.dylib
cd ./YUVviewer.app/Contents/MacOS
otool -L YUVviewer
cd ./build_release/out
macdeployqt YUVviewer.app -dmg -verbose=2
echo build success!
###############################################################################
