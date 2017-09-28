#include "equationdialog.h"
#include "ui_equationdialog.h"

#include <QAbstractButton>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

#include <cmath>

equationDialog::equationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::equationDialog)
{
    ui->setupUi(this);

//    title = new QLabel("None", this);

    selex = NULL;
    dataModel = NULL;

    equationNum = 0;
    parameters = NULL;

    chartview = new QChartView(this);
    cht = new QChart();
    firstSeries = new QLineSeries(cht);
    lastSeries = new QLineSeries(cht);
    selSeries = new QLineSeries(cht);
    selSeries->append(0, 0);
    cht->addSeries(selSeries);
    chartview->setChart(cht);
    ui->verticalLayout_graph->addWidget(chartview, 1);
    axisXsel = new QValueAxis();
    axisYalt = new QValueAxis();
    axisY = new QValueAxis();

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
    ui->spinBox_bins_min->setValue(binMin);
    ui->spinBox_bins_max->setValue(binMax);
    ui->spinBox_bins_width->setValue(binWidth);
    ui->doubleSpinBox_bins_midbin->setValue(binMidWidth);

    connect (ui->spinBox_bins_min, SIGNAL(valueChanged(int)), SLOT(binMinChanged(int)));
    connect (ui->spinBox_bins_max, SIGNAL(valueChanged(int)), SLOT(binMaxChanged(int)));
    connect (ui->spinBox_bins_width, SIGNAL(valueChanged(int)), SLOT(binWidthChanged(int)));
    connect (ui->doubleSpinBox_bins_midbin, SIGNAL(valueChanged(double)), SLOT(binMidChanged(double)));

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

void equationDialog::setSelex (selectivity *slx)
{
    selex = slx;
    special = selex->getSpecial();
    equationNum = selex->getPattern();
    parameters = selex->getParameterModel();
//    setSpecial(selex->getSpecial());
//    setEquationNumber(selex->getPattern());
//    setParameters(slx->getParameterModel());

//    if (dataModel != NULL)
        restoreAll();
    update();
}

void equationDialog::setDataModel (ss_model *data)
{
    dataModel = data;
//    setLengths();
//    setAges();
}

void equationDialog::setXvals(QStringList vals)
{
    xValList.clear();

    for (int i = 0; i < vals.count(); i++)
        xValList.append(QString(vals.at(i)).toFloat());

    binMin = xValList.at(0);
    binMax = xValList.at(xValList.count() - 1);
    binWidth = xValList.at(2) - xValList.at(1);
    refresh();
}

void equationDialog::setMidBin(float val)
{
    binMidWidth = val;
}

/*void equationDialog::setAges()
{
    QStringList ages;
    compositionAge *ageComp = dataModel->get_age_composition();
    if (ageComp == NULL)
        ages.clear();
    else
        ages = ageComp->getBins();
    ageList.clear();
    for (int i = 0; i < ages.count(); i++)
        ageList.append(QString(ages.at(i)).toFloat());
}*/

void equationDialog::setEquationNumber(int num)
{
    equationNum = num;
    update();
//    parameters = NULL;
}

void equationDialog::setParameters(tablemodel *params)
{
    parameters = params;
    refresh();
}

void equationDialog::getParameterValues()
{
    double val;
    QStringList values;
    if (parameters->rowCount() > 0)
    {
        values = parameters->getRowData(0);
        val = (values.at(0)).toDouble();
        ui->doubleSpinBox_1_min->setValue(val);
        val = (values.at(1)).toDouble();
        ui->doubleSpinBox_1_max->setValue(val);
        val = (values.at(2)).toDouble();
        ui->doubleSpinBox_1_value->setValue(val);
    }
    if (parameters->rowCount() > 1)
    {
        values = parameters->getRowData(1);
        val = (values.at(0)).toDouble();
        ui->doubleSpinBox_2_min->setValue(val);
        val = (values.at(1)).toDouble();
        ui->doubleSpinBox_2_max->setValue(val);
        val = (values.at(2)).toDouble();
        ui->doubleSpinBox_2_value->setValue(val);
    }
}

void equationDialog::setParameterValues()
{
    QStringList values;
    if (parameters->rowCount() > 0)
    {
        values = parameters->getRowData(0);
        values[0] = QString::number(ui->doubleSpinBox_1_min->value());
        values[1] = QString::number(ui->doubleSpinBox_1_max->value());
        values[2] = QString::number(ui->doubleSpinBox_1_input->value());
        parameters->setRowData(0, values);
    }
    if (parameters->rowCount() > 1)
    {
        values = parameters->getRowData(1);
        values[0] = QString::number(ui->doubleSpinBox_2_min->value());
        values[1] = QString::number(ui->doubleSpinBox_2_max->value());
        values[2] = QString::number(ui->doubleSpinBox_2_input->value());
        parameters->setRowData(1, values);
    }
}

void equationDialog::refresh()
{
    ui->spinBox_bins_min->setValue(binMin);
    ui->spinBox_bins_max->setValue(binMax);
    ui->spinBox_bins_width->setValue(binWidth);
    ui->doubleSpinBox_bins_midbin->setValue(binMidWidth);
    restoreAll();
    update();
}

void equationDialog::update()
{
    QStringList templist;
    float min, max, init;
    switch (equationNum) {
    case 0:
    case 10:
        constant(1.0);
        break;
    case 1:
    case 12:
        if (parameters != NULL && parameters->rowCount() == 2)
        {
            templist = parameters->getRowData(0);
            min = QString(templist.at(0)).toFloat();
            max = QString(templist.at(1)).toFloat();
            init = QString(templist.at(2)).toFloat();
            setSlider1(min, max, init);
            templist = parameters->getRowData(1);
            min = QString(templist.at(0)).toFloat();
            max = QString(templist.at(1)).toFloat();
            init = QString(templist.at(2)).toFloat();
            setSlider2(min, max, init);
            logistic();
        }
        break;
//    case 6:
        linear();
        break;
    default:
        blank();
        break;
    }

}

void equationDialog::setSlider1(float min, float max, float init)
{
    min1 = min; max1 = max;
    ui->doubleSpinBox_1_min->setValue(min);
    ui->doubleSpinBox_1_max->setValue(max);
    ui->doubleSpinBox_1_input->setValue(init);
    ui->doubleSpinBox_1_value->setValue(init);
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

void equationDialog::min1Changed (double value)
{
    min1 = value;
    ui->doubleSpinBox_1_value->setMinimum(value);
}

void equationDialog::max1Changed (double value)
{
    max1 = value;
    ui->doubleSpinBox_1_value->setMaximum(value);
}

void equationDialog::setSlider2(float min, float max, float init)
{
    min2 = min; max2 = max;
    ui->doubleSpinBox_2_min->setValue(min);
    ui->doubleSpinBox_2_max->setValue(max);
    ui->doubleSpinBox_2_input->setValue(init);
    ui->doubleSpinBox_2_value->setValue(init);
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
//    ui->doubleSpinBox_2_value->setValue(value);

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
}

void equationDialog::setSlider3(float min, float max, float init)
{
    min3 = min; max3 = max;
    ui->doubleSpinBox_3_min->setValue(min);
    ui->doubleSpinBox_3_max->setValue(max);
    ui->doubleSpinBox_3_input->setValue(init);
    ui->doubleSpinBox_3_value->setValue(init);
}

void equationDialog::slider3Changed(int value)
{
    double val = (value / 1000.0) * (max3 - min3) + min3;
/*    if (val > max3)
        val = max3;
    if (val < min3)
        val = min3;*/
    ui->doubleSpinBox_3_value->setValue(val);
}

void equationDialog::value3Changed (double value)
{
/*    if (value > max3)
        value = max3;
    if (value < min3)
        value = min3;
    ui->doubleSpinBox_3_value->setValue(value);*/

    int val = (int)((value - min3)/(max3 - min3) * 1000);
    ui->horizontalSlider_3->setValue(val);
}

void equationDialog::min3Changed (double value)
{
    min3 = value;
    ui->doubleSpinBox_3_value->setMinimum(value);
}

void equationDialog::max3Changed (double value)
{
    max3 = value;
    ui->doubleSpinBox_3_value->setMaximum(value);
}

void equationDialog::setSlider4(float min, float max, float init)
{
    min4 = min; max4 = max;
    ui->doubleSpinBox_4_min->setValue(min);
    ui->doubleSpinBox_4_max->setValue(max);
    ui->doubleSpinBox_4_input->setValue(init);
    ui->doubleSpinBox_4_value->setValue(init);
}

void equationDialog::slider4Changed(int value)
{
    double val = (value / 1000.0) * (max4 - min4) + min4;
/*    if (val > max4)
        val = max4;
    if (val < min4)
        val = min4;*/
    ui->doubleSpinBox_4_value->setValue(val);
}

void equationDialog::value4Changed (double value)
{
/*    if (value > max4)
        value = max4;
    if (value < min4)
        value = min4;
    ui->doubleSpinBox_4_value->setValue(value);*/

    int val = (int)((value - min4)/(max4 - min4) * 1000);
    ui->horizontalSlider_4->setValue(val);
}

void equationDialog::min4Changed (double value)
{
    min4 = value;
    ui->doubleSpinBox_4_value->setMinimum(value);
}

void equationDialog::max4Changed (double value)
{
    max4 = value;
    ui->doubleSpinBox_4_value->setMaximum(value);
}

void equationDialog::setSlider5(float min, float max, float init)
{
    min5 = min; max5 = max;
    ui->doubleSpinBox_5_min->setValue(min);
    ui->doubleSpinBox_5_max->setValue(max);
    ui->doubleSpinBox_5_input->setValue(init);
    ui->doubleSpinBox_5_value->setValue(init);
}

void equationDialog::slider5Changed(int value)
{
    double val = (value / 1000.0) * (max5 - min5) + min5;
/*    if (val > max5)
        val = max5;
    if (val < min5)
        val = min5;*/
    ui->doubleSpinBox_5_value->setValue(val);
}

void equationDialog::value5Changed (double value)
{
/*    if (value > max5)
        value = max5;
    if (value < min5)
        value = min5;
    ui->doubleSpinBox_5_value->setValue(value);*/

    int val = (int)((value - min5)/(max5 - min5) * 1000);
    ui->horizontalSlider_5->setValue(val);
}

void equationDialog::min5Changed (double value)
{
    min5 = value;
    ui->doubleSpinBox_5_value->setMinimum(value);
}

void equationDialog::max5Changed (double value)
{
    max5 = value;
    ui->doubleSpinBox_5_value->setMaximum(value);
}


void equationDialog::setSlider6(float min, float max, float init)
{
    min6 = min; max6 = max;
    ui->doubleSpinBox_6_min->setValue(min);
    ui->doubleSpinBox_6_max->setValue(max);
    ui->doubleSpinBox_6_input->setValue(init);
    ui->doubleSpinBox_6_value->setValue(init);
}

void equationDialog::slider6Changed(int value)
{
    double val = (value / 1000.0) * (max6 - min6) + min6;
/*    if (val > max6)
        val = max6;
    if (val < min6)
        val = min6;*/
    ui->doubleSpinBox_6_value->setValue(val);
}

void equationDialog::value6Changed (double value)
{
/*    if (value > max6)
        value = max6;
    if (value < min6)
        value = min6;
    ui->doubleSpinBox_6_value->setValue(value);*/

    int val = (int)((value - min6)/(max6 - min6) * 1000);
    ui->horizontalSlider_6->setValue(val);
}

void equationDialog::min6Changed (double value)
{
    min6 = value;
    ui->doubleSpinBox_6_value->setMinimum(value);
}

void equationDialog::max6Changed (double value)
{
    max6 = value;
    ui->doubleSpinBox_6_value->setMaximum(value);
}


void equationDialog::setSlider7(float min, float max, float init)
{
    min7 = min; max7 = max;
    ui->doubleSpinBox_7_min->setValue(min);
    ui->doubleSpinBox_7_max->setValue(max);
    ui->doubleSpinBox_7_input->setValue(init);
    ui->doubleSpinBox_7_value->setValue(init);
}

void equationDialog::slider7Changed(int value)
{
    double val = (value / 1000.0) * (max7 - min7) + min7;
/*    if (val > max7)
        val = max7;
    if (val < min7)
        val = min7;*/
    ui->doubleSpinBox_7_value->setValue(val);
}


void equationDialog::value7Changed (double value)
{
/*    if (value > max7)
        value = max7;
    if (value < min7)
        value = min7;
    ui->doubleSpinBox_7_value->setValue(value);*/

    int val = (int)((value - min7)/(max7 - min7) * 1000);
    ui->horizontalSlider_7->setValue(val);
}


void equationDialog::min7Changed (double value)
{
    min7 = value;
    ui->doubleSpinBox_7_value->setMinimum(value);
}

void equationDialog::max7Changed (double value)
{
    max7 = value;
    ui->doubleSpinBox_7_value->setMaximum(value);
}


void equationDialog::setSlider8(float min, float max, float init)
{
    min8 = min; max8 = max;
    ui->doubleSpinBox_8_min->setValue(min);
    ui->doubleSpinBox_8_max->setValue(max);
    ui->doubleSpinBox_8_input->setValue(init);
    ui->doubleSpinBox_8_value->setValue(init);
}

void equationDialog::slider8Changed(int value)
{
    double val = (value / 1000.0) * (max8 - min8) + min8;
/*    if (val > max8)
        val = max8;
    if (val < min8)
        val = min8;*/
    ui->doubleSpinBox_8_value->setValue(val);
}

void equationDialog::value8Changed (double value)
{
/*    if (value > max8)
        value = max8;
    if (value < min8)
        value = min8;
    ui->doubleSpinBox_8_value->setValue(value);*/

    int val = (int)((value - min8)/(max8 - min8) * 1000);
    ui->horizontalSlider_8->setValue(val);
}

void equationDialog::min8Changed (double value)
{
    min8 = value;
    ui->doubleSpinBox_8_value->setMinimum(value);
}

void equationDialog::max8Changed (double value)
{
    max8 = value;
    ui->doubleSpinBox_8_value->setMaximum(value);
}

void equationDialog::binMinChanged (int value)
{
    binMin = value;
    update();
}

void equationDialog::binMaxChanged (int value)
{
    binMax = value;
    update();
}

void equationDialog::binWidthChanged (int value)
{
    binWidth = value;
    update();
}

void equationDialog::binMidChanged (double value)
{
    binMidWidth = value;
    update();
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
    // set input spinboxes
    ui->doubleSpinBox_1_input->setValue(ui->doubleSpinBox_1_value->value());
    ui->doubleSpinBox_2_input->setValue(ui->doubleSpinBox_2_value->value());
    ui->doubleSpinBox_3_input->setValue(ui->doubleSpinBox_3_value->value());
    ui->doubleSpinBox_4_input->setValue(ui->doubleSpinBox_4_value->value());
    ui->doubleSpinBox_5_input->setValue(ui->doubleSpinBox_5_value->value());
    ui->doubleSpinBox_6_input->setValue(ui->doubleSpinBox_6_value->value());
    ui->doubleSpinBox_7_input->setValue(ui->doubleSpinBox_7_value->value());
    ui->doubleSpinBox_8_input->setValue(ui->doubleSpinBox_8_value->value());
    // write values to parameters
    setParameterValues();

}

void equationDialog::resetValues()
{
    // get values from input spinboxes
    ui->doubleSpinBox_1_value->setValue(ui->doubleSpinBox_1_input->value());
    ui->doubleSpinBox_2_value->setValue(ui->doubleSpinBox_2_input->value());
    ui->doubleSpinBox_3_value->setValue(ui->doubleSpinBox_3_input->value());
    ui->doubleSpinBox_4_value->setValue(ui->doubleSpinBox_4_input->value());
    ui->doubleSpinBox_5_value->setValue(ui->doubleSpinBox_5_input->value());
    ui->doubleSpinBox_6_value->setValue(ui->doubleSpinBox_6_input->value());
    ui->doubleSpinBox_7_value->setValue(ui->doubleSpinBox_7_input->value());
    ui->doubleSpinBox_8_value->setValue(ui->doubleSpinBox_8_input->value());
    update();
}

void equationDialog::restoreAll()
{
    if (!xValList.isEmpty())
    {
        // get bin values
        binMin = xValList.at(0);
        binMax = xValList.at(xValList.count() - 1);
        binWidth = xValList.at(2) - xValList.at(1);
    }
    binMidWidth = fleet->getSeasTiming();
    ui->spinBox_bins_min->setValue(binMin);
    ui->spinBox_bins_max->setValue(binMax);
    ui->spinBox_bins_width->setValue(binWidth);
    ui->doubleSpinBox_bins_midbin->setValue(binMidWidth);
    // get values from parameters
    if (parameters != NULL)
        getParameterValues();
    resetValues ();
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
        resetValues();
    else if (btn->text().contains("Restore"))
        restoreAll();
    else if (btn->text().contains("Close"))
        close();
}

void equationDialog::closeEvent(QCloseEvent *event)
{
    emit closed();
}

void equationDialog::parametersChanged()
{
    refresh();
}

void equationDialog::setupChanged()
{

    refresh();
}

Fleet *equationDialog::getFleet() const
{
    return fleet;
}

void equationDialog::setFleet(Fleet *value)
{
    fleet = value;
}

int equationDialog::getSpecial() const
{
    return special;
}

void equationDialog::setSpecial(int value)
{
    special = value;
    update();
}

void equationDialog::showSliders(int num)
{
    ui->label_name->setVisible(false);
    ui->label_value->setVisible(false);
    ui->label_type->setVisible(false);
    ui->label_min->setVisible(false);
    ui->label_max->setVisible(false);
    ui->label_trans->setVisible(false);
    ui->label_input->setVisible(false);
    ui->label_sliders->setVisible(false);

    ui->label_1_name->setVisible(false);
    ui->doubleSpinBox_1_value->setVisible(false);
    ui->label_1_type->setVisible(false);
    ui->doubleSpinBox_1_trans->setVisible(false);
    ui->doubleSpinBox_1_min->setVisible(false);
    ui->doubleSpinBox_1_max->setVisible(false);
    ui->doubleSpinBox_1_input->setVisible(false);
    ui->horizontalSlider_1->setVisible(false);

    ui->label_2_name->setVisible(false);
    ui->doubleSpinBox_2_value->setVisible(false);
    ui->label_2_type->setVisible(false);
    ui->doubleSpinBox_2_trans->setVisible(false);
    ui->doubleSpinBox_2_min->setVisible(false);
    ui->doubleSpinBox_2_max->setVisible(false);
    ui->doubleSpinBox_2_input->setVisible(false);
    ui->horizontalSlider_2->setVisible(false);

    ui->label_3_name->setVisible(false);
    ui->doubleSpinBox_3_value->setVisible(false);
    ui->label_3_type->setVisible(false);
    ui->doubleSpinBox_3_trans->setVisible(false);
    ui->doubleSpinBox_3_min->setVisible(false);
    ui->doubleSpinBox_3_max->setVisible(false);
    ui->doubleSpinBox_3_input->setVisible(false);
    ui->horizontalSlider_3->setVisible(false);

    ui->label_4_name->setVisible(false);
    ui->doubleSpinBox_4_value->setVisible(false);
    ui->label_4_type->setVisible(false);
    ui->doubleSpinBox_4_trans->setVisible(false);
    ui->doubleSpinBox_4_min->setVisible(false);
    ui->doubleSpinBox_4_max->setVisible(false);
    ui->doubleSpinBox_4_input->setVisible(false);
    ui->horizontalSlider_4->setVisible(false);

    ui->label_5_name->setVisible(false);
    ui->doubleSpinBox_5_value->setVisible(false);
    ui->label_5_type->setVisible(false);
    ui->doubleSpinBox_5_trans->setVisible(false);
    ui->doubleSpinBox_5_min->setVisible(false);
    ui->doubleSpinBox_5_max->setVisible(false);
    ui->doubleSpinBox_5_input->setVisible(false);
    ui->horizontalSlider_5->setVisible(false);

    ui->label_6_name->setVisible(false);
    ui->doubleSpinBox_6_value->setVisible(false);
    ui->label_6_type->setVisible(false);
    ui->doubleSpinBox_6_trans->setVisible(false);
    ui->doubleSpinBox_6_min->setVisible(false);
    ui->doubleSpinBox_6_max->setVisible(false);
    ui->doubleSpinBox_6_input->setVisible(false);
    ui->horizontalSlider_6->setVisible(false);

    ui->label_7_name->setVisible(false);
    ui->doubleSpinBox_7_value->setVisible(false);
    ui->label_7_type->setVisible(false);
    ui->doubleSpinBox_7_trans->setVisible(false);
    ui->doubleSpinBox_7_min->setVisible(false);
    ui->doubleSpinBox_7_max->setVisible(false);
    ui->doubleSpinBox_7_input->setVisible(false);
    ui->horizontalSlider_7->setVisible(false);

    ui->label_8_name->setVisible(false);
    ui->doubleSpinBox_8_value->setVisible(false);
    ui->label_8_type->setVisible(false);
    ui->doubleSpinBox_8_trans->setVisible(false);
    ui->doubleSpinBox_8_min->setVisible(false);
    ui->doubleSpinBox_8_max->setVisible(false);
    ui->doubleSpinBox_8_input->setVisible(false);
    ui->horizontalSlider_8->setVisible(false);

    if (num > 0)
    {
        ui->label_name->setVisible(true);
        ui->label_value->setVisible(true);
        ui->label_type->setVisible(true);
        ui->label_min->setVisible(true);
        ui->label_max->setVisible(true);
        ui->label_trans->setVisible(true);
        ui->label_input->setVisible(true);
        ui->label_sliders->setVisible(true);

        ui->label_1_name->setVisible(true);
        ui->doubleSpinBox_1_value->setVisible(true);
        ui->label_1_type->setVisible(true);
        ui->doubleSpinBox_1_trans->setVisible(true);
        ui->doubleSpinBox_1_min->setVisible(true);
        ui->doubleSpinBox_1_max->setVisible(true);
        ui->doubleSpinBox_1_input->setVisible(true);
        ui->horizontalSlider_1->setVisible(true);
    }
    if (num > 1)
    {
        ui->label_2_name->setVisible(true);
        ui->doubleSpinBox_2_value->setVisible(true);
        ui->label_2_type->setVisible(true);
        ui->doubleSpinBox_2_trans->setVisible(true);
        ui->doubleSpinBox_2_min->setVisible(true);
        ui->doubleSpinBox_2_max->setVisible(true);
        ui->doubleSpinBox_2_input->setVisible(true);
        ui->horizontalSlider_2->setVisible(true);
    }
    if (num > 2)
    {
        ui->label_3_name->setVisible(true);
        ui->doubleSpinBox_3_value->setVisible(true);
        ui->label_3_type->setVisible(true);
        ui->doubleSpinBox_3_trans->setVisible(true);
        ui->doubleSpinBox_3_min->setVisible(true);
        ui->doubleSpinBox_3_max->setVisible(true);
        ui->doubleSpinBox_3_input->setVisible(true);
        ui->horizontalSlider_3->setVisible(true);
    }
    if (num > 3)
    {
        ui->label_4_name->setVisible(true);
        ui->doubleSpinBox_4_value->setVisible(true);
        ui->label_4_type->setVisible(true);
        ui->doubleSpinBox_4_trans->setVisible(true);
        ui->doubleSpinBox_4_min->setVisible(true);
        ui->doubleSpinBox_4_max->setVisible(true);
        ui->doubleSpinBox_4_input->setVisible(true);
        ui->horizontalSlider_4->setVisible(true);
    }
    if (num > 4)
    {
        ui->label_5_name->setVisible(true);
        ui->doubleSpinBox_5_value->setVisible(true);
        ui->label_5_type->setVisible(true);
        ui->doubleSpinBox_5_trans->setVisible(true);
        ui->doubleSpinBox_5_min->setVisible(true);
        ui->doubleSpinBox_5_max->setVisible(true);
        ui->doubleSpinBox_5_input->setVisible(true);
        ui->horizontalSlider_5->setVisible(true);
    }
    if (num > 5)
    {
        ui->label_6_name->setVisible(true);
        ui->doubleSpinBox_6_value->setVisible(true);
        ui->label_6_type->setVisible(true);
        ui->doubleSpinBox_6_trans->setVisible(true);
        ui->doubleSpinBox_6_min->setVisible(true);
        ui->doubleSpinBox_6_max->setVisible(true);
        ui->doubleSpinBox_6_input->setVisible(true);
        ui->horizontalSlider_6->setVisible(true);
    }
    if (num > 6)
    {
        ui->label_7_name->setVisible(true);
        ui->doubleSpinBox_7_value->setVisible(true);
        ui->label_7_type->setVisible(true);
        ui->doubleSpinBox_7_trans->setVisible(true);
        ui->doubleSpinBox_7_min->setVisible(true);
        ui->doubleSpinBox_7_max->setVisible(true);
        ui->doubleSpinBox_7_input->setVisible(true);
        ui->horizontalSlider_7->setVisible(true);
    }
    if (num > 7)
    {
        ui->label_8_name->setVisible(true);
        ui->doubleSpinBox_8_value->setVisible(true);
        ui->label_8_type->setVisible(true);
        ui->doubleSpinBox_8_trans->setVisible(true);
        ui->doubleSpinBox_8_min->setVisible(true);
        ui->doubleSpinBox_8_max->setVisible(true);
        ui->doubleSpinBox_8_input->setVisible(true);
        ui->horizontalSlider_8->setVisible(true);
    }

}

void equationDialog::showBins(bool flag)
{
    ui->label_bins->setVisible(flag);
    ui->label_bins_min->setVisible(flag);
    ui->label_bins_max->setVisible(flag);
    ui->label_bins_width->setVisible(flag);
    ui->label_bins_midbin->setVisible(flag);
    ui->spinBox_bins_min->setVisible(flag);
    ui->spinBox_bins_max->setVisible(flag);
    ui->spinBox_bins_width->setVisible(flag);
    ui->doubleSpinBox_bins_midbin->setVisible(flag);
}

void equationDialog::showJoins(int num)
{
    ui->label_steepness->setVisible(false);
    ui->label_steep_join1->setVisible(false);
    ui->label_steep_join2->setVisible(false);
    ui->label_steep_join3->setVisible(false);
    ui->spinBox_steep_join1->setVisible(false);
    ui->spinBox_steep_join2->setVisible(false);
    ui->spinBox_steep_join3->setVisible(false);

    switch (num)
    {
    case 3:
        ui->label_steep_join3->setVisible(false);
        ui->spinBox_steep_join3->setVisible(false);

    case 2:
        ui->label_steep_join2->setVisible(false);
        ui->spinBox_steep_join2->setVisible(false);

    case 1:
        ui->label_steepness->setVisible(true);
        ui->label_steep_join1->setVisible(true);
        ui->spinBox_steep_join1->setVisible(true);
        break;
//    case 0:        // done above, show none
//    default:
    }
}

void equationDialog::blank ()
{
    // This is for equations not yet implemented.
    // INTENTIONALLY LEFT BLANK
    QString msg ("This equation is not yet implemented.\n\n THIS PAGE INTENTIONALLY LEFT BANK");
    ui->label_title->setText(msg);
    showSliders(0);
    showBins (false);
    showJoins(0);
    selSeries->clear();
    firstSeries->clear();
    lastSeries->clear();
    chartview->hide();
//    cht->removeAllSeries();
}

/* Size selex equation 0, age selex equation 10 */
void equationDialog::constant (float val)
{
    int xRange = binMax - binMin;
    int xTicks = 9; //xRange / 2;
    float binMid = binWidth * binMidWidth;

    ui->label_title->setText(QString("Constant Value %1").arg(QString::number(val,'g',2)));
    showSliders(0);
    showBins(true);
    showJoins(0);

    chartview->setChart(new QChart());
    delete cht;
    cht = chartview->chart();
    cht->legend()->setVisible(false);

    selSeries = new QLineSeries(cht);
    selSeries->append(QPointF(binMin + binMid, val));
    selSeries->append(QPointF(binMax + binMid, val));
    selSeries->setPen(QPen(QBrush(Qt::blue), 3));
    cht->addSeries(selSeries);

    axisXsel->setRange((binMin), (binMax + binWidth));
    axisXsel->setTickCount(xRange % 2? xTicks: xTicks + 1);
    axisY->setRange(0, (val + .2));
    axisY->setTickCount(7);
//    axisX->applyNiceNumbers();
//    axisY->applyNiceNumbers();
    cht->setAxisX(axisXsel, selSeries);
    cht->setAxisY(axisY, selSeries);
}

/* Size selex equation 1, age selex equation 11 */
void equationDialog::logistic ()
{
    int xRange = binMax - binMin;
    int xTicks = 9;
    int yTicks = 7;
    float yVal = 0;
    float temp = 0;
    float len = 0;
    float par1 = ui->doubleSpinBox_1_value->value();
    float par2 = ui->doubleSpinBox_2_value->value();

    ui->label_title->setText(QString("Exponential"));
    showSliders(2);
    ui->label_1_name->setText("Value at 50%");
    ui->label_1_type->setText("Value");
    ui->label_2_name->setText("Diff 95% & 50%");
    ui->label_2_type->setText("Value");
    showBins(true);
    showJoins(0);

    chartview->setChart(new QChart());
    delete cht;
    cht = chartview->chart();
    cht->legend()->setVisible(false);
    xTicks = cht->rect().width() / 100;
    yTicks = cht->rect().height() / 50;

    selSeries = new QLineSeries(cht);
    for (int i = 0; i < xValList.count(); i++)
    {
        len = xValList.at(i);
        temp = -log(19) * (len - par1) / par2;
        yVal = 1 / (1 + exp(temp));
        selSeries->append(QPointF(len, yVal));
    }
    selSeries->setPen(QPen(QBrush(Qt::red), 3));
    cht->addSeries(selSeries);

    axisXsel->setRange((binMin), (binMax + binWidth));
    axisXsel->setTickCount(xTicks % 2? xTicks: xTicks + 1);
    axisY->setRange(0, (1 + .2));
    axisY->setTickCount(yTicks);
//    axisXsel->applyNiceNumbers();
//    axisY->applyNiceNumbers();
    cht->setAxisX(axisXsel, selSeries);
    cht->setAxisY(axisY, selSeries);
}

/* Size selex equation 6, age selex equation 16 */
void equationDialog::linear ()
{
    QPointF pt;
    ui->label_title->setText(QString("Line Segments"));
    showSliders(parameters->rowCount());
    showBins(true);
    showJoins(0);
    cht->removeAllSeries();
    selSeries->clear();
}

