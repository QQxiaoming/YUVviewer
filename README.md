[![CI](https://github.com/QQxiaoming/YUVviewer/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/QQxiaoming/YUVviewer/actions/workflows/ci.yml)
[![CodeFactor](https://www.codefactor.io/repository/github/qqxiaoming/yuvviewer/badge)](https://www.codefactor.io/repository/github/qqxiaoming/yuvviewer)
[![GitHub tag (latest SemVer)](https://img.shields.io/github/tag/QQxiaoming/YUVviewer.svg)](https://github.com/QQxiaoming/YUVviewer/releases)
[![GitHub All Releases](https://img.shields.io/github/downloads/QQxiaoming/YUVviewer/total.svg)](https://github.com/QQxiaoming/YUVviewer/releases)
[![GitHub stars](https://img.shields.io/github/stars/QQxiaoming/YUVviewer.svg)](https://github.com/QQxiaoming/YUVviewer)

# YUVviewer

English | [简体中文](./README_zh_CN.md)

YUVviewer is a small tool for developers to view and preview various original image formats. It is based on the framework of Qt and opencv and supports windows/linux/macos. The interface is simple and clear, and the operation is portable. The main interface is as follows:

![img0](./img/docimg0.png)

## Feature

1. Select parameters on the main interface.
2. Click Open File or Folder to parse the image data and display the image.
3. Used in the image display interface
    - scroll wheel to zoom in and out of the image,
    - use the left button to drag the image,
    - Double click the left button to save the image as png format,
    - Right click to reset image size and position,
    - Double click on the right button to swap the image R and B channel display,
    - Middle click to display the original size of the image.

## Format

Currently supported formats include:

YV12、YU12/I420、NV21、NV12、YUY2/YUYV、YVYU、UYVY、YUV444、RGB565_L、RGB565_B、BGR565_L、BGR565_B、RGB888、BayerBG、BayerGB、BayerRG、BayerGR、BayerBG_RAW12、BayerGB_RAW12、BayerRG_RAW12、BayerGR_RAW12、

## Build

　> [Build documentation](./DEVELOPNOTE.md)
