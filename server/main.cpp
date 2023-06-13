#include <QCoreApplication>
#include "netserver.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (!QSslSocket::supportsSsl())
    {
        qDebug() << "SSL is not supported";
        std::exit(1);
    }

    NetServer server;

    return a.exec();
}
