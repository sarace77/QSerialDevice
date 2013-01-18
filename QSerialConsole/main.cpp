#include <QApplication>
#include "qserialconsolewindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSerialConsoleWindow w;
    w.show();
    
    return a.exec();
}
