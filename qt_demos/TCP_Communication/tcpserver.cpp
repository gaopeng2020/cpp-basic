#include "tcpserver.h"
#include "ui_tcpserver.h"

TCPServer::TCPServer(QWidget *parent)  : QMainWindow(parent) , ui(new Ui::TCPServer)
{
    ui->setupUi(this);
    tcpServer = nullptr;
    tcpSocket = nullptr;

    //创建套接字并监听客户端请求，指定父对象的目的是Qt其自动回收内存空间
    tcpServer = new QTcpServer(this);
    tcpServer->listen(QHostAddress::Any,8888);

    //与客户端建立连接
    connect(tcpServer,&QTcpServer::newConnection,[=](){
        //取出与客户端建立好连接的套接字
        tcpSocket = tcpServer->nextPendingConnection();

        //在textEdit_RxMsg中显示客户端套接字信息
        QString ipvAddress =  tcpSocket->peerAddress().toString();
        ipvAddress = ipvAddress.right(ipvAddress.length() - ipvAddress.lastIndexOf(":")-1);
        uint16_t port =  tcpSocket->peerPort();
        QString clientInfo = QString("Succesfully conneced to client :[%1 : %2]").arg(ipvAddress).arg(port);
        ui->textEdit_RxMsg->append(clientInfo);

        //接收client数据，如果有客户端发送数据给服务器，socket会自动触发readyRead信号，可以在槽函数中通过tcpSocket->readAll()读取数据
        connect(tcpSocket,&QTcpSocket::readyRead,[=](){
            QByteArray data = tcpSocket->readAll();
            ui->textEdit_RxMsg->append(QString("[%1 : %2]").arg(ipvAddress).arg(port)+data);
            //接受的消息设置左对齐
            auto cursor = ui->textEdit_RxMsg->textCursor();
            auto blockFormat = cursor.blockFormat();
            blockFormat.setAlignment(Qt::AlignLeft);
            cursor.mergeBlockFormat(blockFormat);
            ui->textEdit_RxMsg->setTextCursor(cursor);
        });

        //断开连接提示
        connect(tcpSocket,&QTcpSocket::disconnected,[=](){
            QString clientInfo =  QString("Succesfully disconnected from client:[%1 : %2]").arg(ipvAddress).arg(port);
            ui->textEdit_RxMsg->append(clientInfo);
        });
    });
}

TCPServer::~TCPServer()
{
    delete ui;
}

//按button_Send按钮时，发送数据到client
void TCPServer::on_button_Send_released()
{
    if(tcpSocket == nullptr){
        ui->textEdit_RxMsg->append("no client connected to Server");
        return;
    }
    char * data =  ui->textEdit_TxMsg->toPlainText().toUtf8().data();
    ui->textEdit_RxMsg->append(ui->textEdit_TxMsg->toPlainText());
    tcpSocket->write(data);

    //清空发送消息框
    ui->textEdit_TxMsg->clear();

    //自己发送的消息设置右对齐
    auto cursor = ui->textEdit_RxMsg->textCursor();
    auto blockFormat = cursor.blockFormat();
    blockFormat.setAlignment(Qt::AlignRight);
    cursor.mergeBlockFormat(blockFormat);
    ui->textEdit_RxMsg->setTextCursor(cursor);
}

//按button_Close按钮时，释放与client的Socket.
void TCPServer::on_button_Close_released()
{
    if(tcpSocket != nullptr){
        tcpSocket->disconnectFromHost();
        tcpSocket->close();
        tcpSocket = nullptr;
    }
    this->close();
}

