#include "tcprelay.h"

TCPRelay::TCPRelay(AbstractProtocol *p,
                   const bool _is_local,
                   QObject *parent) :
                    QObject(parent),
                    is_local(_is_local),
                    protocol(p),
                    stage(INIT)
{
    local  = new QTcpSocket(this);
    remote = new QTcpSocket(this);
    server = new QTcpServer(this);

    if(protocol)
    {
        //protocol params setting
        local_address  = protocol->getLocalAddress();
        local_port     = protocol->getLocalPort();

        // remote <--> proxy server
        remote_port    = protocol->getServerPort();
        remote_address = protocol->getServerAddress();
    }

    //bind events
    connect(server, SIGNAL(newConnection()), this, SLOT(handleLocalConnection()));
    //local socket events
    connect(local, SIGNAL(readyRead()), this, SLOT(onLocalRead()));
    //remote socket events
    connect(remote, SIGNAL(readyRead()),this, SLOT(onRemoteRead()));
}

TCPRelay::~TCPRelay()
{
    // close socket
    TCPRelay::close();

    server->deleteLater();
    local->deleteLater();
    remote->deleteLater();
}

bool TCPRelay::listen()
{
    return server->listen(local_address, local_port);
}

void TCPRelay::close()
{
    local->close();
    remote->close();

    if(server->isListening())
    {
        server->close();
    }
}

void TCPRelay::handleLocalConnection()
{
    local = server->nextPendingConnection();
}

// on local read
void TCPRelay::onLocalRead()
{
    qDebug() << "wtf local";
}

void TCPRelay::onRemoteRead()
{
    qDebug() << "wtf remote";
}
