[![CI](https://github.com/QQxiaoming/YUVviewer/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/QQxiaoming/YUVviewer/actions/workflows/ci.yml)
[![CodeFactor](https://www.codefactor.io/repository/github/qqxiaoming/yuvviewer/badge)](https://www.codefactor.io/repository/github/qqxiaoming/yuvviewer)
[![GitHub tag (latest SemVer)](https://img.shields.io/github/tag/QQxiaoming/YUVviewer.svg)](https://github.com/QQxiaoming/YUVviewer/releases)
[![GitHub All Releases](https://img.shields.io/github/downloads/QQxiaoming/YUVviewer/total.svg)](https://github.com/QQxiaoming/YUVviewer/releases)
[![GitHub stars](https://img.shields.io/github/stars/QQxiaoming/YUVviewer.svg)](https://github.com/QQxiaoming/YUVviewer)
[![GitHub forks](https://img.shields.io/github/forks/QQxiaoming/YUVviewer.svg)](https://github.com/QQxiaoming/YUVviewer)
[![Gitee stars](https://gitee.com/QQxiaoming/YUVviewer/badge/star.svg?theme=dark)](https://gitee.com/QQxiaoming/YUVviewer)
[![Gitee forks](https://gitee.com/QQxiaoming/YUVviewer/badge/fork.svg?theme=dark)](https://gitee.com/QQxiaoming/YUVviewer)

# YUVviewer

[English](./README.md) | 简体中文

YUVviewer一款用于开发人员查看预览各种原始图像格式的小工具，其基于Qt和opencv的框架，支持windows/linux/macos，界面简单清晰，操作便携，主界面如下：

![img0](./img/docimg0.png)

## 功能描述

1. 主界面选择参数。
2. 点击打开文件或文件夹将进行图像数据解析并显示图像。
3. 图像显示界面中使用
    - 滚轮放大缩小图像，
    - 使用左键可拖动图像，
    - 双击左键保存图像为png格式，
    - 单击右键复位图像大小和位置，
    - 双击右键交换图像R和B通道显示，
    - 单击中键显示图像原始大小。

## 格式支持

目前支持格式包括：

YV12、YU12/I420、NV21、NV12、YUY2/YUYV、YVYU、UYVY、YUV444、RGB565_L、RGB565_B、BGR565_L、BGR565_B、RGB888、BayerBG、BayerGB、BayerRG、BayerGR、BayerBG_RAW12、BayerGB_RAW12、BayerRG_RAW12、BayerGR_RAW12、

## 编译说明

　> [编译说明](./DEVELOPNOTE.md)