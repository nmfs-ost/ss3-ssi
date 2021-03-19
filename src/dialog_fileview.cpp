#include "dialog_fileview.h"
#include "ui_dialog_fileview.h"

#include <QPrinter>
#include <QPrintDialog>

Dialog_fileView::Dialog_fileView(QWidget *parent, QString title, bool editable) :
    QDialog(parent),
    ui(new Ui::Dialog_fileView)
{
    ui->setupUi(this);
    setTitle(title);

    connect (ui->pushButton_print, SIGNAL(clicked()), SLOT(print()));
    connect (ui->pushButton_edit, SIGNAL(clicked()), SLOT(edit()));
    connect (ui->pushButton_close, SIGNAL(clicked()), SLOT(close()));

    ui->pushButton_edit->setVisible(editable);
    ui->pushButton_edit->setEnabled(editable);


    sfont = QFont(fontInfo().family(), fontInfo().pointSize());
    vfont = QFont(sfont);
    tfont = QFont(sfont);
    tfont.setBold(true);
    connect (ui->spinBox_fontsize, SIGNAL(valueChanged(int)), SLOT(setFontSize(int)));
    ui->spinBox_fontsize->setValue(9);
}

Dialog_fileView::~Dialog_fileView()
{
    delete ui;
}

void Dialog_fileView::viewFile(QString filename)
{
    if (!filename.isEmpty())
    {
        QFile qf(filename);
        viewFile(&qf);
    }
}

bool Dialog_fileView::close()
{
    emit closed();
    return QDialog::close();
}

void Dialog_fileView::viewFile(QFile *file)
{
    QByteArray text;

    ui->label_filename->setText(file->fileName());
    if (file->open(QIODevice::ReadOnly))
    {
        text = file->readAll();
        ui->plainTextEdit->setPlainText(QString(text));
    }
    file->close();
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

void Dialog_fileView::edit()
{
    emit editRequested();
}

void Dialog_fileView::increaseFont (bool flag)
{
    int f_size = vfont.pointSize();
    if (flag)
    if (f_size < 14)
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
    // System font (sfont) 10 or greater
    if (ptsize > 10) {
        sfont.setPointSize(ptsize);
    }
    else {
        sfont.setPointSize(10);
    }
    QDialog::setFont(sfont);

    // title font (tfont)
    tfont.setPointSize(sfont.pointSize() + 1);
    tfont.setBold(true);
    ui->label_title->setFont(tfont);

    // file contents font (vfont)
    vfont.setPointSize(ptsize);
    ui->label_filename->setFont(vfont);
    ui->plainTextEdit->setFont(vfont);
}

void Dialog_fileView::setFixedPitch(bool fixed)
{
    if (fixed)
        vfont.setFamily("Monospace");
    vfont.setFixedPitch(fixed);
    setFontSize(vfont.pointSize());
}

void Dialog_fileView::setTitle(QString title)
{
    if (title.isEmpty()) {
        ui->label_title->hide();
    }
    else {
        ui->label_title->setText(title);
        ui->label_title->setVisible(true);
        setFontSize(tfont.pointSize());
    }
}
