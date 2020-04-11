#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QToolTip>
#include <QCloseEvent>
#include <QPoint>
#include <QDebug>
#include <QString>
#include "YUVviewer.h"
#include "ui_UI_YUVviewer.h"

const QString VERSION = "V0.3.4";

YUVviewer::YUVviewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::YUVviewer)
{
    int currentIndex;

    ui->setupUi(this);

    this->setWindowTitle("YUVviewer " + VERSION + " by liwq");
    QRect screen = QApplication::desktop()->screenGeometry();
    QRect size = this->geometry();
    this->move((screen.width() - size.width()) / 2, (screen.height() - size.height()) / 2);

    YUVviewerConfigFile = new ConfigFile(QCoreApplication::applicationDirPath()+"/YUVViewer.ini");

    if(YUVviewerConfigFile->config_dict.frameSizeType == "Other")
    {
        ui->frameSizeType_Other_RadioButton->setChecked(true);
        ui->frameSize_Width_LineEdit->setText(YUVviewerConfigFile->config_dict.frameSize_Width);
        ui->frameSize_Height_LineEdit->setText(YUVviewerConfigFile->config_dict.frameSize_Height);
    }
    else if (YUVviewerConfigFile->config_dict.frameSizeType == "CIF")
    {
        ui->frameSizeType_CIF_RadioButton->setChecked(true);
        ui->frameSize_Width_LineEdit->setText("352");
        ui->frameSize_Width_LineEdit->setFocusPolicy(Qt::NoFocus);
        YUVviewerConfigFile->config_dict.frameSize_Width = "352";
        ui->frameSize_Height_LineEdit->setText("288");
        ui->frameSize_Height_LineEdit->setFocusPolicy(Qt::NoFocus);
        YUVviewerConfigFile->config_dict.frameSize_Height = "288";
    }
    else
    {
        ui->frameSizeType_CIF_RadioButton->setChecked(true);
        ui->frameSize_Width_LineEdit->setText("176");
        ui->frameSize_Width_LineEdit->setFocusPolicy(Qt::NoFocus);
        YUVviewerConfigFile->config_dict.frameSize_Width = "176";
        ui->frameSize_Height_LineEdit->setText("144");
        ui->frameSize_Height_LineEdit->setFocusPolicy(Qt::NoFocus);
        YUVviewerConfigFile->config_dict.frameSize_Height = "144";
    }

    QStringList YUVFormat_list = {"YV12", "YU12/I420", "NV21", "NV12", "YUY2/YUYV", "YVYU", "UYVY", "4:4:4"};
    currentIndex = 0;
    foreach(QString s ,YUVFormat_list)
    {
        if(s == YUVviewerConfigFile->config_dict.YUVFormat)
        {
            ui->YUVFormat_ComboBox->setCurrentIndex(currentIndex);
        }
        currentIndex++;
    }

    QStringList frameRate_list = {"30", "60", "120"};
    currentIndex = 0;
    foreach(QString s ,frameRate_list)
    {
        if(s == YUVviewerConfigFile->config_dict.frameRate)
        {
            ui->frameRate_ComboBox->setCurrentIndex(currentIndex);
        }
        currentIndex++;
    }

    ui->startFrame_LineEdit->setText(YUVviewerConfigFile->config_dict.startFrame);
    ui->endFrame_LineEdit->setText(YUVviewerConfigFile->config_dict.endFrame);

    QObject::connect(ui->frameSizeType_CIF_RadioButton, SIGNAL(clicked()), this, SLOT(configCIF()));
    QObject::connect(ui->frameSizeType_QCIF_RadioButton, SIGNAL(clicked()), this, SLOT(configQCIF()));
    QObject::connect(ui->frameSizeType_Other_RadioButton, SIGNAL(clicked()), this, SLOT(configOther()));

    QObject::connect(ui->frameSize_Height_LineEdit, SIGNAL(textEdited(const QString &)), this, SLOT(frameSizeHeightValidator(const QString &)));
    QObject::connect(ui->frameSize_Width_LineEdit, SIGNAL(textEdited(const QString &)), this, SLOT(frameSizeWidthValidator(const QString &)));
    QObject::connect(ui->startFrame_LineEdit, SIGNAL(textEdited(const QString &)), this, SLOT(startFrameValidator(const QString &)));
    QObject::connect(ui->endFrame_LineEdit, SIGNAL(textEdited(const QString &)), this, SLOT(endFrameValidator(const QString &)));

    QObject::connect(ui->openFile_PushButton, SIGNAL(clicked()), this, SLOT(openFile()));
    QObject::connect(ui->openFolder_PushButton, SIGNAL(clicked()), this, SLOT(openFolder()));
    QObject::connect(ui->about_PushButton, SIGNAL(clicked()), this, SLOT(about()));
    QObject::connect(ui->help_PushButton, SIGNAL(clicked()), this, SLOT(help()));
    imgViewer = nullptr;
}

