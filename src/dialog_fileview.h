/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef DIALOG_FILEVIEW_H
#define DIALOG_FILEVIEW_H

#include <QDialog>
#include <QString>

namespace Ui {
class Dialog_fileView;
}

class Dialog_fileView : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_fileView(QWidget *parent = 0);
    ~Dialog_fileView();

public slots:
    void viewFile(QString filename);
    void increaseFont (bool flag = true);
    void decreaseFont(bool flag = true);
    void setFontSize (int ptsize);

private:
    Ui::Dialog_fileView *ui;

    QFont vfont;

private slots:
    void print();
};

#endif // DIALOG_FILEVIEW_H
