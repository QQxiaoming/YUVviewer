#include <QFileDialog>
#include <QMessageBox>

#include "ImgExport.h"
#include "ImgViewer.h"

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
    if(ui->f0RadioButton->isChecked()) {
        //"png";
        export_png(currentImg,savefilename);
    } else if(ui->f1RadioButton->isChecked()) {
        //"YV12";         
    } else if(ui->f2RadioButton->isChecked()) {
        //"YU12/I420";     
    } else if(ui->f3RadioButton->isChecked()) {
        //"NV21";         
    } else if(ui->f4RadioButton->isChecked()) {
        //"NV12";         
    } else if(ui->f5RadioButton->isChecked()) { 
        //"YUY2/YUYV";    
    } else if(ui->f6RadioButton->isChecked()) { 
        //"YVYU";         
    } else if(ui->f7RadioButton->isChecked()) { 
        //"UYVY";         
    } else if(ui->f8RadioButton->isChecked()) { 
        //"4:4:4";        
    } else if(ui->f9RadioButton->isChecked()) { 
        //"RGB565_L";     
    } else if(ui->f10RadioButton->isChecked()) { 
        //"RGB565_B";     
    } else if(ui->f11RadioButton->isChecked()) { 
        //"BGR565_L";     
    } else if(ui->f12RadioButton->isChecked()) { 
        //"BGR565_B";     
    } else if(ui->f13RadioButton->isChecked()) { 
        //"RGB888";       
    } else if(ui->f14RadioButton->isChecked()) {
        //"BayerBG";
        export_bayer(currentImg,"BGGR",8,savefilename);
    } else if(ui->f15RadioButton->isChecked()) { 
        //"BayerGB";
        export_bayer(currentImg,"GBRG",8,savefilename);
    } else if(ui->f16RadioButton->isChecked()) { 
        //"BayerRG";
        export_bayer(currentImg,"RGGB",8,savefilename);
    } else if(ui->f17RadioButton->isChecked()) { 
        //"BayerGR";
        export_bayer(currentImg,"GRBG",8,savefilename);
    } else if(ui->f18RadioButton->isChecked()) { 
        //"BayerBG_RAW10";
        export_bayer(currentImg,"BGGR",10,savefilename);
    } else if(ui->f19RadioButton->isChecked()) {
        //"BayerGB_RAW10";
        export_bayer(currentImg,"GBRG",10,savefilename);
    } else if(ui->f20RadioButton->isChecked()) {
        //"BayerRG_RAW10";
        export_bayer(currentImg,"RGGB",10,savefilename);
    } else if(ui->f21RadioButton->isChecked()) {
        //"BayerGR_RAW10";
        export_bayer(currentImg,"GRBG",10,savefilename);
    } else if(ui->f22RadioButton->isChecked()) {
        //"BayerBG_RAW12";
        export_bayer(currentImg,"BGGR",12,savefilename);
    } else if(ui->f23RadioButton->isChecked()) {
        //"BayerGB_RAW12";
        export_bayer(currentImg,"GBRG",12,savefilename);
    } else if(ui->f24RadioButton->isChecked()) {
        //"BayerRG_RAW12";
        export_bayer(currentImg,"RGGB",12,savefilename);
    } else if(ui->f25RadioButton->isChecked()) {
        //"BayerGR_RAW12";
        export_bayer(currentImg,"GRBG",12,savefilename);
    } else if(ui->f26RadioButton->isChecked()) {
        //"BayerBG_RAW16";
        export_bayer(currentImg,"BGGR",16,savefilename);
    } else if(ui->f27RadioButton->isChecked()) {
        //"BayerGB_RAW16";
        export_bayer(currentImg,"GBRG",16,savefilename);
    } else if(ui->f28RadioButton->isChecked()) {
        //"BayerRG_RAW16";
        export_bayer(currentImg,"RGGB",16,savefilename);
    } else if(ui->f29RadioButton->isChecked()) {
        //"BayerGR_RAW16";
        export_bayer(currentImg,"GRBG",16,savefilename);
    }


    emit this->accepted();
}

void ImgExport::buttonBoxRejected(void)
{
    emit this->rejected();
}

void ImgExport::export_png(QImage *Img, const QString &name)
{
    QString savefile_name = QFileDialog::getSaveFileName(this, "保存文件", name + ".png", "Image files(*.png)");
    if(savefile_name != nullptr) {
        Img->save(savefile_name);
    }
}

