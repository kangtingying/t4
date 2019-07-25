TEMPLATE=lib
CONFIG += static
CONFIG += c++11
QT += core

TARGET = ../../../lib/mrx-device

DEFINES += _LIB_DEVICE

win32 {
# visa
INCLUDEPATH += "C:/Program Files (x86)/IVI Foundation/VISA/WinNT/Include"
#LIBS += -L"C:/Program Files (x86)/IVI Foundation/VISA/WinNT/lib/msc"
#LIBS += -lvisa32
#DEFINES += NI_VISA
#DEFINES += ARCH_32
}

INCLUDEPATH += ./MegaRoboMRHT_SDK/src

OBJECTS_DIR = ./obj
MOC_DIR = ./obj

SOURCES = \
    local.cpp \
    assist.cpp
HEADERS = \ 
    assist.h

linux-arm* {
SOURCES += ./vxi11/vxi11_clnt.c \
            ./vxi11/vxi11_user.c \
            ./vxi11/vxi11_xdr.c
}

