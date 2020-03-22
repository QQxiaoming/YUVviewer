#ifndef IMGVIEWER_H
#define IMGVIEWER_H

#include <QWidget>
#include <QThread>
#include <QString>
#include <QImage>
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

protected:
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    Ui::ImgViewerWindow *ui;
    QWidget *parentWindow;
    bool left_click;

    QList<QList<QImage*>> img_list;
    QStringList filelist;

    QList<QImage*> currentImg_RGB_list;
    QImage *currentImg;
    QImage scaled_img;
    QPoint point;
};

#endif // IMGVIEWER_H
