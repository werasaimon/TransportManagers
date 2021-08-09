#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostAddress>
#include <QScrollBar>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QMessageBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QDateTime>


#include "../Common/blockreader.h"
#include "../Common/blockwriter.h"
#include "../Common/help_func.hpp"
#include "../Common/qoperators.hpp"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle( "Meneger Client" );

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

    //-------------------------------------------------------//

    ui->listWidget_A->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget_A, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));

    //-------------------------------------------------------//

    connect(ui->pushButton_ExtudeAnOreder, SIGNAL(clicked()), this, SLOT(SendItem()));

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
        QString key = "key";
        BlockReader br(m_TCPSocket);
        br.stream() >> key;
        qDebug()  << key;

        switch(const_hash(key.toStdString().c_str()))
        {
            case const_hash("sql"):  AnswerToKey_SQL(br.stream()); break;
            case const_hash("user"): AnswerToKey_User(br.stream()); break;
            case const_hash("order"): AnswerToKey_Order(br.stream()); break;
            case const_hash("list"): AnswerToKey_List(br.stream()); break;
            case const_hash("fulfillment"): AnswerToKey_fulfillment(br.stream()); break;
        }


        //ui->textEdit->setHtml(ui->textEdit->toHtml() + s.toUtf8() + "<br>");
        //ui->textEdit->setHtml(ui->textEdit->toHtml() + key.toUtf8() + "<br>");
        //ui->textEdit->setHtml(ui->textEdit->toHtml() + QString::fromUtf8(datas).toUtf8() + "<br>");
       // ui->textEdit->verticalScrollBar()->setSliderPosition(ui->textEdit->verticalScrollBar()->maximum());


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

    QByteArray  SQL_Edit = "SELECT * FROM preorders LIMIT 2";//ui->lineEdit_command_sql_send->text().toUtf8();
    QString key = "sql";
    BlockWriter* bw = new BlockWriter(m_TCPSocket);
    bw->stream() << key << SQL_Edit;
    delete bw;

   // m_TCPSocket.write(ui->lineEdit_command_sql_send->text().toUtf8());
}


void MainWindow::on_pushButton_2_clicked()
{
    QHostAddress IP = QHostAddress(ui->lineEdit_IP->text());
    quint16 Port = 1234;


    if( isConnected == false )
    {
        m_TCPSocket->connectToHost(IP, Port);

        if (m_TCPSocket->waitForConnected(1000))
        {
            qDebug("Connected!");

            if(m_TCPSocket->state() == QTcpSocket::ConnectedState)
            {
                QString key = "user";
                BlockWriter* bw = new BlockWriter(m_TCPSocket);
                bw->stream() << key;

                QString username = ui->lineEdit_username->text();
                QString password = ui->lineEdit_password->text();
                bw->stream() << username;
                bw->stream() << password;

                delete bw;
            }

        }
        else if(m_TCPSocket->state() != QTcpSocket::ConnectedState)
        {
            qDebug("No Connected!");
            m_TCPSocket->disconnectFromHost();
        }
    }
    else
    {
        m_TCPSocket->disconnectFromHost();

        //-----------------------------------------------------//

//        QAbstractItemModel* mdl = ui->tableView->model();
//        mdl->removeRows(0,mdl->rowCount());
//        mdl->removeColumns(0,mdl->columnCount());



//        ui->tableView->setModel(nullptr);

        //-----------------------------------------------------//

        isConnected = false;
        ui->pushButton_2->setText("Connected");
        ui->listWidget_A->clear();
    }

}


void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    QJsonObject object = episodes->getJsonObject( index );



