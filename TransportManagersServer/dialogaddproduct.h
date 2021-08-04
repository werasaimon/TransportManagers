#ifndef DIALOGADDPRODUCT_H
#define DIALOGADDPRODUCT_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class Dialogaddproduct;
}

class Dialogaddproduct : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogaddproduct(QWidget *parent = nullptr);
    ~Dialogaddproduct();

    QSqlQuery *Query() const;
    void setQuery(QSqlQuery *newQuery);
    void resetQuery();

signals:
    void QueryChanged();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Dialogaddproduct *ui;
    QSqlQuery *m_Query;
    Q_PROPERTY(QSqlQuery * Query READ Query WRITE setQuery RESET resetQuery NOTIFY QueryChanged)
};

#endif // DIALOGADDPRODUCT_H
