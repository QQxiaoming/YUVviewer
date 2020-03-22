#include <QDir>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "configFile.h"

ConfigFile::ConfigFile(QString path)
{
    config_dict.frameSizeType = "Other";
    config_dict.YUVFormat = "YV12";
    config_dict.frameSize_Width = "2528";
    config_dict.frameSize_Height = "1080";
    config_dict.frameRate = "30";
    config_dict.startFrame = "0";
    config_dict.endFrame = "0";

    configFilePath = path;

    QFileInfo fileInfo(configFilePath);
    if(!fileInfo.isFile())
    {
        QFile file(configFilePath);
        file.open(QFile::WriteOnly | QFile::Text);
        QXmlStreamWriter writer(&file);
        writer.setCodec("UTF-8");  // XML 编码
        writer.setAutoFormatting(true); // 自动格式化
        writer.writeStartDocument("1.0");  // 开始文档（XML 声明）
        writer.writeStartElement("config");
        writer.writeTextElement("frameSizeType", config_dict.frameSizeType);
        writer.writeTextElement("YUVFormat", config_dict.YUVFormat);
        writer.writeTextElement("frameSize_Width", config_dict.frameSize_Width);
        writer.writeTextElement("frameSize_Height", config_dict.frameSize_Height);
        writer.writeTextElement("frameRate", config_dict.frameRate);
        writer.writeTextElement("startFrame", config_dict.startFrame);
        writer.writeTextElement("endFrame", config_dict.endFrame);
        writer.writeEndElement();
        writer.writeEndDocument();
        file.close();
    }
    else
    {
        QFile file(configFilePath);
        file.open(QFile::ReadOnly | QFile::Text);
        QXmlStreamReader reader(&file);

        while(!reader.atEnd())
        {
            if(reader.isStartElement())
            {
                if(reader.name() == "frameSizeType")
                {
                    config_dict.frameSizeType = reader.readElementText();
                }
                else if(reader.name() == "YUVFormat")
                {
                    config_dict.YUVFormat = reader.readElementText();
                }
                else if(reader.name() == "frameSize_Width")
                {
                    config_dict.frameSize_Width = reader.readElementText();
                }
                else if(reader.name() == "frameSize_Height")
                {
                    config_dict.frameSize_Height = reader.readElementText();
                }
                else if(reader.name() == "frameRate")
                {
                    config_dict.frameRate = reader.readElementText();
                }
                else if(reader.name() == "startFrame")
                {
                    config_dict.startFrame = reader.readElementText();
                }
                else if(reader.name() == "endFrame")
                {
                    config_dict.endFrame = reader.readElementText();
                }
            }
            reader.readNext();
        }
        file.close();
    }
}


ConfigFile::~ConfigFile()
{
    QFile file(configFilePath);
    file.open(QFile::ReadWrite | QFile::Text);

    QXmlStreamWriter writer(&file);
    writer.setCodec("UTF-8");  // XML 编码
    writer.setAutoFormatting(true); // 自动格式化
    writer.writeStartDocument("1.0");  // 开始文档（XML 声明）
    writer.writeStartElement("config");
    writer.writeTextElement("frameSizeType", config_dict.frameSizeType);
    writer.writeTextElement("YUVFormat", config_dict.YUVFormat);
    writer.writeTextElement("frameSize_Width", config_dict.frameSize_Width);
    writer.writeTextElement("frameSize_Height", config_dict.frameSize_Height);
    writer.writeTextElement("frameRate", config_dict.frameRate);
    writer.writeTextElement("startFrame", config_dict.startFrame);
    writer.writeTextElement("endFrame", config_dict.endFrame);
    writer.writeEndElement();
    writer.writeEndDocument();
    file.close();
}
