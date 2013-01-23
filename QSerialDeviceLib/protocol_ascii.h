#ifndef PROTOCOL_ASCII_H
#define PROTOCOL_ASCII_H

#include "protocol.h"

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

class Protocol_ASCII : public Protocol
{
public:
    Protocol_ASCII();

    QByteArray decode(QByteArray data);
    QByteArray encode(QByteArray data);
};

#endif // PROTOCOL_ASCII_H
