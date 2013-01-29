#ifndef QSERIALDEVICE_H
#define QSERIALDEVICE_H

#include <QIODevice>
#include <QQueue>
#include <QPushButton>
#include <QToolBar>

class QSerialDevice : public QObject
{
    Q_OBJECT

/*
 * \class QSerialDevice
 *
 * \brief QSerialDevice is a QIODevice class extension based on QExtSerialPort implementation. It abstracts serial port communication in both \
 * event-driven/polling ways.
 *
 */
public:
    /*
     * \brief Class constructor
     *
     * \param sFileSettings: (in) name of file used to store/retrieve port configuration. If not passed, it uses standard file/path to save/load conf.
     * \param parent: (in) pointer to QObject parent
     */
    QSerialDevice(QString sFileSettingsName = QString(), QObject *parent = 0);

    /*
     * \brief Class destructor
     */
    ~QSerialDevice();

    bool isOpen();
    QByteArray read();
    QToolBar* toolbar();
    QWidget* widget();

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

/* Private Members internal use only */
private:
    QIODevice *_serialPort;
    QWidget *_led, *_serialSettingsWidget;
    QPushButton *_closeButton, *_openButton;
    QToolBar *_mainToolBar;
    bool _isPortConfigured;
    QQueue<QByteArray > _inBuffer;

private slots:
    void onDataAvailable();
/* Private Members internal use only */
};

#endif // QSERIALDEVICE_H
