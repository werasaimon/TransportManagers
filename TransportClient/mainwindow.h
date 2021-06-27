#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include "qjsontablemodel.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onReadyRead();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_pushButton_test_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket  m_TCPSocket;

    QJsonTableModel* episodes;
};
#endif // MAINWINDOW_H
