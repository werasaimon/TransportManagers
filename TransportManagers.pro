
TEMPLATE = subdirs
SUBDIRS  = \
    TransportManagersServer/TransportManagersServer.pro \
    TransportClient/TransportClient.pro
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
