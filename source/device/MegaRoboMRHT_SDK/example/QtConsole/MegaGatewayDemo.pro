QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    $$PWD/../../src/bus.c \
    $$PWD/../../src/device.c \
    $$PWD/../../src/errorcode.c \
    $$PWD/../../src/megarobot.c \
    $$PWD/../../src/mrqdevice.c \
    $$PWD/../../src/project.c \
    $$PWD/../../src/storage.c \
    $$PWD/../../src/system.c

HEADERS += \
    $$PWD/../../src/bus.h \
    $$PWD/../../src/device.h \
    $$PWD/../../src/errorcode.h \
    $$PWD/../../src/MegaGateway.h \
    $$PWD/../../src/megarobot.h \
    $$PWD/../../src/mrqdevice.h \
    $$PWD/../../src/platform.h \
    $$PWD/../../src/project.h \
    $$PWD/../../src/storage.h \
    $$PWD/../../src/system.h

INCLUDEPATH += $$PWD/../../src

win32 {
INCLUDEPATH += $$PWD/../../win
DEPENDPATH += $$PWD/../../win
LIBS += -L$$PWD/../../win -llibiphlpapi -llibmsvcr100
LIBS += -L$$PWD/../../win -llibws2_32
LIBS += -L$$PWD/../../win -lvisa32
}

unix {
INCLUDEPATH += $$PWD/../../src/vxi11
HEADERS += $$PWD/../../src/vxi11/vxi11.h
HEADERS += $$PWD/../../src/vxi11/vxi11_user.h
SOURCES += $$PWD/../../src/vxi11/vxi11_clnt.c
SOURCES += $$PWD/../../src/vxi11/vxi11_user.c
SOURCES += $$PWD/../../src/vxi11/vxi11_xdr.c
}