void ImgExport::export_bayer(QImage *Img, const QString &sequence,int bit, const QString &name)
{
    QMessageBox::warning(this, "Warning",
                             "导出此格式将通过模拟抽样的方式，图像信息将会出现损失！",
                             QMessageBox::StandardButtons(QMessageBox::Ok));
    QString savefile_name = QFileDialog::getSaveFileName(this, "保存文件", name + "-bayer" + sequence + QString::number(bit) +".raw", "RAW files(*.raw)");
    if(savefile_name != nullptr) {
        QFile save(savefile_name);
        if (save.open(QIODevice::WriteOnly)) {
            for(int j = 0;j < Img->height();j++) {
                if(j%2 == 0) {
                    for(int i = 0;i < Img->width();i+=4) {
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

                        unsigned char P0 = 0;
                        unsigned char P1 = 0;
                        unsigned char P2 = 0;
                        unsigned char P3 = 0;
                        if(sequence == "BGGR") {
                            P0 = B0;
                            P1 = G1;
                            P2 = B2;
                            P3 = G3;
                        } else if(sequence == "RGGB"){
                            P0 = R0;
                            P1 = G1;
                            P2 = R2;
                            P3 = G3;
                        } else if(sequence == "GBRG"){
                            P0 = G0;
                            P1 = B1;
                            P2 = G2;
                            P3 = B3;
                        } else if(sequence == "GRBG"){
                            P0 = G0;
                            P1 = R1;
                            P2 = G2;
                            P3 = R3;
                        }

                        if(bit == 8) {
                            save.write((const char *)&P0,1);
                            save.write((const char *)&P1,1);
                            save.write((const char *)&P2,1);
                            save.write((const char *)&P3,1);
                        } else if(bit == 10) {
                            unsigned short P0_10 = P0*4;
                            unsigned short P1_10 = P1*4;
                            unsigned short P2_10 = P2*4;
                            unsigned short P3_10 = P3*4;
                            unsigned char piexl[5] = {
                                (unsigned char)(P0_10>>2),
                                (unsigned char)(P1_10>>2),
                                (unsigned char)(P2_10>>2),
                                (unsigned char)(P3_10>>2),
                                (unsigned char)(((P3_10&0x03)<<6)|((P2_10&0x03)<<4)|((P1_10&0x03)<<2)|(P0_10&0x03)) };
                            save.write((const char *)piexl,5);
                        } else if(bit == 12) {
                            unsigned short P0_12 = P0*16;
                            unsigned short P1_12 = P1*16;
                            unsigned short P2_12 = P2*16;
                            unsigned short P3_12 = P3*16;
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
                            unsigned short P0_16 = P0*256;
                            unsigned short P1_16 = P1*256;
                            unsigned short P2_16 = P2*256;
                            unsigned short P3_16 = P3*256;
                            save.write((const char *)&P0_16,2);
                            save.write((const char *)&P1_16,2);
                            save.write((const char *)&P2_16,2);
                            save.write((const char *)&P3_16,2);
                        }
                    }
                } else {
                    for(int i = 0;i < Img->width();i+=4) {
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

                        unsigned char P0 = 0;
                        unsigned char P1 = 0;
                        unsigned char P2 = 0;
                        unsigned char P3 = 0;
                        if(sequence == "BGGR") {
                            P0 = G0;
                            P1 = R1;
                            P2 = G2;
                            P3 = R3;
                        } else if(sequence == "RGGB"){
                            P0 = G0;
                            P1 = B1;
                            P2 = G2;
                            P3 = B3;
                        } else if(sequence == "GBRG"){
                            P0 = R0;
                            P1 = G1;
                            P2 = R2;
                            P3 = G3;
                        } else if(sequence == "GRBG"){
                            P0 = B0;
                            P1 = G1;
                            P2 = B2;
                            P3 = G3;
                        }

                        if(bit == 8) {
                            save.write((const char *)&P0,1);
                            save.write((const char *)&P1,1);
                            save.write((const char *)&P2,1);
                            save.write((const char *)&P3,1);
                        } else if(bit == 10) {
                            unsigned short P0_10 = P0*4;
                            unsigned short P1_10 = P1*4;
                            unsigned short P2_10 = P2*4;
                            unsigned short P3_10 = P3*4;
                            unsigned char piexl[5] = {
                                (unsigned char)(P0_10>>2),
                                (unsigned char)(P1_10>>2),
                                (unsigned char)(P2_10>>2),
                                (unsigned char)(P3_10>>2),
                                (unsigned char)(((P3_10&0x03)<<6)|((P2_10&0x03)<<4)|((P1_10&0x03)<<2)|(P0_10&0x03)) };
                            save.write((const char *)piexl,5);
                        } else if(bit == 12) {
                            unsigned short P0_12 = P0*16;
                            unsigned short P1_12 = P1*16;
                            unsigned short P2_12 = P2*16;
                            unsigned short P3_12 = P3*16;
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
                            unsigned short P0_16 = P0*256;
                            unsigned short P1_16 = P1*256;
                            unsigned short P2_16 = P2*256;
                            unsigned short P3_16 = P3*256;
                            save.write((const char *)&P0_16,2);
                            save.write((const char *)&P1_16,2);
                            save.write((const char *)&P2_16,2);
                            save.write((const char *)&P3_16,2);
                        }
                    }
                }
            }
            save.close();
        }
    }
}
