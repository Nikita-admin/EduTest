#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <QObject>

QString authentication(QString login, QString pass)
{
    if(login == "admin")
    {
        if(pass == "1234")
        {
            return "Welcome, Admin!\r\n";
        }
    }
    return "Wrong credentials\r\n";
}

QString registration(QString login, QString pass, QString name)
{
    return "Your credentials:\r\nLogin:" + login + "\r\nPass:" + pass + "\r\nName:" + name + "\r\n";
}

void check_task(){}
void query(){}

QString parsing(QString data)
{
    QString clear_data = data.left(data.lastIndexOf("\xd"));
    QList<QString> parametrs = clear_data.split("+");

    if(parametrs[0] == "auth" and parametrs.count() == 3)
    {
       return authentication(parametrs[1], parametrs[2]);
    }
    else if(parametrs[0] == "reg" and parametrs.count() == 4)
    {
       return registration(parametrs[1], parametrs[2], parametrs[3]);
    }
    return "Error data parsing\r\n";
}

#endif // FUNCTIONS_H
