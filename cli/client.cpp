#include "client.h"
#include "config.h"
#include "protocol/shadowsocks.h"
Client::Client(QObject *parent) : QObject(parent),ctl(nullptr)
{

}

bool Client::start(Config &config){
    //remove original controller instance
    if (ctl) {
       ctl->deleteLater();
    }

    // shadowsocks??
    QJsonObject c = config.parseConfig();

    AbstractProtocol *p = new Shadowsocks(c);
    ctl = new Controller(p);

    std::cout << std::endl << "[LOG] start Client " << std::endl;

    return ctl->start();
}
