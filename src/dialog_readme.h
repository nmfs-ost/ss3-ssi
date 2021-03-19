/** /class Dialog_readme
 *  Create, modify, or view meta data about this model.
*/
#ifndef DIALOG_README_H
#define DIALOG_README_H

#include <QDialog>
#include <QFile>
#include <QDir>
#include <QDate>
#include <QDateEdit>

#include "dialog_fileview.h"
#include "model.h"

enum RType {
    View,
    Create
};

namespace Ui {
class Dialog_readme;
}

class Dialog_readme : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_readme(QWidget *parent = nullptr, ss_model *mdata = nullptr,
                           QString dir = QDir::current().path());
    ~Dialog_readme();

public slots:
    void setDirectory(QString dir);
//    void exec(RType type);

    void viewReadme();
    void createReadme();
    void readReadme();
    void editReadme();
    void writeReadme();

    void cancelReadme();
    void okayReadme();

    void setFontSize(int ptsize);

//    void closeEvent(QCloseEvent *);

signals:
    void fileView();

private:
    Ui::Dialog_readme *ui;

    QFile readmeFile;
    Dialog_fileView *fview;
    void viewFile();
    QString &formatFeatures();
    QString features;

    ss_model *model;

private slots:
    bool parseDate(QString line, QDateEdit *de);
    bool parseVersion(QString line);
    void readOther();
};

#endif // DIALOG_README_H
