#ifndef TCPRELAY_H
#define TCPRELAY_H

#include <QObject>

class TcpRelay : public QObject
{
    Q_OBJECT
public:
    explicit TcpRelay(QObject *parent = 0);

signals:

public slots:
};

#endif // TCPRELAY_H
