#ifndef MAINWINDOWRUN_H
#define MAINWINDOWRUN_H

#include <QMainWindow>

namespace Ui {
class MainWindowRun;
}

class MainWindowRun : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowRun(QWidget *parent = nullptr);
    ~MainWindowRun();

private:
    Ui::MainWindowRun *ui;
};

#endif // MAINWINDOWRUN_H
