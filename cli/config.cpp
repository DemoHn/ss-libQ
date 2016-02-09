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

bool Config::readConfig()
{
    QFile configFile(filename);
    if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "config file " << filename << " cannot open! ";
        return false;
    }

    if (!configFile.isReadable()) {
        qDebug() << "config file " << filename << " is not readable!";
        return false;
    }

    QByteArray configArray = configFile.readAll();
    configFile.close();

    QJsonParseError *error = new QJsonParseError;
    QJsonDocument confJson = QJsonDocument::fromJson(configArray,error);

    if(error->error == QJsonParseError::NoError){
        QJsonObject confObj = confJson.object();

        profile.local_address = confObj["local_address"].toString();
        profile.local_port    = confObj["local_port"].toInt();
        profile.method        = confObj["method"].toString();
        profile.password      = confObj["password"].toString();
        profile.server        = confObj["server"].toString();
        profile.server_port   = confObj["server_port"].toInt();
        profile.timeout       = confObj["timeout"].toInt();
        profile.http_proxy    = false;

        return true;
    }else{
        qDebug() << "JSON parse error: " << error->errorString();
        delete error;
        return false;
    }
}

QSS::Profile & Config::getProfile()
{
    return profile;
}
