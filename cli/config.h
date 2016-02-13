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
    QJsonObject parseConfig();

private:
    QString filename;
};

#endif // CONFIG_H
