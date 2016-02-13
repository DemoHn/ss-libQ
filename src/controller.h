#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "protocol/ab_protocol.h"
#include "export.h"
#include "tcprelay.h"

class SSQ_EXPORT Controller : public QObject
{
    Q_OBJECT

public:
    enum MODE { SERVER, CLIENT, MANAGER, REDIR };

    explicit Controller(AbstractProtocol *protocol, MODE mode = CLIENT, QObject *parent = 0);
    ~Controller();

    bool start();
    void stop();

private:
    AbstractProtocol *protocol;
    MODE mode;
    TCPRelay *tcpRelay;
   // UdpRelay *udpRelay;

signals:

public slots:
};

#endif // CONTROLLER_H
