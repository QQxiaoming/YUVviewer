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
    }

    @classmethod
    def yv12(cls,yuvfilename, W, H, startframe, totalframe):
        # 从第startframe（含）开始读（0-based），共读totalframe帧
        arr = np.zeros((totalframe, H, W, 3), np.uint8)

        with open(yuvfilename, 'rb') as fp:
            seekPixels = startframe * H * W * 3 // 2
            fp.seek(8 * seekPixels)  # 跳过前startframe帧
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
            fp.seek(8 * seekPixels)  # 跳过前startframe帧
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
            fp.seek(8 * seekPixels)  # 跳过前startframe帧
            for i in range(totalframe):
                oneframe_I420 = np.zeros((H * 3 // 2, W), np.uint8)
                for j in range(H * 3 // 2):
                    for k in range(W):
                        oneframe_I420[j, k] = int.from_bytes(fp.read(1), byteorder='little', signed=False)
                oneframe_RGB = cv2.cvtColor(oneframe_I420, cv2.COLOR_YUV2RGB_NV21)
                arr[i] = oneframe_RGB
        return arr

    @classmethod
    def nv12(cls,yuvfilename, W, H, startframe, totalframe):
        # 从第startframe（含）开始读（0-based），共读totalframe帧
        arr = np.zeros((totalframe, H, W, 3), np.uint8)

        with open(yuvfilename, 'rb') as fp:
            seekPixels = startframe * H * W * 3 // 2
            fp.seek(8 * seekPixels)  # 跳过前startframe帧
            for i in range(totalframe):
                oneframe_I420 = np.zeros((H * 3 // 2, W), np.uint8)
                for j in range(H * 3 // 2):
                    for k in range(W):
                        oneframe_I420[j, k] = int.from_bytes(fp.read(1), byteorder='little', signed=False)
                oneframe_RGB = cv2.cvtColor(oneframe_I420, cv2.COLOR_YUV2RGB_NV12)
                arr[i] = oneframe_RGB
        return arr
    
    @classmethod
    def yuy2(cls,yuvfilename, W, H, startframe, totalframe):
        # 从第startframe（含）开始读（0-based），共读totalframe帧
        arr = np.zeros((totalframe, H, W, 3), np.uint8)

        with open(yuvfilename, 'rb') as fp:
            seekPixels = startframe * H * W * 2
            fp.seek(8 * seekPixels)  # 跳过前startframe帧
            for i in range(totalframe):
                oneframe_I420 = np.zeros((H * 2, W), np.uint8)
                for j in range(H * 2):
                    for k in range(W):
                        oneframe_I420[j, k] = int.from_bytes(fp.read(1), byteorder='little', signed=False)
                oneframe_RGB = cv2.cvtColor(oneframe_I420.reshape([H, W, 2]), cv2.COLOR_YUV2RGB_YUY2)
                arr[i] = oneframe_RGB
        return arr
    
    @classmethod
    def yvyu(cls,yuvfilename, W, H, startframe, totalframe):
        # 从第startframe（含）开始读（0-based），共读totalframe帧
        arr = np.zeros((totalframe, H, W, 3), np.uint8)

        with open(yuvfilename, 'rb') as fp:
            seekPixels = startframe * H * W * 2
            fp.seek(8 * seekPixels)  # 跳过前startframe帧
            for i in range(totalframe):
                oneframe_I420 = np.zeros((H * 2, W), np.uint8)
                for j in range(H * 2):
                    for k in range(W):
                        oneframe_I420[j, k] = int.from_bytes(fp.read(1), byteorder='little', signed=False)
                oneframe_RGB = cv2.cvtColor(oneframe_I420.reshape([H, W, 2]), cv2.COLOR_YUV2RGB_YVYU)
                arr[i] = oneframe_RGB
        return arr
    
    @classmethod
    def uyvy(cls,yuvfilename, W, H, startframe, totalframe):
        # 从第startframe（含）开始读（0-based），共读totalframe帧
        arr = np.zeros((totalframe, H, W, 3), np.uint8)

        with open(yuvfilename, 'rb') as fp:
            seekPixels = startframe * H * W * 2
            fp.seek(8 * seekPixels)  # 跳过前startframe帧
            for i in range(totalframe):
                oneframe_I420 = np.zeros((H * 2, W), np.uint8)
                for j in range(H * 2):
                    for k in range(W):
                        oneframe_I420[j, k] = int.from_bytes(fp.read(1), byteorder='little', signed=False)
                oneframe_RGB = cv2.cvtColor(oneframe_I420.reshape([H, W, 2]), cv2.COLOR_YUV2RGB_UYVY)
                arr[i] = oneframe_RGB
        return arr

    @classmethod
    def yuv444(cls,yuvfilename, W, H, startframe, totalframe):
        # 从第startframe（含）开始读（0-based），共读totalframe帧
        arr = np.zeros((totalframe, H, W, 3), np.uint8)

        with open(yuvfilename, 'rb') as fp:
            seekPixels = startframe * H * W * 3
            fp.seek(8 * seekPixels)  # 跳过前startframe帧
            for i in range(totalframe):
                oneframe_I420 = np.zeros((H * 3, W), np.uint8)
                for j in range(H * 3):
                    for k in range(W):
                        oneframe_I420[j, k] = int.from_bytes(fp.read(1), byteorder='little', signed=False)
                oneframe_RGB = cv2.cvtColor(oneframe_I420.reshape([H, W, 3]), cv2.COLOR_YUV2RGB)
                arr[i] = oneframe_RGB
        return arr
