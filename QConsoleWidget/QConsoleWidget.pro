TEMPLATE = lib

CONFIG += debug_and_release

HEADERS += \
    qconsolewidget.h

SOURCES += \
    qconsolewidget.cpp

CONFIG(debug, debug|release) {
    DEFINES += _DEBUG_QCONSOLEWIDGET
    unix: TARGET = $$join(TARGET,,,_debug)
    win32: TARGET = $$join(TARGET,,d)
}
