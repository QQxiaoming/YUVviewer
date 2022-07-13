# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'UI_ImgViewer.ui'
#
# Created by: PyQt5 UI code generator 5.12.3
#
# WARNING! All changes made in this file will be lost!


from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_ImgViewerWindow(object):
    def setupUi(self, ImgViewerWindow):
        ImgViewerWindow.setObjectName("ImgViewerWindow")
        ImgViewerWindow.resize(612, 300)
        self.gridLayout = QtWidgets.QGridLayout(ImgViewerWindow)
        self.gridLayout.setObjectName("gridLayout")
        self.imgViewer = QtWidgets.QLabel(ImgViewerWindow)
        self.imgViewer.setAlignment(QtCore.Qt.AlignCenter)
        self.imgViewer.setObjectName("imgViewer")
        self.gridLayout.addWidget(self.imgViewer, 0, 1, 1, 1)
        self.left_PushButton = QtWidgets.QPushButton(ImgViewerWindow)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.left_PushButton.sizePolicy().hasHeightForWidth())
        self.left_PushButton.setSizePolicy(sizePolicy)
        self.left_PushButton.setText("")
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/img/img/left.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.left_PushButton.setIcon(icon)
        self.left_PushButton.setObjectName("left_PushButton")
        self.gridLayout.addWidget(self.left_PushButton, 0, 0, 1, 1)
        self.right_PushButton = QtWidgets.QPushButton(ImgViewerWindow)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.right_PushButton.sizePolicy().hasHeightForWidth())
        self.right_PushButton.setSizePolicy(sizePolicy)
        self.right_PushButton.setText("")
        icon1 = QtGui.QIcon()
        icon1.addPixmap(QtGui.QPixmap(":/img/img/right.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.right_PushButton.setIcon(icon1)
        self.right_PushButton.setObjectName("right_PushButton")
        self.gridLayout.addWidget(self.right_PushButton, 0, 2, 1, 1)

        self.retranslateUi(ImgViewerWindow)
        QtCore.QMetaObject.connectSlotsByName(ImgViewerWindow)

    def retranslateUi(self, ImgViewerWindow):
        _translate = QtCore.QCoreApplication.translate
        ImgViewerWindow.setWindowTitle(_translate("ImgViewerWindow", "Form"))
        self.imgViewer.setText(_translate("ImgViewerWindow", "<html><head/><body><p><span style=\" font-size:24pt; font-weight:600;\">loading file, please wait ....</span></p></body></html>"))
import img_rc
