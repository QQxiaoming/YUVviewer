/**
 * @file ImgViewer.h
 * @brief 图像显示界面
 * @version 1.0
 * @date 2020-04-14
 */
#ifndef IMGVIEWER_H
#define IMGVIEWER_H

#include <QWidget>
#include <QThread>
#include <QString>
#include <QImage>
#include <QPainter>
#include "YUVdecoder.h"

namespace Ui {
class ImgViewerWindow;
}

class YUVDecodeThread : public QThread
{
    Q_OBJECT

public:
    explicit YUVDecodeThread(QWidget *parent = nullptr,
                             QString yuvfilename = nullptr,
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
    yuvdecoder_t decoder;
};

class ImgViewer : public QWidget
{
    Q_OBJECT

public:
    explicit ImgViewer(QWidget *parent = nullptr,QWidget *parentWindow = nullptr);
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
    QWidget *parentWindow;
    bool left_click;

    QList<QList<QImage*>> img_list;
    QStringList filelist;

    QList<YUVDecodeThread*> decode_thread;
    QList<YUVDecodeThread*> decode_thread_finsh;

    QList<QImage*> currentImg_RGB_list;
    QImage *currentImg;
    QImage scaled_img;
    QPoint point;
    QPoint startPos;
    QPoint endPos;
    bool flipRGB = false;
};

#endif // IMGVIEWER_H
