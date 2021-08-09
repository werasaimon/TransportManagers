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
#include "../Common/qoperators.hpp"



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

    // fulfillment order list

    switch(const_hash(key.toStdString().c_str()))
    {
        case const_hash("sql"):  AnswerToKey_SQL(br.stream()); break;
        case const_hash("user"): AnswerToKey_User(br.stream()); break;
        case const_hash("order"): AnswerToKey_Order(br.stream()); break;
        case const_hash("list"): AnswerToKey_List(br.stream()); break;
        case const_hash("fulfillment"): AnswerToKey_fulfillment(br.stream()); break;
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



//    m_Query->prepare("SELECT ID_Oreder FROM fulfill WHERE Username = :username AND ID_Oreder = :id AND Data = :data");
//    m_Query->bindValue(":id", _order.ID);
//    m_Query->bindValue(":username", _order.Username);
//    m_Query->bindValue(":data", _order.Data);
//    m_Query->exec();

////    if(!m_Query->isActive())
////    {
////        qDebug() << ("SQL Statement execution failed");
////        return;
////    }

//    while (m_Query->next())
//    {
//        qDebug() << ("SQL While");
//        return;
//    }


    qDebug() << "ID: " << _order.ID;
    qDebug() << "Text: " << _order.Text;
    qDebug() << "Data: " << _order.Data;
    qDebug() << "Username: " << _order.Username;


    m_Query->prepare("INSERT INTO fulfill (ID_Oreder, Username, Text, Data ) "
                     "VALUES (:ID_Oreder, :Username, :Text, :Data )");




    m_Query->bindValue(":ID_Oreder", _order.ID);
    m_Query->bindValue(":Username", _order.Username);
    m_Query->bindValue(":Text", _order.Text);
    m_Query->bindValue(":Data", _order.Data);



//    if(m_Query->isActive())
//    {
//        qDebug() << ("SQL Statement execution failed");
//        return;
//    }

//    while (m_Query->next())
//    {
//        return;
//    }


    int i_cid = m_Query->record().indexOf("ID_Oreder");
    int counter = 0;
    while (m_Query->next())
    {
        counter++;
        qDebug() << m_Query->value(i_cid).toInt();
    }

    if(counter == 0)
    m_Query->exec();
}

void MyClient::AnswerToKey_List(QDataStream &_stream_tcp_ip)
{
     QString sql;
    _stream_tcp_ip >> sql;

    if(!m_Query->exec("SELECT * FROM preorders WHERE Username IS NULL OR Username = '' "))
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

         QVector<Order> List;

         int i_cid   = m_Query->record().indexOf("ID");
         int i_ctext = m_Query->record().indexOf("Text");
         int i_cdate = m_Query->record().indexOf("Date");
         int i_cweight = m_Query->record().indexOf("Weight");
         int i_caddress = m_Query->record().indexOf("Address");
         int i_cproduct = m_Query->record().indexOf("Product");
         int i_cstatus = m_Query->record().indexOf("Status");

         while (m_Query->next())
         {
             int ID = m_Query->value(i_cid).toInt();
             QString Text = m_Query->value(i_ctext).toString();
             QString Data = m_Query->value(i_cdate).toString();
             int Weight = m_Query->value(i_cweight).toInt();
             QString Address = m_Query->value(i_caddress).toString();
             QString Product = m_Query->value(i_cproduct).toString();
             QString Status = m_Query->value(i_cstatus).toString();


             Order order;
             order.ID = ID;
             order.Text = Text;
             order.Data = Data;
             order.Weight = Weight;
             order.Address = Address;
             order.Product = Product;
             order.Status = Status;

             List.push_back(order);

//            qDebug() << "ID: " << ID <<
//                        "Text: " << Text <<
//                        " Data: " << order.Data <<
//                        " Weight: " << order.Weight <<
//                        " Product: " << order.Product <<
//                        " Address: " << order.Address;
         }



         QString key = "list";
         BlockWriter* bw = new BlockWriter(socket);
         bw->stream() << key;// << List;
         bw->stream() << List.size();

         for(int i=0;i<List.size();++i)
         {
            bw->stream() << List[i];

//            bw->stream() << List[i].ID;
//            bw->stream() << List[i].Text;
//            bw->stream() << List[i].Data;
//            bw->stream() << List[i].Username;
//            bw->stream() << List[i].Weight;
//            bw->stream() << List[i].Address;
//            bw->stream() << List[i].Product;

            qDebug() << "ID: " << List[i].ID <<
                        "Text: " << List[i].Text <<
                        " Data: " << List[i].Data <<
                        " Username: " << List[i].Username <<
                        " Weight: " << List[i].Weight <<
                        " Product: " << List[i].Product <<
                        " Address: " << List[i].Address;
         }

         delete bw;



    }


}

