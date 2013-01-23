#include "hled.h"

#include "protocol_ascii.h"

#include "qserialdevice.h"
#include "qserialsettingswidget.h"

#ifdef _DEBUG_QSERIALDEVICE_LIB
#include <QDebug>
#define _MODULE_NAME "[QSERIALDEVICE] - "
#endif //_DEBUG_QSERIALDEVICE_LIB

#include <QTime>

#define _MAX_BUFF_SIZE 255
#define _READ_TIMEOUT 500

QSerialDevice::QSerialDevice(QObject *parent) : QObject(parent) {
    _closeButton = new QPushButton("Close");
    _led = new HLed();
    _mainToolBar = new QToolBar();
    _openButton = new QPushButton("Open");
    _prt = NULL;
    _serialSettingsWidget = new QSerialSettingsWidget();

    _mainToolBar->addWidget(((QSerialSettingsWidget*)_serialSettingsWidget)->getPortWidget());
    _mainToolBar->addWidget(_openButton);
    _mainToolBar->addWidget(_closeButton);
    _mainToolBar->addSeparator();
    _mainToolBar->addWidget(_led);

    _isPortConfigured = false;

    _closeButton->setEnabled(false);
    ((HLed *)_led)->setColor(QColor(Qt::gray));
    ((HLed *)_led)->turnOff();
    _openButton->setEnabled(true);


    connect(&_serialPort, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
    connect(_openButton, SIGNAL(clicked()), this, SLOT(open()));
    connect(_closeButton, SIGNAL(clicked()), this, SLOT(close()));

#ifdef _DEBUG_QSERIALDEVICE_LIB
    connect(this, SIGNAL(msgAvailable(QByteArray)), this, SLOT(_DEBUG_printMsg(QByteArray)));
#endif //_DEBUG_QSERIALDEVICE_LIB
}

QSerialDevice::~QSerialDevice() {
    delete _closeButton;
    delete _mainToolBar;
    delete _openButton;
    delete _serialSettingsWidget;
}

void QSerialDevice::close() {
    if (_serialPort.isOpen()) {
        _serialPort.close();
#ifdef _DEBUG_QSERIALDEVICE_LIB
        qDebug() << _MODULE_NAME << "close() - Closing Serial port" << _serialPort.portName();
    } else {
        qWarning() << _MODULE_NAME << "close() - " <<_serialPort.portName() << " is not opened";
#endif //_DEBUG_QSERIALDEVICE_LIB
    }
    _closeButton->setEnabled(false);
    ((HLed *)_led)->setColor(QColor(Qt::gray));
    ((HLed *)_led)->turnOff();
    _openButton->setEnabled(true);
    _serialSettingsWidget->setEnabled(true);
    ((QSerialSettingsWidget*)_serialSettingsWidget)->getPortWidget()->setEnabled(true);
    emit portClosed();
}

QByteArray QSerialDevice::decodeData(Protocol *proto, QByteArray data) {
#ifdef _DEBUG_QSERIALDEVICE_LIB
    qDebug() << _MODULE_NAME << "decodeData() - trying to decoding: " << data.size() << "bytes";
#endif //_DEBUG_QSERIALDEVICE_LIB
    if (proto) {
        QByteArray dataDecoded = proto->decode(data);
#ifdef _DEBUG_QSERIALDEVICE_LIB
        qDebug() << _MODULE_NAME << "decodeData() - Decoded Data:" << dataDecoded;
#endif //_DEBUG_QSERIALDEVICE_LIB
        return dataDecoded;
    }
    return data;
}

QToolBar* QSerialDevice::getToolBar() {
    return _mainToolBar;
}

QWidget* QSerialDevice::getWidget(QWidget *parent) {
    _serialSettingsWidget->setParent(parent);
    return _serialSettingsWidget;
}

bool QSerialDevice::isOpen() {
    return _serialPort.isOpen();
}

void QSerialDevice::onDataAvailable() {
    QTime readWaitTimer;
    readWaitTimer.start();
    while(readWaitTimer.elapsed() < _READ_TIMEOUT);
#ifdef _DEBUG_QSERIALDEVICE_LIB
    qDebug() << _MODULE_NAME << "onDataAvailable() - " <<_serialPort.bytesAvailable() << " Bytes on " << _serialPort.portName();
#endif //_DEBUG_QSERIALDEVICE_LIB
    QByteArray data = _serialPort.readAll();
#ifdef _DEBUG_QSERIALDEVICE_LIB
    qDebug() << _MODULE_NAME << "onDataAvailable() - Data:" << data;
#endif //_DEBUG_QSERIALDEVICE_LIB
    ((HLed *) _led)->blink(data.size());
    ((HLed *) _led)->setColor(QColor(Qt::green));
    ((HLed *) _led)->turnOn();
    while (_inBuffer.count() >= _MAX_BUFF_SIZE)
        _inBuffer.dequeue();
    _inBuffer.enqueue(data);
    emit dataAvailable();
    emit msgAvailable(data);
}

bool QSerialDevice::open(QIODevice::OpenMode mode) {
    if (_serialPort.isOpen()) {
        if(_serialPort.portName() == ((QSerialSettingsWidget*)_serialSettingsWidget)->getPortName()) {
#ifdef _DEBUG_QSERIALDEVICE_LIB
            qWarning() << _MODULE_NAME << "open() - " << _serialPort.portName() << " already opened!";
#endif //_DEBUG_QSERIALDEVICE_LIB
            return false;
        } else {
#ifdef _DEBUG_QSERIALDEVICE_LIB
            qWarning() << _MODULE_NAME << "closing() - " << _serialPort.portName();
#endif //_DEBUG_QSERIALDEVICE_LIB
            close();
        }
    }
    if (!_isPortConfigured) {
        _serialPort.setPortName(((QSerialSettingsWidget*)_serialSettingsWidget)->getPortName());
        _serialPort.setBaudRate(((QSerialSettingsWidget*)_serialSettingsWidget)->getPortSettings().BaudRate);
        _serialPort.setDataBits(((QSerialSettingsWidget*)_serialSettingsWidget)->getPortSettings().DataBits);
        _serialPort.setFlowControl(((QSerialSettingsWidget*)_serialSettingsWidget)->getPortSettings().FlowControl);
        _serialPort.setParity(((QSerialSettingsWidget*)_serialSettingsWidget)->getPortSettings().Parity);
        _serialPort.setStopBits(((QSerialSettingsWidget*)_serialSettingsWidget)->getPortSettings().StopBits);
        _serialPort.setTimeout(((QSerialSettingsWidget*)_serialSettingsWidget)->getPortSettings().Timeout_Millisec);
    }

    if(_serialPort.open(mode)) {
        ((HLed *)_led)->setColor(QColor(Qt::green));
#ifdef _DEBUG_QSERIALDEVICE_LIB
        qDebug() << _MODULE_NAME << "open() - " << _serialPort.portName() << " succesfully opened!";
#endif //_DEBUG_QSERIALDEVICE_LIB
        emit portOpened();
        _closeButton->setEnabled(true);
        ((HLed *)_led)->turnOn();
        _openButton->setEnabled(false);
        _serialSettingsWidget->setDisabled(true);
        ((QSerialSettingsWidget*)_serialSettingsWidget)->getPortWidget()->setEnabled(false);
    } else {
#ifdef _DEBUG_QSERIALDEVICE_LIB
        qWarning() << _MODULE_NAME << "open() - Error opening " << _serialPort.portName();
        qWarning() << _MODULE_NAME << "open() - Error: " << _serialPort.errorString();
#endif //_DEBUG_QSERIALDEVICE_LIB
        ((HLed *)_led)->setColor(QColor(Qt::red));
        return false;
    }
    return true;
}

QByteArray QSerialDevice::read() {
    QByteArray res;
    res.clear();
    if (_serialPort.isOpen()) {
        if(_inBuffer.isEmpty()) {
#ifdef _DEBUG_QSERIALDEVICE_LIB
            qWarning() << _MODULE_NAME << "read() - no message available in queue!";
#endif //_DEBUG_QSERIALDEVICE_LIB
        } else {
            res = _inBuffer.dequeue();
#ifdef _DEBUG_QSERIALDEVICE_LIB
            qDebug() << _MODULE_NAME << "read() - Message dequeued (" << res.size() << "bytes)";
#endif //_DEBUG_QSERIALDEVICE_LIB
        }
#ifdef _DEBUG_QSERIALDEVICE_LIB
    } else {
        qWarning() << _MODULE_NAME << "read() - no serial port opened!";
#endif //_DEBUG_QSERIALDEVICE_LIB
    }
    return res;
}

bool QSerialDevice::setSerialParams(QString pName, QString bRate, QString dBits, QString par, QString sBits, QString fControl) {
    QStringList sPorts;
    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
    foreach (QextPortInfo port, ports) {
        sPorts << port.portName;
    }
    if(pName.isEmpty() || !sPorts.contains(pName)) {
#ifdef _DEBUG_QSERIALDEVICE_LIB
        qWarning() << _MODULE_NAME << "setSerialParams() - " << pName << " is not a valid COM port!";
#endif //_DEBUG_QSERIALDEVICE_LIB
        return false;
    }
    if (dBits.toInt() < 5 || dBits.toInt() > 8) {
#ifdef _DEBUG_QSERIALDEVICE_LIB
        qWarning() << _MODULE_NAME << "setSerialParams() - " << dBits << " is not a valid Data Bits value!";
#endif //_DEBUG_QSERIALDEVICE_LIB
        return false;
    }
    if (par.toUpper() != "NONE" && par.toUpper() != "EVEN" && par.toUpper() != "ODD" &&
#ifdef Q_OS_WIN
            par.toUpper() != "MARK" &&
#endif //Q_OS_WIN
            par.toUpper() != "SPACE") {
#ifdef _DEBUG_QSERIALDEVICE_LIB
        qWarning() << _MODULE_NAME << "setSerialParams() - " << par.toUpper() << " is not a valid Parity value!";
#endif //_DEBUG_QSERIALDEVICE_LIB
        return false;
    }
    if (sBits != "1" &&
#ifdef Q_OS_WIN
            sBits() != "1.5" &&
#endif //Q_OS_WIN
            sBits != "2") {
#ifdef _DEBUG_QSERIALDEVICE_LIB
        qWarning() << _MODULE_NAME << "setSerialParams() - " << sBits << " is not a valid Stop Bits value!";
#endif //_DEBUG_QSERIALDEVICE_LIB
        return false;
    }
    if (fControl.toUpper() != "OFF" &&
            fControl.toUpper() != "HW" &&
            fControl.toUpper() != "HARDWARE" &&
            fControl.toUpper() != "XON/XOFF" &&
            fControl.toUpper() != "XONXOFF" &&
            fControl.toUpper() != "XON-XOFF" &&
            fControl.toUpper() != "XON XOFF" ) {
#ifdef _DEBUG_QSERIALDEVICE_LIB
        qWarning() << _MODULE_NAME << "setSerialParams() - " << fControl.toUpper() << " is not a valid Flow Control value!";
#endif //_DEBUG_QSERIALDEVICE_LIB
        return false;
    }
    _serialPort.setPortName(pName);
    _serialPort.setBaudRate((BaudRateType) bRate.toInt());
    _serialPort.setDataBits((DataBitsType) dBits.toInt());

    if (fControl.toUpper() == "OFF")
        _serialPort.setFlowControl(FLOW_OFF);
    else if (fControl.toUpper() == "HW" || fControl.toUpper() == "HARDWARE" )
        _serialPort.setFlowControl(FLOW_HARDWARE);
    else
        _serialPort.setFlowControl(FLOW_XONXOFF);

    if (par.toUpper() == "NONE")
        _serialPort.setParity(PAR_NONE);
    else if (par.toUpper() == "EVEN")
        _serialPort.setParity(PAR_NONE);
    else if (par.toUpper() == "ODD")
        _serialPort.setParity(PAR_ODD);
    else if (par.toUpper() == "SPACE")
        _serialPort.setParity(PAR_NONE);
#ifdef Q_OS_WIN
    else if (par.toUpper() == "MARK")
        _serialPort.setParity(PAR_MARK);
#endif //Q_OS_WIN

    if (sBits == "1")
        _serialPort.setStopBits(STOP_1);
    else if (sBits == "2")
        _serialPort.setStopBits(STOP_1);
#ifdef Q_OS_WIN
    else if (sBits == "1.5")
        _serialPort.setStopBits(STOP_1_5);
#endif //Q_OS_WIN

    _serialPort.setTimeout(1000);

    _isPortConfigured = true;
    return true;
}

void QSerialDevice::setProtocol(Protocol *proto) {
    _prt = proto;
}

qint64 QSerialDevice::write(QByteArray data) {
    qint64 res = -1;
    if (_serialPort.isOpen()) {
        if (_prt) {
            data = _prt->encode(data);
#ifdef _DEBUG_QSERIALDEVICE_LIB
            qDebug() << _MODULE_NAME << "write() - Encoded Data:" << data << "(" << data.size() << " Bytes)";
#endif //_DEBUG_QSERIALDEVICE_LIB
        }
        res = _serialPort.write(data);
        ((HLed *) _led)->blink(res);
        ((HLed *) _led)->setColor(QColor(Qt::green));
        ((HLed *) _led)->turnOn();
        if (res < data.size()) {
#ifdef _DEBUG_QSERIALDEVICE_LIB
            qWarning() << _MODULE_NAME << "write() - Write Error";
            qWarning() << _MODULE_NAME << "write() - " << _serialPort.portName() << " returns" << _serialPort.errorString();
#endif //_DEBUG_QSERIALDEVICE_LIB
            ((HLed *)_led)->setColor(QColor(Qt::red));
        }
        if (res > 0)
            emit dataWritten(res);
    } else {
#ifdef _DEBUG_QSERIALDEVICE_LIB
        qWarning() << _MODULE_NAME << "write() - no serial port opened!";
#endif //_DEBUG_QSERIALDEVICE_LIB
    }
    return res;
}

#ifdef _DEBUG_QSERIALDEVICE_LIB
void QSerialDevice::_DEBUG_printMsg(QByteArray data) {
    qDebug() << "{DEBUG} - " << _MODULE_NAME << data;
}
#endif //_DEBUG_QSERIALDEVICE_LIB
