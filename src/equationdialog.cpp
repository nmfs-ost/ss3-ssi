#include "equationdialog.h"
#include "ui_equationdialog.h"

#include <QAbstractButton>
#include <cmath>

equationDialog::equationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::equationDialog)
{
    ui->setupUi(this);

    fleet = NULL;
    dataModel = NULL;

    min1 = -5; max1 = 5;
    min2 = -5; max2 = 5;
    min3 = -5; max3 = 5;
    min4 = -5; max4 = 5;
    min5 = -5; max5 = 5;
    min6 = -5; max6 = 5;
    min7 = -5; max7 = 5;
    min8 = -5; max8 = 5;
    ui->doubleSpinBox_1_min->setValue(-5);  ui->doubleSpinBox_1_max->setValue(5);
    ui->doubleSpinBox_2_min->setValue(-5);  ui->doubleSpinBox_2_max->setValue(5);
    ui->doubleSpinBox_3_min->setValue(-5);  ui->doubleSpinBox_3_max->setValue(5);
    ui->doubleSpinBox_4_min->setValue(-5);  ui->doubleSpinBox_4_max->setValue(5);
    ui->doubleSpinBox_5_min->setValue(-5);  ui->doubleSpinBox_5_max->setValue(5);
    ui->doubleSpinBox_6_min->setValue(-5);  ui->doubleSpinBox_6_max->setValue(5);
    ui->doubleSpinBox_7_min->setValue(-5);  ui->doubleSpinBox_7_max->setValue(5);
    ui->doubleSpinBox_8_min->setValue(-5);  ui->doubleSpinBox_8_max->setValue(5);
    value1Changed(0);
    value2Changed(0);
    value3Changed(0);
    value4Changed(0);
    value5Changed(0);
    value6Changed(0);
    value7Changed(0);
    value8Changed(0);

    join1 = join2 = join3 = 10;
    ui->spinBox_steep_join1->setValue(join1);
    ui->spinBox_steep_join2->setValue(join2);
    ui->spinBox_steep_join3->setValue(join3);
    binMin = 0; binMax = 20; binWidth = 2; binMidWidth = 0.5;
    ui->spinBox_bins_min->setValue(0);
    ui->spinBox_bins_max->setValue(20);
    ui->spinBox_bins_width->setValue(2);
    ui->doubleSpinBox_bins_midbin->setValue(binMidWidth);

    connect (ui->horizontalSlider_1, SIGNAL(valueChanged(int)), SLOT(slider1Changed(int)));
    connect (ui->doubleSpinBox_1_value, SIGNAL(valueChanged(double)), SLOT(value1Changed(double)));

    connect (ui->horizontalSlider_2, SIGNAL(valueChanged(int)), SLOT(slider2Changed(int)));
    connect (ui->doubleSpinBox_2_value, SIGNAL(valueChanged(double)), SLOT(value2Changed(double)));
    connect (ui->doubleSpinBox_2_min, SIGNAL(valueChanged(double)), SLOT(min2Changed(double)));
    connect (ui->doubleSpinBox_2_max, SIGNAL(valueChanged(double)), SLOT(max2Changed(double)));

    connect (ui->horizontalSlider_3, SIGNAL(valueChanged(int)), SLOT(slider3Changed(int)));
    connect (ui->doubleSpinBox_3_value, SIGNAL(valueChanged(double)), SLOT(value3Changed(double)));
    connect (ui->doubleSpinBox_3_min, SIGNAL(valueChanged(double)), SLOT(min3Changed(double)));
    connect (ui->doubleSpinBox_3_max, SIGNAL(valueChanged(double)), SLOT(max3Changed(double)));

    connect (ui->horizontalSlider_4, SIGNAL(valueChanged(int)), SLOT(slider4Changed(int)));
    connect (ui->doubleSpinBox_4_value, SIGNAL(valueChanged(double)), SLOT(value4Changed(double)));
    connect (ui->doubleSpinBox_4_min, SIGNAL(valueChanged(double)), SLOT(min4Changed(double)));
    connect (ui->doubleSpinBox_4_max, SIGNAL(valueChanged(double)), SLOT(max4Changed(double)));

    connect (ui->horizontalSlider_5, SIGNAL(valueChanged(int)), SLOT(slider5Changed(int)));
    connect (ui->doubleSpinBox_5_value, SIGNAL(valueChanged(double)), SLOT(value5Changed(double)));
    connect (ui->doubleSpinBox_5_min, SIGNAL(valueChanged(double)), SLOT(min5Changed(double)));
    connect (ui->doubleSpinBox_5_max, SIGNAL(valueChanged(double)), SLOT(max5Changed(double)));

    connect (ui->horizontalSlider_6, SIGNAL(valueChanged(int)), SLOT(slider6Changed(int)));
    connect (ui->doubleSpinBox_6_input, SIGNAL(valueChanged(double)), SLOT(value6Changed(double)));
    connect (ui->doubleSpinBox_6_min, SIGNAL(valueChanged(double)), SLOT(min6Changed(double)));
    connect (ui->doubleSpinBox_6_max, SIGNAL(valueChanged(double)), SLOT(max6Changed(double)));

    connect (ui->horizontalSlider_7, SIGNAL(valueChanged(int)), SLOT(slider7Changed(int)));
    connect (ui->doubleSpinBox_7_value, SIGNAL(valueChanged(double)), SLOT(value7Changed(double)));
    connect (ui->doubleSpinBox_7_min, SIGNAL(valueChanged(double)), SLOT(min7Changed(double)));
    connect (ui->doubleSpinBox_7_max, SIGNAL(valueChanged(double)), SLOT(max7Changed(double)));

    connect (ui->horizontalSlider_8, SIGNAL(valueChanged(int)), SLOT(slider8Changed(int)));
    connect (ui->doubleSpinBox_8_value, SIGNAL(valueChanged(double)), SLOT(value8Changed(double)));
    connect (ui->doubleSpinBox_8_min, SIGNAL(valueChanged(double)), SLOT(min8Changed(double)));
    connect (ui->doubleSpinBox_8_max, SIGNAL(valueChanged(double)), SLOT(max8Changed(double)));

    connect (ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), SLOT(buttonClicked(QAbstractButton*)));
}

