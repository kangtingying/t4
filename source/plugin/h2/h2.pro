TEMPLATE=lib
QT       += gui widgets
CONFIG += static

TARGET = ../../../../lib/mrx-h2

INCLUDEPATH += ./widget
INCLUDEPATH += ../model/widget

win32 {
# visa
INCLUDEPATH += "C:/Program Files (x86)/IVI Foundation/VISA/WinNT/Include"
#LIBS += -L"C:/Program Files (x86)/IVI Foundation/VISA/WinNT/lib/msc"
#LIBS += -lvisa32
#DEFINES += NI_VISA
#DEFINES += ARCH_32
}

INCLUDEPATH += "../../widget"

OBJECTS_DIR = ./obj
MOC_DIR = ./obj

SOURCES = \
    h2para.cpp \
    h2.cpp \
    info.cpp \
    h2status.cpp \
    h2oppanel.cpp


HEADERS = \
    h2para.h \
    h2.h \
    info.h \
    h2status.h \
    h2oppanel.h
    
FORMS += \
    h2ops.ui \
    h2status.ui \
    h2info.ui
    

#RESOURCES += ../../../res/res.qrc

