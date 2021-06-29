#include "datawidget.h"
#include "ui_datawidget.h"

dataWidget::dataWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dataWidget)
{
    ui->setupUi(this);
    dataFile = new ssFile(this);
}

dataWidget::~dataWidget()
{
    delete ui;
    delete dataFile;
}

bool dataWidget::readFile(QString file)
{
    bool okay = true;

    dataFile->setFileName(file);

    return okay;
}

bool dataWidget::writeFile()
{
    bool okay = true;
    return okay;
}
