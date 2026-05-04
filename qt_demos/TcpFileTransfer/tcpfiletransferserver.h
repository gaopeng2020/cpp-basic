#ifndef TCPFILETRANSFERSERVER_H
#define TCPFILETRANSFERSERVER_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QTimer>
#include <QMessageBox>


QT_BEGIN_NAMESPACE
namespace Ui { class TcpFileTransferServer; }
QT_END_NAMESPACE

class TcpFileTransferServer : public QMainWindow
{
	Q_OBJECT

public:
	TcpFileTransferServer(QWidget* parent = nullptr);
	~TcpFileTransferServer();

private slots:
	void on_button_Select_clicked();
	void on_button_Send_clicked();
    void newConnectionHandling();
    void receiveMessage();

private:
	Ui::TcpFileTransferServer* ui;

	QTcpServer* tcpServer;
	QTcpSocket* tcpSocket;

	QFile* file;
	QString fileName;
	int64_t fileSize;
	int64_t sendedFileSize;

private:
	void sendFileData();

protected:
	void closeEvent(QCloseEvent* event) override;
};
#endif // TCPFILETRANSFERSERVER_H
