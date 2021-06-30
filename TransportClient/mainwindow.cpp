#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostAddress>
#include <QScrollBar>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QMessageBox>

#include "../Common/blockreader.h"
#include "../Common/blockwriter.h"


unsigned constexpr const_hash(char const *input)
{
    return *input ? static_cast<unsigned int>(*input) + 33 * const_hash(input + 1) : 5381;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QJsonTableModel::Header header;
    header.push_back( QJsonTableModel::Heading( { {"title","ID"},   {"index","ID"} }) );
    header.push_back( QJsonTableModel::Heading( { {"title","Text"}, {"index","Text"} }) );
    header.push_back( QJsonTableModel::Heading( { {"title","Date"}, {"index","Data"} }) );

    episodes = new QJsonTableModel( header, this );

    //------------------------------------------------------//

    m_TCPSocket = new QTcpSocket(this);

    connect(m_TCPSocket, SIGNAL(hostFound()), this, SLOT(weAreConnected()));
    connect(m_TCPSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    connect(m_TCPSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(m_TCPSocket, SIGNAL(disconnected()), m_TCPSocket, SLOT(sockDisc()));

}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onReadyRead()
{
    if(!m_TCPSocket || (m_TCPSocket->state() != QTcpSocket::ConnectedState)) return;

    if(m_TCPSocket->waitForConnected(500))
    {

        QByteArray datas;
        QString key = "key";
        BlockReader br(m_TCPSocket);
        br.stream() >> key >> datas;
        qDebug()  << key << datas;
        //delete br;

        switch(const_hash(key.toStdString().c_str()))
        {
            case const_hash("key"): qDebug() << "one();"; break;
            case const_hash("two"): qDebug() << "two();"; break;
        }


        //ui->textEdit->setHtml(ui->textEdit->toHtml() + s.toUtf8() + "<br>");
        ui->textEdit->setHtml(ui->textEdit->toHtml() + key.toUtf8() + "<br>");
        ui->textEdit->setHtml(ui->textEdit->toHtml() + QString::fromUtf8(datas).toUtf8() + "<br>");
        ui->textEdit->verticalScrollBar()->setSliderPosition(ui->textEdit->verticalScrollBar()->maximum());

        /**/

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(datas, &parseError);
        episodes->setJson( doc );
        ui->tableView->setModel(episodes);

        if(parseError.error ==  QJsonParseError::NoError)
        {
          // Has a Error
          return;
        }

        QJsonObject obj = doc.object();

        // acessing all object Keys
        for(const QString& key : obj.keys())
        {
           qDebug() << obj[key];
        }
        /**/
    }
    else
    {
        qDebug() << "<-- disconnect -->";
        delete m_TCPSocket;
    }

}


void MainWindow::on_pushButton_clicked()
{
    if(!m_TCPSocket || (m_TCPSocket->state() != QTcpSocket::ConnectedState)) return;

    QByteArray  SQL_Edit = ui->lineEdit_command_sql_send->text().toUtf8();
    QString key = "key";
    BlockWriter* bw = new BlockWriter(m_TCPSocket);
    bw->stream() << key << SQL_Edit;
    delete bw;

   // m_TCPSocket.write(ui->lineEdit_command_sql_send->text().toUtf8());
}


void MainWindow::on_pushButton_2_clicked()
{
    QHostAddress IP = QHostAddress(ui->lineEdit_IP->text());
    quint16 Port = 1234;

    m_TCPSocket->connectToHost(IP, Port);

    if (m_TCPSocket->waitForConnected(1000))
    {
        qDebug("Connected!");
    }
    else if(m_TCPSocket->state() != QTcpSocket::ConnectedState)
    {
        qDebug("No Connected!");
        m_TCPSocket->disconnectFromHost();
    }
}


void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    QJsonObject object = episodes->getJsonObject( index );
    qDebug() << object["Text"].toString();


    QMessageBox Msgbox;
    Msgbox.setText(object["Text"].toString());
    Msgbox.exec();


}


void MainWindow::on_pushButton_test_clicked()
{

    QJsonTableModel::Header header;
    header.push_back( QJsonTableModel::Heading( { {"title","A"},   {"index","ID"} }) );
    header.push_back( QJsonTableModel::Heading( { {"title","B"}, {"index","Text"} }) );
   // header.push_back( QJsonTableModel::Heading( { {"title","C"}, {"index","Data"} }) );

    if(episodes) delete episodes;

    episodes = new QJsonTableModel( header, this );

}



void MainWindow::weAreConnected()
{
    qDebug() << ("<-- Connected -->");
}

void MainWindow::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        case QAbstractSocket::HostNotFoundError:
            QMessageBox::information(this, tr("IPShare Client"),
                                     tr("The host was not found. Please check the "
                                        "host name and make sure it is sharing."));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            QMessageBox::information(this, tr("IPShare Client"),
                                     tr("The connection was refused by the peer. "
                                        "Make sure the other side is sharing, "
                                        "and check that the host name and port "
                                        "settings are correct."));
            break;
        default:
            QMessageBox::information(this, tr("IPShare Client"),
                                     tr("The following error occurred: %1.")
                                     .arg(m_TCPSocket->errorString()));
     }

   // delete m_TCPSocket;
   // m_TCPSocket = nullptr;
}

void MainWindow::sockDisc()
{
    m_TCPSocket->disconnectFromHost();
    m_TCPSocket->deleteLater();

   // delete m_TCPSocket;
   // m_TCPSocket = nullptr;
}

