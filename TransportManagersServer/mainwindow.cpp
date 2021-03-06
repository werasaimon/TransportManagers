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

//    db = QSqlDatabase::addDatabase("QMYSQL");
//    db.setHostName("127.0.0.1");
//    db.setDatabaseName("wera");
//    db.setUserName("werasaimon");
//    db.setPassword("123qwe");


    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./users.db");
    //db.setUserName("werasaimon");
    //db.setPassword("123qwe");

    if(!db.open())
    {
        qDebug() << db.lastError().text();
    }
    else
    {
        qDebug() << "Succses!";
        QString quary = "CREATE TABLE test ("
                        "ID INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,"
                        "Text VARCHAR(255),"
                        "Data DATATIME);";

        QSqlQuery qry(db);
        if(!qry.exec(quary))
        {
            qDebug() << "error creating table";
        }
        //qDebug() << "yes create table";
    }

    t_model = new QSqlTableModel2(this,db);
    t_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    t_model->setTable("test");
    t_model->select();

    ui->tableView->setModel(t_model);
    ui->tableView->setSortingEnabled(true);

    //----------------------------------------//

    EditForm = new DialogRecortEditForm(this);
    EditForm->setParent(this,Qt::Window);
    EditForm->SetModel(t_model);

    //----------------------------------------//

    m_Query = new QSqlQuery(db);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    if(!m_Query->exec("SELECT * FROM test"))
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
        EditForm->SetModel(t_model);

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
    EditForm->show();
}


void MainWindow::on_pushButton_7_clicked()
{
    QHostAddress IP = QHostAddress(ui->lineEdit_IP->text());
    quint16 PORT = 1234;

    m_Server = new MyServer();
    m_Server->StartServer(IP,PORT);
    m_Server->m_Query = m_Query;
}

