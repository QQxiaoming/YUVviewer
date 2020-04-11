#include "YUVdecoder.h"
#include <QList>
#include <QFile>
#include <QFileInfo>
#include <QDataStream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>

using namespace cv;

QMap<QString, yuvdecoder_t> YUV2RGB::yuvdecoder_map =
{
    {"YV12",      YUV2RGB::yv12},
    {"YU12/I420", YUV2RGB::i420},
    {"NV21",      YUV2RGB::nv21},
    {"NV12",      YUV2RGB::nv12},
    {"YUY2/YUYV", YUV2RGB::yuy2},
    {"YVYU",      YUV2RGB::yvyu},
    {"UYVY",      YUV2RGB::uyvy},
    {"4:4:4",     YUV2RGB::yuv444},
};

YUV2RGB::YUV2RGB()
{

}

QList<cv::Mat*> YUV2RGB::yv12(QString yuvfilename,int W, int H, int startframe, int totalframe)
{
    QList<cv::Mat*> rgbImglist;
    Mat yuvImg;
    QFile file(yuvfilename);
    QFileInfo fileInfo(yuvfilename);
    file.open(QFile::ReadOnly);
    QDataStream out(&file);
    out.skipRawData(startframe*W*H*3/2);

    while((!out.atEnd()) && totalframe != 0)
    {
        Mat *rgbImg = new Mat;
        yuvImg.create(H*3/2, W, CV_8UC1);
        out.readRawData((char *)yuvImg.data,W*H*3/2);
        cvtColor(yuvImg, *rgbImg, COLOR_YUV2RGB_YV12);
        totalframe--;
        rgbImglist.insert(rgbImglist.end(), rgbImg);
    }

    file.close();

    return rgbImglist;
}

QList<cv::Mat*> YUV2RGB::i420(QString yuvfilename,int W, int H, int startframe, int totalframe)
{
    QList<cv::Mat*> rgbImglist;
    Mat yuvImg;
    QFile file(yuvfilename);
    QFileInfo fileInfo(yuvfilename);
    file.open(QFile::ReadOnly);
    file.seek(startframe*W*H*3/2);
    QDataStream out(&file);

    while((!out.atEnd()) && (totalframe != 0))
    {
        Mat *rgbImg = new Mat;
        yuvImg.create(H*3/2, W, CV_8UC1);
        out.readRawData((char *)yuvImg.data,W*H*3/2);
        cvtColor(yuvImg, *rgbImg, COLOR_YUV2RGB_I420);
        totalframe--;
        rgbImglist.insert(rgbImglist.end(), rgbImg);
    }

    file.close();

    return rgbImglist;
}

QList<cv::Mat*> YUV2RGB::nv21(QString yuvfilename,int W, int H, int startframe, int totalframe)
{
    QList<cv::Mat*> rgbImglist;
    Mat yuvImg;
    QFile file(yuvfilename);
    QFileInfo fileInfo(yuvfilename);
    file.open(QFile::ReadOnly);
    file.seek(startframe*W*H*3/2);
    QDataStream out(&file);

    while((!out.atEnd()) && (totalframe != 0))
    {
        Mat *rgbImg = new Mat;
        yuvImg.create(H*3/2, W, CV_8UC1);
        out.readRawData((char *)yuvImg.data,W*H*3/2);
        cvtColor(yuvImg, *rgbImg, COLOR_YUV2RGB_NV21);
        totalframe--;
        rgbImglist.insert(rgbImglist.end(), rgbImg);
    }

    file.close();

    return rgbImglist;
}