YUVviewer::~YUVviewer()
{
    if(imgViewer != nullptr)
    {
        delete imgViewer;
    }
    delete ui;
}

void YUVviewer::configCIF()
{
    ui->frameSize_Width_LineEdit->setText("352");
    ui->frameSize_Width_LineEdit->setFocusPolicy(Qt::NoFocus);
    ui->frameSize_Height_LineEdit->setText("288");
    ui->frameSize_Height_LineEdit->setFocusPolicy(Qt::NoFocus);
}

void YUVviewer::configQCIF()
{
    ui->frameSize_Width_LineEdit->setText("176");
    ui->frameSize_Width_LineEdit->setFocusPolicy(Qt::NoFocus);
    ui->frameSize_Height_LineEdit->setText("144");
    ui->frameSize_Height_LineEdit->setFocusPolicy(Qt::NoFocus);
}

void YUVviewer::configOther()
{
    ui->frameSize_Width_LineEdit->setText(YUVviewerConfigFile->config_dict.frameSize_Width);
    ui->frameSize_Height_LineEdit->setText(YUVviewerConfigFile->config_dict.frameSize_Height);
    ui->frameSize_Width_LineEdit->setFocusPolicy(Qt::StrongFocus);
    ui->frameSize_Height_LineEdit->setFocusPolicy(Qt::StrongFocus);
}

void YUVviewer::frameSizeHeightValidator(const QString &currentText)
{
    bool isInt;
    int currentVale = currentText.toInt(&isInt);
    if(isInt == true)
    {
        if(((currentVale%2) == 0) && (currentVale > 0))
        {
            ui->frameSize_Height_LineEdit->setStyleSheet("QLineEdit{border:1px solid gray border-radius:1px}");
        }
        else
        {
            QToolTip::showText(ui->frameSize_Height_LineEdit->mapToGlobal(QPoint(0, 10)), "Height must be positive even");
            ui->frameSize_Height_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
        }
    }
    else
    {
        QToolTip::showText(ui->frameSize_Height_LineEdit->mapToGlobal(QPoint(0, 10)), "Height must be num");
        ui->frameSize_Height_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
    }
}

void YUVviewer::frameSizeWidthValidator(const QString &currentText)
{
    bool isInt;
    int currentVale = currentText.toInt(&isInt);
    if(isInt == true)
    {
        if(((currentVale%2) == 0) && (currentVale > 0))
        {
            ui->frameSize_Width_LineEdit->setStyleSheet("QLineEdit{border:1px solid gray border-radius:1px}");
        }
        else
        {
            QToolTip::showText(ui->frameSize_Width_LineEdit->mapToGlobal(QPoint(0, 10)), "Width must be positive even");
            ui->frameSize_Width_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
        }
    }
    else
    {
        QToolTip::showText(ui->frameSize_Width_LineEdit->mapToGlobal(QPoint(0, 10)), "Width must be num");
        ui->frameSize_Width_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
    }
}

