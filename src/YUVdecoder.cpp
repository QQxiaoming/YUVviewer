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
#include <QList>
#include <QFile>
#include <QFileInfo>
#include <QDataStream>
#include <QtEndian>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include "YUVdecoder.h"


QMap<QString, ImageDecoder::yuvdecoder_t> ImageDecoder::yuvdecoder_map = {
    {"YV12",            ImageDecoder::yv12},
    {"YU12/I420",       ImageDecoder::i420},
    {"NV21",            ImageDecoder::nv21},
    {"NV12",            ImageDecoder::nv12},
    {"YUY2/YUYV",       ImageDecoder::yuy2},
    {"YVYU",            ImageDecoder::yvyu},
    {"UYVY",            ImageDecoder::uyvy},
    {"4:4:4",           ImageDecoder::yuv444},
    {"RGB565_L",        ImageDecoder::rgb565_little_endian},
    {"RGB565_B",        ImageDecoder::rgb565_big_endian},
    {"BGR565_L",        ImageDecoder::bgr565_little_endian},
    {"BGR565_B",        ImageDecoder::bgr565_big_endian},
    {"RGB888",          ImageDecoder::rgb888},
    #define BAYER_FUNC(code,bit,type) [](const QString &yuvfilename,int W, int H, int startframe, int totalframe) -> QList<cv::Mat*> { \
                                    return ImageDecoder::bayer(yuvfilename,W,H,startframe,totalframe,code,bit,type);}
    {"BayerBG",                 BAYER_FUNC(cv::COLOR_BayerBG2RGB,8,ImageDecoder::CSI)},
    {"BayerGB",                 BAYER_FUNC(cv::COLOR_BayerGB2RGB,8,ImageDecoder::CSI)},
    {"BayerRG",                 BAYER_FUNC(cv::COLOR_BayerRG2RGB,8,ImageDecoder::CSI)},
    {"BayerGR",                 BAYER_FUNC(cv::COLOR_BayerGR2RGB,8,ImageDecoder::CSI)},
    {"BayerBG_RAW10_CSI",       BAYER_FUNC(cv::COLOR_BayerBG2RGB,10,ImageDecoder::CSI)},
    {"BayerGB_RAW10_CSI",       BAYER_FUNC(cv::COLOR_BayerGB2RGB,10,ImageDecoder::CSI)},
    {"BayerRG_RAW10_CSI",       BAYER_FUNC(cv::COLOR_BayerRG2RGB,10,ImageDecoder::CSI)},
    {"BayerGR_RAW10_CSI",       BAYER_FUNC(cv::COLOR_BayerGR2RGB,10,ImageDecoder::CSI)},
    {"BayerBG_RAW10_COMPACT",   BAYER_FUNC(cv::COLOR_BayerBG2RGB,10,ImageDecoder::compact)},
    {"BayerGB_RAW10_COMPACT",   BAYER_FUNC(cv::COLOR_BayerGB2RGB,10,ImageDecoder::compact)},
    {"BayerRG_RAW10_COMPACT",   BAYER_FUNC(cv::COLOR_BayerRG2RGB,10,ImageDecoder::compact)},
    {"BayerGR_RAW10_COMPACT",   BAYER_FUNC(cv::COLOR_BayerGR2RGB,10,ImageDecoder::compact)},
    {"BayerBG_RAW10_ALIGN16",   BAYER_FUNC(cv::COLOR_BayerBG2RGB,10,ImageDecoder::align16)},
    {"BayerGB_RAW10_ALIGN16",   BAYER_FUNC(cv::COLOR_BayerGB2RGB,10,ImageDecoder::align16)},
    {"BayerRG_RAW10_ALIGN16",   BAYER_FUNC(cv::COLOR_BayerRG2RGB,10,ImageDecoder::align16)},
    {"BayerGR_RAW10_ALIGN16",   BAYER_FUNC(cv::COLOR_BayerGR2RGB,10,ImageDecoder::align16)},
    {"BayerBG_RAW12_CSI",       BAYER_FUNC(cv::COLOR_BayerBG2RGB,12,ImageDecoder::CSI)},
    {"BayerGB_RAW12_CSI",       BAYER_FUNC(cv::COLOR_BayerGB2RGB,12,ImageDecoder::CSI)},
    {"BayerRG_RAW12_CSI",       BAYER_FUNC(cv::COLOR_BayerRG2RGB,12,ImageDecoder::CSI)},
    {"BayerGR_RAW12_CSI",       BAYER_FUNC(cv::COLOR_BayerGR2RGB,12,ImageDecoder::CSI)},
    {"BayerBG_RAW12_COMPACT",   BAYER_FUNC(cv::COLOR_BayerBG2RGB,12,ImageDecoder::compact)},
    {"BayerGB_RAW12_COMPACT",   BAYER_FUNC(cv::COLOR_BayerGB2RGB,12,ImageDecoder::compact)},
    {"BayerRG_RAW12_COMPACT",   BAYER_FUNC(cv::COLOR_BayerRG2RGB,12,ImageDecoder::compact)},
    {"BayerGR_RAW12_COMPACT",   BAYER_FUNC(cv::COLOR_BayerGR2RGB,12,ImageDecoder::compact)},
    {"BayerBG_RAW12_ALIGN16",   BAYER_FUNC(cv::COLOR_BayerBG2RGB,10,ImageDecoder::align16)},
    {"BayerGB_RAW12_ALIGN16",   BAYER_FUNC(cv::COLOR_BayerGB2RGB,10,ImageDecoder::align16)},
    {"BayerRG_RAW12_ALIGN16",   BAYER_FUNC(cv::COLOR_BayerRG2RGB,10,ImageDecoder::align16)},
    {"BayerGR_RAW12_ALIGN16",   BAYER_FUNC(cv::COLOR_BayerGR2RGB,10,ImageDecoder::align16)},
    {"BayerBG_RAW16",           BAYER_FUNC(cv::COLOR_BayerBG2RGB,16,ImageDecoder::CSI)},
    {"BayerGB_RAW16",           BAYER_FUNC(cv::COLOR_BayerGB2RGB,16,ImageDecoder::CSI)},
    {"BayerRG_RAW16",           BAYER_FUNC(cv::COLOR_BayerRG2RGB,16,ImageDecoder::CSI)},
    {"BayerGR_RAW16",           BAYER_FUNC(cv::COLOR_BayerGR2RGB,16,ImageDecoder::CSI)},
    {"PNG",                     ImageDecoder::png},
};

