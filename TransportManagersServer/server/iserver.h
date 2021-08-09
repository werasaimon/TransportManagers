#ifndef ISERVER_H
#define ISERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QAbstractSocket>
#include "iclient.h"

#include <QPlainTextEdit>
#include <QSqlQuery>

class IServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit IServer(QObject *parent = 0);
    void StartServer(QHostAddress ip = QHostAddress::Any , quint16 port = 1234);

    QSqlQuery *m_Query;

protected:
    void incomingConnection(int handle);
    
signals:
    
public slots:
    
};

#endif // ISERVER_H
