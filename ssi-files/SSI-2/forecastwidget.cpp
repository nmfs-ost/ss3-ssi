#include "forecastwidget.h"
#include "ui_forecastwidget.h"

forecastWidget::forecastWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::forecastWidget)
{
    ui->setupUi(this);
    forecastFile = new ssFile(this);
}

forecastWidget::~forecastWidget()
{
    delete ui;
    delete forecastFile;
}

bool forecastWidget::readFile(QString file)
{
    bool okay = true;

    forecastFile->setFileName(file);

    return okay;
}

bool forecastWidget::writeFile()
{
    bool okay = true;
    return okay;
}
