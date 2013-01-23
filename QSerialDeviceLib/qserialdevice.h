#ifndef QSERIALDEVICE_H
#define QSERIALDEVICE_H


#include <QQueue>
#include <QPushButton>
#include <QToolBar>

#include <QtExtSerialPort/qextserialport.h>

#include "protocol.h"

class QSerialDevice : public QObject
{
    Q_OBJECT

private:
    QWidget *_led, *_serialSettingsWidget;
    QextSerialPort _serialPort;

    QPushButton *_closeButton, *_openButton;
    QToolBar *_mainToolBar;

    bool _isPortConfigured;

    QQueue<QByteArray > _inBuffer;

    Protocol *_prt;

private slots:
    void onDataAvailable();

#ifdef _DEBUG_QSERIALDEVICE_LIB
    void _DEBUG_printMsg(QByteArray data);
#endif //__DEBUG_QSERIALDEVICE_LIB

public:
    QSerialDevice(QObject *parent = 0);
    ~QSerialDevice();

    QWidget* getWidget(QWidget *parent);
    QToolBar* getToolBar();
    bool isOpen();

    QByteArray read();

    void setProtocol(Protocol *proto);

    static QByteArray decodeData(Protocol *proto, QByteArray data);

public slots:
    void close();
    bool open(QIODevice::OpenMode mode = QIODevice::ReadWrite);
    bool setSerialParams(QString pName, QString bRate = "57600", QString dBits = "8", QString par = "NONE", QString sBits = "1", QString fControl = "OFF");
    qint64 write(QByteArray data);

signals:
    void dataAvailable();
    void dataWritten(qint64 bytes);
    void msgAvailable(QByteArray msg);
    void portOpened();
    void portClosed();
};

#endif // QSERIALDEVICE_H
