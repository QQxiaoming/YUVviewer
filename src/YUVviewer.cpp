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
#include <iostream>
#include <QApplication>
#include <QScreen>
#include <QMessageBox>
#include <QToolTip>
#include <QPoint>
#include <QDebug>
#include <QString>
#include <QSvgRenderer>
#include <QXmlStreamReader>
#include <QTranslator>
#include <QLibraryInfo>
#include "filedialog.h"
#include "YUVviewer.h"
#include "ui_UI_YUVviewer.h"

#include "build_info.inc"

const QString VERSION = APP_VERSION;
const QString GIT_TAG = BUILD_INFO;
const QString DATE_TAG = BUILD_DATE;
const QString HASH_TAG = BUILD_HASH;
const QString SHORT_HASH_TAG = BUILD_SHORT_HASH;

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

#define UI_A        { .num = 0, .color = {0,0,0,0}, .bit = {0,0,0,0} }
#define UI_R        { .num = 1, .color = {0xffff0000UL,0,0,0}, .bit = {1,0,0,0} }
#define UI_G        { .num = 1, .color = {0xff00ff00UL,0,0,0}, .bit = {1,0,0,0} }
#define UI_B        { .num = 1, .color = {0xff0000ffUL,0,0,0}, .bit = {1,0,0,0} }
#define UI_Y        { .num = 1, .color = {0xff808080UL,0,0,0}, .bit = {1,0,0,0} }
#define UI_U        { .num = 1, .color = {0xffff00ffUL,0,0,0}, .bit = {1,0,0,0} }
#define UI_V        { .num = 1, .color = {0xff00ffffUL,0,0,0}, .bit = {1,0,0,0} }
#define UI_RG53     { .num = 2, .color = {0xffff0000UL,0xff00ff00UL,0,0}, .bit = {5,3,0,0} }
#define UI_GB35     { .num = 2, .color = {0xff00ff00UL,0xff0000ffUL,0,0}, .bit = {3,5,0,0} } 
#define UI_BG53     { .num = 2, .color = {0xff0000ffUL,0xff00ff00UL,0,0}, .bit = {5,3,0,0} } 
#define UI_GR35     { .num = 2, .color = {0xff00ff00UL,0xffff0000UL,0,0}, .bit = {3,5,0,0} } 
#define UI_RG44     { .num = 2, .color = {0xffff0000UL,0xff00ff00UL,0,0}, .bit = {4,4,0,0} } 
#define UI_GB44     { .num = 2, .color = {0xff00ff00UL,0xff0000ffUL,0,0}, .bit = {4,4,0,0} } 
#define UI_BG44     { .num = 2, .color = {0xff0000ffUL,0xff00ff00UL,0,0}, .bit = {4,4,0,0} } 
#define UI_GR44     { .num = 2, .color = {0xff00ff00UL,0xffff0000UL,0,0}, .bit = {4,4,0,0} }
#define UI_RGRG2222 { .num = 4, .color = {0xffff0000UL,0xff00ff00UL,0xffff0000UL,0xff00ff00UL}, .bit = {2,2,2,2} } 
#define UI_GBGB2222 { .num = 4, .color = {0xff00ff00UL,0xff0000ffUL,0xff00ff00UL,0xff0000ffUL}, .bit = {2,2,2,2} } 
#define UI_BGBG2222 { .num = 4, .color = {0xff0000ffUL,0xff00ff00UL,0xff0000ffUL,0xff00ff00UL}, .bit = {2,2,2,2} } 
#define UI_GRGR2222 { .num = 4, .color = {0xff00ff00UL,0xffff0000UL,0xff00ff00UL,0xffff0000UL}, .bit = {2,2,2,2} }
const QList<QPair<QString, QList<YUVviewer::UICodePoint>>> YUVviewer::YUVFormat_pattern = {
    {"YV12",            {UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,
                         UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,
                         UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,
                         UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V}},
    {"YU12/I420",       {UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,
                         UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,
                         UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,UI_V,
                         UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U,UI_U}},
    {"NV21",            {UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,
                         UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,
                         UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,
                         UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U}},
    {"NV12",            {UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,
                         UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,UI_Y,
                         UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,
                         UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V,UI_U,UI_V}},
    {"YUY2/YUYV",       {UI_Y,UI_U,UI_Y,UI_V}},
    {"YVYU",            {UI_Y,UI_V,UI_Y,UI_U}},
    {"UYVY",            {UI_U,UI_Y,UI_V,UI_Y}},
    {"4:4:4",           {UI_Y,UI_U,UI_V}},
    {"RGB565_L",        {UI_GB35,UI_RG53}},
    {"RGB565_B",        {UI_RG53,UI_GB35}},
    {"BGR565_L",        {UI_GR35,UI_BG53}},
    {"BGR565_B",        {UI_BG53,UI_GR35}},
    {"RGB888",          {UI_R,UI_G,UI_B}},
    {"BayerBG",         {UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,
                         UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R}},
    {"BayerGB",         {UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,
                         UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G}},
    {"BayerRG",         {UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,
                         UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B}},
    {"BayerGR",         {UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,UI_G,UI_R,
                         UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G,UI_B,UI_G}},
    {"BayerBG_RAW10",   {UI_A,UI_A,UI_B,UI_G,UI_B,UI_G,UI_BGBG2222,UI_B,UI_G,UI_B,UI_G,UI_BGBG2222,UI_B,UI_G,UI_B,UI_G,UI_BGBG2222,UI_B,UI_G,UI_B,UI_G,UI_BGBG2222,UI_A,UI_A,
                         UI_A,UI_A,UI_G,UI_R,UI_G,UI_R,UI_GRGR2222,UI_G,UI_R,UI_G,UI_R,UI_GRGR2222,UI_G,UI_R,UI_G,UI_R,UI_GRGR2222,UI_G,UI_R,UI_G,UI_R,UI_GRGR2222,UI_A,UI_A}},
    {"BayerGB_RAW10",   {UI_A,UI_A,UI_G,UI_B,UI_G,UI_B,UI_GBGB2222,UI_G,UI_B,UI_G,UI_B,UI_GBGB2222,UI_G,UI_B,UI_G,UI_B,UI_GBGB2222,UI_G,UI_B,UI_G,UI_B,UI_GBGB2222,UI_A,UI_A,
                         UI_A,UI_A,UI_R,UI_G,UI_R,UI_G,UI_RGRG2222,UI_R,UI_G,UI_R,UI_G,UI_RGRG2222,UI_R,UI_G,UI_R,UI_G,UI_RGRG2222,UI_R,UI_G,UI_R,UI_G,UI_RGRG2222,UI_A,UI_A}},
    {"BayerRG_RAW10",   {UI_A,UI_A,UI_R,UI_G,UI_R,UI_G,UI_RGRG2222,UI_R,UI_G,UI_R,UI_G,UI_RGRG2222,UI_R,UI_G,UI_R,UI_G,UI_RGRG2222,UI_R,UI_G,UI_R,UI_G,UI_RGRG2222,UI_A,UI_A,
                         UI_A,UI_A,UI_G,UI_B,UI_G,UI_B,UI_GBGB2222,UI_G,UI_B,UI_G,UI_B,UI_GBGB2222,UI_G,UI_B,UI_G,UI_B,UI_GBGB2222,UI_G,UI_B,UI_G,UI_B,UI_GBGB2222,UI_A,UI_A}},
    {"BayerGR_RAW10",   {UI_A,UI_A,UI_G,UI_R,UI_G,UI_R,UI_GRGR2222,UI_G,UI_R,UI_G,UI_R,UI_GRGR2222,UI_G,UI_R,UI_G,UI_R,UI_GRGR2222,UI_G,UI_R,UI_G,UI_R,UI_GRGR2222,UI_A,UI_A,
                         UI_A,UI_A,UI_B,UI_G,UI_B,UI_G,UI_BGBG2222,UI_B,UI_G,UI_B,UI_G,UI_BGBG2222,UI_B,UI_G,UI_B,UI_G,UI_BGBG2222,UI_B,UI_G,UI_B,UI_G,UI_BGBG2222,UI_A,UI_A}},
    {"BayerBG_RAW12",   {UI_B,UI_G,UI_BG44,UI_B,UI_G,UI_BG44,UI_B,UI_G,UI_BG44,UI_B,UI_G,UI_BG44,UI_B,UI_G,UI_BG44,UI_B,UI_G,UI_BG44,UI_B,UI_G,UI_BG44,UI_B,UI_G,UI_BG44,
                         UI_G,UI_R,UI_GR44,UI_G,UI_R,UI_GR44,UI_G,UI_R,UI_GR44,UI_G,UI_R,UI_GR44,UI_G,UI_R,UI_GR44,UI_G,UI_R,UI_GR44,UI_G,UI_R,UI_GR44,UI_G,UI_R,UI_GR44}},
    {"BayerGB_RAW12",   {UI_G,UI_B,UI_GB44,UI_G,UI_B,UI_GB44,UI_G,UI_B,UI_GB44,UI_G,UI_B,UI_GB44,UI_G,UI_B,UI_GB44,UI_G,UI_B,UI_GB44,UI_G,UI_B,UI_GB44,UI_G,UI_B,UI_GB44,
                         UI_R,UI_G,UI_RG44,UI_R,UI_G,UI_RG44,UI_R,UI_G,UI_RG44,UI_R,UI_G,UI_RG44,UI_R,UI_G,UI_RG44,UI_R,UI_G,UI_RG44,UI_R,UI_G,UI_RG44,UI_R,UI_G,UI_RG44}},
    {"BayerRG_RAW12",   {UI_R,UI_G,UI_RG44,UI_R,UI_G,UI_RG44,UI_R,UI_G,UI_RG44,UI_R,UI_G,UI_RG44,UI_R,UI_G,UI_RG44,UI_R,UI_G,UI_RG44,UI_R,UI_G,UI_RG44,UI_R,UI_G,UI_RG44,
                         UI_G,UI_B,UI_GB44,UI_G,UI_B,UI_GB44,UI_G,UI_B,UI_GB44,UI_G,UI_B,UI_GB44,UI_G,UI_B,UI_GB44,UI_G,UI_B,UI_GB44,UI_G,UI_B,UI_GB44,UI_G,UI_B,UI_GB44}},
    {"BayerGR_RAW12",   {UI_G,UI_R,UI_GR44,UI_G,UI_R,UI_GR44,UI_G,UI_R,UI_GR44,UI_G,UI_R,UI_GR44,UI_G,UI_R,UI_GR44,UI_G,UI_R,UI_GR44,UI_G,UI_R,UI_GR44,UI_G,UI_R,UI_GR44,
                         UI_B,UI_G,UI_BG44,UI_B,UI_G,UI_BG44,UI_B,UI_G,UI_BG44,UI_B,UI_G,UI_BG44,UI_B,UI_G,UI_BG44,UI_B,UI_G,UI_BG44,UI_B,UI_G,UI_BG44,UI_B,UI_G,UI_BG44}},
    {"BayerBG_RAW16",   {UI_B,UI_B,UI_G,UI_G,UI_B,UI_B,UI_G,UI_G,UI_B,UI_B,UI_G,UI_G,UI_B,UI_B,UI_G,UI_G,UI_B,UI_B,UI_G,UI_G,UI_B,UI_B,UI_G,UI_G,
                         UI_G,UI_G,UI_R,UI_R,UI_G,UI_G,UI_R,UI_R,UI_G,UI_G,UI_R,UI_R,UI_G,UI_G,UI_R,UI_R,UI_G,UI_G,UI_R,UI_R,UI_G,UI_G,UI_R,UI_R}},
    {"BayerGB_RAW16",   {UI_G,UI_G,UI_B,UI_B,UI_G,UI_G,UI_B,UI_B,UI_G,UI_G,UI_B,UI_B,UI_G,UI_G,UI_B,UI_B,UI_G,UI_G,UI_B,UI_B,UI_G,UI_G,UI_B,UI_B,
                         UI_R,UI_R,UI_G,UI_G,UI_R,UI_R,UI_G,UI_G,UI_R,UI_R,UI_G,UI_G,UI_R,UI_R,UI_G,UI_G,UI_R,UI_R,UI_G,UI_G,UI_R,UI_R,UI_G,UI_G}},
    {"BayerRG_RAW16",   {UI_R,UI_R,UI_G,UI_G,UI_R,UI_R,UI_G,UI_G,UI_R,UI_R,UI_G,UI_G,UI_R,UI_R,UI_G,UI_G,UI_R,UI_R,UI_G,UI_G,UI_R,UI_R,UI_G,UI_G,
                         UI_G,UI_G,UI_B,UI_B,UI_G,UI_G,UI_B,UI_B,UI_G,UI_G,UI_B,UI_B,UI_G,UI_G,UI_B,UI_B,UI_G,UI_G,UI_B,UI_B,UI_G,UI_G,UI_B,UI_B}},
    {"BayerGR_RAW16",   {UI_G,UI_G,UI_R,UI_R,UI_G,UI_G,UI_R,UI_R,UI_G,UI_G,UI_R,UI_R,UI_G,UI_G,UI_R,UI_R,UI_G,UI_G,UI_R,UI_R,UI_G,UI_G,UI_R,UI_R,
                         UI_B,UI_B,UI_G,UI_G,UI_B,UI_B,UI_G,UI_G,UI_B,UI_B,UI_G,UI_G,UI_B,UI_B,UI_G,UI_G,UI_B,UI_B,UI_G,UI_G,UI_B,UI_B,UI_G,UI_G}},
    {"PNG",             {UI_A}},
};

