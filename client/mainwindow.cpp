#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->SearchLabel->hide();
    ui->SearchLine->hide();
    ui->StatView->hide();
    ui_auth = new AuthForm;
    connect(ui_auth,&AuthForm::auth_ok,this, &MainWindow::auth_ok_slot);
    connect(NetClient::Connect(),&NetClient::ServerDisconnectedSignal,this,&MainWindow::on_ServerDisconnected);
    connect(NetClient::Connect(),&NetClient::ReceiveNameData,this,&MainWindow::on_ReceiveNameData);
    connect(NetClient::Connect(),&NetClient::ReceiveStatData,this,&MainWindow::on_ReceiveStatData);
    connect(NetClient::Connect(),&NetClient::ReceiveLogoutData,this,&MainWindow::on_ReceiveLogoutData);
    ui_auth->show();
    ui->StatView->hide();
    ui_task = new TaskForm;
    ui_change = new ChangePassword;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete ui_task;
    delete ui_auth;
    delete ui_change;
    delete SearchTableModel;
}

void MainWindow::on_ServerDisconnected()
{
    disconnect(ui_auth,&AuthForm::auth_ok,this, &MainWindow::auth_ok_slot);
    if (ui_auth != nullptr)
    {
        delete ui_auth;
    }
    ui_auth = new AuthForm;
    connect(ui_auth,&AuthForm::auth_ok,this, &MainWindow::auth_ok_slot);
    ui_task->close();
    ui_change->close();
    this->close();
    ui_auth->show();
    QMessageBox::critical(this, "Ошибка", "Потеряно соединение с сервером.");
}

