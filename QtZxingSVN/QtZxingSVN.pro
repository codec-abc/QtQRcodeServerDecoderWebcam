#-------------------------------------------------
#
# Project created by QtCreator 2013-11-13T17:18:17
#
#-------------------------------------------------

QT       += core gui widgets multimedia multimediawidgets network

TARGET = QtZxingSVN
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    qimagetoluminancesource.cpp \
    httpdeamon.cpp \
    threadtest.cpp

HEADERS  += mainwindow.h \
    qimagetoluminancesource.h \
    httpdeamon.h \
    threadtest.h

FORMS    += mainwindow.ui


unix|win32: LIBS += -L$$PWD/../lib/zbarWin32sharedMinGW/ -llibzbar-0

INCLUDEPATH += $$PWD/../zbarSource/include/zbar
INCLUDEPATH += $$PWD/../zbarSource/include
DEPENDPATH += $$PWD/../zbarSource/include/zbar


unix|win32: LIBS += -L$$PWD/../lib/zxingWin32sharedMinGW/ -llibZxingSVN

INCLUDEPATH += $$PWD/../zxingSource
DEPENDPATH += $$PWD/../zxingSource

DEPENDPATH += -L$$PWD/../lib
LIBS+= -L$$PWD/../lib
