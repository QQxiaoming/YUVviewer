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
#include <QImage>
#include <QFileInfo>
#include <QList>
#include <QString>
#include <QPoint>
#include <QPainter>
#include <QMessageBox>
#include <QDebug>
#include "filedialog.h"
#include "ImgViewer.h"
#include "YUVdecoder.h"
#include "ui_UI_ImgViewer.h"

YUVDecodeThread::YUVDecodeThread(QWidget *parent,const QString &yuvfilename,QString YUVFormat,
                                 int W, int H, int startframe, int totalframe) :
    QThread(parent) {
    this->window = parent;
    this->yuvfilename = yuvfilename;
    this->W = W;
    this->H = H;
    this->startframe = startframe;
    this->totalframe = totalframe;
    // 获取该格式的解码函数
    this->decoder = ImageDecoder::yuvdecoder_map.find(YUVFormat).value();
}

void YUVDecodeThread::run() {
    // 定义img列表用了保存每一帧的QImage*
    QList<QImage*> img_RGB_list;
    if(this->decoder == nullptr) {
        // 未能成功获取则返回无法解码
        emit finsh_signal(img_RGB_list,nullptr);
    } else {
        QList<cv::Mat*> frame_RGB_list;
        try{
            // 成功获取则返回计算结果
            frame_RGB_list = this->decoder(this->yuvfilename, this->W, this->H, this->startframe, this->totalframe);
        } catch(cv::Exception& e) {
            emit finsh_signal(img_RGB_list,nullptr);
            return;
        }
        // 将原始帧转换到QImage*并保存到img列表
        foreach( cv::Mat* img,frame_RGB_list) {
            // 提取图像的通道和尺寸，用于将OpenCV下的image转换成Qimage
            QImage *qImg = new QImage((const unsigned char*)(img->data), img->cols, img->rows, img->step, QImage::Format_RGB888,(QImageCleanupFunction)this->image_cleanup,img);
            *qImg = qImg->rgbSwapped();
            img_RGB_list.insert(img_RGB_list.end(), qImg);
        }
        emit finsh_signal(img_RGB_list,this->yuvfilename);
    }
}

void YUVDecodeThread::image_cleanup(cv::Mat* ptr) {
    delete ptr;
}

ImgViewer::ImgViewer(const QString &folderpath, QWidget *parent,QWidget *parentWindow) :
    QWidget(parent),
    ui(new Ui::ImgViewerWindow) {
    ui->setupUi(this);
    qRegisterMetaType<QList<QImage*>>("QList<QImage*>");
    this->parentWindow = parentWindow;
    this->folderpath = folderpath;
    imgExportWindow = new ImgExport(this);
    setWindowTitle("loading file, please wait ....");
    ui->left_PushButton->setFlat(true);
    ui->right_PushButton->setFlat(true);
    setMouseTracking(true);
    QObject::connect(ui->left_PushButton, SIGNAL(clicked()), this, SLOT(previousImg()));
    QObject::connect(ui->right_PushButton, SIGNAL(clicked()), this, SLOT(nextImg()));
    left_click = false;
}

ImgViewer::~ImgViewer() {
    delete imgExportWindow;
    delete ui;
}

bool ImgViewer::setFileList(QStringList filenamelist,QString YUVFormat, int W, int H, int startframe, int totalframe) {
    // 获取该格式的解码函数
    ImageDecoder::yuvdecoder_t decoder = ImageDecoder::yuvdecoder_map.find(YUVFormat).value();
    if(decoder == nullptr) {
        // 未能成功获取则返回无法解码
        return false;
    } else {
        // 成功获取解码器则准备解码
        ui->imgViewer->setText("");
        // 遍历文件列表
        foreach( QString filename, filenamelist) {
            QList<cv::Mat*> frame_RGB_list;
            try{
                // 使用获取的解码函数进行解码得到RGB的原始帧列表
                frame_RGB_list = decoder(filename, W, H, startframe, totalframe);
            } catch(cv::Exception& e) {
                continue;
            }
            if (frame_RGB_list.empty()) {
                return false;
            }
            // 定义img列表用来保存每一帧的Qimage*
            QList<QImage*> img_RGB_list;
            // 将原始帧转换到Qimage*并保存到img列表
            foreach (cv::Mat* img, frame_RGB_list) {
                // 提取图像的通道和尺寸，用于将OpenCV下的image转换成Qimage
                QImage *qImg = new QImage((const unsigned char*)(img->data), img->cols, img->rows, img->step, QImage::Format_RGB888, (QImageCleanupFunction)this->image_cleanup,img);
                *qImg = qImg->rgbSwapped();
                img_RGB_list.insert(img_RGB_list.end(), qImg);
            }
            // img_RGB_list以及文件名存入列表
            this->img_list.insert(this->img_list.end(),img_RGB_list);
            QFileInfo fileInfo(filename);
            this->filelist.insert(this->filelist.end(),fileInfo.fileName());
        }
        // 设置显示第一个YUV文件的第一帧图像
        this->currentImg_RGB_list = this->img_list.at(0);
        this->currentImg = this->currentImg_RGB_list.at(0);
        this->setWindowTitle(this->filelist.at(0)+"-0");
        this->scaled_img = this->currentImg->scaled(this->size());
        if(this->flipRGB) {
            this->scaled_img = this->scaled_img.rgbSwapped();
        }
        this->point = QPoint(0, 0);
        return true;
    }
}

