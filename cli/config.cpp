#include "config.h"
#include <QDebug>

Config::Config()
{
    filename = "config.json";
}

Config::Config(const QString &file)
{
    filename = file;
}

QJsonObject Config::parseConfig()
{
    QJsonObject emptyObj;
    QFile configFile(filename);
    if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "config file " << filename << " cannot open! ";
        return emptyObj;
    }

    if (!configFile.isReadable()) {
        qDebug() << "config file " << filename << " is not readable!";
        return emptyObj;
    }

    QByteArray configArray = configFile.readAll();
    configFile.close();

    QJsonParseError *error = new QJsonParseError;
    QJsonDocument confJson = QJsonDocument::fromJson(configArray, error);

    if(error->error == QJsonParseError::NoError){
        return confJson.object();
    }else{
        qDebug() << "JSON parse error: " << error->errorString();
        delete error;
        return emptyObj;
    }
}
/*
QSS::Profile & Config::getProfile()
{
    return profile;
}
*/
