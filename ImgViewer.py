#!/usr/bin/env python
# -*- coding:utf-8 -*-

import sys
import os
from PyQt5 import QtCore
from PyQt5.QtWidgets import QWidget, QFileDialog, QMessageBox, QDesktopWidget
from PyQt5.QtCore import QPoint, Qt, QThread, pyqtSignal
from PyQt5.QtGui import QPixmap, QImage, QPainter
from UI_ImgViewer import Ui_ImgViewerWindow
from YUVdecoder import YUV2RGB

class YUVDecodeThread(QThread):
    finsh_signal = pyqtSignal(list, str)

    def __init__(self, Window, yuvfilename, YUVFormat, W, H, startframe, totalframe):
        super(YUVDecodeThread, self).__init__()
        self.window = Window
        self.filename = yuvfilename
        self.W = W
        self.H = H
        self.startframe = startframe
        self.totalframe = totalframe
        # 获取该格式的解码函数
        self.decoder = YUV2RGB.YUVdecoder_dict.get(YUVFormat)

    def run(self):
        if self.decoder is None:
            # 未能成功获取则返回无法解码
            self.finsh_signal.emit([],'')
        else:
            try:
                # 成功获取则返回计算结果
                frame_RGB_list = self.decoder(self.filename, self.W, self.H, self.startframe, self.totalframe)
            except Exception as e:
                self.finsh_signal.emit([], '')
                return
            # 定义img列表用了保存每一帧的QPixmap
            img_RGB_list = []
            # 将原始帧转换到QPixmap并保存到img列表
            for img in frame_RGB_list:
                # 提取图像的通道和尺寸，用于将OpenCV下的image转换成Qimage
                height, width, channel = img.shape
                bytesPerline = 3 * width
                qImg = QImage(img.data, width, height, bytesPerline, QImage.Format_RGB888).rgbSwapped()
                img_RGB_list.append(QPixmap.fromImage(qImg))
            self.finsh_signal.emit(img_RGB_list,self.filename)

