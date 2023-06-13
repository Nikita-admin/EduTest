#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "database.h"

const int MAX_DATA_SIZE = 1024;

QString authentication(const QString &login, const QString &pass, const QString &connection_id)
{
    QList<QString> query_list = {"SELECT login FROM Users WHERE login = :login and password = :password;",
                                 ":login", login,
                                 ":password", pass
                                };
    QString response = DataBase::Connect()->query_execute(query_list);

    if (response == "")
    {
        return "auth+wrong";
    }
    else
    {
        query_list = {"UPDATE Users SET connection_id = :connection_id WHERE login = :login;",
                      ":login", login,
                      ":connection_id", connection_id
                     };
        DataBase::Connect()->query_execute(query_list);

        return "auth+ok";
    }
}

QString check_login(const QString &login)
{
    QList<QString> query_list = {"SELECT login FROM Users WHERE login = :login;",
                                 ":login", login
                                };
    QString response = DataBase::Connect()->query_execute(query_list);

    if (response == "")
    {
        return "regcheck+ok";
    }
    else
    {
        return "regcheck+already";
    }
}

QString get_name(const QString &connection_id)
{
    QList<QString> query_list = {"SELECT name FROM Users WHERE connection_id = :connection_id;",
                                 ":connection_id", connection_id
                                };
    QString response = DataBase::Connect()->query_execute(query_list);

    if (response != "")
    {
        return "getname+ok+" + response;
    }
    else
    {
        return "getname+autherr";
    }
}

QString registration(const QString &login, const QString &pass, const QString &name, const QString &last_name, const QString &patronymic_name, const QString &role)
{
    QList<QString> query_list = {"SELECT login FROM Users WHERE login = :login;",
                                 ":login", login
                                };
    QString response = DataBase::Connect()->query_execute(query_list);

    if (response != "")
    {
        return "reg+already";
    }

    if (role == "student")
    {
        query_list = {"INSERT INTO Users "
                      "(login, password, name, last_name, patronymic_name, role, connection_id, task1_stat, task2_stat, task3_stat, task4_stat, task5_stat)"
                      " VALUES (:login, :pass, :name, :last_name, :patronymic_name, 'student', NULL, 0, 0, 0, 0, 0);",
                      ":login", login,
                      ":pass", pass,
                      ":name", name,
                      ":last_name", last_name,
                      ":patronymic_name", patronymic_name,
                     };
        DataBase::Connect()->query_execute(query_list);
    }
    else if (role == "teacher")
    {
        query_list = {"INSERT INTO Users "
                      "(login, password, name, last_name, patronymic_name, role, connection_id, task1_stat, task2_stat, task3_stat, task4_stat, task5_stat) "
                      "VALUES (:login, :pass, :name, :last_name, :patronymic_name, 'teacher', NULL, 0, 0, 0, 0, 0);",
                      ":login", login,
                      ":pass", pass,
                      ":name", name,
                      ":last_name", last_name,
                      ":patronymic_name", patronymic_name,
                     };
        DataBase::Connect()->query_execute(query_list);
    }
    query_list = {"SELECT login FROM Users WHERE login = :login;",
                  ":login", login
                 };
    response = DataBase::Connect()->query_execute(query_list);

    if (response == "")
    {
        return "reg+error";
    }
    return "reg+ok";
}

bool check_answer(const QString &ans, const int &num)
{
    return true;
}

QString check_task(const QString &connection_id, const QString &task, const QString &ans)
{
    QList<QString> query_list = {"SELECT login FROM Users WHERE connection_id = :connection_id;",
                                 ":connection_id", connection_id
                                };
    QString response = DataBase::Connect()->query_execute(query_list);

    if (response != "")
    {
        if (task == "1")
        {
            if (check_answer(ans, 1))
            {
                query_list = {"UPDATE Users SET task1_stat = task1_stat + 1 WHERE connection_id = :connection_id;",
                              ":connection_id", connection_id
                             };
                DataBase::Connect()->query_execute(query_list);
            }
            else
            {
                query_list = {"UPDATE Users SET task1_stat = task1_stat - 1 WHERE connection_id = :connection_id;",
                              ":connection_id", connection_id
                             };
                DataBase::Connect()->query_execute(query_list);
            }
        }
        if (task == "2")
        {
            if (check_answer(ans, 2))
            {
                query_list = {"UPDATE Users SET task2_stat = task2_stat + 1 WHERE connection_id = :connection_id;",
                              ":connection_id", connection_id
                             };
                DataBase::Connect()->query_execute(query_list);
            }
            else
            {
                query_list = {"UPDATE Users SET task2_stat = task2_stat - 1 WHERE connection_id = :connection_id;",
                              ":connection_id", connection_id
                             };
                DataBase::Connect()->query_execute(query_list);
            }
        }
        if (task == "3")
        {
            if (check_answer(ans, 3))
            {
                query_list = {"UPDATE Users SET task3_stat = task3_stat + 1 WHERE connection_id = :connection_id;",
                              ":connection_id", connection_id
                             };
                DataBase::Connect()->query_execute(query_list);
            }
            else
            {
                query_list = {"UPDATE Users SET task3_stat = task3_stat - 1 WHERE connection_id = :connection_id;",
                              ":connection_id", connection_id
                             };
                DataBase::Connect()->query_execute(query_list);
            }
        }
        if (task == "4")
        {
            if (check_answer(ans, 4))
            {
                query_list = {"UPDATE Users SET task4_stat = task4_stat + 1 WHERE connection_id = :connection_id;",
                              ":connection_id", connection_id
                             };
                DataBase::Connect()->query_execute(query_list);
            }
            else
            {
                query_list = {"UPDATE Users SET task4_stat = task4_stat - 1 WHERE connection_id = :connection_id;",
                              ":connection_id", connection_id
                             };
                DataBase::Connect()->query_execute(query_list);
            }
        }
        if (task == "5")
        {
            if (check_answer(ans, 5))
            {
                query_list = {"UPDATE Users SET task5_stat = task5_stat + 1 WHERE connection_id = :connection_id;",
                              ":connection_id", connection_id
                             };
                DataBase::Connect()->query_execute(query_list);
            }
            else
            {
                query_list = {"UPDATE Users SET task5_stat = task5_stat - 1 WHERE connection_id = :connection_id;",
                              ":connection_id", connection_id
                             };
                DataBase::Connect()->query_execute(query_list);
            }
        }
        return "check+ok";
    }
    return "chek+autherr";
}

