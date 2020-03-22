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
    {"YV12", YUV2RGB::yv12},
    {"I420", YUV2RGB::i420},
    {"YUY2", nullptr},
    {"UYUV", nullptr},
    {"4:2:2", nullptr},
    {"4:4:4", nullptr},
};

YUV2RGB::YUV2RGB()
{

}


QList<cv::Mat*> YUV2RGB::yv12(QString yuvfilename,int W, int H, int startframe, int totalframe)
{
    QList<cv::Mat*> yuvImglist;
    Mat yuvImg;
    QFile file(yuvfilename);
    QFileInfo fileInfo(yuvfilename);
    file.open(QFile::ReadOnly);
    file.seek(startframe*W*H*3/2);
    QDataStream out(&file);

    //while(out.atEnd() || totalframe == 0)
    while(out.atEnd() || totalframe != 0)
    {
        Mat *rgbImg = new Mat;
        yuvImg.create(H*3/2, W, CV_8UC1);
        out.readRawData((char *)yuvImg.data,W*H*3/2);
        cvtColor(yuvImg, *rgbImg, COLOR_YUV2RGB_YV12);
        totalframe--;
        yuvImglist.insert(yuvImglist.end(), rgbImg);
    }

    file.close();

    return yuvImglist;
}


QList<cv::Mat*> YUV2RGB::i420(QString yuvfilename,int W, int H, int startframe, int totalframe)
{
    QList<cv::Mat*> yuvImglist;
    Mat yuvImg;
    QFile file(yuvfilename);
    QFileInfo fileInfo(yuvfilename);
    file.open(QFile::ReadOnly);
    file.seek(startframe*W*H*3/2);
    QDataStream out(&file);

    while((!out.atEnd()) || (totalframe != 0))
    {
        Mat *rgbImg = new Mat;
        yuvImg.create(H*3/2, W, CV_8UC1);
        out.readRawData((char *)yuvImg.data,W*H*3/2);
        cvtColor(yuvImg, *rgbImg, COLOR_YUV2RGB_I420);
        totalframe--;
        yuvImglist.insert(yuvImglist.end(), rgbImg);
    }

    file.close();

    return yuvImglist;
}
