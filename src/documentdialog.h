/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef DOCUMENTDIALOG_H
#define DOCUMENTDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QString>

namespace Ui {
class documentDialog;
}

class documentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit documentDialog(QWidget *parent = 0);
    ~documentDialog();

public slots:
    void chooseManual ();
    void chooseTechnical ();
    QString chooseDocument (QString title);

private:
    Ui::documentDialog *ui;
    QSettings settings;
};

#endif // DOCUMENTDIALOG_H
