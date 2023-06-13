#ifndef NETCLIENT_H
#define NETCLIENT_H

#include <QObject>
#include <QSslSocket>
#include <QtNetwork>

class NetClient;

class NetClientDestroyer
{
    private:
        NetClient *instance;
    public:
        ~NetClientDestroyer() { delete instance; }
        void initialize(NetClient *link){ instance = link; }
};

class NetClient : public QObject
{
    Q_OBJECT
private:
    static NetClient *instance;
    static NetClientDestroyer destroyer;
    QSslSocket *TCPSocket;
protected:
    explicit NetClient(QObject *parent = nullptr);
    NetClient(const NetClient &) = delete;
    NetClient &operator = (NetClient &) = delete;
    ~NetClient() { TCPSocket->close(); }
    friend class NetClientDestroyer;
public:
    static NetClient *Connect();
    void SendData(QString data);
    signals:
        void ReceiveAuthData(QString data);
        void ReceiveConnectData(QString data);
        void ReceiveRegCheckData(QString data);
        void ReceiveRegData(QString data);
        void ReceiveNameData(QString data);
        void ReceiveCheckData(QString data);
        void ReceiveStatData(QString data);
        void ReceiveLogoutData(QString data);
        void ReceivePasswordChangeData(QString data);
        void ServerDisconnectedSignal();
private slots:
    void ServerRead();
    void SSLErrors(const QList<QSslError> &errors);
    void ServerDisconnected();
};



#endif // NETCLIENT_H
