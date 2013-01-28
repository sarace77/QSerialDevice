#include "protocol_ascii.h"

#ifdef _DEBUG_PROTOCOL
#include <QDebug>
#define _MODULE_NAME "[PROTOCOL::ASCII] -"
#endif //_DEBUG_PROTOCOL

enum asciiValueType {
    NUL = 0x00, // NULl
    SOH = 0x01, // Start Of Heading
    STX = 0x02, // Start Of Text
    ETX = 0x03, // End Of Text
    EOT = 0x04, // End Of Transmission
    ENQ = 0x05, // ENQuiry
    ACK = 0x06, // ACKnowledge
    BEL = 0x07, // BELl
    BS  = 0x08, // BackSpace
    HT  = 0x09, // Horizontal Tab
    NL  = 0x0a, // New Line (or LF, Line Feed)
    VT  = 0x0b, // Vertical Tab
    NP  = 0x0c, // New Page (or FF, Form Feed)
    CR  = 0x0d, // Carriage Return
    SO  = 0x0e, // Shift Out
    SI  = 0x0f, // Shift In
    DLE = 0x10, // Data Link Escape
    DC1 = 0x11, // Device Control 1
    DC2 = 0x12, // Device Control 2
    DC3 = 0x13, // Device Control 3
    DC4 = 0x14, // Device Control 4
    NAK = 0x15, // Negative AcKnowledge
    SYN = 0x16, // SYNchronous idle
    ETB = 0x17, // End of Transmission Block
    CAN = 0x18, // CANcel
    EM  = 0x19, // End of Medium
    SUB = 0x1a, // SUBstitute
    ESC = 0x1b, // ESCape
    FS  = 0x1c, // File Separator
    GS  = 0x1d, // Group Separator
    RS  = 0x1e, // Record Separator
    US  = 0x1f, // Unit Separator
    SP  = 0x20, // SPace
    DEL = 0x7f  // DELete
};

Protocol_ASCII::Protocol_ASCII(QObject *parent): Protocol(parent)  {
    _tagsList.append("[NUL]");
    _tagsList.append("[SOH]");
    _tagsList.append("[STX]");
    _tagsList.append("[ETX]");
    _tagsList.append("[EOT]");
    _tagsList.append("[ENQ]");
    _tagsList.append("[ACK]");
    _tagsList.append("[BEL]");
    _tagsList.append("[BS]");
    _tagsList.append("[HT]");
    _tagsList.append("[NL]");
    _tagsList.append("[VT]");
    _tagsList.append("[NP]");
    _tagsList.append("[CR]");
    _tagsList.append("[SO]");
    _tagsList.append("[SI]");
    _tagsList.append("[DLE]");
    _tagsList.append("[DC1]");
    _tagsList.append("[DC2]");
    _tagsList.append("[DC3]");
    _tagsList.append("[DC4]");
    _tagsList.append("[NAK]");
    _tagsList.append("[SYN]");
    _tagsList.append("[ETB]");
    _tagsList.append("[CAN]");
    _tagsList.append("[EM]");
    _tagsList.append("[SUB]");
    _tagsList.append("[ESC]");
    _tagsList.append("[FS]");
    _tagsList.append("[GS]");
    _tagsList.append("[RS]");
    _tagsList.append("[US]");
    _tagsList.append("[SP]");
    _tagsList.append("[DEL]");
#ifdef _DEBUG_PROTOCOL
    qDebug() << _MODULE_NAME << "Protocol_ASCII() - Supported Tags : " << _tagsList;
#endif //_DEBUG_PROTOCOL
}

