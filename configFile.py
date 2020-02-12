import sys
import os
from xml.etree import ElementTree
import xml.etree.ElementTree as etree
from xml.etree.ElementTree import SubElement
import xml.etree.ElementTree as ET

yuvviewer_xml_head = '''<?xml version="1.0" encoding="UTF-8"?>'''

class ConfigFile(object):
    def __init__(self, path):
        self.config_dict = {
            'frameSizeType': 'Other',
            'YUVFormat': 'YV12',
            'frameSize_Width': '2528',
            'frameSize_Height': '1080',
            'frameRate': '30',
            'startFrame': '0',
            'endFrame': '0',
        }
        self.configFilePath = path
        if not os.path.exists(path):
            configFile = open(path, 'a+', encoding='utf-8')

            configFile.write(yuvviewer_xml_head)
            configFile.write('\r\n')

            xml_file = ET.Element('config')

            for key, value in self.config_dict.items():
                xml_fileVersion = ET.SubElement(xml_file, key)
                xml_fileVersion.text = value

            self._xml_indent(xml_file)
            configFile.write(etree.tostring(xml_file, encoding='utf-8').decode())
            configFile.close()
        else:
            xml_file_tree = ElementTree.parse(path)
            xml_file = xml_file_tree.getroot()
            for key in self.config_dict:
                self.config_dict[key] = xml_file.find(key).text

    def __del__(self):
        xml_file_tree = ElementTree.parse(self.configFilePath)
        xml_file = xml_file_tree.getroot()
        for key in self.config_dict:
            xml_file.find(key).text = self.config_dict[key]

        configFile = open(self.configFilePath, 'w', encoding='utf-8')
        configFile.write(yuvviewer_xml_head)
        configFile.write('\r\n')
        self._xml_indent(xml_file)
        configFile.write(etree.tostring(xml_file, encoding='utf-8').decode())
        configFile.close()


    def _xml_indent(self, elem, num=4, level=0):
        """xml文件输出增加换行

        Arguments:
            elem {obj} -- xml构造器

        Keyword Arguments:
            num {int} -- 换行缩进大小 (default: {4})
            level {int} -- 缩进级别 (default: {0})
        """
        i = "\n" + num * level * " "
        if len(elem):
            if not elem.text or not elem.text.strip():
                elem.text = i + num * " "
            if not elem.tail or not elem.tail.strip():
                elem.tail = i
            for elem in elem:
                self._xml_indent(elem, num, level + 1)
            if not elem.tail or not elem.tail.strip():
                elem.tail = i
        else:
            if level and (not elem.tail or not elem.tail.strip()):
                elem.tail = i
