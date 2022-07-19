#!/bin/sh

###############################################################################
# 定义Qt目录
QT_DIR=/opt/Qt6.2.0/6.2.0/gcc_64
# 定义opencv目录
OPENCV_DIR=/home/xiaoming/Desktop/opencv

# 定义版本号
YUVVIEWER_MAJARVERSION="0"
YUVVIEWER_SUBVERSION="4"
YUVVIEWER_REVISION="10"
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
mkdir -p ./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/YUVviewer
cp -r ./test ./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/YUVviewer/test
cp ./build_release/out/YUVviewer ./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/YUVviewer/YUVviewer
# 使用linuxdeployqt拷贝依赖so库到打包目录
./tools/linuxdeployqt ./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/YUVviewer/YUVviewer -appimage
# 由于linuxdeployqt目前（截止2022.07.18）还不支持Qt6的plugins依赖分析，这里我们手动拷贝
mkdir ./dpkg/TTFviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins
mkdir ./dpkg/TTFviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/iconengines
cp $QT_PLUGIN_PATH/iconengines/libqsvgicon.so ./dpkg/TTFviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/iconengines/libqsvgicon.so
mkdir ./dpkg/TTFviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/imageformats
cp $QT_PLUGIN_PATH/imageformats/libqgif.so ./dpkg/TTFviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/imageformats/libqgif.so
cp $QT_PLUGIN_PATH/imageformats/libqicns.so ./dpkg/TTFviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/imageformats/libqicns.so
cp $QT_PLUGIN_PATH/imageformats/libqico.so ./dpkg/TTFviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/imageformats/libqico.so
cp $QT_PLUGIN_PATH/imageformats/libqjpeg.so ./dpkg/TTFviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/imageformats/libqjpeg.so
cp $QT_PLUGIN_PATH/imageformats/libqsvg.so ./dpkg/TTFviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/imageformats/libqsvg.so
cp $QT_PLUGIN_PATH/imageformats/libqtga.so ./dpkg/TTFviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/imageformats/libqtga.so
cp $QT_PLUGIN_PATH/imageformats/libqtiff.so ./dpkg/TTFviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/imageformats/libqtiff.so
cp $QT_PLUGIN_PATH/imageformats/libqwbmp.so ./dpkg/TTFviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/imageformats/libqwbmp.so
cp $QT_PLUGIN_PATH/imageformats/libqwebp.so ./dpkg/TTFviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/imageformats/libqwebp.so
mkdir ./dpkg/TTFviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/platforminputcontexts
cp $QT_PLUGIN_PATH/platforminputcontexts/libcomposeplatforminputcontextplugin.so ./dpkg/TTFviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/platforminputcontexts/libcomposeplatforminputcontextplugin.so
cp $QT_PLUGIN_PATH/platforminputcontexts/libibusplatforminputcontextplugin.so ./dpkg/TTFviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/platforminputcontexts/libibusplatforminputcontextplugin.so
mkdir ./dpkg/TTFviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/platforms
cp $QT_PLUGIN_PATH/platforms/libqxcb.so ./dpkg/TTFviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/platforms/libqxcb.so
mkdir ./dpkg/TTFviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/xcbglintegrations
cp $QT_PLUGIN_PATH/xcbglintegrations/libqxcb-egl-integration.so ./dpkg/TTFviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/xcbglintegrations/libqxcb-egl-integration.so
cp $QT_PLUGIN_PATH/xcbglintegrations/libqxcb-glx-integration.so ./dpkg/TTFviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/xcbglintegrations/libqxcb-glx-integration.so
cp $QT_DIR/lib/libQt6XcbQpa.so.6 ./dpkg/TTFviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/TTFviewer/lib/libQt6XcbQpa.so.6
cp $QT_DIR/lib/libQt6OpenGL.so.6 ./dpkg/TTFviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/TTFviewer/lib/libQt6OpenGL.so.6
rm -rf ./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/YUVviewer/doc ./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/YUVviewer/default.png ./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/YUVviewer/AppRun ./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/YUVviewer/default.desktop
cp ./img/ico.png ./dpkg/YUVviewer_Linux_"$YUVVIEWER_VERSION"_x86_64/opt/YUVviewer/YUVviewer.png
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
