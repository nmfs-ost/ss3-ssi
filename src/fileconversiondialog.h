/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef FILECONVERSIONDIALOG_H
#define FILECONVERSIONDIALOG_H

#include <QDialog>

namespace Ui {
class FileConversionDialog;
}

class FileConversionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileConversionDialog(QWidget *parent = 0);
    ~FileConversionDialog();

private:
    Ui::FileConversionDialog *ui;
};

#endif // FILECONVERSIONDIALOG_H
