#ifndef DIALOGADDPREORDER_H
#define DIALOGADDPREORDER_H

#include <QDialog>
#include <QTcpSocket>
#include <QString>

class BlockReader;

namespace Ui {
class Dialogaddpreorder;
}

class Dialogaddpreorder : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogaddpreorder(QWidget *parent = nullptr);
    ~Dialogaddpreorder();

    QTcpSocket *TCPSocket() const;
    void setTCPSocket(QTcpSocket *newTCPSocket);

    void setUsername(const QString &newUsername);

    void onReadyRead( QString key , BlockReader *br);

private slots:
    void on_pushButton_AddPreorder_clicked();
    void on_pushButton_UpdateProducts_clicked();

private:

    void AnswerToKey_Products(QDataStream &_stream_tcp_ip);

private:
    Ui::Dialogaddpreorder *ui;
    QTcpSocket  *m_TCPSocket = nullptr;
    QString      m_Username;

};

#endif // DIALOGADDPREORDER_H
