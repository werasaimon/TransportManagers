#ifndef DIALOGRECORTEDITFORM_H
#define DIALOGRECORTEDITFORM_H

#include <QDialog>
#include <QDataWidgetMapper>
#include <QSqlQuery>

namespace Ui {
class DialogRecortEditForm;
}

class DialogRecortEditForm : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRecortEditForm(QWidget *parent = nullptr);
    ~DialogRecortEditForm();

    void setModel(QAbstractItemModel *model);
    void setQuery(QSqlQuery *newQuery);

     void FindProducts();

private slots:

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_SelectedProduct(QString _current);
    void on_SelectedClient(QString _current);

    void on_checkBox_Calendar_toggled(bool checked);
    void on_pushButton_Delete_clicked();

private:
    Ui::DialogRecortEditForm *ui;

public:
    QDataWidgetMapper *mapper;
    QSqlQuery *m_Query;

};

#endif // DIALOGRECORTEDITFORM_H
