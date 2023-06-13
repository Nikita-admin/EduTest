#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "authform.h"
#include "taskform.h"
#include "changepassword.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void auth_ok_slot();

    void on_ReceiveNameData(QString data);

    void on_ReceiveStatData(QString data);

    void on_ReceiveLogoutData(QString data);

    void on_StartTask1Button_clicked();

    void on_StartTask2Button_clicked();

    void on_StartTask3Button_clicked();

    void on_StartTask4Button_clicked();

    void on_StartTask5Button_clicked();

    void on_ExitButton_clicked();

    void on_ChangePasswordButton_clicked();

    void on_ReceivePasswordChangeData(QString data);

    void on_ServerDisconnected();

    void on_SearchLine_textChanged(const QString &text);

private:
    Ui::MainWindow *ui;
    AuthForm *ui_auth;
    TaskForm *ui_task;
    ChangePassword *ui_change;
    QSortFilterProxyModel *SearchTableModel;
};
#endif // MAINWINDOW_H
