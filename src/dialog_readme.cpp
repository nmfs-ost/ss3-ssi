#include "dialog_readme.h"
#include "ui_dialog_readme.h"

#include "dialog_fileview.h"

#include <QFile>
#include <QDir>
#include <QDate>
#include <QMessageBox>


Dialog_readme::Dialog_readme(QWidget *parent, ss_model *mdata, QString dir) :
    QDialog(parent),
    ui(new Ui::Dialog_readme)
{
    model = mdata;
    ui->setupUi(this);

    connect(ui->pushButton_save, SIGNAL(clicked()), SLOT(okayReadme()));
    connect(ui->pushButton_show, SIGNAL(clicked()), SLOT(viewReadme()));
    connect(ui->pushButton_close, SIGNAL(clicked()), SLOT(cancelReadme()));

    QString readmeName = QString(QString("%1/readme.txt").arg(dir));
    readmeFile.setFileName(readmeName);

    fview = new Dialog_fileView(this, "Model Meta Data", true);
    fview->setFixedPitch(true);
    connect (fview, SIGNAL(editRequested()), SLOT(editReadme()));
    connect (fview, SIGNAL(closed()), SLOT(cancelReadme()));
//    connect (this, SIGNAL(fileView()), fview, SLOT(exec()));

}

Dialog_readme::~Dialog_readme()
{
    delete fview;
    delete ui;
}

void Dialog_readme::setDirectory(QString dir)
{
    QString readmeName = QString(QString("%1/readme.txt").arg(dir));
    readmeFile.setFileName(readmeName);
}

void Dialog_readme::setFontSize(int ptsize)
{
    fview->setFontSize(ptsize);
}
//void Dialog_readme::exec(RType type) {
//    if (type == View) {
//        viewReadme();
//    }
//    else {
//        createReadme();
//    }
//    QDialog::exec();
//}



void Dialog_readme::viewReadme()
{
    if (readmeFile.exists()) {
        readReadme();
        hide();
        viewFile();
    }
    else {
        editReadme();
    }
}

void Dialog_readme::createReadme()
{
    if (readmeFile.exists()) {
        QMessageBox::information(this, tr("File "), tr("File already exists . . . reading data."));
        readReadme();
        hide();
        viewFile();
    }
    else {
        QString model = QDir::currentPath();
        model = model.split('/', QString::SkipEmptyParts).takeLast();
        ui->lineEdit_model->setText(model);
        ui->dateEdit->setDate(QDate::currentDate());
        writeReadme();
        editReadme();
    }
}

void Dialog_readme::readReadme()
{
    QString line;
    QStringList values;
    bool okay = true;
//    bool firstLine = false;
//    bool firstName = false;
//    bool lastName = false;
    if(readmeFile.open(QIODevice::ReadOnly)) {
        while (!readmeFile.atEnd() && okay) {
            line = QString(readmeFile.readLine(1024));
            values = line.split(':', QString::SkipEmptyParts);
            if (line.contains("changed") || line.contains("created")) {//!firstLine) {
                ui->label_readme->setText(line);
            }
            else if (values.at(0).contains("Model Name")) {
                ui->lineEdit_model->setText(values.at(1).simplified());
            }
            else if (values.at(0).contains("Version")) {
                okay = parseVersion(values.at(1).simplified());
            }
            else if (values.at(0).contains("Name")) {
                ui->lineEdit_name->setText(values.at(1).simplified());
            }
            else if (values.at(0).contains("eMail")) {
                ui->lineEdit_email->setText(values.at(1).simplified());
            }
            else if (values.at(0).contains("Date")) {
                okay = parseDate(values.at(1).simplified(), ui->dateEdit);
            }
            else if (values.at(0).contains("Features")) {
                line = QString(readmeFile.readLine(1024));
                ui->plainTextEdit_features->clear();
                while (line.count() > 1 && !line.contains("Other Information")) {
                    ui->plainTextEdit_features->appendPlainText(line);
                    line = QString(readmeFile.readLine(1024));
                }
                if (line.contains("Other Information"))
                    readOther();
            }
            else if (values.at(0).contains("Other")) {
                readOther();
            }
        }
        readmeFile.close();
    }
    else {
        QMessageBox::information(this, tr("File Error"), tr("File could not be opened to read. Creating . . ."));
        createReadme();
    }
}

// this assumes the readmeFile is open to read
void Dialog_readme::readOther()
{
    QString line = QString(readmeFile.readLine(1024));
    ui->plainTextEdit_info->clear();
    while (line.count() > 1) {
        ui->plainTextEdit_info->appendPlainText(line);
        if (readmeFile.atEnd())
            break;
        line = QString(readmeFile.readLine(1024));
    }
}

void Dialog_readme::editReadme()
{
    fview->hide();
    readReadme();
    show();
}

void Dialog_readme::okayReadme()
{
    writeReadme();
    viewReadme();
}

