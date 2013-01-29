#ifndef QSERIALCONSOLEWINDOW_H
#define QSERIALCONSOLEWINDOW_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QMainWindow>
#include <QVBoxLayout>

#include "qconsolewidget.h"
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
    QConsoleWidget *_consoleWidget;
    QDialog *_portSettingsDialog;

    Protocol *_protocol;

    QDialogButtonBox *_dialogButtonBox;
    QVBoxLayout *_mainLayout;

private slots:
    void on_actionASCII_triggered();
    void on_actionNone_triggered();
    void on_actionPortSettings_triggered();
    void portClosed();
    void portOpened();

public:
    explicit QSerialConsoleWindow(QWidget *parent = 0);
    ~QSerialConsoleWindow();
};

#endif // QSERIALCONSOLEWINDOW_H
