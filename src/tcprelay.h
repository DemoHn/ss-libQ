#ifndef TCPRELAY_H
#define TCPRELAY_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QTimer>
#include "protocol/ab_protocol.h"
#include "export.h"
#include <QTcpServer>

#include "buffer.h"
#include "asyncdns.h"
#include "utils.h"
#define SOCKS5_VER 5

#define CMD_UDP_ASSOC 3
#define CMD_TCP_CONNECT 1

class SSQ_EXPORT TCPRelay : public QObject
{
    Q_OBJECT
public:
    enum STAGE {INIT, ADDR, UDP_ASSOC, DNS, CONNECTING, STREAM, DESTROYED};

    enum ATYP { IPV4 = 1, IPV6 = 4, HOST = 3};
    explicit TCPRelay(AbstractProtocol *protocol,
                      const bool is_local,
                      QObject *parent = 0);

    struct SocksHeader{
        char atyp;
        QString dest_addr;
        quint16 dest_port;
        uint header_length;
    };

    ~TCPRelay();
    bool listen();
    void close();

    void parseSocksHeader(const QByteArray &data, SocksHeader * s_header);
private:
    QTcpSocket *local;
    QTcpSocket *remote;
    QTcpServer *server;

    QByteArray send_data;

    AsyncDNS *DNSResolver;
    bool is_local;
    STAGE stage;
    AbstractProtocol *protocol;
    QTimer timer;

    QHostAddress local_address;
    QHostAddress server_address;
    quint16 local_port;
    quint16 server_port;

    void createRemoteSocket(QHostAddress address, quint16 port, QByteArray &data);
    void handleStageAddr(QByteArray &data);

    void handleStageConnecting();
    QByteArray build_connect_reply();
signals:
    //void send();
public slots:
    void handleLocalConnection();
    void onLocalRead();
    void onRemoteRead();
    void onRemoteWrite();
    void showCurrentState(QAbstractSocket::SocketState state);
};

#endif // TCPRELAY_H
