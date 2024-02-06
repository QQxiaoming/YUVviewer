[![Windows ci](https://img.shields.io/github/actions/workflow/status/qqxiaoming/yuvviewer/windows.yml?branch=main&logo=windows)](https://github.com/QQxiaoming/yuvviewer/actions/workflows/windows.yml)
[![Linux ci](https://img.shields.io/github/actions/workflow/status/qqxiaoming/yuvviewer/linux.yml?branch=main&logo=linux)](https://github.com/QQxiaoming/yuvviewer/actions/workflows/linux.yml)
[![Macos ci](https://img.shields.io/github/actions/workflow/status/qqxiaoming/yuvviewer/macos.yml?branch=main&logo=apple)](https://github.com/QQxiaoming/yuvviewer/actions/workflows/macos.yml)
[![CodeFactor](https://img.shields.io/codefactor/grade/github/qqxiaoming/yuvviewer.svg?logo=codefactor)](https://www.codefactor.io/repository/github/qqxiaoming/yuvviewer)
[![License](https://img.shields.io/github/license/qqxiaoming/yuvviewer.svg?colorB=f48041&logo=gnu)](https://github.com/QQxiaoming/yuvviewer)
[![GitHub tag (latest SemVer)](https://img.shields.io/github/tag/QQxiaoming/YUVviewer.svg?logo=git)](https://github.com/QQxiaoming/YUVviewer/releases)
[![GitHub All Releases](https://img.shields.io/github/downloads/QQxiaoming/YUVviewer/total.svg?logo=pinboard)](https://github.com/QQxiaoming/YUVviewer/releases)
[![GitHub stars](https://img.shields.io/github/stars/QQxiaoming/YUVviewer.svg?logo=github)](https://github.com/QQxiaoming/YUVviewer)
[![GitHub forks](https://img.shields.io/github/forks/QQxiaoming/YUVviewer.svg?logo=github)](https://github.com/QQxiaoming/YUVviewer)
[![Gitee stars](https://gitee.com/QQxiaoming/YUVviewer/badge/star.svg?theme=dark)](https://gitee.com/QQxiaoming/YUVviewer)
[![Gitee forks](https://gitee.com/QQxiaoming/YUVviewer/badge/fork.svg?theme=dark)](https://gitee.com/QQxiaoming/YUVviewer)

# YUVviewer

🇺🇸 English | [🇨🇳 简体中文](./README_zh_CN.md)

YUVviewer is a small tool for developers to view and preview various original image formats. It is based on the framework of Qt and opencv and supports windows/linux/macos. The interface is simple and clear, and the operation is portable. The main interface is as follows:

![img0](./img/docimg0.png)

## Feature

1. Select parameters on the main interface.
2. Click Open File or Folder to parse the image data and display the image.
3. Used in the image display interface
    - scroll wheel to zoom in and out of the image,
    - use the left button to drag the image,
    - Double click the left button to export the image as png format or other raw format,
    - Right click to reset image size and position,
    - Double click on the right button to swap the image R and B channel display,
    - Middle click to display the original size of the image.

## Format

Currently supported formats include:

YV12、YU12/I420、NV21、NV12、YUY2/YUYV、YVYU、UYVY、YUV444、RGB565_L、RGB565_B、BGR565_L、BGR565_B、RGB888、BayerBG、BayerGB、BayerRG、BayerGR、BayerBG_RAW12、BayerGB_RAW12、BayerRG_RAW12、BayerGR_RAW12、PNG

## Build

　> [Build documentation](./DEVELOPNOTE.md)

## Contributing

If you have suggestions or ideas for this project, please submit issues and pull requests on GitHub or Gitee.

At present, the project has been completely migrated to Qt6 development, it is recommended to use version Qt6.2.0 or higher.
