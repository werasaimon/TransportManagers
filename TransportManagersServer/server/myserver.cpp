#include "myserver.h"

MyServer::MyServer(QObject *parent) :
    QTcpServer(parent)
{
}

void MyServer::StartServer(QHostAddress ip, quint16 port)
{
    if(listen(ip, port))
    {
        qDebug() << "Server: started";
    }
    else
    {
        qDebug() << "Server: not started!";
    }
}

void MyServer::incomingConnection(int handle)
{
    // at the incoming connection, make a client
    MyClient *client = new MyClient(this);
    client->m_Query = m_Query;
    client->SetSocket(handle);
}
