#ifndef AUTHFORM_H
#define AUTHFORM_H

#include <QWidget>
#include "netclient.h"

namespace Ui {
class AuthForm;
}

class AuthForm : public QWidget
{
    Q_OBJECT

public:
    explicit AuthForm(QWidget *parent = nullptr);
    ~AuthForm();
signals:
    void auth_ok();

private slots:
    void on_GoRegisterButton_clicked();

    void on_NextStepRegister_clicked();

    void on_BackStepRegister_clicked();

    void on_NewLoginLine_inputRejected();

    void on_RegisterButton_clicked();

    void on_ReceiveRegCheckData(QString data);

    void on_ReceiveRegData(QString data);

    void on_ReceiveAuthData(QString data);

    void on_ReceiveConnectData(QString data);

    void on_LoginButton_clicked();

    void on_LoginLine_inputRejected();

    void on_NewPasswordLine_inputRejected();

    void on_NewPasswordRepeatLine_inputRejected();

    void on_PasswordLine_inputRejected();

private:
    Ui::AuthForm *ui;
    QString PasswordHash(QString password);
};

#endif // AUTHFORM_H
