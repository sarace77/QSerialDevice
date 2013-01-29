#include "qserialconsolewindow.h"
#include "ui_qserialconsolewindow.h"

#include "protocol_ascii.h"

QSerialConsoleWindow::QSerialConsoleWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QSerialConsoleWindow)
{
    ui->setupUi(this);
    _portSettingsDialog = new QDialog(this);
    _dialogButtonBox = new QDialogButtonBox(_portSettingsDialog);
    _mainLayout = new QVBoxLayout(_portSettingsDialog);
    _serialPort = new QSerialDevice();
    addToolBar(_serialPort->toolbar());
    _dialogButtonBox->addButton(QDialogButtonBox::Ok);
    _portSettingsDialog->setWindowTitle("Port Settings");
    _mainLayout->addWidget(_serialPort->widget());
    _mainLayout->addWidget(_dialogButtonBox);
    _protocol = new Protocol();

    _consoleWidget = new QConsoleWidget(ui->consoleFrame);
    _consoleWidget->showBanner("Welcome to QSerialConsole v 1.0\n");
    portClosed();

    connect(_serialPort, SIGNAL(portOpened()), this, SLOT(portOpened()));
    connect(_serialPort, SIGNAL(portClosed()), this, SLOT(portClosed()));
    connect(_serialPort, SIGNAL(msgAvailable(QByteArray)), _protocol, SLOT(decode(QByteArray)));
    connect(_protocol, SIGNAL(dataDecoded(QByteArray)), _consoleWidget, SLOT(showIncomingMessage(QByteArray)));
    connect(_consoleWidget, SIGNAL(newOutgoingMessage(QByteArray)), _protocol, SLOT(encode(QByteArray)));
    connect(_protocol, SIGNAL(dataEncoded(QByteArray)), _serialPort, SLOT(write(QByteArray)));
    connect(_dialogButtonBox, SIGNAL(accepted()), _portSettingsDialog, SLOT(close()));
}

QSerialConsoleWindow::~QSerialConsoleWindow()
{
    if (_protocol)
        delete _protocol;
    delete _serialPort;
    delete ui;
}

void QSerialConsoleWindow::on_actionASCII_triggered() {
    disconnect(_serialPort, SIGNAL(msgAvailable(QByteArray)), _protocol, SLOT(decode(QByteArray)));
    disconnect(_protocol, SIGNAL(dataDecoded(QByteArray)), _consoleWidget, SLOT(showIncomingMessage(QByteArray)));
    disconnect(_consoleWidget, SIGNAL(newOutgoingMessage(QByteArray)), _protocol, SLOT(encode(QByteArray)));
    disconnect(_protocol, SIGNAL(dataEncoded(QByteArray)), _serialPort, SLOT(write(QByteArray)));
    if (_protocol)
        delete _protocol;
    _protocol = new Protocol_ASCII();
    connect(_serialPort, SIGNAL(msgAvailable(QByteArray)), _protocol, SLOT(decode(QByteArray)));
    connect(_protocol, SIGNAL(dataDecoded(QByteArray)), _consoleWidget, SLOT(showIncomingMessage(QByteArray)));
    connect(_consoleWidget, SIGNAL(newOutgoingMessage(QByteArray)), _protocol, SLOT(encode(QByteArray)));
    connect(_protocol, SIGNAL(dataEncoded(QByteArray)), _serialPort, SLOT(write(QByteArray)));
    _consoleWidget->refreshProtocolTags(_protocol->tagsList());
    ui->actionNone->setChecked(false);
    _consoleWidget->setFocus();
}

void QSerialConsoleWindow::on_actionNone_triggered() {
    disconnect(_serialPort, SIGNAL(msgAvailable(QByteArray)), _protocol, SLOT(decode(QByteArray)));
    disconnect(_protocol, SIGNAL(dataDecoded(QByteArray)), _consoleWidget, SLOT(showIncomingMessage(QByteArray)));
    disconnect(_consoleWidget, SIGNAL(newOutgoingMessage(QByteArray)), _protocol, SLOT(encode(QByteArray)));
    disconnect(_protocol, SIGNAL(dataEncoded(QByteArray)), _serialPort, SLOT(write(QByteArray)));
    if (_protocol)
        delete _protocol;
    _protocol = new Protocol();
    connect(_serialPort, SIGNAL(msgAvailable(QByteArray)), _protocol, SLOT(decode(QByteArray)));
    connect(_protocol, SIGNAL(dataDecoded(QByteArray)), _consoleWidget, SLOT(showIncomingMessage(QByteArray)));
    connect(_consoleWidget, SIGNAL(newOutgoingMessage(QByteArray)), _protocol, SLOT(encode(QByteArray)));
    connect(_protocol, SIGNAL(dataEncoded(QByteArray)), _serialPort, SLOT(write(QByteArray)));
    _consoleWidget->refreshProtocolTags(_protocol->tagsList());
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
