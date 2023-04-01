#ifndef NETSERVER_H
#define NETSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtNetwork>
#include <QByteArray>
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
    QTcpServer *TCPServer;
    QMap<QTcpSocket*, long long> Clients;
    int server_status;
};

#endif // NETSERVER_H
