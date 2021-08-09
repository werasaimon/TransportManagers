#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include "dialogrecorteditform.h"
#include "dialogaddclient.h"
#include "dialogaddpreorder.h"
#include "dialogaddproduct.h"

#include "server/iserver.h"

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
    void on_pushButton_Submit_clicked();
    void on_pushButton_StartServer_clicked();

    void customMenuRequested(QPoint pos);
    void slotEditRecord();
    void slotRemoveRecord();

    // QMenu--QToolBar
    void on_actionAdd_Orders_triggered();
    void on_actionAdd_Product_triggered();
    void on_actionAdd_User_triggered();




    void on_comboBox_TABLE_currentTextChanged(const QString &arg1);

private:


    Ui::MainWindow *ui;

    DialogRecortEditForm *EditForm;
    DialogAddClient   *m_AddClientWidget;
    Dialogaddpreorder *m_AddPreorderWidget;
    Dialogaddproduct  *m_AddProductWidget;

    //--------------------------------//

    QSqlDatabase db;
    QSqlTableModel2 *t_model;

    // Create an instance of a server.
    IServer *m_Server = nullptr;
    QSqlQuery *m_Query;
};
#endif // MAINWINDOW_H
