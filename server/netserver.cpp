#include "netserver.h"
#include "functions.h"

NetServer::~NetServer()
{
    close();
    server_is_up = false;
}

NetServer::NetServer(QObject *parent) : QTcpServer(parent)
{
    QFile certFile("../server/server.crt");
    if (!certFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error opening certificate file";
        std::exit(1);
    }
    certificate = QSslCertificate(&certFile, QSsl::Pem);
    certFile.close();
    if (certificate.isNull())
    {
        qDebug() << "Error loading certificate";
        std::exit(1);
    }

    QFile keyFile("../server/server.key");
    if (!keyFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error opening key file";
        std::exit(1);
    }
    privateKey = QSslKey(&keyFile, QSsl::Rsa, QSsl::Pem);
    keyFile.close();
    if (privateKey.isNull())
    {
        qDebug() << "Error loading key";
        std::exit(1);
    }

    if(!listen(QHostAddress::Any, 33333))
    {
        qDebug() << "Error. Server is not started";
        std::exit(1);
    }
    else
    {
        server_is_up = true;
        qDebug() << "Server is started";
    }
}

void NetServer::incomingConnection(qintptr socketDescriptor)
{
    if(server_is_up)
    {
        qDebug() << "New client connected";
        QSslSocket *Current_TCPSocket = new QSslSocket(this);

        if (Current_TCPSocket->setSocketDescriptor(socketDescriptor))
        {
            connect(Current_TCPSocket, &QSslSocket::sslErrors,
                    this, &NetServer::SSLErrors);

            QSslConfiguration sslConfig = Current_TCPSocket->sslConfiguration();
            sslConfig.setLocalCertificate(certificate);
            sslConfig.setPrivateKey(privateKey);
            Current_TCPSocket->setSslConfiguration(sslConfig);
            Current_TCPSocket->startServerEncryption();

            long long connection_id = Current_TCPSocket->socketDescriptor();
            Clients.insert(Current_TCPSocket,connection_id);
            connect(Current_TCPSocket, &QSslSocket::readyRead,
                    this, &NetServer::ServerDataRead);
            connect(Current_TCPSocket, &QSslSocket::disconnected,
                    this, &NetServer::ClientDisconnected);
        }
        else
        {
            delete Current_TCPSocket;
        }
    }
}

void NetServer::ServerDataRead()
{
    QSslSocket *Current_TCPSocket = (QSslSocket*)sender();
    QByteArray data_output;
    QString data_input;
    data_input += Current_TCPSocket->readAll();
    QString connection_id = QString::number(Current_TCPSocket->socketDescriptor());
    data_output = parsing(data_input, connection_id).toUtf8();
    Current_TCPSocket->write(data_output);
}

void NetServer::ClientDisconnected()
{
    QSslSocket *Current_TCPSocket = (QSslSocket*)sender();
    qDebug() << "Client disconnected";
    QString connection_id = QString::number(Clients.value(Current_TCPSocket));
    close_session(connection_id);
    Clients.remove(Current_TCPSocket);
    Current_TCPSocket->close();
}

void NetServer::SSLErrors(const QList<QSslError> &errors)
{
    for (const QSslError &error : errors)
    {
        qDebug() << "SSL error:" << error.errorString();
    }
}
