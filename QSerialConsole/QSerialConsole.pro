#-------------------------------------------------
#
# Project created by QtCreator 2013-01-18T14:55:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QSerialConsole
TEMPLATE = app


SOURCES += main.cpp\
        qserialconsolewindow.cpp \
    consolewidget.cpp

HEADERS  += qserialconsolewindow.h \
    consolewidget.h

FORMS    += qserialconsolewindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../QSerialDeviceLib/release/ -lQSerialDeviceLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../QSerialDeviceLib/debug/ -lQSerialDeviceLibd
else:unix:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../QSerialDeviceLib/ -lQSerialDeviceLib
else:unix:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../QSerialDeviceLib/ -lQSerialDeviceLib_debug

INCLUDEPATH += $$PWD/../QSerialDeviceLib
DEPENDPATH += $$PWD/../QSerialDeviceLib

CONFIG += debug_and_release
CONFIG(debug, debug|release) {
    DEFINES += _DEBUG_QSERIALDEVICE_LIB \
                _DEBUG_QSERIALDEVICE_WIDGET
}
