#include <QImage>
#include <QFileInfo>
#include <QList>
#include <QString>
#include <QPoint>
#include <QDebug>
#include "ImgViewer.h"
#include "YUVdecoder.h"
#include "ui_UI_ImgViewer.h"


YUVDecodeThread::YUVDecodeThread(QWidget *parent,QString yuvfilename,QString YUVFormat,
                                 int W, int H, int startframe, int totalframe) :
    QThread(parent)
{
    this->window = parent;
    this->yuvfilename = yuvfilename;
    this->W = W;
    this->H = H;
    this->startframe = startframe;
    this->totalframe = totalframe;
    this->decoder = YUV2RGB::yuvdecoder_map.find(YUVFormat).value();
}

void YUVDecodeThread::run()
{
    QList<QImage*> img_RGB_list;
    if(this->decoder == nullptr)
    {
        emit finsh_signal(img_RGB_list,nullptr);
    }
    else
    {
        QList<cv::Mat*> frame_RGB_list = this->decoder(this->yuvfilename, this->W, this->H, this->startframe, this->totalframe);
        foreach( cv::Mat* img,frame_RGB_list)
        {
            QImage *qImg = new QImage((const unsigned char*)(img->data), img->cols, img->rows, img->step, QImage::Format_RGB888);
            qImg->rgbSwapped();
            img_RGB_list.insert(img_RGB_list.end(), qImg);
            delete img;
        }

        emit finsh_signal(img_RGB_list,this->yuvfilename);
    }
}


ImgViewer::ImgViewer(QWidget *parent,QWidget *parentWindow) :
    QWidget(parent),
    ui(new Ui::ImgViewerWindow)
{
    ui->setupUi(this);
    this->parentWindow = parentWindow;
    setWindowTitle("loading file, please wait ....");
    ui->left_PushButton->setFlat(true);
    ui->right_PushButton->setFlat(true);
    //QObject::connect(ui->left_PushButton, SIGNAL(clicked()), this, SLOT(previousImg()));
    //QObject::connect(ui->right_PushButton, SIGNAL(clicked()), this, SLOT(nextImg()));
    left_click = false;
}

ImgViewer::~ImgViewer()
{
    delete ui;
}

bool ImgViewer::setFileList(QStringList filenamelist,QString YUVFormat, int W, int H, int startframe, int totalframe)
{
    yuvdecoder_t decoder = YUV2RGB::yuvdecoder_map.find(YUVFormat).value();
    if(decoder == nullptr)
    {
        return false;
    }
    else
    {
        ui->imgViewer->setText("");
        foreach( QString filename, filenamelist)
        {
            QList<cv::Mat*> frame_RGB_list = decoder(filename, W, H, startframe, totalframe);
            QList<QImage*> img_RGB_list;
            foreach (cv::Mat* img, frame_RGB_list)
            {
                QImage *qImg = new QImage((const unsigned char*)(img->data), img->cols, img->rows, img->step, QImage::Format_RGB888);
                qImg->rgbSwapped();
                img_RGB_list.insert(img_RGB_list.end(), qImg);
                delete img;
            }

            this->img_list.insert(img_list.end(),img_RGB_list);
            QFileInfo fileInfo(filename);
            this->filelist.insert(filelist.end(),fileInfo.fileName());
        }
        this->currentImg_RGB_list = this->img_list.at(0);
        this->currentImg = this->currentImg_RGB_list.at(0);
        this->setWindowTitle(this->filelist.at(0)+"-0");
        this->scaled_img = this->currentImg->scaled(this->size());
        this->point = QPoint(0, 0);
        return true;
    }
}

void ImgViewer::closeEvent(QCloseEvent *event)
{
    this->parentWindow->show();
    event->accept();
}

void ImgViewer::resizeEvent(QResizeEvent *event)
{
    if (!this->img_list.empty())
    {
        this->scaled_img = this->currentImg->scaled(this->size());
        this->point = QPoint(0, 0);
        this->update();
    }
}

