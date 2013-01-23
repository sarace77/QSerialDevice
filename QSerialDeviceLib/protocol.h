#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QtCore>

class Protocol
{
protected:
    QStringList _tagList;
public:
    Protocol();
    virtual ~Protocol();

    virtual QByteArray decode(QByteArray data);
    virtual QByteArray encode(QByteArray data);
    virtual QStringList getTagList();
};

#endif // PROTOCOL_H
