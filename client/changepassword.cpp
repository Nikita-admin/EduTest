#include "changepassword.h"
#include "ui_changepassword.h"
#include <QRegularExpressionValidator>

ChangePassword::ChangePassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangePassword)
{
    ui->setupUi(this);
    ui->ChangePasswordMessage->hide();
    QRegularExpression password("[A-Za-z0-9!#$%&*()[\\]<>:.;=_-]{1,60}$");
    ui->ChangePasswordMessage->setStyleSheet("color: rgb(255,0,0)");
    ui->OldPasswordLine->setValidator(new QRegularExpressionValidator(password, this));
    ui->NewPasswordLine->setValidator(new QRegularExpressionValidator(password, this));
    ui->NewPasswordRepeatLine->setValidator(new QRegularExpressionValidator(password, this));
}

ChangePassword::~ChangePassword()
{
    delete ui;
}

void ChangePassword::on_ChangePasswordButton_clicked()
{
    if (ui->OldPasswordLine->text() != "" and ui->NewPasswordLine->text() != "")
    {
        if (ui->NewPasswordLine->text() == ui->NewPasswordRepeatLine->text())
        {
            NetClient::Connect()->SendData("changepass+" + PasswordHash(ui->OldPasswordLine->text()) + "+" + PasswordHash(ui->NewPasswordLine->text()));
        }
        else
        {
            ui->ChangePasswordMessage->show();
            ui->ChangePasswordMessage->setText("Новые пароли не совпадают");
        }
    }
    else
    {
        ui->ChangePasswordMessage->show();
        ui->ChangePasswordMessage->setText("Пароль не может быть пустым");
    }
}

void ChangePassword::on_CancelButton_clicked()
{
    this->close();
}

QString ChangePassword::PasswordHash(QString password)
{
    password = "dfgtm48d" + password;
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha3_512);
    return hash.toHex();
}


void ChangePassword::on_NewPasswordLine_inputRejected()
{
    ui->ChangePasswordMessage->show();
    ui->ChangePasswordMessage->setText("Не используйте запрещённые символы");
    QApplication::beep();
}


void ChangePassword::on_NewPasswordRepeatLine_inputRejected()
{
    ui->ChangePasswordMessage->show();
    ui->ChangePasswordMessage->setText("Не используйте запрещённые символы");
    QApplication::beep();
}

