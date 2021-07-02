#include "starterwidget.h"
#include "ui_starterwidget.h"

starterWidget::starterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::starterWidget)
{
    ui->setupUi(this);

    starterFile = new ssFile(this);
}

starterWidget::~starterWidget()
{
    delete ui;
    delete starterFile;
}

bool starterWidget::readFile(QString file)
{
    bool okay = true;

    starterFile->setFileName(file);
    if (starterFile->open(QIODevice::ReadOnly))
    {
        starterFile->readComments();
        ui->commentBlock->setPlainText(starterFile->getCommentString());

    }
    else
    {
        okay = false;
    }

    return okay;
}

bool starterWidget::writeFile()
{
    bool okay = true;
    return okay;
}

QString starterWidget::getDataFile()
{
    return ui->lineEdit_dataFile->text();
}

QString starterWidget::getControlFile()
{
    return ui->lineEdit_controlFile->text();
}
