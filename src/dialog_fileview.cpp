#include "dialog_fileview.h"
#include "ui_dialog_fileview.h"

#include <QPrinter>
#include <QPrintDialog>

Dialog_fileView::Dialog_fileView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_fileView)
{
    ui->setupUi(this);

    connect (ui->pushButton_print, SIGNAL(clicked()), SLOT(print()));
    connect (ui->pushButton_close, SIGNAL(clicked()), SLOT(close()));

    connect (ui->spinBox_fontsize, SIGNAL(valueChanged(int)), SLOT(setFontSize(int)));

    vfont = QFont(fontInfo().family(), fontInfo().pointSize());
    setFontSize(9);
}

Dialog_fileView::~Dialog_fileView()
{
    delete ui;
}

void Dialog_fileView::viewFile(QString filename)
{
    ui->label_filename->setText(filename);

    QByteArray text;
    if (!filename.isEmpty())
    {
        QFile qf(filename);
        if (qf.open(QIODevice::ReadOnly))
        {
            text = qf.readAll();
            ui->plainTextEdit->setPlainText(QString(text));
        }
    }
}

void Dialog_fileView::print()
{
    QPrinter printer;
    QPrintDialog *qpd = new QPrintDialog(&printer, this);
    int action;
    action = qpd->exec();
    if (action == QDialog::Accepted)
        ui->plainTextEdit->print(&printer);
}

void Dialog_fileView::increaseFont (bool flag)
{
    int f_size = vfont.pointSize();
    if (flag)
    if (f_size < 16)
    {
        f_size += 1;
    }
    setFontSize(f_size);
}

void Dialog_fileView::decreaseFont(bool flag)
{
    int f_size = vfont.pointSize();
    if (flag)
    if (f_size > 8)
    {
        f_size -= 1;
    }
    setFontSize(f_size);
}

void Dialog_fileView::setFontSize(int ptsize)
{
    vfont.setPointSize(ptsize);
    setFont(vfont);
}
