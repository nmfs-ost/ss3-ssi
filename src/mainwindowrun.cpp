#include "mainwindowrun.h"
#include "ui_mainwindowrun.h"

MainWindowRun::MainWindowRun(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowRun)
{
    ui->setupUi(this);
}

MainWindowRun::~MainWindowRun()
{
    delete ui;
}