void ImgViewer::image_cleanup(cv::Mat* ptr) {
    delete ptr;
}

void ImgViewer::reciveimgdata(QList<QImage*> img_RGB_list,QString filename) {
    if (!img_RGB_list.empty()) {
        // img_RGB_list以及文件名存入列表
        this->img_list.insert(this->img_list.end(),img_RGB_list);
        QFileInfo fileInfo(filename);
        this->filelist.insert(this->filelist.end(),fileInfo.fileName());
        if(this->img_list.count() == 1) {
            // 设置显示第一个YUV文件的第一帧图像
            ui->imgViewer->setText("");
            this->currentImg_RGB_list = this->img_list.at(0);
            this->currentImg = this->currentImg_RGB_list.at(0);
            this->setWindowTitle(this->filelist.at(0)+"-0");
            this->scaled_img = this->currentImg->scaled(this->size());
            if(this->flipRGB) {
                this->scaled_img = this->scaled_img.rgbSwapped();
            }
            this->point = QPoint(0, 0);
            this->repaint();
        }
    }

    this->decode_thread.pop_front();
    if (!this->decode_thread.empty()) {
        this->decode_thread[0]->start();
    } else {
        if(this->img_list.empty()) {
            QMessageBox::critical(this, "Error", "unknow error!!", QMessageBox::Ok);
            this->close();
        }
    }
}

bool ImgViewer::setFileList_multithreading(QStringList filenamelist,QString YUVFormat,
                                            int W, int H, int startframe, int totalframe) {
    // 获取该格式的解码函数
    ImageDecoder::yuvdecoder_t decoder = ImageDecoder::yuvdecoder_map.find(YUVFormat).value();
    if(decoder == nullptr) {
        // 未能成功获取则返回无法解码
        return false;
    }
    // 遍历文件列表
    foreach( QString filename, filenamelist) {
        YUVDecodeThread *decodeThread = new YUVDecodeThread(this, filename, YUVFormat, W, H, startframe, totalframe);
        QObject::connect(decodeThread, SIGNAL(finsh_signal(QList<QImage*>, QString)), this, SLOT(reciveimgdata(QList<QImage*>, QString)));
        this->decode_thread.insert(this->decode_thread.end(),decodeThread);
    }
    this->decode_thread[0]->start();
    return true;
}

void ImgViewer::closeEvent(QCloseEvent *event) {
    this->parentWindow->show();
    event->accept();
    if(!this->img_list.empty()) {
        foreach(QList<QImage*> list,this->img_list) {
            if(!list.empty()) {
                foreach(QImage* img,list) {
                    delete img;
                }
            }
        }
    }
}

void ImgViewer::draw_img(QPainter *painter) {
    painter->drawPixmap(this->point, QPixmap::fromImage(this->scaled_img));
}

void ImgViewer::draw_info(QPainter *painter) {
    if (isMouseInImg) {
        int r = qRed(currentMousePosColor);
        int g = qGreen(currentMousePosColor);
        int b = qBlue(currentMousePosColor);
        int x = currentMousePos.x();
        int y = currentMousePos.y();
        QString info = QString("x:%1 y:%2 R:%3 G:%4 B:%5").arg(x).arg(y).arg(r).arg(g).arg(b);
        QPen pen;
        pen.setColor(Qt::black);
        painter->setPen(pen);
        painter->drawText(20, this->height() - 20, info);
    }
}

