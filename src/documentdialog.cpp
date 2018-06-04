#include "documentdialog.h"
#include "ui_documentdialog.h"
#include "metadata.h"

#include <QFileInfo>
#include <QFileDialog>
#include <QProcess>

documentDialog::documentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::documentDialog)
{
    ui->setupUi(this);
    connect (ui->pushButton_manual,    SIGNAL(clicked()), SLOT(chooseManual()));
    connect (ui->pushButton_technical, SIGNAL(clicked()), SLOT(chooseTechnical()));
    connect (ui->pushButton_guiGuide,  SIGNAL(clicked()), SLOT(chooseGuiGuide()));

    connect (ui->pushButton_viewManual,    SIGNAL(clicked()), SLOT(viewManual()));
    connect (ui->pushButton_viewTechnical, SIGNAL(clicked()), SLOT(viewTechnical()));
    connect (ui->pushButton_viewGuiGuide,  SIGNAL(clicked()), SLOT(viewGuiGuide()));
    getSettings();
}

documentDialog::~documentDialog()
{
    saveSettings();
    delete ui;
}

void documentDialog::getSettings()
{
    QSettings settings (app_copyright_org, app_name);
    settings.beginGroup("documents");
    ui->lineEdit_manual->setText(settings.value("manual").toString());
    ui->lineEdit_technical->setText(settings.value("technical").toString());
    ui->lineEdit_guiGuide->setText(settings.value("guiGuide").toString());
    settings.endGroup();
}

void documentDialog::saveSettings()
{
    QSettings settings (app_copyright_org, app_name);
    settings.beginGroup("documents");
    settings.setValue("manual", ui->lineEdit_manual->text());
    settings.setValue("technical", ui->lineEdit_technical->text());
    settings.setValue("guiGuide", ui->lineEdit_guiGuide->text());
    settings.endGroup();
}

void documentDialog::viewManual()
{
    viewDocument(ui->lineEdit_manual->text());
}

void documentDialog::chooseManual()
{
    QString filename (chooseDocument("User Manual"));
    ui->lineEdit_manual->setText(filename);
}

void documentDialog::viewTechnical()
{
    viewDocument(ui->lineEdit_technical->text());
}

void documentDialog::chooseTechnical()
{
    QString filename (chooseDocument("Technical Description"));
    ui->lineEdit_technical->setText(filename);
}

void documentDialog::viewGuiGuide()
{
    viewDocument(ui->lineEdit_guiGuide->text());
}

void documentDialog::chooseGuiGuide()
{
    QString filename (chooseDocument("Interface Guide"));
    ui->lineEdit_guiGuide->setText(filename);
}

void documentDialog::viewDocument(QString name)
{
    QFileInfo qf (name);

    if (qf.exists())
    {
        QProcess *process = new QProcess (this);
        QString filename (qf.fileName());
        QString command = QString("cmd /k %1").arg(filename);
        process->start(command);
    }
}

QString documentDialog::chooseDocument(QString title)
{
    QString filename ("");
    QString str (QString ("Select %1 File").arg(title));
    filename = (QFileDialog::getOpenFileName (this, tr(str.toUtf8()),
        qApp->applicationDirPath(), tr("documentation files (*.pdf)")));
    return filename;
}

