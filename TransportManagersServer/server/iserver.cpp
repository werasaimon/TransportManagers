#include "iserver.h"

IServer::IServer(QObject *parent) :
    QTcpServer(parent)
{
}

void IServer::StartServer(QHostAddress ip, quint16 port)
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

void IServer::incomingConnection(int handle)
{
    // at the incoming connection, make a client
    IClient *client = new IClient(this);
    client->m_Query = m_Query;
    client->SetSocket(handle);
}
