#ifndef CONFIG_H
#define CONFIG_H

#include <QJsonDocument>
#include <QFile>
#include <QJsonObject>
#include <sslibQ>

class Config
{
public:
    Config();
    Config(const QString &file);
    bool readConfig();
//    QSS::Profile & getProfile();
    //bool generateConfigFileModel();

private:
    QString filename;
//    QSS::Profile profile;
};

#endif // CONFIG_H
