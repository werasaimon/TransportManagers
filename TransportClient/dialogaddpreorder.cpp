#include "dialogaddpreorder.h"
#include "ui_dialogaddpreorder.h"
#include <QtSql/QSqlQuery>
#include "../Common/blockreader.h"
#include "../Common/blockwriter.h"
#include "../Common/help_func.hpp"
#include "mainwindow.h"

Dialogaddpreorder::Dialogaddpreorder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogaddpreorder)
{
    ui->setupUi(this);
}

Dialogaddpreorder::~Dialogaddpreorder()
{
    delete ui;
}

QTcpSocket *Dialogaddpreorder::TCPSocket() const
{
    return m_TCPSocket;
}

void Dialogaddpreorder::setTCPSocket(QTcpSocket *newTCPSocket)
{
    m_TCPSocket = newTCPSocket;
}

void Dialogaddpreorder::on_pushButton_AddPreorder_clicked()
{
    if(!m_TCPSocket || (m_TCPSocket->state() != QTcpSocket::ConnectedState)) return;

//    QByteArray  SQL_Edit = "SELECT * FROM preorders LIMIT 2";
//    QString key = "sql";
//    BlockWriter* bw = new BlockWriter(m_TCPSocket);
//    bw->stream() << key << SQL_Edit;
//    delete bw;

    QString weight = QString::number(ui->spinBox_Weight->value());
    QString text = ui->textEdit->toPlainText();
    QString date = ui->dateEdit->dateTime().toString("yyyy-MM-dd");
    QString address = ui->lineEdit_Address->text();
    QString product = ui->comboBox_NameProducts->currentText();
    QString status = "Waiting";

    QString SQL_Query = "INSERT INTO preorders (Text,Date,Weight,Address,Product,Status,Manager) "
                         "VALUES (  '" + text + "'," +
                                   "'" + date + "'," +
                                   "'" + weight + "'," +
                                   "'" + address + "'," +
                                   "'" + product + "'," +
                                   "'" + status + "'," +
                                   "'" + m_Username + "'" +
                                   ")";

    QString key = "query_sql";
    BlockWriter* bw = new BlockWriter(m_TCPSocket);
    bw->stream() << key <<  SQL_Query;
    delete bw;
    this->close();
}

void Dialogaddpreorder::AnswerToKey_Products(QDataStream &_stream_tcp_ip)
{
     int size;
     _stream_tcp_ip >> size;

     ui->comboBox_NameProducts->clear();
     for (int i = 0; i < size; ++i)
     {
         QString Products;
         _stream_tcp_ip >> Products;
         ui->comboBox_NameProducts->addItem(Products);
     }
}

void Dialogaddpreorder::onReadyRead(QString key, BlockReader *br)
{
    switch(const_hash(key.toStdString().c_str()))
    {
        case const_hash("products"): AnswerToKey_Products(br->stream()); break;
    }
}

void Dialogaddpreorder::setUsername(const QString &newUsername)
{
    m_Username = newUsername;
}



void Dialogaddpreorder::on_pushButton_UpdateProducts_clicked()
{
    QString key = "products";
    BlockWriter* bw = new BlockWriter(m_TCPSocket);
    bw->stream() << key;
    delete bw;
}

