#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QThreadPool>
#include <QString>

#include <QPlainTextEdit>
#include <QSqlQuery>

#include "mytask.h"

class MyClient : public QObject
{
    Q_OBJECT
public:
    explicit MyClient(QObject *parent = 0);
    void SetSocket(int Descriptor);

    QSqlQuery *m_Query;
    
signals:
    
public slots:

    void connected();
    void disconnected();
    void readyRead();

    // make the server fully ascynchronous
    // by doing time consuming task
    void TaskResult(QByteArray message);


    QString SQLQuery(const QString & sqlquery, QSqlQuery query);

private:
    QTcpSocket *socket;
    QString _str;
    
};

#endif // MYCLIENT_H
