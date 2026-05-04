#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>

/**
 *1. 客户端通过指定的Socket与服务器建立 TCP链接(tcpSocket->connectToHost(QHostAddress(ipAddr),port);建立连接)；
 *  成功建立连接之后，通信套接字会自动触发 connected信号，可在槽函数中将Server信息显示出来;
 *2. 成功建立连接之后，通过通信套接字的readyRead信号，在槽函数中通过tcpSocket->readAll()获取数据 (接收数据);
 *3. 成功建立连接之后，可通过tcpSocket->write(const char* data)发送数据给Server（发送数据);
 *4. 如果对方主动断开连接，通信套接字会自动触发 disconnecte().
 */
namespace Ui {
class TCPClient;
}

class TCPClient : public QWidget
{
    Q_OBJECT

public:
    explicit TCPClient(QWidget *parent = nullptr);
    ~TCPClient();

private slots:
    void on_button_Send_released();

    void on_button_Connect_released();

    void on_button_Close_released();

private:
    Ui::TCPClient *ui;
    QTcpSocket *tcpSocket;

    QString getServerInfo(QString prefix);
};

#endif // TCPCLIENT_H
