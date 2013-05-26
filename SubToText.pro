#-------------------------------------------------
#
# Project created by QtCreator 2013-05-26T19:29:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SubToText
TEMPLATE = app
unix {
    target.path = /usr/local/bin
}

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui


TRANSLATIONS += lng/ru.ts \
                lng/uk.ts


win32 {
#OS WIN
RC_FILE = winicon.rc
 }

RESOURCES += \
    res.qrc

OTHER_FILES += \
    winicon.rc
