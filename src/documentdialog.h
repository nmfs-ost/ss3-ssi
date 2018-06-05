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

    void getSettings();
    void saveSettings();

public slots:
    void viewManual ();
    void viewTechnical ();
    void viewGuiGuide ();
    void viewDocument (QString filename);
    void chooseManual ();
    void chooseTechnical ();
    void chooseGuiGuide ();
    QString chooseDocument (QString title);

private:
    Ui::documentDialog *ui;
    QSettings settings;
    void closeEvent(QCloseEvent *evt);
};

#endif // DOCUMENTDIALOG_H
