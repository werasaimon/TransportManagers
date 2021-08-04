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
    void TaskResult(QTcpSocket *socket);


    QString SQLQuery(const QString & sqlquery, QSqlQuery query);

private:
    QTcpSocket *socket;
    QString _str;


    void AnswerToKey_SQL(QDataStream &_stream_tcp_ip);
    void AnswerToKey_User(QDataStream &_stream_tcp_ip);
    void AnswerToKey_Order(QDataStream &_stream_tcp_ip);
    void AnswerToKey_List(QDataStream &_stream_tcp_ip);
    void AnswerToKey_fulfillment(QDataStream &_stream_tcp_ip);
    
};

#endif // MYCLIENT_H