YUVviewer::YUVviewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::YUVviewer) {
    int currentIndex;

    ui->setupUi(this);

    this->setWindowTitle("YUVviewer " + VERSION);
    QRect screen = QGuiApplication::screenAt(this->mapToGlobal(QPoint(this->width()/2,0)))->geometry();
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

    QFileInfo dir(QDir::homePath()+"/.YUVViewer");
    if(!dir.isDir()) {
        if(!dir.isFile()) {
            QDir mkdir(QDir::homePath());
            mkdir.mkdir(".YUVViewer");
        }
    }

    YUVviewerConfigFile = new ConfigFile(QDir::homePath()+"/.YUVViewer/YUVViewer.xml");
    if(YUVviewerConfigFile->config_dict.YUVFormat == "PNG") {
        if(YUVviewerConfigFile->config_dict.frameSizeType == "Other") {
            ui->frameSizeType_Other_RadioButton->setChecked(true);
            ui->frameSizeType_ComboBox->setEnabled(false);
        } else {
            ui->frameSizeType_Combo_RadioButton->setChecked(true);
            ui->frameSizeType_ComboBox->setEnabled(true);
            ui->frameSizeType_ComboBox->setCurrentText(YUVviewerConfigFile->config_dict.frameSizeType);
        }
        ui->label_svgBox->setPixmap(QPixmap::fromImage(QImage(":/img/img/ico.png").scaled(480,80,Qt::KeepAspectRatio,Qt::SmoothTransformation)));
        ui->label_svgBox->setAlignment(Qt::AlignCenter);
        ui->YUVFormat_ComboBox->setCurrentText("PNG");
        ui->frameSize_Width_LineEdit->setText("\\");
        ui->frameSize_Height_LineEdit->setText("\\");
        ui->frameSize_Width_LineEdit->setFocusPolicy(Qt::NoFocus);
        ui->frameSize_Height_LineEdit->setFocusPolicy(Qt::NoFocus);
    } else {
        if(YUVviewerConfigFile->config_dict.frameSizeType == "Other") {
            ui->frameSizeType_Other_RadioButton->setChecked(true);
            ui->frameSizeType_ComboBox->setEnabled(false);
            ui->frameSize_Width_LineEdit->setText(YUVviewerConfigFile->config_dict.frameSize_Width);
            ui->frameSize_Height_LineEdit->setText(YUVviewerConfigFile->config_dict.frameSize_Height);
        } else {
            ui->frameSizeType_Combo_RadioButton->setChecked(true);
            ui->frameSizeType_ComboBox->setEnabled(true);
            QList<QPair<QString, QStringList>>::const_iterator config_it = frameSizeTypeDict.begin();
            while (config_it != frameSizeTypeDict.end()) {
                if(config_it->first == YUVviewerConfigFile->config_dict.frameSizeType) {
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

        currentIndex = 0;
        QList<YUVviewer::UICodePoint> color_list;
        QList<QPair<QString, QList<YUVviewer::UICodePoint>>>::const_iterator yuvformat_it = YUVFormat_pattern.begin();
        while (yuvformat_it != YUVFormat_pattern.end()) {
            if(yuvformat_it->first == YUVviewerConfigFile->config_dict.YUVFormat) {
                ui->YUVFormat_ComboBox->setCurrentIndex(currentIndex);
                color_list = yuvformat_it->second;
                break;
            }
            currentIndex++;
            yuvformat_it++;
        }
        updateUiSvg(color_list);
    }

    QStringList frameRate_list = {"30", "60", "120"};
    currentIndex = 0;
    foreach(QString s ,frameRate_list) {
        if(s == YUVviewerConfigFile->config_dict.frameRate) {
            ui->frameRate_ComboBox->setCurrentIndex(currentIndex);
        }
        currentIndex++;
    }

    ui->startFrame_LineEdit->setText(YUVviewerConfigFile->config_dict.startFrame);
    ui->endFrame_LineEdit->setText(YUVviewerConfigFile->config_dict.endFrame);

    QObject::connect(ui->YUVFormat_ComboBox, SIGNAL(currentTextChanged(const QString &)), 
                                                    this, SLOT(changeFormat(const QString &)));
    QObject::connect(ui->frameSizeType_Combo_RadioButton, SIGNAL(clicked()), this, SLOT(configComboBox()));
    QObject::connect(ui->frameSizeType_Other_RadioButton, SIGNAL(clicked()), this, SLOT(configOther()));
    QObject::connect(ui->frameSizeType_ComboBox, SIGNAL(currentTextChanged(const QString &)), 
                                                    this,SLOT(changeFrameSizeType(const QString &)));

    QObject::connect(ui->frameSize_Height_LineEdit, SIGNAL(textEdited(const QString &)), 
                                                    this, SLOT(frameSizeHeightValidator(const QString &)));
    QObject::connect(ui->frameSize_Width_LineEdit, SIGNAL(textEdited(const QString &)), 
                                                    this, SLOT(frameSizeWidthValidator(const QString &)));
    QObject::connect(ui->startFrame_LineEdit, SIGNAL(textEdited(const QString &)), 
                                                    this, SLOT(startFrameValidator(const QString &)));
    QObject::connect(ui->endFrame_LineEdit, SIGNAL(textEdited(const QString &)), 
                                                    this, SLOT(endFrameValidator(const QString &)));

    QObject::connect(ui->exchange_PushButton, SIGNAL(clicked()), this, SLOT(exchaneSize()));
    QObject::connect(ui->openFile_PushButton, SIGNAL(clicked()), this, SLOT(openFile()));
    QObject::connect(ui->openFolder_PushButton, SIGNAL(clicked()), this, SLOT(openFolder()));
    QObject::connect(ui->help_PushButton, SIGNAL(clicked()), this, SLOT(help()));
    QObject::connect(ui->about_PushButton, SIGNAL(clicked()), this, SLOT(about()));
    QObject::connect(ui->aboutQt_PushButton, SIGNAL(clicked()), this, SLOT(aboutQt()));

    imgViewer = nullptr;
}

YUVviewer::~YUVviewer() {
    if(imgViewer != nullptr) {
        delete imgViewer;
    }
    if(YUVviewerConfigFile != nullptr) {
        delete YUVviewerConfigFile;
    }
    delete ui;
}

QString YUVviewer::svgBoxSrc(int x, int y, int w, YUVviewer::UICodePoint c) {
    switch(c.num) {
        case 1:{
            uint32_t color = c.color[0] & 0xffffff;
            return QString("<line class=\"0\" x1=\"%1\" y1=\"%2\" x2=\"%3\" y2=\"%4\" stroke=\"#%5\" fill=\"none\" stroke-width=\"%6\" />\n")
                .arg(x).arg(y+w/2).arg(x+w).arg(y+w/2).arg(color,6,16,QLatin1Char('0')).arg(w);
        }
        case 2:{
            uint32_t color0 = c.color[0] & 0xffffff;
            uint32_t color1 = c.color[1] & 0xffffff;
            uint8_t fix_w = (w*(c.bit[0]))/(c.bit[1]+c.bit[0]);
            QString ret = "";
            ret += QString("<line class=\"0\" x1=\"%1\" y1=\"%2\" x2=\"%3\" y2=\"%4\" stroke=\"#%5\" fill=\"none\" stroke-width=\"%6\" />\n")
                       .arg(x).arg(y+w/2).arg(x+fix_w).arg(y+w/2).arg(color0,6,16,QLatin1Char('0')).arg(w);
            ret += QString("<line class=\"0\" x1=\"%1\" y1=\"%2\" x2=\"%3\" y2=\"%4\" stroke=\"#%5\" fill=\"none\" stroke-width=\"%6\" />\n")
                       .arg(x+fix_w).arg(y+w/2).arg(x+w).arg(y+w/2).arg(color1,6,16,QLatin1Char('0')).arg(w);
            return ret;
        }
        case 4:{
            uint32_t color0 = c.color[0] & 0xffffff;
            uint32_t color1 = c.color[1] & 0xffffff;
            uint32_t color2 = c.color[2] & 0xffffff;
            uint32_t color3 = c.color[3] & 0xffffff;
            uint8_t fix_w1 = (w*(c.bit[0])                  ) / (c.bit[3]+c.bit[2]+c.bit[1]+c.bit[0]);
            uint8_t fix_w2 = (w*(c.bit[0]+c.bit[1])         ) / (c.bit[3]+c.bit[2]+c.bit[1]+c.bit[0]);
            uint8_t fix_w3 = (w*(c.bit[0]+c.bit[1]+c.bit[2])) / (c.bit[3]+c.bit[2]+c.bit[1]+c.bit[0]);
            QString ret = "";
            ret += QString("<line class=\"0\" x1=\"%1\" y1=\"%2\" x2=\"%3\" y2=\"%4\" stroke=\"#%5\" fill=\"none\" stroke-width=\"%6\" />\n")
                       .arg(x).arg(y+w/2).arg(x+fix_w1).arg(y+w/2).arg(color0,6,16,QLatin1Char('0')).arg(w);
            ret += QString("<line class=\"0\" x1=\"%1\" y1=\"%2\" x2=\"%3\" y2=\"%4\" stroke=\"#%5\" fill=\"none\" stroke-width=\"%6\" />\n")
                       .arg(x+fix_w1).arg(y+w/2).arg(x+fix_w2).arg(y+w/2).arg(color1,6,16,QLatin1Char('0')).arg(w);
            ret += QString("<line class=\"0\" x1=\"%1\" y1=\"%2\" x2=\"%3\" y2=\"%4\" stroke=\"#%5\" fill=\"none\" stroke-width=\"%6\" />\n")
                       .arg(x+fix_w2).arg(y+w/2).arg(x+fix_w3).arg(y+w/2).arg(color2,6,16,QLatin1Char('0')).arg(w);
            ret += QString("<line class=\"0\" x1=\"%1\" y1=\"%2\" x2=\"%3\" y2=\"%4\" stroke=\"#%5\" fill=\"none\" stroke-width=\"%6\" />\n")
                       .arg(x+fix_w3).arg(y+w/2).arg(x+w).arg(y+w/2).arg(color3,6,16,QLatin1Char('0')).arg(w);
            return ret;
        }
        case 0:
        default:
            return QString();
    }
}

QString YUVviewer::svgBoxArraySrc(int x, int y, int w, int od, int xn, int yn,QList<YUVviewer::UICodePoint> ca) {
    QString ret = "";
    int num = ca.count();
    if(num == 0) return  ret;
    for(int i=0;i<yn;i++) {
        for(int j=0;j<xn;j++) {
            ret += svgBoxSrc(x+(w+od)*j,y+(w+od)*i,w,ca.at((i*xn+j)%num));
        }
    }
    return  ret;
}

QString YUVviewer::getLegend(const QString &type) {
    if(type == "RGB") {
        return 
        "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n"
            "<line class=\"0\" x1=\"0\" y1=\"2\" x2=\"3\" y2=\"2\" stroke=\"#ff0000\" fill=\"none\" stroke-width=\"3\" />\n"
            "<text x=\"5\" y=\"3\" fill=\"#000000\" font-size=\"3\">R</text>\n"
            "<line class=\"0\" x1=\"0\" y1=\"7\" x2=\"3\" y2=\"7\" stroke=\"#00ff00\" fill=\"none\" stroke-width=\"3\" />\n"
            "<text x=\"5\" y=\"8\" fill=\"#000000\" font-size=\"3\">G</text>\n"
            "<line class=\"0\" x1=\"0\" y1=\"12\" x2=\"3\" y2=\"12\" stroke=\"#0000ff\" fill=\"none\" stroke-width=\"3\" />\n"
            "<text x=\"5\" y=\"13\" fill=\"#000000\" font-size=\"3\">B</text>\n"
        "</svg>\n";
    } else if(type == "YUV") {
        return 
        "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n"
            "<line class=\"0\" x1=\"0\" y1=\"2\" x2=\"3\" y2=\"2\" stroke=\"#808080\" fill=\"none\" stroke-width=\"3\" />\n"
            "<text x=\"5\" y=\"3\" fill=\"#000000\" font-size=\"3\">Y</text>\n"
            "<line class=\"0\" x1=\"0\" y1=\"7\" x2=\"3\" y2=\"7\" stroke=\"#ff00ff\" fill=\"none\" stroke-width=\"3\" />\n"
            "<text x=\"5\" y=\"8\" fill=\"#000000\" font-size=\"3\">U</text>\n"
            "<line class=\"0\" x1=\"0\" y1=\"12\" x2=\"3\" y2=\"12\" stroke=\"#00ffff\" fill=\"none\" stroke-width=\"3\" />\n"
            "<text x=\"5\" y=\"13\" fill=\"#000000\" font-size=\"3\">V</text>\n"
        "</svg>\n";
    } else {
        return 
        "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n"
        "</svg>\n";
    }
}

void YUVviewer::updateUiSvg(QList<YUVviewer::UICodePoint> color_list) {
    QXmlStreamReader svgXmlStreamReader(
        "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n" +
        svgBoxArraySrc(10,10,40,5,24,4,color_list) +
        "</svg>\n");
    QSvgRenderer svgRender;
    svgRender.load(&svgXmlStreamReader);
    QPixmap svgPixmap(480,80);
    svgPixmap.fill(Qt::transparent);
    QPainter svgPainter(&svgPixmap);
    svgRender.render(&svgPainter);
    ui->label_svgBox->setPixmap(svgPixmap);
    ui->label_svgBox->setAlignment(Qt::AlignCenter);

    QString legend = getLegend("");
    switch (color_list.at(0).color[0]&0xffffff)
    {
        case 0xFF0000:
        case 0x00FF00:
        case 0x0000FF:
            legend = getLegend("RGB");
            break;
        case 0x808080:
        case 0xFF00FF:
        case 0x00FFFF:
            legend = getLegend("YUV");
            break;
    }
    QXmlStreamReader svgXmlStreamReaderLegend(legend);
    QSvgRenderer svgRenderLegend;
    svgRenderLegend.load(&svgXmlStreamReaderLegend);
    QPixmap svgPixmapLegend(50,80);
    svgPixmapLegend.fill(Qt::transparent);
    QPainter svgPainterLegend(&svgPixmapLegend);
    svgRenderLegend.render(&svgPainterLegend);
    ui->label_Legend->setPixmap(svgPixmapLegend);
    ui->label_Legend->setAlignment(Qt::AlignVCenter);
}

void YUVviewer::changeFormat(const QString &text) {
    QList<YUVviewer::UICodePoint> color_list;
    QList<QPair<QString, QList<YUVviewer::UICodePoint>>>::const_iterator yuvformat_it = YUVFormat_pattern.begin();
    while (yuvformat_it != YUVFormat_pattern.end()) {
        if(yuvformat_it->first == text) {
            color_list = yuvformat_it->second;
            break;
        }
        yuvformat_it++;
    }
    updateUiSvg(color_list);
    if(text == "PNG") {
        ui->label_svgBox->setPixmap(QPixmap::fromImage(QImage(":/img/img/ico.png").scaled(480,80,Qt::KeepAspectRatio,Qt::SmoothTransformation)));
        ui->frameSize_Width_LineEdit->setText("\\");
        ui->frameSize_Height_LineEdit->setText("\\");
        ui->frameSize_Width_LineEdit->setFocusPolicy(Qt::NoFocus);
        ui->frameSize_Height_LineEdit->setFocusPolicy(Qt::NoFocus);
    } else {
        if(ui->frameSizeType_Combo_RadioButton->isChecked()) {
            QList<QPair<QString, QStringList>>::const_iterator config_it = frameSizeTypeDict.begin();
            while (config_it != frameSizeTypeDict.end()) {
                if(config_it->first == ui->frameSizeType_ComboBox->currentText()) {
                    QStringList value = config_it->second;
                    ui->frameSize_Width_LineEdit->setText(value[0]);
                    ui->frameSize_Width_LineEdit->setFocusPolicy(Qt::NoFocus);
                    ui->frameSize_Height_LineEdit->setText(value[1]);
                    ui->frameSize_Height_LineEdit->setFocusPolicy(Qt::NoFocus);
                    break;
                }
                config_it++;
            }
        } else if(ui->frameSizeType_Other_RadioButton->isChecked()) {
            if(YUVviewerConfigFile->config_dict.frameSize_Width != "\\") {
                ui->frameSize_Width_LineEdit->setText(YUVviewerConfigFile->config_dict.frameSize_Width);
            } else {
                ui->frameSize_Width_LineEdit->setText("640");
            }
            ui->frameSize_Width_LineEdit->setText(YUVviewerConfigFile->config_dict.frameSize_Width);
            ui->frameSize_Width_LineEdit->setFocusPolicy(Qt::StrongFocus);
            if(YUVviewerConfigFile->config_dict.frameSize_Height != "\\") {
                ui->frameSize_Height_LineEdit->setText(YUVviewerConfigFile->config_dict.frameSize_Height);
            } else {
                ui->frameSize_Height_LineEdit->setText("480");
            }
            ui->frameSize_Height_LineEdit->setFocusPolicy(Qt::StrongFocus);
        }
    }
    ui->label_svgBox->repaint();
}

void YUVviewer::configComboBox() {
    ui->frameSizeType_ComboBox->setEnabled(true);
    QList<QPair<QString, QStringList>>::const_iterator config_it = frameSizeTypeDict.begin();
    while (config_it != frameSizeTypeDict.end()) {
        if(config_it->first == ui->frameSizeType_ComboBox->currentText()) {
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

void YUVviewer::changeFrameSizeType(const QString &text) {
    QList<QPair<QString, QStringList>>::const_iterator config_it = frameSizeTypeDict.begin();
    while (config_it != frameSizeTypeDict.end()) {
        if(config_it->first == text) {
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

void YUVviewer::configOther() {
    ui->frameSizeType_ComboBox->setEnabled(false);
    ui->frameSize_Width_LineEdit->setText(YUVviewerConfigFile->config_dict.frameSize_Width);
    ui->frameSize_Height_LineEdit->setText(YUVviewerConfigFile->config_dict.frameSize_Height);
    ui->frameSize_Width_LineEdit->setFocusPolicy(Qt::StrongFocus);
    ui->frameSize_Height_LineEdit->setFocusPolicy(Qt::StrongFocus);
}

void YUVviewer::frameSizeHeightValidator(const QString &currentText) {
    bool isInt;
    int currentVale = currentText.toInt(&isInt);
    if(isInt == true) {
        if(((currentVale%2) == 0) && (currentVale > 0)) {
            ui->frameSize_Height_LineEdit->setStyleSheet("QLineEdit{border:1px solid gray border-radius:1px}");
        } else {
            QToolTip::showText(ui->frameSize_Height_LineEdit->mapToGlobal(QPoint(0, 10)), "Height must be positive even");
            ui->frameSize_Height_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
        }
    } else {
        QToolTip::showText(ui->frameSize_Height_LineEdit->mapToGlobal(QPoint(0, 10)), "Height must be num");
        ui->frameSize_Height_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
    }
}

void YUVviewer::frameSizeWidthValidator(const QString &currentText) {
    bool isInt;
    int currentVale = currentText.toInt(&isInt);
    if(isInt == true) {
        if(((currentVale%2) == 0) && (currentVale > 0)) {
            ui->frameSize_Width_LineEdit->setStyleSheet("QLineEdit{border:1px solid gray border-radius:1px}");
        } else {
            QToolTip::showText(ui->frameSize_Width_LineEdit->mapToGlobal(QPoint(0, 10)), "Width must be positive even");
            ui->frameSize_Width_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
        }
    } else {
        QToolTip::showText(ui->frameSize_Width_LineEdit->mapToGlobal(QPoint(0, 10)), "Width must be num");
        ui->frameSize_Width_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
    }
}

void YUVviewer::startFrameValidator(const QString & currentText) {
    bool isInt;
    int currentVale = currentText.toInt(&isInt);
    if(isInt != true) {
        QToolTip::showText(ui->startFrame_LineEdit->mapToGlobal(QPoint(0, 10)), "startFrame must be num");
        ui->startFrame_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
        return;
    }
    if(currentVale >= 0) {
        ui->startFrame_LineEdit->setStyleSheet("QLineEdit{border:1px solid gray border-radius:1px}");
        int endFrameVale = ui->endFrame_LineEdit->text().toInt(&isInt);
        if(isInt != true) {
            QToolTip::showText(ui->endFrame_LineEdit->mapToGlobal(QPoint(0, 10)), "endFrame must be num");
            ui->endFrame_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
            return;
        }
        if(currentVale <= endFrameVale) {
            ui->endFrame_LineEdit->setStyleSheet("QLineEdit{border:1px solid gray border-radius:1px}");
        } else {
            QToolTip::showText(ui->endFrame_LineEdit->mapToGlobal(QPoint(0, 10)), "endFrame must be greater than or equal to startFrame");
            ui->endFrame_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
        }
    } else {
        QToolTip::showText(ui->startFrame_LineEdit->mapToGlobal(QPoint(0, 10)), "startFrame must be greater than or equal to 0");
        ui->startFrame_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
    }
}

void YUVviewer::endFrameValidator(const QString & currentText) {
    bool isInt;
    int currentVale = currentText.toInt(&isInt);
    if(isInt != true) {
        QToolTip::showText(ui->endFrame_LineEdit->mapToGlobal(QPoint(0, 10)), "endFrame must be num");
        ui->endFrame_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
        return;
    }

    ui->endFrame_LineEdit->setStyleSheet("QLineEdit{border:1px solid gray border-radius:1px}");

    int startVale = ui->startFrame_LineEdit->text().toInt(&isInt);
    if(isInt == true) {
        if(currentVale < startVale) {
            QToolTip::showText(ui->endFrame_LineEdit->mapToGlobal(QPoint(0, 10)), "endFrame must be greater than or equal to startFrame");
            ui->endFrame_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
        }
    } else {
        QToolTip::showText(ui->startFrame_LineEdit->mapToGlobal(QPoint(0, 10)), "startFrame must be num");
        ui->startFrame_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
    }
}

void YUVviewer::exchaneSize() { 
    if(ui->YUVFormat_ComboBox->currentText() != "PNG") {
        ui->frameSizeType_Other_RadioButton->setChecked(true);
        ui->frameSizeType_ComboBox->setEnabled(false);
        QString width = ui->frameSize_Width_LineEdit->text();
        ui->frameSize_Width_LineEdit->setText(ui->frameSize_Height_LineEdit->text());
        ui->frameSize_Height_LineEdit->setText(width);
        frameSizeWidthValidator(ui->frameSize_Width_LineEdit->text());
        frameSizeHeightValidator(ui->frameSize_Height_LineEdit->text());
    }
}

void YUVviewer::showParaErrMessageBox(void) {
    QMessageBox::critical(this, "Error", "parameter invalid!!", QMessageBox::Ok);
}

bool YUVviewer::updateConfig(void) {
    bool isInt;
    int startFrame = ui->startFrame_LineEdit->text().toInt(&isInt);
    if(!isInt) {
        showParaErrMessageBox();
        return false;
    }
    int endFrame = ui->endFrame_LineEdit->text().toInt(&isInt);
    if(!isInt) {
        showParaErrMessageBox();
        return false;
    }
    int temp_Width = ui->frameSize_Width_LineEdit->text().toInt(&isInt);
    if(!isInt && ui->YUVFormat_ComboBox->currentText() != "PNG") {
        showParaErrMessageBox();
        return false;
    }
    int temp_Height = ui->frameSize_Height_LineEdit->text().toInt(&isInt);
    if(!isInt && ui->YUVFormat_ComboBox->currentText() != "PNG") {
        showParaErrMessageBox();
        return false;
    }

    if(startFrame <= endFrame) {
        if(ui->YUVFormat_ComboBox->currentText() != "PNG") {
            if(((temp_Width % 2) == 0) && ((temp_Height % 2) == 0) && (temp_Width > 0) && (temp_Height > 0)) {
                YUVviewerConfigFile->config_dict.frameSize_Width = ui->frameSize_Width_LineEdit->text();
                YUVviewerConfigFile->config_dict.frameSize_Height = ui->frameSize_Height_LineEdit->text();
            } else {
                QMessageBox::critical(this, "Error", "frameSize invalid!!", QMessageBox::Ok);
                return false;
            }
        }

        if(ui->frameSizeType_Combo_RadioButton->isChecked()) {
            YUVviewerConfigFile->config_dict.frameSizeType = ui->frameSizeType_ComboBox->currentText();
        } else if (ui->frameSizeType_Other_RadioButton->isChecked()) {
            YUVviewerConfigFile->config_dict.frameSizeType = "Other";
        }
        YUVviewerConfigFile->config_dict.YUVFormat = ui->YUVFormat_ComboBox->currentText();
        YUVviewerConfigFile->config_dict.frameRate = ui->frameRate_ComboBox->currentText();
        YUVviewerConfigFile->config_dict.startFrame = ui->startFrame_LineEdit->text();
        YUVviewerConfigFile->config_dict.endFrame = ui->endFrame_LineEdit->text();

        return true;
    } else {
        QMessageBox::critical(this, "Error", "startFrame or endFrame invalid!!", QMessageBox::Ok);
        return false;
    }
}

bool YUVviewer::imgView(QStringList openfile_list, const QString &folderpath) {
    if (openfile_list.empty()) {
        return false;
    }
    if(imgViewer != nullptr) {
        delete imgViewer;
        imgViewer = nullptr;
    }
    imgViewer = new ImgViewer(folderpath,nullptr,this);
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
    if(!isSuccess) {
        QMessageBox::critical(this, "Error", "unsupport YUVFormat!!", QMessageBox::Ok);
        this->show();
        return false;
    }
    float fframeSize_Width = (float)frameSize_Width;
    float fframeSize_Height = (float)frameSize_Height;
    if(frameSize_Width > frameSize_Height) {
        imgViewer->resize(800, fframeSize_Height/fframeSize_Width*800.0);
    } else {
        imgViewer->resize(fframeSize_Width/fframeSize_Height*400.0, 400);
    }
    QRect screen = QGuiApplication::screenAt(this->mapToGlobal(QPoint(this->width()/2,0)))->geometry();
    QRect size = imgViewer->geometry();
    imgViewer->move((screen.width() - size.width()) / 2, (screen.height() - size.height()) / 2);
    this->hide();
    imgViewer->show();

    return true;
}

void YUVviewer::openFile() {
    if(updateConfig()) {
        QString openDir = "";
        QFileInfo lastPath(YUVviewerConfigFile->config_dict.lastPath);
        if(lastPath.isDir()) {
            openDir = YUVviewerConfigFile->config_dict.lastPath;
        }
        QStringList openfile_list = FileDialog::getOpenFileNames(this, tr("Open File"), openDir, "files(*.yuv *.data *.raw *.png)");
        if(openfile_list.size() != 0) {
            QFileInfo file(openfile_list[0]);
            YUVviewerConfigFile->config_dict.lastPath = file.absolutePath();
            imgView(openfile_list,file.absolutePath());
        }
    }
}

void YUVviewer::openFolder() {
    if(updateConfig()) {
        QString openDir = "";
        QFileInfo lastPath(YUVviewerConfigFile->config_dict.lastPath);
        if(lastPath.isDir()) {
            openDir = YUVviewerConfigFile->config_dict.lastPath;
        }
        QString openfolder_name = FileDialog::getExistingDirectory(this, tr("Open Folder"), openDir);
        if (!openfolder_name.isEmpty()) {
            YUVviewerConfigFile->config_dict.lastPath = openfolder_name;
            QDir dir(openfolder_name);
            QStringList nameFilters = {"*.yuv","*.data","*.raw","*.png"};
            QStringList openfilename_list = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
            QStringList openfile_list;
            foreach (QString file_name, openfilename_list) {
                openfile_list.append(QDir::toNativeSeparators(openfolder_name + '/' +file_name));
            }
            if(openfile_list.size() != 0) {
                imgView(openfile_list,openfolder_name);
            }
        }
    }
}

void YUVviewer::help() {
    QMessageBox::question(this, "Help", tr(
        "1.Select parameters on the main interface.\n"
        "2.Click Open File or Folder to parse the image data and display the image.\n"
        "3.Used in the image display interface\n"
            "  - scroll wheel to zoom in and out of the image,\n"
            "  - use the left button to drag the image,\n"
            "  - Double click the left button to export the image as png format or other raw format,\n"
            "  - Right click to reset image size and position,\n"
            "  - Double click on the right button to swap the image R and B channel display,\n"
            "  - Middle click to display the original size of the image."),
        QMessageBox::StandardButtons(QMessageBox::Ok));
}

void YUVviewer::about() {
    QMessageBox::about(this, tr("About"),
        tr(
            "<p>Version</p>"
            "<p>&nbsp;%1</p>"
            "<p>Commit</p>"
            "<p>&nbsp;%2</p>"
            "<p>Author</p>"
            "<p>&nbsp;qiaoqm@aliyun.com</p>"
            "<p>&nbsp;wenqing.li@aliyun.com</p>"
            "<p>Website</p>"
            "<p>&nbsp;<a href='https://github.com/QQxiaoming/YUVviewer'>https://github.com/QQxiaoming</p>"
            "<p>&nbsp;<a href='https://gitee.com/QQxiaoming/YUVviewer'>https://gitee.com/QQxiaoming</a></p>"
        ).arg(VERSION,GIT_TAG)
    );
}

void YUVviewer::aboutQt()
{
    QMessageBox::aboutQt(this);
}

void YUVviewer::closeEvent(QCloseEvent *event) {
    if(YUVviewerConfigFile != nullptr) {
        delete YUVviewerConfigFile;
        YUVviewerConfigFile = nullptr;
    }
    event->accept();
}

static QTranslator qtTranslator;
static QTranslator qtbaseTranslator;
static QTranslator appTranslator;

int main(int argc, char *argv[]) {
    if(argc == 2) {
        if((!strncmp(argv[1],"--version",9)) | (!strncmp(argv[1],"-v",2)) ) {
            std::cout << "YUVviewer " << VERSION.toStdString() << "\n" << GIT_TAG.toStdString() << "\n";
            return 0;
        }
    }
    QApplication::setAttribute(Qt::AA_DontUseNativeDialogs);
    QApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);
    QApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    QApplication application(argc, argv);

    QLocale locale;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QString qlibpath = QLibraryInfo::path(QLibraryInfo::TranslationsPath);
#else
    QString qlibpath = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
#endif
    QLocale::Language lang = locale.language();
    switch(lang) {
    case QLocale::Chinese:
        if(qtTranslator.load("qt_zh_CN.qm",qlibpath))
            application.installTranslator(&qtTranslator);
        if(qtbaseTranslator.load("qtbase_zh_CN.qm",qlibpath))
            application.installTranslator(&qtbaseTranslator);
        if(appTranslator.load(":/lang/lang/yuvviewer_zh_CN.qm"))
            application.installTranslator(&appTranslator);
        break;
    case QLocale::Japanese:
        if(qtTranslator.load("qt_ja.qm",qlibpath))
            application.installTranslator(&qtTranslator);
        if(qtbaseTranslator.load("qtbase_ja.qm",qlibpath))
            application.installTranslator(&qtbaseTranslator);
        if(appTranslator.load(":/lang/lang/yuvviewer_ja_JP.qm"))
            application.installTranslator(&appTranslator);
        break;
    default:
    case QLocale::English:
        if(qtTranslator.load("qt_en.qm",qlibpath))
            application.installTranslator(&qtTranslator);
        if(qtbaseTranslator.load("qtbase_en.qm",qlibpath))
            application.installTranslator(&qtbaseTranslator);
        if(appTranslator.load(":/lang/lang/yuvviewer_en_US.qm"))
            application.installTranslator(&appTranslator);
        break;
    }

    YUVviewer window;
    window.show();

    return application.exec();
}
