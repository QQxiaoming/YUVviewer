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
#ifndef BOOTSELECTBOX_H
#define BOOTSELECTBOX_H

#include <QDialog>
#include <QImage>

namespace Ui {
class ImgExportWindow;
}

class ImgExport : public QDialog
{
    Q_OBJECT

public:
    explicit ImgExport(QWidget *parent = nullptr);
    ~ImgExport();
    void setSaveFileName(const QString &name);
    void setSaveImage(QImage *image);

private:
    void export_png(QImage *Img, const QString &name);
    void export_yuv(QImage *Img, const QString &sequence, const QString &name);
    void export_rgb(QImage *Img, const QString &sequence, const QString &name);
    void export_bayer(QImage *Img, const QString &sequence, int bit, const QString &fileName);

protected:
    void showEvent(QShowEvent* event) override;

private slots:
    void buttonBoxAccepted(void);
    void buttonBoxRejected(void);

private:
    Ui::ImgExportWindow *ui;
    QString savefilename;
    QImage *currentImg;
};

#endif // BOOTSELECTBOX_H
