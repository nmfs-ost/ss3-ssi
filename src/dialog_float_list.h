/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef DIALOG_FLOAT_LIST_H
#define DIALOG_FLOAT_LIST_H

#include <QDialog>

namespace Ui {
class Dialog_float_list;
}

class Dialog_float_list : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_float_list(QWidget *parent = 0);
    ~Dialog_float_list();

private:
    Ui::Dialog_float_list *ui;
};

#endif // DIALOG_FLOAT_LIST_H