QString get_stat(const QString &connection_id)
{
    QList<QString> query_list = {"SELECT role FROM Users WHERE connection_id = :connection_id;",
                                 ":connection_id", connection_id
                                };
    QString role = DataBase::Connect()->query_execute(query_list);

    QString response = "";

    if (role == "student;")
    {
        QList<QString> query_list = {"SELECT name, last_name, patronymic_name, task1_stat, task2_stat, task3_stat, task4_stat, task5_stat FROM Users "
                                     "WHERE connection_id = :connection_id;",
                                     ":connection_id", connection_id
                                    };
        response = DataBase::Connect()->query_execute(query_list);
        return "stat+student+" + response;
    }
    else if (role == "teacher;")
    {
        QList<QString> query_list = {"SELECT name, last_name, patronymic_name, task1_stat, task2_stat, task3_stat, task4_stat, task5_stat FROM Users "
                                     "WHERE role = 'student';"
                                    };
        response = DataBase::Connect()->query_execute(query_list);
        return "stat+teacher+" + response;
    }
    return "stat+autherr";
}

QString changepass(const QString &oldpass, const QString &newpass, const QString &connection_id)
{
    QList<QString> query_list = {"SELECT * FROM Users WHERE connection_id = :connection_id AND password = :password;",
                                ":password", oldpass,
                                ":connection_id", connection_id
                                };
    QString response = DataBase::Connect()->query_execute(query_list);

    if (response != "")
    {
        query_list = {"UPDATE Users SET password = :password WHERE connection_id = :connection_id;",
                     ":password", newpass,
                     ":connection_id", connection_id
                     };
        DataBase::Connect()->query_execute(query_list);
        return "changepass+ok";
    }
    return "changepass+err";
}

void close_session(const QString &connection_id)
{
    QList<QString> query_list = {"UPDATE Users SET connection_id = NULL WHERE connection_id = :connection_id;",
                                 ":connection_id", connection_id
                                };
    DataBase::Connect()->query_execute(query_list);
}

QString parsing(const QString &data, const QString &connection_id)
{
    if (data.size() > MAX_DATA_SIZE) {
        return "parsing+errsize";
    }
    else
    {
        QList<QString> parametrs = data.split("+");

        if(parametrs[0] == "auth" and parametrs.count() == 3)
        {
           return authentication(parametrs[1], parametrs[2], connection_id);
        }
        else if(parametrs[0] == "regcheck" and parametrs.count() == 2)
        {
           return check_login(parametrs[1]);
        }
        else if(parametrs[0] == "reg" and parametrs.count() == 7)
        {
           return registration(parametrs[1], parametrs[2], parametrs[3], parametrs[4], parametrs[5], parametrs[6]);
        }
        else if(parametrs[0] == "check" and parametrs.count() == 3)
        {
           return check_task(connection_id, parametrs[1], parametrs[2]);
        }
        else if(parametrs[0] == "stat" and parametrs.count() == 1)
        {
           return get_stat(connection_id);
        }
        else if(parametrs[0] == "getname" and parametrs.count() == 1)
        {
           return get_name(connection_id);
        }
        else if(parametrs[0] == "changepass" and parametrs.count() == 3)
        {
           return changepass(parametrs[1], parametrs[2], connection_id);
        }
        else if(parametrs[0] == "logout" and parametrs.count() == 1)
        {
           close_session(connection_id);
           return "logout+ok";
        }
        else if(parametrs[0] == "hi")
        {
           return "ok";
        }
    }
    return "parsing+err";
}

#endif // FUNCTIONS_H
