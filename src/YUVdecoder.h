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

class ImageDecoder {
public:
    typedef QList<cv::Mat*> (* yuvdecoder_t)(const QString &yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> yv12(const QString &yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> i420(const QString &yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> nv21(const QString &yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> nv12(const QString &yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> yuy2(const QString &yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> yvyu(const QString &yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> uyvy(const QString &yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> yuv444(const QString &yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> rgb565_little_endian(const QString &yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> rgb565_big_endian(const QString &yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> bgr565_little_endian(const QString &yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> bgr565_big_endian(const QString &yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> rgb888(const QString &yuvfilename,int W, int H, int startframe, int totalframe);
    static QList<cv::Mat*> bayer(const QString &yuvfilename,int W, int H, int startframe, int totalframe,int code,int bit);
    static QList<cv::Mat*> png(const QString &yuvfilename,int W, int H, int startframe, int totalframe);
    static QMap<QString, yuvdecoder_t> yuvdecoder_map;
};


#endif // YUVDECODER_H
