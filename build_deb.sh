#!/bin/sh

# 定义版本号
YVYVIEWER_MAJARVERSION="0"
YVYVIEWER_SUBVERSION="3"
YVYVIEWER_REVISION="4"

YVYVIEWER_VERSION="V"$YVYVIEWER_MAJARVERSION""$YVYVIEWER_SUBVERSION""$YVYVIEWER_REVISION""

rm -rf ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64 
rm -f ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64.deb

cp -r ./dpkg/YUVviewer ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64
mkdir -p ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64/opt/YUVviewer
cp ./build-YUVviewer-Desktop_Qt_5_12_2_GCC_64bit-Release/YUVviewer ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64/opt/YUVviewer/YUVviewer

export PATH=/opt/Qt5.12.2/5.12.2/gcc_64/bin:$PATH
export LD_LIBRARY_PATH=/home/xiaoming/Desktop/opencv/lib:/opt/Qt5.12.2/5.12.2/gcc_64/lib:$LD_LIBRARY_PATH
export QT_PLUGIN_PATH=/opt/Qt5.12.2/5.12.2/gcc_64/plugins:$QT_PLUGIN_PATH
export QML2_IMPORT_PATH=/opt/Qt5.12.2/5.12.2/gcc_64/qml:$QML2_IMPORT_PATH
./tools/linuxdeployqt ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64/opt/YUVviewer/YUVviewer -appimage
rm -rf ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64/opt/YUVviewer/doc ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64/opt/YUVviewer/default.png ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64/opt/YUVviewer/AppRun ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64/opt/YUVviewer/default.desktop
cp ./img/ico.png ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64/opt/YUVviewer/YUVviewer.png

sed -i "s/Version: 0.33/Version: $YVYVIEWER_MAJARVERSION.$YVYVIEWER_SUBVERSION$YVYVIEWER_REVISION/g" ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64/DEBIAN/control
InstalledSize=94515.2 # TODO:
sed -i "s/Installed-Size: 81203.2/Installed-Size: $InstalledSize/g" ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64/DEBIAN/control

chmod 755 ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64/* -R

dpkg -b ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64 ./dpkg/YUVviewer_Linux_"$YVYVIEWER_VERSION"_x86_64.deb
