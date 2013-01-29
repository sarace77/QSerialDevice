#include "qserialsettingswidget.h"
#include "ui_qserialsettingswidget.h"

#ifdef _DEBUG_QSERIALDEVICE_WIDGET
#include <QDebug>
#endif //_DEBUG_QSERIALDEVICE_WIDGET

#define _MODULE_NAME "[QSERIALSETTINGSWIDGET] - "

QSerialSettingsWidget::QSerialSettingsWidget(QWidget *parent) : QWidget(parent), ui(new Ui::QSerialSettingsWidget) {
    ui->setupUi(this);
    setMinimumSize(400,200);
    _serialEnumerator = new QextSerialEnumerator(this);
    _toolBarSerialPortCombo = new QComboBox(this);

#ifdef _DEBUG_QSERIALDEVICE_WIDGET
    _isFirstTime = true;
#endif //_DEBUG_QSERIALDEVICE_WIDGET
    updateDevicesList();

    quint8 index = 0;
#if (defined(B230400) && defined(B4000000))
#ifdef _DEBUG_QSERIALDEVICE_WIDGET
    qDebug() << _MODULE_NAME << "QSerialSettingsWidget() - Adding B230400 & B4000000 Baud Rates";
#endif //_DEBUG_QSERIALDEVICE_WIDGET
    ui->baudrateCombo->addItem("4000000");
    ++index;
    ui->baudrateCombo->addItem("3500000");
    ++index;
    ui->baudrateCombo->addItem("3000000");
    ++index;
    ui->baudrateCombo->addItem("2500000");
    ++index;
    ui->baudrateCombo->addItem("2000000");
    ++index;
    ui->baudrateCombo->addItem("1500000");
    ++index;
    ui->baudrateCombo->addItem("1152000");
    ++index;
    ui->baudrateCombo->addItem("1000000");
    ++index;
    ui->baudrateCombo->addItem("921600");
    ++index;
    ui->baudrateCombo->addItem("576000");
    ++index;
    ui->baudrateCombo->addItem("500000");
    ++index;
    ui->baudrateCombo->addItem("460800");
    ++index;
#endif // B230400 && B4000000
#ifdef Q_OS_WIN
#ifdef _DEBUG_QSERIALDEVICE_WIDGET
    qDebug() << _MODULE_NAME << "QSerialSettingsWidget() - Windows Baud Rates";
#endif //_DEBUG_QSERIALDEVICE_WIDGET
    ui->baudrateCombo->addItem("256000");
    ++index;
#endif // Q_OS_WIN
#if (defined(B230400) && defined(B4000000))
    ui->baudrateCombo->addItem("230400");
    ++index;
#endif // B230400 && B4000000
#ifdef Q_OS_WIN
    ui->baudrateCombo->addItem("128000");
    ++index;
#endif // Q_OS_WIN
    ui->baudrateCombo->addItem("115200");
    ++index;
#ifdef B76800
    ui->baudrateCombo->addItem("76800");
    ++index;
#endif
    ui->baudrateCombo->addItem("57600");
#ifdef Q_OS_WIN
    ui->baudrateCombo->addItem("56000");
#endif // Q_OS_WIN
    ui->baudrateCombo->addItem("38400");
    ui->baudrateCombo->addItem("19200");
#ifdef Q_OS_WIN
    ui->baudrateCombo->addItem("14400");
#endif //Q_OS_WIN
    ui->baudrateCombo->addItem("9600");
    ui->baudrateCombo->addItem("4800");
    ui->baudrateCombo->addItem("2400");
#ifdef Q_OS_UNIX
#ifdef _DEBUG_QSERIALDEVICE_WIDGET
    qDebug() << _MODULE_NAME << "QSerialSettingsWidget() - Adding *Nix Baud Rates";
#endif //_DEBUG_QSERIALDEVICE_WIDGET

    ui->baudrateCombo->addItem("1800");
#endif //Q_OS_UNIX
    ui->baudrateCombo->addItem("1200");
    ui->baudrateCombo->addItem("600");
    ui->baudrateCombo->addItem("300");
#ifdef Q_OS_UNIX
    ui->baudrateCombo->addItem("200");
    ui->baudrateCombo->addItem("150");
    ui->baudrateCombo->addItem("134");
#endif // Q_OS_UNIX
    ui->baudrateCombo->addItem("110");
#ifdef Q_OS_UNIX
    ui->baudrateCombo->addItem("75");
    ui->baudrateCombo->addItem("50");
#endif // Q_OS_UNIX
    ui->baudrateCombo->setCurrentIndex(index);

    ui->databitsCombo->addItem("8");
    ui->databitsCombo->addItem("7");
    ui->databitsCombo->addItem("6");
    ui->databitsCombo->addItem("5");
    ui->databitsCombo->setCurrentIndex(0);

    ui->parityCombo->addItem("NONE");
    ui->parityCombo->addItem("ODD");
    ui->parityCombo->addItem("EVEN");
#ifdef Q_OS_WIN
#ifdef _DEBUG_QSERIALDEVICE_WIDGET
    qDebug() << _MODULE_NAME << "QSerialSettingsWidget() - Adding Windows Parity specs";
#endif //_DEBUG_QSERIALDEVICE_WIDGET

    ui->parityCombo->addItem("MARK");
#endif //Q_OS_WIN
    ui->parityCombo->addItem("SPACE");
    ui->parityCombo->setCurrentIndex(0);

    ui->stopbitsCombo->addItem("1");
#ifdef Q_OS_WIN
#ifdef Q_OS_WIN#ifdef _DEBUG_QSERIALDEVICE_WIDGET
    qDebug() << _MODULE_NAME << "QSerialSettingsWidget() - Adding Windows Stop-bits specs";
#endif //_DEBUG_QSERIALDEVICE_WIDGET
    ui->stopbitsCombo->addItem("1.5");
#endif //Q_OS_WIN
    ui->stopbitsCombo->addItem("2");
    ui->stopbitsCombo->setCurrentIndex(0);

    ui->flowcontrolCombo->addItem("OFF");
    ui->flowcontrolCombo->addItem("HW");
    ui->flowcontrolCombo->addItem("XON/XOFF");
    ui->flowcontrolCombo->setCurrentIndex(0);

    connect(_serialEnumerator, SIGNAL(deviceDiscovered(QextPortInfo)), this, SLOT(updateDevicesList()));
    connect(_serialEnumerator, SIGNAL(deviceRemoved(QextPortInfo)), this, SLOT(updateDevicesList()));
    connect(_toolBarSerialPortCombo, SIGNAL(currentIndexChanged(int)), ui->serialPortCombo, SLOT(setCurrentIndex(int)));
    connect(ui->serialPortCombo, SIGNAL(currentIndexChanged(int)), _toolBarSerialPortCombo, SLOT(setCurrentIndex(int)));
}

