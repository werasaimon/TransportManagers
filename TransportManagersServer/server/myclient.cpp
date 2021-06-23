#include "myclient.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

MyClient::MyClient(QObject *parent) :
    QObject(parent)
{
    QThreadPool::globalInstance()->setMaxThreadCount(50);
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
    MyTask *mytask = new MyTask();
    mytask->setAutoDelete(true);

    mytask->_str = socket->readAll();
    qDebug() << socket->readAll();

    connect(mytask, SIGNAL(Result(QByteArray)), this, SLOT(TaskResult(QByteArray)), Qt::QueuedConnection);

    qDebug() << "Starting a new task using a thread from the QThreadPool";
    QThreadPool::globalInstance()->start(mytask);

}

// After a task performed a time consuming task.
// We grab the result here, and send it to client
void MyClient::TaskResult(QByteArray message)
{
   // QByteArray Buffer;
   // Buffer.append("\r\nTask result = ");
   // Buffer.append(Number);
   // Buffer.append(QString("\n"));
   // Buffer.append(message);
    //pl->insertPlainText(message);

    if(!m_Query->exec(message))
    //if(!m_Query->exec("SELECT * FROM test LIMIT 2"))
    {
        qDebug() << m_Query->lastError().databaseText();
        qDebug() << m_Query->lastError().driverText();
        return;
    }
    else
    {

        socket->write( SQLQuery(message,*m_Query).toUtf8() );

//        while (m_Query->next())
//        {
//            QString id   = "<br> ID: " + m_Query->value(0).toString() + " ";
//            QString text = "Text: " + m_Query->value(1).toString() + " ";
//            QString data = "Data: " +m_Query->value(2).toString() + " ";
//            QString send = id+text+data + " </br>";

//            qDebug() << id << " " << text << " " << data;

//            socket->write(send.toUtf8());
//        }
    }
    //socket->write(message);
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


















