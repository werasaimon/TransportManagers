
#include "itask.h"
#include <QDebug>


MyTask::MyTask(QTcpSocket *_socket) :
    m_TCPSocket(_socket)
{
    qDebug() << "MyTask()";
}

// When the thread pool kicks up, and implements Qrunnable
// it's going to hit this run, and it's going to do this time consuming task.
// After it's done, we're going to send the results back to our main thread.
// This runs in the separate thread, and we do not have any control over this thread,
// but Qt does.
// This may just stay in the queue for several ms depending on how busy the server is.
void MyTask::run()
{
    // time consumer
    qDebug() << "Task start";
    emit Result(m_TCPSocket);
}
