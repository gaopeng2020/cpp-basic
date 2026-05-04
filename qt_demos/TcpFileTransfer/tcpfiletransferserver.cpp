#include "tcpfiletransferserver.h"
#include "ui_tcpfiletransferserver.h"

TcpFileTransferServer::TcpFileTransferServer(QWidget* parent) : QMainWindow(parent), ui(new Ui::TcpFileTransferServer)
{
	ui->setupUi(this);
	this->setWindowTitle("TCP File Server");

	tcpServer = nullptr;
	tcpSocket = nullptr;

	//创建TCPServer，并监听客户端请求
	tcpServer = new QTcpServer(this);
	tcpServer->listen(QHostAddress::Any, 8888);

	//在与client成功建立连接之前，文件选择与发送按钮不可使用
	ui->button_Select->setEnabled(false);
	ui->button_Send->setEnabled(false);

	//如果有客户端请求与服务器建立连接，则tcpServer套接字会自动触发newConnection信号
	connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnectionHandling()));
}

TcpFileTransferServer::~TcpFileTransferServer()
{
	delete ui;
}


//与客户端建立连接
void TcpFileTransferServer::newConnectionHandling()
{
	//通过nextPendingConnection获取tcpSocket;
	tcpSocket = tcpServer->nextPendingConnection();

	//获取client的IP与Port
	QString ip = tcpSocket->peerAddress().toString();
	uint16_t port = tcpSocket->peerPort();
	QString clientInfo = QString("Client Info : [%1 : %2]").arg(ip).arg(port);
	ui->textEdit_FileInfo->setText(clientInfo);

	//与客户端成功建立连接之后，文件选择与发送按钮方可使用
	ui->button_Select->setEnabled(true);
	ui->button_Send->setEnabled(false);

	//接收client数据，如果有客户端发送数据给服务器，socket会自动触发readyRead信号，可以在槽函数中通过tcpSocket->readAll()读取数据
	connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
}


void TcpFileTransferServer::receiveMessage()
{
	QString dateRx = QString(tcpSocket->readAll());
	if (dateRx.startsWith("Receice")) {
		ui->textEdit_FileInfo->append("Client agreed to receive this file");
		sendFileData();
	}
	else if (dateRx.startsWith("Refuse")) {
		ui->textEdit_FileInfo->append("Client refused to receive this file");
		file->close();
		ui->button_Select->setEnabled(true);
		ui->button_Send->setEnabled(false);
	}
}

/*
* send File by tcp
*/
void TcpFileTransferServer::sendFileData()
{
	uint64_t len = 0;
	do {
		len = 0;
		//定义一个缓冲区，每次发送8k大小的数据
		char buffer[32 * 1024] = { 0 };
		//读取文件数据并缓存到buffer
		len = file->read(buffer, sizeof(buffer));
		//将buffer数据通过tcpSocket发送出去,读多少发多少
		len = tcpSocket->write(buffer, len);
		//tcpSocket->waitForBytesWritten(2000);
		sendedFileSize += len;

		//512kb的数据量数据显示一次
		if (sendedFileSize % (512 * 1024) == 0)
		{
			QString sendingInfo = QString("File Size ： %1 KB，Already Sended : %2 KB").arg(fileSize / 1024).arg(sendedFileSize / 1024);
			ui->textEdit_FileInfo->append(sendingInfo);
		}

	} while (len > 0);

	if (sendedFileSize == fileSize)
	{
		file->close();
		ui->button_Select->setEnabled(true);
		ui->button_Send->setEnabled(false);
		ui->textEdit_FileInfo->append("File Send succesfully");
	}
}

//选择文件
void TcpFileTransferServer::on_button_Select_clicked()
{
	//通过对话框获取文件路径
	const QString filter = "Images (*.png *.xpm *.jpg);;Text (*.txt *.log *.xml *.arxml *.json);;All(*.*);;Vedio (*.avi *.mp4 *.flv *.ts)";
	QString* selectedFilter = new QString("Vedio (*.avi *.mp4 *.flv *.ts)");
	QString filepath = QFileDialog::getOpenFileName(this, "Please Select a file", "../", filter, selectedFilter);
	if (filepath.isEmpty())
	{
		ui->textEdit_FileInfo->append("Slected File is invalid !");
		return;
	}
	//QFileDialog::getSaveFileName

	//创建file，并设置文件路径
	file = new QFile(this);
	file->setFileName(filepath);

	//将filepath显示在textEdit框中
	ui->textEdit_FileInfo->append(filepath);

	//获取文件信息
	QFileInfo fileInfo(*file);
	fileName = fileInfo.fileName();
	fileSize = fileInfo.size();
	sendedFileSize = 0;

	bool isOpend = file->open(QIODevice::ReadOnly);
	if (!isOpend)
	{
		ui->textEdit_FileInfo->append("File opened faild");
		return;
	}

	//文件打开之后，在文件发送完成之前，文件选择按钮不能在选择了
	ui->button_Select->setEnabled(false);
	ui->button_Send->setEnabled(true);
}

//发送文件信息，文件传输需要客户端确认是否接收
void TcpFileTransferServer::on_button_Send_clicked()
{
	if (tcpSocket == nullptr || !tcpSocket->isValid())
	{
		ui->textEdit_FileInfo->append("Loss connection to client");
		return;
	}
	//先发送文件信息，filename,filesize
	QString  fileInfo = QString("FileName=%1 , FileSize = %2").arg(fileName).arg(fileSize);
	int64_t len = tcpSocket->write(fileInfo.toUtf8());

	//发送文件
	if (len > 0)
	{
		ui->textEdit_FileInfo->append("File basic information send succesfully !");
	}
}


void TcpFileTransferServer::closeEvent(QCloseEvent* event)
{
	if (tcpSocket != nullptr)
	{
		tcpSocket->disconnectFromHost();
		tcpSocket->close();
		tcpSocket = nullptr;
		ui->textEdit_FileInfo->append("disconnect from client succesfully !");
	}
	if (tcpServer != nullptr)
	{
		tcpServer->close();
		tcpServer = nullptr;
		ui->textEdit_FileInfo->append("tcpServer socket closed succesfully !");
	}
}