QList<cv::Mat*> YUV2RGB::nv12(QString yuvfilename,int W, int H, int startframe, int totalframe)
{
    QList<cv::Mat*> rgbImglist;
    Mat yuvImg;
    QFile file(yuvfilename);
    QFileInfo fileInfo(yuvfilename);
    file.open(QFile::ReadOnly);
    file.seek(startframe*W*H*3/2);
    QDataStream out(&file);

    while((!out.atEnd()) && (totalframe != 0))
    {
        Mat *rgbImg = new Mat;
        yuvImg.create(H*3/2, W, CV_8UC1);
        out.readRawData((char *)yuvImg.data,W*H*3/2);
        cvtColor(yuvImg, *rgbImg, COLOR_YUV2RGB_NV12);
        totalframe--;
        rgbImglist.insert(rgbImglist.end(), rgbImg);
    }

    file.close();

    return rgbImglist;
}

QList<cv::Mat*> YUV2RGB::yuy2(QString yuvfilename,int W, int H, int startframe, int totalframe)
{
    QList<cv::Mat*> rgbImglist;
    Mat yuvImg;
    QFile file(yuvfilename);
    QFileInfo fileInfo(yuvfilename);
    file.open(QFile::ReadOnly);
    file.seek(startframe*W*H*2);
    QDataStream out(&file);

    while((!out.atEnd()) && (totalframe != 0))
    {
        Mat *rgbImg = new Mat;
        yuvImg.create(H, W, CV_8UC2);
        out.readRawData((char *)yuvImg.data,W*H*2);
        cvtColor(yuvImg, *rgbImg, COLOR_YUV2RGB_YUY2);
        totalframe--;
        rgbImglist.insert(rgbImglist.end(), rgbImg);
    }

    file.close();

    return rgbImglist;
}

QList<cv::Mat*> YUV2RGB::yvyu(QString yuvfilename,int W, int H, int startframe, int totalframe)
{
    QList<cv::Mat*> rgbImglist;
    Mat yuvImg;
    QFile file(yuvfilename);
    QFileInfo fileInfo(yuvfilename);
    file.open(QFile::ReadOnly);
    file.seek(startframe*W*H*2);
    QDataStream out(&file);

    while((!out.atEnd()) && (totalframe != 0))
    {
        Mat *rgbImg = new Mat;
        yuvImg.create(H, W, CV_8UC2);
        out.readRawData((char *)yuvImg.data,W*H*2);
        cvtColor(yuvImg, *rgbImg, COLOR_YUV2RGB_YVYU);
        totalframe--;
        rgbImglist.insert(rgbImglist.end(), rgbImg);
    }

    file.close();

    return rgbImglist;
}

QList<cv::Mat*> YUV2RGB::uyvy(QString yuvfilename,int W, int H, int startframe, int totalframe)
{
    QList<cv::Mat*> rgbImglist;
    Mat yuvImg;
    QFile file(yuvfilename);
    QFileInfo fileInfo(yuvfilename);
    file.open(QFile::ReadOnly);
    file.seek(startframe*W*H*2);
    QDataStream out(&file);

    while((!out.atEnd()) && (totalframe != 0))
    {
        Mat *rgbImg = new Mat;
        yuvImg.create(H, W, CV_8UC2);
        out.readRawData((char *)yuvImg.data,W*H*2);
        cvtColor(yuvImg, *rgbImg, COLOR_YUV2RGB_UYVY);
        totalframe--;
        rgbImglist.insert(rgbImglist.end(), rgbImg);
    }

    file.close();

    return rgbImglist;
}

QList<cv::Mat*> YUV2RGB::yuv444(QString yuvfilename,int W, int H, int startframe, int totalframe)
{
    QList<cv::Mat*> rgbImglist;
    Mat yuvImg;
    QFile file(yuvfilename);
    QFileInfo fileInfo(yuvfilename);
    file.open(QFile::ReadOnly);
    file.seek(startframe*W*H*3);
    QDataStream out(&file);

    while((!out.atEnd()) && (totalframe != 0))
    {
        Mat *rgbImg = new Mat;
        yuvImg.create(H, W, CV_8UC3);
        out.readRawData((char *)yuvImg.data,W*H*3);
        cvtColor(yuvImg, *rgbImg, COLOR_YUV2RGB);
        totalframe--;
        rgbImglist.insert(rgbImglist.end(), rgbImg);
    }

    file.close();

    return rgbImglist;
}
