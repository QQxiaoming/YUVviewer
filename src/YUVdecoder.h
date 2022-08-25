/**
 * @file YUVdecoder.h
 * @brief 解码YUV数据
 * @version 1.0
 * @date 2020-04-14
 */
#ifndef YUVDECODER_H
#define YUVDECODER_H

#include <QString>
#include <QList>
#include <QMap>
#include <opencv2/opencv.hpp>

typedef QList<cv::Mat*> (* yuvdecoder_t)(QString yuvfilename,int W, int H, int startframe, int totalframe);

class YUV2RGB {
public:
    static QList<cv::Mat*> yv12(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> i420(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> nv21(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> nv12(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> yuy2(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> yvyu(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> uyvy(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> yuv444(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> rgb565_little_endian(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> rgb565_big_endian(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> bgr565_little_endian(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> bgr565_big_endian(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> rgb888(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> bayer(QString yuvfilename,int W, int H, int startframe, int totalframe,int code,int bit);
    static QList<cv::Mat*> bayerBG(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> bayerGB(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> bayerRG(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> bayerGR(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> bayerBG_RAW10(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> bayerGB_RAW10(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> bayerRG_RAW10(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> bayerGR_RAW10(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> bayerBG_RAW12(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> bayerGB_RAW12(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> bayerRG_RAW12(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> bayerGR_RAW12(QString yuvfilename,int W, int H, int startframe, int totalframe);
    static QMap<QString, yuvdecoder_t> yuvdecoder_map;
};


#endif // YUVDECODER_H
