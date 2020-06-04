#include <QDir>
#include <QFileDialog>

#include "dialogdirectories.h"
#include "ui_dialogdirectories.h"


DialogDirectories::DialogDirectories(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDirectories)
{
    ui->setupUi(this);
    QDir curdir(QDir::current());
    setDirectory(curdir.path());

    connect(ui->pushButton_ok, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->pushButton_cancel, SIGNAL(clicked()), SLOT(reject()));
    connect(ui->pushButton_change1, SIGNAL(clicked()), SLOT(selectDir1()));
    connect(ui->pushButton_change2, SIGNAL(clicked()), SLOT(selectDir2()));
}

DialogDirectories::~DialogDirectories()
{
    delete ui;
}

QString &DialogDirectories::getDirectory(int value) {
    QString *dir = new QString("");
    if (value == 1) {
        dir->append(model1);
    }
    else if (value == 2) {
        dir->append(model2);
    }
    return *dir;
}

void DialogDirectories::setDirectory(const QString &value) {
    setModel1(value);
    setModel2(QString("choose directory"));
}

QString &DialogDirectories::selectDirectory(QString dir) {
    newDir = (QFileDialog::getExistingDirectory(this, tr("Select Directory"),
                                dir, QFileDialog::ShowDirsOnly));
    return newDir;
}

void DialogDirectories::selectDir1() {
    model1 = selectDirectory(model1);
}

void DialogDirectories::selectDir2() {
    if (model2.isEmpty() || model2.contains("choose")) {
        model2 = selectDirectory(model1);
    }
    else {
        model2 = selectDirectory(model2);
    }
    ui->label_directory2->setText(model2);
}

void DialogDirectories::accept() {
    model1 = ui->label_directory1->text();
    model2 = ui->label_directory2->text();
    accepted();
    hide();
}

void DialogDirectories::reject() {
    model1 = QString("");
    model2 = QString("");
    hide();
}

QString DialogDirectories::getModel2() const
{
    return model2;
}

void DialogDirectories::setModel2(const QString &value)
{
    model2 = value;
    ui->label_directory2->setText(model2);
}

QString DialogDirectories::getModel1() const
{
    return model1;
}

void DialogDirectories::setModel1(const QString &value)
{
    model1 = value;
    ui->label_directory1->setText(model1);
}
