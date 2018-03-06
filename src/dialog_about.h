/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef DIALOG_ABOUT_H
#define DIALOG_ABOUT_H

#include <QDialog>

namespace Ui {
class Dialog_about;
}

class Dialog_about : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_about(QWidget *parent = 0);
    ~Dialog_about();

public slots:
    void goToWebsite();

signals:
    void show_manual ();
    void show_webpage (QString pg);

private:
    Ui::Dialog_about *ui;
};

#endif // DIALOG_ABOUT_H
