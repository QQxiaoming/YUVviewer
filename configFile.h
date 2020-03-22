#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <QString>

typedef struct
{
    QString frameSizeType;
    QString YUVFormat;
    QString frameSize_Width;
    QString frameSize_Height;
    QString frameRate;
    QString startFrame;
    QString endFrame;
}config_dict_t;

class ConfigFile
{
public:
    ConfigFile(QString path);
    ~ConfigFile();
    config_dict_t config_dict;
private:
    QString configFilePath;
};

#endif // CONFIGFILE_H