void MainWindow::on_ReceiveStatData(QString data)
{
    QList<QString> parametrs = data.split("+");
    QString role = parametrs[1];
    QStringList UsersData = parametrs[2].split(';', Qt::SkipEmptyParts);

    if (role == "student" and UsersData.size() == 1)
    {
        ui->SearchLabel->hide();
        ui->SearchLine->hide();
        ui->StatView->hide();
        ui->InfoLabel->show();
        ui->Task1StatLabel->show();
        ui->Task2StatLabel->show();
        ui->Task3StatLabel->show();
        ui->Task4StatLabel->show();
        ui->Task5StatLabel->show();
        ui->Task1Label->show();
        ui->Task2Label->show();
        ui->Task3Label->show();
        ui->Task4Label->show();
        ui->Task5Label->show();

        QStringList UserData = UsersData[0].split(',', Qt::SkipEmptyParts);
        ui->Task1StatLabel->setText(UserData[3]);
        ui->Task2StatLabel->setText(UserData[4]);
        ui->Task3StatLabel->setText(UserData[5]);
        ui->Task4StatLabel->setText(UserData[6]);
        ui->Task5StatLabel->setText(UserData[7]);
    }
    else if (role == "teacher")
    {
        ui->InfoLabel->hide();
        ui->Task1StatLabel->hide();
        ui->Task2StatLabel->hide();
        ui->Task3StatLabel->hide();
        ui->Task4StatLabel->hide();
        ui->Task5StatLabel->hide();
        ui->Task1Label->hide();
        ui->Task2Label->hide();
        ui->Task3Label->hide();
        ui->Task4Label->hide();
        ui->Task5Label->hide();
        ui->StatView->show();
        ui->SearchLabel->show();
        ui->SearchLine->show();

        int NumRows = UsersData.size();
        int NumColumns = 8;

        QStandardItemModel *DataTableModel = new QStandardItemModel(NumRows, NumColumns);

        for (int row = 0; row < NumRows; ++row) {
            QStringList userData = UsersData[row].split(',', Qt::SkipEmptyParts);
            for (int col = 0; col < NumColumns; ++col) {
                QStandardItem *item = new QStandardItem(userData[col]);
                DataTableModel->setItem(row, col, item);
            }
        }

        DataTableModel->setHeaderData(0, Qt::Horizontal, "Имя");
        DataTableModel->setHeaderData(1, Qt::Horizontal, "Фамилия");
        DataTableModel->setHeaderData(2, Qt::Horizontal, "Отчество");
        DataTableModel->setHeaderData(3, Qt::Horizontal, "№1");
        DataTableModel->setHeaderData(4, Qt::Horizontal, "№2");
        DataTableModel->setHeaderData(5, Qt::Horizontal, "№3");
        DataTableModel->setHeaderData(6, Qt::Horizontal, "№4");
        DataTableModel->setHeaderData(7, Qt::Horizontal, "№5");

        SearchTableModel = new QSortFilterProxyModel(this);
        SearchTableModel->setSourceModel(DataTableModel);
        SearchTableModel->setFilterKeyColumn(-1);

        ui->StatView->setModel(SearchTableModel);
        ui->StatView->verticalHeader()->setVisible(false);
        ui->StatView->resizeRowsToContents();
        ui->StatView->resizeColumnsToContents();
        ui->StatView->setSortingEnabled(true);
        ui->StatView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
}

void MainWindow::auth_ok_slot()
{
    ui_auth->close();
    disconnect(ui_auth,&AuthForm::auth_ok,this, &MainWindow::auth_ok_slot);
    this->show();
    NetClient::Connect()->SendData("getname");
}

void MainWindow::on_ReceiveNameData(QString data)
{
    QList<QString> parametrs = data.split("+");
    if (parametrs[1] == "ok")
    {
        QString name = parametrs[2].left(parametrs[2].lastIndexOf(";"));
        int time = QTime::currentTime().hour();
        if (time >= 0 and time < 6)
        {
            ui->WelcomeMessage->setText("Доброй ночи, " + name);
        }
        else if (time >= 6 and time < 12)
        {
            ui->WelcomeMessage->setText("Доброе утро, " + name);
        }
        else if (time >= 12 and time < 18)
        {
            ui->WelcomeMessage->setText("Добрый день, " + name);
        }
        else if (time >= 18 and time <= 23)
        {
            ui->WelcomeMessage->setText("Добрый вечер, " + name);
        }
        else
        {
            ui->WelcomeMessage->setText("Добро пожаловать, " + name);
        }

        NetClient::Connect()->SendData("stat");
    }
    else
    {
        ui->WelcomeMessage->setText("Ошибка авторизации.");
        this->close();
    }
}

void MainWindow::on_StartTask1Button_clicked()
{
    if (ui_task != nullptr)
    {
        delete ui_task;
    }
    ui_task = new TaskForm;
    ui_task->show_task(1);
}


void MainWindow::on_StartTask2Button_clicked()
{
    if (ui_task != nullptr)
    {
        delete ui_task;
    }
    ui_task = new TaskForm;
    ui_task->show_task(2);
}


void MainWindow::on_StartTask3Button_clicked()
{
    if (ui_task != nullptr)
    {
        delete ui_task;
    }
    ui_task = new TaskForm;
    ui_task->show_task(3);
}


void MainWindow::on_StartTask4Button_clicked()
{
    if (ui_task != nullptr)
    {
        delete ui_task;
    }
    ui_task = new TaskForm;
    ui_task->show_task(4);
}


void MainWindow::on_StartTask5Button_clicked()
{
    if (ui_task != nullptr)
    {
        delete ui_task;
    }
    ui_task = new TaskForm;
    ui_task->show_task(5);
}


void MainWindow::on_ExitButton_clicked()
{
    NetClient::Connect()->SendData("logout");
}

void MainWindow::on_ReceiveLogoutData(QString data)
{
    if (data == "logout+ok")
    {
        if (ui_auth != nullptr)
        {
            delete ui_auth;
        }
        ui_auth = new AuthForm;
        connect(ui_auth,&AuthForm::auth_ok,this, &MainWindow::auth_ok_slot);
        this->close();
        ui_auth->show();
    }
}

void MainWindow::on_ChangePasswordButton_clicked()
{
    if (ui_change != nullptr)
    {
        delete ui_change;
    }
    ui_change = new ChangePassword;
    connect(NetClient::Connect(),&NetClient::ReceivePasswordChangeData,this,&MainWindow::on_ReceivePasswordChangeData);
    ui_change->show();
}

void MainWindow::on_ReceivePasswordChangeData(QString data)
{
    QList<QString> parametrs = data.split("+");
    if (parametrs[1] == "ok")
    {
        QMessageBox::information(this, "EduTest", "Пароль успешно изменён.");
    }
    else
    {
        QMessageBox::warning(this, "EduTest", "Старый пароль неверный.");
    }
}


void MainWindow::on_SearchLine_textChanged(const QString &text)
{
    SearchTableModel->setFilterRegularExpression(QRegularExpression("^" + text, QRegularExpression::CaseInsensitiveOption));
}

