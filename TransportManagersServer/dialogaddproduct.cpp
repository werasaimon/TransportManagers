#include "dialogaddproduct.h"
#include "ui_dialogaddproduct.h"

Dialogaddproduct::Dialogaddproduct(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogaddproduct)
{
    ui->setupUi(this);
}

Dialogaddproduct::~Dialogaddproduct()
{
    delete ui;
}

void Dialogaddproduct::on_buttonBox_accepted()
{
    m_Query->prepare("INSERT INTO products (Name) "
                     "VALUES (:Name)");

    QString NameProduct = ui->lineEdit_NameProduct->text();
    m_Query->bindValue(":Name", NameProduct);
    m_Query->exec();
}

QSqlQuery *Dialogaddproduct::Query() const
{
    return m_Query;
}

void Dialogaddproduct::setQuery(QSqlQuery *newQuery)
{
    if (m_Query == newQuery)
        return;
    m_Query = newQuery;
    emit QueryChanged();
}

void Dialogaddproduct::resetQuery()
{
    setQuery({}); // TODO: Adapt to use your actual default value
}

