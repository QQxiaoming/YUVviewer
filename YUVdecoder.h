#ifndef YUVDECODER_H
#define YUVDECODER_H

#include <QString>
#include <QList>
#include <QMap>
#include <opencv2/opencv.hpp>

typedef QList<cv::Mat*> (* yuvdecoder_t)(QString yuvfilename,int W, int H, int startframe, int totalframe);

class YUV2RGB
{
public:
    YUV2RGB();
    static QList<cv::Mat*> yv12(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> i420(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> nv21(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> nv12(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> yuy2(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> yvyu(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> uyvy(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> yuv444(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QMap<QString, yuvdecoder_t> yuvdecoder_map;
};


#endif // YUVDECODER_H
