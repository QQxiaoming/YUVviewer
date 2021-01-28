# -*- coding: utf-8 -*-
import cv2
import numpy as np

class YUV2RGB(object):
    YUVdecoder_dict = {
        'YV12'      : lambda yuvfilename, W, H, startframe, totalframe: YUV2RGB.yv12(yuvfilename, W, H, startframe, totalframe),
        'YU12/I420' : lambda yuvfilename, W, H, startframe, totalframe: YUV2RGB.i420(yuvfilename, W, H, startframe, totalframe),
        'NV21'      : lambda yuvfilename, W, H, startframe, totalframe: YUV2RGB.nv21(yuvfilename, W, H, startframe, totalframe),
        'NV12'      : lambda yuvfilename, W, H, startframe, totalframe: YUV2RGB.nv12(yuvfilename, W, H, startframe, totalframe),
        'YUY2/YUYV' : lambda yuvfilename, W, H, startframe, totalframe: YUV2RGB.yuy2(yuvfilename, W, H, startframe, totalframe),
        'YVYU'      : lambda yuvfilename, W, H, startframe, totalframe: YUV2RGB.yvyu(yuvfilename, W, H, startframe, totalframe),
        'UYVY'      : lambda yuvfilename, W, H, startframe, totalframe: YUV2RGB.uyvy(yuvfilename, W, H, startframe, totalframe),
        '4:4:4'     : lambda yuvfilename, W, H, startframe, totalframe: YUV2RGB.yuv444(yuvfilename, W, H, startframe, totalframe),
        'RGB565_L'  : lambda yuvfilename, W, H, startframe, totalframe: YUV2RGB.rgb565_little_endian(yuvfilename, W, H, startframe, totalframe),
        'RGB565_B'  : lambda yuvfilename, W, H, startframe, totalframe: YUV2RGB.rgb565_big_endian(yuvfilename, W, H, startframe, totalframe),
        'BGR565_L'  : lambda yuvfilename, W, H, startframe, totalframe: YUV2RGB.bgr565_little_endian(yuvfilename, W, H, startframe, totalframe),
        'BGR565_B'  : lambda yuvfilename, W, H, startframe, totalframe: YUV2RGB.bgr565_big_endian(yuvfilename, W, H, startframe, totalframe),
    }

    @classmethod
    def yv12(cls,yuvfilename, W, H, startframe, totalframe):
        # 从第startframe（含）开始读（0-based），共读totalframe帧
        arr = np.zeros((totalframe, H, W, 3), np.uint8)

        with open(yuvfilename, 'rb') as fp:
            seekPixels = startframe * H * W * 3 // 2
            fp.seek(seekPixels)  # 跳过前startframe帧
            for i in range(totalframe):
                oneframe_YV12 = np.zeros((H * 3 // 2, W), np.uint8)
                for j in range(H * 3 // 2):
                    for k in range(W):
                        oneframe_YV12[j, k] = int.from_bytes(fp.read(1), byteorder='little', signed=False)
                oneframe_RGB = cv2.cvtColor(oneframe_YV12, cv2.COLOR_YUV2RGB_YV12)
                arr[i] = oneframe_RGB
        return arr

    @classmethod
    def i420(cls,yuvfilename, W, H, startframe, totalframe):
        # 从第startframe（含）开始读（0-based），共读totalframe帧
        arr = np.zeros((totalframe, H, W, 3), np.uint8)

        with open(yuvfilename, 'rb') as fp:
            seekPixels = startframe * H * W * 3 // 2
            fp.seek(seekPixels)  # 跳过前startframe帧
            for i in range(totalframe):
                oneframe_I420 = np.zeros((H * 3 // 2, W), np.uint8)
                for j in range(H * 3 // 2):
                    for k in range(W):
                        oneframe_I420[j, k] = int.from_bytes(fp.read(1), byteorder='little', signed=False)
                oneframe_RGB = cv2.cvtColor(oneframe_I420, cv2.COLOR_YUV2RGB_I420)
                arr[i] = oneframe_RGB
        return arr

    @classmethod
    def nv21(cls,yuvfilename, W, H, startframe, totalframe):
        # 从第startframe（含）开始读（0-based），共读totalframe帧
        arr = np.zeros((totalframe, H, W, 3), np.uint8)

        with open(yuvfilename, 'rb') as fp:
            seekPixels = startframe * H * W * 3 // 2
            fp.seek(seekPixels)  # 跳过前startframe帧
            for i in range(totalframe):
                oneframe_NV21 = np.zeros((H * 3 // 2, W), np.uint8)
                for j in range(H * 3 // 2):
                    for k in range(W):
                        oneframe_NV21[j, k] = int.from_bytes(fp.read(1), byteorder='little', signed=False)
                oneframe_RGB = cv2.cvtColor(oneframe_NV21, cv2.COLOR_YUV2RGB_NV21)
                arr[i] = oneframe_RGB
        return arr

    @classmethod
    def nv12(cls,yuvfilename, W, H, startframe, totalframe):
        # 从第startframe（含）开始读（0-based），共读totalframe帧
        arr = np.zeros((totalframe, H, W, 3), np.uint8)

        with open(yuvfilename, 'rb') as fp:
            seekPixels = startframe * H * W * 3 // 2
            fp.seek(seekPixels)  # 跳过前startframe帧
            for i in range(totalframe):
                oneframe_NV12 = np.zeros((H * 3 // 2, W), np.uint8)
                for j in range(H * 3 // 2):
                    for k in range(W):
                        oneframe_NV12[j, k] = int.from_bytes(fp.read(1), byteorder='little', signed=False)
                oneframe_RGB = cv2.cvtColor(oneframe_NV12, cv2.COLOR_YUV2RGB_NV12)
                arr[i] = oneframe_RGB
        return arr
    
    @classmethod
    def yuy2(cls,yuvfilename, W, H, startframe, totalframe):
        # 从第startframe（含）开始读（0-based），共读totalframe帧
        arr = np.zeros((totalframe, H, W, 3), np.uint8)

        with open(yuvfilename, 'rb') as fp:
            seekPixels = startframe * H * W * 2
            fp.seek(seekPixels)  # 跳过前startframe帧
            for i in range(totalframe):
                oneframe_YUV2 = np.zeros((H, W, 2), np.uint8)
                for j in range(H):
                    for k in range(W):
                        for l in range(2):
                            oneframe_YUV2[j, k, l] = int.from_bytes(fp.read(1), byteorder='little', signed=False)
                oneframe_RGB = cv2.cvtColor(oneframe_YUV2, cv2.COLOR_YUV2RGB_YUY2)
                arr[i] = oneframe_RGB
        return arr
    
    @classmethod
    def yvyu(cls,yuvfilename, W, H, startframe, totalframe):
        # 从第startframe（含）开始读（0-based），共读totalframe帧
        arr = np.zeros((totalframe, H, W, 3), np.uint8)

        with open(yuvfilename, 'rb') as fp:
            seekPixels = startframe * H * W * 2
            fp.seek(seekPixels)  # 跳过前startframe帧
            for i in range(totalframe):
                oneframe_YVYU = np.zeros((H, W, 2), np.uint8)
                for j in range(H):
                    for k in range(W):
                        for l in range(2):
                            oneframe_YVYU[j, k, l] = int.from_bytes(fp.read(1), byteorder='little', signed=False)
                oneframe_RGB = cv2.cvtColor(oneframe_YVYU, cv2.COLOR_YUV2RGB_YVYU)
                arr[i] = oneframe_RGB
        return arr
    
    @classmethod
    def uyvy(cls,yuvfilename, W, H, startframe, totalframe):
        # 从第startframe（含）开始读（0-based），共读totalframe帧
        arr = np.zeros((totalframe, H, W, 3), np.uint8)

        with open(yuvfilename, 'rb') as fp:
            seekPixels = startframe * H * W * 2
            fp.seek(seekPixels)  # 跳过前startframe帧
            for i in range(totalframe):
                oneframe_UYVY = np.zeros((H, W, 2), np.uint8)
                for j in range(H):
                    for k in range(W):
                        for l in range(2):
                            oneframe_UYVY[j, k, l] = int.from_bytes(fp.read(1), byteorder='little', signed=False)
                oneframe_RGB = cv2.cvtColor(oneframe_UYVY, cv2.COLOR_YUV2RGB_UYVY)
                arr[i] = oneframe_RGB
        return arr

    @classmethod
    def yuv444(cls,yuvfilename, W, H, startframe, totalframe):
        # 从第startframe（含）开始读（0-based），共读totalframe帧
        arr = np.zeros((totalframe, H, W, 3), np.uint8)

        with open(yuvfilename, 'rb') as fp:
            seekPixels = startframe * H * W * 3
            fp.seek(seekPixels)  # 跳过前startframe帧
            for i in range(totalframe):
                oneframe_YUV444 = np.zeros((H, W, 3), np.uint8)
                for j in range(H):
                    for k in range(W):
                        for l in range(3):
                            oneframe_YUV444[j, k, l] = int.from_bytes(fp.read(1), byteorder='little', signed=False)
                oneframe_RGB = cv2.cvtColor(oneframe_YUV444, cv2.COLOR_YUV2RGB)
                arr[i] = oneframe_RGB
        return arr

    @classmethod
    def rgb565_little_endian(cls,yuvfilename, W, H, startframe, totalframe):
        # 从第startframe（含）开始读（0-based），共读totalframe帧
        arr = np.zeros((totalframe, H, W, 3), np.uint8)

        with open(yuvfilename, 'rb') as fp:
            seekPixels = startframe * H * W * 2
            fp.seek(seekPixels)  # 跳过前startframe帧
            for i in range(totalframe):
                oneframe_RGB565 = np.zeros((H, W, 2), np.uint8)
                for j in range(H):
                    for k in range(W):
                        for l in range(2):
                            oneframe_RGB565[j, k, l] = int.from_bytes(fp.read(1), byteorder='little', signed=False)
                oneframe_BGR = cv2.cvtColor(oneframe_RGB565, cv2.COLOR_BGR5652RGB)
                oneframe_RGB = cv2.cvtColor(oneframe_BGR, cv2.COLOR_BGR2RGB)
                arr[i] = oneframe_RGB
        return arr

    @classmethod
    def rgb565_big_endian(cls,yuvfilename, W, H, startframe, totalframe):
        # 从第startframe（含）开始读（0-based），共读totalframe帧
        arr = np.zeros((totalframe, H, W, 3), np.uint8)

        with open(yuvfilename, 'rb') as fp:
            seekPixels = startframe * H * W * 2
            fp.seek(seekPixels)  # 跳过前startframe帧
            for i in range(totalframe):
                oneframe_RGB565 = np.zeros((H, W, 2), np.uint8)
                for j in range(H):
                    for k in range(W):
                        oneframe_RGB565[j, k, 1] = int.from_bytes(fp.read(1), byteorder='little', signed=False)
                        oneframe_RGB565[j, k, 0] = int.from_bytes(fp.read(1), byteorder='little', signed=False)
                oneframe_BGR = cv2.cvtColor(oneframe_RGB565, cv2.COLOR_BGR5652RGB)
                oneframe_RGB = cv2.cvtColor(oneframe_BGR, cv2.COLOR_BGR2RGB)
                arr[i] = oneframe_RGB
        return arr

    @classmethod
    def bgr565_little_endian(cls,yuvfilename, W, H, startframe, totalframe):
        # 从第startframe（含）开始读（0-based），共读totalframe帧
        arr = np.zeros((totalframe, H, W, 3), np.uint8)

        with open(yuvfilename, 'rb') as fp:
            seekPixels = startframe * H * W * 2
            fp.seek(seekPixels)  # 跳过前startframe帧
            for i in range(totalframe):
                oneframe_BGR565 = np.zeros((H, W, 2), np.uint8)
                for j in range(H):
                    for k in range(W):
                        for l in range(2):
                            oneframe_BGR565[j, k, l] = int.from_bytes(fp.read(1), byteorder='little', signed=False)
                oneframe_RGB = cv2.cvtColor(oneframe_BGR565, cv2.COLOR_BGR5652RGB)
                arr[i] = oneframe_RGB
        return arr

    @classmethod
    def bgr565_big_endian(cls,yuvfilename, W, H, startframe, totalframe):
        # 从第startframe（含）开始读（0-based），共读totalframe帧
        arr = np.zeros((totalframe, H, W, 3), np.uint8)

        with open(yuvfilename, 'rb') as fp:
            seekPixels = startframe * H * W * 2
            fp.seek(seekPixels)  # 跳过前startframe帧
            for i in range(totalframe):
                oneframe_BGR565 = np.zeros((H, W, 2), np.uint8)
                for j in range(H):
                    for k in range(W):
                        oneframe_BGR565[j, k, 1] = int.from_bytes(fp.read(1), byteorder='little', signed=False)
                        oneframe_BGR565[j, k, 0] = int.from_bytes(fp.read(1), byteorder='little', signed=False)
                oneframe_RGB = cv2.cvtColor(oneframe_BGR565, cv2.COLOR_BGR5652RGB)
                arr[i] = oneframe_RGB
        return arr
