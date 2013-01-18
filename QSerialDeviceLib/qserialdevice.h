#ifndef QSERIALDEVICE_H
#define QSERIALDEVICE_H

#include <QQueue>
#include <QPushButton>
#include <QToolBar>

#include <QtExtSerialPort/qextserialport.h>

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

private slots:
    void onDataAvailable();

    void _DEBUG_printMsg(QByteArray data);

public:
    QSerialDevice(QObject *parent = 0);
    ~QSerialDevice();

    QWidget* getWidget(QWidget *parent);
    QToolBar* getToolBar();
    bool isOpen();

    QByteArray read();

signals:
    void dataAvailable();
    void dataWritten(qint64 bytes);
    void msgAvailable(QByteArray msg);
    void portOpened();
    void portClosed();

public slots:
    void close();
    bool open(QIODevice::OpenMode mode = QIODevice::ReadWrite);
    bool setSerialParams(QString pName, QString bRate = "57600", QString dBits = "8", QString par = "NONE", QString sBits = "1", QString fControl = "OFF");
    qint64 write(QByteArray data);
};

#endif // QSERIALDEVICE_H
