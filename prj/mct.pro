#-------------------------------------------------
#
# Project created by QtCreator 2018-10-25T16:00:52
#
#-------------------------------------------------

QT       += core gui charts xml network

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = mct
TEMPLATE = app
#qt >= 5.10
VERSION = 0.0.1.6

CONFIG += C++11 #use lambda

#release:DEFINES += QT_NO_WARNING_OUTPUT \
#                   QT_NO_DEBUG_OUTPUT

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
#DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += ../include/mystd.h           \
    ../include/mydebug.h                \
    ../include/mydef.h                  \
    ../source/sys/sysapi.h              \
    ../source/sys/xthread.h             \
    ../source/mrp/mdatarow.h            \
    ../source/mrp/mdataset.h            \
    ../source/wnd/mainwindow.h          \
    ../source/wnd/aboutdlg.h            \
    ../source/wnd/roboconfig.h          \
    ../source/wnd/helppanel.h           \
    ../source/widget/megalcdnumber.h    \
    ../source/widget/megamessagebox.h   \
    ../source/widget/megasplinechart.h  \
    ../source/widget/megainterface.h \
    ../source/wnd/login.h \
    ../source/wnd/syslogout.h \
    ../source/wnd/syspref.h \
    ../source/wnd/syspara.h \
    ../source/wnd/welcomepage.h \
    ../source/wnd/stopwidget.h \
    ../source/wnd/prompt.h \
    ../source/wnd/changedpw.h \
    ../source/wnd/spacewidget.h

SOURCES += ../source/main/main.cpp      \
    ../source/sys/sysapi.cpp            \
    ../source/sys/xthread.cpp           \
    ../source/mrp/mdatarow.cpp          \
    ../source/mrp/mdataset.cpp          \
    ../source/wnd/mainwindow.cpp        \
    ../source/wnd/aboutdlg.cpp          \
    ../source/wnd/helppanel.cpp         \
    ../source/wnd/roboconfig.cpp        \
    ../source/widget/megalcdnumber.cpp  \
    ../source/widget/megamessagebox.cpp \
    ../source/widget/megasplinechart.cpp\
    ../source/widget/megainterface.cpp \
    ../source/wnd/login.cpp \
    ../source/wnd/syslogout.cpp \
    ../source/wnd/syspref.cpp \
    ../source/wnd/syspara.cpp \
    ../source/wnd/welcomepage.cpp \
    ../source/wnd/stopwidget.cpp \
    ../source/wnd/prompt.cpp \
    ../source/wnd/changedpw.cpp \
    ../source/wnd/spacewidget.cpp

!win32: HEADERS += ../source/device/vxi11/vxi11.h
!win32: HEADERS += ../source/device/vxi11/vxi11_user.h
!win32: SOURCES += ../source/device/vxi11/vxi11_clnt.c
!win32: SOURCES += ../source/device/vxi11/vxi11_user.c
!win32: SOURCES += ../source/device/vxi11/vxi11_xdr.c


FORMS += ../source/wnd/mainwindow.ui    \
    ../source/wnd/aboutdlg.ui           \
    ../source/wnd/helppanel.ui          \
    ../source/wnd/roboconfig.ui         \
    ../source/widget/megainterface.ui \
    ../source/wnd/login.ui \
    ../source/wnd/syslogout.ui \
    ../source/wnd/syspref.ui \
    ../source/wnd/welcomepage.ui \
    ../source/wnd/stopwidget.ui \
    ../source/wnd/prompt.ui \
    ../source/wnd/changedpw.ui \
    ../source/wnd/spacewidget.ui

INCLUDEPATH += ../include
INCLUDEPATH += ../source/sys
INCLUDEPATH += ../source/mrp
INCLUDEPATH += ../source/device
INCLUDEPATH += ../source/model
INCLUDEPATH += ../source/wnd
INCLUDEPATH += ../source/widget
INCLUDEPATH += ../source/plugin/model/widget
INCLUDEPATH += ../source/plugin


win32:INCLUDEPATH += "C:/Program Files (x86)/IVI Foundation/VISA/WinNT/Include"

INCLUDEPATH += "../source/device/MegaRoboMRHT_SDK/src"

# libs
LIBS += -L"../lib"

LIBS += -lxfactory -lmrx-t4 -lmrx-h2 -lxplugin
LIBS += -lmrx-device
LIBS += -lmodel

win32:LIBS += -L"C:/Program Files (x86)/IVI Foundation/VISA/WinNT/lib/msc"
win32:LIBS += -lvisa32

win32:LIBS += -L"../3rdlib"
win32:LIBS += -llibws2_32 -llibiphlpapi


RESOURCES += ../res/res.qrc

RC_ICONS = ../res/image/megarobo.ico

DISTFILES += ../build/doc/readme.txt \
             ../build/doc/errant.txt \
             ../build/doc/faq.txt

#TRANSLATIONS += ../res/ts/mct_zh_CN.ts \
#                ../res/ts/mct_EN.ts \
#                ../res/ts/mct_zh_TW.ts