equationDialog::~equationDialog()
{
    delete ui;
}

void equationDialog::setFleet (Fleet *flt)
{
    fleet = flt;

    if (dataModel != NULL)
        reset();
}

void equationDialog::setDataModel (ss_model *data)
{
    QStringList ages, lengths;

    dataModel = data;
    compositionAge *ageComp = dataModel->get_age_composition();
    if (ageComp == NULL)
        ages.clear();
    else
        ages = ageComp->getBins();
    compositionLength *lenComp = dataModel->get_length_composition();
    if (lenComp == NULL)
        lengths.clear();
    else
        lengths = lenComp->getBins();
}


void equationDialog::slider1Changed(int value)
{
    double val = (value / 1000.0) * (max1 - min1) + min1;
    if (val > max1)
        val = max1;
    if (val < min1)
        val = min1;
    ui->doubleSpinBox_1_value->setValue(val);
}

void equationDialog::value1Changed (double value)
{
    if (value > max1)
        value = max1;
    if (value < min1)
        value = min1;
    ui->doubleSpinBox_1_value->setValue(value);

    int val = (int)((value - min1)/(max1 - min1) * 1000);
    ui->horizontalSlider_1->setValue(val);
}

void equationDialog::slider2Changed(int value)
{
    double val = (value / 1000.0) * (max2 - min2) + min2;
    if (val > max2)
        val = max2;
    if (val < min2)
        val = min2;
    ui->doubleSpinBox_2_value->setValue(val);
}

void equationDialog::value2Changed (double value)
{
    if (value > max2)
        value = max2;
    if (value < min2)
        value = min2;
    ui->doubleSpinBox_2_value->setValue(value);

    int val = (int)((value - min2)/(max2 - min2) * 1000);
    ui->horizontalSlider_2->setValue(val);
}

void equationDialog::min2Changed (double value)
{
    min2 = value;
    ui->doubleSpinBox_2_value->setMinimum(value);
    value2Changed(ui->doubleSpinBox_2_value->value());
}

void equationDialog::max2Changed (double value)
{
    max2 = value;
    ui->doubleSpinBox_2_value->setMaximum(value);
    value2Changed(ui->doubleSpinBox_2_value->value());
}

