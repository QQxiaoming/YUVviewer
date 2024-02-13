#!/bin/sh

###############################################################################
# 定义Qt目录
QT_DIR=/opt/Qt6.2.0/6.2.0/gcc_64
# 定义opencv目录
OPENCV_DIR=/home/xiaoming/Desktop/opencv
###############################################################################


###############################################################################
# 定义版本号
YUVVIEWER_MAJARVERSION=$(< ./version.txt cut -d '.' -f 1)
YUVVIEWER_SUBVERSION=$(< ./version.txt cut -d '.' -f 2)
YUVVIEWER_REVISION=$(< ./version.txt cut -d '.' -f 3)
export PATH=$QT_DIR/bin:$PATH
export LD_LIBRARY_PATH=$OPENCV_DIR/lib:$QT_DIR/lib:$LD_LIBRARY_PATH
export QT_PLUGIN_PATH=$QT_DIR/plugins
export QML2_IMPORT_PATH=$QT_DIR/qml
# 合成版本号
YUVVIEWER_VERSION="V"$YUVVIEWER_MAJARVERSION$YUVVIEWER_SUBVERSION$YUVVIEWER_REVISION
# 编译
rm -rf .qmake.stash Makefile
$QT_DIR/bin/lrelease ./YUVviewer.pro
$QT_DIR/bin/qmake -makefile
make
cp -R ./test ./build_release/out/YUVviewer.app/Contents/Resources/test
cp ./tools/create-dmg/build-dmg.sh ./build_release/out/build-dmg.sh
cp ./tools/create-dmg/installer_background.png ./build_release/out/installer_background.png
cd ./build_release/out
# 打包
$QT_DIR/bin/macdeployqt YUVviewer.app
cp $OPENCV_DIR/lib/libopencv_imgproc.4.0.dylib ./YUVviewer.app/Contents/Frameworks/libopencv_imgproc.4.0.dylib
cp $OPENCV_DIR/lib/libopencv_imgcodecs.4.0.dylib ./YUVviewer.app/Contents/Frameworks/libopencv_imgcodecs.4.0.dylib
cp $OPENCV_DIR/lib/libopencv_core.4.0.dylib ./YUVviewer.app/Contents/Frameworks/libopencv_core.4.0.dylib
otool -L ./YUVviewer.app/Contents/MacOS/YUVviewer
./build-dmg.sh YUVviewer
cd ../../
mkdir dmgOut
cpu=$(sysctl -n machdep.cpu.brand_string)
ARCH="x86_64"
case $cpu in
  *Intel*) ARCH="x86_64" ;;
  *Apple*) ARCH="arm64" ;;
esac
cp ./build_release/out/YUVviewer.dmg ./dmgOut/YUVviewer_macos_"$YUVVIEWER_VERSION"_"$ARCH".dmg
echo build success!
###############################################################################
