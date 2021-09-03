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
export PATH=$QT_DIR/bin:$PATH
export LD_LIBRARY_PATH=$OPENCV_DIR/lib:$QT_DIR/lib:$LD_LIBRARY_PATH
export QT_PLUGIN_PATH=$QT_DIR/plugins:$QT_PLUGIN_PATH
export QML2_IMPORT_PATH=$QT_DIR/qml:$QML2_IMPORT_PATH
# 合成版本号
YVYVIEWER_VERSION="V"$YVYVIEWER_MAJARVERSION""$YVYVIEWER_SUBVERSION""$YVYVIEWER_REVISION""
# 编译
rm -rf .qmake.stash Makefile
qmake ./YUVviewer.pro -spec linux-g++ CONFIG+=qtquickcompiler
make clean
make -j8 
# clean打包目录
rm -rf ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64 
rm -f ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64.deb
# 构建打包目录
cp -r ./dpkg/YUVviewer ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64
mkdir -p ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64/opt/YUVviewer
cp -r ./test ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64/opt/YUVviewer/test
cp ./build_release/out/YUVviewer ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64/opt/YUVviewer/YUVviewer
# 使用linuxdeployqt拷贝依赖so库到打包目录
./tools/linuxdeployqt ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64/opt/YUVviewer/YUVviewer -appimage
rm -rf ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64/opt/YUVviewer/doc ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64/opt/YUVviewer/default.png ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64/opt/YUVviewer/AppRun ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64/opt/YUVviewer/default.desktop
cp ./img/ico.png ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64/opt/YUVviewer/YUVviewer.png
# 配置打包信息
sed -i "s/Version: 0.34/Version: $YVYVIEWER_MAJARVERSION.$YVYVIEWER_SUBVERSION$YVYVIEWER_REVISION/g" ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64/DEBIAN/control
cd ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64
SIZE=$(du -sh -B 1024 ./ | sed "s/.\///g")
InstalledSize=$SIZE
cd -
sed -i "s/Installed-Size: 81203.2/Installed-Size: $InstalledSize/g" ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64/DEBIAN/control
chmod 755 ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64/* -R
# 打包
dpkg -b ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64 ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64.deb
echo build success!
###############################################################################
