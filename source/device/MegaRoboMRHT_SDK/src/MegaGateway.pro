QT       -= core gui
TEMPLATE = lib
TARGET = MegaGateway

VERSION = 1.0.0

###打开后只生成静态库
#CONFIG += staticlib

#DEFINES += ARM_LINUX    # 表示arm上的Linux,比如树莓派
#DEFINES += ANDROID      # 表示安卓

unix {
    CONFIG += plugin
    contains(DEFINES, ANDROID) {
        message( "Configuring for android" )
        DESTDIR = $$PWD/../lib/android
    }
    else {
        contains(DEFINES, ARM_LINUX) {
            message( "Configuring for arm_Linux" )
            DESTDIR = $$PWD/../lib/armlinux
        }
        else{
            message( "Configuring for x86_Linux" )
            DESTDIR = $$PWD/../lib/linux
        }
    }
}

win32 {
    message( "Configuring for x86_windows" )
    message( "Use visa" )
    TARGET = MegaGateway-
    DESTDIR = $$PWD/../lib/windows
}

######################## SRC ##############################
unix {
    contains(DEFINES, ANDROID) {
        message( "Use TCP socket" )
    }
    else {
        message( "Use vxi11" )
        DEFINES += _VXI11_
        INCLUDEPATH += $$PWD/lib/vxi11
        HEADERS += $$PWD/vxi11/vxi11.h
        HEADERS += $$PWD/vxi11/vxi11_user.h
        SOURCES += $$PWD/vxi11/vxi11_clnt.c
        SOURCES += $$PWD/vxi11/vxi11_user.c
        SOURCES += $$PWD/vxi11/vxi11_xdr.c
    }
}

INCLUDEPATH += $$PWD
SOURCES +=  \
        $$PWD/bus.c \
        $$PWD/device.c \
        $$PWD/errorcode.c \
        $$PWD/megarobot.c \
        $$PWD/mrqdevice.c \
        $$PWD/project.c \
        $$PWD/storage.c \
        $$PWD/system.c

HEADERS += \
        $$PWD/bus.h \
        $$PWD/device.h \
        $$PWD/errorcode.h \
        $$PWD/MegaGateway.h \
        $$PWD/megarobot.h \
        $$PWD/mrqdevice.h \
        $$PWD/platform.h \
        $$PWD/project.h \
        $$PWD/storage.h \
        $$PWD/system.h

################### LIBS ###################
win32 {
INCLUDEPATH += $$PWD/../win
DEPENDPATH += $$PWD/../win

LIBS += -L$$PWD/../win -llibws2_32
PRE_TARGETDEPS += $$PWD/../win/libws2_32.a

LIBS += -L$$PWD/../win/ -llibiphlpapi
PRE_TARGETDEPS += $$PWD/../win/libiphlpapi.a

LIBS += -L$$PWD/../win/ -lvisa32
PRE_TARGETDEPS += $$PWD/../win/visa32.lib

LIBS += -L$$PWD/../win/ -llibmsvcr100
PRE_TARGETDEPS += $$PWD/../win/libmsvcr100.a
}


