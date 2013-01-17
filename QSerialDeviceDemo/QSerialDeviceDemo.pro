#-------------------------------------------------
#
# Project created by QtCreator 2013-01-17T09:51:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QSerialDeviceDemo
TEMPLATE = app


SOURCES += main.cpp\
        qserialdevicedemo.cpp

HEADERS  += qserialdevicedemo.h

FORMS    += qserialdevicedemo.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../QSerialDeviceLib/release/ -lQSerialDeviceLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../QSerialDeviceLib/debug/ -lQSerialDeviceLib
else:symbian: LIBS += -lQSerialDeviceLib
else:unix: LIBS += -L$$OUT_PWD/../QSerialDeviceLib/ -lQSerialDeviceLib

INCLUDEPATH += $$PWD/../QSerialDeviceLib
DEPENDPATH += $$PWD/../QSerialDeviceLib
