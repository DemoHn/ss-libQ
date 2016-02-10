#include "tcprelay.h"

TcpRelay::TcpRelay(AbstractProtocol *protocol,
                   QTcpSocket *local,
                   const bool is_local,
                   QObject *parent) :

                    QObject(parent),
                    protocol(protocol),
                    local(local),
                    is_local(is_local),
                    stage(INIT)
{
    //timer = new QTimer(this);

    remote = new QTcpSocket(this);
    // connect local socket
    connect(local, SIGNAL(readyRead()),this,SLOT(onLocalRead()));
}

TcpRelay::~TcpRelay()
{

}

void TcpRelay::close()
{
    local->close();
    remote->close();
}

// on local read
void TcpRelay::onLocalRead()
{
    qDebug() << "wtf";
}

void TcpRelay::onRemoteRead()
{

}
