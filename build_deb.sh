#!/bin/sh

###############################################################################
# 定义Qt目录
QT_DIR=/opt/Qt6.2.0/6.2.0/gcc_64
# 定义opencv目录
OPENCV_DIR=/home/xiaoming/Desktop/opencv

# 定义版本号
YUVVIEWER_MAJARVERSION="0"
YUVVIEWER_SUBVERSION="5"
YUVVIEWER_REVISION="3"
###############################################################################


###############################################################################
export PATH=$QT_DIR/bin:$PATH
export LD_LIBRARY_PATH=$OPENCV_DIR/lib:$QT_DIR/lib:$LD_LIBRARY_PATH
export QT_PLUGIN_PATH=$QT_DIR/plugins
export QML2_IMPORT_PATH=$QT_DIR/qml
# 合成版本号
YUVVIEWER_VERSION="V"$YUVVIEWER_MAJARVERSION$YUVVIEWER_SUBVERSION$YUVVIEWER_REVISION
# 编译
rm -rf .qmake.stash Makefile
qmake ./YUVviewer.pro -spec linux-g++ CONFIG+=qtquickcompiler
make clean
make -j8 
# clean打包目录
rm -rf ./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64 
rm -f ./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64.deb
# 构建打包目录
cp -r ./dpkg/YUVviewer ./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64
# 使用linuxdeployqt拷贝依赖so库到打包目录
export QMAKE=$QT_DIR/bin/qmake
./tools/linuxdeploy-x86_64.AppImage --executable=./build_release/out/YUVviewer --appdir=./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt --plugin=qt
rm -rf ./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/apprun-hooks
mv ./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/usr ./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/YUVviewer
mv ./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/YUVviewer/bin/YUVviewer ./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/YUVviewer/YUVviewer
mv ./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/YUVviewer/bin/qt.conf ./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/YUVviewer/qt.conf
rm -rf ./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/YUVviewer/bin
sed -i "s/Prefix = ..\//Prefix = .\//g" ./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/YUVviewer/qt.conf
chrpath -r "\$ORIGIN/./lib" ./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/YUVviewer/YUVviewer
rm -rf ./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/YUVviewer/share
cp ./img/ico.png ./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/YUVviewer/YUVviewer.png
mkdir -p ./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/YUVviewer
cp -r ./test ./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/YUVviewer/test
# 配置打包信息
sed -i "s/#VERSION#/$YUVVIEWER_MAJARVERSION.$YUVVIEWER_SUBVERSION$YUVVIEWER_REVISION/g" ./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/DEBIAN/control
SIZE=$(du -sh -B 1024 ./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64 | sed "s/.\///g")
InstalledSize=$SIZE
sed -i "s/#SIZE#/$InstalledSize/g" ./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/DEBIAN/control
chmod 755 ./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/* -R
# 打包
dpkg -b ./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64 ./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64.deb
echo build success!
###############################################################################
