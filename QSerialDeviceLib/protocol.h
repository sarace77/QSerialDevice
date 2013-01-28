#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QByteArray>
#include <QObject>
#include <QStringList>

class Protocol : public QObject
{
    Q_OBJECT

protected:
    QStringList _tagsList;

public:
    explicit Protocol(QObject *parent = 0);
    virtual ~Protocol();

    QStringList tagsList();

public slots:
    virtual QByteArray decode(QByteArray data);
    virtual QByteArray encode(QByteArray data);

signals:
    void dataDecoded(QByteArray data);
    void dataEncoded(QByteArray data);
};

#endif // PROTOCOL_H
