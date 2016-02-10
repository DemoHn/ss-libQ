#ifndef TCPRELAY_H
#define TCPRELAY_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QTimer>
#include "protocol/ab_protocol.h"
#include "export.h"

class SSQ_EXPORT TcpRelay : public QObject
{
    Q_OBJECT
public:
    const static int BUF_SIZE = 1024 * 32;
    enum STAGE {INIT, ADDR, UDP_ASSOC, DNS, CONNECTING, STREAM, DESTROYED};

    explicit TcpRelay(AbstractProtocol *protocol,
                      QTcpSocket *local,
                      const bool is_local,
                      QObject *parent = 0);

    ~TcpRelay();
    void close();
private:
    QTcpSocket *local;
    QTcpSocket *remote;
    bool is_local;
    STAGE stage;
    QByteArray data;
    AbstractProtocol *protocol;
    QTimer timer;

signals:
    //void send();
public slots:
    void onLocalRead();
    void onRemoteRead();
};

#endif // TCPRELAY_H
