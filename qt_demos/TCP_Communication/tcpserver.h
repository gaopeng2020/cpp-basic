#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextCursor>

/**
 * TCP Server 通信流程：
 * 服务端QTcpServer有个listen(QHostAddress::Any,port)函数自动监听来自任意客户端的请求（监听）；
 * 如果连接成功服务器会触发newConnection信号，通过槽函数可以取出 tcpSocket = tcpServer->nextPendingConnection()（获取client Socket）；
 * 成功建立连接之后，可以通过通信套接字的readyRead信号，在槽函数中接受client发送的数据 (接收数据);
 * 成功建立连接之后，通信套接字会自动触发 connected信号，可在槽函数中发送数据给client（发送数据);
 * 如果对方主动断开连接，通信套接字会自动触发 disconnecte信号
 */

QT_BEGIN_NAMESPACE
namespace Ui { class TCPServer; }
QT_END_NAMESPACE

class TCPServer : public QMainWindow
{
    Q_OBJECT

public:
    TCPServer(QWidget *parent = nullptr);
    ~TCPServer();

private slots:
    void on_button_Send_released();

    void on_button_Close_released();

private:
    Ui::TCPServer *ui;
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;

};
#endif // TCPSERVER_H
