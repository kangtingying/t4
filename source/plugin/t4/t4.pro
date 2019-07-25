TEMPLATE=lib
QT       += gui widgets concurrent network
CONFIG += static
CONFIG += c++11
TARGET = ../../../../lib/mrx-t4

DEFINES += QT_DEPRECATED_WARNINGS

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
INCLUDEPATH += "./widget"

INCLUDEPATH += "../../device/MegaRoboMRHT_SDK/src"

OBJECTS_DIR = ./obj
MOC_DIR = ./obj

SOURCES = \
    t4.cpp \
    t4oppanel.cpp \
    config.cpp \
    motorconfig.cpp \
    actiontable.cpp \
    errormgrtable.cpp \
    controllerstatus.cpp \
    traceplot.cpp \
    joint.cpp \
    model/debugtable.cpp \
#    model/recordtable.cpp \
    dlgio.cpp \
    info.cpp \
    model/iodelegate.cpp \
    widget/codeeditor.cpp\
    iowidget.cpp \
    quadwiget.cpp \
    scripteditor.cpp \
    coordinate.cpp \
    coordpara.cpp \
    t4_device.cpp \
    t4para.cpp \
    t4_serial.cpp \
    advpara.cpp \
    comassist.cpp \
    model\treeitem.cpp \
    model\treemodel.cpp \
    motorcurrent.cpp \
    iostate.cpp \
    regexpinputdialog.cpp \
    widget/ioindicator.cpp \
    widget/keytableview.cpp \
    widget/axislimit.cpp \
    maintainpage.cpp \
    updatedialog.cpp \
    widget/filemanager.cpp \
    hlx.cpp


HEADERS = \
    t4.h \
    t4oppanel.h \
    config.h \
    motorconfig.h \
    actiontable.h \
    errormgrtable.h \
    controllerstatus.h \
    traceplot.h \
    joint.h \
    model/debugtable.h \
#    model/recordtable.h \
    dlgio.h \
    info.h \
    model/iodelegate.h \
    widget/codeeditor.h \
    iowidget.h \
    quadwiget.h \
    scripteditor.h \
    coordinate.h \
    coordpara.h \
    t4para.h \
    advpara.h \
    comassist.h \
    model\treeitem.h \
    model\treemodel.h \
    motorcurrent.h \
    iostate.h \
    regexpinputdialog.h \
    widget/ioindicator.h \
    widget/keytableview.h \
    widget/axislimit.h \
    maintainpage.h \
    updatedialog.h \
    widget/filemanager.h \
    hlx.h

FORMS += \
    config.ui \
    motorconfig.ui \
    actiontable.ui \
    errormgrtable.ui \
    controllerstatus.ui \
    traceplot.ui \
    joint.ui \
    dlgio.ui \
    info.ui \
    iowidget.ui \
    quadwiget.ui \
    scripteditor.ui \
    coordinate.ui \
    coordpara.ui \
    advpara.ui \
    motorcurrent.ui \
    iostate.ui \
    widget/ioindicator.ui \
    t4oppanel.ui \
    widget/axislimit.ui \
    maintainpage.ui \
    updatedialog.ui \
    widget/filemanager.ui \
    hlx.ui

RESOURCES += ../../../res/res.qrc

