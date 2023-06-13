#include "authform.h"
#include "ui_authform.h"
#include <QRegularExpressionValidator>
#include <QMessageBox>

AuthForm::AuthForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AuthForm)
{
    ui->setupUi(this);
    QRegularExpression login("[A-Za-z0-9-._]{1,40}$");
    QRegularExpression password("[A-Za-z0-9!#$%&*()[\\]<>:.;=_-]{1,60}$");
    QRegularExpression name("[A-Za-zА-Яа-я]{1,30}$");
    ui->NewLoginLine->setValidator(new QRegularExpressionValidator(login, this));
    ui->LoginLine->setValidator(new QRegularExpressionValidator(login, this));
    ui->NewPasswordLine->setValidator(new QRegularExpressionValidator(password, this));
    ui->NewPasswordRepeatLine->setValidator(new QRegularExpressionValidator(password, this));
    ui->PasswordLine->setValidator(new QRegularExpressionValidator(password, this));
    ui->NameLine->setValidator(new QRegularExpressionValidator(name, this));
    ui->LastNameLine->setValidator(new QRegularExpressionValidator(name, this));
    ui->PatronymicNameLine->setValidator(new QRegularExpressionValidator(name, this));
    ui->LoginMessage->setStyleSheet("color: rgb(255,0,0)");
    ui->RegisterMessage->setStyleSheet("color: rgb(255,0,0)");
    ui->FirstStepRegisterBox->hide();
    ui->SecondStepRegisterBox->hide();
    ui->LoginMessage->hide();
    ui->LoginBox->show();
    ui->LoginLine->setDisabled(true);
    ui->PasswordLine->setDisabled(true);
    ui->LoginButton->setDisabled(true);
    ui->GoRegisterButton->setDisabled(true);
    connect(NetClient::Connect(),&NetClient::ReceiveConnectData,this,&AuthForm::on_ReceiveConnectData);
    NetClient::Connect()->SendData("hi");
}

AuthForm::~AuthForm()
{
    delete ui;
}

void AuthForm::on_ReceiveConnectData(QString data)
{
    if (data == "ok")
    {
        ui->LoginLine->setDisabled(false);
        ui->PasswordLine->setDisabled(false);
        ui->LoginButton->setDisabled(false);
        ui->GoRegisterButton->setDisabled(false);
        ui->NoConnectInfo->hide();
    }
}

void AuthForm::on_GoRegisterButton_clicked()
{
    ui->LoginBox->hide();
    ui->RegisterMessage->hide();
    ui->FirstStepRegisterBox->show();
}


void AuthForm::on_NextStepRegister_clicked()
{
    if (ui->NewLoginLine->text() != "")
    {
        if (ui->NewPasswordLine->text() != ui->NewPasswordRepeatLine->text())
        {
            ui->RegisterMessage->setText("Пароли не совпадают");
            ui->RegisterMessage->show();
            QApplication::beep();
        }
        else
        {
            if (ui->NewPasswordLine->text() == "")
            {
                ui->NewPasswordLine->setFocus();
                ui->RegisterMessage->setText("Пароль не может быть пустым");
                ui->RegisterMessage->show();
                QApplication::beep();
            }
            else
            {
                connect(NetClient::Connect(),&NetClient::ReceiveRegCheckData,this,&AuthForm::on_ReceiveRegCheckData);
                NetClient::Connect()->SendData("regcheck+" + ui->NewLoginLine->text());
            }
        }
    }
    else
    {
        ui->NewLoginLine->setFocus();
        ui->RegisterMessage->setText("Логин не может быть пустым");
        ui->RegisterMessage->show();
        QApplication::beep();
    }
}

void AuthForm::on_ReceiveRegCheckData(QString data)
{
    if (data == "regcheck+already")
    {
        ui->NewLoginLine->setFocus();
        ui->RegisterMessage->setText("Такой логин уже занят");
        ui->RegisterMessage->show();
        QApplication::beep();
    }
    else
    {
        ui->RegisterMessage->hide();
        ui->FirstStepRegisterBox->hide();
        ui->SecondStepRegisterBox->show();
    }
}

