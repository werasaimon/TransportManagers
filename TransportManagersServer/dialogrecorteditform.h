#ifndef DIALOGRECORTEDITFORM_H
#define DIALOGRECORTEDITFORM_H

#include <QDialog>
#include <QDataWidgetMapper>

namespace Ui {
class DialogRecortEditForm;
}

class DialogRecortEditForm : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRecortEditForm(QWidget *parent = nullptr);
    ~DialogRecortEditForm();

    void SetModel(QAbstractItemModel *model);

private slots:

    void on_pushButton_clicked();


private:
    Ui::DialogRecortEditForm *ui;

public:
    QDataWidgetMapper *mapper;
};

#endif // DIALOGRECORTEDITFORM_H
