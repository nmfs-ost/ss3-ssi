#include "dialog_about_gui.h"
#include "ui_dialog_about_gui.h"
#include "metadata.h"
#include "dialog_copyright.h"
#include "dialog_fileview.h"

Dialog_about_gui::Dialog_about_gui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_about_gui)
{
    QFont qf("Arial", 14);
    ui->setupUi(this);

    ui->label_app->setText(getAppName());
    ui->label_app->setFont(qf);
    qf.setPointSize(12);
    ui->label_ver->setText(QString("%1: %2  -  %3: %4").arg(tr("Version"), getAppVersion(), tr("Build date"), getAppBuildDate()));
    ui->label_ver->setFont(qf);
    ui->label_appliesTo->setText(QString("For use with Stock Synthesis version %1").arg(getAppAppliesTo()));
    ui->label_appliesTo->setFont(qf);
//    ui->label_copyright->setText(QString("copyright %1 by %2").arg(getAppCopyright(), getAppOrg()));

    qf.setPointSize(10);
    QString txt("This interface will let the user read and examine data files for Stock Synthesis, ");
    txt.append(QString("run the ss.exe program, and check for errors.\n\n"));
    txt.append(QString("The interface was designed and programmed by Neal Schindler using the following tools:\n"));
    txt.append(QString("    Qt ver 5.12 graphics libraries,\n"));
    txt.append(QString("    Qt Creator (a cross platform IDE), and\n"));
//    txt.append(QString("    Inno Setup (for Windows installer),\n"));
//    txt.append(QString("    Doxygen (for documentation), and\n"));
    txt.append(QString("    MinGW 7.3 (GCC for Windows)."));
    ui->textEdit->setText(txt);
    ui->textEdit->setFont(qf);

    readmeview = new Dialog_fileView(this);
    readmeview->hide();

    connect (ui->pushButton_userGuide, SIGNAL(clicked()), SIGNAL(showGuide()));
    connect (ui->pushButton_copyright, SIGNAL(clicked()), SIGNAL(showCopyright()));
    connect (ui->toolButton_aboutQt, SIGNAL(clicked()), SIGNAL(showAboutQt()));
    connect (ui->pushButton_readme, SIGNAL(clicked()), SLOT(showReadme()));
    connect (ui->pushButton_ok, SIGNAL(clicked()), SLOT(close()));
}

Dialog_about_gui::~Dialog_about_gui()
{
    delete ui;
}

void Dialog_about_gui::showReadme() {
    QString dir (qApp->applicationDirPath());
    readmeview->viewFile(dir + QString("/SSI_readme.txt"));
    readmeview->show();
}
