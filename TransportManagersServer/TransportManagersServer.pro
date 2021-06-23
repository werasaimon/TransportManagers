QT += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialogrecorteditform.cpp \
    main.cpp \
    mainwindow.cpp \
    server/myclient.cpp \
    server/myserver.cpp \
    server/mytask.cpp


HEADERS += \
    dialogrecorteditform.h \
    mainwindow.h \
    server/myclient.h \
    server/myserver.h \
    server/mytask.h


FORMS += \
    dialogrecorteditform.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


#win32:CONFIG(release, debug|release): LIBS += -LC:/Program Files/MySQL/MySQL Server 8.0/lib  -llibmysql.lib
#else:win32:CONFIG(debug, debug|release): LIBS += -LC:/Program Files/MySQL/MySQL Server 8.0/lib -llibmysql.lib
#else:unix: LIBS += -L$$PWD/'../../../../Program Files/MySQL/MySQL Server 8.0/lib/' -llibmysql

#INCLUDEPATH += $$PWD/'../../../../Program Files/MySQL/MySQL Server 8.0/include'
#DEPENDPATH += $$PWD/'../../../../Program Files/MySQL/MySQL Server 8.0/include'

#win32:CONFIG(release, debug|release): LIBS += -LC:/Program Files/MySQL/MySQL Server 8.0/lib  -lmysqlclient.lib
#else:win32:CONFIG(debug, debug|release): LIBS += -LC:/Program Files/MySQL/MySQL Server 8.0/lib  -lmysqlclient.lib
#else:unix: LIBS += -LC:/Program Files/MySQL/MySQL Server 8.0/lib  -lmysqlclient

#INCLUDEPATH += $$PWD/'../../../../Program Files/MySQL/MySQL Server 8.0/include'
#DEPENDPATH += $$PWD/'../../../../Program Files/MySQL/MySQL Server 8.0/include'

RESOURCES += \
    data_files.qrc
