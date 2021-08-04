#ifndef DIALOGADDPREORDER_H
#define DIALOGADDPREORDER_H

#include <QDialog>
#include <QSqlQuery>


namespace Ui {
class Dialogaddpreorder;
}

class Dialogaddpreorder : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogaddpreorder(QWidget *parent = nullptr);
    ~Dialogaddpreorder();

    QSqlQuery *Query() const;

    void setQuery(QSqlQuery *newQuery);
    void resetQuery();

    void FindProducts();

signals:
    void QueryChanged();

private slots:
    void on_pushButton_Add_clicked();
    void on_FindPorodutsInCombobox();

private:
    Ui::Dialogaddpreorder *ui;
    QSqlQuery *m_Query;

    Q_PROPERTY(QSqlQuery * Query READ Query WRITE setQuery RESET resetQuery NOTIFY QueryChanged)
};

#endif // DIALOGADDPREORDER_H
