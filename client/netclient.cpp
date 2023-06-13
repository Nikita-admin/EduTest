#include "netclient.h"
#include <QSslConfiguration>
#include <QSslCertificate>

NetClient::NetClient(QObject *parent) : QObject(parent)
{
    TCPSocket = new QSslSocket(this);
    QSslConfiguration sslConfig = TCPSocket->sslConfiguration();
    TCPSocket->setSslConfiguration(sslConfig);
    connect(TCPSocket, QOverload<const QList<QSslError> &>::of(&QSslSocket::sslErrors), this, &NetClient::SSLErrors);
    TCPSocket->connectToHostEncrypted("127.0.0.1", 33333);
    connect(TCPSocket, &QSslSocket::disconnected,
            this, &NetClient::ServerDisconnected);
    connect(TCPSocket, &QSslSocket::readyRead,
            this, &NetClient::ServerRead);
    TCPSocket->waitForConnected(500);
    if (TCPSocket->state() == QTcpSocket::ConnectedState)
    {
        qDebug() << "Connected to server";
    }
}

NetClient* NetClient::Connect()
{
    if (!instance)
    {
        instance = new NetClient();
        destroyer.initialize(instance);
    }
    return instance;
}

void NetClient::SendData(QString data)
{
    TCPSocket->write(data.toUtf8());
    qDebug() << "Send:" << data;
}

void NetClient::ServerRead()
{
    QString data_input;
    data_input = TCPSocket->readAll();
    qDebug() << "Get:" << data_input;
    QString action = data_input.left(data_input.indexOf("+"));
    if (action == "auth")
    {
        emit ReceiveAuthData(data_input);
    }
    else if (action == "ok")
    {
        emit ReceiveConnectData(data_input);
    }
    else if (action == "regcheck")
    {
        emit ReceiveRegCheckData(data_input);
    }
    else if (action == "reg")
    {
        emit ReceiveRegData(data_input);
    }
    else if (action == "getname")
    {
        emit ReceiveNameData(data_input);
    }
    else if (action == "check")
    {
        emit ReceiveCheckData(data_input);
    }
    else if (action == "stat")
    {
        emit ReceiveStatData(data_input);
    }
    else if (action == "changepass")
    {
        emit ReceivePasswordChangeData(data_input);
    }
    else if (action == "logout")
    {
        emit ReceiveLogoutData(data_input);
    }
}

void NetClient::SSLErrors(const QList<QSslError> &errors)
{
    bool ignoreSslErrors = false;
    QList<QSslError> errorsToIgnore;

    for (const auto &error : errors) {
        qDebug() << "SSL error:" << error.errorString();
        if (error.error() == QSslError::SelfSignedCertificate || error.error() == QSslError::CertificateUntrusted) {
            ignoreSslErrors = true;
            errorsToIgnore << error;
        }
    }

    if (ignoreSslErrors) {
        TCPSocket->ignoreSslErrors(errorsToIgnore);
    } else {
        TCPSocket->disconnectFromHost();
    }
}

void NetClient::ServerDisconnected()
{
    qDebug() << "Disconnected from server";
    emit ServerDisconnectedSignal();
}

NetClient *NetClient::instance;
NetClientDestroyer NetClient::destroyer;
