#include "qserialdevicedemo.h"
#include "ui_qserialdevicedemo.h"



QSerialDeviceDemo::QSerialDeviceDemo(QWidget *parent) : QMainWindow(parent), ui(new Ui::QSerialDeviceDemo) {
    ui->setupUi(this);

    _errorDialog = new QErrorMessage(this);
    _portSettingsDialog = new QDialog(this);
    _serialDevice = new QSerialDevice();

    _errorDialog->setWindowTitle("Error");
    _portSettingsDialog->setWindowTitle("Port Settings");
    _serialDevice->getWidget(_portSettingsDialog);

    ui->centralWidget->setEnabled(false);

    addToolBar(_serialDevice->getToolBar());
    connect(_serialDevice, SIGNAL(msgAvailable(QByteArray)), this, SLOT(read(QByteArray)));
    connect(_serialDevice, SIGNAL(portClosed()), this, SLOT(onPortClosed()));
    connect(_serialDevice, SIGNAL(portOpened()), this, SLOT(onPortOpened()));
}

QSerialDeviceDemo::~QSerialDeviceDemo() {
    delete _errorDialog;
    if(_serialDevice->isOpen())
            _serialDevice->close();
    //delete _serialDevice;
    delete _portSettingsDialog;
    delete ui;
}

void QSerialDeviceDemo::on_actionPortSettings_triggered() {
    _portSettingsDialog->exec();
}

void QSerialDeviceDemo::on_autoReceiveBox_toggled(bool value) {
    if (value)
        ui->readButton->setEnabled(false);
}

void QSerialDeviceDemo::on_readButton_clicked() {
    if (_serialDevice->isOpen()) {
        QByteArray data = _serialDevice->read();
        if (data.isEmpty()) {
            _errorDialog->showMessage("No Data received");
        } else {
            ui->readEdit->setText(QString(data));
            ui->statusBar->showMessage(QString("%1 bytes read").arg(data.size()), 5000);
        }
    } else {
        _errorDialog->showMessage("No Serial port opened!");
    }
    ui->readButton->setEnabled(false);
}

void QSerialDeviceDemo::on_sendButton_clicked() {
    if (_serialDevice->isOpen()) {
        QByteArray data = ui->sendEdit->text().toAscii();
        if (data.isEmpty()) {
            _errorDialog->showMessage("No Data to send");
        } else {
            data.append((0x0a));
            if (_serialDevice->write(data) != data.size()) {
                _errorDialog->showMessage("Error writing data on serial port!");
            } else {
                ui->statusBar->showMessage(QString("%1 bytes sent").arg(data.size()), 5000);
            }
        }
    } else {
        _errorDialog->showMessage("No Serial port opened!");
    }
}

void QSerialDeviceDemo::onPortClosed() {
    ui->centralWidget->setEnabled(false);
    ui->actionPortSettings->setEnabled(true);
}

void QSerialDeviceDemo::onPortOpened() {
    ui->actionPortSettings->setEnabled(false);
    ui->centralWidget->setEnabled(true);
}


void QSerialDeviceDemo::read(QByteArray data) {
    if (ui->autoReceiveBox->isChecked()) {
        ui->readEdit->setText(QString(data));
        _serialDevice->read();
        ui->statusBar->showMessage(QString("%1 bytes read").arg(data.size()), 5000);
    }
    else {
        ui->readEdit->clear();
        ui->readButton->setEnabled(true);
    }
}
