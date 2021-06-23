#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostAddress>
#include <QScrollBar>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QMessageBox>


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
    QByteArray datas = m_TCPSocket.readAll();
    qDebug() << datas;
    //_socket.write(QByteArray("ok !\n"));

    ui->textEdit->setHtml(ui->textEdit->toHtml() + QString::fromUtf8(datas) + "<br>");
    ui->textEdit->verticalScrollBar()->setSliderPosition(ui->textEdit->verticalScrollBar()->maximum());



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


}


void MainWindow::on_pushButton_clicked()
{
    m_TCPSocket.write(ui->lineEdit_command_sql_send->text().toUtf8());
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

