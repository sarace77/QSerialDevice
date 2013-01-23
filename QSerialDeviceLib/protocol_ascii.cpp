#include "protocol_ascii.h"

#ifdef _DEBUG_PROTOCOL
#include <QDebug>
#define _MODULE_NAME "[PROTOCOL_ASCII] - "
#endif //_DEBUG_PROTOCOL


Protocol_ASCII::Protocol_ASCII() {
    _tagList.append("[NUL]");
    _tagList.append("[SOH]");
    _tagList.append("[STX]");
    _tagList.append("[ETX]");
    _tagList.append("[EOT]");
    _tagList.append("[ENQ]");
    _tagList.append("[ACK]");
    _tagList.append("[BEL]");
    _tagList.append("[BS]");
    _tagList.append("[HT]");
    _tagList.append("[NL]");
    _tagList.append("[VT]");
    _tagList.append("[NP]");
    _tagList.append("[CR]");
    _tagList.append("[SO]");
    _tagList.append("[SI]");
    _tagList.append("[DLE]");
    _tagList.append("[DC1]");
    _tagList.append("[DC2]");
    _tagList.append("[DC3]");
    _tagList.append("[DC4]");
    _tagList.append("[NAK]");
    _tagList.append("[SYN]");
    _tagList.append("[ETB]");
    _tagList.append("[CAN]");
    _tagList.append("[EM]");
    _tagList.append("[SUB]");
    _tagList.append("[ESC]");
    _tagList.append("[FS]");
    _tagList.append("[GS]");
    _tagList.append("[RS]");
    _tagList.append("[US]");
    _tagList.append("[SP]");
    _tagList.append("[DEL]");
#ifdef _DEBUG_PROTOCOL
    qDebug() << _MODULE_NAME << "Protocol_ASCII() - Supported Tags : " << _tagList;
#endif //_DEBUG_PROTOCOL
}

QByteArray Protocol_ASCII::decode(QByteArray data) {
    QByteArray dataDecoded;
    for (qint64 i = 0; i < data.size(); i++) {
        switch ((asciiValueType) data.at(i)) {
        case NUL:
            dataDecoded.append("[NUL]");
            break;
        case SOH:
            dataDecoded.append("[SOH]");
            break;
        case STX:
            dataDecoded.append("[STX]");
            break;
        case ETX:
            dataDecoded.append("[ETX]");
            break;
        case EOT:
            dataDecoded.append("[EOT]");
            break;
        case ENQ:
            dataDecoded.append("[ENQ]");
            break;
        case ACK:
            dataDecoded.append("[ACK]");
            break;
        case BEL:
            dataDecoded.append("[BEL]");
            break;
        case BS:
            dataDecoded.append("[BS]");
            break;
        case HT:
            dataDecoded.append("[HT]");
            break;
        case NL:
            dataDecoded.append("[NL]");
            break;
        case VT:
            dataDecoded.append("[VT]");
            break;
        case NP:
            dataDecoded.append("[NP]");
            break;
        case CR:
            dataDecoded.append("[CR]");
            break;
        case SO:
            dataDecoded.append("[SO]");
            break;
        case SI:
            dataDecoded.append("[SI]");
            break;
        case DLE:
            dataDecoded.append("[DLE]");
            break;
        case DC1:
            dataDecoded.append("[DC1]");
            break;
        case DC2:
            dataDecoded.append("[DC2]");
            break;
        case DC3:
            dataDecoded.append("[DC3]");
            break;
        case DC4:
            dataDecoded.append("[DC4]");
            break;
        case NAK:
            dataDecoded.append("[NAK]");
            break;
        case SYN:
            dataDecoded.append("[SYN]");
            break;
        case ETB:
            dataDecoded.append("[ETB]");
            break;
        case CAN:
            dataDecoded.append("[CAN]");
            break;
        case EM:
            dataDecoded.append("[EM]");
            break;
        case SUB:
            dataDecoded.append("[SUB]");
            break;
        case ESC:
            dataDecoded.append("[ESC]");
            break;
        case FS:
            dataDecoded.append("[FS]");
            break;
        case GS:
            dataDecoded.append("[GS]");
            break;
        case RS:
            dataDecoded.append("[RS]");
            break;
        case US:
            dataDecoded.append("[US]");
            break;
        case SP:
            dataDecoded.append("[SP]");
            break;
        case DEL:
            dataDecoded.append("[DEL]");
            break;
        default:
            dataDecoded.append(data.at(i));
            break;
        }
    }
    return dataDecoded;
}

QByteArray Protocol_ASCII::encode(QByteArray data) {
    QByteArray dataEncoded;
    for(qint64 i = 0; i < data.size(); i++) {
        if (data.at(i) == '[') {
            QString sTag = QString(data.at(i++));
            while(data.at(i) != ']' && i < data.size()) {
                sTag += QString(data.at(i++));
            }
            sTag += QString(data.at(i));
            if (_tagList.contains(sTag)) {
                char eChar = (char) _tagList.indexOf(sTag) != 33 ? _tagList.indexOf(sTag) : 0x7f;
                qDebug() << _MODULE_NAME << "encode() - Encoded" << sTag << " in" << (int) eChar;
                dataEncoded.append(eChar);

            }
        } else {
            dataEncoded.append(data.at(i));
        }
    }
    return dataEncoded;
}
