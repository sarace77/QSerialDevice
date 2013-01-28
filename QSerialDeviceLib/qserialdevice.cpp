#include <QDir>
#include <QFile>
#include <QTime>

#include "hled.h"
#include "protocol_ascii.h"
#include "qserialdevice.h"
#include "qserialsettingswidget.h"

#include <QtExtSerialPort/qextserialport.h>

#define _MAX_BUFF_SIZE 255
#define _READ_TIMEOUT 500

#ifdef _DEBUG_QSERIALDEVICE_LIB
#include <QDebug>
#define _MODULE_NAME "[QSERIALDEVICE] - "
#endif //_DEBUG_QSERIALDEVICE_LIB

#ifdef Q_OS_WIN
#define _QSERIALDEVICE_CONFIG_DIR "Settings"
#define _QSERIALDEVICE_CONFIG_FILE "settings.ini"
#else //Q_OS_UNIX/LINUX
#define _QSERIALDEVICE_CONFIG_DIR ".qserialdevice"
#define _QSERIALDEVICE_CONFIG_FILE "config.xml"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
QStringList xmlLoad(QFile *iFile) {
    QStringList res;
    QXmlStreamReader xmlReader(iFile);
    while(!xmlReader.atEnd()) {
        xmlReader.readNext();
        if (xmlReader.attributes().hasAttribute("name"))
            res << QString(xmlReader.attributes().value("name").toAscii());
        if (xmlReader.name() == "BaudRate")
            res << xmlReader.readElementText();
        if (xmlReader.name() == "DataBits")
            res << xmlReader.readElementText();
        if (xmlReader.name() == "Parity")
            res << xmlReader.readElementText();
        if (xmlReader.name() == "StopBits")
            res << xmlReader.readElementText();
        if (xmlReader.name() == "FlowControl")
            res << xmlReader.readElementText();
    }
#ifdef _DEBUG_QSERIALDEVICE_LIB
    if(xmlReader.hasError())
        qWarning() << "[XML_LOAD] - error: " << xmlReader.errorString();
    else
        qDebug() << "[XML_LOAD] - Read Configuration: " << res;
#endif //_DEBUG_QSERIALDEVICE_LIB
    return res;
}

void xmlSave(QFile *oFile, QString sPortName, QStringList sSettings) {
    QXmlStreamWriter xmlWriter(oFile);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("QSerialDevice");
    xmlWriter.writeStartElement("Port");
    xmlWriter.writeAttribute("name", sPortName);
    xmlWriter.writeTextElement("BaudRate", sSettings.at(0));
    xmlWriter.writeTextElement("DataBits", sSettings.at(1));
    xmlWriter.writeTextElement("Parity", sSettings.at(2));
    xmlWriter.writeTextElement("StopBits", sSettings.at(3));
    xmlWriter.writeTextElement("FlowControl", sSettings.at(4));
    xmlWriter.writeEndElement();
    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
}
#endif //Q_OS_WIN



