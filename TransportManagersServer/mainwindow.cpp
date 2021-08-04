#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle( "Meneger Server" );
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("db_wera2");
    db.setUserName("wera2");
    db.setPassword("123");


//    db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setDatabaseName("./database.db");
////    db.setUserName("werasaimon");
////    db.setPassword("123qwe");

    if(!db.open())
    {
        qDebug() << db.lastError().text();
    }
    else
    {
                //        qDebug() << "Succses!";
                //        QString quary = "CREATE TABLE preorders ("
                //                        "ID INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,"
                //                        "Text VARCHAR(255),"
                //                        "Data DATATIME,"
                //                        "Username VARCHAR(255),"
                //                        "Weight  INTEGER,"
                //                        "Address VARCHAR(255),"
                //                        "Product VARCHAR(255),"
                //                        "Status  VARCHAR(255));";

                //                        //"Status  ENUM ('Waiting', 'Executo', 'Final'));";

                //        QString quary2 = "CREATE TABLE users ("
                //                         "Username VARCHAR(255),"
                //                         "Password VARCHAR(255),"
                //                         "ID INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE);";


                //        QString quary3 = "CREATE TABLE fulfill ("
                //                         //"ID INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,"
                //                         "ID_Oreder INTEGER NOT NULL PRIMARY KEY,"
                //                         "Username VARCHAR(255) UNIQUE,"
                //                         "Text VARCHAR(255),"
                //                         "Data DATATIME);";

                //        QString quary4 = "CREATE TABLE products ("
                //                         "ID INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,"
                //                         "Name VARCHAR(255));";

                //        QSqlQuery qry(db);

                //        if(!qry.exec(quary))
                //        {
                //            qDebug() << "error creating table preorders";
                //        }

                //        if(!qry.exec(quary2))
                //        {
                //            qDebug() << "error creating table users";
                //        }

                //        if(!qry.exec(quary3))
                //        {
                //            qDebug() << "error creating table fulfill";
                //        }

                //        if(!qry.exec(quary4))
                //        {
                //            qDebug() << "error creating table products";
                //        }

        //qDebug() << "yes create table";
    }


    m_Query = new QSqlQuery(db);

    t_model = new QSqlTableModel2(this,db);
    t_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    t_model->setTable("preorders");
    t_model->select();

    ui->tableView->setModel(t_model);
    ui->tableView->setSortingEnabled(true);

    //----------------------------------------//

    EditForm = new DialogRecortEditForm(this);
    EditForm->setParent(this,Qt::Window);
    EditForm->setModel(t_model);
    EditForm->setQuery(m_Query);

    //----------------------------------------//

    m_AddClientWidget = new DialogAddClient(this);
    m_AddPreorderWidget = new Dialogaddpreorder(this);
    m_AddProductWidget = new Dialogaddproduct(this);
    //----------------------------------------//


}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_Query;
}


void MainWindow::on_pushButton_clicked()
{
    if(!m_Query->exec(ui->plainTextEditScript->toPlainText().toStdString().c_str()))
    {
        qDebug() << m_Query->lastError().databaseText();
        qDebug() << m_Query->lastError().driverText();
        return;
    }
    else
    {
        qDebug() << "query.record()";
        while (m_Query->next())
        {
            qDebug() << m_Query->record();
        }
    }



//       query.prepare("INSERT INTO test (ID, Text, Date) "
//              "VALUES (?, ?, ?)");
//       query.addBindValue(1001);
//       query.addBindValue("Thad Beaumont");
//       query.addBindValue("16.06.2021");



//       t_model->setTable("test");
//       t_model->select();


//       ui->tableView->setModel(t_model);
//       ui->tableView->setSortingEnabled(true);



//        t_model->setQuery(query);

//        ui->tableView->setModel(t_model);



       // QSqlQueryModel *model = new QSqlQueryModel;
        t_model->setQuery(QSqlQuery(ui->plainTextEditScript->toPlainText().toStdString().c_str()));
       // ui->tableView->setModel(model);
       // ui->tableView->setSortingEnabled(true);


        //----------------------------------------//

        EditForm->setParent(this,Qt::Window);
        EditForm->setModel(t_model);

       // query.exec();

       // t_model->submitAll();

}


