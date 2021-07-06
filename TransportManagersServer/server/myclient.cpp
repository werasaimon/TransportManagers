#include "myclient.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "../Common/blockreader.h"
#include "../Common/blockwriter.h"
#include "../Common/help_func.hpp"
#include "../Common/order.h"


MyClient::MyClient(QObject *parent) :
    QObject(parent)
{
    QThreadPool::globalInstance()->setMaxThreadCount(4);
}

void MyClient::SetSocket(int Descriptor)
{
    // make a new socket
    socket = new QTcpSocket(this);

    qDebug() << "A new socket created!";

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    socket->setSocketDescriptor(Descriptor);

    qDebug() << " Client connected at " << Descriptor;
}


// asynchronous - runs separately from the thread we created
void MyClient::connected()
{
    qDebug() << "Client connected event";
}

// asynchronous
void MyClient::disconnected()
{
    qDebug() << "Client disconnected";
}

// Our main thread of execution
// This happening via main thread
// Our code running in our thread not in Qthread
// That's why we're getting the thread from the pool
void MyClient::readyRead()
{
    qDebug() << "MyClient::readyRead()";
    //qDebug() << socket->readAll();
    //_str = socket->readAll();
   // socket->write(socket->readAll());

    // Time consumer
    MyTask *mytask = new MyTask(socket);
    mytask->setAutoDelete(true);

//    mytask->_socket = socket;

//    mytask->_str = socket->readAll();
//    qDebug() << socket->readAll();

    connect(mytask, SIGNAL(Result(QTcpSocket*)), this, SLOT(TaskResult(QTcpSocket*)), Qt::QueuedConnection);

    qDebug() << "Starting a new task using a thread from the QThreadPool";
    QThreadPool::globalInstance()->start(mytask);

}

// After a task performed a time consuming task.
// We grab the result here, and send it to client
void MyClient::TaskResult(QTcpSocket* socket)
{
   // QByteArray Buffer;
   // Buffer.append("\r\nTask result = ");
   // Buffer.append(Number);
   // Buffer.append(QString("\n"));
   // Buffer.append(message);
    //pl->insertPlainText(message);


    QString key;
    BlockReader br(socket);
    br.stream() >> key;

    switch(const_hash(key.toStdString().c_str()))
    {
        case const_hash("sql"):  AnswerToKey_SQL(br.stream()); break;
        case const_hash("user"): AnswerToKey_User(br.stream()); break;
        case const_hash("order"): AnswerToKey_Order(br.stream()); break;
        case const_hash("list"): AnswerToKey_List(br.stream()); break;
    }
}

QString MyClient::SQLQuery(const QString &sqlquery , QSqlQuery query)
{
    QJsonDocument  json;
    QJsonArray     recordsArray;

    while(query.next())
    {
        QJsonObject recordObject;
        for(int x=0; x < query.record().count(); x++)
        {
            recordObject.insert( query.record().fieldName(x),QJsonValue::fromVariant(query.value(x)) );
        }
        recordsArray.push_back(recordObject);
    }
    json.setArray(recordsArray);

    return json.toJson();
}

void MyClient::AnswerToKey_SQL(QDataStream &_stream_tcp_ip)
{
      QByteArray SQL_Edit;
     _stream_tcp_ip >> SQL_Edit;

     if(!m_Query->exec(SQL_Edit))
     {
         qDebug() << m_Query->lastError().databaseText();
         qDebug() << m_Query->lastError().driverText();

         QString key = "error";
         BlockWriter* bw = new BlockWriter(socket);
         bw->stream() << key;
         bw->stream() << m_Query->lastError().databaseText();
         bw->stream() << m_Query->lastError().driverText();
         delete bw;

         return;

     }
     else
     {

         QString key = "sql";
         BlockWriter* bw = new BlockWriter(socket);
         bw->stream() << key << SQLQuery(SQL_Edit,*m_Query).toUtf8();
         delete bw;
     }
}

void MyClient::AnswerToKey_User(QDataStream &_stream_tcp_ip)
{

    QString username;
    QString password;

    _stream_tcp_ip >> username;
    _stream_tcp_ip >> password;

    qDebug() << username;
    qDebug() << password;

    m_Query->prepare("SELECT ID FROM users WHERE Username = :username AND Password = :password");
    m_Query->bindValue(":username", username);
    m_Query->bindValue(":password", password);
    m_Query->exec();
    if(!m_Query->isActive())
    {
        qDebug() << ("SQL Statement execution failed");
        return;
    }


     int i_cid = m_Query->record().indexOf("ID");
    int counter = 0;
    while (m_Query->next())
    {
        counter++;
        qDebug() << m_Query->value(i_cid).toInt();
    }

    //==========================================//

    BlockWriter* bw = new BlockWriter(socket);

    QString key = "user";
    bw->stream() << key;

    if(counter==1)
    {
        qDebug() << ("Sign in successful");
        bw->stream() << QString("yes connect user");
    }
    if(counter<1)
    {
        qDebug() << ("Sign in unsuccessful");
        bw->stream() << QString("no connect user");
    }

    delete bw;
}

void MyClient::AnswerToKey_Order(QDataStream &_stream_tcp_ip)
{
    Order _order;
    _stream_tcp_ip >> _order.ID;
    _stream_tcp_ip >> _order.Text;
    _stream_tcp_ip >> _order.Data;
    _stream_tcp_ip >> _order.Username;



    qDebug() << "ID: " << _order.ID;
    qDebug() << "Text: " << _order.Text;
    qDebug() << "Data: " << _order.Data;
    qDebug() << "Username: " << _order.Username;


    m_Query->prepare("INSERT INTO fulfill (ID , Username, Text, Data, ID_Oreder) "
                     "VALUES (:ID, :Username, :Text, :Data, :ID_Oreder)");

    m_Query->bindValue(":Username", _order.Username);
    m_Query->bindValue(":Text", _order.Text);
    m_Query->bindValue(":Data", _order.Data);
    m_Query->bindValue(":ID_Oreder", _order.ID);

    m_Query->exec();
}

void MyClient::AnswerToKey_List(QDataStream &_stream_tcp_ip)
{

}


















