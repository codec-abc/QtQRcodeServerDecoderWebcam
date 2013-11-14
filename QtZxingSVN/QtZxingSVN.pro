#-------------------------------------------------
#
# Project created by QtCreator 2013-11-13T17:18:17
#
#-------------------------------------------------

QT       += core gui widgets multimedia multimediawidgets

TARGET = QtZxingSVN
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    qimagetoluminancesource.cpp

HEADERS  += mainwindow.h \
    qimagetoluminancesource.h

FORMS    += mainwindow.ui

unix|win32: LIBS += -L$$PWD/../../NetBeansProjects/zxingLibSVN/dist/Debug/MinGW-Windows/ -llibZxingSVN



INCLUDEPATH += $$PWD/../../NetBeansProjects/zxingLibSVN/dist/Debug/MinGW-Windows
INCLUDEPATH += $$PWD/../../../../../Users/VIOT/Desktop
INCLUDEPATH += C:\Users\VIOT\Documents\projects\zbar-0.10\include\zbar
INCLUDEPATH += C:\Users\VIOT\Documents\projects\zbar-0.10\include
DEPENDPATH += $$PWD/../../NetBeansProjects/zxingLibSVN/dist/Debug/MinGW-Windows


unix|win32: LIBS += -L$$PWD/../../../Desktop/lib/ -llibzbar-0

INCLUDEPATH += $$PWD/../../../Desktop
DEPENDPATH += $$PWD/../../../Desktop
