#ifndef QSERIALSETTINGSWIDGET_H
#define QSERIALSETTINGSWIDGET_H

#include <QWidget>
#include <QComboBox>

#include <QtExtSerialPort/qextserialport.h>
#include <QtExtSerialPort/qextserialenumerator.h>

namespace Ui {
class QSerialSettingsWidget;
}

class QSerialSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QSerialSettingsWidget(QWidget *parent = 0);
    ~QSerialSettingsWidget();

    PortSettings getPortSettings();
    QString getPortName();
    QWidget *getPortWidget();

    void setPortName(QString sName);
    void setPortSettings(QStringList sSettings);

    static QStringList settings2QStringList(PortSettings pSettings);

private:
    Ui::QSerialSettingsWidget *ui;
    QextSerialEnumerator *_serialEnumerator;
#ifdef _DEBUG_QSERIALDEVICE_WIDGET
    bool _isFirstTime;
#endif //_DEBUG_QSERIALDEVICE_WIDGET
    QComboBox *_toolBarSerialPortCombo;

private slots:
    void updateDevicesList();

};

#endif // QSERIALSETTINGSWIDGET_H