QList<cv::Mat*> ImageDecoder::yv12(const QString &yuvfilename,int W, int H, int startframe, int totalframe) {
    QList<cv::Mat*> rgbImglist;
    cv::Mat yuvImg;
    QFile file(yuvfilename);
    file.open(QFile::ReadOnly);
    QDataStream out(&file);
    out.skipRawData(startframe*W*H*3/2);

    while((!out.atEnd()) && totalframe != 0) {
        cv::Mat *rgbImg = new cv::Mat;
        yuvImg.create(H*3/2, W, CV_8UC1);
        out.readRawData((char *)yuvImg.data,W*H*3/2);
        cvtColor(yuvImg, *rgbImg, cv::COLOR_YUV2RGB_YV12);
        totalframe--;
        rgbImglist.insert(rgbImglist.constEnd(), rgbImg);
    }

    file.close();

    return rgbImglist;
}

QList<cv::Mat*> ImageDecoder::i420(const QString &yuvfilename,int W, int H, int startframe, int totalframe) {
    QList<cv::Mat*> rgbImglist;
    cv::Mat yuvImg;
    QFile file(yuvfilename);
    file.open(QFile::ReadOnly);
    file.seek(startframe*W*H*3/2);
    QDataStream out(&file);

    while((!out.atEnd()) && (totalframe != 0)) {
        cv::Mat *rgbImg = new cv::Mat;
        yuvImg.create(H*3/2, W, CV_8UC1);
        out.readRawData((char *)yuvImg.data,W*H*3/2);
        cvtColor(yuvImg, *rgbImg, cv::COLOR_YUV2RGB_I420);
        totalframe--;
        rgbImglist.insert(rgbImglist.constEnd(), rgbImg);
    }

    file.close();

    return rgbImglist;
}

