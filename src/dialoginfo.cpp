#include "dialoginfo.h"
#include "ui_dialoginfo.h"

DialogInfo::DialogInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogInfo)
{
    ui->setupUi(this);
    setModal(false);

    connect (this, SIGNAL(accepted()), SLOT(hide()));
    connect (this, SIGNAL(rejected()), SLOT(hide()));
    connect (ui->buttonBox, SIGNAL(accepted()), SLOT(hide()));
    ui->label_title->setText("");
    ui->label_title->hide();
}

DialogInfo::~DialogInfo()
{
    delete ui;
}

void DialogInfo::setTitle(QString title)
{
    ui->label_title->setText(title);
    ui->label_title->show();
}

void DialogInfo::setText (QString text)
{
    ui->plainTextEdit->setPlainText(text);
}
