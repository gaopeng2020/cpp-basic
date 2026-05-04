#include "tcpserver.h"
#include "tcpclient.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TCPServer w;
    w.show();

    TCPClient client;
    client.show();

    return a.exec();
}