void ImgViewer::paintEvent(QPaintEvent *event) {
    if (!this->img_list.empty()) {
        QPainter painter;
        painter.begin(this);
        draw_img(&painter);
        draw_info(&painter);
        painter.end();
    }
    Q_UNUSED(event);
}

void ImgViewer::mouseMoveEvent(QMouseEvent *event) {
    if (!this->img_list.empty()) {
        QRgb current_color = currentMousePosColor;
        bool current_isMouseInImg = isMouseInImg;
        if (this->scaled_img.rect().contains(event->pos()-this->point)) {
            isMouseInImg = true;
            currentMousePosColor = this->scaled_img.pixel(event->pos()-this->point);
            currentMousePos = event->pos()-this->point;
        } else {
            isMouseInImg = false;
        }
        if( this->left_click) {
            this->endPos = event->pos() - this->startPos;
            this->point = this->point + this->endPos;
            this->startPos = event->pos();
            this->update();
        } else {
            if(current_color != currentMousePosColor || current_isMouseInImg != isMouseInImg) {
                this->update();
            }
        }
    }
    (void)event;
}

void ImgViewer::mousePressEvent(QMouseEvent *event) {
    if (!this->img_list.empty()) {
        if( event->button() == Qt::LeftButton) {
            this->left_click = true;
            this->startPos = event->pos();
        }
    }
}

void ImgViewer::mouseReleaseEvent(QMouseEvent *event) {
    if (!this->img_list.empty()) {
        if( event->button() == Qt::LeftButton) {
            this->left_click = false;
        } else if(event->button() == Qt::RightButton) {
            this->point = QPoint(0, 0);
            this->scaled_img = this->currentImg->scaled(this->size());
            if(this->flipRGB) {
                this->scaled_img = this->scaled_img.rgbSwapped();
            }
            this->repaint();
        } else if(event->button() == Qt::MiddleButton) {
            this->scaled_img = this->currentImg->scaled(this->currentImg->size().width(),this->currentImg->size().height());
            if(this->flipRGB) {
                this->scaled_img = this->scaled_img.rgbSwapped();
            }
            this->point = QPoint(0, 0);
            this->repaint();
        }
    }
}

void ImgViewer::mouseDoubleClickEvent(QMouseEvent *event) {
    if (!this->img_list.empty()) {
        if( event->button() == Qt::LeftButton) {
            int list_index = img_list.indexOf(currentImg_RGB_list);
            QList<QImage*> img_RGB_list = img_list[list_index];
            int img_index = img_RGB_list.indexOf(currentImg);
            QString name = folderpath + "/" + filelist[list_index].replace(".yuv","-").replace(".data","-").replace(".raw","-").replace(".png","-") + QString::number(img_index);
            imgExportWindow->setSaveFileName(name);
            imgExportWindow->setSaveImage(currentImg);
            imgExportWindow->show();
        } else if(event->button() == Qt::RightButton) {
            this->flipRGB = this->flipRGB ? false : true;
            this->point = QPoint(0, 0);
            this->scaled_img = this->currentImg->scaled(this->size());
            if(this->flipRGB) {
                this->scaled_img = this->scaled_img.rgbSwapped();
            }
            this->repaint();
        }
    }
}

void ImgViewer::wheelEvent(QWheelEvent *event) {
    #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    double event_x=event->position().x();
    double event_y=event->position().y();
    #else
    double event_x=event->x();
    double event_y=event->y();
    #endif
    if (!this->img_list.empty()) {
        if( event->angleDelta().y() > 0) {
            // 放大图片
            if( this->scaled_img.width() != 0 && this->scaled_img.height() != 0) {
                float setpsize_x = ((float)this->scaled_img.width())/16.0f;
                float setpsize_y = ((float)this->scaled_img.height())/16.0f; //缩放可能导致比例不精确

                this->scaled_img = this->currentImg->scaled(
                    this->scaled_img.width() + setpsize_x,this->scaled_img.height() + setpsize_y);
                if(this->flipRGB) {
                    this->scaled_img = this->scaled_img.rgbSwapped();
                }
                float new_w = event_x -
                    (this->scaled_img.width() * (event_x - this->point.x())) / (this->scaled_img.width() - setpsize_x);
                float new_h = event_y -
                    (this->scaled_img.height() * (event_y - this->point.y())) / (this->scaled_img.height() - setpsize_y);
                this->point = QPoint(new_w, new_h);
                this->repaint();
            }
        } else if( event->angleDelta().y() < 0) {
            // 缩小图片
            if(this->scaled_img.width() > 25 && this->scaled_img.height() > 25)
            {
                float setpsize_x = ((float)this->scaled_img.width())/16.0f;
                float setpsize_y = ((float)this->scaled_img.height())/16.0f; //缩放可能导致比例不精确

                this->scaled_img = this->currentImg->scaled(
                    this->scaled_img.width() - setpsize_x,this->scaled_img.height() - setpsize_y);
                if(this->flipRGB) {
                    this->scaled_img = this->scaled_img.rgbSwapped();
                }
                float new_w = event_x -
                    (this->scaled_img.width() * (event_x - this->point.x())) / (this->scaled_img.width() + setpsize_x);
                float new_h = event_y -
                    (this->scaled_img.height() * (event_y - this->point.y())) / (this->scaled_img.height() + setpsize_y);
                this->point = QPoint(new_w, new_h);
                this->repaint();
            }
        }
    }
}

