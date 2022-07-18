/**
 * @file configFile.h
 * @brief 管理配置文件
 * @version 1.0
 * @date 2020-04-14
 */
#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <QString>

typedef struct {
    QString lastPath;
    QString frameSizeType;
    QString YUVFormat;
    QString frameSize_Width;
    QString frameSize_Height;
    QString frameRate;
    QString startFrame;
    QString endFrame;
}config_dict_t;

class ConfigFile {
public:
    ConfigFile(QString path);
    ~ConfigFile();
    config_dict_t config_dict;

private:
    QString configFilePath;
};

#endif // CONFIGFILE_H