void AuthForm::on_BackStepRegister_clicked()
{
    ui->SecondStepRegisterBox->hide();
    ui->FirstStepRegisterBox->show();
}


void AuthForm::on_NewLoginLine_inputRejected()
{
    ui->RegisterMessage->show();
    ui->RegisterMessage->setText("Логин должен состоять из латиницы и цифр");
    QApplication::beep();
}


void AuthForm::on_RegisterButton_clicked()
{
    if (ui->NameLine->text() == "")
    {
        ui->NameLine->setFocus();
        QApplication::beep();
    }
    else if (ui->LastNameLine->text() == "")
    {
        ui->LastNameLine->setFocus();
        QApplication::beep();
    }
    else if (ui->PatronymicNameLine->text() == "")
    {
        ui->PatronymicNameLine->setFocus();
        QApplication::beep();
    }
    else
    {
        QString role;
        if (ui->TeacherButton->isChecked())
        {
            role = "teacher";
        }
        else
        {
            role = "student";
        }
        connect(NetClient::Connect(),&NetClient::ReceiveRegData,this,&AuthForm::on_ReceiveRegData);
        NetClient::Connect()->SendData("reg+" + ui->NewLoginLine->text() + "+" + PasswordHash(ui->NewPasswordLine->text())
                                       + "+" + ui->NameLine->text() + "+" + ui->LastNameLine->text() + "+" + ui->PatronymicNameLine->text() + "+" + role);
    }
}

void AuthForm::on_ReceiveRegData(QString data)
{
    if (data == "reg+ok")
    {
        ui->SecondStepRegisterBox->hide();
        ui->LoginBox->show();
    }
    else
    {
        QMessageBox::critical(this, "Ошибка", "Произошла неизвестная ошибка при рестрации.");
    }
}


void AuthForm::on_LoginButton_clicked()
{
    if (ui->LoginLine->text() == "")
    {
        ui->LoginLine->setFocus();
        ui->LoginMessage->show();
        ui->LoginMessage->setText("Логин не может быть пустым");
        QApplication::beep();
    }
    else if (ui->PasswordLine->text() == "")
    {
        ui->PasswordLine->setFocus();
        ui->LoginMessage->show();
        ui->LoginMessage->setText("Пароль не может быть пустым");
        QApplication::beep();
    }
    else
    {
        ui->LoginMessage->hide();
        connect(NetClient::Connect(),&NetClient::ReceiveAuthData,this,&AuthForm::on_ReceiveAuthData);
        NetClient::Connect()->SendData("auth+" + ui->LoginLine->text() + "+" + PasswordHash(ui->PasswordLine->text()));
    }
}

void AuthForm::on_ReceiveAuthData(QString data)
{
    if (data == "auth+ok")
    {
        ui->LoginMessage->hide();
        emit auth_ok();
    }
    else
    {
        ui->LoginMessage->show();
        ui->LoginMessage->setText("Неверный логин или пароль");
        QApplication::beep();
    }
}

void AuthForm::on_LoginLine_inputRejected()
{
    ui->LoginMessage->show();
    ui->LoginMessage->setText("Логин должен состоять из латиницы и цифр");
    QApplication::beep();
}

QString AuthForm::PasswordHash(QString password)
{
    password = "dfgtm48d" + password;
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha3_512);
    return hash.toHex();
}


void AuthForm::on_NewPasswordLine_inputRejected()
{
    ui->RegisterMessage->show();
    ui->RegisterMessage->setText("Не используйте запрещённые символы");
    QApplication::beep();
}


void AuthForm::on_NewPasswordRepeatLine_inputRejected()
{
    ui->RegisterMessage->show();
    ui->RegisterMessage->setText("Не используйте запрещённые символы");
    QApplication::beep();
}


void AuthForm::on_PasswordLine_inputRejected()
{
    ui->LoginMessage->show();
    ui->LoginMessage->setText("Не используйте запрещённые символы");
    QApplication::beep();
}

