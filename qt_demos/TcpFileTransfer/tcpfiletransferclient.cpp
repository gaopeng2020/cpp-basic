#include "tcpfiletransferclient.h"
#include "ui_tcpfiletransferclient.h"

TcpFileTransferClient::TcpFileTransferClient(QWidget* parent) : QWidget(parent), ui(new Ui::TcpFileTransferClient)
{
    ui->setupUi(this);
    this->setWindowTitle("TCP File Receiver");

    tcpSocket = nullptr;
    tcpSocket = new QTcpSocket(this);


    ui->pushButton_Receive->setEnabled(false);
    ui->pushButton_Reject->setEnabled(false);

    //成功建立连接之后，通过通信套接字的readyRead信号，在槽函数中通过tcpSocket->readAll()获取Server发送的数据
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(receiverFile()));

    //在TestEdit框中提示连接成功信息
    connect(tcpSocket, &QTcpSocket::connected, [=]() {
        ui->textEdit_Status->setText("Succesfully connected to Server");
    });

    connect(tcpSocket, &QTcpSocket::disconnected, this, [=]() {
        ui->textEdit_Status->setText("Succesfully disconnected from Server");
    });

}

TcpFileTransferClient::~TcpFileTransferClient()
{
    delete ui;
}

void TcpFileTransferClient::on_pushButton_Connect_clicked()
{
    //连接到Server
    QString text = ui->pushButton_Connect->text();
    if (text == "Connect")
    {
        QString  ipAddr = ui->lineEdit_IpAddress->text();
        uint port = ui->lineEdit_Port->text().toUInt();
        tcpSocket->connectToHost(QHostAddress(ipAddr), port);
        if (!tcpSocket->waitForConnected())
        {
            ui->textEdit_Status->setText("connection error");
            return;
        }
        ui->pushButton_Connect->setText("DisConnect");
    }
    else
    {
        tcpSocket->disconnectFromHost();
        ui->pushButton_Connect->setText("Connect");
    }
}


void TcpFileTransferClient::on_pushButton_Receive_clicked()
{
    file = new QFile(this);
    QString   dirPath = QFileDialog::getExistingDirectory(this, "Select Directory", "./");
    QString filePath = QString("%1/%2").arg(dirPath).arg(fileName);
    ui->textEdit_Status->append("File will be saved in : " + filePath);

    file->setFileName(filePath);
    bool isFileOpend = file->open(QIODevice::WriteOnly);
    if (!isFileOpend)
    {
        ui->textEdit_Status->append("File opend failed !");
        return;
    }
    tcpSocket->write(QString("Receice the file").toUtf8());
    //tcpSocket->waitForBytesWritten(2000);

    ui->pushButton_Receive->setEnabled(false);
    ui->pushButton_Reject->setEnabled(false);
    ui->pushButton_Connect->setEnabled(false);
}

void TcpFileTransferClient::receiverFile() {
    QByteArray bufferRx = tcpSocket->readAll();
    //判断buf中数据是不是文件信息,只有文件数据才需要写入到文件，文件名称大小信息不写入。
    if (isFileInfoData)
    {
        //解析文件信息，结构：QString  fileInfo = QString("FileName=%1 , FileSize = %2").arg(fileName).arg(fileSize);
        QString::SectionFlag flag = QString::SectionSkipEmpty;
        fileName = QString(bufferRx).section('=', 1, 1).section(',', 0, 0);
        fileSize = QString(bufferRx).section('=', 2, 2, flag).toUInt();

        QString fileInfo = tr("FileName = ").append(fileName).append("FileSize = ").append(QString("%1").arg(fileSize));
        ui->textEdit_Status->append(fileInfo);
        ui->pushButton_Receive->setEnabled(true);
        ui->pushButton_Reject->setEnabled(true);
        isFileInfoData = false;
        fileSizeReceived = 0;
    } else {
        //解析文件本身
        int len = file->write(bufferRx);
        fileSizeReceived += len;
        if (fileSizeReceived % (1024 *  8) == 0) {
            progressBarHanding();
            ui->textEdit_Status->append(QString("Received %1 KB, left %2 KB to receive")
                                            .arg(fileSizeReceived / (1024*8))
                                            .arg((fileSize - fileSizeReceived) / (1024*8)));
        }

        if (fileSizeReceived == fileSize)
        {
            file->close();
            isFileInfoData = true;
            ui->textEdit_Status->append("File transferd succesfully!");
            ui->textEdit_Status->append(QString("File Saved in %1").arg(file->fileName()));
            ui->pushButton_Connect->setEnabled(true);
        }
    }
}

void TcpFileTransferClient::on_pushButton_Reject_clicked()
{
    tcpSocket->write(QString("Refuse to receive file").toUtf8());
    ui->pushButton_Receive->setEnabled(false);
    ui->pushButton_Reject->setEnabled(false);
}

void TcpFileTransferClient::progressBarHanding()
{
    //progress bar
    int progress = (int)(fileSizeReceived * 100 / fileSize);
    ui->progressBar->setValue(progress);
    //qDebug() << "progress = " << progress;

    //transmissionRate
    float transmissionRate = 0.0f;
    int currentTime = QTime::currentTime().msec();

    if (lastTime == 0) {
        lastTime = currentTime;
        lastfileRxBufLen = fileSizeReceived;
    }else{
        float deltaLenMB = (fileSizeReceived - lastfileRxBufLen) / (1024 * 1024);
        transmissionRate = deltaLenMB * 1000 / (currentTime - lastTime);
        lastTime = currentTime;
        lastfileRxBufLen = fileSizeReceived;
        //qDebug() << "deltaLenMB = " << deltaLenMB << "transmissionRate = " << transmissionRate;
    }
    ui->label_Speed->setText(QString::number(transmissionRate, 'f', 2).append(" MB/s"));
}

void TcpFileTransferClient::closeEvent(QCloseEvent* event)
{
    if (tcpSocket != nullptr)
    {
        tcpSocket->disconnectFromHost();
        tcpSocket->close();
        tcpSocket = nullptr;
    }
}


