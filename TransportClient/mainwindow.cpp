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

}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onReadyRead()
{
   // QByteArray datas = m_TCPSocket.readAll();
   // qDebug() << datas;
    //_socket.write(QByteArray("ok !\n"));

    //QString s1,s2;
    //BlockReader br(&m_TCPSocket);
    //br.stream() >> s1 >> s2;
    //qDebug() << s1 << s2;

    QByteArray datas;
    QString s,s2;
    BlockReader br(&m_TCPSocket);
    br.stream() >> s >> s2 >> datas;
    qDebug()  << s << s2 << datas;
    //delete br;


    //ui->textEdit->setHtml(ui->textEdit->toHtml() + s.toUtf8() + "<br>");
    //ui->textEdit->setHtml(ui->textEdit->toHtml() + s2.toUtf8() + "<br>");
    ui->textEdit->setHtml(ui->textEdit->toHtml() + QString::fromUtf8(datas).toUtf8() + "<br>");
    ui->textEdit->verticalScrollBar()->setSliderPosition(ui->textEdit->verticalScrollBar()->maximum());


    /**
    //First get the filesize we're going to download
    QDataStream in(&m_TCPSocket);
    in.setVersion(QDataStream::Qt_4_0);


     ui->textEdit->setHtml(ui->textEdit->toHtml() + hello_there.toUtf8() + "<br>");
     ui->textEdit->verticalScrollBar()->setSliderPosition(ui->textEdit->verticalScrollBar()->maximum());


    /**
      QByteArray block;
      QDataStream in(&m_TCPSocket);
      in.setVersion( QDataStream::Qt_4_8 );
      in >> block;





    ui->textEdit->setHtml(ui->textEdit->toHtml() + QString::fromUtf8(block) + "<br>");
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


void MainWindow::on_pushButton_clicked()
{
    QByteArray  SQL_Edit = ui->lineEdit_command_sql_send->text().toUtf8();
    quint16 key = 100;
    BlockWriter* bw = new BlockWriter(&m_TCPSocket);
    bw->stream() << SQL_Edit << key;
    delete bw;

   // m_TCPSocket.write(ui->lineEdit_command_sql_send->text().toUtf8());
}


void MainWindow::on_pushButton_2_clicked()
{
    QHostAddress IP = QHostAddress(ui->lineEdit_IP->text());
    quint16 Port = 1234;
    m_TCPSocket.connectToHost(IP, Port);

    if (m_TCPSocket.waitForConnected(1000))
    {
        qDebug("Connected!");

       //m_TCPSocket.connectToHost(QHostAddress("192.168.1.3"), 1234);
        connect(&m_TCPSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    }
    else
    {
       qDebug("No Connected!");
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
