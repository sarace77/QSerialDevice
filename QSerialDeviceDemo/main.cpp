#include <QApplication>
#include "qserialdevicedemo.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSerialDeviceDemo w;
    w.show();
    
    return a.exec();
}