//    QMessageBox Msgbox;
//    Msgbox.setText(object["Text"].toString());
//    Msgbox.exec();

     QMessageBox::StandardButton reply;
     reply = QMessageBox::question(this, "Я хочу Выполнить этот заказ!",
                                   object["Text"].toString(),
                                   QMessageBox::Yes|QMessageBox::No);
     if (reply == QMessageBox::Yes)
     {
       qDebug() << "Yes was clicked";
       //QApplication::quit();

        Order _order;
       _order.ID = object["ID"].toInt();
       _order.Text = object["Text"].toString();
       _order.Data = object["Data"].toString();
       _order.Username = ui->lineEdit_username->text();

        qDebug() << "ID: " << _order.ID;
        qDebug() << "Text: " << _order.Text;
        qDebug() << "Data: " << _order.Data;
        qDebug() << "Username: " << _order.Username;

        //=========================================//

        BlockWriter* bw = new BlockWriter(m_TCPSocket);

        QString key = "order";
        bw->stream() << key;

        bw->stream() << _order.ID;
        bw->stream() << _order.Text;
        bw->stream() << _order.Data;
        bw->stream() << _order.Username;

        delete bw;

     }
     else
     {
       qDebug() << "Yes was *not* clicked";
     }


}


void MainWindow::on_pushButton_test_clicked()
{

    QJsonTableModel::Header header;
    header.push_back( QJsonTableModel::Heading( { {"title","A"}, {"index","ID"} }) );
    header.push_back( QJsonTableModel::Heading( { {"title","B"}, {"index","Text"} }) );
    header.push_back( QJsonTableModel::Heading( { {"title","C"}, {"index","Data"} }) );

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


//void MainWindow::on_pushButton_A_clicked()
//{

//   // ui->listWidget_A->clear();

//    auto itemN = new QListWidgetItem();
//    auto widget = new QWidget();


//    QFrame *line;
//    line = new QFrame(widget);
//    line->setFrameShape(QFrame::HLine);
//    line->setFrameShadow(QFrame::Sunken);


//    auto widgetText =  new QLabel("I love PyQt \n "
//                                  "iiiiiiiiiiiiiiiiiiopoopopopopopiii \n"
//                                  "jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj \n"
//                                  "ghjgjgjhg hg wer wera!");


//    auto widgetButton =  new QPushButton("Push Me");

//    QString string = "Tuesday, 23 April 12 22:51:41";
//    QString format = "dddd, d MMMM yy hh:mm:ss";


//    QDateTime nowDateTime;
//    nowDateTime = QDateTime::currentDateTime();
//    long long secs = nowDateTime.toSecsSinceEpoch();
//    QDateTime thisDT = QDateTime::fromTime_t(secs);

//    auto widgetLayout = new QVBoxLayout();
//    widgetLayout->addWidget(new QLabel(thisDT.toString()));
//    widgetLayout->addStretch(1);
//    widgetLayout->addWidget(widgetText);
//    widgetLayout->addWidget(widgetButton);
//    widgetLayout->addWidget(line);
//    widgetLayout->addStretch(2);

//    widgetLayout->setSizeConstraint(QLayout::SetFixedSize);
//    widgetLayout->setAlignment(Qt::AlignTop);
//    widget->setLayout(widgetLayout);
//    itemN->setSizeHint(widget->sizeHint());

//    ui->listWidget_A->addItem(itemN);
//    ui->listWidget_A->setItemWidget(itemN, widget);




//    //    auto bt = new QPushButton("wera");
//    //    QListWidgetItem *item = new QListWidgetItem("wera");
//    //    ui->listWidget_A->addItem(item);
//    //    ui->listWidget_A->setItemWidget(item,bt);
//    //    ui->listWidget_A->addItem("suka");
//}

void MainWindow::AnswerToKey_SQL(QDataStream &_stream_tcp_ip)
{
     QByteArray datas;
    _stream_tcp_ip >> datas;

    /**/
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(datas, &parseError);
    episodes->setJson( doc );
    //ui->tableView->setModel(episodes);

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

void MainWindow::AnswerToKey_User(QDataStream &_stream_tcp_ip)
{
    QString status_connect;
    _stream_tcp_ip >> status_connect;

    if(status_connect == QString("no connect user"))
    {
        m_TCPSocket->disconnectFromHost();
        QMessageBox::information(this, tr("no connect user"), tr(status_connect.toUtf8()));
        isConnected = false;
    }
    else if(status_connect == QString("yes connect user"))
    {
        QMessageBox::information(this, tr("yes connect user"), tr(status_connect.toUtf8()));
        isConnected = true;
        ui->pushButton_2->setText("Disconnected");
    }
}

void MainWindow::AnswerToKey_Order(QDataStream &_stream_tcp_ip)
{

}

void MainWindow::AnswerToKey_List(QDataStream &_stream_tcp_ip)
{

    int size;
    _stream_tcp_ip >> size;

    mMapOrders.clear();
    ui->listWidget_A->clear();
    for(int i=0;i<size;++i)
    {
         Order _Order;
        _stream_tcp_ip >> _Order;

//         _stream_tcp_ip >> _Order.ID;
//         _stream_tcp_ip >> _Order.Text;
//         _stream_tcp_ip >> _Order.Data;
//         _stream_tcp_ip >> _Order.Username;
//         _stream_tcp_ip >> _Order.Weight;
//         _stream_tcp_ip >> _Order.Address;
//         _stream_tcp_ip >> _Order.Product;

        //-----------------------------------------//
        auto itemN = new QListWidgetItem;
        auto widget = new QWidget();

        QFrame *line = new QFrame(widget);
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        auto widgetText =  new QLabel("ID: " + QString::number(_Order.ID) + "\n" +
                                      "Data: " + _Order.Data + "\n" +
                                      "Weight: " + QString::number(_Order.Weight) + "\n" +
                                      "Address: " + _Order.Address + "\n" +
                                      "Product: " + _Order.Product + "\n" +
                                      "Text: " + _Order.Text + "\n");

        widgetText->setStyleSheet("background-color: rgb(200, 200, 200);");

        auto widgetLayout = new QVBoxLayout();

        widgetLayout->addStretch(1);
        widgetLayout->addWidget(widgetText);

        widgetLayout->addWidget(line);
        widgetLayout->addStretch(5);

        widgetLayout->setSizeConstraint(QLayout::SetFixedSize);
        widgetLayout->setAlignment(Qt::AlignTop);

        widget->setLayout(widgetLayout);
        itemN->setSizeHint(widget->sizeHint());

        ui->listWidget_A->addItem(itemN);
        ui->listWidget_A->setItemWidget(itemN, widget);
         //-------------------------------------//

        mMapOrders.insert(itemN,_Order);

//        mMapOrders[itemN].ID = _Order.ID;
//        mMapOrders[itemN].Text = _Order.Text;
//        mMapOrders[itemN].Data = _Order.Data;
//        mMapOrders[itemN].Weight = _Order.Weight;
//        mMapOrders[itemN].Address = _Order.Address;

        qDebug() << _Order.ID << _Order.Text << _Order.Data << _Order.Username << _Order.Address << _Order.Product;
    }
}

void MainWindow::AnswerToKey_fulfillment(QDataStream &_stream_tcp_ip)
{

    int size;
    _stream_tcp_ip >> size;

    mMapOrders.clear();
    ui->listWidget_A->clear();
    for(int i=0;i<size;++i)
    {
        Order _Order;
       _stream_tcp_ip >> _Order;

//        _stream_tcp_ip >> _Order.ID;
//        _stream_tcp_ip >> _Order.Text;
//        _stream_tcp_ip >> _Order.Data;
//        _stream_tcp_ip >> _Order.Username;
//        _stream_tcp_ip >> _Order.Weight;
//        _stream_tcp_ip >> _Order.Address;

        //-----------------------------------------//
        auto itemN = new QListWidgetItem;
        auto widget = new QWidget();

        QFrame *line = new QFrame(widget);
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        auto widgetText =  new QLabel("Status: " + _Order.Status + "\n" +
                                      "ID: " + QString::number(_Order.ID) + "\n" +
                                      "Data: " + _Order.Data + "\n" +
                                      "Weight: " + QString::number(_Order.Weight) + "\n" +
                                      "Address: " + _Order.Address + "\n" +
                                      "Executor: " + _Order.Username + "\n" +
                                      "Product: " + _Order.Product + "\n" +
                                      "Text: " + _Order.Text + "\n" );


         if(_Order.Status == QString("Final"))
         {
             widgetText->setStyleSheet("background-color: rgb(20, 250, 20);"
                                       "font-size: 24px;");
         }
         else
         {
             widgetText->setStyleSheet("background-color: rgb(250, 20, 20);"
                                       "font-size: 24px;");
         }



        auto widgetLayout = new QVBoxLayout();

        widgetLayout->addStretch(1);
        widgetLayout->addWidget(widgetText);

        widgetLayout->addWidget(line);
        widgetLayout->addStretch(5);

        widgetLayout->setSizeConstraint(QLayout::SetFixedSize);
        widgetLayout->setAlignment(Qt::AlignTop);

        widget->setLayout(widgetLayout);
        itemN->setSizeHint(widget->sizeHint());

        ui->listWidget_A->addItem(itemN);
        ui->listWidget_A->setItemWidget(itemN, widget);
         //-------------------------------------//

        mMapOrders.insert(itemN,_Order);

//        mMapOrders[itemN].ID = _Order.ID;
//        mMapOrders[itemN].Text = _Order.Text;
//        mMapOrders[itemN].Data = _Order.Data;
//        mMapOrders[itemN].Username = _Order.Username;
//        mMapOrders[itemN].Weight = _Order.Weight;
//        mMapOrders[itemN].Address = _Order.Address;

        qDebug() << _Order.ID << _Order.Text << _Order.Data << _Order.Username << _Order.Address << _Order.Product << _Order.Status;
    }
}


void MainWindow::on_pushButton_List_clicked()
{
    QByteArray  SQL_Edit = "SELECT * FROM preorders LIMIT 2";//ui->lineEdit_command_sql_send->text().toUtf8();
    QString key = "list";
    QString sql = "SELECT * FROM preorders;";
    BlockWriter* bw = new BlockWriter(m_TCPSocket);
    bw->stream() << key << sql;
    delete bw;

    ui->pushButton_ExtudeAnOreder->setEnabled(true);
}




void MainWindow::showContextMenu(const QPoint &pos)
{
        // Handle global position
        //QPoint globalPos = ui->listWidget_A->mapToGlobal(pos);

        // Create menu and insert some actions
        //QMenu myMenu;
        //myMenu.addAction("Я Хочу Выполнить", this, SLOT(SendItem()));
        //myMenu.addAction("Erase",  this, SLOT(eraseItem()));

        // Show context menu at handling position
        //myMenu.exec(globalPos);
}

void MainWindow::SendItem()
{

    // If multiple selection is on, we need to erase all selected items
    for (int i = 0; i < ui->listWidget_A->selectedItems().size(); ++i)
    {
        // Get curent item on selected row
        QListWidgetItem *item = ui->listWidget_A->takeItem(ui->listWidget_A->currentRow());
        Order _order = mMapOrders[item];
        _order.Username = ui->lineEdit_username->text();
        _order.Data = _order.Data;


        qDebug() << "DDDDAATTEE: " << _order.Data;

        //---------------------------------------------//

        BlockWriter* bw = new BlockWriter(m_TCPSocket);

        QString key = "order";
        bw->stream() << key;

        bw->stream() << _order.ID;
        bw->stream() << _order.Text;
        bw->stream() << _order.Data;
        bw->stream() << _order.Username;

        delete bw;

        // And remove it
        //delete item;
    }
}


void MainWindow::on_pushButton_fulfillment_clicked()
{
    if(m_TCPSocket->state() == QTcpSocket::ConnectedState)
    {
        QString key = "fulfillment";
        BlockWriter* bw = new BlockWriter(m_TCPSocket);
        bw->stream() << key;

        QString username = ui->lineEdit_username->text();
        bw->stream() << username;
        //bw->stream() << password;

        ui->pushButton_ExtudeAnOreder->setEnabled(false);

        delete bw;
    }
}

