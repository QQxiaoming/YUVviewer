#-------------------------------------------------
#
# Project created by QtCreator 2020-03-22T12:27:18
#
#-------------------------------------------------

QT       += core gui
QT       += xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = YUVviewer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

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

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc

win32:{
    INCLUDEPATH += D:\Qt\opencv4.2.0\include\opencv2 \
                    D:\Qt\opencv4.2.0\include

    LIBS += D:\Qt\opencv4.2.0\x64\mingw\lib\libopencv_*.a

    VERSION = 0.3.4.000
    RC_ICONS = "img\ico.ico"
    QMAKE_TARGET_PRODUCT = "YUVviewer"
    QMAKE_TARGET_DESCRIPTION = "YUVviewer based on Qt 5.9.2 (MinGW 5.3.2, 32 bit)"
    QMAKE_TARGET_COPYRIGHT = "GNU General Public License v3.0"
}

unix:{
    QMAKE_RPATHDIR=$ORIGIN
    QMAKE_LFLAGS += -no-pie

    INCLUDEPATH += -I /home/xiaoming/Desktop/opencv/include/opencv4
    DEPENDPATH +=/home/xiaoming/Desktop/opencv/include/opencv4

    INCLUDEPATH += -I /home/xiaoming/Desktop/opencv/include
    DEPENDPATH +=/home/xiaoming/Desktop/opencv/include

    LIBS += -L /home/xiaoming/Desktop/opencv/lib/ -lopencv_videoio
    LIBS += -L /home/xiaoming/Desktop/opencv/lib/ -lopencv_video
    LIBS += -L /home/xiaoming/Desktop/opencv/lib/ -lopencv_stitching
    LIBS += -L /home/xiaoming/Desktop/opencv/lib/ -lopencv_photo
    LIBS += -L /home/xiaoming/Desktop/opencv/lib/ -lopencv_objdetect
    LIBS += -L /home/xiaoming/Desktop/opencv/lib/ -lopencv_ml
    LIBS += -L /home/xiaoming/Desktop/opencv/lib/ -lopencv_imgproc
    LIBS += -L /home/xiaoming/Desktop/opencv/lib/ -lopencv_imgcodecs
    LIBS += -L /home/xiaoming/Desktop/opencv/lib/ -lopencv_highgui
    LIBS += -L /home/xiaoming/Desktop/opencv/lib/ -lopencv_gapi
    LIBS += -L /home/xiaoming/Desktop/opencv/lib/ -lopencv_flann
    LIBS += -L /home/xiaoming/Desktop/opencv/lib/ -lopencv_features2d
    LIBS += -L /home/xiaoming/Desktop/opencv/lib/ -lopencv_dnn
    LIBS += -L /home/xiaoming/Desktop/opencv/lib/ -lopencv_core
    LIBS += -L /home/xiaoming/Desktop/opencv/lib/ -lopencv_calib3d
}
