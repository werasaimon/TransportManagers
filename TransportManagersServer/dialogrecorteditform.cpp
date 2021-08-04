#include "dialogrecorteditform.h"
#include "ui_dialogrecorteditform.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

DialogRecortEditForm::DialogRecortEditForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRecortEditForm)
{
    ui->setupUi(this);

    //----------------------------------------------------//

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

    //----------------------------------------------------//

    QObject::connect(ui->comboBox_NameProducts, SIGNAL(activated(QString)), this, SLOT(on_InsertProduct(QString)));

}

DialogRecortEditForm::~DialogRecortEditForm()
{
    delete ui;
}

void DialogRecortEditForm::setModel(QAbstractItemModel *model)
{
    mapper->setModel(model);
    mapper->addMapping(ui->lineEdit_ID,0);
    mapper->addMapping(ui->plainTextEdit,1);
    mapper->addMapping(ui->dateEdit,2);
    mapper->addMapping(ui->comboBox_Clients,3);
    mapper->addMapping(ui->spinBox_Weight,4);
    mapper->addMapping(ui->lineEdit_Address,5);
    mapper->addMapping(ui->comboBox_NameProducts,6);
    mapper->addMapping(ui->comboBox_Status,7);
}

void DialogRecortEditForm::setQuery(QSqlQuery *newQuery)
{
    m_Query = newQuery;
}

void DialogRecortEditForm::FindProducts()
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

void DialogRecortEditForm::on_pushButton_clicked()
{
   mapper->submit();
   close();
}




void DialogRecortEditForm::on_pushButton_2_clicked()
{
    m_Query->prepare("SELECT * FROM fulfill WHERE ID_Oreder = :id ");
    m_Query->bindValue(":id", ui->lineEdit_ID->text().toInt());
    m_Query->exec();

   // int nameID = (*m_Query).record().;

    ui->comboBox_Clients->clear();
    int name = m_Query->record().indexOf("Username");
    while(m_Query->next())
    {
        QString item = m_Query->value(name).toString();
        ui->comboBox_Clients->insertItem(0,item);
        //ui->comboBox_Clients->setCurrentText(item);
    }
     ui->comboBox_Clients->insertItem(0,"");


//    while (query.next()) {
//          ui->comboBox->addItem(query.value(nameID).toString());
//          ui->comboBox->addItem(query.value(groupID).toString());
     //        }
}

void DialogRecortEditForm::on_InsertProduct(QString _current)
{
   ui->lineEdit_CurrentProduct->setText(_current);
   //ui->comboBox_Status->setCurrentText("Executor");
}


