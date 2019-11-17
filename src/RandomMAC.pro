#-------------------------------------------------
#
# Project created by QtCreator 2019-11-14T03:02:53
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RandomMAC
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

win32-msvc* {
    RC_ICONS = images/icon.ico
    CONFIG += embed_manifest_exe
    QMAKE_LFLAGS_WINDOWS += /MANIFESTUAC:level=\'requireAdministrator\'
    win32:RC_FILE = proj.rc

}

win32-g++ {
    RC_ICONS = images/icon.ico
    CONFIG += embed_manifest_exe
    win32:RC_FILE = proj.rc

}

SOURCES += \
        MyMACAddr.cpp \
        dialog.cpp \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        MyMACAddr.h \
        dialog.h \
        mainwindow.h \
        settings.h \
        version.h

FORMS += \
        dialog.ui \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


win32: LIBS += -LC:\\WinSDK\\10.0.17763.0\\um\\x86 \
            -liphlpapi \
            -lAdvAPI32 \
             -lOle32

RESOURCES += \
    asset.qrc
