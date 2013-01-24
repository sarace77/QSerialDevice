#-------------------------------------------------
#
# Project created by QtCreator 2013-01-18T14:55:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QSerialConsole
TEMPLATE = app

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../QSerialDeviceLib/release/ -lQSerialDeviceLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../QSerialDeviceLib/debug/ -lQSerialDeviceLibd
else:unix:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../QSerialDeviceLib/ -lQSerialDeviceLib
else:unix:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../QSerialDeviceLib/ -lQSerialDeviceLib_debug
INCLUDEPATH += $$PWD/../QSerialDeviceLib
DEPENDPATH += $$PWD/../QSerialDeviceLib


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../QConsoleWidget/release/ -lQConsoleWidget
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../QConsoleWidget/debug/ -lQConsoleWidgetd
else:unix:CONFIG(release, debug|release) LIBS += -L$$OUT_PWD/../QConsoleWidget/ -lQConsoleWidget
else:unix:CONFIG(debug, debug|release) LIBS += -L$$OUT_PWD/../QConsoleWidget/ -lQConsoleWidget_debug

INCLUDEPATH += $$PWD/../QConsoleWidget
DEPENDPATH += $$PWD/../QConsoleWidget



SOURCES += main.cpp\
        qserialconsolewindow.cpp

HEADERS  += qserialconsolewindow.h

FORMS    += qserialconsolewindow.ui

