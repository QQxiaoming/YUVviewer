###############################################################################
#                                                                             #
# YUVviewer 工程文件                                                           #  
#                                                                             # 
###############################################################################

###############################################################################
# 定义opencv lib路径
win32:{
    OPENCV_DIR=D:\Qt\opencv4.2.0
}
unix:{
    OPENCV_DIR=/home/xiaoming/Desktop/opencv
}
# 定义版本号路径
YVYVIEWER_VERSION=0.3.4
###############################################################################


###############################################################################
# 定义需要的Qt组件
QT       += core gui
QT       += xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# 编译配置
TARGET = YUVviewer
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += APP_VERSION="\\\"V$${YVYVIEWER_VERSION}\\\""
CONFIG += c++11

# 源文件配置
SOURCES += \
        YUVviewer.cpp \
        ImgViewer.cpp \
        YUVdecoder.cpp \
        configFile.cpp

HEADERS += \
        YUVviewer.h \
        ImgViewer.h \
        YUVdecoder.h \
        configFile.h

FORMS += \
        UI_YUVviewer.ui \
        UI_ImgViewer.ui

RESOURCES += \
    img.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# 输出配置
build_type =
CONFIG(debug, debug|release) {
    build_type = build_debug
} else {
    build_type = build_release
}

DESTDIR     = $$build_type/out
OBJECTS_DIR = $$build_type/obj
MOC_DIR     = $$build_type/moc
RCC_DIR     = $$build_type/rcc
UI_DIR      = $$build_type/ui

# 平台配置
win32:{
    INCLUDEPATH += $${OPENCV_DIR}\include\opencv2 \
                    $${OPENCV_DIR}\include

    LIBS += $${OPENCV_DIR}\x64\mingw\lib\libopencv_*.a

    VERSION = $${YVYVIEWER_VERSION}.000
    RC_ICONS = "img\ico.ico"
    QMAKE_TARGET_PRODUCT = "YUVviewer"
    QMAKE_TARGET_DESCRIPTION = "YUVviewer based on Qt 5.9.2 (MinGW 5.3.2, 32 bit)"
    QMAKE_TARGET_COPYRIGHT = "GNU General Public License v3.0"
}

unix:{
    QMAKE_RPATHDIR=$ORIGIN
    QMAKE_LFLAGS += -no-pie

    INCLUDEPATH += -I $${OPENCV_DIR}/include/opencv4
    DEPENDPATH +=$${OPENCV_DIR}/include/opencv4

    INCLUDEPATH += -I $${OPENCV_DIR}/include
    DEPENDPATH +=$${OPENCV_DIR}/include

    LIBS += -L $${OPENCV_DIR}/lib/ -lopencv_imgproc
    LIBS += -L $${OPENCV_DIR}/lib/ -lopencv_core
}
###############################################################################
