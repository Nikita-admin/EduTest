#ifndef NETSERVER_H
#define NETSERVER_H
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtNetwork>
#include <QByteArray>
#include <QDebug>
#include <QMap>

class NetServer : public QObject
{
    Q_OBJECT
public:
    explicit NetServer(QObject *parent = nullptr);
    ~NetServer();
public slots:
    void NewConnection();
    void ClientDisconnected();
    void ServerDataRead();
private:
    QTcpServer * TCPServer;
    QMap<int, QTcpSocket*> TCPSockets;
    int server_status;
};

#endif // NETSERVER_H







