#include "controller.h"

Controller::Controller(AbstractProtocol *protocol, const MODE mode, QObject *parent) :
    QObject(parent), server(nullptr), protocol(protocol), mode(mode)
{

}

Controller::~Controller()
{

}

bool Controller::start()
{
    server = new TcpServer(this);
    return server->start(1234);
}

void Controller::stop()
{

}
