#include "server.h"
#include "protocol/shadowsocks.h"

Server::Server(QObject *parent) : QObject(parent),ctl(nullptr)
{

}

bool Server::start(Config &config){
    //remove original controller instance
    if (ctl) {
       ctl->deleteLater();
    }

    // shadowsocks??
    QJsonObject c = config.parseConfig();

    AbstractProtocol *p = new Shadowsocks(c);
    ctl = new Controller(p, Controller::MODE::SERVER, this);

    std::cout << std::endl << "[LOG] start Server " << std::endl;

    return ctl->start();
}