void YUVviewer::startFrameValidator(const QString & currentText)
{
    bool isInt;
    int currentVale = currentText.toInt(&isInt);
    if(isInt != true)
    {
        QToolTip::showText(ui->startFrame_LineEdit->mapToGlobal(QPoint(0, 10)), "startFrame must be num");
        ui->startFrame_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
        return;
    }
    if(currentVale >= 0)
    {
        ui->startFrame_LineEdit->setStyleSheet("QLineEdit{border:1px solid gray border-radius:1px}");
        int endFrameVale = ui->endFrame_LineEdit->text().toInt(&isInt);
        if(isInt != true)
        {
            QToolTip::showText(ui->endFrame_LineEdit->mapToGlobal(QPoint(0, 10)), "endFrame must be num");
            ui->endFrame_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
            return;
        }
        if(currentVale <= endFrameVale)
        {
            ui->endFrame_LineEdit->setStyleSheet("QLineEdit{border:1px solid gray border-radius:1px}");
        }
        else
        {
            QToolTip::showText(ui->endFrame_LineEdit->mapToGlobal(QPoint(0, 10)), "endFrame must be greater than or equal to startFrame");
            ui->endFrame_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
        }
    }
    else
    {
        QToolTip::showText(ui->startFrame_LineEdit->mapToGlobal(QPoint(0, 10)), "startFrame must be greater than or equal to 0");
        ui->startFrame_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
    }
}

void YUVviewer::endFrameValidator(const QString & currentText)
{
    bool isInt;
    int currentVale = currentText.toInt(&isInt);
    if(isInt != true)
    {
        QToolTip::showText(ui->endFrame_LineEdit->mapToGlobal(QPoint(0, 10)), "endFrame must be num");
        ui->endFrame_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
        return;
    }

    ui->endFrame_LineEdit->setStyleSheet("QLineEdit{border:1px solid gray border-radius:1px}");

    int startVale = ui->startFrame_LineEdit->text().toInt(&isInt);
    if(isInt == true)
    {
        if(currentVale < startVale)
        {
            QToolTip::showText(ui->endFrame_LineEdit->mapToGlobal(QPoint(0, 10)), "endFrame must be greater than or equal to startFrame");
            ui->endFrame_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
        }
    }
    else
    {
        QToolTip::showText(ui->startFrame_LineEdit->mapToGlobal(QPoint(0, 10)), "startFrame must be num");
        ui->startFrame_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
    }
}

void YUVviewer::showParaErrMessageBox(void)
{
    QMessageBox::critical(this, "Error", "parameter invalid!!", QMessageBox::Ok);
}

bool YUVviewer::updateConfig(void)
{
    bool isInt;
    int startFrame = ui->startFrame_LineEdit->text().toInt(&isInt);
    if(!isInt)
    {
        showParaErrMessageBox();
        return false;
    }
    int endFrame = ui->endFrame_LineEdit->text().toInt(&isInt);
    if(!isInt)
    {
        showParaErrMessageBox();
        return false;
    }
    int temp_Width = ui->frameSize_Width_LineEdit->text().toInt(&isInt);
    if(!isInt)
    {
        showParaErrMessageBox();
        return false;
    }
    int temp_Height = ui->frameSize_Height_LineEdit->text().toInt(&isInt);
    if(!isInt)
    {
        showParaErrMessageBox();
        return false;
    }

    if(startFrame <= endFrame)
    {
        if(((temp_Width % 2) == 0) && ((temp_Height % 2) == 0) && (temp_Width > 0) && (temp_Height > 0))
        {
            if(ui->frameSizeType_CIF_RadioButton->isChecked())
            {
                YUVviewerConfigFile->config_dict.frameSizeType = ui->frameSizeType_CIF_RadioButton->text();

            }
            else if(ui->frameSizeType_QCIF_RadioButton->isChecked())
            {
                YUVviewerConfigFile->config_dict.frameSizeType = ui->frameSizeType_QCIF_RadioButton->text();

            }
            else if (ui->frameSizeType_Other_RadioButton->isChecked())
            {
                YUVviewerConfigFile->config_dict.frameSizeType = ui->frameSizeType_Other_RadioButton->text();
            }
            YUVviewerConfigFile->config_dict.YUVFormat = ui->YUVFormat_ComboBox->currentText();
            YUVviewerConfigFile->config_dict.frameSize_Width = ui->frameSize_Width_LineEdit->text();
            YUVviewerConfigFile->config_dict.frameSize_Height = ui->frameSize_Height_LineEdit->text();
            YUVviewerConfigFile->config_dict.frameRate = ui->frameRate_ComboBox->currentText();
            YUVviewerConfigFile->config_dict.startFrame = ui->startFrame_LineEdit->text();
            YUVviewerConfigFile->config_dict.endFrame = ui->endFrame_LineEdit->text();

            return true;
        }
        else
        {
            QMessageBox::critical(this, "Error", "frameSize invalid!!", QMessageBox::Ok);
            return false;
        }
    }
    else
    {
        QMessageBox::critical(this, "Error", "startFrame or endFrame invalid!!", QMessageBox::Ok);
        return false;
    }
}

