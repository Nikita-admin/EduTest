#ifndef TASKFORM_H
#define TASKFORM_H

#include <QWidget>
#include "netclient.h"

namespace Ui {
class TaskForm;
}

class TaskForm : public QWidget
{
    Q_OBJECT

public:
    explicit TaskForm(QWidget *parent = nullptr);
    ~TaskForm();
    void show_task(int number);

private slots:
    void on_SendAnswerButton_clicked();

    void on_ReceiveCheckData(QString data);
private:
    Ui::TaskForm *ui;
    int task_number;
};

#endif // TASKFORM_H
