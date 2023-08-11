/*
 * This file is part of the https://github.com/QQxiaoming/YUVviewer.git
 * project.
 *
 * Copyright (C) 2020 Quard <2014500726@smail.xtu.edu.cn>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */
#ifndef IMGVIEWER_H
#define IMGVIEWER_H

#include <QWidget>
#include <QThread>
#include <QString>
#include <QImage>
#include <QPainter>
#include <QCloseEvent> 
#include <QPaintEvent> 
#include <QMouseEvent> 
#include <QWheelEvent> 
#include <QResizeEvent>
#include "ImgExport.h"
#include "YUVdecoder.h"

namespace Ui {
class ImgViewerWindow;
}

class YUVDecodeThread : public QThread {
    Q_OBJECT

public:
    explicit YUVDecodeThread(QWidget *parent = nullptr,
                             const QString &yuvfilename = QString(),
                             QString YUVFormat = nullptr,
                             int W = 0, int H = 0,
                             int startframe = 0,
                             int totalframe = 0);

protected:
    void run();

signals:
    void finsh_signal(QList<QImage*> frame_RGB_list,QString str);

private:
    QWidget *window;
    QString yuvfilename;
    int W;
    int H;
    int startframe;
    int totalframe;
    ImageDecoder::yuvdecoder_t decoder;
    static void image_cleanup(cv::Mat* ptr);
};

class ImgViewer : public QWidget {
    Q_OBJECT

public:
    explicit ImgViewer(const QString &folderpath,QWidget *parent = nullptr,QWidget *parentWindow = nullptr);
    ~ImgViewer();
    bool setFileList(QStringList filelist,QString YUVFormat, int W, int H, int startframe, int totalframe);
    bool setFileList_multithreading(QStringList filenamelist, QString YUVFormat, int W, int H, int startframe, int totalframe);

private slots:
    void reciveimgdata(QList<QImage *> img_RGB_list, QString filename);
    void previousImg();
    void nextImg();

protected:
    void closeEvent(QCloseEvent *event);
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    void draw_img(QPainter *painter);
    Ui::ImgViewerWindow *ui;
    ImgExport *imgExportWindow;
    QWidget *parentWindow;
    bool left_click;
    QList<YUVDecodeThread*> decode_thread;
    QList<YUVDecodeThread*> decode_thread_finsh;
    QString folderpath;
    QList<QList<QImage*>> img_list;
    QStringList filelist;
    QList<QImage*> currentImg_RGB_list;
    QImage *currentImg;
    QImage scaled_img;
    QPoint point;
    QPoint startPos;
    QPoint endPos;
    bool flipRGB = false;
    static void image_cleanup(cv::Mat* ptr);
};

#endif // IMGVIEWER_H
