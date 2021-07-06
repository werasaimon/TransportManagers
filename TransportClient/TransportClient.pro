QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
TARGET = TransportClient

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../Common/blockreader.cpp \
    ../Common/blockwriter.cpp \
    ../Common/order.cpp \
    ../Common/qjsontablemodel.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ../Common/blockreader.h \
    ../Common/blockwriter.h \
    ../Common/help_func.hpp \
    ../Common/order.h \
    ../Common/qjsontablemodel.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