void MyClient::AnswerToKey_fulfillment(QDataStream &_stream_tcp_ip)
{
    QString username;
    _stream_tcp_ip >> username;

    qDebug() << username;

    m_Query->prepare("SELECT * FROM preorders WHERE Username = :username ");
    m_Query->bindValue(":username", username);

    if(!m_Query->exec())
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

         QVector<Order> List;

         int i_cid   = m_Query->record().indexOf("ID");
         int i_ctext = m_Query->record().indexOf("Text");
         int i_cdate = m_Query->record().indexOf("Date");
         int i_cusername = m_Query->record().indexOf("Username");
         int i_cweight = m_Query->record().indexOf("Weight");
         int i_caddress = m_Query->record().indexOf("Address");
         int i_cproduct = m_Query->record().indexOf("Product");
         int i_cstatus = m_Query->record().indexOf("Status");

         while (m_Query->next())
         {
             int ID = m_Query->value(i_cid).toInt();
             QString Text = m_Query->value(i_ctext).toString();
             QString Data = m_Query->value(i_cdate).toString();
             QString UserName = m_Query->value(i_cusername).toString();
             int Weight = m_Query->value(i_cweight).toInt();
             QString Address = m_Query->value(i_caddress).toString();
             QString Product = m_Query->value(i_cproduct).toString();
             QString Status = m_Query->value(i_cstatus).toString();


             Order order;
             order.ID = ID;
             order.Text = Text;
             order.Data = Data;
             order.Username = UserName;
             order.Weight = Weight;
             order.Address = Address;
             order.Product = Product;
             order.Status = Status;

             List.push_back(order);


             qDebug() << "ID: " << ID <<
                         "Text: " << Text <<
                         " Data: " << order.Data <<
                         " Username: " << order.Username <<
                         " Weight: " << order.Weight <<
                         " Product: " << order.Product <<
                         " Address: " << order.Address;

           // qDebug() << "<<<<<<<<<<<<<" << ID <<">>>>>>>>>>>" << Text << " Data: " << order.Data << " Username: " << order.Username;
         }


         BlockWriter* bw = new BlockWriter(socket);

         QString key = "fulfillment";
         bw->stream() << key;// << List;
         bw->stream() << List.size();

         for(int i=0;i<List.size();++i)
         {
            bw->stream() << List[i];

//            bw->stream() << List[i].ID;
//            bw->stream() << List[i].Text;
//            bw->stream() << List[i].Data;
//            bw->stream() << List[i].Username;
//            bw->stream() << List[i].Weight;
//            bw->stream() << List[i].Address;

             //qDebug() <<  " Data: " << List[i].Data;

            qDebug() << "ID: " << List[i].ID <<
                        "Text: " << List[i].Text <<
                        " Data: " << List[i].Data <<
                        " Username: " << List[i].Username <<
                        " Weight: " << List[i].Weight <<
                        " Product: " << List[i].Product <<
                        " Address: " << List[i].Address;
         }

         delete bw;
    }

}


















