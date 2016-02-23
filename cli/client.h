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

    bool start(Config &config);
private:
/*    QSS::Profile profile;*/
    Controller *ctl;

private slots:
   // void logHandler(const QString &);
};

#endif // CLIENT_H
