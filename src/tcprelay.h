#ifndef TCPRELAY_H
#define TCPRELAY_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QTimer>
#include "protocol/ab_protocol.h"
#include "export.h"
#include <QTcpServer>

class SSQ_EXPORT TCPRelay : public QObject
{
    Q_OBJECT
public:
    enum STAGE {INIT, ADDR, UDP_ASSOC, DNS, CONNECTING, STREAM, DESTROYED};

    explicit TCPRelay(AbstractProtocol *protocol,
                      const bool is_local,
                      QObject *parent = 0);

    ~TCPRelay();
    bool listen();
    void close();
private:
    QTcpSocket *local;
    QTcpSocket *remote;
    QTcpServer *server;

    bool is_local;
    STAGE stage;
    AbstractProtocol *protocol;
    QTimer timer;

    QHostAddress local_address;
    QHostAddress remote_address;
    quint16 local_port;
    quint16 remote_port;

signals:
    //void send();
public slots:
    void handleLocalConnection();
    void onLocalRead();
    void onRemoteRead();
};

#endif // TCPRELAY_H
