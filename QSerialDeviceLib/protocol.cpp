#include "protocol.h"

#ifdef _DEBUG_PROTOCOL
#include <QDebug>
#define _MODULE_NAME "[PROTOCOL::template] -"
#endif //_DEBUG_PROTOCOL

Protocol::Protocol(QObject *parent) : QObject(parent) {
    _tagsList.clear();
}

Protocol::~Protocol() { }

QByteArray Protocol::decode(QByteArray data) {
#ifdef _DEBUG_PROTOCOL
    qDebug() << _MODULE_NAME << "decode() - Decoding: " << data << " (" << data.size() << " bytes)";
#endif //_DEBUG_PROTOCOL
    emit dataDecoded(data);
#ifdef _DEBUG_PROTOCOL
    qDebug() << _MODULE_NAME << "decode() - Decoded: " << data << " (" << data.size() << " bytes)";
#endif //_DEBUG_PROTOCOL
    return data;
}

QByteArray Protocol::encode(QByteArray data) {
#ifdef _DEBUG_PROTOCOL
    qDebug() << _MODULE_NAME << "decode() - Encoding: " << data << " (" << data.size() << " bytes)";
#endif //_DEBUG_PROTOCOL
    emit dataEncoded(data);
#ifdef _DEBUG_PROTOCOL
    qDebug() << _MODULE_NAME << "decode() - Encoded: " << data << " (" << data.size() << " bytes)";
#endif //_DEBUG_PROTOCOL
    return data;
}

QStringList Protocol::tagsList() {
#ifdef _DEBUG_PROTOCOL
    qDebug() << _MODULE_NAME << "tagsList() - " << _tagsList.count() << " elements: " << _tagsList;
#endif //_DEBUG_PROTOCOL
    return _tagsList;
}
