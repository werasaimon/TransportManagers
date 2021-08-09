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
//    db = QSqlDatabase::addDatabase("QMYSQL");
//    db.setHostName("127.0.0.1");
//    db.setDatabaseName("db_wera2");
//    db.setUserName("wera2");
//    db.setPassword("123");


    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./database.db");
//////    db.setUserName("werasaimon");
//////    db.setPassword("123qwe");

    if(!db.open())
    {
        qDebug() << db.lastError().text();
    }
    else
    {
        qDebug() << "Succses!";
        QString quary = "CREATE TABLE preorders ("
                                        "ID INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,"
                                        "Text VARCHAR(255),"
                                        "Date DATATIME, "
                                        "Username VARCHAR(255),"
                                        "Weight INTEGER,"
                                        "Address VARCHAR(255),"
                                        "Product VARCHAR(255),"
                                        "Status VARCHAR(255));";

        //"Status  ENUM ('Waiting', 'Executo', 'Final'));";

        QString quary2 = "CREATE TABLE users ("
                                         "Username VARCHAR(255),"
                                         "Password VARCHAR(255),"
                                         "ID INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE);";


        QString quary3 = "CREATE TABLE fulfill ("
                                         //"ID INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,"
                                         "ID_Oreder INTEGER,"
                                         "Username VARCHAR(25),"
                                         "Text VARCHAR(255),"
                                         "Data DATATIME,"
                                         "PRIMARY KEY(ID_Oreder,Username));";

        QString quary4 = "CREATE TABLE products ("
                                         "ID INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,"
                                         "Name VARCHAR(255));";

        QSqlQuery qry(db);

        if(!qry.exec(quary))
        {
            qDebug() << "error creating table preorders";
        }

        if(!qry.exec(quary2))
        {
            qDebug() << "error creating table users";
        }

        if(!qry.exec(quary3))
        {
            qDebug() << "error creating table fulfill";
        }

        if(!qry.exec(quary4))
        {
            qDebug() << "error creating table products";
        }

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


    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), SLOT(customMenuRequested(QPoint)));


}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_Query;
}


void MainWindow::on_pushButton_clicked()
{
//    if(!m_Query->exec(ui->plainTextEditScript->toPlainText().toStdString().c_str()))
//    {
//        qDebug() << m_Query->lastError().databaseText();
//        qDebug() << m_Query->lastError().driverText();
//        return;
//    }
//    else
//    {
//        qDebug() << "query.record()";
//        while (m_Query->next())
//        {
//            qDebug() << m_Query->record();
//        }
//    }



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
        ui->tableView->setModel(t_model);
       // ui->tableView->setSortingEnabled(true);


        //----------------------------------------//

        EditForm->setParent(this,Qt::Window);
        EditForm->setModel(t_model);

       // query.exec();

       // t_model->submitAll();

}


//void MainWindow::on_pushButton_3_clicked()
//{
//    int selectRow = ui->tableView->currentIndex().row();
//    if( selectRow >= 0)
//    {
//        qDebug() << "Delete row " << t_model->removeRow(selectRow);
//       //QDataWidgetMapper *mapper;
//       //mapper->setModel(model);
//        //mapper->addMapping(ui->lineEdit_ID,0);
//    }
//    else
//    {
//        qDebug() << "no row selected ";
//    }

//}






QModelIndex index;
void MainWindow::customMenuRequested(QPoint pos)
{
    index=ui->tableView->indexAt(pos);

    QMenu *menu=new QMenu(this);

    QAction * editDevice = new QAction(trUtf8("Редактировать"), this);
    QAction * deleteDevice = new QAction(trUtf8("Удалить"), this);

    connect(editDevice, SIGNAL(triggered()), this, SLOT(slotEditRecord()));     // Call Handler dialog editing
    connect(deleteDevice, SIGNAL(triggered()), this, SLOT(slotRemoveRecord())); // Handler delete records

    menu->addAction(editDevice);
    menu->addAction(deleteDevice);

    menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
}

void MainWindow::slotEditRecord()
{
    qDebug() << "Редактировать";

    EditForm->mapper->setCurrentModelIndex(index);
    EditForm->FindProducts();
    EditForm->show();
}

void MainWindow::slotRemoveRecord()
{
    qDebug() << "Удалить";
    int selectRow = ui->tableView->currentIndex().row();
    if( selectRow >= 0)
    {
        if(ui->comboBox_TABLE->currentText() == QString("preorders"))
        {
            auto index = ui->tableView->selectionModel()->currentIndex();
            auto value = index.sibling(index.row(),0).data();
            int ID = value.toInt();
            m_Query->prepare("DELETE FROM fulfill WHERE ID_Oreder = :id");
            m_Query->bindValue(":id", ID);
            m_Query->exec();

            qDebug() << "Print  : " << value.toInt() ;
        }

        qDebug() << "Delete row " << t_model->removeRow(selectRow);
       //QDataWidgetMapper *mapper;
       //mapper->setModel(model);
        //mapper->addMapping(ui->lineEdit_ID,0);
        //t_model->submitAll();
    }
    else
    {
        qDebug() << "no row selected ";
    }
}


void MainWindow::on_actionAdd_Orders_triggered()
{
    m_AddPreorderWidget->setQuery(m_Query);
    m_AddPreorderWidget->FindProducts();
    m_AddPreorderWidget->show();
    //t_model->select();
    //t_model->submitAll();
}


void MainWindow::on_actionAdd_Product_triggered()
{
    m_AddProductWidget->setQuery(m_Query);
    m_AddProductWidget->show();
    //t_model->submit();
}


void MainWindow::on_actionAdd_User_triggered()
{
    m_AddClientWidget->setQuery(m_Query);
    m_AddClientWidget->show();
}



void MainWindow::on_pushButton_Submit_clicked()
{
    t_model->submitAll();
}


void MainWindow::on_pushButton_StartServer_clicked()
{
    if(!m_Server)
    {
        QHostAddress IP = QHostAddress(ui->lineEdit_IP->text());
        quint16 PORT = 1234;

        m_Server = new IServer();
        m_Server->StartServer(IP,PORT);
        m_Server->m_Query = m_Query;
        ui->pushButton_StartServer->setText("Stop Server");
    }
    else
    {
        m_Server->disconnect();
        m_Server->close();
        delete  m_Server;
        m_Server = nullptr;
         qDebug() << "Server: stop";
        ui->pushButton_StartServer->setText("Start Server");
    }
}


void MainWindow::on_comboBox_TABLE_currentTextChanged(const QString &arg1)
{
    qDebug() << arg1;

    QString table_name = arg1;
    t_model->setQuery(QSqlQuery(QString("SELECT * FROM") + QString(table_name)));
    t_model->setTable(table_name);
    t_model->select();
}

