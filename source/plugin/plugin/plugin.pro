TEMPLATE=lib
QT       += gui widgets
CONFIG += static

TARGET = ../../../../lib/xplugin

INCLUDEPATH += "../../device/MegaRoboMRHT_SDK/src"
win32 {
# visa
INCLUDEPATH += "C:/Program Files (x86)/IVI Foundation/VISA/WinNT/Include"
#LIBS += -L"C:/Program Files (x86)/IVI Foundation/VISA/WinNT/lib/msc"
#LIBS += -lvisa32
#DEFINES += NI_VISA
#DEFINES += ARCH_32
}

OBJECTS_DIR = ./obj
MOC_DIR = ./obj

SOURCES = \
    xplugin.cpp \
    xpage.cpp \
    xpluginintf.cpp \
    xpluginworkingthread.cpp \
    mlog.cpp

HEADERS = \
    xplugin.h \
    xpage.h \
    xpluginintf.h \
    xpluginworkingthread.h \
    mlog.h



