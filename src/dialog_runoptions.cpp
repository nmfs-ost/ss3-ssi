#include "dialog_runoptions.h"
#include "ui_dialog_runoptions.h"

Dialog_runoptions::Dialog_runoptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_runoptions)
{
    ui->setupUi(this);

    connect (ui->checkBox_nohess, SIGNAL(toggled(bool)), SLOT(noHessChecked(bool)));
    connect (ui->checkBox_shess,  SIGNAL(toggled(bool)), SLOT(sHessChecked(bool)));
    connect (ui->checkBox_est,    SIGNAL(toggled(bool)), SLOT(estChecked(bool)));
    connect (ui->checkBox_noest,  SIGNAL(toggled(bool)), SLOT(noestChecked(bool)));
    connect (ui->checkBox_lprof,  SIGNAL(toggled(bool)), SIGNAL(doLikeProf(bool)));
    connect (ui->checkBox_mcdiag, SIGNAL(toggled(bool)), SIGNAL(doMCDiag(bool)));

//    connect (ui->buttonBox, SIGNAL(accepted()), SLOT(acceptOptions()));
//    connect (ui->buttonBox, SIGNAL(rejected()), SLOT(rejectOptions()));
    connect (ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), SLOT(buttonPressed(QAbstractButton*)));
}

Dialog_runoptions::~Dialog_runoptions()
{
    delete ui;
}

void Dialog_runoptions::noHessChecked(bool flag)
{
    if (flag)
        ui->checkBox_shess->setChecked(false);

    emit doNoHess(flag);
}

void Dialog_runoptions::sHessChecked(bool flag)
{
    if (flag)
        ui->checkBox_nohess->setChecked(false);

    emit doSparseHess(flag);
}

void Dialog_runoptions::estChecked(bool flag)
{
    if (flag)
        ui->checkBox_noest->setChecked(false);

    emit doEstOnly(flag);
}

void Dialog_runoptions::noestChecked(bool flag)
{
    if (flag)
        ui->checkBox_est->setChecked(false);

    emit doNoEst(flag);
}

void Dialog_runoptions::buttonPressed(QAbstractButton *btn)
{
    if (btn->text().compare("Apply") == 0)
    {
        acceptOptions();
    }
    else if (btn->text().compare("Reset") == 0)
    {
        rejectOptions();
    }
    else if (btn->text().compare("Cancel") == 0)
    {
        rejectOptions();
        reject();
    }
    else if (btn->text().compare("OK") == 0)
    {
        accept();
    }
}

void Dialog_runoptions::acceptOptions()
{
    emit accepted();
}

void Dialog_runoptions::rejectOptions()
{
    ui->checkBox_nohess->setChecked(false);
    ui->checkBox_shess->setChecked(false);
    ui->checkBox_est->setChecked(false);
    ui->checkBox_noest->setChecked(false);
    ui->checkBox_lprof->setChecked(false);
    ui->checkBox_mcdiag->setChecked(false);
    emit rejected();
}