QList<cv::Mat*> ImageDecoder::nv21(const QString &yuvfilename,int W, int H, int startframe, int totalframe) {
    QList<cv::Mat*> rgbImglist;
    cv::Mat yuvImg;
    QFile file(yuvfilename);
    file.open(QFile::ReadOnly);
    file.seek(startframe*W*H*3/2);
    QDataStream out(&file);

    while((!out.atEnd()) && (totalframe != 0)) {
        cv::Mat *rgbImg = new cv::Mat;
        yuvImg.create(H*3/2, W, CV_8UC1);
        out.readRawData((char *)yuvImg.data,W*H*3/2);
        cvtColor(yuvImg, *rgbImg, cv::COLOR_YUV2RGB_NV12); // NV21
        totalframe--;
        rgbImglist.insert(rgbImglist.constEnd(), rgbImg);
    }

    file.close();

    return rgbImglist;
}

QList<cv::Mat*> ImageDecoder::nv12(const QString &yuvfilename,int W, int H, int startframe, int totalframe) {
    QList<cv::Mat*> rgbImglist;
    cv::Mat yuvImg;
    QFile file(yuvfilename);
    file.open(QFile::ReadOnly);
    file.seek(startframe*W*H*3/2);
    QDataStream out(&file);

    while((!out.atEnd()) && (totalframe != 0)) {
        cv::Mat *rgbImg = new cv::Mat;
        yuvImg.create(H*3/2, W, CV_8UC1);
        out.readRawData((char *)yuvImg.data,W*H*3/2);
        cvtColor(yuvImg, *rgbImg, cv::COLOR_YUV2RGB_NV21); // NV12
        totalframe--;
        rgbImglist.insert(rgbImglist.constEnd(), rgbImg);
    }

    file.close();

    return rgbImglist;
}

QList<cv::Mat*> ImageDecoder::yuy2(const QString &yuvfilename,int W, int H, int startframe, int totalframe) {
    QList<cv::Mat*> rgbImglist;
    cv::Mat yuvImg;
    QFile file(yuvfilename);
    file.open(QFile::ReadOnly);
    file.seek(startframe*W*H*2);
    QDataStream out(&file);

    while((!out.atEnd()) && (totalframe != 0)) {
        cv::Mat *rgbImg = new cv::Mat;
        yuvImg.create(H, W, CV_8UC2);
        out.readRawData((char *)yuvImg.data,W*H*2);
        cvtColor(yuvImg, *rgbImg, cv::COLOR_YUV2RGB_YUY2);
        totalframe--;
        rgbImglist.insert(rgbImglist.constEnd(), rgbImg);
    }

    file.close();

    return rgbImglist;
}

QList<cv::Mat*> ImageDecoder::yvyu(const QString &yuvfilename,int W, int H, int startframe, int totalframe) {
    QList<cv::Mat*> rgbImglist;
    cv::Mat yuvImg;
    QFile file(yuvfilename);
    file.open(QFile::ReadOnly);
    file.seek(startframe*W*H*2);
    QDataStream out(&file);

    while((!out.atEnd()) && (totalframe != 0)) {
        cv::Mat *rgbImg = new cv::Mat;
        yuvImg.create(H, W, CV_8UC2);
        out.readRawData((char *)yuvImg.data,W*H*2);
        cvtColor(yuvImg, *rgbImg, cv::COLOR_YUV2RGB_YVYU);
        totalframe--;
        rgbImglist.insert(rgbImglist.constEnd(), rgbImg);
    }

    file.close();

    return rgbImglist;
}

QList<cv::Mat*> ImageDecoder::uyvy(const QString &yuvfilename,int W, int H, int startframe, int totalframe) {
    QList<cv::Mat*> rgbImglist;
    cv::Mat yuvImg;
    QFile file(yuvfilename);
    file.open(QFile::ReadOnly);
    file.seek(startframe*W*H*2);
    QDataStream out(&file);

    while((!out.atEnd()) && (totalframe != 0)) {
        cv::Mat *rgbImg = new cv::Mat;
        yuvImg.create(H, W, CV_8UC2);
        out.readRawData((char *)yuvImg.data,W*H*2);
        cvtColor(yuvImg, *rgbImg, cv::COLOR_YUV2RGB_UYVY);
        totalframe--;
        rgbImglist.insert(rgbImglist.constEnd(), rgbImg);
    }

    file.close();

    return rgbImglist;
}

