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
#ifndef YUVDECODER_H
#define YUVDECODER_H

#include <QString>
#include <QList>
#include <QMap>
#include <opencv2/opencv.hpp>

class ImageDecoder {
public:
    enum BayerFormat {
        NONE,
        CSI,
        compact,
        align16
    };
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
    static QList<cv::Mat*> bayer(const QString &yuvfilename,int W, int H, int startframe, int totalframe,int code,int bit,BayerFormat type);
    static QList<cv::Mat*> png(const QString &yuvfilename,int W, int H, int startframe, int totalframe);
    static QMap<QString, yuvdecoder_t> yuvdecoder_map;
};


#endif // YUVDECODER_H
