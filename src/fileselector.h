/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef FILESELECTOR_H
#define FILESELECTOR_H

#include <QWidget>

namespace Ui {
class FileSelector;
}

class FileSelector : public QWidget
{
    Q_OBJECT
    
public:
    explicit FileSelector(QWidget *parent = 0);
    ~FileSelector();
    
private:
    Ui::FileSelector *ui;
};

#endif // FILESELECTOR_H
