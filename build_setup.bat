@echo off

set "PATH=D:\Qt\Qt5.9.2\5.9.2\mingw53_32\bin;D:/Qt/Qt5.9.2/Tools/mingw530_32\bin;C:\Program Files (x86)\Inno Setup 6;%PATH%"

if exist ".\InnoSetup\build" (
    rmdir /Q /S .\InnoSetup\build
)

xcopy /y .\build-YUVviewer-Desktop_Qt_5_9_2_MinGW_32bit-Release\release\YUVviewer.exe .\InnoSetup\build\

windeployqt --dir .\InnoSetup\build .\InnoSetup\build\YUVviewer.exe

xcopy /y D:\Qt\opencv4.2.0\x64\mingw\bin\libopencv_imgproc420.dll .\InnoSetup\build\
xcopy /y D:\Qt\opencv4.2.0\x64\mingw\bin\libopencv_core420.dll .\InnoSetup\build\

echo "wait inno build setup..."
compil32 /cc ".\InnoSetup\build_setup.iss"
echo "build success!"

pause
