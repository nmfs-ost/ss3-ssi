#include "doublelimitspinbox.h"
//#include "ui_doublelimitspinbox.h"
#include <cmath>

doubleLimitSpinBox::doubleLimitSpinBox(QWidget *parent) :
    QDoubleSpinBox (parent)//,
//    ui(new Ui::doubleLimitSpinBox)
{
//    ui->setupUi(this);
    connect (this, SIGNAL(valueChanged(double)), SLOT(setRangeValue(double)));
}

doubleLimitSpinBox::~doubleLimitSpinBox()
{
//    delete ui;
}

void doubleLimitSpinBox::setRangeValue(double value)
{
    double limitValue = (fabs(value) + 1) * 10;
    double min = value - limitValue;
    double max = limitValue;

    QDoubleSpinBox::setRange(min, max);
}