void MainWindow::on_pushButton_2_clicked()
{
  qDebug() << " insert row " <<   t_model->insertRow(t_model->rowCount());
}


void MainWindow::on_pushButton_3_clicked()
{
    int selectRow = ui->tableView->currentIndex().row();
    if( selectRow >= 0)
    {
        qDebug() << "Delete row " << t_model->removeRow(selectRow);
    }
    else
    {
        qDebug() << "no row selected ";
    }
}


void MainWindow::on_pushButton_4_clicked()
{
    t_model->submitAll();
}


void MainWindow::on_pushButton_5_clicked()
{
    t_model->revertAll();
}


void MainWindow::on_pushButton_6_clicked()
{
    t_model->select();
}


void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    EditForm->mapper->setCurrentModelIndex(index);
    EditForm->FindProducts();
    EditForm->show();
}


void MainWindow::on_pushButton_7_clicked()
{

    if(!m_Server)
    {
        QHostAddress IP = QHostAddress(ui->lineEdit_IP->text());
        quint16 PORT = 1234;

        m_Server = new MyServer();
        m_Server->StartServer(IP,PORT);
        m_Server->m_Query = m_Query;
        ui->pushButton_7->setText("Stop Server");
    }
    else
    {
        m_Server->disconnect();
        m_Server->close();
        delete  m_Server;
        m_Server = nullptr;
        ui->pushButton_7->setText("Start Server");
    }


}



void MainWindow::on_pushButton_8_clicked()
{
    m_Query->prepare("SELECT * FROM fulfill WHERE ID_Oreder = :id ");
    m_Query->bindValue(":id", 4);
    m_Query->exec();

    while(m_Query->next())
    {
        qDebug() << m_Query->value(1).toString();
    }
}


void MainWindow::on_pushButton_AddUser_clicked()
{
    //     QSqlQuery query;
    //     query.prepare("INSERT INTO users (Username, Password, ID) "
    //                   "VALUES (:Username, :Password, :ID)");

    //     query.bindValue(":Username", "wera");
    //     query.bindValue(":Password", "123");
    //     query.bindValue(":ID", 1001);

    //     query.exec();

         m_AddClientWidget->setQuery(m_Query);
         m_AddClientWidget->show();
}


void MainWindow::on_pushButton_Preorder_clicked()
{

        //      m_Query->prepare("INSERT INTO tab (ID, Text, Date) "
        //                    "VALUES (:ID, :Text, :Date)");
        //      m_Query->bindValue(":ID", 1001);
        //      m_Query->bindValue(":Text", "Bart");
        //      m_Query->bindValue(":Date", QString("2000-01-01"));
        //     /// query.exec();

//    m_Query->prepare("INSERT INTO tab ( Data ) "
//                     "VALUES ( :Data )");

    //m_Query->bindValue(":ID_Oreder", _order.ID);
    //m_Query->bindValue(":Text", "_order.Text");
   // m_Query->bindValue(":Data", "01.01.2000");

//    m_Query->prepare("INSERT preorders (ID, Text, Data) "
//                       "VALUES (:ID , :Text, :Data)");
//    m_Query->bindValue(":Text", "wkkkxxxxxxxxx");
   // m_Query->bindValue(":ID" ,20);
   // m_Query->bindValue(":Data" ,QString("01.01.2020"));
   // m_Query->exec();

    m_AddPreorderWidget->setQuery(m_Query);
    m_AddPreorderWidget->FindProducts();
    m_AddPreorderWidget->show();
    t_model->submit();
}


void MainWindow::on_pushButton_AddProduct_clicked()
{
    m_AddProductWidget->setQuery(m_Query);
    m_AddProductWidget->show();
    t_model->submit();
}

