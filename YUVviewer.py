#!/usr/bin/env python
# -*- coding:utf-8 -*-

import sys
import os
from PyQt5 import QtCore, QtWidgets
from PyQt5.QtWidgets import QFileDialog, QMessageBox, QDesktopWidget, QToolTip
from PyQt5.QtCore import QPoint
from UI_YUVviewer import Ui_YUVviewer
from configFile import ConfigFile
from ImgViewer import ImgViewer

VERSION = 'V0.3.3'

class YUVviewer(QtWidgets.QMainWindow, Ui_YUVviewer):
    def __init__(self):
        super(YUVviewer, self).__init__()
        self.ui = Ui_YUVviewer()
        self.ui.setupUi(self)
        self.setWindowTitle('YUVviewer ' + VERSION + ' by liwq')
        screen = QDesktopWidget().screenGeometry()
        size = self.geometry()
        self.move((screen.width() - size.width()) / 2, (screen.height() - size.height()) / 2)

        self.YUVviewerConfigFile = ConfigFile(os.path.join(os.getcwd(),'YUVViewer.ini'));

        if self.YUVviewerConfigFile.config_dict['frameSizeType'] == 'Other':
            self.ui.frameSizeType_Other_RadioButton.setChecked(True)
            self.ui.frameSize_Width_LineEdit.setText(self.YUVviewerConfigFile.config_dict['frameSize_Width'])
            self.ui.frameSize_Height_LineEdit.setText(self.YUVviewerConfigFile.config_dict['frameSize_Height'])
        elif self.YUVviewerConfigFile.config_dict['frameSizeType'] == 'CIF':
            self.ui.frameSizeType_CIF_RadioButton.setChecked(True)
            self.ui.frameSize_Width_LineEdit.setText('352')
            self.ui.frameSize_Width_LineEdit.setFocusPolicy(QtCore.Qt.NoFocus)
            self.YUVviewerConfigFile.config_dict['frameSize_Width'] = '352'
            self.ui.frameSize_Height_LineEdit.setText('288')
            self.ui.frameSize_Height_LineEdit.setFocusPolicy(QtCore.Qt.NoFocus)
            self.YUVviewerConfigFile.config_dict['frameSize_Height'] = '288'
        elif self.YUVviewerConfigFile.config_dict['frameSizeType'] == 'QCIF':
            self.ui.frameSizeType_QCIF_RadioButton.setChecked(True)
            self.ui.frameSize_Width_LineEdit.setText('176')
            self.ui.frameSize_Width_LineEdit.setFocusPolicy(QtCore.Qt.NoFocus)
            self.YUVviewerConfigFile.config_dict['frameSize_Width'] = '176'
            self.ui.frameSize_Height_LineEdit.setText('144')
            self.ui.frameSize_Height_LineEdit.setFocusPolicy(QtCore.Qt.NoFocus)
            self.YUVviewerConfigFile.config_dict['frameSize_Height'] = '144'

        YUVFormat_list = ['YV12', 'I420', 'YUY2', 'UYUV', '4:2:2', '4:4:4']
        currentIndex = YUVFormat_list.index(self.YUVviewerConfigFile.config_dict['YUVFormat'] )
        self.ui.YUVFormat_ComboBox.setCurrentIndex(currentIndex)

        frameRate_list = ['30', '60', '120']
        currentIndex = frameRate_list.index(self.YUVviewerConfigFile.config_dict['frameRate'])
        self.ui.frameRate_ComboBox.setCurrentIndex(currentIndex)

        self.ui.startFrame_LineEdit.setText(self.YUVviewerConfigFile.config_dict['startFrame'])
        self.ui.endFrame_LineEdit.setText(self.YUVviewerConfigFile.config_dict['endFrame'])

        self.ui.frameSizeType_CIF_RadioButton.clicked.connect(self.configCIF)
        self.ui.frameSizeType_QCIF_RadioButton.clicked.connect(self.configQCIF)
        self.ui.frameSizeType_Other_RadioButton.clicked.connect(self.configOther)

        self.ui.frameSize_Height_LineEdit.textEdited.connect(self.frameSizeHeightValidator)
        self.ui.frameSize_Width_LineEdit.textEdited.connect(self.frameSizeWidthValidator)
        self.ui.startFrame_LineEdit.textEdited.connect(self.startFrameValidator)
        self.ui.endFrame_LineEdit.textEdited.connect(self.endFrameValidator)

        self.ui.openFile_PushButton.clicked.connect(self.openFile)
        self.ui.openFolder_PushButton.clicked.connect(self.openFolder)
        self.ui.about_PushButton.clicked.connect(self.about)
        self.ui.help_PushButton.clicked.connect(self.help)

    def configCIF(self):
        self.ui.frameSize_Width_LineEdit.setText('352')
        self.ui.frameSize_Width_LineEdit.setFocusPolicy(QtCore.Qt.NoFocus)
        self.ui.frameSize_Height_LineEdit.setText('288')
        self.ui.frameSize_Height_LineEdit.setFocusPolicy(QtCore.Qt.NoFocus)

    def configQCIF(self):
        self.ui.frameSize_Width_LineEdit.setText('176')
        self.ui.frameSize_Width_LineEdit.setFocusPolicy(QtCore.Qt.NoFocus)
        self.ui.frameSize_Height_LineEdit.setText('144')
        self.ui.frameSize_Height_LineEdit.setFocusPolicy(QtCore.Qt.NoFocus)

    def configOther(self):
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


    def _updateConfig(self):
        try:
            if int(self.ui.startFrame_LineEdit.text()) <= int(self.ui.endFrame_LineEdit.text()):
                temp_Width = int(self.ui.frameSize_Width_LineEdit.text())
                temp_Height = int(self.ui.frameSize_Height_LineEdit.text())
                if (temp_Width % 2) == 0 and (temp_Height % 2) == 0 and temp_Width > 0 and temp_Height > 0:
                    if self.ui.frameSizeType_CIF_RadioButton.isChecked():
                        self.YUVviewerConfigFile.config_dict['frameSizeType'] = self.ui.frameSizeType_CIF_RadioButton.text()
                    elif self.ui.frameSizeType_QCIF_RadioButton.isChecked():
                        self.YUVviewerConfigFile.config_dict['frameSizeType'] = self.ui.frameSizeType_QCIF_RadioButton.text()
                    elif self.ui.frameSizeType_Other_RadioButton.isChecked():
                        self.YUVviewerConfigFile.config_dict['frameSizeType'] = self.ui.frameSizeType_Other_RadioButton.text()
                    self.YUVviewerConfigFile.config_dict['YUVFormat'] = self.ui.YUVFormat_ComboBox.currentText()
                    self.YUVviewerConfigFile.config_dict['frameSize_Width'] = self.ui.frameSize_Width_LineEdit.text()
                    self.YUVviewerConfigFile.config_dict['frameSize_Height'] = self.ui.frameSize_Height_LineEdit.text()
                    self.YUVviewerConfigFile.config_dict['frameRate'] = self.ui.frameRate_ComboBox.currentText()
                    self.YUVviewerConfigFile.config_dict['startFrame'] = self.ui.startFrame_LineEdit.text()
                    self.YUVviewerConfigFile.config_dict['endFrame'] = self.ui.endFrame_LineEdit.text()
                    return True
                else:
                    QMessageBox.critical(self, 'Error', 'frameSize invalid!!', QMessageBox.Ok)
                    return False
            else:
                QMessageBox.critical(self, 'Error', 'startFrame or endFrame invalid!!', QMessageBox.Ok)
                return False
        except Exception as e:
            QMessageBox.critical(self, 'Error', 'parameter invalid!!', QMessageBox.Ok)

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
            '''
            try:
                ret = self.imgViewer.setFileList(openfile_list,
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
            
            except Exception as e:
                QMessageBox.critical(self, 'Error', 'unknow error!!', QMessageBox.Ok)
                self.show()
                return False
            '''
            if frameSize_Width > frameSize_Height:
                self.imgViewer.resize(800, frameSize_Height/frameSize_Width*800)
            else:
                self.imgViewer.resize(frameSize_Width/frameSize_Height*400, 400)
            screen = QDesktopWidget().screenGeometry()
            size = self.imgViewer.geometry()
            self.imgViewer.move((screen.width() - size.width()) / 2, (screen.height() - size.height()) / 2)
            self.hide()
            self.imgViewer.show()
            return True

    def openFile(self):
        if self._updateConfig():
            openfile_name = QFileDialog.getOpenFileNames(self, '选择文件', '', 'YUV files(*.yuv)')
            openfile_list = openfile_name[0]
            self._imgView(openfile_list)

    def openFolder(self):
        if self._updateConfig():
            openfolder_name = QFileDialog.getExistingDirectory(self, '选择文件夹')
            if openfolder_name:
                openfile_list = []
                for filename in os.listdir(openfolder_name):
                    filepath = os.path.join(openfolder_name, filename)
                    if os.path.isfile(filepath):
                        file_ext = filepath.rsplit('.', maxsplit=1)
                        if file_ext[1] == 'yuv':
                            openfile_list.append(filepath)
                self._imgView(openfile_list)

    def about(self):
        QMessageBox.about(self, 'About', '版本 ' + VERSION + ' 作者:liwq')

    def help(self):
        QMessageBox.question(self, 'Help', '目前暂无帮助说明', QMessageBox.Ok)

    def closeEvent(self, event):
        del self.YUVviewerConfigFile
        event.accept()

    def exit(self):
        self.close()

if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    show = YUVviewer()
    show.show()
    sys.exit(app.exec_())
