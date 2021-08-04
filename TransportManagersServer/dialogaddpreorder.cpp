#include "dialogaddpreorder.h"
#include "ui_dialogaddpreorder.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

Dialogaddpreorder::Dialogaddpreorder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogaddpreorder)
{
    ui->setupUi(this);

//   QObject::connect(ui->comboBox_NameProducts, SIGNAL(activated(QString)), this, SLOT(on_FindPorodutsInCombobox()));

//    m_Query->prepare("SELECT * FROM fulfill WHERE ID_Oreder = :id ");
//    m_Query->bindValue(":id", ui->lineEdit_ID->text().toInt());
//    m_Query->exec();

//   // int nameID = (*m_Query).record().;

//    ui->comboBox_Clients->clear();
//    int name = m_Query->record().indexOf("Username");
//    while(m_Query->next())
//    {
//        QString item = m_Query->value(name).toString();
//        ui->comboBox_Clients->insertItem(0,item);
//        //ui->comboBox_Clients->setCurrentText(item);
//    }
}

Dialogaddpreorder::~Dialogaddpreorder()
{
    delete ui;
}

QSqlQuery *Dialogaddpreorder::Query() const
{
    return m_Query;
}

void Dialogaddpreorder::setQuery(QSqlQuery *newQuery)
{
    if (m_Query == newQuery)
        return;
    m_Query = newQuery;
    emit QueryChanged();
}

void Dialogaddpreorder::resetQuery()
{
    setQuery({}); // TODO: Adapt to use your actual default value
}

void Dialogaddpreorder::FindProducts()
{
    m_Query->prepare("SELECT * FROM products");
    m_Query->exec();

    ui->comboBox_NameProducts->clear();
    int name = m_Query->record().indexOf("Name");
    while(m_Query->next())
    {
        QString item = m_Query->value(name).toString();
        ui->comboBox_NameProducts->insertItem(0,item);
        ui->comboBox_NameProducts->setCurrentText(item);
    }
}


void Dialogaddpreorder::on_pushButton_Add_clicked()
{
    //qDebug() << "||| " << ui->dateEdit->dateTime().toString("yyyy-MM-dd");

    int Weight = ui->spinBox_Weight->value();
    QString date = ui->dateEdit->dateTime().toString("yyyy-MM-dd");
    QString address = ui->lineEdit_Address->text();
    QString product = ui->comboBox_NameProducts->currentText();

    m_Query->prepare("INSERT INTO preorders (ID, Text, Date, Weight, Address, Product) "
                     "VALUES (:ID, :Text, :Date, :Weight, :Address, :Product)");
    //m_Query->bindValue(":ID", 1001);
    m_Query->bindValue(":Text",ui->textEdit->toPlainText());
    m_Query->bindValue(":Date",date);
    m_Query->bindValue(":Weight",Weight);
    m_Query->bindValue(":Address",address);
    m_Query->bindValue(":Product",product);
    m_Query->exec();
    close();
}

void Dialogaddpreorder::on_FindPorodutsInCombobox()
{
    //if(arg == 0)
    {
        qDebug() << "ARG -----------------";
    }
}

