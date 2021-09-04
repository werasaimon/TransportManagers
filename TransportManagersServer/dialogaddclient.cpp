#include "dialogaddclient.h"
#include "ui_dialogaddclient.h"

DialogAddClient::DialogAddClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddClient)
{
    ui->setupUi(this);
    this->setWindowTitle( "Добавить Клиента!" );
}

DialogAddClient::~DialogAddClient()
{
    delete ui;
}

void DialogAddClient::on_buttonBox_accepted()
{
    m_Query->prepare("INSERT INTO users (Username, Password, ID) "
                     "VALUES (:Username, :Password, :ID)");

    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();

    m_Query->bindValue(":Username", username);
    m_Query->bindValue(":Password", password);
   // m_Query->bindValue(":ID", 1001);

    m_Query->exec();
}

QSqlQuery *DialogAddClient::Query() const
{
    return m_Query;
}

void DialogAddClient::setQuery(QSqlQuery *newQuery)
{
    if (m_Query == newQuery)
        return;
    m_Query = newQuery;
    emit QueryChanged();
}

void DialogAddClient::resetQuery()
{
    setQuery({}); // TODO: Adapt to use your actual default value
}

