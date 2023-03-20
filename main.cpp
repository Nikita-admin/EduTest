#include <QCoreApplication>
#include "netserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    NetServer server;

    return a.exec();
}
