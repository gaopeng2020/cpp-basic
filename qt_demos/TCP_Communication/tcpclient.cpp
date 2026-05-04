#include "tcpclient.h"
#include "ui_tcpclient.h"


QString TCPClient::getServerInfo(QString prefix)
{
    QString ipAddr =   ui->lineEdit_IPv4->text();
    quint16 port = ui->lineEdit_Port->text().toUInt();
    QString serverInfo =  QString(prefix+"[%1 : %2]").arg(ipAddr).arg(port);

    return serverInfo;
}

TCPClient::TCPClient(QWidget *parent) : QWidget(parent), ui(new Ui::TCPClient)
{
    ui->setupUi(this);
    tcpSocket = new QTcpSocket(this);

    //如果成功和对方建立好连接，通信套接字会自动触发 connected信号,可以在槽函数中显示提示信息
    connect(tcpSocket,&QTcpSocket::connected,[=](){
        QString serverInfo = getServerInfo("Succesfully connected to Server");
        ui->textEdit_RxMsg->append(serverInfo);
    });


    //成功建立连接之后，通过通信套接字的readyRead信号，在槽函数中通过tcpSocket->readAll()获取Server发送的数据,并在  ui->textEdit_RxMsg显示
    connect(tcpSocket,&QTcpSocket::readyRead,[=](){
        QByteArray data =  tcpSocket->readAll();
        QString serverInfo = getServerInfo("")+data;
        ui->textEdit_RxMsg->append(serverInfo.toUtf8());

        //接受的消息设置右对齐
        auto cursor = ui->textEdit_RxMsg->textCursor();
        auto blockFormat = cursor.blockFormat();
        blockFormat.setAlignment(Qt::AlignLeft);
        cursor.mergeBlockFormat(blockFormat);
        ui->textEdit_RxMsg->setTextCursor(cursor);
    });

    //断开连接提示
    connect(tcpSocket,&QTcpSocket::disconnected,[=](){
        QString serverInfo = getServerInfo("Succesfully disconnected from Server");
        ui->textEdit_RxMsg->append(serverInfo);
    });
}

TCPClient::~TCPClient()
{
    delete ui;
}


//建立连接
void TCPClient::on_button_Connect_released()
{
    //通过指定的socket与server建立TCP连接
    if( ui->button_Connect->text()=="Connect"){
        QString ipAddr =   ui->lineEdit_IPv4->text();
        quint16 port = ui->lineEdit_Port->text().toUInt();
        tcpSocket->connectToHost(QHostAddress(ipAddr),port);
        ui->button_Connect->setText("Disconnect");
    }else {
        if (!tcpSocket->isOpen()){
            return;
        }
        tcpSocket->disconnectFromHost();
        tcpSocket->close();
        ui->button_Connect->setText("Connect");
    }
}

//发送数据
void TCPClient::on_button_Send_released()
{
    if (!tcpSocket->isValid()){
        ui->textEdit_RxMsg->append("client has not connected to Server");
        return;
    }
    //获取ui->textEdit_TxMsg内容，并通过tcpSocket->write()将其发送给服务器
    QString msg_Tx =  ui->textEdit_TxMsg->toPlainText();
    tcpSocket->write(msg_Tx.toUtf8().data());

    //清空发送消息框
    ui->textEdit_TxMsg->clear();

    //自己发送的消息设置右对齐
    ui->textEdit_RxMsg->append(msg_Tx);
    auto cursor = ui->textEdit_RxMsg->textCursor();
    auto blockFormat = cursor.blockFormat();
    blockFormat.setAlignment(Qt::AlignRight);
    cursor.mergeBlockFormat(blockFormat);
    ui->textEdit_RxMsg->setTextCursor(cursor);
}

//释放连接
void TCPClient::on_button_Close_released()
{
    if (tcpSocket->isOpen()){
        tcpSocket->disconnectFromHost();
        tcpSocket->close();
    }
    this->close();
}

