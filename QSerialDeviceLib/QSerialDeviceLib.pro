TEMPLATE = lib

HEADERS += \
    qserialdevice.h \
    qserialsettingswidget.h \
    hled.h

SOURCES += \
    qserialdevice.cpp \
    qserialsettingswidget.cpp \
    hled.cpp

FORMS += \
    qserialsettingswidget.ui

unix|win32: LIBS += -lqextserialport
