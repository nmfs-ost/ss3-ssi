/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef DIALOGINPUTFILES_H
#define DIALOGINPUTFILES_H

#include <QDialog>

namespace Ui {
class DialogInputFiles;
}

class DialogInputFiles : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogInputFiles(QWidget *parent = 0);
    ~DialogInputFiles();
    
private:
    Ui::DialogInputFiles *ui;
};

#endif // DIALOGINPUTFILES_H