void Dialog_readme::writeReadme()
{
    bool exists = readmeFile.exists();
    if(readmeFile.open(QIODevice::WriteOnly)) {
        QString wline;

        QString mName = ui->lineEdit_model->text();
        QString cName = ui->lineEdit_name->text();
        QString eMail = ui->lineEdit_email->text();
        QString info = ui->plainTextEdit_info->toPlainText();

        if (exists) {
            wline.append(QString("Readme file changed on %1\n").arg(QDate::currentDate().toString()));
        }
        else {
            wline.append(QString("Readme file created on %1\n").arg(QDate::currentDate().toString()));
        }
        wline.append(QString("SS Version   : %1%2%3\n").arg(ui->spinBox_ver1->text(),
                                               ui->spinBox_ver2->text(),
                                               ui->spinBox_ver3->text()));
        wline.append(QString("\nModel Name   : "));
        if (!mName.isEmpty()) {
            wline.append(mName);
        }
        wline.append(QString("\n\nContact Name : "));
        if (!cName.isEmpty()) {
            wline.append(cName);
        }
        wline.append(QString("\nContact eMail: "));
        if (!eMail.isEmpty()) {
            wline.append(eMail);
        }
        wline.append(QString("\nDate         : %1\n").arg(ui->dateEdit->text()));
        wline.append(QString("\nBasic Model Data\n"));
        wline.append(QString("Num of years       : %1\n").arg(QString::number(model->getTotalYears())));
        wline.append(QString("Num of seasons     : %1\n").arg(QString::number(model->get_num_seasons())));
        wline.append(QString("Num of sub-seasons : %1\n").arg(QString::number(model->get_num_subseasons())));
        wline.append(QString("Num of fisheries   : %1\n").arg(QString::number(model->get_num_fisheries())));
        wline.append(QString("Num of surveys     : %1\n").arg(QString::number(model->get_num_surveys())));
        wline.append(QString("Num of sexes       : %1\n").arg(QString::number(model->get_num_genders())));
        wline.append(QString("Num of growth pttns: %1\n").arg(QString::number(model->getPopulation()->Grow()->getNum_patterns())));
        wline.append(QString("Read wtatage.ss?   : %1\n").arg(QString(model->getReadWtAtAge()?"Y":"N")));

        wline.append(QString("\nFeatures Used      :\n"));
        wline.append(formatFeatures());

        wline.append(QString("\nOther Information  :\n"));
        if (!info.isEmpty()) {
            wline.append(info);
        }

        readmeFile.write(wline.toUtf8());
        readmeFile.close();
    }
    else {
        QMessageBox::information(this, tr("File Error"), tr("File could not be opened to write."));
    }
}

void Dialog_readme::cancelReadme()
{
    fview->hide();
    close();
}

//void Dialog_readme::closeEvent(QCloseEvent *event)
//{
//    Q_UNUSED(event);
//    fview->hide();
//    hide();
//}

bool Dialog_readme::parseDate(QString line, QDateEdit *de)
{
    bool okay = true;
    QDate date = de->date();
    int y = 0, m = 0, d = 0;
    QStringList toks(line.split('/'));
    d = toks[0].toInt(&okay);
    if (okay)
        m = toks[1].toInt(&okay);
    if (okay)
        y = toks[2].toInt(&okay);
    if (okay)
        date.setDate(y,m,d);
    de->setDate(date);
    if (!okay) {
        QMessageBox::information(this, tr("File Error"), tr("Problem reading date information."));
    }
    return okay;
}

bool Dialog_readme::parseVersion(QString line)
{
    bool okay = true;
    int v1 = 3, v2 = 30, v3 = 0;
    QStringList toks(line.split('.'));
    v1 = toks[0].toInt(&okay);
    if (okay)
        v2 = toks[1].toInt(&okay);
    if (okay)
        v3 = toks[2].toInt(&okay);
    if (okay) {
        ui->spinBox_ver1->setValue(v1);
        ui->spinBox_ver2->setValue(v2);
        ui->spinBox_ver3->setValue(v3);
    }
    if (!okay) {
        QMessageBox::information(this, tr("File Error"), tr("Problem reading version information."));
    }
    return okay;
}

void Dialog_readme::viewFile()
{
    fview->viewFile(&readmeFile);
    fview->show();
    hide();
}

QString &Dialog_readme::formatFeatures()
{
    int numGPs = model->getPopulation()->Grow()->getNum_patterns();
    bool vonBert = true;
    QStringList featurelist;
    for (int i = 0; i < numGPs; i++)
        if (model->getPopulation()->SR()->getAssignment(i).at(0) > 1)
            vonBert = false;

    if (!features.isEmpty())
        features.clear();

    if (model->get_num_seasons() > 1)
        featurelist.append("Multiple seasons");
    if (model->get_num_areas() > 1)
        featurelist.append("Multiple areas");
    if (model->getNumEnvironVars() > 0)
        featurelist.append("Using environment variable(s)");
    if (model->getDoTags())
        featurelist.append("Using tags");
    if (model->getDoMorphComp())
        featurelist.append("Using Morph composition");
    if (model->getPopulation()->Grow()->getNum_morphs() > 1)
        featurelist.append("Multiple platoons");
    if (!vonBert)
        featurelist.append("Growth pattern not vonBertel");
    if (model->getPopulation()->SR()->getMethod() != 3)
        featurelist.append("Recruitment not Beverton/Holt");
    if (model->getUse2DAR1())
        featurelist.append("Using 2DAR1");

    if (!featurelist.isEmpty()) {
        features.append(featurelist.at(0));
        for (int i = 1; i < featurelist.count(); i++)
            features.append(QString(", %1").arg(featurelist.at(i)));
    }
    features.append('\n');

    return features;
}
