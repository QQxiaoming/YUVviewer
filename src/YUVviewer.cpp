/**
 * @file YUVviewer.cpp
 * @brief 主界面
 * @version 1.0
 * @date 2020-04-14
 */
#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QToolTip>
#include <QCloseEvent>
#include <QPoint>
#include <QScreen>
#include <QDebug>
#include <QString>
#include "YUVviewer.h"
#include "ui_UI_YUVviewer.h"

const QString VERSION = APP_VERSION;

const QList<QPair<QString, QStringList>> YUVviewer::frameSizeTypeDict = {
    {"QQCIF",   {"88","72"}},
    {"QQVGA",   {"160","120"}},
    {"QCIF",    {"176","144"}},
    {"HQVGA",   {"240","160"}},
    {"QVGA",    {"320","240"}},
    {"CIF",     {"352","288"}},
    {"WQVGA",   {"400","240"}},
    {"HVGA",    {"480","320"}},
    {"nHD",     {"640","340"}},
    {"VGA",     {"640","480"}},
    {"WVGA",    {"800","480"}},
    {"SVGA",    {"800","600"}},
    {"qHD",     {"960","540"}},
    {"DVGA",    {"960","640"}},
    {"XGA",     {"1024","768"}},
    {"HD",      {"1280","720"}},
    {"UVGA",    {"1280","960"}},
    {"SXGA",    {"1280","1024"}},
    {"HD+",     {"1600","900"}},
    {"UXGA",    {"1600","1200"}},
    {"FHD",     {"1920","1080"}},
    {"WUXGA",   {"1920","1200"}},
    {"FHD+",    {"2160","1440"}},
    {"QXGA",    {"2048","1536"}},
    {"QHD",     {"2560","1440"}},
    {"WQXGA",   {"2560","1600"}},
    {"QSXGA",   {"2560","2048"}},
    {"QHD+",    {"3200","1800"}},
    {"QUXGA",   {"3200","2400"}},
    {"4K UHD",  {"3840","2160"}},
    {"8K UHD",  {"7680","4320"}},
};


YUVviewer::YUVviewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::YUVviewer)
{
    int currentIndex;

    ui->setupUi(this);

    this->setWindowTitle("YUVviewer " + VERSION);
    QRect screen = QGuiApplication::primaryScreen()->geometry();
    QRect size = this->geometry();
    this->move((screen.width() - size.width()) / 2, (screen.height() - size.height()) / 2);

    ui->frameSizeType_ComboBox->setStyleSheet("combobox-popup: 0;");
    ui->YUVFormat_ComboBox->setStyleSheet("combobox-popup: 0;");
    ui->frameRate_ComboBox->setStyleSheet("combobox-popup: 0;");

    QList<QPair<QString, QStringList>>::const_iterator config_it = frameSizeTypeDict.begin();
    while (config_it != frameSizeTypeDict.end()) {
        ui->frameSizeType_ComboBox->insertItem(ui->frameSizeType_ComboBox->count(),config_it->first);
        config_it++;
    }

    YUVviewerConfigFile = new ConfigFile(QCoreApplication::applicationDirPath()+"/YUVViewer.ini");
    if(YUVviewerConfigFile->config_dict.frameSizeType == "Other")
    {
        ui->frameSizeType_Other_RadioButton->setChecked(true);
        ui->frameSizeType_ComboBox->setEnabled(false);
        ui->frameSize_Width_LineEdit->setText(YUVviewerConfigFile->config_dict.frameSize_Width);
        ui->frameSize_Height_LineEdit->setText(YUVviewerConfigFile->config_dict.frameSize_Height);
    }
    else
    {
        ui->frameSizeType_Combo_RadioButton->setChecked(true);
        ui->frameSizeType_ComboBox->setEnabled(true);
        QList<QPair<QString, QStringList>>::const_iterator config_it = frameSizeTypeDict.begin();
        while (config_it != frameSizeTypeDict.end()) {
            if(config_it->first == YUVviewerConfigFile->config_dict.frameSizeType)
            {
                QStringList value = config_it->second;
                ui->frameSizeType_ComboBox->setCurrentText(YUVviewerConfigFile->config_dict.frameSizeType);
                ui->frameSize_Width_LineEdit->setText(value[0]);
                ui->frameSize_Width_LineEdit->setFocusPolicy(Qt::NoFocus);
                YUVviewerConfigFile->config_dict.frameSize_Width = value[0];
                ui->frameSize_Height_LineEdit->setText(value[1]);
                ui->frameSize_Height_LineEdit->setFocusPolicy(Qt::NoFocus);
                YUVviewerConfigFile->config_dict.frameSize_Height = value[1];
                break;
            }
            config_it++;
        }
    }

    QStringList YUVFormat_list = {"YV12", "YU12/I420", "NV21", "NV12", "YUY2/YUYV", "YVYU", "UYVY", "4:4:4","RGB565_L","RGB565_B","BGR565_L","BGR565_B","RGB888"};
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


    QObject::connect(ui->frameSizeType_Combo_RadioButton, SIGNAL(clicked()), this, SLOT(configComboBox()));
    QObject::connect(ui->frameSizeType_Other_RadioButton, SIGNAL(clicked()), this, SLOT(configOther()));
    QObject::connect(ui->frameSizeType_ComboBox, SIGNAL(currentTextChanged(const QString &)), this,SLOT(changeFrameSizeType(const QString &)));

    QObject::connect(ui->frameSize_Height_LineEdit, SIGNAL(textEdited(const QString &)), this, SLOT(frameSizeHeightValidator(const QString &)));
    QObject::connect(ui->frameSize_Width_LineEdit, SIGNAL(textEdited(const QString &)), this, SLOT(frameSizeWidthValidator(const QString &)));
    QObject::connect(ui->startFrame_LineEdit, SIGNAL(textEdited(const QString &)), this, SLOT(startFrameValidator(const QString &)));
    QObject::connect(ui->endFrame_LineEdit, SIGNAL(textEdited(const QString &)), this, SLOT(endFrameValidator(const QString &)));

    QObject::connect(ui->exchange_PushButton, SIGNAL(clicked()), this, SLOT(exchaneSize()));
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
    delete YUVviewerConfigFile;
    delete ui;
}

