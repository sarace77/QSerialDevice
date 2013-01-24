#include "qserialconsolewindow.h"
#include "ui_qserialconsolewindow.h"

#include "protocol_ascii.h"

QSerialConsoleWindow::QSerialConsoleWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QSerialConsoleWindow)
{
    ui->setupUi(this);
    _portSettingsDialog = new QDialog(this);
    _serialPort = new QSerialDevice();
    addToolBar(_serialPort->getToolBar());
    _portSettingsDialog->setWindowTitle("Port Settings");
    _serialPort->getWidget(_portSettingsDialog);

    _protocol = NULL;

    _consoleWidget = new QConsoleWidget(ui->consoleFrame);
    _consoleWidget->showBanner("Welcome to QSerialConsole v 1.0\n");
    portClosed();

    connect(_serialPort, SIGNAL(portOpened()), this, SLOT(portOpened()));
    connect(_serialPort, SIGNAL(portClosed()), this, SLOT(portClosed()));
    connect(_serialPort, SIGNAL(msgAvailable(QByteArray)), this, SLOT(incomingMessage(QByteArray)));
    connect(_consoleWidget, SIGNAL(newOutgoingMessage(QByteArray)), _serialPort, SLOT(write(QByteArray)));
    //connect(this, SIGNAL(protocolChanged(Protocol*)), _consoleWidget, SLOT(protocolChanged(Protocol*)));
}

QSerialConsoleWindow::~QSerialConsoleWindow()
{
    if (_protocol)
        delete _protocol;
    delete _consoleWidget;
    delete ui;
}

void QSerialConsoleWindow::incomingMessage(QByteArray msg) {
    if (_protocol)
        msg = _protocol->decode(msg);
    _consoleWidget->showIncomingMessage(msg);
}

void QSerialConsoleWindow::on_actionASCII_triggered() {
    if (_protocol)
        delete _protocol;
    _protocol = new Protocol_ASCII();
    _serialPort->setProtocol(_protocol);
    _consoleWidget->refreshProtocolTags(_protocol->getTagList());
    ui->actionNone->setChecked(false);
    _consoleWidget->setFocus();
}

void QSerialConsoleWindow::on_actionNone_triggered() {
    if (_protocol)
        delete _protocol;
    _protocol = NULL;
    _serialPort->setProtocol(_protocol);
    _consoleWidget->refreshProtocolTags(_protocol->getTagList());
    ui->actionASCII->setChecked(false);
    _consoleWidget->setFocus();
}

void QSerialConsoleWindow::on_actionPortSettings_triggered() {
    _portSettingsDialog->exec();
}

void QSerialConsoleWindow::portClosed() {
    ui->actionPortSettings->setEnabled(true);
    ui->consoleFrame->setEnabled(false);
}

void QSerialConsoleWindow::portOpened() {
    ui->actionPortSettings->setEnabled(false);
    ui->consoleFrame->setEnabled(true);
    _consoleWidget->showBanner("Serial Port succesfully opened!\n\n");
    _consoleWidget->setFocus();
}
