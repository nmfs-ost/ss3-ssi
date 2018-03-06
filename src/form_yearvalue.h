/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef FORM_YEARVALUE_H
#define FORM_YEARVALUE_H

#include <QWidget>

namespace Ui {
class Form_YearValue;
}

class Form_YearValue : public QWidget
{
    Q_OBJECT
    
public:
    explicit Form_YearValue(QWidget *parent = 0);
    ~Form_YearValue();
    
private:
    Ui::Form_YearValue *ui;
};

#endif // FORM_YEARVALUE_H
