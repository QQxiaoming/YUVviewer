/**
 * @file YUVviewer.h
 * @brief 主界面
 * @version 1.0
 * @date 2020-04-14
 */
#ifndef YUVVIEWER_H
#define YUVVIEWER_H

#include <QMainWindow>
#include "ImgViewer.h"
#include "configFile.h"

namespace Ui {
class YUVviewer;
}

class YUVviewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit YUVviewer(QWidget *parent = nullptr);
    ~YUVviewer();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void configCIF();
    void configQCIF();
    void configOther();
    void frameSizeHeightValidator(const QString &currentText);
    void frameSizeWidthValidator(const QString &currentText);
    void startFrameValidator(const QString &currentText);
    void endFrameValidator(const QString &currentText);
    void openFile();
    void openFolder();
    void about();
    void help();

private:
    void showParaErrMessageBox(void);
    bool updateConfig(void);
    bool imgView(QStringList openfile_list);
    Ui::YUVviewer *ui;
    ConfigFile *YUVviewerConfigFile;
    ImgViewer *imgViewer;
};

#endif // YUVVIEWER_H
