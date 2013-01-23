#ifndef QSERIALCONSOLEWINDOW_H
#define QSERIALCONSOLEWINDOW_H

#include <QDialog>
#include <QMainWindow>

#include "consolewidget.h"
#include "protocol.h"
#include "qserialdevice.h"

namespace Ui {
class QSerialConsoleWindow;
}

class QSerialConsoleWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::QSerialConsoleWindow *ui;
    QSerialDevice *_serialPort;
    ConsoleWidget *_consoleWidget;
    QDialog *_portSettingsDialog;

    Protocol *_protocol;

private slots:
    void on_actionASCII_triggered();
    void on_actionNone_triggered();
    void on_actionPortSettings_triggered();
    void portClosed();
    void portOpened();

public:
    explicit QSerialConsoleWindow(QWidget *parent = 0);
    ~QSerialConsoleWindow();

signals:
    void protocolChanged(Protocol *proto);
};

#endif // QSERIALCONSOLEWINDOW_H
