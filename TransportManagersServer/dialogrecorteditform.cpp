#include "dialogrecorteditform.h"
#include "ui_dialogrecorteditform.h"

DialogRecortEditForm::DialogRecortEditForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRecortEditForm)
{
    ui->setupUi(this);

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

}

DialogRecortEditForm::~DialogRecortEditForm()
{
    delete ui;
}

void DialogRecortEditForm::SetModel(QAbstractItemModel *model)
{
    mapper->setModel(model);
    mapper->addMapping(ui->plainTextEdit,1);
    mapper->addMapping(ui->dateEdit,2);
}

void DialogRecortEditForm::on_pushButton_clicked()
{
   mapper->submit();
   close();
}



