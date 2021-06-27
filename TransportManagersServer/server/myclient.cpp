#include "myclient.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "../Common/blockreader.h"
#include "../Common/blockwriter.h"

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

    QByteArray SQL_Edit;
    quint16 key;
    BlockReader br(socket);
    br.stream() >> SQL_Edit >> key;

    if(!m_Query->exec(SQL_Edit))
    //if(!m_Query->exec("SELECT * FROM test LIMIT 2"))
    {
        qDebug() << m_Query->lastError().databaseText();
        qDebug() << m_Query->lastError().driverText();
        return;
    }
    else
    {


        BlockWriter* bw = new BlockWriter(socket);
        bw->stream() << QString("Hello World!") << QString("bababa") << SQLQuery(SQL_Edit,*m_Query).toUtf8();
        delete bw;



        /**
               QByteArray block;
               QDataStream out(&block, QIODevice::WriteOnly);      //we will serialize the data in the array block

               out.setVersion(QDataStream::Qt_4_0);

               out << (quint16)0;                                  //we reserve space for a 16 bit integer that will contain the total size of the data block we are sending
               out << QString( "something" );                      //Sending the name of the file
               out.device()->seek(0);                              //Going back to the beginning of the array
               out << (quint16)(block.size() - sizeof(quint16));   //overwrite the reserved 16 bit integer value with the total size of the array

               socket->write(block);                               //Send message


        /**
          QByteArray block;
          QDataStream out( &block, QIODevice::WriteOnly );
          out.setDevice( socket );
          out.setVersion( QDataStream::Qt_4_8 );
          out << "HELLO CLIENT!\x0d\x0a";


         QBuffer buffer;
         buffer.open(QIODevice::WriteOnly);

         QDataStream _stream;
         _stream.setVersion(QDataStream::Qt_4_8);
         _stream.setDevice(&buffer);


        /**
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << "Some Text";
        socket->write(block);


        /**
        BlockWriter bw(socket);
        bw.stream() << QString("wera") << QString("Hello World!");
        bw.~BlockWriter();
        **/

//        socket->write( SQLQuery(message,*m_Query).toUtf8() );

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


















