QT += core gui widgets xml network

CONFIG += c++11
TEMPLATE += app

TARGET = MRHT_SDK_Demo
VERSION = 00.01.00.00

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
#DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

################### QtGUI ###################
INCLUDEPATH += ./mainwindow
INCLUDEPATH += ./widget
INCLUDEPATH += ./dialog
INCLUDEPATH += ./thread

SOURCES +=  \
    mainwindow/mainwindow.cpp   \
    widget/mainwidget.cpp       \
    dialog/TestPanelDialog.cpp \
    dialog/CanDebugDialog.cpp

HEADERS += \
    mainwindow/mainwindow.h     \
    widget/mainwidget.h         \
    dialog/TestPanelDialog.h \
    dialog/CanDebugDialog.h

FORMS += \
    mainwindow/mainwindow.ui    \
    widget/mainwidget.ui        \
    dialog/TestPanelDialog.ui \
    dialog/CanDebugDialog.ui


RC_ICONS = resource/app.ico

RESOURCES += \
    resource/res.qrc

#DEFINES += ARM_LINUX    # 表示arm上的Linux,比如树莓派
#DEFINES += ANDROID      # 表示安卓

unix {
    contains(DEFINES, ANDROID) {
        message( "Configuring for android" )
    }
    else {
        contains(DEFINES, ARM_LINUX) {
            message( "Configuring for arm_Linux" )
        }
        else{
            message( "Configuring for x86_Linux" )
        }
    }
}

win32 {
    message( "Configuring for x86_windows" )
    message( "Use visa" )
}

################### SDK ###################
unix {
    contains(DEFINES, ANDROID) {
        message( "Use TCP socket" )
    }
    else {
        message( "Use vxi11" )
        DEFINES += _VXI11_
        HEADERS += ../../src/vxi11/vxi11.h
        HEADERS += ../../src/vxi11/vxi11_user.h
        SOURCES += ../../src/vxi11/vxi11_clnt.c
        SOURCES += ../../src/vxi11/vxi11_user.c
        SOURCES += ../../src/vxi11/vxi11_xdr.c
    }
}

INCLUDEPATH += ../../src/
SOURCES += ./main.cpp \
        ../../src/bus.c \
        ../../src/device.c \
        ../../src/megarobot.c \
        ../../src/mrqdevice.c \
        ../../src/project.c \
        ../../src/storage.c \
        ../../src/system.c \
        ../../src/common.c  \
    thread/xthread.cpp \
    sysapi.cpp

HEADERS += \
        ../../src/bus.h \
        ../../src/device.h \
        ../../src/MegaGateway.h \
        ../../src/megarobot.h \
        ../../src/mrqdevice.h \
        ../../src/platform.h \
        ../../src/project.h \
        ../../src/storage.h \
        ../../src/system.h \
        ../../src/megatype.h \
        ../../src/common.h \
    thread/xthread.h \
    sysapi.h

################### LIBS ###################
win32 {
INCLUDEPATH += $$PWD/../../win
DEPENDPATH += $$PWD/../../win

LIBS += -L$$PWD/../../win -llibws2_32
PRE_TARGETDEPS += $$PWD/../../win/libws2_32.a

LIBS += -L$$PWD/../../win/ -llibiphlpapi
PRE_TARGETDEPS += $$PWD/../../win/libiphlpapi.a

LIBS += -L$$PWD/../../win/ -lvisa32
PRE_TARGETDEPS += $$PWD/../../win/visa32.lib

LIBS += -L$$PWD/../../win/ -llibmsvcr100
PRE_TARGETDEPS += $$PWD/../../win/libmsvcr100.a
}

CONFIG(debug, debug|release) {
    DEFINES += MRG_DEBUG
}
