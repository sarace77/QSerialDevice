#ifndef QSERIALDEVICEDEMO_H
#define QSERIALDEVICEDEMO_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QErrorMessage>
#include <QMainWindow>
#include <QVBoxLayout>


#include "qserialdevice.h"

namespace Ui {
class QSerialDeviceDemo;
}

class QSerialDeviceDemo : public QMainWindow
{
    Q_OBJECT

private:
    Ui::QSerialDeviceDemo *ui;
    QErrorMessage *_errorDialog;
    QDialog *_portSettingsDialog;
    QSerialDevice *_serialDevice;

    QDialogButtonBox *_dialogButtonBox;
    QVBoxLayout *_mainLayout;

private slots:
    void on_actionPortSettings_triggered();
    void on_autoReceiveBox_toggled(bool value);
    void on_readButton_clicked();
    void on_sendButton_clicked();
    void onPortClosed();
    void onPortOpened();
    void read(QByteArray data);
public:
    explicit QSerialDeviceDemo(QWidget *parent = 0);
    ~QSerialDeviceDemo();
    
};

#endif // QSERIALDEVICEDEMO_H