void YUVviewer::configComboBox()
{
    ui->frameSizeType_ComboBox->setEnabled(true);
    QList<QPair<QString, QStringList>>::const_iterator config_it = frameSizeTypeDict.begin();
    while (config_it != frameSizeTypeDict.end()) {
        if(config_it->first == ui->frameSizeType_ComboBox->currentText())
        {
            QStringList value = config_it->second;
            ui->frameSize_Width_LineEdit->setText(value[0]);
            ui->frameSize_Width_LineEdit->setFocusPolicy(Qt::NoFocus);
            ui->frameSize_Height_LineEdit->setText(value[1]);
            ui->frameSize_Height_LineEdit->setFocusPolicy(Qt::NoFocus);
            break;
        }
        config_it++;
    }
}

void YUVviewer::changeFrameSizeType(const QString &text)
{
    QList<QPair<QString, QStringList>>::const_iterator config_it = frameSizeTypeDict.begin();
    while (config_it != frameSizeTypeDict.end()) {
        if(config_it->first == text)
        {
            QStringList value = config_it->second;
            ui->frameSize_Width_LineEdit->setText(value[0]);
            ui->frameSize_Width_LineEdit->setFocusPolicy(Qt::NoFocus);
            ui->frameSize_Height_LineEdit->setText(value[1]);
            ui->frameSize_Height_LineEdit->setFocusPolicy(Qt::NoFocus);
            break;
        }
        config_it++;
    }
}

void YUVviewer::configOther()
{
    ui->frameSizeType_ComboBox->setEnabled(false);
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

void YUVviewer::exchaneSize() 
{ 
    ui->frameSizeType_Other_RadioButton->setChecked(true);
    ui->frameSizeType_ComboBox->setEnabled(false);
    QString width = ui->frameSize_Width_LineEdit->text();
    ui->frameSize_Width_LineEdit->setText(ui->frameSize_Height_LineEdit->text());
    ui->frameSize_Height_LineEdit->setText(width);
    frameSizeWidthValidator(ui->frameSize_Width_LineEdit->text());
    frameSizeHeightValidator(ui->frameSize_Height_LineEdit->text());
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
            if(ui->frameSizeType_Combo_RadioButton->isChecked())
            {
                YUVviewerConfigFile->config_dict.frameSizeType = ui->frameSizeType_ComboBox->currentText();
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
    QRect screen = QGuiApplication::primaryScreen()->geometry();
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
        QString openDir = "";
        QFileInfo lastPath(YUVviewerConfigFile->config_dict.lastPath);
        if(lastPath.isDir())
        {
            openDir = YUVviewerConfigFile->config_dict.lastPath;
        }
        QStringList openfile_list = QFileDialog::getOpenFileNames(this, "选择文件", openDir, "YUV files(*.yuv *.data)");
        if(openfile_list.size() != 0)
        {
            QFileInfo file(openfile_list[0]);
            YUVviewerConfigFile->config_dict.lastPath = file.absolutePath();
            imgView(openfile_list);
        }
    }
}

void YUVviewer::openFolder()
{
    if(updateConfig())
    {
        QString openDir = "";
        QFileInfo lastPath(YUVviewerConfigFile->config_dict.lastPath);
        if(lastPath.isDir())
        {
            openDir = YUVviewerConfigFile->config_dict.lastPath;
        }
        QString openfolder_name = QFileDialog::getExistingDirectory(this, "选择文件夹", openDir);
        if (!openfolder_name.isEmpty())
        {
            YUVviewerConfigFile->config_dict.lastPath = openfolder_name;
            QDir dir(openfolder_name);
            QStringList nameFilters = {"*.yuv","*.data"};
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
    QMessageBox::about(this, "About", "版本 \n " + VERSION + "\n作者\n wenqing.li@aliyun.com \n qiaoqm@aliyun.com");
}

void YUVviewer::help()
{
    QMessageBox::question(this, "Help", "1.主界面选择数据参数。\n2.点击打开文件或文件夹将进行图像数据解析并显示图像。\n3.图像显示界面中使用滚轮放大缩小图像，使用左键可拖动图像，双击左键保存图像为png格式，单击右键复位图像大小和位置，双击右键交换图像R和B通道显示。", QMessageBox::Ok);
}

void YUVviewer::closeEvent(QCloseEvent *event)
{
    delete YUVviewerConfigFile;
    event->accept();
}

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication application(argc, argv);
    QFont font = application.font();
    font.setFamily(font.defaultFamily());
    font.setPixelSize(13);
    application.setFont(font);
    YUVviewer window;
    window.show();

    return application.exec();
}
