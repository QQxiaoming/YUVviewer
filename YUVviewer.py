#!/usr/bin/env python
# -*- coding:utf-8 -*-

import sys
import os
from PyQt5 import QtCore, QtWidgets
from PyQt5.QtWidgets import QFileDialog, QMessageBox, QDesktopWidget
from UI_YUVviewer import Ui_MainWindow
from configFile import ConfigFile
from ImgViewer import ImgViewerWindow

VERSION = 'V0.2.2'

class MainWindow(QtWidgets.QMainWindow, Ui_MainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()
        self.ui = Ui_MainWindow()
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

        YUVFormat_list = ['YV12', 'YUY2', 'UYUV', '4:2:2', '4:4:4']
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

    def _updateConfig(self):
        try:
            if int(self.ui.startFrame_LineEdit.text()) <= int(self.ui.endFrame_LineEdit.text()):
                temp = int(self.ui.frameSize_Width_LineEdit.text())
                temp = int(self.ui.frameSize_Height_LineEdit.text())
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
                QMessageBox.critical(self, 'Error', 'startFrame or endFrame invalid!!', QMessageBox.Ok)
                return False
        except Exception as e:
            QMessageBox.critical(self, 'Error', 'parameter invalid!!', QMessageBox.Ok)

    def _imgView(self, openfile_list):
        if openfile_list:
            self.imgViewer = ImgViewerWindow(self)
            #多线程的方法
            ret = self.imgViewer.setFileList_multithreading(openfile_list,
                        self.YUVviewerConfigFile.config_dict['YUVFormat'],
                        int(self.YUVviewerConfigFile.config_dict['frameSize_Width']),
                        int(self.YUVviewerConfigFile.config_dict['frameSize_Height']),
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
                                int(self.YUVviewerConfigFile.config_dict['frameSize_Width']),
                                int(self.YUVviewerConfigFile.config_dict['frameSize_Height']),
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
            self.imgViewer.resize(800, 400)
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
    show = MainWindow()
    show.show()
    sys.exit(app.exec_())