void ImgViewer::resizeEvent(QResizeEvent *event) {
    if (!this->img_list.empty()) {
        this->scaled_img = this->currentImg->scaled(this->size());
        if(this->flipRGB) {
            this->scaled_img = this->scaled_img.rgbSwapped();
        }
        this->point = QPoint(0, 0);
        this->update();
    }
    (void)event;
}

void ImgViewer::previousImg() {
    if (!this->img_list.empty()) {
        //得到当前显示的文件序号
        int list_index = this->img_list.indexOf(this->currentImg_RGB_list);
        QList<QImage*> img_RGB_list = this->img_list[list_index];
        //得到当前显示的图像是文件的帧序号
        int img_index = img_RGB_list.indexOf(this->currentImg);

        //判断当前是否是第一帧
        if(img_index == 0) {
            //如果当前是第一帧,则判断当前是否是第一个文件
            if(list_index == 0) {
                //如果是第一个文件则文件序号更新代到最后一个序号
                list_index = this->img_list.count() - 1;
            } else {
                //否则文件序号更新到前一个文件序号
                list_index -= 1;
            }
            //更新帧序号为文件的最后一帧序号
            img_index = this->img_list[list_index].count() - 1;
        } else {
            //否则更新帧序号为前一帧序号,此时文件序号不用更新
            img_index -= 1;
        }

        //序号更新完成,代入序号配置当前显示的页面
        setWindowTitle(this->filelist[list_index] + "-" + QString::number(img_index));
        this->currentImg_RGB_list = this->img_list[list_index];
        this->currentImg = this->currentImg_RGB_list[img_index];
        this->point = QPoint(0, 0);
        this->scaled_img = this->currentImg->scaled(this->size());
        if(this->flipRGB) {
            this->scaled_img = this->scaled_img.rgbSwapped();
        }
        this->repaint();
    }
}

void ImgViewer::nextImg() {
    if (!this->img_list.empty()) {
        //得到当前显示的文件序号
        int list_index = this->img_list.indexOf(this->currentImg_RGB_list);
        QList<QImage*> img_RGB_list = this->img_list[list_index];
        //得到当前显示的图像是文件的帧序号
        int img_index = img_RGB_list.indexOf(this->currentImg);

        //判断当前是否是最后一帧
        if(img_index == img_RGB_list.count() - 1) {
            //如果当前是最后一帧,则判断当前是否是最后一个文件
            if(list_index == this->img_list.count() - 1) {
                //如果是最后一个文件则文件序号更新代到第一个序号
                list_index = 0;
            } else {
                //否则文件序号更新到后一个文件序号
                list_index += 1;
            }
            //更新帧序号为文件的第一帧序号
            img_index = 0;
        } else {
            //否则更新帧序号为后一帧序号,此时文件序号不用更新
            img_index += 1;
        }

        //序号更新完成,代入序号配置当前显示的页面
        setWindowTitle(this->filelist[list_index] + "-" + QString::number(img_index));
        this->currentImg_RGB_list = this->img_list[list_index];
        this->currentImg = this->currentImg_RGB_list[img_index];
        this->point = QPoint(0, 0);
        this->scaled_img = this->currentImg->scaled(this->size());
        if(this->flipRGB) {
            this->scaled_img = this->scaled_img.rgbSwapped();
        }
        this->repaint();
    }
}