QSerialSettingsWidget::~QSerialSettingsWidget()
{
    delete ui;
    delete _serialEnumerator;
}

QString QSerialSettingsWidget::getPortName() {
    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
    return ports.at(ui->serialPortCombo->currentIndex()).portName;
}

PortSettings QSerialSettingsWidget::getPortSettings() {
    PortSettings settings;
    settings.BaudRate = (BaudRateType) ui->baudrateCombo->currentText().toInt();
    settings.DataBits = (DataBitsType) ui->databitsCombo->currentText().toInt();
    settings.FlowControl = (FlowType) ui->flowcontrolCombo->currentIndex();
    settings.Parity = (ParityType) ui->parityCombo->currentIndex();
    settings.StopBits = (StopBitsType) ui->stopbitsCombo->currentIndex();
    settings.Timeout_Millisec = 1000;
    return settings;
}

QWidget* QSerialSettingsWidget::getPortWidget() {
    return _toolBarSerialPortCombo;
}

void QSerialSettingsWidget::setPortName(QString sName) {
    ui->serialPortCombo->setCurrentIndex(ui->serialPortCombo->findText(sName, Qt::MatchContains|Qt::MatchCaseSensitive));
}

void QSerialSettingsWidget::setPortSettings(QStringList sSettings) {
    ui->baudrateCombo->setCurrentIndex(ui->baudrateCombo->findText(sSettings.at(0)));
    ui->databitsCombo->setCurrentIndex(ui->databitsCombo->findText(sSettings.at(1)));
    ui->parityCombo->setCurrentIndex(ui->parityCombo->findText(sSettings.at(2)));
    ui->stopbitsCombo->setCurrentIndex(ui->stopbitsCombo->findText(sSettings.at(3)));
    ui->flowcontrolCombo->setCurrentIndex(ui->flowcontrolCombo->findText(sSettings.at(4)));
}

QStringList QSerialSettingsWidget::settings2QStringList(PortSettings pSettings) {
    QStringList res;
    res << QString("%1").arg((quint32) pSettings.BaudRate);
    res << QString("%1").arg((quint8) pSettings.DataBits);
    switch (pSettings.Parity) {
    case PAR_NONE:
        res << QString("NONE");
        break;
    case PAR_ODD:
        res << QString("ODD");
        break;
    case PAR_EVEN:
        res << QString("EVEN");
        break;
#ifdef Q_OS_WIN
    case PAR_MARK:
        res << QString("MARK");
        break;
#endif // Q_OS_WIN
    case PAR_SPACE:
        res << QString("SPACE");
        break;
    }
    switch (pSettings.StopBits) {
    case STOP_1:
        res << QString("1");
        break;
#ifdef Q_OS_WIN
    case STOP_1_5:
        res << QString("1.5");
        break;
#endif
    case STOP_2:
        res << QString("2");
        break;
    }
    switch (pSettings.FlowControl) {
    case FLOW_OFF:
        res << QString("OFF");
        break;
    case FLOW_HARDWARE:
        res << QString("HW");
        break;
    case FLOW_XONXOFF:
        res << QString("XON/XOFF");
        break;
    }

    res << QString("%1").arg(pSettings.FlowControl);
    return res;
}

void QSerialSettingsWidget::updateDevicesList() {
#ifdef _DEBUG_QSERIALDEVICE_WIDGET
    if (!_isFirstTime)
        qDebug() << _MODULE_NAME << "updateDevicesList() - Port Connection/Disconnection Event";
    else
        _isFirstTime = false;
#endif //_DEBUG_QSERIALDEVICE_WIDGET
    ui->serialPortCombo->clear();
    _toolBarSerialPortCombo->clear();
    QStringList sPorts;
    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
    foreach (QextPortInfo port, ports) {
        //sPorts << port.portName + "(" + port.friendName + " - " + port.physName + ")";
        sPorts << port.friendName + " (" + port.physName + ")";
    }
#ifdef _DEBUG_QSERIALDEVICE_WIDGET
    qDebug() << _MODULE_NAME << "updateDevicesList() - " << sPorts.count() << " ports found:" << sPorts;
#endif //_DEBUG_QSERIALDEVICE_WIDGET
    ui->serialPortCombo->addItems(sPorts);
    _toolBarSerialPortCombo->addItems(sPorts);
}

