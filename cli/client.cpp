#include "client.h"
#include <QObject>

Client::Client(QObject *parent) : QObject(parent),ctl(nullptr)
{

}
/*
void Client::setup(const QSS::Profile &_profile)
{
    profile = _profile;
}

void Client::setup(const QString &remote_addr,
               const quint16 &remote_port,
               const QString &local_addr,
               const quint16 &local_port,
               const QString &password,
               const QString &method,
               const int &timeout,
               const bool http_proxy,
               const bool debug,
               const bool auth)
{
    profile.server        = remote_addr;
    profile.server_port   = remote_port;
    profile.local_address = local_addr;
    profile.local_port    = local_port;
    profile.password      = password;
    profile.method        = method;
    profile.timeout       = timeout;
    profile.http_proxy    = http_proxy;
    profile.debug         = debug;
    profile.auth          = auth;
}

void Client::setHttpMode(bool http){
    profile.http_proxy = http;
}

void Client::setDebugMode(bool debug){
    profile.debug = debug;
}

QString Client::getMethod(){
    return profile.method;
}
*/
bool Client::start(){
    //remove original controller instance
    if (ctl) {
       ctl->deleteLater();
    }

    ctl = new Controller(nullptr);
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
