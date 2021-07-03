#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include "dialogrecorteditform.h"

#include "server/myserver.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class QSqlTableModel2 : public QSqlTableModel
{
    //Q_OBJECT

public:

    explicit QSqlTableModel2(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase()) :
        QSqlTableModel(parent,db)
    {

    }

    void setQuery(const QSqlQuery &query)
    {
        QSqlTableModel::setQuery(query);
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_pushButton_7_clicked();

    void on_pushButton_ttt_clicked();

private:

    QSqlDatabase db;
    Ui::MainWindow *ui;
    QSqlTableModel2 *t_model;
    DialogRecortEditForm *EditForm;

    // Create an instance of a server.
    MyServer *m_Server;
    QSqlQuery *m_Query;
};
#endif // MAINWINDOW_H
