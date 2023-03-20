#include "netserver.h"
#include "functions.h"

NetServer::~NetServer()
{
    TCPServer->close();
    server_status = 0;
}
NetServer::NetServer(QObject *parent) : QObject(parent)
{
    TCPServer = new QTcpServer(this);
    connect(TCPServer, &QTcpServer::newConnection,
            this, &NetServer::NewConnection);

    if(!TCPServer->listen(QHostAddress::Any, 33333))
    {
        qDebug() << "Error. Server is not started";
    } else {
        server_status = 1;
        qDebug() << "Server is started";
    }
}

void NetServer::NewConnection()
{
    if(server_status == 1)
    {
        qDebug() << "New client connected";
        QTcpSocket *Current_TCPSocket;
        Current_TCPSocket = TCPServer->nextPendingConnection();
        Current_TCPSocket->write("Hello, World!!! I am EduTest Server!\r\n");
        connect(Current_TCPSocket, &QTcpSocket::readyRead,
                this,&NetServer::ServerDataRead);
        connect(Current_TCPSocket,&QTcpSocket::disconnected,
                this,&NetServer::ClientDisconnected);
        long long connection_id = Current_TCPSocket->socketDescriptor();
        Current_TCPSocket->write("Your connection ID: ");
        Current_TCPSocket->write(QString::number(connection_id).toUtf8());
        Current_TCPSocket->write("\r\n");
    }
}

void NetServer::ServerDataRead()
{
    QTcpSocket *Current_TCPSocket = (QTcpSocket*)sender();
    QByteArray data_output;
    QString data_input;
    while(Current_TCPSocket->bytesAvailable()>0)
    {
        data_input += Current_TCPSocket->readAll();
    }
    data_output = parsing(data_input).toUtf8();
    Current_TCPSocket->write(data_output);
}

void NetServer::ClientDisconnected()
{
    QTcpSocket *Current_TCPSocket = (QTcpSocket*)sender();
    qDebug() << "Client disconnected";
    Current_TCPSocket->close();
}
