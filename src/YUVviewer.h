/**
 * @file YUVviewer.h
 * @brief 主界面
 * @version 1.0
 * @date 2020-04-14
 */
#ifndef YUVVIEWER_H
#define YUVVIEWER_H

#include <QMainWindow>
#include <QCloseEvent>
#include "ImgViewer.h"
#include "configFile.h"

namespace Ui {
class YUVviewer;
}

class YUVviewer : public QMainWindow {
    Q_OBJECT

public:
    explicit YUVviewer(QWidget *parent = nullptr);
    ~YUVviewer();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void changeFormat(const QString &text);
    void configComboBox();
    void configOther();
    void changeFrameSizeType(const QString &text);
    void frameSizeHeightValidator(const QString &currentText);
    void frameSizeWidthValidator(const QString &currentText);
    void startFrameValidator(const QString &currentText);
    void endFrameValidator(const QString &currentText);
    void exchaneSize();
    void openFile();
    void openFolder();
    void help();
    void about();
    void aboutQt();

private:
    QString svgBoxSrc(int x, int y, int w, uint64_t c);
    QString svgBoxArraySrc(int x, int y, int w, int od, int xn, int yn,QList<uint64_t> ca);
    void updateUiSvg(QList<uint64_t> color_list);
    void showParaErrMessageBox(void);
    bool updateConfig(void);
    bool imgView(QStringList openfile_list);
    Ui::YUVviewer *ui;
    ConfigFile *YUVviewerConfigFile;
    ImgViewer *imgViewer;
    static const QList<QPair<QString, QStringList>> frameSizeTypeDict;
    static const QList<QPair<QString, QList<uint64_t>>> YUVFormat_list;
};

#endif // YUVVIEWER_H
