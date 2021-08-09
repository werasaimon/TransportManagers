#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QAbstractSocket>
#include "myclient.h"

#include <QPlainTextEdit>
#include <QSqlQuery>

class MyServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyServer(QObject *parent = 0);
    void StartServer(QHostAddress ip = QHostAddress::Any , quint16 port = 1234);

    QSqlQuery *m_Query;

protected:
    void incomingConnection(int handle);
    
signals:
    
public slots:
    
};

#endif // MYSERVER_H