QList<cv::Mat*> ImageDecoder::yuv444(const QString &yuvfilename,int W, int H, int startframe, int totalframe) {
    QList<cv::Mat*> rgbImglist;
    cv::Mat yuvImg;
    QFile file(yuvfilename);
    file.open(QFile::ReadOnly);
    file.seek(startframe*W*H*3);
    QDataStream out(&file);

    while((!out.atEnd()) && (totalframe != 0)) {
        cv::Mat *rgbImg = new cv::Mat;
        yuvImg.create(H, W, CV_8UC3);
        out.readRawData((char *)yuvImg.data,W*H*3);
        cvtColor(yuvImg, *rgbImg, cv::COLOR_YUV2RGB);
        totalframe--;
        rgbImglist.insert(rgbImglist.constEnd(), rgbImg);
    }

    file.close();

    return rgbImglist;
}

QList<cv::Mat*> ImageDecoder::rgb565_little_endian(const QString &yuvfilename,int W, int H, int startframe, int totalframe) {
    QList<cv::Mat*> rgbImglist;
    cv::Mat yuvImg;
    QFile file(yuvfilename);
    file.open(QFile::ReadOnly);
    file.seek(startframe*W*H*2);
    QDataStream out(&file);

    while((!out.atEnd()) && (totalframe != 0)) {
        cv::Mat *rgbImg = new cv::Mat;
        yuvImg.create(H, W, CV_8UC2);
        out.readRawData((char *)yuvImg.data,W*H*2);
        cv::Mat bgrImg;
        cvtColor(yuvImg, bgrImg, cv::COLOR_BGR5652RGB);
        cvtColor(bgrImg, *rgbImg, cv::COLOR_BGR2RGB);
        totalframe--;
        rgbImglist.insert(rgbImglist.constEnd(), rgbImg);
    }

    file.close();

    return rgbImglist;
}

QList<cv::Mat*> ImageDecoder::rgb565_big_endian(const QString &yuvfilename,int W, int H, int startframe, int totalframe) {
    QList<cv::Mat*> rgbImglist;
    cv::Mat yuvImg;
    QFile file(yuvfilename);
    file.open(QFile::ReadOnly);
    file.seek(startframe*W*H*2);
    QDataStream out(&file);

    while((!out.atEnd()) && (totalframe != 0)) {
        cv::Mat *rgbImg = new cv::Mat;
        yuvImg.create(H, W, CV_8UC2);
        out.readRawData((char *)yuvImg.data,W*H*2);
        short *raw_buff = (short *)yuvImg.data;
        for(int i=0;i < W*H;i++)
        {
            raw_buff[i] = qFromBigEndian(raw_buff[i]);
        }
        cv::Mat bgrImg;
        cvtColor(yuvImg, bgrImg, cv::COLOR_BGR5652RGB);
        cvtColor(bgrImg, *rgbImg, cv::COLOR_BGR2RGB);
        totalframe--;
        rgbImglist.insert(rgbImglist.constEnd(), rgbImg);
    }

    file.close();

    return rgbImglist;
}

QList<cv::Mat*> ImageDecoder::bgr565_little_endian(const QString &yuvfilename,int W, int H, int startframe, int totalframe) {
    QList<cv::Mat*> rgbImglist;
    cv::Mat yuvImg;
    QFile file(yuvfilename);
    file.open(QFile::ReadOnly);
    file.seek(startframe*W*H*2);
    QDataStream out(&file);

    while((!out.atEnd()) && (totalframe != 0)) {
        cv::Mat *rgbImg = new cv::Mat;
        yuvImg.create(H, W, CV_8UC2);
        out.readRawData((char *)yuvImg.data,W*H*2);
        cvtColor(yuvImg, *rgbImg, cv::COLOR_BGR5652RGB);
        totalframe--;
        rgbImglist.insert(rgbImglist.constEnd(), rgbImg);
    }

    file.close();

    return rgbImglist;
}

