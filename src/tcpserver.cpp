#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);

    connect(server, SIGNAL(newConnection()) , this, SLOT(handleConnection()));
}

TcpServer::~TcpServer()
{
    if(server->isListening())
    {
        server->close();
    }

    server->deleteLater();
}

bool TcpServer::start(quint16 port)
{
    bool listen_result = server->listen(QHostAddress::Any, port);

    if(!listen_result){
        qDebug() << "listen Failed!";
    }else{
        qDebug() << "listen on port " << port;
    }

    return listen_result;
}

void TcpServer::handleConnection()
{
    QTcpSocket *conn = server->nextPendingConnection();

    connect(conn, SIGNAL(disconnected()) , conn, SLOT(deleteLater()));

    qDebug() << conn->readAll();

    QByteArray b('a',100);
    conn->write(b,b.length());
    conn->disconnectFromHost();
}

