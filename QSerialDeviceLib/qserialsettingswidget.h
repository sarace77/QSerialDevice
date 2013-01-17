#ifndef QSERIALSETTINGSWIDGET_H
#define QSERIALSETTINGSWIDGET_H

#include <QWidget>

#include <QtExtSerialPort/qextserialport.h>
#include <QtExtSerialPort/qextserialenumerator.h>

namespace Ui {
class QSerialSettingsWidget;
}

class QSerialSettingsWidget : public QWidget
{
    Q_OBJECT

private:
    QextSerialEnumerator *_serialEnumerator;

private slots:
    void updateDevicesList();

public:
    explicit QSerialSettingsWidget(QWidget *parent = 0);
    ~QSerialSettingsWidget();

    PortSettings getPortSettings();
    QString getPortName();

private:
    Ui::QSerialSettingsWidget *ui;
};

#endif // QSERIALSETTINGSWIDGET_H
