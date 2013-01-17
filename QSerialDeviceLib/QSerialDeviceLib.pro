TEMPLATE = lib

HEADERS += \
    qserialdevice.h \
    qserialsettingswidget.h

SOURCES += \
    qserialdevice.cpp \
    qserialsettingswidget.cpp

FORMS += \
    qserialsettingswidget.ui

unix|win32: LIBS += -lqextserialport
