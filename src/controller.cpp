#include "controller.h"

Controller::Controller(const AbstractProtocol &protocol, const MODE mode) :
    QObject(parent), server(nullptr), udpRelay(nullptr), protocol(protocol), mode(mode)
{

}

Controller::~Controller()
{

}

bool Controller::start()
{

}

void Controller::stop()
{

}
