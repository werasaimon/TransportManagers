#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include "../Common/qjsontablemodel.h"



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


    void weAreConnected();
    void displayError(QAbstractSocket::SocketError socketError);
    void sockDisc();
//    void sendSomething();


    void on_pushButton_A_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket  *m_TCPSocket = nullptr;

    QJsonTableModel* episodes;

    void AnswerToKey_SQL(QDataStream &_stream_tcp_ip);
    void AnswerToKey_User(QDataStream &_stream_tcp_ip);
};
#endif // MAINWINDOW_H
