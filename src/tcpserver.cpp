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
    connect(conn, SIGNAL(readyRead()), this, SLOT(readData()));
}

void TcpServer::readData()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());

    qDebug() << socket->readAll();

    QByteArray b(10,'a');
    socket->write(b);
    socket->flush();
}

