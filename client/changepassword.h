#ifndef CHANGEPASSWORD_H
#define CHANGEPASSWORD_H

#include "netclient.h"
#include <QDialog>

namespace Ui {
class ChangePassword;
}

class ChangePassword : public QDialog
{
    Q_OBJECT

public:
    explicit ChangePassword(QWidget *parent = nullptr);
    ~ChangePassword();

private slots:
    void on_ChangePasswordButton_clicked();

    void on_CancelButton_clicked();

    void on_NewPasswordLine_inputRejected();

    void on_NewPasswordRepeatLine_inputRejected();

private:
    Ui::ChangePassword *ui;

    QString PasswordHash(QString password);
};

#endif // CHANGEPASSWORD_H
