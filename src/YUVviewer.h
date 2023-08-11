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
    struct UICodePoint
    {
        unsigned int num;
        uint32_t color[4];
        uint32_t bit[4];
    };
    QString svgBoxSrc(int x, int y, int w, UICodePoint c);
    QString svgBoxArraySrc(int x, int y, int w, int od, int xn, int yn,QList<UICodePoint> ca);
    QString getLegend(const QString &type);
    void updateUiSvg(QList<UICodePoint> color_list);
    void showParaErrMessageBox(void);
    bool updateConfig(void);
    bool imgView(QStringList openfile_list, const QString &folderpath);
    Ui::YUVviewer *ui;
    ConfigFile *YUVviewerConfigFile;
    ImgViewer *imgViewer;
    static const QList<QPair<QString, QStringList>> frameSizeTypeDict;
    static const QList<QPair<QString, QList<UICodePoint>>> YUVFormat_pattern;
};

#endif // YUVVIEWER_H
