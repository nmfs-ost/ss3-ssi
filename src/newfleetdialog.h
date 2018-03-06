/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef NEWFLEETDIALOG_H
#define NEWFLEETDIALOG_H

#include <QDialog>

namespace Ui {
class NewFleetDialog;
}

class NewFleetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewFleetDialog(QWidget *parent = 0);
    ~NewFleetDialog();

private:
    Ui::NewFleetDialog *ui;
};

#endif // NEWFLEETDIALOG_H