class ImgViewer(QWidget, Ui_ImgViewerWindow):
    def __init__(self,parentWindow):
        super(ImgViewer, self).__init__()
        self.ui = Ui_ImgViewerWindow()
        self.ui.setupUi(self)
		
        self.parentWindow = parentWindow
        self.setWindowTitle('loading file, please wait ....')
        self.ui.left_PushButton.setFlat(True)
        self.ui.right_PushButton.setFlat(True)
        self.ui.left_PushButton.clicked.connect(self.previousImg)
        self.ui.right_PushButton.clicked.connect(self.nextImg)
        self.left_click = False
        
    def setFileList(self,filelist,YUVFormat, W, H, startframe, totalframe):
        # 获取该格式的解码函数
        decoder = YUV2RGB.YUVdecoder_dict.get(YUVFormat)
        if decoder is None:
            # 未能成功获取则返回无法解码
            return False
        else:
            # 成功获取解码器则准备解码
            # 定义空列表
            self.img_list = []
            self.ui.imgViewer.setText('')
            self.filelist = []
            # 遍历文件列表
            for filename in filelist:
                try:
                    # 使用获取的解码函数进行解码得到RGB的原始帧列表
                    frame_RGB_list = decoder(filename, W, H, startframe, totalframe)
                except Exception as e:
                    continue
                # 定义img列表用来保存每一帧的QPixmap
                img_RGB_list = []
                # 将原始帧转换到QPixmap并保存到img列表
                for img in frame_RGB_list:
                    # 提取图像的通道和尺寸，用于将OpenCV下的image转换成Qimage
                    height, width, channel = img.shape
                    bytesPerline = 3 * width
                    qImg = QImage(img.data, width, height, bytesPerline, QImage.Format_RGB888).rgbSwapped()
                    img_RGB_list.append(QPixmap.fromImage(qImg))
                # img_RGB_list以及文件名存入列表
                self.img_list.append(img_RGB_list)
                self.filelist.append(os.path.split(filename)[1])
            #设置显示第一个YUV文件的第一帧图像
            self.currentImg_RGB_list = self.img_list[0]
            self.currentImg = self.currentImg_RGB_list[0]
            self.setWindowTitle(self.filelist[0]+'-'+str(0))
            self.scaled_img = self.currentImg.scaled(self.size())
            self.point = QPoint(0, 0)
            return True

    def reciveimgdata(self,img_RGB_list,filename):
        if not img_RGB_list == []:
            # img_RGB_list以及文件名存入列表
            self.img_list.append(img_RGB_list)
            self.filelist.append(os.path.split(filename)[1])
            if len(self.img_list) == 1:
                # 设置显示第一个YUV文件的第一帧图像
                self.ui.imgViewer.setText('')
                self.currentImg_RGB_list = self.img_list[0]
                self.currentImg = self.currentImg_RGB_list[0]
                self.setWindowTitle(self.filelist[0] + '-' + str(0))
                self.scaled_img = self.currentImg.scaled(self.size())
                self.point = QPoint(0, 0)
                self.repaint()

        self.decode_thread_finsh.append(self.decode_thread.pop(0))
        if len(self.decode_thread) > 0:
            self.decode_thread[0].start()
        else:
            if len(self.img_list) == 0:
                QMessageBox.critical(self.parentWindow, 'Error', 'unknow error!!', QMessageBox.Ok)
                self.close()

    def setFileList_multithreading(self,filelist,YUVFormat, W, H, startframe, totalframe):
        # 获取该格式的解码函数
        decoder = YUV2RGB.YUVdecoder_dict.get(YUVFormat)
        if decoder is None:
            # 未能成功获取则返回无法解码
            return False
        # 定义空列表
        self.img_list = []
        self.filelist = []
        self.decode_thread = []
        self.decode_thread_finsh = []
        # 遍历文件列表
        for filename in filelist:
            decodeThread = YUVDecodeThread(self,filename,YUVFormat, W, H, startframe, totalframe)
            decodeThread.finsh_signal.connect(self.reciveimgdata)
            self.decode_thread.append(decodeThread)
        self.decode_thread[0].start()
        return True

    def closeEvent(self, event):
        self.parentWindow.show()
        event.accept()

    def draw_img(self, painter):
        painter.drawPixmap(self.point, self.scaled_img)

    def paintEvent(self, e):
        if not len(self.img_list) == 0:
            painter = QPainter()
            painter.begin(self)
            self.draw_img(painter)
            painter.end()

    def mouseMoveEvent(self, e):
        if not len(self.img_list) == 0:
            if self.left_click:
                self._endPos = e.pos() - self._startPos
                self.point = self.point + self._endPos
                self._startPos = e.pos()
                self.repaint()

    def mousePressEvent(self, e):
        if not len(self.img_list) == 0:
            if e.button() == Qt.LeftButton:
                self.left_click = True
                self._startPos = e.pos()

    def mouseReleaseEvent(self, e):
        if not len(self.img_list) == 0:
            if e.button() == Qt.LeftButton:
                self.left_click = False
            elif e.button() == Qt.RightButton:
                self.point = QPoint(0, 0)
                self.scaled_img = self.currentImg.scaled(self.size())
                self.repaint()

    def mouseDoubleClickEvent(self, event):
        if not len(self.img_list) == 0:
            if event.buttons() == Qt.LeftButton:
                list_index = self.img_list.index(self.currentImg_RGB_list)
                img_RGB_list = self.img_list[list_index]
                img_index = img_RGB_list.index(self.currentImg)
                savefile_name = QFileDialog.getSaveFileName(self, '保存文件', self.filelist[list_index].replace('.yuv','-') + str(img_index) + '.png', 'Image files(*.png)')
                if savefile_name[0]:
                    self.currentImg.save(savefile_name[0])

    def wheelEvent(self, e):
        if not len(self.img_list) == 0:
            if e.angleDelta().y() > 0:
                # 放大图片
                if not self.scaled_img.width() == 0:
                    setpsize_x = 25
                    setpsize_y = setpsize_x * self.scaled_img.height() / self.scaled_img.width() #缩放可能导致比例不精确

                    self.scaled_img = self.currentImg.scaled(self.scaled_img.width() + setpsize_x,self.scaled_img.height() + setpsize_y)
                    new_w = e.x() - (self.scaled_img.width() * (e.x() - self.point.x())) / (self.scaled_img.width() - setpsize_x)
                    new_h = e.y() - (self.scaled_img.height() * (e.y() - self.point.y())) / (self.scaled_img.height() - setpsize_y)
                    self.point = QPoint(new_w, new_h)
                    self.repaint()

            elif e.angleDelta().y() < 0:
                # 缩小图片
                if self.scaled_img.width() > 25:
                    setpsize_x = 25
                    setpsize_y = setpsize_x * self.scaled_img.height() / self.scaled_img.width() #缩放可能导致比例不精确

                    self.scaled_img = self.currentImg.scaled(self.scaled_img.width() - setpsize_x,self.scaled_img.height() - setpsize_y)
                    new_w = e.x() - (self.scaled_img.width() * (e.x() - self.point.x())) / (self.scaled_img.width() + setpsize_x)
                    new_h = e.y() - (self.scaled_img.height() * (e.y() - self.point.y())) / (self.scaled_img.height() + setpsize_y)
                    self.point = QPoint(new_w, new_h)
                    self.repaint()

    def resizeEvent(self, e):
        if not len(self.img_list) == 0:
            if self.parent is not None:
                self.scaled_img = self.currentImg.scaled(self.size())
                self.point = QPoint(0, 0)
                self.update()

    def previousImg(self):
        if not len(self.img_list) == 0:
            #得到当前显示的文件序号
            list_index = self.img_list.index(self.currentImg_RGB_list)
            img_RGB_list = self.img_list[list_index]
            #得到当前显示的图像是文件的帧序号
            img_index = img_RGB_list.index(self.currentImg)

            #判断当前是否是第一帧
            if img_index == 0:
                #如果当前是第一帧,则判断当前是否是第一个文件
                if list_index == 0:
                    #如果是第一个文件则文件序号更新代到最后一个序号
                    list_index = len(self.img_list) - 1
                else:
                    #否则文件序号更新到前一个文件序号
                    list_index -= 1
                #更新帧序号为文件的最后一帧序号
                img_index = len(self.img_list[list_index]) - 1
            else:
                #否则更新帧序号为前一帧序号,此时文件序号不用更新
                img_index -= 1

            #序号更新完成,代入序号配置当前显示的页面
            self.setWindowTitle(self.filelist[list_index] + '-' + str(img_index))
            self.currentImg_RGB_list = self.img_list[list_index]
            self.currentImg = self.currentImg_RGB_list[img_index]
            self.point = QPoint(0, 0)
            self.scaled_img = self.currentImg.scaled(self.size())
            self.repaint()

    def nextImg(self):
        if not len(self.img_list) == 0:
            # 得到当前显示的文件序号
            list_index = self.img_list.index(self.currentImg_RGB_list)
            img_RGB_list = self.img_list[list_index]
            # 得到当前显示的图像是文件的帧序号
            img_index = img_RGB_list.index(self.currentImg)

            # 判断当前是否是最后一帧
            if img_index == len(img_RGB_list) - 1:
                # 如果当前是最后一帧,则判断当前是否是最后一个文件
                if list_index == len(self.img_list) - 1:
                    # 如果是最后一个文件则文件序号更新代到第一个序号
                    list_index = 0
                else:
                    # 否则文件序号更新到后一个文件序号
                    list_index += 1
                # 更新帧序号为文件的第一帧序号
                img_index = 0
            else:
                # 否则更新帧序号为后一帧序号,此时文件序号不用更新
                img_index += 1

            # 序号更新完成,代入序号配置当前显示的页面
            self.setWindowTitle(self.filelist[list_index] + '-' + str(img_index))
            self.currentImg_RGB_list = self.img_list[list_index]
            self.currentImg = self.currentImg_RGB_list[img_index]
            self.point = QPoint(0, 0)
            self.scaled_img = self.currentImg.scaled(self.size())
            self.repaint()
