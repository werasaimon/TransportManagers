#ifndef DIALOGADDCLIENT_H
#define DIALOGADDCLIENT_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class DialogAddClient;
}

class DialogAddClient : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddClient(QWidget *parent = nullptr);
    ~DialogAddClient();

    QSqlQuery *Query() const;
    void setQuery(QSqlQuery *newQuery);
    void resetQuery();

signals:
    void QueryChanged();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogAddClient *ui;
    QSqlQuery *m_Query;
    Q_PROPERTY(QSqlQuery * Query READ Query WRITE setQuery RESET resetQuery NOTIFY QueryChanged)
};

#endif // DIALOGADDCLIENT_H
