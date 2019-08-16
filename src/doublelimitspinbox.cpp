#include "doublelimitspinbox.h"
#include "ui_doublelimitspinbox.h"

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
    double limitValue = value * 10;

    if (value > 0)
        QDoubleSpinBox::setRange(0, (limitValue + 10));
    else if (value < 0)
        QDoubleSpinBox::setRange((limitValue - 10), 0);
    else
        QDoubleSpinBox::setRange(-11, 11);
}
