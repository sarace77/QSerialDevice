#ifndef QSERIALCONSOLEWINDOW_H
#define QSERIALCONSOLEWINDOW_H

#include <QMainWindow>

#include "qserialdevice.h"
#include "consolewidget.h"

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

private slots:
    void portClosed();
    void portOpened();

public:
    explicit QSerialConsoleWindow(QWidget *parent = 0);
    ~QSerialConsoleWindow();
    
};

#endif // QSERIALCONSOLEWINDOW_H