bool YUVviewer::imgView(QStringList openfile_list)
{
    if (openfile_list.empty())
    {
        return false;
    }
    if(imgViewer != nullptr)
    {
        delete imgViewer;
        imgViewer = nullptr;
    }
    imgViewer = new ImgViewer(nullptr,this);
    int startFrame = ui->startFrame_LineEdit->text().toInt();
    int endFrame = ui->endFrame_LineEdit->text().toInt();
    int frameSize_Width = ui->frameSize_Width_LineEdit->text().toInt();
    int frameSize_Height = ui->frameSize_Height_LineEdit->text().toInt();
    #if 1
    // 多线程
    bool isSuccess = imgViewer->setFileList_multithreading(openfile_list,
                           YUVviewerConfigFile->config_dict.YUVFormat,
                           frameSize_Width,
                           frameSize_Height,
                           startFrame,
                           endFrame-startFrame+1
                           );
    #else
    // 单线程
    bool isSuccess = imgViewer->setFileList(openfile_list,
                           YUVviewerConfigFile->config_dict.YUVFormat,
                           frameSize_Width,
                           frameSize_Height,
                           startFrame,
                           endFrame-startFrame+1
                           );
    #endif
    if(!isSuccess)
    {
        QMessageBox::critical(this, "Error", "unsupport YUVFormat!!", QMessageBox::Ok);
        this->show();
        return false;
    }
    float fframeSize_Width = (float)frameSize_Width;
    float fframeSize_Height = (float)frameSize_Height;
    if(frameSize_Width > frameSize_Height)
    {
        imgViewer->resize(800, fframeSize_Height/fframeSize_Width*800.0);
    }
    else
    {
        imgViewer->resize(fframeSize_Width/fframeSize_Height*400.0, 400);
    }
    QRect screen = QApplication::desktop()->screenGeometry();
    QRect size = imgViewer->geometry();
    imgViewer->move((screen.width() - size.width()) / 2, (screen.height() - size.height()) / 2);
    this->hide();
    imgViewer->show();

    return true;
}

void YUVviewer::openFile()
{
    if(updateConfig())
    {
        QStringList openfile_list = QFileDialog::getOpenFileNames(this, "选择文件", "", "YUV files(*.yuv)");
        if(openfile_list.size() != 0)
        {
            imgView(openfile_list);
        }
    }
}

void YUVviewer::openFolder()
{
    if(updateConfig())
    {
        QString openfolder_name = QFileDialog::getExistingDirectory(this, "选择文件夹");
        if (!openfolder_name.isEmpty())
        {
            QDir dir(openfolder_name);
            QStringList nameFilters = {"*.yuv"};
            QStringList openfilename_list = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
            QStringList openfile_list;
            foreach (QString file_name, openfilename_list)
            {
                openfile_list.append(QDir::toNativeSeparators(openfolder_name + '/' +file_name));
            }
            if(openfile_list.size() != 0)
            {
                imgView(openfile_list);
            }
        }
    }
}

void YUVviewer::about()
{
    QMessageBox::about(this, "About", "版本 " + VERSION + " 作者:liwq");
}

void YUVviewer::help()
{
    QMessageBox::question(this, "Help", "目前暂无帮助说明", QMessageBox::Ok);
}

void YUVviewer::closeEvent(QCloseEvent *event)
{
    delete YUVviewerConfigFile;
    event->accept();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    YUVviewer w;
    w.show();

    return a.exec();
}
