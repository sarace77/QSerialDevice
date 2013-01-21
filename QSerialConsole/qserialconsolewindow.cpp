#include "qserialconsolewindow.h"
#include "ui_qserialconsolewindow.h"

QSerialConsoleWindow::QSerialConsoleWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QSerialConsoleWindow)
{
    ui->setupUi(this);
    _serialPort = new QSerialDevice();
    addToolBar(_serialPort->getToolBar());

    _consoleWidget = new ConsoleWidget(ui->consoleFrame);
    _consoleWidget->printBanner("Welcome to QSerialConsole v 1.0\n");
    portClosed();

    connect(_serialPort, SIGNAL(portOpened()), this, SLOT(portOpened()));
    connect(_serialPort, SIGNAL(portClosed()), this, SLOT(portClosed()));
    connect(_serialPort, SIGNAL(msgAvailable(QByteArray)), _consoleWidget, SLOT(printIncomingMessage(QByteArray)));
    connect(_consoleWidget, SIGNAL(newCmdSent(QByteArray)), _serialPort, SLOT(write(QByteArray)));
}

QSerialConsoleWindow::~QSerialConsoleWindow()
{
    delete _consoleWidget;
    delete ui;
}

void QSerialConsoleWindow::portClosed() {
    ui->consoleFrame->setEnabled(false);
}

void QSerialConsoleWindow::portOpened() {
    ui->consoleFrame->setEnabled(true);
    _consoleWidget->printBanner("Serial Port succesfully opened!\n\n");
}
