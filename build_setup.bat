@echo off

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:: 定义Qt目录
set "QT_DIR=C:/Qt/Qt6.2.0/6.2.0/mingw81_32/bin"
set "QT_TOOLS_DIR=C:/Qt/Qt6.2.0/Tools/mingw810_32/bin"
:: 定义Inno Setup目录
set "INNO_SETUP_DIR=C:/Program Files (x86)/Inno Setup 6"
:: 定义opencv目录
set "OPENCV_DIR=D:/Qt/opencv4.2.0/x64/mingw/bin"
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: 定义版本号
set /p YUVVIEWER_VERSION=<version.txt
:: 设置环境变量
set "PATH=%QT_DIR%;%QT_TOOLS_DIR%;%INNO_SETUP_DIR%;%PATH%"
:: 编译
del .qmake.stash Makefile
if exist ".\build_debug" (
    rmdir /Q /S .\build_debug
)
if exist ".\build_release" (
    rmdir /Q /S .\build_release
)
lrelease YUVviewer.pro
qmake YUVviewer.pro -spec win32-g++
mingw32-make -j8
:: clean打包目录
if exist ".\InnoSetup\build" (
    rmdir /Q /S .\InnoSetup\build
)
:: 配置打包信息
copy /y .\InnoSetup\build_setup.iss .\InnoSetup\build_temp_setup.iss
.\tools\sed\sed.exe -i "s/#VERSION#/%YUVVIEWER_VERSION%/g" .\InnoSetup\build_temp_setup.iss
.\tools\sed\sed.exe -i "s/#VERSIONINFOVERSION#/%YUVVIEWER_VERSION%.000/g" .\InnoSetup\build_temp_setup.iss
del /f /q /a .\sed*
:: 构建打包目录
xcopy /y .\build_release\out\YUVviewer.exe .\InnoSetup\build\
xcopy /y .\test\* .\InnoSetup\build\test\
:: 使用windeployqt拷贝依赖dll库到打包目录
windeployqt --dir .\InnoSetup\build .\InnoSetup\build\YUVviewer.exe
xcopy /y "%OPENCV_DIR%\libopencv_imgproc420.dll" ".\InnoSetup\build\"
xcopy /y "%OPENCV_DIR%\libopencv_imgcodecs420.dll" ".\InnoSetup\build\"
xcopy /y "%OPENCV_DIR%\libopencv_core420.dll" ".\InnoSetup\build\"
xcopy /y "%QT_TOOLS_DIR%\*.dll" ".\InnoSetup\build\"
:: 打包
echo "wait inno build setup..."
iscc /q ".\InnoSetup\build_temp_setup.iss"
del .\InnoSetup\build_temp_setup.iss
echo "build success!"
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