QSerialDevice::QSerialDevice(QString sFileSettingsName, QObject *parent) : QObject(parent) {
    _closeButton = new QPushButton("Close");
    _led = new HLed();
    _mainToolBar = new QToolBar();
    _openButton = new QPushButton("Open");
    _serialPort = new QextSerialPort();
    _serialSettingsWidget = new QSerialSettingsWidget();

    if (sFileSettingsName.isEmpty())
        sFileSettingsName = QString(_QSERIALDEVICE_CONFIG_FILE);

    QDir _configDir;
    if (!_configDir.exists(_QSERIALDEVICE_CONFIG_DIR)) {
#ifdef _DEBUG_QSERIALDEVICE_LIB
        qDebug() << _MODULE_NAME << "QSerialDevice() - Creating " << _configDir.absolutePath() + "/" + _QSERIALDEVICE_CONFIG_DIR;
        _configDir.mkdir(_QSERIALDEVICE_CONFIG_DIR);
#endif //_DEBUG_QSERIALDEVICE_LIB
    } else {
        _configDir.cd(_QSERIALDEVICE_CONFIG_DIR);
        QFile _configFile(_configDir.absolutePath()+ QString("/") + sFileSettingsName);
        if (_configFile.exists()) {
#ifdef _DEBUG_QSERIALDEVICE_LIB
        qDebug() << _MODULE_NAME << "QSerialDevice() - Loading Settings from: " << sFileSettingsName;
#endif //_DEBUG_QSERIALDEVICE_LIB
            if(_configFile.open(QIODevice::ReadOnly)) {
#ifdef Q_OS_WIN
                /// \todo
#else //Q_OS_UNIX | Q_OS_LINUX
                QStringList sP = xmlLoad(&_configFile);
                setSerialParams(sP.at(0), sP.at(1), sP.at(2), sP.at(3), sP.at(4), sP.at(5));
#endif //Q_OS_WIN
                _configFile.close();
#ifdef _DEBUG_QSERIALDEVICE_LIB
            } else {
                qWarning() << _MODULE_NAME << "QSerialDevice() - Unable to open " << _QSERIALDEVICE_CONFIG_FILE;
#endif //_DEBUG_QSERIALDEVICE_LIB
            }
        }
    }

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


    connect(_serialPort, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
    connect(_openButton, SIGNAL(clicked()), this, SLOT(open()));
    connect(_closeButton, SIGNAL(clicked()), this, SLOT(close()));
}

QSerialDevice::~QSerialDevice() {
    QString sPortName = ((QSerialSettingsWidget*)_serialSettingsWidget)->getPortName();
    QStringList sSettings = QSerialSettingsWidget::settings2QStringList(((QSerialSettingsWidget*)_serialSettingsWidget)->getPortSettings());
    close();
    QDir _configDir;
    _configDir.cd(_QSERIALDEVICE_CONFIG_DIR);
    QFile _configFile(_configDir.absolutePath()+QString("/") + QString(+_QSERIALDEVICE_CONFIG_FILE));
    if(_configFile.open(QIODevice::WriteOnly)) {
#ifdef Q_OS_WIN
        /// \todo
#else // Q_OS_UNIX | Q_OS_LINUX
        xmlSave(&_configFile, sPortName, sSettings);
#endif // Q_OS_WIN
#ifdef _DEBUG_QSERIALDEVICE_LIB
    } else {
        qWarning() << _MODULE_NAME << "~QSerialDevice() - Unable to open " << _QSERIALDEVICE_CONFIG_FILE;
#endif //_DEBUG_QSERIALDEVICE_LIB
    }
    _configFile.close();
    delete _closeButton;
    delete _openButton;
    delete _mainToolBar;
    delete _serialSettingsWidget;
}

void QSerialDevice::close() {
    if (_serialPort->isOpen()) {
        _serialPort->close();
#ifdef _DEBUG_QSERIALDEVICE_LIB
        qDebug() << _MODULE_NAME << "close() - Closing Serial port" << ((QextSerialPort *)_serialPort)->portName();
    } else {
        qWarning() << _MODULE_NAME << "close() - " <<((QextSerialPort *)_serialPort)->portName() << " is not opened";
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

QToolBar* QSerialDevice::getToolBar() {
    return _mainToolBar;
}

QWidget* QSerialDevice::getWidget(QWidget *parent) {
    _serialSettingsWidget->setParent(parent);
    return _serialSettingsWidget;
}

bool QSerialDevice::isOpen() {
    return _serialPort->isOpen();
}

void QSerialDevice::onDataAvailable() {
    QTime readWaitTimer;
    readWaitTimer.start();
    while(readWaitTimer.elapsed() < _READ_TIMEOUT);
#ifdef _DEBUG_QSERIALDEVICE_LIB
    qDebug() << _MODULE_NAME << "onDataAvailable() - " <<((QextSerialPort *)_serialPort)->bytesAvailable() << " Bytes on " << ((QextSerialPort *)_serialPort)->portName();
#endif //_DEBUG_QSERIALDEVICE_LIB
    QByteArray data = _serialPort->readAll();
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
    if (((QextSerialPort *)_serialPort)->isOpen()) {
        if(((QextSerialPort *)_serialPort)->portName() == ((QSerialSettingsWidget*)_serialSettingsWidget)->getPortName()) {
#ifdef _DEBUG_QSERIALDEVICE_LIB
            qWarning() << _MODULE_NAME << "open() - " << ((QextSerialPort *)_serialPort)->portName() << " already opened!";
#endif //_DEBUG_QSERIALDEVICE_LIB
            return false;
        } else {
#ifdef _DEBUG_QSERIALDEVICE_LIB
            qWarning() << _MODULE_NAME << "closing() - " << ((QextSerialPort *)_serialPort)->portName();
#endif //_DEBUG_QSERIALDEVICE_LIB
            close();
        }
    }
    if (!_isPortConfigured) {
        ((QextSerialPort *)_serialPort)->setPortName(((QSerialSettingsWidget*)_serialSettingsWidget)->getPortName());
        ((QextSerialPort *)_serialPort)->setBaudRate(((QSerialSettingsWidget*)_serialSettingsWidget)->getPortSettings().BaudRate);
        ((QextSerialPort *)_serialPort)->setDataBits(((QSerialSettingsWidget*)_serialSettingsWidget)->getPortSettings().DataBits);
        ((QextSerialPort *)_serialPort)->setFlowControl(((QSerialSettingsWidget*)_serialSettingsWidget)->getPortSettings().FlowControl);
        ((QextSerialPort *)_serialPort)->setParity(((QSerialSettingsWidget*)_serialSettingsWidget)->getPortSettings().Parity);
        ((QextSerialPort *)_serialPort)->setStopBits(((QSerialSettingsWidget*)_serialSettingsWidget)->getPortSettings().StopBits);
        ((QextSerialPort *)_serialPort)->setTimeout(((QSerialSettingsWidget*)_serialSettingsWidget)->getPortSettings().Timeout_Millisec);
    }

    if(((QextSerialPort *)_serialPort)->open(mode)) {
        ((HLed *)_led)->setColor(QColor(Qt::green));
#ifdef _DEBUG_QSERIALDEVICE_LIB
        qDebug() << _MODULE_NAME << "open() - " << ((QextSerialPort *)_serialPort)->portName() << " succesfully opened!";
#endif //_DEBUG_QSERIALDEVICE_LIB
        emit portOpened();
        _closeButton->setEnabled(true);
        ((HLed *)_led)->turnOn();
        _openButton->setEnabled(false);
        _serialSettingsWidget->setDisabled(true);
        ((QSerialSettingsWidget*)_serialSettingsWidget)->getPortWidget()->setEnabled(false);
    } else {
#ifdef _DEBUG_QSERIALDEVICE_LIB
        qWarning() << _MODULE_NAME << "open() - Error opening " << ((QextSerialPort *)_serialPort)->portName();
        qWarning() << _MODULE_NAME << "open() - Error: " << ((QextSerialPort *)_serialPort)->errorString();
#endif //_DEBUG_QSERIALDEVICE_LIB
        ((HLed *)_led)->setColor(QColor(Qt::red));
        return false;
    }
    return true;
}

QByteArray QSerialDevice::read() {
    QByteArray res;
    res.clear();
    if (_serialPort->isOpen()) {
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
    ((QextSerialPort *)_serialPort)->setPortName(pName);
    ((QextSerialPort *)_serialPort)->setBaudRate((BaudRateType) bRate.toInt());
    ((QextSerialPort *)_serialPort)->setDataBits((DataBitsType) dBits.toInt());
    QStringList sSettings;
    sSettings << bRate << dBits << par << sBits << fControl;
    ((QSerialSettingsWidget *)_serialSettingsWidget)->setPortName(pName);
    ((QSerialSettingsWidget *)_serialSettingsWidget)->setPortSettings(sSettings);
    if (fControl.toUpper() == "OFF")
        ((QextSerialPort *)_serialPort)->setFlowControl(FLOW_OFF);
    else if (fControl.toUpper() == "HW" || fControl.toUpper() == "HARDWARE" )
        ((QextSerialPort *)_serialPort)->setFlowControl(FLOW_HARDWARE);
    else
        ((QextSerialPort *)_serialPort)->setFlowControl(FLOW_XONXOFF);

    if (par.toUpper() == "NONE")
        ((QextSerialPort *)_serialPort)->setParity(PAR_NONE);
    else if (par.toUpper() == "EVEN")
        ((QextSerialPort *)_serialPort)->setParity(PAR_NONE);
    else if (par.toUpper() == "ODD")
        ((QextSerialPort *)_serialPort)->setParity(PAR_ODD);
    else if (par.toUpper() == "SPACE")
        ((QextSerialPort *)_serialPort)->setParity(PAR_NONE);
#ifdef Q_OS_WIN
    else if (par.toUpper() == "MARK")
        _serialPort.setParity(PAR_MARK);
#endif //Q_OS_WIN

    if (sBits == "1")
        ((QextSerialPort *)_serialPort)->setStopBits(STOP_1);
    else if (sBits == "2")
        ((QextSerialPort *)_serialPort)->setStopBits(STOP_1);
#ifdef Q_OS_WIN
    else if (sBits == "1.5")
        ((QextSerialPort *)_serialPort)->setStopBits(STOP_1_5);
#endif //Q_OS_WIN

    ((QextSerialPort *)_serialPort)->setTimeout(1000);

    _isPortConfigured = true;
    return true;
}


qint64 QSerialDevice::write(QByteArray data) {
#ifdef _DEBUG_QSERIALDEVICE_LIB
            qDebug() << _MODULE_NAME << "write() - Sending:" << data << "(" << data.size() << " Bytes)";
#endif //_DEBUG_QSERIALDEVICE_LIB
    qint64 res = -1;
    if (_serialPort->isOpen()) {
        res = _serialPort->write(data);
        ((HLed *) _led)->blink(res);
        ((HLed *) _led)->setColor(QColor(Qt::green));
        ((HLed *) _led)->turnOn();
        if (res < data.size()) {
#ifdef _DEBUG_QSERIALDEVICE_LIB
            qWarning() << _MODULE_NAME << "write() - Write Error";
            qWarning() << _MODULE_NAME << "write() - " << ((QextSerialPort *)_serialPort)->portName() << " returns" << ((QextSerialPort *)_serialPort)->errorString();
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
