# -*- coding: utf-8 -*-
import cv2
import numpy as np

class YUV2RGB(object):
    YUVdecoder_dict = {
        'YV12' : lambda yuvfilename, W, H, startframe, totalframe: YUV2RGB.yv12(yuvfilename, W, H, startframe, totalframe),
        'I420' : lambda yuvfilename, W, H, startframe, totalframe: YUV2RGB.i420(yuvfilename, W, H, startframe, totalframe),
        'YUY2' : None, # 暂未支持该格式
        'UYUV' : None, # 暂未支持该格式
        '4:2:2': None, # 暂未支持该格式
        '4:4:4': None, # 暂未支持该格式
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