QList<cv::Mat*> ImageDecoder::bgr565_big_endian(const QString &yuvfilename,int W, int H, int startframe, int totalframe) {
    QList<cv::Mat*> rgbImglist;
    cv::Mat yuvImg;
    QFile file(yuvfilename);
    file.open(QFile::ReadOnly);
    file.seek(startframe*W*H*2);
    QDataStream out(&file);

    while((!out.atEnd()) && (totalframe != 0)) {
        cv::Mat *rgbImg = new cv::Mat;
        yuvImg.create(H, W, CV_8UC2);
        out.readRawData((char *)yuvImg.data,W*H*2);
        short *raw_buff = (short *)yuvImg.data;
        for(int i=0;i < W*H;i++)
        {
            raw_buff[i] = qFromBigEndian(raw_buff[i]);
        }
        cvtColor(yuvImg, *rgbImg, cv::COLOR_BGR5652RGB);
        totalframe--;
        rgbImglist.insert(rgbImglist.constEnd(), rgbImg);
    }

    file.close();

    return rgbImglist;
}

QList<cv::Mat*> ImageDecoder::rgb888(const QString &yuvfilename,int W, int H, int startframe, int totalframe) {
    QList<cv::Mat*> rgbImglist;
    cv::Mat yuvImg;
    QFile file(yuvfilename);
    file.open(QFile::ReadOnly);
    file.seek(startframe*W*H*3);
    QDataStream out(&file);

    while((!out.atEnd()) && (totalframe != 0)) {
        cv::Mat *rgbImg = new cv::Mat;
        yuvImg.create(H, W, CV_8UC3);
        out.readRawData((char *)yuvImg.data,W*H*3);
        cvtColor(yuvImg, *rgbImg, cv::COLOR_BGR2RGB);
        totalframe--;
        rgbImglist.insert(rgbImglist.constEnd(), rgbImg);
    }

    file.close();

    return rgbImglist;
}

