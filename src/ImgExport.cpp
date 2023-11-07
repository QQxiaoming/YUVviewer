/*
 * This file is part of the https://github.com/QQxiaoming/YUVviewer.git
 * project.
 *
 * Copyright (C) 2022 Quard <2014500726@smail.xtu.edu.cn>
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
#include <QMessageBox>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>

#include "filedialog.h"
#include "ImgExport.h"

#include "ui_UI_ImgExport.h"

ImgExport::ImgExport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImgExportWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(buttonBoxAccepted()));
    QObject::connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(buttonBoxRejected()));
}

ImgExport::~ImgExport()
{
    delete ui;
}

void ImgExport::setSaveFileName(const QString &name)
{
    savefilename = name;
}

void ImgExport::setSaveImage(QImage *image)
{
    currentImg = image;
}

void ImgExport::showEvent(QShowEvent* event) {
    //"png"
    ui->f0RadioButton->setChecked(true);
    QDialog::showEvent(event);
}

void ImgExport::buttonBoxAccepted(void)
{
    QMap<QString, QPair<QRadioButton *, int>> table = {
        {"png"     , {ui->f0RadioButton  , 0} },
        {"yv12"    , {ui->f1RadioButton  , 1} },
        {"i420"    , {ui->f2RadioButton  , 1} },
        {"nv21"    , {ui->f3RadioButton  , 1} },
        {"nv12"    , {ui->f4RadioButton  , 1} },
        {"yuyv"    , {ui->f5RadioButton  , 1} },
        {"yvyu"    , {ui->f6RadioButton  , 1} },
        {"uyvy"    , {ui->f7RadioButton  , 1} },
        {"4:4:4"   , {ui->f8RadioButton  , 1} },
        {"RGB565_L", {ui->f9RadioButton  , 2} },
        {"RGB565_B", {ui->f10RadioButton , 2} },
        {"BGR565_L", {ui->f11RadioButton , 2} },
        {"BGR565_B", {ui->f12RadioButton , 2} },
        {"RGB888"  , {ui->f13RadioButton , 2} },
        {"BGGR_8"  , {ui->f14RadioButton , 3} },
        {"GBRG_8"  , {ui->f15RadioButton , 3} },
        {"RGGB_8"  , {ui->f16RadioButton , 3} },
        {"GRBG_8"  , {ui->f17RadioButton , 3} },
        {"BGGR_10" , {ui->f18RadioButton , 3} },
        {"GBRG_10" , {ui->f19RadioButton , 3} },
        {"RGGB_10" , {ui->f20RadioButton , 3} },
        {"GRBG_10" , {ui->f21RadioButton , 3} },
        {"BGGR_12" , {ui->f22RadioButton , 3} },
        {"GBRG_12" , {ui->f23RadioButton , 3} },
        {"RGGB_12" , {ui->f24RadioButton , 3} },
        {"GRBG_12" , {ui->f25RadioButton , 3} },
        {"BGGR_16" , {ui->f26RadioButton , 3} },
        {"GBRG_16" , {ui->f27RadioButton , 3} },
        {"RGGB_16" , {ui->f28RadioButton , 3} },
        {"GRBG_16" , {ui->f29RadioButton , 3} },
    };

    foreach(QString key, table.keys()) {
        if(table.value(key).first->isChecked()) {
            switch (table.value(key).second)
            {
            case 0:
                export_png(currentImg,savefilename);
                break;
            case 1:
                export_yuv(currentImg,key,savefilename);
                break;
            case 2:
                export_rgb(currentImg,key,savefilename);
                break;
            case 3:
                export_bayer(currentImg,key.split("_").at(0),key.split("_").at(1).toInt(),savefilename);
                break;
            default:
                break;
            }
        }
    }

    emit this->accepted();
}

void ImgExport::buttonBoxRejected(void)
{
    emit this->rejected();
}

void ImgExport::export_png(QImage *Img, const QString &name)
{
    QString savefile_name = FileDialog::getSaveFileName(this, tr("Save File"), name + "-png" + ".png", "Image files(*.png)");
    if(savefile_name != nullptr) {
        Img->save(savefile_name);
    }
}

void ImgExport::export_yuv(QImage *Img, const QString &sequence, const QString &name)
{
    QString savefile_name = FileDialog::getSaveFileName(this, tr("Save File"), name + "-" + sequence + ".yuv", "YUV files(*.yuv)");
    if(savefile_name != nullptr) {
        QFile save(savefile_name);
        if (save.open(QIODevice::WriteOnly)) {
            cv::Mat yuvImg;
            cv::Mat rgbImg;
            rgbImg.create(Img->height(), Img->width(), CV_8UC3);
            uint8_t *dest = (uint8_t *)rgbImg.data;
            for(int j = 0;j < Img->height();j++) {
                for(int i = 0;i < Img->width()*3;i+=3) {
                    dest[j*Img->width()*3+i+0] = (uint8_t)qBlue(Img->pixel(i/3,j));
                    dest[j*Img->width()*3+i+1] = (uint8_t)qGreen(Img->pixel(i/3,j));
                    dest[j*Img->width()*3+i+2] = (uint8_t)qRed(Img->pixel(i/3,j));
                }
            }

            if(sequence == "yv12") {
                cvtColor(rgbImg, yuvImg, cv::COLOR_RGB2YUV_YV12);
                save.write((const char *)yuvImg.data,Img->height()*Img->width()*3/2);
            } else if(sequence == "i420") {
                cvtColor(rgbImg, yuvImg, cv::COLOR_RGB2YUV_I420);
                save.write((const char *)yuvImg.data,Img->height()*Img->width()*3/2);
            } else if(sequence == "4:4:4") {
                cvtColor(rgbImg, yuvImg, cv::COLOR_RGB2YUV);
                save.write((const char *)yuvImg.data,Img->height()*Img->width()*3);
            } else if(sequence == "nv12") {
                cvtColor(rgbImg, yuvImg, cv::COLOR_RGB2YUV_YV12);
                save.write((const char *)yuvImg.data,Img->height()*Img->width());
                for(int i=0;i<Img->height()*Img->width()/2;i++) {
                    save.write((const char *)&yuvImg.data[i+Img->height()*Img->width()],1);
                    save.write((const char *)&yuvImg.data[i+Img->height()*Img->width()*5/4],1);
                }
            } else if(sequence == "nv21") {
                cvtColor(rgbImg, yuvImg, cv::COLOR_RGB2YUV_I420);
                save.write((const char *)yuvImg.data,Img->height()*Img->width());
                for(int i=0;i<Img->height()*Img->width()/2;i++) {
                    save.write((const char *)&yuvImg.data[i+Img->height()*Img->width()],1);
                    save.write((const char *)&yuvImg.data[i+Img->height()*Img->width()*5/4],1);
                }
            } else if(sequence == "yuyv") {
                cvtColor(rgbImg, yuvImg, cv::COLOR_RGB2YUV_I420);
                for(int j=0;j<Img->height();j++) {
                    if(j%2==0){
                        for(int i=0;i<Img->width();i+=2) {
                            save.write((const char *)&yuvImg.data[i+j*Img->width()],1);
                            save.write((const char *)&yuvImg.data[i/2+Img->height()*Img->width()+j*Img->width()/4],1);
                            save.write((const char *)&yuvImg.data[i+1+j*Img->width()],1);
                            save.write((const char *)&yuvImg.data[i/2+Img->height()*Img->width()*5/4+j*Img->width()/4],1);
                        }
                    } else {
                        for(int i=0;i<Img->width();i+=2) {
                            save.write((const char *)&yuvImg.data[i+j*Img->width()],1);
                            save.write((const char *)&yuvImg.data[i/2+Img->height()*Img->width()+(j-1)*Img->width()/4],1);
                            save.write((const char *)&yuvImg.data[i+1+j*Img->width()],1);
                            save.write((const char *)&yuvImg.data[i/2+Img->height()*Img->width()*5/4+(j-1)*Img->width()/4],1);
                        }
                    }
                }
            } else if(sequence == "yvyu") {
                cvtColor(rgbImg, yuvImg, cv::COLOR_RGB2YUV_YV12);
                for(int j=0;j<Img->height();j++) {
                    if(j%2==0){
                        for(int i=0;i<Img->width();i+=2) {
                            save.write((const char *)&yuvImg.data[i+j*Img->width()],1);
                            save.write((const char *)&yuvImg.data[i/2+Img->height()*Img->width()+j*Img->width()/4],1);
                            save.write((const char *)&yuvImg.data[i+1+j*Img->width()],1);
                            save.write((const char *)&yuvImg.data[i/2+Img->height()*Img->width()*5/4+j*Img->width()/4],1);
                        }
                    } else {
                        for(int i=0;i<Img->width();i+=2) {
                            save.write((const char *)&yuvImg.data[i+j*Img->width()],1);
                            save.write((const char *)&yuvImg.data[i/2+Img->height()*Img->width()+(j-1)*Img->width()/4],1);
                            save.write((const char *)&yuvImg.data[i+1+j*Img->width()],1);
                            save.write((const char *)&yuvImg.data[i/2+Img->height()*Img->width()*5/4+(j-1)*Img->width()/4],1);
                        }
                    }
                }
            } else if(sequence == "uyvy") {
                cvtColor(rgbImg, yuvImg, cv::COLOR_RGB2YUV_I420);
                for(int j=0;j<Img->height();j++) {
                    if(j%2==0){
                        for(int i=0;i<Img->width();i+=2) {
                            save.write((const char *)&yuvImg.data[i/2+Img->height()*Img->width()+j*Img->width()/4],1);
                            save.write((const char *)&yuvImg.data[i+j*Img->width()],1);
                            save.write((const char *)&yuvImg.data[i/2+Img->height()*Img->width()*5/4+j*Img->width()/4],1);
                            save.write((const char *)&yuvImg.data[i+1+j*Img->width()],1);
                        }
                    } else {
                        for(int i=0;i<Img->width();i+=2) {
                            save.write((const char *)&yuvImg.data[i/2+Img->height()*Img->width()+(j-1)*Img->width()/4],1);
                            save.write((const char *)&yuvImg.data[i+j*Img->width()],1);
                            save.write((const char *)&yuvImg.data[i/2+Img->height()*Img->width()*5/4+(j-1)*Img->width()/4],1);
                            save.write((const char *)&yuvImg.data[i+1+j*Img->width()],1);
                        }
                    }
                }
            }

            save.close();
        }
    }
}

void ImgExport::export_rgb(QImage *Img, const QString &sequence, const QString &name)
{
    QString savefile_name = FileDialog::getSaveFileName(this, tr("Save File"), name + "-" + sequence + ".data", "Data files(*.data)");
    if(savefile_name != nullptr) {
        QFile save(savefile_name);
        if (save.open(QIODevice::WriteOnly)) {
            cv::Mat yuvImg;
            cv::Mat rgbImg;
            rgbImg.create(Img->height(), Img->width(), CV_8UC3);
            uint8_t *dest = (uint8_t *)rgbImg.data;
            for(int j = 0;j < Img->height();j++) {
                for(int i = 0;i < Img->width()*3;i+=3) {
                    dest[j*Img->width()*3+i+0] = (uint8_t)qBlue(Img->pixel(i/3,j));
                    dest[j*Img->width()*3+i+1] = (uint8_t)qGreen(Img->pixel(i/3,j));
                    dest[j*Img->width()*3+i+2] = (uint8_t)qRed(Img->pixel(i/3,j));
                }
            }

            if(sequence == "RGB888") {
                cvtColor(rgbImg, yuvImg, cv::COLOR_RGB2BGR);
                save.write((const char *)yuvImg.data,Img->height()*Img->width()*3);
            } else if(sequence == "RGB565_L") {
                cvtColor(rgbImg, yuvImg, cv::COLOR_BGR2BGR565);
                save.write((const char *)yuvImg.data,Img->height()*Img->width()*2);
            } else if(sequence == "RGB565_B") {
                cvtColor(rgbImg, yuvImg, cv::COLOR_BGR2BGR565);
                for(int i=0;i<Img->height()*Img->width()*2;i+=2) {
                    save.write((const char *)&yuvImg.data[i+1],1);
                    save.write((const char *)&yuvImg.data[i+0],1);
                }
            } else if(sequence == "BGR565_L") {
                cvtColor(rgbImg, yuvImg, cv::COLOR_RGB2BGR565);
                save.write((const char *)yuvImg.data,Img->height()*Img->width()*2);
            } else if(sequence == "BGR565_B") {
                cvtColor(rgbImg, yuvImg, cv::COLOR_RGB2BGR565);
                for(int i=0;i<Img->height()*Img->width()*2;i+=2) {
                    save.write((const char *)&yuvImg.data[i+1],1);
                    save.write((const char *)&yuvImg.data[i+0],1);
                }
            }

            save.close();
        }
    }
}

void ImgExport::export_bayer(QImage *Img, const QString &sequence,int bit, const QString &name)
{
    QMessageBox::warning(this, "Warning",
                             "导出此格式将通过模拟抽样的方式，图像信息将会出现损失！",
                             QMessageBox::StandardButtons(QMessageBox::Ok));
    QString savefile_name = FileDialog::getSaveFileName(this, tr("Save File"), name + "-bayer" + sequence + QString::number(bit) +".raw", "RAW files(*.raw)");
    if(savefile_name != nullptr) {
        QFile save(savefile_name);
        if (save.open(QIODevice::WriteOnly)) {
            auto func_get_pixes = [&Img,&sequence](int i,int j,int odd) -> QList<unsigned char>
            {
                QRgb p0 = Img->pixel(i,j);
                QRgb p1 = Img->pixel(i+1,j);
                QRgb p2 = Img->pixel(i+2,j);
                QRgb p3 = Img->pixel(i+3,j);

                unsigned char B0 = qBlue(p0);
                unsigned char G0 = qGreen(p0);
                unsigned char R0 = qRed(p0);
                unsigned char B1 = qBlue(p1);
                unsigned char G1 = qGreen(p1);
                unsigned char R1 = qRed(p1);
                unsigned char B2 = qBlue(p2);
                unsigned char G2 = qGreen(p2);
                unsigned char R2 = qRed(p2);
                unsigned char B3 = qBlue(p3);
                unsigned char G3 = qGreen(p3);
                unsigned char R3 = qRed(p3);

                unsigned char P0_0 = 0;
                unsigned char P1_0 = 0;
                unsigned char P2_0 = 0;
                unsigned char P3_0 = 0;
                unsigned char P0_1 = 0;
                unsigned char P1_1 = 0;
                unsigned char P2_1 = 0;
                unsigned char P3_1 = 0;
                if(sequence == "BGGR") {
                    P0_0 = B0;
                    P1_0 = G1;
                    P2_0 = B2;
                    P3_0 = G3;
                    P0_1 = G0;
                    P1_1 = R1;
                    P2_1 = G2;
                    P3_1 = R3;
                } else if(sequence == "RGGB"){
                    P0_0 = R0;
                    P1_0 = G1;
                    P2_0 = R2;
                    P3_0 = G3;
                    P0_1 = G0;
                    P1_1 = B1;
                    P2_1 = G2;
                    P3_1 = B3;
                } else if(sequence == "GBRG"){
                    P0_0 = G0;
                    P1_0 = B1;
                    P2_0 = G2;
                    P3_0 = B3;
                    P0_1 = R0;
                    P1_1 = G1;
                    P2_1 = R2;
                    P3_1 = G3;
                } else if(sequence == "GRBG"){
                    P0_0 = G0;
                    P1_0 = R1;
                    P2_0 = G2;
                    P3_0 = R3;
                    P0_1 = B0;
                    P1_1 = G1;
                    P2_1 = B2;
                    P3_1 = G3;
                }

                if(odd)
                    return {P0_1,P1_1,P2_1,P3_1};
                else
                    return {P0_0,P1_0,P2_0,P3_0};
            };
            auto func_write_pixes = [&save,&bit](QList<unsigned char> P) -> void
            {
                if(bit == 8) {
                    save.write((const char *)&P[0],1);
                    save.write((const char *)&P[1],1);
                    save.write((const char *)&P[2],1);
                    save.write((const char *)&P[3],1);
                } else if(bit == 10) {
                    unsigned short P0_10 = P[0]*4;
                    unsigned short P1_10 = P[1]*4;
                    unsigned short P2_10 = P[2]*4;
                    unsigned short P3_10 = P[3]*4;
                    unsigned char piexl[5] = {
                        (unsigned char)(P0_10>>2),
                        (unsigned char)(P1_10>>2),
                        (unsigned char)(P2_10>>2),
                        (unsigned char)(P3_10>>2),
                        (unsigned char)(((P3_10&0x03)<<6)|((P2_10&0x03)<<4)|((P1_10&0x03)<<2)|(P0_10&0x03)) };
                    save.write((const char *)piexl,5);
                } else if(bit == 12) {
                    unsigned short P0_12 = P[0]*16;
                    unsigned short P1_12 = P[1]*16;
                    unsigned short P2_12 = P[2]*16;
                    unsigned short P3_12 = P[3]*16;
                    unsigned char piexl[6] = {
                        (unsigned char)(P0_12>>4),
                        (unsigned char)(P1_12>>4),
                        (unsigned char)(((P1_12&0x0f)<<4)|(P0_12&0x0f)),
                        (unsigned char)(P2_12>>4),
                        (unsigned char)(P3_12>>4),
                        (unsigned char)(((P3_12&0x0f)<<4)|(P2_12&0x0f)) 
                        };
                    save.write((const char *)piexl,6);
                } else if(bit == 16) {
                    unsigned short P0_16 = P[0]*256;
                    unsigned short P1_16 = P[1]*256;
                    unsigned short P2_16 = P[2]*256;
                    unsigned short P3_16 = P[3]*256;
                    save.write((const char *)&P0_16,2);
                    save.write((const char *)&P1_16,2);
                    save.write((const char *)&P2_16,2);
                    save.write((const char *)&P3_16,2);
                }
            };
            for(int j = 0;j < Img->height();j++) {
                if(j%2 == 0) {
                    for(int i = 0;i < Img->width();i+=4) {
                        func_write_pixes(func_get_pixes(i,j,0));
                    }
                } else {
                    for(int i = 0;i < Img->width();i+=4) {
                        func_write_pixes(func_get_pixes(i,j,1));
                    }
                }
            }
            save.close();
        }
    }
}
