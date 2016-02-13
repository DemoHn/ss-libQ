#include "controller.h"

Controller::Controller(AbstractProtocol *protocol, const MODE mode, QObject *parent) :
    QObject(parent), protocol(protocol), mode(mode)
{
    if(mode == CLIENT)
    {
        tcpRelay = new TCPRelay(protocol, true, this);
    }else if(mode == SERVER)
    {
        tcpRelay = new TCPRelay(protocol, false, this);
    }
}

Controller::~Controller()
{

}

bool Controller::start()
{
    return tcpRelay->listen();
}

void Controller::stop()
{
    tcpRelay->close();
}