void equationDialog::slider3Changed(int value)
{
    double val = (value / 1000.0) * (max3 - min3) + min3;
    if (val > max3)
        val = max3;
    if (val < min3)
        val = min3;
    ui->doubleSpinBox_3_value->setValue(val);
}

void equationDialog::value3Changed (double value)
{
    if (value > max3)
        value = max3;
    if (value < min3)
        value = min3;
    ui->doubleSpinBox_3_value->setValue(value);

    int val = (int)((value - min3)/(max3 - min3) * 1000);
    ui->horizontalSlider_3->setValue(val);
}

void equationDialog::min3Changed (double value)
{
    min3 = value;
    ui->doubleSpinBox_3_value->setMinimum(value);
    value3Changed(ui->doubleSpinBox_3_value->value());
}

void equationDialog::max3Changed (double value)
{
    max3 = value;
    ui->doubleSpinBox_3_value->setMaximum(value);
    value3Changed(ui->doubleSpinBox_3_value->value());
}

void equationDialog::slider4Changed(int value)
{
    double val = (value / 1000.0) * (max4 - min4) + min4;
    if (val > max4)
        val = max4;
    if (val < min4)
        val = min4;
    ui->doubleSpinBox_4_value->setValue(val);
}

void equationDialog::value4Changed (double value)
{
    if (value > max4)
        value = max4;
    if (value < min4)
        value = min4;
    ui->doubleSpinBox_4_value->setValue(value);

    int val = (int)((value - min4)/(max4 - min4) * 1000);
    ui->horizontalSlider_4->setValue(val);
}

void equationDialog::min4Changed (double value)
{
    min4 = value;
    ui->doubleSpinBox_4_value->setMinimum(value);
    value4Changed(ui->doubleSpinBox_4_value->value());
}

void equationDialog::max4Changed (double value)
{
    max4 = value;
    ui->doubleSpinBox_4_value->setMaximum(value);
    value4Changed(ui->doubleSpinBox_4_value->value());
}

void equationDialog::slider5Changed(int value)
{
    double val = (value / 1000.0) * (max5 - min5) + min5;
    if (val > max5)
        val = max5;
    if (val < min5)
        val = min5;
    ui->doubleSpinBox_5_value->setValue(val);
}

void equationDialog::value5Changed (double value)
{
    if (value > max5)
        value = max5;
    if (value < min5)
        value = min5;
    ui->doubleSpinBox_5_value->setValue(value);

    int val = (int)((value - min5)/(max5 - min5) * 1000);
    ui->horizontalSlider_5->setValue(val);
}

void equationDialog::min5Changed (double value)
{
    min5 = value;
    ui->doubleSpinBox_5_value->setMinimum(value);
    value5Changed(ui->doubleSpinBox_5_value->value());
}

void equationDialog::max5Changed (double value)
{
    max5 = value;
    ui->doubleSpinBox_5_value->setMaximum(value);
    value5Changed(ui->doubleSpinBox_5_value->value());
}


void equationDialog::slider6Changed(int value)
{
    double val = (value / 1000.0) * (max6 - min6) + min6;
    if (val > max6)
        val = max6;
    if (val < min6)
        val = min6;
    ui->doubleSpinBox_6_value->setValue(val);
}

void equationDialog::value6Changed (double value)
{
    if (value > max6)
        value = max6;
    if (value < min6)
        value = min6;
    ui->doubleSpinBox_6_value->setValue(value);

    int val = (int)((value - min6)/(max6 - min6) * 1000);
    ui->horizontalSlider_6->setValue(val);
}

void equationDialog::min6Changed (double value)
{
    min6 = value;
    ui->doubleSpinBox_6_value->setMinimum(value);
    value6Changed(ui->doubleSpinBox_6_value->value());
}

void equationDialog::max6Changed (double value)
{
    max6 = value;
    ui->doubleSpinBox_6_value->setMaximum(value);
    value6Changed(ui->doubleSpinBox_6_value->value());
}



void equationDialog::slider7Changed(int value)
{
    double val = (value / 1000.0) * (max7 - min7) + min7;
    if (val > max7)
        val = max7;
    if (val < min7)
        val = min7;
    ui->doubleSpinBox_7_value->setValue(val);
}


