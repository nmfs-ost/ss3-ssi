#include "dialog_copyright.h"
#include "ui_dialog_copyright.h"

Dialog_copyright::Dialog_copyright(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_copyright)
{
    ui->setupUi(this);
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(close()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

Dialog_copyright::~Dialog_copyright()
{
    delete ui;
}
