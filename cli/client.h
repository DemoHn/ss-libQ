#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <sslibQ>

#include "config.h"
class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
/*    void setup(const QSS::Profile &_profile);
    void setup(const QString &remote_addr,
           const quint16 &remote_port,
           const QString &local_addr,
           const quint16 &local_port,
           const QString &password,
           const QString &method,
           const int &timeout,
           const bool http_proxy = false,
           const bool debug = false,
           const bool auth = false);

    void setHttpMode(bool http);
    void setDebugMode(bool debug);
    QString getMethod();
*/
    //run method
    bool start(Config &config);
private:
/*    QSS::Profile profile;*/
    Controller *ctl;

private slots:
   // void logHandler(const QString &);
};

#endif // CLIENT_H