QList<cv::Mat*> ImageDecoder::bayer(const QString &yuvfilename,int W, int H, int startframe, int totalframe,int code,int bit, BayerFormat type) {
    QList<cv::Mat*> rgbImglist;
    cv::Mat yuvImg;
    uint32_t fsize = 0;
    switch (bit) {
    case 8:
        fsize = W*H;
        break;
    case 10:
        if(type == align16) {
            fsize = W*H*2;
        } else {
            fsize = W*H*5/4;
        }
        break;
    case 12:
        if(type == align16) {
            fsize = W*H*2;
        } else {
            fsize = W*H*3/2;
        }
        break;
    case 16:
        fsize = W*H*2;
        break;
    }

    QFile file(yuvfilename);
    file.open(QFile::ReadOnly);
    file.seek(startframe*fsize);
    QDataStream out(&file);
    uint8_t *temp = new uint8_t[fsize];

    while((!out.atEnd()) && (totalframe != 0)) {
        cv::Mat *rgbImg = new cv::Mat;
        yuvImg.create(H, W, CV_8U);
        uint8_t * dest = (uint8_t *)yuvImg.data;
        switch (bit) {
        case 8: {
            out.readRawData((char *)temp,fsize);
            for(int i=0;i<W*H;i++) {
                dest[i] = ((uint8_t)temp[i]);
            }
            break;
        }
        case 10: {
            out.readRawData((char *)temp,fsize);
            if(type == compact) {
                for(int i=0,j=0;i<W*H*5/4;i+=5) {
                    uint16_t piex[5] = {(uint16_t)temp[i],(uint16_t)temp[i+1],(uint16_t)temp[i+2],(uint16_t)temp[i+3],(uint16_t)temp[i+4]};
                    dest[j+0] = (uint8_t)((((piex[0]>>0)&0xff) | ((piex[1]<<8)&0x300))/4);
                    dest[j+1] = (uint8_t)((((piex[1]>>2)&0x3f) | ((piex[2]<<6)&0x3c0))/4);
                    dest[j+2] = (uint8_t)((((piex[2]>>4)&0xf) | ((piex[3]<<4)&0x3f0))/4);
                    dest[j+3] = (uint8_t)((((piex[3]>>6)&0x3) | ((piex[4]<<2)&0x3fc))/4);
                    j+=4;
                }
            } else if(type == CSI) {
                for(int i=0,j=0;i<W*H*5/4;i+=5) {
                    uint16_t piex[5] = {(uint16_t)temp[i],(uint16_t)temp[i+1],(uint16_t)temp[i+2],(uint16_t)temp[i+3],(uint16_t)temp[i+4]};
                    dest[j+0] = (uint8_t)(((piex[0]<<2) | ((piex[4]>>0)&0x3))/4);
                    dest[j+1] = (uint8_t)(((piex[1]<<2) | ((piex[4]>>2)&0x3))/4);
                    dest[j+2] = (uint8_t)(((piex[2]<<2) | ((piex[4]>>4)&0x3))/4);
                    dest[j+3] = (uint8_t)(((piex[3]<<2) | ((piex[4]>>6)&0x3))/4);
                    j+=4;
                }
            } else if(type == align16) {
                for(int i=0,j=0;i<W*H*2;i+=2) {
                    uint16_t piex[2] = {(uint16_t)temp[i],(uint16_t)temp[i+1]};
                    dest[j] = (uint8_t)(((piex[1]<<8) | (piex[0]&0xff))/4);
                    j++;
                }
            }
            break;
        }
        case 12: {
            out.readRawData((char *)temp,fsize);
            if(compact) {
                for(int i=0,j=0;i<W*H*3/2;i+=3) {
                    uint16_t piex[3] = {(uint16_t)temp[i],(uint16_t)temp[i+1],(uint16_t)temp[i+2]};
                    dest[j+0] = (uint8_t)((((piex[0]>>0)&0xff) | ((piex[1]<<8)&0xf00))/16);
                    dest[j+1] = (uint8_t)((((piex[1]>>4)&0xf) | ((piex[2]<<4)&0xff0))/16);
                    j+=2;
                }
            } else if(type == CSI) {
                for(int i=0,j=0;i<W*H*3/2;i+=3) {
                    uint16_t piex[3] = {(uint16_t)temp[i],(uint16_t)temp[i+1],(uint16_t)temp[i+2]};
                    dest[j+0] = (uint8_t)(((piex[0]<<4) | ((piex[2]>>0)&0xf))/16);
                    dest[j+1] = (uint8_t)(((piex[1]<<4) | ((piex[2]>>4)&0xf))/16);
                    j+=2;
                }
            } else if(type == align16) {
                for(int i=0,j=0;i<W*H*2;i+=2) {
                    uint16_t piex[2] = {(uint16_t)temp[i],(uint16_t)temp[i+1]};
                    dest[j] = (uint8_t)(((piex[1]<<8) | (piex[0]&0xff))/16);
                    j++;
                }
            }
            break;
        }
        case 16: {
            out.readRawData((char *)temp,fsize);
            for(int i=0,j=0;i<W*H*2;i+=2) {
                uint16_t piex[2] = {(uint16_t)temp[i],(uint16_t)temp[i+1]};
                dest[j] = (uint8_t)(((piex[1]<<8) | (piex[0]&0xff))/256);
                j++;
            }
            break;
        }
        default:
            break;
        }
        cvtColor(yuvImg, *rgbImg, code);
        totalframe--;
        rgbImglist.insert(rgbImglist.constEnd(), rgbImg);
    }

    delete[] temp;
    file.close();

    return rgbImglist;
}

QList<cv::Mat*> ImageDecoder::png(const QString &yuvfilename,int W, int H, int startframe, int totalframe) {
    QList<cv::Mat*> rgbImglist;

    cv::Mat *rgbImg = new cv::Mat;
    *rgbImg = cv::imread(yuvfilename.toStdString());
    rgbImglist.insert(rgbImglist.constEnd(), rgbImg);

    Q_UNUSED(W);
    Q_UNUSED(H);
    Q_UNUSED(startframe);
    Q_UNUSED(totalframe);
    return rgbImglist;
}
