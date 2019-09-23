#ifndef DOUBLELIMITSPINBOX_H
#define DOUBLELIMITSPINBOX_H

#include <QWidget>
#include <QDoubleSpinBox>

/*namespace Ui {
class doubleLimitSpinBox;
}*/

class doubleLimitSpinBox : public QDoubleSpinBox
{
    Q_OBJECT

public:
    explicit doubleLimitSpinBox(QWidget *parent = nullptr);
    ~doubleLimitSpinBox();

public slots:
    void setRange(double value);

/*private:
    Ui::doubleLimitSpinBox *ui;*/
};

#endif // DOUBLELIMITSPINBOX_H
