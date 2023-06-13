#include "taskform.h"
#include "ui_taskform.h"
#include <QRegularExpressionValidator>

TaskForm::TaskForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TaskForm)
{
    ui->setupUi(this);
    ui->ResultCheckedAnswer->hide();
    QRegularExpression answer("[^+]*");
    ui->AnswerLine->setValidator(new QRegularExpressionValidator(answer, this));
    ui->ErrorLabel->setStyleSheet("color: rgb(255,0,0)");
    ui->ErrorLabel->hide();
}

TaskForm::~TaskForm()
{
    delete ui;
}

void TaskForm::show_task(int number)
{
    this->task_number = number;
    ui->TaskNumber->setText(QString::fromStdString("Задание номер ")+ QString::number(number));
    this->show();
}

void TaskForm::on_SendAnswerButton_clicked()
{
    if (ui->AnswerLine->text() != "")
    {
        ui->ErrorLabel->hide();
        connect(NetClient::Connect(),&NetClient::ReceiveCheckData,this,&TaskForm::on_ReceiveCheckData);
        NetClient::Connect()->SendData("check+" + QString::number(task_number) + "+1");
    }
    else
    {
        ui->AnswerLine->setFocus();
        ui->ErrorLabel->show();
        QApplication::beep();
    }
}

void TaskForm::on_ReceiveCheckData(QString data)
{
    QList<QString> parametrs = data.split("+");
    if (parametrs[1] == "ok")
    {
        ui->SendAnswerButton->hide();
        ui->ResultCheckedAnswer->setText("Верно.");
        ui->ResultCheckedAnswer->show();
    }
    if (parametrs[1] == "wrong")
    {
        ui->SendAnswerButton->hide();
        ui->ResultCheckedAnswer->setText("Неверно.");
        ui->ResultCheckedAnswer->show();
    }
    NetClient::Connect()->SendData("stat");
}