QByteArray Protocol_ASCII::decode(QByteArray data) {
#ifdef _DEBUG_PROTOCOL
    qDebug() << _MODULE_NAME << "decode() - Decoding: " << data << " (" << data.size() << " bytes)";
#endif //_DEBUG_PROTOCOL
    QByteArray buffer = data;
    for (qint64 i = 0; i < data.size(); i++) {
        switch ((asciiValueType) data.at(i)) {
        case NUL:
            buffer.append("[NUL]");
            break;
        case SOH:
            buffer.append("[SOH]");
            break;
        case STX:
            buffer.append("[STX]");
            break;
        case ETX:
            buffer.append("[ETX]");
            break;
        case EOT:
            buffer.append("[EOT]");
            break;
        case ENQ:
            buffer.append("[ENQ]");
            break;
        case ACK:
            buffer.append("[ACK]");
            break;
        case BEL:
            buffer.append("[BEL]");
            break;
        case BS:
            buffer.append("[BS]");
            break;
        case HT:
            buffer.append("[HT]");
            break;
        case NL:
            buffer.append("[NL]");
            break;
        case VT:
            buffer.append("[VT]");
            break;
        case NP:
            buffer.append("[NP]");
            break;
        case CR:
            buffer.append("[CR]");
            break;
        case SO:
            buffer.append("[SO]");
            break;
        case SI:
            buffer.append("[SI]");
            break;
        case DLE:
            buffer.append("[DLE]");
            break;
        case DC1:
            buffer.append("[DC1]");
            break;
        case DC2:
            buffer.append("[DC2]");
            break;
        case DC3:
            buffer.append("[DC3]");
            break;
        case DC4:
            buffer.append("[DC4]");
            break;
        case NAK:
            buffer.append("[NAK]");
            break;
        case SYN:
            buffer.append("[SYN]");
            break;
        case ETB:
            buffer.append("[ETB]");
            break;
        case CAN:
            buffer.append("[CAN]");
            break;
        case EM:
            buffer.append("[EM]");
            break;
        case SUB:
            buffer.append("[SUB]");
            break;
        case ESC:
            buffer.append("[ESC]");
            break;
        case FS:
            buffer.append("[FS]");
            break;
        case GS:
            buffer.append("[GS]");
            break;
        case RS:
            buffer.append("[RS]");
            break;
        case US:
            buffer.append("[US]");
            break;
        case SP:
            buffer.append("[SP]");
            break;
        case DEL:
            buffer.append("[DEL]");
            break;
        default:
            buffer.append(data.at(i));
            break;
        }
    }
    emit dataDecoded(buffer);
#ifdef _DEBUG_PROTOCOL
    qDebug() << _MODULE_NAME << "decode() - Decoded: " << buffer << " (" << buffer.size() << " bytes)";
#endif //_DEBUG_PROTOCOL
    return buffer;
}

QByteArray Protocol_ASCII::encode(QByteArray data) {
#ifdef _DEBUG_PROTOCOL
    qDebug() << _MODULE_NAME << "encode() - Encoding: " << data << " (" << data.size() << " bytes)";
#endif //_DEBUG_PROTOCOL
    QByteArray buffer;
    for(qint64 i = 0; i < data.size(); i++) {
        if (data.at(i) == '[') {
            QString sTag = QString(data.at(i++));
            while(data.at(i) != ']' && i < data.size()) {
                sTag += QString(data.at(i++));
            }
            sTag += QString(data.at(i));
            if (_tagsList.contains(sTag)) {
                char eChar = (char) _tagsList.indexOf(sTag) != 33 ? _tagsList.indexOf(sTag) : 0x7f;
#ifdef _DEBUG_PROTOCOL
                qDebug() << _MODULE_NAME << "encode() - Encoded" << sTag << " in" << (int) eChar;
#endif //_DEBUG_PROTOCOL
                buffer.append(eChar);

            }
        } else {
            buffer.append(data.at(i));
        }
    }
    emit dataEncoded(buffer);
#ifdef _DEBUG_PROTOCOL
    qDebug() << _MODULE_NAME << "encode() - Encoded: " << buffer << " (" << buffer.size() << " bytes)";
#endif //_DEBUG_PROTOCOL
    return buffer;
}
