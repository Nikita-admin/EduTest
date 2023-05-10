#ifndef NETSERVER_H
#define NETSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QSslSocket>
#include <QtNetwork>

class NetServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit NetServer(QObject *parent = nullptr);
    ~NetServer();
protected:
    void incomingConnection(qintptr socketDescriptor) override;
private slots:
    void ClientDisconnected();
    void ServerDataRead();
    void SSLErrors(const QList<QSslError> &errors);
private:
    QTcpServer *TCPServer;
    QMap<QSslSocket*, long long> Clients;
    bool server_is_up = false;
    QSslCertificate certificate;
    QSslKey privateKey;
};

#endif // NETSERVER_H
