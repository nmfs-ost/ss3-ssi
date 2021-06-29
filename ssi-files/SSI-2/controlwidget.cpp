#include "controlwidget.h"
#include "ui_controlwidget.h"

controlWidget::controlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::controlWidget)
{
    ui->setupUi(this);
    controlFile = new ssFile(this);
}

controlWidget::~controlWidget()
{
    delete ui;
    delete controlFile;
}

bool controlWidget::readFile(QString file)
{
    bool okay = true;

    controlFile->setFileName(file);

    return okay;
}

bool controlWidget::writeFile()
{
    bool okay = true;
    return okay;
}
