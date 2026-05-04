#ifndef TCPFILETRANSFERCLIENT_H
#define TCPFILETRANSFERCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QHostAddress>
#include <QFile>
#include <QFileDialog>
#include <QTime>
#include <qdebug.h>

namespace Ui {
	class TcpFileTransferClient;
}

class TcpFileTransferClient : public QWidget
{
	Q_OBJECT

public:
	explicit TcpFileTransferClient(QWidget* parent = nullptr);
	~TcpFileTransferClient();

private slots:
	void on_pushButton_Connect_clicked();
	void on_pushButton_Receive_clicked();
	void on_pushButton_Reject_clicked();
	void receiverFile();

private:
	Ui::TcpFileTransferClient* ui;
	QTcpSocket* tcpSocket;

	QFile* file;
	QString fileName;
	int64_t fileSize;
	int64_t fileSizeReceived;

	int64_t lastTime = 0;
	int64_t lastfileRxBufLen = 0;
	bool isFileInfoData = true;

	void progressBarHanding();

protected:
	void closeEvent(QCloseEvent* event) override;
};

#endif // TCPFILETRANSFERCLIENT_H
