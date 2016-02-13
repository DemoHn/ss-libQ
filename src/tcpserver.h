#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include "export.h"

class SSQ_EXPORT TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);
    ~TcpServer();

    bool start(quint16 port);
private:
    QTcpServer *server;

protected:
//    void incomingConnection(qintptr fd);

signals:    

public slots:
    void handleConnection();
    void readData();
};

#endif // TCPSERVER_H