void equationDialog::value7Changed (double value)
{
    if (value > max7)
        value = max7;
    if (value < min7)
        value = min7;
    ui->doubleSpinBox_7_value->setValue(value);

    int val = (int)((value - min7)/(max7 - min7) * 1000);
    ui->horizontalSlider_7->setValue(val);
}


void equationDialog::min7Changed (double value)
{
    min7 = value;
    ui->doubleSpinBox_7_value->setMinimum(value);
    value7Changed(ui->doubleSpinBox_7_value->value());
}

void equationDialog::max7Changed (double value)
{
    max7 = value;
    ui->doubleSpinBox_7_value->setMaximum(value);
    value7Changed(ui->doubleSpinBox_7_value->value());
}



void equationDialog::slider8Changed(int value)
{
    double val = (value / 1000.0) * (max8 - min8) + min8;
    if (val > max8)
        val = max8;
    if (val < min8)
        val = min8;
    ui->doubleSpinBox_8_value->setValue(val);
}

void equationDialog::value8Changed (double value)
{
    if (value > max8)
        value = max8;
    if (value < min8)
        value = min8;
    ui->doubleSpinBox_8_value->setValue(value);

    int val = (int)((value - min8)/(max8 - min8) * 1000);
    ui->horizontalSlider_8->setValue(val);
}

void equationDialog::min8Changed (double value)
{
    min8 = value;
    ui->doubleSpinBox_8_value->setMinimum(value);
    value8Changed(ui->doubleSpinBox_8_value->value());
}

void equationDialog::max8Changed (double value)
{
    max8 = value;
    ui->doubleSpinBox_8_value->setMaximum(value);
    value8Changed(ui->doubleSpinBox_8_value->value());
}

void equationDialog::binMinChanged (int value)
{
    binMin = value;
}

void equationDialog::binMaxChanged (int value)
{
    binMax = value;
}

void equationDialog::binWidthChanged (int value)
{
    binWidth = value;
}

void equationDialog::binMidChanged (double value)
{
    binMidWidth = value;
}

void equationDialog::join1Changed (int value)
{
    join1 = value;
}

void equationDialog::join2Changed (int value)
{
    join2 = value;
}

void equationDialog::join3Changed (int value)
{
    join3 = value;
}

void equationDialog::apply()
{
    // write values to parameters
    ui->doubleSpinBox_1_input->setValue(ui->doubleSpinBox_1_value->value());
    ui->doubleSpinBox_2_input->setValue(ui->doubleSpinBox_2_value->value());
    ui->doubleSpinBox_3_input->setValue(ui->doubleSpinBox_3_value->value());
    ui->doubleSpinBox_4_input->setValue(ui->doubleSpinBox_4_value->value());
    ui->doubleSpinBox_5_input->setValue(ui->doubleSpinBox_5_value->value());
    ui->doubleSpinBox_6_input->setValue(ui->doubleSpinBox_6_value->value());
    ui->doubleSpinBox_7_input->setValue(ui->doubleSpinBox_7_value->value());
    ui->doubleSpinBox_8_input->setValue(ui->doubleSpinBox_8_value->value());
    // writeParameters();
}

void equationDialog::reset()
{
    // get values from parameters
    value1Changed(ui->doubleSpinBox_1_input->value());
    value2Changed(ui->doubleSpinBox_2_input->value());
    value3Changed(ui->doubleSpinBox_3_input->value());
    value4Changed(ui->doubleSpinBox_4_input->value());
    value5Changed(ui->doubleSpinBox_5_input->value());
    value6Changed(ui->doubleSpinBox_6_input->value());
    value7Changed(ui->doubleSpinBox_7_input->value());
    value8Changed(ui->doubleSpinBox_8_input->value());
}

void equationDialog::restore()
{

}

void equationDialog::close()
{
    emit closed();
    hide();
}

void equationDialog::buttonClicked(QAbstractButton *btn)
{
    if (btn->text().contains("Apply"))
        apply();
    else if (btn->text().contains("Reset"))
        reset();
    else if (btn->text().contains("Restore"))
        restore();
    else if (btn->text().contains("Close"))
        close();
}

void equationDialog::closeEvent(QCloseEvent *event)
{
    emit closed();
}

void equationDialog::parametersChanged()
{

}

void equationDialog::setupChanged()
{

}
