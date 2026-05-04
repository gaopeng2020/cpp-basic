#include "tcpfiletransferserver.h"
#include "tcpfiletransferclient.h"

#include <QApplication>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	TcpFileTransferServer server;
	server.show();

	TcpFileTransferClient client;
	client.show();

	return a.exec();
}
