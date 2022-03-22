/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef DIALOG_FILEVIEW_H
#define DIALOG_FILEVIEW_H

#include <QDialog>
#include <QString>
#include <QFile>

namespace Ui {
class Dialog_fileView;
}

class Dialog_fileView : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_fileView(QWidget *parent = nullptr, QString title = QString(), bool editable = false);
    ~Dialog_fileView();
    bool close();

public slots:
    void viewFile(QString filename);
    void viewFile(QFile *file);
    void increaseFont (bool flag = true);
    void decreaseFont(bool flag = true);
    void setFontSize (int ptsize);
    void setTitle (QString title = QString(""));
    void setFixedPitch(bool fixed = false);
    void hideFont();

signals:
    void editRequested();
    void closed();

private:
    Ui::Dialog_fileView *ui;

    QFont sfont;
    QFont vfont;
    QFont tfont;

private slots:
    void print();
    void edit();
};

#endif // DIALOG_FILEVIEW_H
