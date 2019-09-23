#include "doublelimitspinbox.h"
//#include "ui_doublelimitspinbox.h"
#include <cmath>

doubleLimitSpinBox::doubleLimitSpinBox(QWidget *parent) :
    QDoubleSpinBox (parent)//,
//    ui(new Ui::doubleLimitSpinBox)
{
//    ui->setupUi(this);
    connect (this, SIGNAL(valueChanged(double)), SLOT(setRange(double)));
}

doubleLimitSpinBox::~doubleLimitSpinBox()
{
//    delete ui;
}

void doubleLimitSpinBox::setRange(double value)
{
    double limitValue = fabs(value) + 10;
    double min = value - limitValue;
    double max = value + limitValue;

    QDoubleSpinBox::setRange(min, max);
}
