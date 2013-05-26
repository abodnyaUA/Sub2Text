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
win32 {
#OS WIN
RC_FILE = myapp.rc
 }

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui


TRANSLATIONS += lng/ru.ts \
                lng/uk.ts

RESOURCES += \
    res.qrc
