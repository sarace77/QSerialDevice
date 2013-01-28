#ifndef PROTOCOL_ASCII_H
#define PROTOCOL_ASCII_H

#include "protocol.h"

class Protocol_ASCII : public Protocol
{

public:
    Protocol_ASCII(QObject *parent = 0);

public slots:
    QByteArray decode(QByteArray data);
    QByteArray encode(QByteArray data);
};

#endif // PROTOCOL_ASCII_H
