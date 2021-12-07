#!/usr/bin/env python
# -*- coding:utf-8 -*-

import sys
import os
from PyQt5 import QtCore, QtWidgets
from PyQt5.QtWidgets import QFileDialog, QMessageBox, QToolTip
from PyQt5.QtCore import QPoint, QXmlStreamReader
from PyQt5.QtGui import QGuiApplication,QPixmap,QPainter
from PyQt5.QtSvg import QSvgRenderer
from UI_YUVviewer import Ui_YUVviewer
from configFile import ConfigFile
from ImgViewer import ImgViewer

VERSION = 'V0.4.8'
GIT_TAG = 'error:not found!'
with open('git_tag.inc') as fp:
    GIT_TAG = fp.read()

class YUVviewer(QtWidgets.QMainWindow, Ui_YUVviewer):
    UI_SINGLE = (0xff000000)
    UI_R      = (0xff0000|UI_SINGLE)
    UI_G      = (0x00ff00|UI_SINGLE)
    UI_B      = (0x0000ff|UI_SINGLE)
    UI_Y      = (0x808080|UI_SINGLE)
    UI_U      = (0xff00ff|UI_SINGLE)
    UI_V      = (0x00ffff|UI_SINGLE)
    UI_RG53   = (((0xff0000|0x5000000)<<32)|(0x00ff00|0x3000000))
    UI_GB35   = (((0x00ff00|0x3000000)<<32)|(0x0000ff|0x5000000))
    UI_BG53   = (((0x0000ff|0x5000000)<<32)|(0x00ff00|0x3000000))
    UI_GR35   = (((0x00ff00|0x3000000)<<32)|(0xff0000|0x5000000))
    UI_RG44   = (((0xff0000|0x4000000)<<32)|(0x00ff00|0x4000000))
    UI_GB44   = (((0x00ff00|0x4000000)<<32)|(0x0000ff|0x4000000))
    UI_BG44   = (((0x0000ff|0x4000000)<<32)|(0x00ff00|0x4000000))
    UI_GR44   = (((0x00ff00|0x4000000)<<32)|(0xff0000|0x4000000))
    frameSizeTypeDict = {
        'QQCIF':   ['88', '72'],
        'QQVGA':   ['160','120'],
        'QCIF':    ['176','144'],
        'HQVGA':   ['240','160'],
        'QVGA':    ['320','240'],
        'CIF':     ['352','288'],
        'WQVGA':   ['400','240'],
        'HVGA':    ['480','320'],
        'nHD':     ['640','340'],
        'VGA':     ['640','480'],
        'WVGA':    ['800','480'],
        'SVGA':    ['800','600'],
        'qHD':     ['960','540'],
        'DVGA':    ['960','640'],
        'XGA':     ['1024','768'],
        'HD':      ['1280','720'],
        'UVGA':    ['1280','960'],
        'SXGA':    ['1280','1024'],
        'HD+':     ['1600','900'],
        'UXGA':    ['1600','1200'],
        'FHD':     ['1920','1080'],
        'WUXGA':   ['1920','1200'],
        'FHD+':    ['2160','1440'],
        'QXGA':    ['2048','1536'],
        'QHD':     ['2560','1440'],
        'WQXGA':   ['2560','1600'],
        'QSXGA':   ['2560','2048'],
        'QHD+':    ['3200','1800'],
        'QUXGA':   ['3200','2400'],
        '4K UHD':  ['3840','2160'],
        '8K UHD':  ['7680','4320'],
    }
    YUVFormat_list = {
        "YV12":      [UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,
                      UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,
                      UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,
                      UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V],
        "YU12/I420": [UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,
                      UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,
                      UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,
                      UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U],
        "NV21":      [UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,
                      UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,
                      UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,
                      UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U],
        "NV12":      [UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,
                      UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,
                      UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,
                      UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V],
        "YUY2/YUYV": [UI_Y,UI_U,UI_Y,UI_V],
        "YVYU":      [UI_Y,UI_V,UI_Y,UI_U],
        "UYVY":      [UI_U,UI_Y,UI_V,UI_Y],
        "4:4:4":     [UI_Y,UI_U,UI_V],
        "RGB565_L":  [UI_GB35,UI_RG53],
        "RGB565_B":  [UI_RG53,UI_GB35],
        "BGR565_L":  [UI_GR35,UI_BG53],
        "BGR565_B":  [UI_BG53,UI_GR35],
        "RGB888":    [UI_R,UI_G,UI_B],
        "BayerBG":   [UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,
                      UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R],
        "BayerGB":   [UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,
                      UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G],
        "BayerRG":   [UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,
                      UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B],
        "BayerGR":   [UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,
                      UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G],
        "BayerBG_RAW12": [UI_B,UI_G,UI_BG44,UI_B,UI_G,UI_BG44,UI_B,UI_G,UI_BG44,UI_B,UI_G,UI_BG44,UI_B,UI_G,UI_BG44,UI_B,UI_G,UI_BG44,UI_B,UI_G,UI_BG44,UI_B,UI_G,UI_BG44,
                          UI_G,UI_R,UI_GR44,UI_G,UI_R,UI_GR44,UI_G,UI_R,UI_GR44,UI_G,UI_R,UI_GR44,UI_G,UI_R,UI_GR44,UI_G,UI_R,UI_GR44,UI_G,UI_R,UI_GR44,UI_G,UI_R,UI_GR44],
        "BayerGB_RAW12": [UI_G,UI_B,UI_GB44,UI_G,UI_B,UI_GB44,UI_G,UI_B,UI_GB44,UI_G,UI_B,UI_GB44,UI_G,UI_B,UI_GB44,UI_G,UI_B,UI_GB44,UI_G,UI_B,UI_GB44,UI_G,UI_B,UI_GB44,
                          UI_R,UI_G,UI_RG44,UI_R,UI_G,UI_RG44,UI_R,UI_G,UI_RG44,UI_R,UI_G,UI_RG44,UI_R,UI_G,UI_RG44,UI_R,UI_G,UI_RG44,UI_R,UI_G,UI_RG44,UI_R,UI_G,UI_RG44],
        "BayerRG_RAW12": [UI_R,UI_G,UI_RG44,UI_R,UI_G,UI_RG44,UI_R,UI_G,UI_RG44,UI_R,UI_G,UI_RG44,UI_R,UI_G,UI_RG44,UI_R,UI_G,UI_RG44,UI_R,UI_G,UI_RG44,UI_R,UI_G,UI_RG44,
                          UI_G,UI_B,UI_GB44,UI_G,UI_B,UI_GB44,UI_G,UI_B,UI_GB44,UI_G,UI_B,UI_GB44,UI_G,UI_B,UI_GB44,UI_G,UI_B,UI_GB44,UI_G,UI_B,UI_GB44,UI_G,UI_B,UI_GB44],
        "BayerGR_RAW12": [UI_G,UI_R,UI_GR44,UI_G,UI_R,UI_GR44,UI_G,UI_R,UI_GR44,UI_G,UI_R,UI_GR44,UI_G,UI_R,UI_GR44,UI_G,UI_R,UI_GR44,UI_G,UI_R,UI_GR44,UI_G,UI_R,UI_GR44,
                           UI_B,UI_G,UI_BG44,UI_B,UI_G,UI_BG44,UI_B,UI_G,UI_BG44,UI_B,UI_G,UI_BG44,UI_B,UI_G,UI_BG44,UI_B,UI_G,UI_BG44,UI_B,UI_G,UI_BG44,UI_B,UI_G,UI_BG44],
    }
    def __init__(self):
        super(YUVviewer, self).__init__()
        self.ui = Ui_YUVviewer()
        self.ui.setupUi(self)
        self.setWindowTitle('YUVviewer ' + VERSION)
        screen = QGuiApplication.primaryScreen().geometry()
        size = self.geometry()
        self.move((screen.width() - size.width()) // 2, (screen.height() - size.height()) // 2)

        self.ui.frameSizeType_ComboBox.setStyleSheet("combobox-popup: 0;")
        self.ui.YUVFormat_ComboBox.setStyleSheet("combobox-popup: 0;")
        self.ui.frameRate_ComboBox.setStyleSheet("combobox-popup: 0;")

        for key,value in self.frameSizeTypeDict.items():
            self.ui.frameSizeType_ComboBox.insertItem(self.ui.frameSizeType_ComboBox.count(), key)

        self.YUVviewerConfigFile = ConfigFile(os.path.join(os.getcwd(),'YUVViewer.ini'))
        if self.YUVviewerConfigFile.config_dict['frameSizeType'] == 'Other':
            self.ui.frameSizeType_Other_RadioButton.setChecked(True)
            self.ui.frameSizeType_ComboBox.setEnabled(False)
            self.ui.frameSize_Width_LineEdit.setText(self.YUVviewerConfigFile.config_dict['frameSize_Width'])
            self.ui.frameSize_Height_LineEdit.setText(self.YUVviewerConfigFile.config_dict['frameSize_Height'])
        else:
            self.ui.frameSizeType_Combo_RadioButton.setChecked(True)
            self.ui.frameSizeType_ComboBox.setEnabled(True)
            for key,value in self.frameSizeTypeDict.items():
                if key == self.YUVviewerConfigFile.config_dict['frameSizeType']:
                    self.ui.frameSizeType_ComboBox.setCurrentText(self.YUVviewerConfigFile.config_dict['frameSizeType'])
                    self.ui.frameSize_Width_LineEdit.setText(value[0])
                    self.ui.frameSize_Width_LineEdit.setFocusPolicy(QtCore.Qt.NoFocus)
                    self.YUVviewerConfigFile.config_dict['frameSize_Width'] = value[0]
                    self.ui.frameSize_Height_LineEdit.setText(value[1])
                    self.ui.frameSize_Height_LineEdit.setFocusPolicy(QtCore.Qt.NoFocus)
                    self.YUVviewerConfigFile.config_dict['frameSize_Height'] = value[1]
                    break
        
        currentIndex = 0
        for key,value in self.YUVFormat_list.items():
            if key == self.YUVviewerConfigFile.config_dict['YUVFormat']:
                color_list = value
                break
            currentIndex += 1
        self.ui.YUVFormat_ComboBox.setCurrentIndex(currentIndex)
        self.updateUiSvg(color_list)

        frameRate_list = ['30', '60', '120']
        currentIndex = frameRate_list.index(self.YUVviewerConfigFile.config_dict['frameRate'])
        self.ui.frameRate_ComboBox.setCurrentIndex(currentIndex)

        self.ui.startFrame_LineEdit.setText(self.YUVviewerConfigFile.config_dict['startFrame'])
        self.ui.endFrame_LineEdit.setText(self.YUVviewerConfigFile.config_dict['endFrame'])

        self.ui.YUVFormat_ComboBox.currentTextChanged.connect(self.changeFormat)
        self.ui.frameSizeType_Combo_RadioButton.clicked.connect(self.configComboBox)
        self.ui.frameSizeType_Other_RadioButton.clicked.connect(self.configOther)
        self.ui.frameSizeType_ComboBox.currentTextChanged.connect(self.changeFrameSizeType)

        self.ui.frameSize_Height_LineEdit.textEdited.connect(self.frameSizeHeightValidator)
        self.ui.frameSize_Width_LineEdit.textEdited.connect(self.frameSizeWidthValidator)
        self.ui.startFrame_LineEdit.textEdited.connect(self.startFrameValidator)
        self.ui.endFrame_LineEdit.textEdited.connect(self.endFrameValidator)

        self.ui.exchange_PushButton.clicked.connect(self.exchaneSize)
        self.ui.openFile_PushButton.clicked.connect(self.openFile)
        self.ui.openFolder_PushButton.clicked.connect(self.openFolder)
        self.ui.about_PushButton.clicked.connect(self.about)
        self.ui.help_PushButton.clicked.connect(self.help)

    def svgBoxSrc(self, x, y, w, c):
        if (c&self.UI_SINGLE) == self.UI_SINGLE:
            color = c & 0xffffff
            return '<line class=\"0\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"#%06x\" fill=\"none\" stroke-width=\"%d\" />\n' % (x,y+w/2,x+w,y+w/2,color,w)
        color0 = (c>>32) & 0xffffff;
        color1 = c & 0xffffff;
        fix_w = (w*(c>>56))/((c>>56)+((c & 0xff000000)>>24))
        return '<line class=\"0\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"#%06x\" fill=\"none\" stroke-width=\"%d\" />\n' % (x,y+w/2,x+fix_w,y+w/2,color0,w) + '<line class=\"0\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"#%06x\" fill=\"none\" stroke-width=\"%d\" />\n' % (x+fix_w,y+w/2,x+w,y+w/2,color1,w)

    def svgBoxArraySrc(self, x, y, w, od, xn, yn, ca):
        num = len(ca)
        if ca == 0:
            return ''
        ret = ''
        for i in range(yn):
            for j in range(xn):
                ret += self.svgBoxSrc(x+(w+od)*j,y+(w+od)*i,w,ca[((i*xn+j)%num)])
        return ret

    def updateUiSvg(self,color_list):
        svgXmlStreamReader = QXmlStreamReader('<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n' +
                            self.svgBoxArraySrc(10,10,40,5,24,4,color_list) +
                            '</svg>\n')
        svgRender = QSvgRenderer()
        svgRender.load(svgXmlStreamReader)
        svgPixmap = QPixmap(480,80)
        svgPixmap.fill(QtCore.Qt.transparent)
        svgPainter = QPainter(svgPixmap)
        svgRender.render(svgPainter)
        self.ui.label_8.setPixmap(svgPixmap)
        self.ui.label_8.setAlignment(QtCore.Qt.AlignCenter)
        svgPainter.end()

    def changeFormat(self,text):
        for key,value in self.YUVFormat_list.items():
            if key == self.ui.YUVFormat_ComboBox.currentText():
                color_list = value
                break
        self.updateUiSvg(color_list)
        self.ui.label_8.repaint()

    def configComboBox(self):
        self.ui.frameSizeType_ComboBox.setEnabled(True)
        for key,value in self.frameSizeTypeDict.items():
            if key == self.ui.frameSizeType_ComboBox.currentText():
                self.ui.frameSize_Width_LineEdit.setText(value[0])
                self.ui.frameSize_Width_LineEdit.setFocusPolicy(QtCore.Qt.NoFocus)
                self.ui.frameSize_Height_LineEdit.setText(value[1])
                self.ui.frameSize_Height_LineEdit.setFocusPolicy(QtCore.Qt.NoFocus)
                break

    def changeFrameSizeType(self, text):
        for key,value in self.frameSizeTypeDict.items():
            if key == text:
                self.ui.frameSize_Width_LineEdit.setText(value[0])
                self.ui.frameSize_Width_LineEdit.setFocusPolicy(QtCore.Qt.NoFocus)
                self.ui.frameSize_Height_LineEdit.setText(value[1])
                self.ui.frameSize_Height_LineEdit.setFocusPolicy(QtCore.Qt.NoFocus)
                break

    def configOther(self):
        self.ui.frameSizeType_ComboBox.setEnabled(False)
        self.ui.frameSize_Width_LineEdit.setText(self.YUVviewerConfigFile.config_dict['frameSize_Width'])
        self.ui.frameSize_Height_LineEdit.setText(self.YUVviewerConfigFile.config_dict['frameSize_Height'])
        self.ui.frameSize_Width_LineEdit.setFocusPolicy(QtCore.Qt.StrongFocus)
        self.ui.frameSize_Height_LineEdit.setFocusPolicy(QtCore.Qt.StrongFocus)

    def frameSizeHeightValidator(self,currentText):
        try:
            currentVale = int(currentText)
            if (currentVale % 2) == 0 and currentVale > 0:
                self.ui.frameSize_Height_LineEdit.setStyleSheet("QLineEdit{border:1px solid gray border-radius:1px}")
            else:
                QToolTip.showText(self.ui.frameSize_Height_LineEdit.mapToGlobal(QPoint(0, 10)), 'Height must be positive even')
                self.ui.frameSize_Height_LineEdit.setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}")
        except Exception as e:
            QToolTip.showText(self.ui.frameSize_Height_LineEdit.mapToGlobal(QPoint(0, 10)), 'Width must be num')
            self.ui.frameSize_Height_LineEdit.setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}")

    def frameSizeWidthValidator(self, currentText):
        try:
            currentVale = int(currentText)
            if (currentVale % 2) == 0 and currentVale > 0:
                self.ui.frameSize_Width_LineEdit.setStyleSheet("QLineEdit{border:1px solid gray border-radius:1px}")
            else:
                QToolTip.showText(self.ui.frameSize_Width_LineEdit.mapToGlobal(QPoint(0, 10)),'Width must be positive even')
                self.ui.frameSize_Width_LineEdit.setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}")
        except Exception as e:
            QToolTip.showText(self.ui.frameSize_Width_LineEdit.mapToGlobal(QPoint(0, 10)), 'Width must be num')
            self.ui.frameSize_Width_LineEdit.setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}")

    def startFrameValidator(self,currentText):
        try:
            currentVale = int(currentText)
        except Exception as e:
            QToolTip.showText(self.ui.startFrame_LineEdit.mapToGlobal(QPoint(0, 10)), 'startFrame must be num')
            self.ui.startFrame_LineEdit.setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}")
            return

        if currentVale >= 0:
            self.ui.startFrame_LineEdit.setStyleSheet("QLineEdit{border:1px solid gray border-radius:1px}")
            try:
                endFrameVale = int(self.ui.endFrame_LineEdit.text())
            except Exception as e:
                QToolTip.showText(self.ui.endFrame_LineEdit.mapToGlobal(QPoint(0, 10)), 'endFrame must be num')
                self.ui.endFrame_LineEdit.setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}")
                return
            if currentVale <= endFrameVale:
                self.ui.endFrame_LineEdit.setStyleSheet("QLineEdit{border:1px solid gray border-radius:1px}")
            else:
                QToolTip.showText(self.ui.endFrame_LineEdit.mapToGlobal(QPoint(0, 10)), 'endFrame must be greater than or equal to startFrame')
                self.ui.endFrame_LineEdit.setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}")
        else:
            QToolTip.showText(self.ui.startFrame_LineEdit.mapToGlobal(QPoint(0, 10)), 'startFrame must be greater than or equal to 0')
            self.ui.startFrame_LineEdit.setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}")

    def endFrameValidator(self,currentText):
        try:
            currentVale = int(currentText)
        except Exception as e:
            QToolTip.showText(self.ui.endFrame_LineEdit.mapToGlobal(QPoint(0, 10)), 'endFrame must be num')
            self.ui.endFrame_LineEdit.setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}")
            return

        self.ui.endFrame_LineEdit.setStyleSheet("QLineEdit{border:1px solid gray border-radius:1px}")

        try:
            startVale = int(self.ui.startFrame_LineEdit.text())
            if currentVale < startVale:
                QToolTip.showText(self.ui.endFrame_LineEdit.mapToGlobal(QPoint(0, 10)), 'endFrame must be greater than or equal to startFrame')
                self.ui.endFrame_LineEdit.setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}")
        except Exception as e:
            QToolTip.showText(self.ui.startFrame_LineEdit.mapToGlobal(QPoint(0, 10)), 'startFrame must be num')
            self.ui.startFrame_LineEdit.setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}")

    def exchaneSize(self):
        self.ui.frameSizeType_Other_RadioButton.setChecked(True)
        self.ui.frameSizeType_ComboBox.setEnabled(False)
        width = self.ui.frameSize_Width_LineEdit.text()
        self.ui.frameSize_Width_LineEdit.setText(self.ui.frameSize_Height_LineEdit.text())
        self.ui.frameSize_Height_LineEdit.setText(width)
        self.frameSizeWidthValidator(self.ui.frameSize_Width_LineEdit.text())
        self.frameSizeHeightValidator(self.ui.frameSize_Height_LineEdit.text())

    def _updateConfig(self):
        try:
            if int(self.ui.startFrame_LineEdit.text()) <= int(self.ui.endFrame_LineEdit.text()):
                temp_Width = int(self.ui.frameSize_Width_LineEdit.text())
                temp_Height = int(self.ui.frameSize_Height_LineEdit.text())
                if (temp_Width % 2) == 0 and (temp_Height % 2) == 0 and temp_Width > 0 and temp_Height > 0:
                    if self.ui.frameSizeType_Combo_RadioButton.isChecked():
                        self.YUVviewerConfigFile.config_dict['frameSizeType'] = self.ui.frameSizeType_ComboBox.currentText()
                    elif self.ui.frameSizeType_Other_RadioButton.isChecked():
                        self.YUVviewerConfigFile.config_dict['frameSizeType'] = self.ui.frameSizeType_Other_RadioButton.text()
                    self.YUVviewerConfigFile.config_dict['YUVFormat'] = self.ui.YUVFormat_ComboBox.currentText()
                    self.YUVviewerConfigFile.config_dict['frameSize_Width'] = self.ui.frameSize_Width_LineEdit.text()
                    self.YUVviewerConfigFile.config_dict['frameSize_Height'] = self.ui.frameSize_Height_LineEdit.text()
                    self.YUVviewerConfigFile.config_dict['frameRate'] = self.ui.frameRate_ComboBox.currentText()
                    self.YUVviewerConfigFile.config_dict['startFrame'] = self.ui.startFrame_LineEdit.text()
                    self.YUVviewerConfigFile.config_dict['endFrame'] = self.ui.endFrame_LineEdit.text()
                    ret = True
                else:
                    QMessageBox.critical(self, 'Error', 'frameSize invalid!!', QMessageBox.Ok)
                    ret = False
            else:
                QMessageBox.critical(self, 'Error', 'startFrame or endFrame invalid!!', QMessageBox.Ok)
                ret = False
        except Exception as e:
            QMessageBox.critical(self, 'Error', 'parameter invalid!!', QMessageBox.Ok)
            ret = False
        return ret

    def _imgView(self, openfile_list):
        if openfile_list:
            self.imgViewer = ImgViewer(self)
            frameSize_Width = int(self.YUVviewerConfigFile.config_dict['frameSize_Width'])
            frameSize_Height = int(self.YUVviewerConfigFile.config_dict['frameSize_Height'])
            #多线程的方法
            ret = self.imgViewer.setFileList_multithreading(openfile_list,
                        self.YUVviewerConfigFile.config_dict['YUVFormat'],
                        frameSize_Width,
                        frameSize_Height,
                        int(self.YUVviewerConfigFile.config_dict['startFrame']),
                        int(self.YUVviewerConfigFile.config_dict['endFrame']) - int(self.YUVviewerConfigFile.config_dict['startFrame']) + 1,
                        )
            if not ret:
                QMessageBox.critical(self, 'Error', 'unsupport YUVFormat!!', QMessageBox.Ok)
                self.show()
                return False
            #单线程的方法
            #try:
            #    ret = self.imgViewer.setFileList(openfile_list,
            #                    self.YUVviewerConfigFile.config_dict['YUVFormat'],
            #                    frameSize_Width,
            #                    frameSize_Height,
            #                    int(self.YUVviewerConfigFile.config_dict['startFrame']),
            #                    int(self.YUVviewerConfigFile.config_dict['endFrame']) - int(self.YUVviewerConfigFile.config_dict['startFrame']) + 1,
            #                    )
            #    if not ret:
            #        QMessageBox.critical(self, 'Error', 'unsupport YUVFormat!!', QMessageBox.Ok)
            #        self.show()
            #        return False
            #
            #except Exception as e:
            #    QMessageBox.critical(self, 'Error', 'unknow error!!', QMessageBox.Ok)
            #    self.show()
            #    return False
            if frameSize_Width > frameSize_Height:
                self.imgViewer.resize(800, frameSize_Height/frameSize_Width*800)
            else:
                self.imgViewer.resize(frameSize_Width/frameSize_Height*400, 400)
            screen = QGuiApplication.primaryScreen().geometry()
            size = self.imgViewer.geometry()
            self.imgViewer.move((screen.width() - size.width()) // 2, (screen.height() - size.height()) // 2)
            self.hide()
            self.imgViewer.show()
            return True

    def openFile(self):
        if self._updateConfig():
            openDir = ''
            if self.YUVviewerConfigFile.config_dict['lastPath']:
                if os.path.isdir(self.YUVviewerConfigFile.config_dict['lastPath']):
                    openDir = self.YUVviewerConfigFile.config_dict['lastPath']
            openfile_name = QFileDialog.getOpenFileNames(self, '选择文件', openDir, 'YUV files(*.yuv *.data *.raw)')
            openfile_list = openfile_name[0]
            if openfile_list:
                if os.path.exists(openfile_name[0][0]):
                    filepath, filename = os.path.split(openfile_name[0][0])
                    self.YUVviewerConfigFile.config_dict['lastPath'] = filepath
            self._imgView(openfile_list)

    def openFolder(self):
        if self._updateConfig():
            openDir = ''
            if self.YUVviewerConfigFile.config_dict['lastPath']:
                if os.path.isdir(self.YUVviewerConfigFile.config_dict['lastPath']):
                    openDir = self.YUVviewerConfigFile.config_dict['lastPath']
            openfolder_name = QFileDialog.getExistingDirectory(self, '选择文件夹', openDir)
            if openfolder_name:
                self.YUVviewerConfigFile.config_dict['lastPath'] = openfolder_name
                openfile_list = []
                for filename in os.listdir(openfolder_name):
                    filepath = os.path.join(openfolder_name, filename)
                    if os.path.isfile(filepath):
                        file_ext = filepath.rsplit('.', maxsplit=1)
                        if file_ext[1] == 'yuv':
                            openfile_list.append(filepath)
                        elif file_ext[1] == 'data':
                            openfile_list.append(filepath)
                        elif file_ext[1] == 'raw':
                            openfile_list.append(filepath)
                self._imgView(openfile_list)

    def about(self):
        QMessageBox.about(self, 'About', 'Version \n ' + VERSION + "\nCommit \n " + GIT_TAG + '\nAuthor\n qiaoqm@aliyun.com \n wenqing.li@aliyun.com \nWebsite\n https://github.com/QQxiaoming/YUVviewer')

    def help(self):
        QMessageBox.question(self, 'Help', '1.主界面选择数据参数。\n2.点击打开文件或文件夹将进行图像数据解析并显示图像。\n3.图像显示界面中使用滚轮放大缩小图像，使用左键可拖动图像，双击左键保存图像为png格式，单击右键复位图像大小和位置，双击右键交换图像R和B通道显示，单机中键显示图像原始大小。', QMessageBox.Ok)

    def closeEvent(self, event):
        del self.YUVviewerConfigFile
        event.accept()

    def exit(self):
        self.close()

if __name__ == '__main__':
    if len(sys.argv) == 2:
        if sys.argv[1] == '--version' or sys.argv[1] == '-v':
            print('YUVviewer ' + VERSION + "\n" + GIT_TAG.replace('"', '').replace('\n', ''))
            sys.exit(0)
    QtWidgets.QApplication.setAttribute(QtCore.Qt.AA_EnableHighDpiScaling)
    QtWidgets.QApplication.setAttribute(QtCore.Qt.AA_UseHighDpiPixmaps)
    QtWidgets.QApplication.setAttribute(QtCore.Qt.AA_DontUseNativeDialogs)
    QtWidgets.QApplication.setAttribute(QtCore.Qt.AA_DontUseNativeMenuBar)
    QtWidgets.QApplication.setAttribute(QtCore.Qt.AA_DontCreateNativeWidgetSiblings)
    application = QtWidgets.QApplication(sys.argv)
    font = application.font()
    font.setFamily(font.defaultFamily())
    font.setPixelSize(13)
    application.setFont(font)
    window = YUVviewer()
    window.show()
    sys.exit(application.exec_())
