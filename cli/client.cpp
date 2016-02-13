#include "client.h"
#include "config.h"
#include "protocol/shadowsocks.h"
Client::Client(QObject *parent) : QObject(parent),ctl(nullptr)
{

}
/*
void Client::setup(const QSS::Profile &_profile)
{
    profile = _profile;
}

*/
bool Client::start(Config &config){
    //remove original controller instance
    if (ctl) {
       ctl->deleteLater();
    }

    // shadowsocks??
    QJsonObject c = config.parseConfig();

    AbstractProtocol *p = new Shadowsocks(c);
    ctl = new Controller(p);
/*
    connect(ctl, &QSS::Controller::info, this, &Client::logHandler);
    if(profile.debug){
        connect(ctl, &QSS::Controller::debug, this, &Client::logHandler);
    }

    ctl->setup(profile);
*/
    std::cout << std::endl << "[LOG] start Client: " << std::endl;

    return ctl->start();
}
/*
void Client::logHandler(const QString &log)
{
    std::cout << log.toUtf8().constData() << std::endl;
}*/
