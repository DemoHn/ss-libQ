#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "protocol/ab_protocol.h"
#include "export.h"
#include "tcpserver.h"

class SSQ_EXPORT Controller : public QObject
{
    Q_OBJECT

public:
    enum MODE { SERVER, LOCAL, MANAGER, REDIR };

    explicit Controller(AbstractProtocol *protocol, MODE mode = LOCAL, QObject *parent = 0);
    ~Controller();

    bool start();
    void stop();

private:
    AbstractProtocol *protocol;
    MODE mode;
    TcpServer *server;
   // UdpRelay *udpRelay;

signals:

public slots:
};

#endif // CONTROLLER_H
