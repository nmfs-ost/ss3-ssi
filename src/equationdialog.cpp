#include "equationdialog.h"
#include "ui_equationdialog.h"

#include <QAbstractButton>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

#define SLIDER_SPAN  100000

#define SLIDER_SCALE 10000
#define VALUE_SCALE .0001

#include <cmath>

double neglog19 = -1.0 * log(19);

equationDialog::equationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::equationDialog)
{
    ui->setupUi(this);

    title = QString("None");

    selex = NULL;
    dataModel = NULL;
    fleet = NULL;

    equationNum = -1;
    parameters = NULL;

    yMax = -10;
    binMin = 0;
    binMax = 20;
    binMidWidth = .5;
    binWidth = 2;

    resetChart(true);
    selSeries->append(0, 0);
    cht->addSeries(selSeries);
    chartview->setChart(cht);

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
    binMin = 0; binMax = 20; binWidth = 2; binMidWidth = 0.;
    ui->spinBox_bins_min->setValue(binMin);
    ui->spinBox_bins_max->setValue(binMax);
    ui->spinBox_bins_width->setValue(binWidth);
    ui->doubleSpinBox_bins_midbin->setValue(binMidWidth);

    connect (ui->spinBox_bins_min, SIGNAL(valueChanged(int)), SLOT(binMinChanged(int)));
    connect (ui->spinBox_bins_max, SIGNAL(valueChanged(int)), SLOT(binMaxChanged(int)));
    connect (ui->spinBox_bins_width, SIGNAL(valueChanged(int)), SLOT(binWidthChanged(int)));
//    connect (ui->doubleSpinBox_bins_midbin, SIGNAL(valueChanged(double)), SLOT(binMidChanged(double)));

    connect (ui->horizontalSlider_1, SIGNAL(valueChanged(int)), SLOT(slider1Changed(int)));
//    connect (ui->horizontalSlider_1, SIGNAL(sliderReleased()), SLOT(update()));
    connect (ui->doubleSpinBox_1_min, SIGNAL(valueChanged(double)), SLOT(min1Changed(double)));
    connect (ui->doubleSpinBox_1_max, SIGNAL(valueChanged(double)), SLOT(max1Changed(double)));
    connect (ui->doubleSpinBox_1_value, SIGNAL(valueChanged(double)), SLOT(value1Changed(double)));
//    connect (ui->doubleSpinBox_1_value, SIGNAL(editingFinished()), SLOT(update()));

    connect (ui->horizontalSlider_2, SIGNAL(valueChanged(int)), SLOT(slider2Changed(int)));
//    connect (ui->horizontalSlider_2, SIGNAL(sliderReleased()), SLOT(update()));
    connect (ui->doubleSpinBox_2_min, SIGNAL(valueChanged(double)), SLOT(min2Changed(double)));
    connect (ui->doubleSpinBox_2_max, SIGNAL(valueChanged(double)), SLOT(max2Changed(double)));
    connect (ui->doubleSpinBox_2_value, SIGNAL(valueChanged(double)), SLOT(value2Changed(double)));
//    connect (ui->doubleSpinBox_2_value, SIGNAL(editingFinished()), SLOT(update()));

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
    connect (ui->doubleSpinBox_6_value, SIGNAL(valueChanged(double)), SLOT(value6Changed(double)));
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

    connect (ui->horizontalSlider_9, SIGNAL(valueChanged(int)), SLOT(slider9Changed(int)));
    connect (ui->doubleSpinBox_9_value, SIGNAL(valueChanged(double)), SLOT(value9Changed(double)));
    connect (ui->doubleSpinBox_9_min, SIGNAL(valueChanged(double)), SLOT(min9Changed(double)));
    connect (ui->doubleSpinBox_9_max, SIGNAL(valueChanged(double)), SLOT(max9Changed(double)));

    connect (this, SIGNAL(numbersUpdated()), SLOT(updateSel()));
//    connect (cht, SIGNAL(plotAreaChanged(QRectF)), SLOT(updateTicks(QRectF)));
    connect (this, SIGNAL(linearUpdated(float)), SLOT(updateLinearExp(float)));

    connect (ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), SLOT(buttonClicked(QAbstractButton*)));
    building = false;
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
//    restoreAll();
}

void equationDialog::changeSelex()
{
    special = selex->getSpecial();
    equationNum = selex->getPattern();
    restoreAll();
}

void equationDialog::setDataModel (ss_model *data)
{
    dataModel = data;
//    setLengths();
//    setAges();
}

void equationDialog::setXvals(const QStringList &vals)
{
    xValList.clear();

    if (!vals.isEmpty())
    {
        for (int i = 0; i < vals.count(); i++)
            xValList.append(QString(vals.at(i)).toFloat());

        ui->spinBox_bins_min->setValue(xValList.first());
        ui->spinBox_bins_max->setValue(xValList.last());
        ui->spinBox_bins_width->setValue(xValList.at(2) - xValList.at(1));
    }
}

void equationDialog::setMidBin(float val)
{
    binMidWidth = val;
    ui->doubleSpinBox_bins_midbin->setValue(val);
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
}

void equationDialog::setParameters(tablemodel *params)
{
    parameters = params;
}

void equationDialog::getParameterValues()
{
    double min, max, val;
    QStringList values;
    int numParams = parameters->rowCount();

    disconnect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));
    showSliders(numParams);

    switch (numParams)
    {
    default:
    case 9:
        values = parameters->getRowData(8);
        min = (values.at(0)).toDouble();
        max = (values.at(1)).toDouble();
        val = (values.at(2)).toDouble();
        ui->doubleSpinBox_9_input->setRange(min, max);
        ui->doubleSpinBox_9_input->setValue(val);
        setSlider9(min, max, val);
    case 8:
        values = parameters->getRowData(7);
        min = (values.at(0)).toDouble();
        max = (values.at(1)).toDouble();
        val = (values.at(2)).toDouble();
        ui->doubleSpinBox_8_input->setMinimum(min);
        ui->doubleSpinBox_8_input->setMaximum(max);
        ui->doubleSpinBox_8_input->setValue(val);
        setSlider8(min, max, val);
    case 7:
        values = parameters->getRowData(6);
        min = (values.at(0)).toDouble();
        max = (values.at(1)).toDouble();
        val = (values.at(2)).toDouble();
        ui->doubleSpinBox_7_input->setMinimum(min);
        ui->doubleSpinBox_7_input->setMaximum(max);
        ui->doubleSpinBox_7_input->setValue(val);
        setSlider7(min, max, val);
    case 6:
        values = parameters->getRowData(5);
        min = (values.at(0)).toDouble();
        max = (values.at(1)).toDouble();
        val = (values.at(2)).toDouble();
        ui->doubleSpinBox_6_input->setMinimum(min);
        ui->doubleSpinBox_6_input->setMaximum(max);
        ui->doubleSpinBox_6_input->setValue(val);
        setSlider6(min, max, val);
    case 5:
        values = parameters->getRowData(4);
        min = (values.at(0)).toDouble();
        max = (values.at(1)).toDouble();
        val = (values.at(2)).toDouble();
        ui->doubleSpinBox_5_input->setMinimum(min);
        ui->doubleSpinBox_5_input->setMaximum(max);
        ui->doubleSpinBox_5_input->setValue(val);
        setSlider5(min, max, val);
    case 4:
        values = parameters->getRowData(3);
        min = (values.at(0)).toDouble();
        max = (values.at(1)).toDouble();
        val = (values.at(2)).toDouble();
        ui->doubleSpinBox_4_input->setMinimum(min);
        ui->doubleSpinBox_4_input->setMaximum(max);
        ui->doubleSpinBox_4_input->setValue(val);
        setSlider4(min, max, val);
    case 3:
        values = parameters->getRowData(2);
        min = (values.at(0)).toDouble();
        max = (values.at(1)).toDouble();
        val = (values.at(2)).toDouble();
        ui->doubleSpinBox_3_input->setMinimum(min);
        ui->doubleSpinBox_3_input->setMaximum(max);
        ui->doubleSpinBox_3_input->setValue(val);
        setSlider3(min, max, val);
    case 2:
        values = parameters->getRowData(1);
        min = (values.at(0)).toDouble();
        max = (values.at(1)).toDouble();
        val = (values.at(2)).toDouble();
        ui->doubleSpinBox_2_input->setMinimum(min);
        ui->doubleSpinBox_2_input->setMaximum(max);
        ui->doubleSpinBox_2_input->setValue(val);
        setSlider2(min, max, val);
    case 1:
        values = parameters->getRowData(0);
        min = (values.at(0)).toDouble();
        max = (values.at(1)).toDouble();
        val = (values.at(2)).toDouble();
        ui->doubleSpinBox_1_input->setMinimum(min);
        ui->doubleSpinBox_1_input->setMaximum(max);
        ui->doubleSpinBox_1_input->setValue(val);
        setSlider1(min, max, val);
    case 0:
        ;
    }

    connect (this, SIGNAL(numbersUpdated()), SLOT(updateSel()));
//    emit numbersUpdated();
}

void equationDialog::setParameterValues()
{
    QStringList values;
    building = true;
    switch (parameters->rowCount())
    {
    case 9:
        values = parameters->getRowData(8);
        values[0] = QString::number(min9);
        values[1] = QString::number(max9);
        values[2] = QString::number(val9);
        parameters->setRowData(8, values);
    case 8:
        values = parameters->getRowData(7);
        values[0] = QString::number(ui->doubleSpinBox_8_min->value());
        values[1] = QString::number(ui->doubleSpinBox_8_max->value());
        values[2] = QString::number(ui->doubleSpinBox_8_input->value());
        parameters->setRowData(7, values);
    case 7:
        values = parameters->getRowData(6);
        values[0] = QString::number(ui->doubleSpinBox_7_min->value());
        values[1] = QString::number(ui->doubleSpinBox_7_max->value());
        values[2] = QString::number(ui->doubleSpinBox_7_input->value());
        parameters->setRowData(6, values);
    case 6:
       values = parameters->getRowData(5);
       values[0] = QString::number(ui->doubleSpinBox_6_min->value());
       values[1] = QString::number(ui->doubleSpinBox_6_max->value());
       values[2] = QString::number(ui->doubleSpinBox_6_input->value());
       parameters->setRowData(5, values);
    case 5:
        values = parameters->getRowData(4);
        values[0] = QString::number(ui->doubleSpinBox_5_min->value());
        values[1] = QString::number(ui->doubleSpinBox_5_max->value());
        values[2] = QString::number(ui->doubleSpinBox_5_input->value());
        parameters->setRowData(4, values);
    case 4:
        values = parameters->getRowData(3);
        values[0] = QString::number(ui->doubleSpinBox_4_min->value());
        values[1] = QString::number(ui->doubleSpinBox_4_max->value());
        values[2] = QString::number(ui->doubleSpinBox_4_input->value());
        parameters->setRowData(3, values);
    case 3:
        values = parameters->getRowData(2);
        values[0] = QString::number(min3);
        values[1] = QString::number(max3);
        values[2] = QString::number(val3);
        parameters->setRowData(2, values);
    case 2:
        values = parameters->getRowData(1);
        values[0] = QString::number(min2);
        values[1] = QString::number(max2);
        values[2] = QString::number(val2);
        parameters->setRowData(1, values);
    case 1:
        values = parameters->getRowData(0);
        values[0] = QString::number(min1);
        values[1] = QString::number(max1);
        values[2] = QString::number(val1);
        parameters->setRowData(0, values);
    }
    building = false;
}

void equationDialog::refresh()
{
    ui->spinBox_bins_min->setValue(binMin);
    ui->spinBox_bins_max->setValue(binMax);
    ui->spinBox_bins_width->setValue(binWidth);
    ui->doubleSpinBox_bins_midbin->setValue(binMidWidth);
    restoreAll();
}

void equationDialog::update()
{
    QString msg;
    int sliders = 0;

    if (!building)
    {
        building = true;
//        getParameterValues();

        switch (equationNum)
        {
        case 0:
        case 10:
            constant(1.0);
            break;

        case 11:
            constantRange();
            break;

        case 1:
        case 12:
            logistic();
            break;

        case 2:
        case 7:
            blank(8, 8);
            break;

        case 3:
            blank (6, 0, QString("Discontinued"));
            break;

        case 13:
            blank (8, 0, QString("Double logistic, IF joiners, Discouraged, use pattern #18"));
            break;

        case 4:
            blank(0, 30);
            break;

        case 14:
            eachAge();
            break;

        case 5:
            sliders = 2;
        case 15:
            mirror(sliders);
            break;

        case 6:
            linear(0);
            break;

        case 16:
            coleGauss();
            break;

        case 17:
            randomWalk(0);
            break;

        case 8:
        case 18:
            dblLogPeak();
            break;

        case 9:
        case 19:
            dblLogistic();
            break;

        case 20:
        case 24:
            dblNormEndpts();
            break;

        case 22:
            dblNormCasal();
            break;

        case 23:
            blank(6, 0, QString("Double Normal, similar to #24"));
            break;

        case 25:
        case 26:
            expLogistic();
            break;

        case 41:
            randomWalk(2.);
            break;

        case 43:
            linear(2);
            break;

        default:
            blank(0);
            break;

        }
        building = false;
    }
}

void equationDialog::updateSel()
{
    if (!building)
    {
        building = true;
        switch (equationNum)
        {
        case 0:
        case 10:
            updateConstant();
            break;

        case 11:
            updateConstantRange();
            break;

        case 1:
        case 12:
            updateLogistic();
            break;

        case 5:
            updateMirror();
            break;

        case 6:
            updateLinear(0);
            break;

        case 8:
            updateDblLogPeak();
            break;

        case 18:
            updateDblLogSmooth();
            break;

        case 9:
        case 19:
            updateDblLogistic();
            break;

        case 14:
            updateEachAge();
            break;

        case 16:
            updateColeGauss();
            break;

        case 17:
            updateRandomWalk(0);
            break;

        case 20:
        case 24:
            updateDblNormEndpts();
            break;

        case 22:
            updateDblNormCasal();
            break;

        case 25:
        case 26:
            updateExpLogistic();
            break;

        case 41:
            updateRandomWalk(2.);
            break;

        case 43:
            updateLinear(2);
            break;

        default:
            break;
        }
        building = false;
    }
}

void equationDialog::setSlider1(double min, double max, double value)
{
    min1 = min; max1 = max;
    ui->doubleSpinBox_1_max->setValue(max1);
    ui->doubleSpinBox_1_min->setValue(min1);
    ui->doubleSpinBox_1_value->setMinimum(min1);
    ui->doubleSpinBox_1_value->setMaximum(max1);
    ui->horizontalSlider_1->setMaximum(max1 * SLIDER_SCALE);
    ui->horizontalSlider_1->setMinimum(min1 * SLIDER_SCALE);
    ui->doubleSpinBox_1_value->setValue(value);
}

void equationDialog::slider1Changed(int value)
{
    double val = (double)value * VALUE_SCALE;
    ui->doubleSpinBox_1_value->setValue(val);
}

void equationDialog::value1Changed (double value)
{
    if (val1 != value)
    {
        val1 = value;
        emit numbersUpdated();
        ui->horizontalSlider_1->setValue(val1 * SLIDER_SCALE);
    }
}

void equationDialog::min1Changed (double value)
{
    if (min1 != value)
    {
        min1 = value;
        if (min1 > max1)
        {
            min1 = max1;
        }
        ui->doubleSpinBox_1_min->setValue(min1);
        ui->doubleSpinBox_1_value->setMinimum(min1);
        ui->horizontalSlider_1->setMinimum(min1 * SLIDER_SCALE);
    }
}

void equationDialog::max1Changed (double value)
{
    if (max1 != value)
    {
        max1 = value;
        if (max1 < min1)
        {
            max1 = min1;
        }
        ui->doubleSpinBox_1_max->setValue(max1);
        ui->doubleSpinBox_1_input->setMaximum(max1);
        ui->horizontalSlider_1->setMaximum(max1 * SLIDER_SCALE);
    }
}

void equationDialog::setSlider2(float min, float max, float value)
{
    min2 = min; max2 = max;
    ui->doubleSpinBox_2_max->setValue(max2);
    ui->doubleSpinBox_2_min->setValue(min2);
    ui->doubleSpinBox_2_value->setMaximum(max2);
    ui->doubleSpinBox_2_value->setMinimum(min2);
    ui->horizontalSlider_2->setMaximum(max2 * SLIDER_SCALE);
    ui->horizontalSlider_2->setMinimum(min2 * SLIDER_SCALE);
    ui->doubleSpinBox_2_value->setValue(value);
}

void equationDialog::slider2Changed(int value)
{
    double val = (double)value * VALUE_SCALE;
    ui->doubleSpinBox_2_value->setValue(val);
}

void equationDialog::value2Changed (double value)
{
    if (val2 != value)
    {
        val2 = value;
        emit numbersUpdated();
        ui->horizontalSlider_2->setValue(val2 * SLIDER_SCALE);
    }
}

void equationDialog::min2Changed (double value)
{
    if (min2 != value)
    {
        min2 = value;
        if (min2 > max2)
        {
            min2 = max2;
        }
        ui->doubleSpinBox_2_min->setValue(min2);
        ui->doubleSpinBox_2_value->setMinimum(min2);
        ui->horizontalSlider_2->setMinimum(min2 * SLIDER_SCALE);
    }
}

void equationDialog::max2Changed (double value)
{
    if (max2 != value)
    {
        max2 = value;
        if (max2 < min2)
        {
            max2 = min2;
        }
        ui->doubleSpinBox_2_max->setValue(max2);
        ui->doubleSpinBox_2_input->setMaximum(max2);
        ui->horizontalSlider_2->setMaximum(max2 * SLIDER_SCALE);
    }
}

void equationDialog::setSlider3(float min, float max, float value)
{
    min3 = min; max3 = max;
    ui->doubleSpinBox_3_max->setValue(max3);
    ui->doubleSpinBox_3_min->setValue(min3);
    ui->doubleSpinBox_3_value->setMaximum(max3);
    ui->doubleSpinBox_3_value->setMinimum(min3);
    ui->horizontalSlider_3->setMaximum(max3 * SLIDER_SCALE);
    ui->horizontalSlider_3->setMinimum(min3 * SLIDER_SCALE);
    ui->doubleSpinBox_3_value->setValue(value);
}

void equationDialog::slider3Changed(int value)
{
    double val = (double)value * VALUE_SCALE;
    ui->doubleSpinBox_3_value->setValue(val);
}

void equationDialog::value3Changed (double value)
{
    if (val3 != value)
    {
        val3 = value;
        emit numbersUpdated();
        ui->horizontalSlider_3->setValue(val3 * SLIDER_SCALE);
    }
}

void equationDialog::min3Changed (double value)
{
    if (min3 != value)
    {
        min3 = value;
        if (min3 > max3)
        {
            min3 = max3;
        }
        ui->doubleSpinBox_3_min->setValue(min3);
        ui->doubleSpinBox_3_value->setMinimum(min3);
        ui->horizontalSlider_3->setMinimum(min3 * SLIDER_SCALE);
    }
}

void equationDialog::max3Changed (double value)
{
    if (max3 != value)
    {
        max3 = value;
        if (max3 < min3)
        {
            max3 = min3;
        }
        ui->doubleSpinBox_3_max->setValue(max3);
        ui->doubleSpinBox_3_value->setMaximum(max3);
        ui->horizontalSlider_3->setMaximum(max3 * SLIDER_SCALE);
    }
}

void equationDialog::setSlider4(float min, float max, float value)
{
    min4 = min; max4 = max;
    ui->doubleSpinBox_4_max->setValue(max4);
    ui->doubleSpinBox_4_min->setValue(min4);
    ui->doubleSpinBox_4_value->setMaximum(max4);
    ui->doubleSpinBox_4_value->setMinimum(min4);
    ui->horizontalSlider_4->setMaximum(max4 * SLIDER_SCALE);
    ui->horizontalSlider_4->setMinimum(min4 * SLIDER_SCALE);
    ui->doubleSpinBox_4_value->setValue(value);
}

void equationDialog::slider4Changed(int value)
{
    double val = (double)value * VALUE_SCALE;
    ui->doubleSpinBox_4_value->setValue(val);
}

void equationDialog::value4Changed (double value)
{
    if (val4 != value)
    {
        val4 = value;
        emit numbersUpdated();
        ui->horizontalSlider_4->setValue(val4 * SLIDER_SCALE);
    }
}

void equationDialog::min4Changed (double value)
{
    if (min4 != value)
    {
        min4 = value;
        if (min4 > max4)
        {
            min4 = max4;
        }
        ui->doubleSpinBox_4_min->setValue(min4);
        ui->doubleSpinBox_4_value->setMinimum(min4);
        ui->horizontalSlider_4->setMinimum(min4 * SLIDER_SCALE);
    }
}

void equationDialog::max4Changed (double value)
{
    if (max4 != value)
    {
        max4 = value;
        if (max4 < min4)
        {
            max4 = min4;
        }
        ui->doubleSpinBox_4_max->setValue(max4);
        ui->doubleSpinBox_4_value->setMaximum(max4);
        ui->horizontalSlider_4->setMaximum(max4 * SLIDER_SCALE);
    }
}

void equationDialog::setSlider5(float min, float max, float value)
{
    min5 = min; max5 = max;
    ui->doubleSpinBox_5_max->setValue(max5);
    ui->doubleSpinBox_5_min->setValue(min5);
    ui->doubleSpinBox_5_value->setMaximum(max5);
    ui->doubleSpinBox_5_value->setMinimum(min5);
    ui->horizontalSlider_5->setMaximum(max5 * SLIDER_SCALE);
    ui->horizontalSlider_5->setMinimum(min5 * SLIDER_SCALE);
    ui->doubleSpinBox_5_value->setValue(value);
}

void equationDialog::slider5Changed(int value)
{
    double val = (double)value * VALUE_SCALE;
    ui->doubleSpinBox_5_value->setValue(val);
}

void equationDialog::value5Changed (double value)
{
    if (val5 != value)
    {
        val5 = value;
        emit numbersUpdated();
        ui->horizontalSlider_5->setValue(val5 * SLIDER_SCALE);
    }
}

void equationDialog::min5Changed (double value)
{
    if (min5 != value)
    {
        min5 = value;
        if (min5 > max5)
        {
            min5 = max5;
        }
        ui->doubleSpinBox_5_min->setValue(min5);
        ui->doubleSpinBox_5_value->setMinimum(min5);
        ui->horizontalSlider_5->setMinimum(min5 * SLIDER_SCALE);
    }
}

void equationDialog::max5Changed (double value)
{
    if (max5 != value)
    {
        max5 = value;
        if (max5 < min5)
        {
            max5 = min5;
        }
        ui->doubleSpinBox_5_max->setValue(max5);
        ui->doubleSpinBox_5_value->setMaximum(max5);
        ui->horizontalSlider_5->setMaximum(max5 * SLIDER_SCALE);
    }
}

void equationDialog::setSlider6(float min, float max, float value)
{
    min6 = min; max6 = max;
    ui->doubleSpinBox_6_max->setValue(max6);
    ui->doubleSpinBox_6_min->setValue(min6);
    ui->doubleSpinBox_6_value->setMaximum(max6);
    ui->doubleSpinBox_6_value->setMinimum(min6);
    ui->horizontalSlider_6->setMaximum(max6 * SLIDER_SCALE);
    ui->horizontalSlider_6->setMinimum(min6 * SLIDER_SCALE);
    ui->doubleSpinBox_6_value->setValue(value);
}

void equationDialog::slider6Changed(int value)
{
    double val = (double)value * VALUE_SCALE;
    ui->doubleSpinBox_6_value->setValue(val);
}

void equationDialog::value6Changed (double value)
{
    if (val6 != value)
    {
        val6 = value;
        emit numbersUpdated();
        ui->horizontalSlider_6->setValue(val6 * SLIDER_SCALE);
    }
}

void equationDialog::min6Changed (double value)
{
    if (min6 != value)
    {
        min6 = value;
/*        if (min6 > max6)
        {
            min6 = max6;
        }
        ui->doubleSpinBox_6_min->setValue(min6);*/
        ui->doubleSpinBox_6_value->setMinimum(min6);
        ui->horizontalSlider_6->setMinimum(min6 * SLIDER_SCALE);
    }
}

void equationDialog::max6Changed (double value)
{
    if (max6 != value)
    {
        max6 = value;
        if (max6 < min6)
        {
            max6 = min6;
        }
        ui->doubleSpinBox_6_max->setValue(max6);
        ui->doubleSpinBox_6_value->setMaximum(max6);
        ui->horizontalSlider_6->setMaximum(max6 * SLIDER_SCALE);
    }
}

void equationDialog::setSlider7(float min, float max, float value)
{
    min7 = min; max7 = max;
    ui->doubleSpinBox_7_max->setValue(max7);
    ui->doubleSpinBox_7_min->setValue(min7);
    ui->doubleSpinBox_7_value->setMaximum(max7);
    ui->doubleSpinBox_7_value->setMinimum(min7);
    ui->horizontalSlider_7->setMaximum(max7 * SLIDER_SCALE);
    ui->horizontalSlider_7->setMinimum(min7 * SLIDER_SCALE);
    ui->doubleSpinBox_7_value->setValue(value);
}

void equationDialog::slider7Changed(int value)
{
    double val = (double)value * VALUE_SCALE;
    ui->doubleSpinBox_7_value->setValue(val);
}

void equationDialog::value7Changed (double value)
{
    if (val7 != value)
    {
        val7 = value;
        emit numbersUpdated();
        ui->horizontalSlider_7->setValue(val7 * SLIDER_SCALE);
    }
}

void equationDialog::min7Changed (double value)
{
    if (min7 != value)
    {
        min7 = value;
        if (min7 > max7)
        {
            min7 = max7;
        }
        ui->doubleSpinBox_7_min->setValue(min7);
        ui->doubleSpinBox_7_value->setMinimum(min7);
        ui->horizontalSlider_7->setMinimum(min7 * SLIDER_SCALE);
    }
}

void equationDialog::max7Changed (double value)
{
    if (max7 != value)
    {
        max7 = value;
        if (max7 < min7)
        {
            max7 = min7;
        }
        ui->doubleSpinBox_7_max->setValue(max7);
        ui->doubleSpinBox_7_value->setMaximum(max7);
        ui->horizontalSlider_7->setMaximum(max7 * SLIDER_SCALE);
    }
}

void equationDialog::setSlider8(float min, float max, float value)
{
    min8 = min; max8 = max;
    ui->doubleSpinBox_8_max->setValue(max8);
    ui->doubleSpinBox_8_min->setValue(min8);
    ui->doubleSpinBox_8_value->setMaximum(max8);
    ui->doubleSpinBox_8_value->setMinimum(min8);
    ui->horizontalSlider_8->setMaximum(max8 * SLIDER_SCALE);
    ui->horizontalSlider_8->setMinimum(min8 * SLIDER_SCALE);
    ui->doubleSpinBox_8_value->setValue(value);
}

void equationDialog::slider8Changed(int value)
{
    double val = (double)value * VALUE_SCALE;
    ui->doubleSpinBox_8_value->setValue(val);
}

void equationDialog::value8Changed (double value)
{
    if (val8 != value)
    {
        val8 = value;
        emit numbersUpdated();
        ui->horizontalSlider_8->setValue(val8 * SLIDER_SCALE);
    }
}

void equationDialog::min8Changed (double value)
{
    if (min8 != value)
    {
        min8 = value;
        if (min8 > max8)
        {
            min8 = max8;
        }
        ui->doubleSpinBox_8_min->setValue(min8);
        ui->doubleSpinBox_8_value->setMinimum(min8);
        ui->horizontalSlider_8->setMinimum(min8 * SLIDER_SCALE);
    }
}

void equationDialog::max8Changed (double value)
{
    if (max8 != value)
    {
        max8 = value;
        if (max8 < min8)
        {
            max8 = min8;
        }
        ui->doubleSpinBox_8_max->setValue(max8);
        ui->doubleSpinBox_8_value->setMaximum(max8);
        ui->horizontalSlider_8->setMaximum(max8 * SLIDER_SCALE);
    }
}

void equationDialog::setSlider9(float min, float max, float value)
{
    min9 = min; max9 = max;
    ui->doubleSpinBox_9_max->setValue(max9);
    ui->doubleSpinBox_9_min->setValue(min9);
    ui->doubleSpinBox_9_value->setMaximum(max9);
    ui->doubleSpinBox_9_value->setMinimum(min9);
    ui->horizontalSlider_9->setMaximum(max9 * SLIDER_SCALE);
    ui->horizontalSlider_9->setMinimum(min9 * SLIDER_SCALE);
    ui->doubleSpinBox_9_value->setValue(value);
}

void equationDialog::slider9Changed(int value)
{
    double val = (double)value * VALUE_SCALE;
    ui->doubleSpinBox_9_value->setValue(val);
}

void equationDialog::value9Changed (double value)
{
    if (val9 != value)
    {
        val9 = value;
        emit numbersUpdated();
        ui->horizontalSlider_9->setValue(val9 * SLIDER_SCALE);
    }
}

void equationDialog::min9Changed (double value)
{
    if (min9 != value)
    {
        min9 = value;
        if (min9 > max9)
        {
            min9 = max9;
        }
        ui->doubleSpinBox_9_min->setValue(min9);
        ui->doubleSpinBox_9_value->setMinimum(min9);
        ui->horizontalSlider_9->setMinimum(min9 * SLIDER_SCALE);
    }
}

void equationDialog::max9Changed (double value)
{
    if (max9 != value)
    {
        max9 = value;
        if (max9 < min9)
        {
            max9 = min9;
        }
        ui->doubleSpinBox_9_max->setValue(max9);
        ui->doubleSpinBox_9_value->setMaximum(max9);
        ui->horizontalSlider_9->setMaximum(max9 * SLIDER_SCALE);
    }
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
    update();
}

void equationDialog::join2Changed (int value)
{
    join2 = value;
    update();
}

void equationDialog::join3Changed (int value)
{
    join3 = value;
    update();
}

void equationDialog::apply()
{
    if (    equationNum == 14 ||
            equationNum == 17 ||
            equationNum == 27 ||
            parameters->rowCount() > 9)
        return;

    // set input spinboxes
    ui->doubleSpinBox_1_input->setRange(min1, max1);
    ui->doubleSpinBox_1_input->setValue(val1);
    ui->doubleSpinBox_2_input->setRange(min2, max2);
    ui->doubleSpinBox_2_input->setValue(val2);
    ui->doubleSpinBox_3_input->setRange(min3, max3);
    ui->doubleSpinBox_3_input->setValue(val3);
    ui->doubleSpinBox_4_input->setRange(min4, max4);
    ui->doubleSpinBox_4_input->setValue(val4);
    ui->doubleSpinBox_5_input->setRange(min5, max5);
    ui->doubleSpinBox_5_input->setValue(val5);
    ui->doubleSpinBox_6_input->setRange(min6, max6);
    ui->doubleSpinBox_6_input->setValue(val6);
    ui->doubleSpinBox_7_input->setRange(min7, max7);
    ui->doubleSpinBox_7_input->setValue(val7);
    ui->doubleSpinBox_8_input->setRange(min8, max8);
    ui->doubleSpinBox_8_input->setValue(val8);
    ui->doubleSpinBox_9_input->setRange(min9, max9);
    ui->doubleSpinBox_9_input->setValue(val9);
    // write values to parameters
    setParameterValues();
    getParameterValues();
}

void equationDialog::resetValues()
{
    // get values from input spinboxes
    double min, max, val;
    min = ui->doubleSpinBox_1_input->minimum();
    max = ui->doubleSpinBox_1_input->maximum();
    val = ui->doubleSpinBox_1_input->value();
    setSlider1(min, max, val);
    min = ui->doubleSpinBox_2_input->minimum();
    max = ui->doubleSpinBox_2_input->maximum();
    val = ui->doubleSpinBox_2_input->value();
    setSlider2(min, max, val);
    min = ui->doubleSpinBox_3_input->minimum();
    max = ui->doubleSpinBox_3_input->maximum();
    val = ui->doubleSpinBox_3_input->value();
    setSlider3(min, max, val);
    min = ui->doubleSpinBox_4_input->minimum();
    max = ui->doubleSpinBox_4_input->maximum();
    val = ui->doubleSpinBox_4_input->value();
    setSlider4(min, max, val);
    min = ui->doubleSpinBox_5_input->minimum();
    max = ui->doubleSpinBox_5_input->maximum();
    val = ui->doubleSpinBox_5_input->value();
    setSlider5(min, max, val);
    min = ui->doubleSpinBox_6_input->minimum();
    max = ui->doubleSpinBox_6_input->maximum();
    val = ui->doubleSpinBox_6_input->value();
    setSlider6(min, max, val);
    min = ui->doubleSpinBox_7_input->minimum();
    max = ui->doubleSpinBox_7_input->maximum();
    val = ui->doubleSpinBox_7_input->value();
    setSlider7(min, max, val);
    min = ui->doubleSpinBox_8_input->minimum();
    max = ui->doubleSpinBox_8_input->maximum();
    val = ui->doubleSpinBox_8_input->value();
    setSlider8(min, max, val);
    min = ui->doubleSpinBox_9_input->minimum();
    max = ui->doubleSpinBox_9_input->maximum();
    val = ui->doubleSpinBox_9_input->value();
    setSlider9(min, max, val);
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
//    binMidWidth = fleet->getSeasTiming();
    ui->spinBox_bins_min->setValue(binMin);
    ui->spinBox_bins_max->setValue(binMax);
    ui->spinBox_bins_width->setValue(binWidth);
    ui->doubleSpinBox_bins_midbin->setValue(binWidth / 2.0);
    // get values from parameters
    if (parameters != NULL)
        getParameterValues();
    else
        resetValues ();

    update();
}

void equationDialog::close()
{
    emit closed();
    hide();
}

void equationDialog::buttonClicked(QAbstractButton *btn)
{
    if (btn->text().contains("Apply")) {
        apply();
    }
    else if (btn->text().contains("Reset")){
        resetValues();
        emit numbersUpdated();
    }
    else if (btn->text().contains("Restore")){
        restoreAll();
        emit numbersUpdated();
    }
    else if (btn->text().contains("Close")){
        close();
    }
}

void equationDialog::closeEvent(QCloseEvent *event)
{
    emit closed();
}

void equationDialog::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    updateTicks(cht->rect());
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
    binMidWidth = fleet->getSeasTiming();
}

int equationDialog::getSpecial() const
{
    return special;
}

void equationDialog::setSpecial(int value)
{
    special = value;
//    emit numbersUpdated();
}

void equationDialog::setTrans1(double value)
{
    double val = value;
    switch (equationNum)
    {
    case 2:
        break;
    }
    if (ui->doubleSpinBox_1_trans->value() != val)
    {
        ui->doubleSpinBox_1_trans->setValue(val);
        emit numbersUpdated();
    }
}

void equationDialog::setTrans2(double value)
{
    double val = value;
    switch (equationNum)
    {
    case 2:
        break;
    }
    if (ui->doubleSpinBox_2_trans->value() != val)
    {
        ui->doubleSpinBox_2_trans->setValue(val);
        emit numbersUpdated();
    }
}

void equationDialog::setTrans3(double value)
{
    double val = value;
    switch (equationNum)
    {
    case 2:
        break;
    }
    ui->doubleSpinBox_3_trans->setValue(val);
    emit numbersUpdated();
}

void equationDialog::setTrans4(double value)
{
    double val = value;
    switch (equationNum)
    {
    case 2:
        break;
    }
    ui->doubleSpinBox_4_trans->setValue(val);
}

void equationDialog::setTrans5(double value)
{
    double val = value;
    switch (equationNum)
    {
    case 2:
        break;
    }
    ui->doubleSpinBox_5_trans->setValue(val);
}

void equationDialog::setTrans6(double value)
{
    double val = value;
    switch (equationNum)
    {
    case 2:
        break;
    }
    ui->doubleSpinBox_6_trans->setValue(val);
}

void equationDialog::setTrans7(double value)
{
    double val = value;
    switch (equationNum)
    {
    case 2:
        break;
    }
    ui->doubleSpinBox_7_trans->setValue(val);
}

void equationDialog::setTrans8(double value)
{
    double val = value;
    switch (equationNum)
    {
    case 2:
        break;
    }
    ui->doubleSpinBox_8_trans->setValue(val);
}

void equationDialog::setTrans9(double value)
{
    double val = value;
    switch (equationNum)
    {
    case 2:
        break;
    }
    ui->doubleSpinBox_9_trans->setValue(val);
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

    ui->label_9_name->setVisible(false);
    ui->doubleSpinBox_9_value->setVisible(false);
    ui->label_9_type->setVisible(false);
    ui->doubleSpinBox_9_trans->setVisible(false);
    ui->doubleSpinBox_9_min->setVisible(false);
    ui->doubleSpinBox_9_max->setVisible(false);
    ui->doubleSpinBox_9_input->setVisible(false);
    ui->horizontalSlider_9->setVisible(false);

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
        ui->label_1_type->setText("value");
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
        ui->label_2_type->setText("value");
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
        ui->label_3_type->setText("value");
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
        ui->label_4_type->setText("value");
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
        ui->label_5_type->setText("value");
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
        ui->label_6_type->setText("value");
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
        ui->label_7_type->setText("value");
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
        ui->label_8_type->setText("value");
        ui->doubleSpinBox_8_trans->setVisible(true);
        ui->doubleSpinBox_8_min->setVisible(true);
        ui->doubleSpinBox_8_max->setVisible(true);
        ui->doubleSpinBox_8_input->setVisible(true);
        ui->horizontalSlider_8->setVisible(true);
    }
    if (num > 8)
    {
        ui->label_9_name->setVisible(true);
        ui->doubleSpinBox_9_value->setVisible(true);
        ui->label_9_type->setVisible(true);
        ui->label_9_type->setText("value");
        ui->doubleSpinBox_9_trans->setVisible(true);
        ui->doubleSpinBox_9_min->setVisible(true);
        ui->doubleSpinBox_9_max->setVisible(true);
        ui->doubleSpinBox_9_input->setVisible(true);
        ui->horizontalSlider_9->setVisible(true);
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
        ui->label_steep_join3->setVisible(true);
        ui->spinBox_steep_join3->setVisible(true);

    case 2:
        ui->label_steep_join2->setVisible(true);
        ui->spinBox_steep_join2->setVisible(true);

    case 1:
        ui->label_steepness->setVisible(true);
        ui->label_steep_join1->setVisible(true);
        ui->spinBox_steep_join1->setVisible(true);
        break;
//    case 0:        // done above, show none
//    default:
    }
}


void equationDialog::updateTicks(QRectF rect)
{
    int xTicks = rect.width() / 100;
    int yTicks = rect.height() / 60;

    updateTicks(xTicks % 2? xTicks: xTicks + 1,
                yTicks % 2? yTicks: yTicks + 1);
}

void equationDialog::updateTicks(int xT, int yT)
{
    axisXsel->setRange((binMin), (binMax + binWidth));
    axisXsel->setTickCount(xT);

    axisY->setRange(0, 1.2);
    axisY->setTickCount(yT);
    axisYalt->setTickCount(yT);
}

void equationDialog::resetChart(bool create)
{
    ui->verticalLayout_graph->removeWidget(chartview);
    if (!create)
    {
        delete axisXsel;
        delete axisY;
        delete axisYalt;
        if (firstSeries != NULL)
            delete firstSeries;
        if (lastSeries != NULL)
            delete lastSeries;
        delete selSeries;
        delete cht;
        delete chartview;
    }
    chartview = new QChartView(this);
    chartview->setChart(new QChart());
    cht = chartview->chart();
    firstSeries = NULL;
    lastSeries = NULL;
    selSeries = new QLineSeries(cht);
    axisXsel = new QValueAxis();
    axisY = new QValueAxis();
    axisYalt = new QValueAxis();

    firstPoints.clear();

    yMax = 0;
    ui->verticalLayout_graph->addWidget(chartview);
    cht->legend()->setVisible(false);
    updateTicks (7, 7);
}

void equationDialog::blank (int num, int rep, QString msg)
{
    resetChart();
    // If replaced by another pattern
    if (rep > 0)
    {
        msg.clear();
        msg.append(QString("Discontinued: use pattern %1 instead.").arg(QString::number(rep)));
    }
    // This is for equations not yet implemented.
    // INTENTIONALLY LEFT BLANK
    if (msg.isEmpty())
        msg.append ("This equation is not yet implemented.\n\n\n THIS PAGE INTENTIONALLY LEFT BANK");

    ui->label_title->setText(msg);
    showSliders(0);
    showBins (false);
    showJoins(0);
}

/* Size selex equation 0, age selex equation 10 */
void equationDialog::constant (float val)
{
    if (!title.isEmpty())
        title.clear();
    title.append(QString("Constant Value %1").arg(QString::number(val,'g',2)));
    ui->label_title->setText(title);
    showSliders(0);
    showBins(true);
    showJoins(0);

    resetChart();

    selSeries->setPen(QPen(QBrush(Qt::red), 3));
    cht->addSeries(selSeries);

    cht->setAxisX(axisXsel, selSeries);
    cht->setAxisY(axisY, selSeries);

    updateConstant();
}

void equationDialog::updateConstant ()
{
    float binMid = binWidth * binMidWidth;
    float start = binMin + binMid;
    float end = binMax + binMid;

    if (!xValList.isEmpty())
    {
        if (equationNum == 0)
            start = xValList.at(0) + binMid;
        else
            start = xValList.at(1) + binMid;
    }
    updateConstant(1.0, start, end);
}

void equationDialog::updateConstant (float val, float first, float last)
{
    float binMid = binWidth * binMidWidth;
    float start = binMin + binMid;
    float end = binMax + binMid;

    selSeries->clear();
    if (equationNum == 0 || start >= first)
    {
        selSeries->append(QPointF(first, val));
    }
    else
    {
        selSeries->append(QPointF(start, 0));
        selSeries->append(QPointF(first, 0));
        selSeries->append(QPointF(first, val));
    }
    if (equationNum == 0 || end <= last)
    {
        selSeries->append(QPointF(end, val));
    }
    else
    {
        selSeries->append(QPointF(last, val));
        selSeries->append(QPointF(last, 0));
        selSeries->append(QPointF(end, 0));
    }
}

void equationDialog::constantRange ()
{
    if (!title.isEmpty())
        title.clear();
    title.append(QString("Constant value within range"));
    ui->label_title->setText(title);
    showSliders(2);
    ui->label_1_name->setText("First age");
    ui->label_1_type->setText("Value");
    ui->label_2_name->setText("Last age");
    ui->label_2_type->setText("Value");
    showBins(true);
    showJoins(0);

    resetChart();

    selSeries->setPen(QPen(QBrush(Qt::red), 3));
    cht->addSeries(selSeries);
    cht->setAxisX(axisXsel, selSeries);
    cht->setAxisY(axisY, selSeries);

    updateConstantRange();
}

void equationDialog::updateConstantRange ()
{
    float par1 = ui->doubleSpinBox_1_value->value();
    float par2 = ui->doubleSpinBox_2_value->value();
    float binMid = binWidth * binMidWidth;
    float start = par1 + binMid;
    float end = par2 + binMid;
    ui->doubleSpinBox_1_trans->setValue(par1);
    ui->doubleSpinBox_2_trans->setValue(par2);

    updateConstant(1.0, start, end);
}

/* Size selex equation 1, age selex equation 12 */
void equationDialog::logistic ()
{
    if (!title.isEmpty())
        title.clear();
    title.append(QString("Logistic"));
    ui->label_title->setText(title);
    showSliders(2);
    ui->label_1_name->setText("Value at 50%");
    ui->label_1_type->setText("Value");
    ui->label_2_name->setText("Diff 95% & 50%");
    ui->label_2_type->setText("Value");
    showBins(true);
    showJoins(0);

    resetChart();

    selSeries->setPen(QPen(QBrush(Qt::red), 3));
    cht->addSeries(selSeries);

    cht->setAxisX(axisXsel, selSeries);
    cht->setAxisY(axisY, selSeries);

    updateLogistic();
}

void equationDialog::updateLogistic()
{
    float yVal = 0;
    float temp = 0;
    float len = 0;
    float par1 = ui->doubleSpinBox_1_value->value();
    float par2 = ui->doubleSpinBox_2_value->value();
    ui->doubleSpinBox_1_trans->setValue(par1);
    ui->doubleSpinBox_2_trans->setValue(par2);

    selSeries->clear();

    for (int i = 0; i < xValList.count(); i++)
    {
        len = xValList.at(i);
        temp = neglog19 * (len - par1) / par2;
        yVal = 1 / (1 + exp(temp));
        selSeries->append(QPointF(len, yVal));
    }
}

/* Size selex equation 5, size and age selex equation 15 */
void equationDialog::mirror (int sliders)
{
    QString msg(QString("Mirror of Fleet (%1)").arg
                (QString::number(getSpecial())));
    resetChart();
    if (sliders > 0)
    {
        ui->label_1_name->setText("Bin select");
        ui->label_1_type->setText("Value");
        ui->label_2_name->setText("Bin select");
        ui->label_2_type->setText("Value");
    }
    ui->label_title->setText(msg);
    showSliders(sliders);
    showBins (sliders > 0);
    showJoins(0);
    updateMirror();
}
void equationDialog::updateMirror()
{
    ui->doubleSpinBox_1_trans->setValue(ui->doubleSpinBox_1_value->value());
    ui->doubleSpinBox_2_trans->setValue(ui->doubleSpinBox_2_value->value());
}

/* Size selex equation 6, age selex equation 16 */
void equationDialog::linear (float scale)
{
    QString msg;
    int num = getSpecial();

    if (!title.isEmpty())
        title.clear();
    msg.append(QString("%1 Linear segments").arg(QString::number(num)));
    title.append(msg);
    ui->label_title->setText(title);

    if (num > 7)
    {
        msg.append(" - Too many segments for this display!");
        blank(9, 0, msg);
    }
    else
    {
        showSliders(num + 2);
        ui->label_1_name->setText("length for P1");
        ui->label_1_type->setText("Value");
        ui->label_2_name->setText("length for P2");
        ui->label_2_type->setText("Value");
        ui->label_3_name->setText("ln of sel at P1");
        ui->label_3_type->setText("Value");
        ui->label_4_name->setText("ln of sel midpt");
        ui->label_4_type->setText("Value");
        ui->label_5_name->setText("ln of sel midpt");
        ui->label_5_type->setText("Value");
        ui->label_6_name->setText("ln of sel midpt");
        ui->label_6_type->setText("Value");
        ui->label_7_name->setText("ln of sel midpt");
        ui->label_7_type->setText("Value");
        ui->label_8_name->setText("ln of sel midpt");
        ui->label_8_type->setText("Value");
        ui->label_9_name->setText("ln of sel midpt");
        ui->label_9_type->setText("Value");

        if (num < 3)
        {
            ui->label_4_name->setText("ln of sel at P2");
        }
        else if (num < 4)
        {
            ui->label_5_name->setText("ln of sel at P2");
        }
        else if (num < 5)
        {
            ui->label_6_name->setText("ln of sel at P2");
        }
        else if (num < 6)
        {
            ui->label_7_name->setText("ln of sel at P2");
        }
        else if (num < 7)
        {
            ui->label_8_name->setText("ln of sel at P2");
        }
        else if (num < 8)
        {
            ui->label_9_name->setText("ln of sel at P2");
        }
        showBins(true);
        showJoins(0);

        resetChart();

        selSeries->setPen(QPen(QBrush(Qt::red), 3));
        cht->addSeries(selSeries);

        cht->addAxis(axisY, Qt::AlignLeft);
        cht->addAxis(axisXsel, Qt::AlignBottom);
        selSeries->attachAxis(axisXsel);
        selSeries->attachAxis(axisY);
        axisY->setTitleText("Selex (red)");

        firstSeries = new QLineSeries(cht);
        firstSeries->setPen(QPen(QBrush(Qt::blue), 2));
        cht->addSeries(firstSeries);

        axisYalt->setTitleText("Ln of Selex (blue)");

        cht->addAxis(axisYalt, Qt::AlignRight);
        firstSeries->attachAxis(axisXsel);
        firstSeries->attachAxis(axisYalt);
        updateLinear(scale);
    }
}

void equationDialog::updateLinear(float scale)
{
    float y = 0;
    int axisYMax = 0;
    int axisYMin = 0;
    float temp = 0;
    float len = 0;
    int num = getSpecial();
    float xVal[10];
    float yVal[10];

    firstSeries->clear();

    if (num > 7)
    {
        QString msg(QString("%1 Linear segments - too many segments for this display!").arg(QString::number(num)));
        blank(num, 0, msg);
        return;
    }

    for (int i = 0; i < 10; i++)
    {
        xVal[i] = 0.0;
        yVal[i] = 0.0;
    }
    xVal[0] = xValList.first();
    yVal[0] = -10.0;
    xVal[1] = ui->doubleSpinBox_1_value->value();
    ui->doubleSpinBox_1_trans->setValue(xVal[1]);
    yVal[1] = ui->doubleSpinBox_3_value->value();
    ui->doubleSpinBox_3_trans->setValue(yVal[1]);
    yVal[2] = ui->doubleSpinBox_4_value->value();
    ui->doubleSpinBox_4_trans->setValue(yVal[2]);
    yVal[3] = ui->doubleSpinBox_5_value->value();
    ui->doubleSpinBox_5_trans->setValue(yVal[3]);
    yVal[4] = ui->doubleSpinBox_6_value->value();
    ui->doubleSpinBox_6_trans->setValue(yVal[4]);
    yVal[5] = ui->doubleSpinBox_7_value->value();
    ui->doubleSpinBox_7_trans->setValue(yVal[5]);
    yVal[6] = ui->doubleSpinBox_8_value->value();
    ui->doubleSpinBox_8_trans->setValue(yVal[6]);
    yVal[7] = ui->doubleSpinBox_9_value->value();
    ui->doubleSpinBox_9_trans->setValue(yVal[7]);

    xVal[num] = ui->doubleSpinBox_2_value->value();
    ui->doubleSpinBox_2_trans->setValue(xVal[num]);
    xVal[num + 1] = xValList.last();
    yVal[num + 1] = yVal[num];

    temp = (xVal[num] - xVal[1]) / (num - 1);
    for (int i = 2; i < num; i++)
    {
        xVal[i] = xVal[1] + (i - 1) * temp;
    }

    for (int i = 0; i <= num; i++)
        firstSeries->append(QPointF(xVal[i], yVal[i]));
    if (xVal[num] > binMax)
        temp = xVal[num] + 2;
    else
        temp = binMax;
    firstSeries->append(QPointF(temp, yVal[num]));

    firstPoints.clear();
    yMax = yVal[0];
    for (int i = 0; i < xValList.count(); i++)
    {
        len = xValList.at(i);
        for (int j = 1; j <= num; j++)
        {
            if (len >= xVal[j -1] && len < xVal[j])
            {
                y = evaluateLine(QPointF(xVal[j-1], yVal[j-1]), QPointF(xVal[j], yVal[j]), len);
                if (y > yMax)
                    yMax = y;
                if (y < axisYMin)
                    axisYMin = (int)y - 2;
                axisYMax = (int)yMax + 1;
                firstPoints.append(QPointF(len, y));
                break;
            }
        }
        if (len >= xVal[num])
        {
            y = yVal[num];
            firstPoints.append(QPointF(len, y));
        }
    }
    axisYalt->setRange(axisYMin, axisYMax);
    emit linearUpdated(scale);
}

void equationDialog::updateLinearExp(float scale)
{
    float yVal;

    selSeries->clear();

    for (int i = 0; i < firstPoints.count(); i++)
    {
        yVal = exp(firstPoints.at(i).y() - yMax);
        selSeries->append(firstPoints.at(i).x(), yVal);
    }
}

float equationDialog::evaluateLine(QPointF pt1, QPointF pt2, float x)
{
    float slope = (pt2.y() - pt1.y()) / (pt2.x() - pt1.x());
    float y = slope * (x - pt2.x()) + pt2.y();
    return y;
}

/** This is the code from ss_selex.tpl
 *     k1 = int(value(sp(5)));
 *     if(k1 > 1)
 *         sel(1,k1-1) = 0.0;
 *     sel(k1,nlength) = elem_prod((1/(1+mfexp(-sp(2)*(len_bins_m(k1,nlength)-sp(1)) ))),
 *                                 (1-1/(1+mfexp(-sp(4)*(len_bins_m(k1,nlength)-(sp(1)*sp(6)+sp(3))) ))) );
 *     sel += 1.0e-6;
 *     sel /= max(sel);
 */

void equationDialog::dblLogistic()
{
    if (!title.isEmpty())
        title.clear();
    title.append(QString("Simple Double Logistic"));
    ui->label_title->setText(title);
    showSliders(6);
    ui->label_1_name->setText("Infl 1");
    ui->label_1_type->setText("Value");
    ui->label_2_name->setText("Slope 1");
    ui->label_2_type->setText("Value");
    ui->label_3_name->setText("Infl 2");
    ui->label_3_type->setText("Value");
    ui->label_4_name->setText("Slope 2");
    ui->label_4_type->setText("Value");
    ui->label_5_name->setText("First Bin");
    ui->label_5_type->setText("Value");
    ui->label_6_name->setText("Offset");
    ui->label_6_type->setText("Value");
    showBins(true);

    resetChart();

    selSeries->setPen(QPen(QBrush(Qt::red), 3));
    cht->addSeries(selSeries);

    cht->setAxisX(axisXsel, selSeries);
    cht->setAxisY(axisY, selSeries);

    updateDblLogistic();
}

void equationDialog::updateDblLogistic()
{
    float maxSel = 0;
    float sel = 0;
    float term1 = 0, term2 = 0;
    int limit, i;
    float par1 = ui->doubleSpinBox_1_value->value();
    float par2 = ui->doubleSpinBox_2_value->value();
    float par3 = ui->doubleSpinBox_3_value->value();
    float par4 = ui->doubleSpinBox_4_value->value();
    float par5 = ui->doubleSpinBox_5_value->value();
    float par6 = ui->doubleSpinBox_6_value->value();
    ui->doubleSpinBox_1_trans->setValue(par1);
    ui->doubleSpinBox_2_trans->setValue(par2);
    ui->doubleSpinBox_3_trans->setValue(par3);
    ui->doubleSpinBox_4_trans->setValue(par4);
    ui->doubleSpinBox_5_trans->setValue(par5);
    ui->doubleSpinBox_6_trans->setValue(par6);

    selSeries->clear();
    firstPoints.clear();
    limit = static_cast<int>(par5);

    for (i = 0; i < limit; i++)
    {
        firstPoints.append(QPointF(xValList.at(i), 0.0));
    }
    for (i = limit; i < xValList.count(); i++)
    {
        term1 = par2 * (xValList.at(i) - par1);
        term2 = par4 * (xValList.at(i) - (par1 * par6 + par3));
        sel = logist(term1) * logist(term2);
        firstPoints.append(QPointF(xValList.at(i), sel));
    }
    maxSel = maxYvalue(firstPoints);
    for (i = 0; i < firstPoints.count(); i++)
    {
        sel = (firstPoints.at(i).y() + .000001) / maxSel;
        selSeries->append(QPointF(xValList.at(i), sel));
    }
}

void equationDialog::dblLogPeak()
{
    if (!title.isEmpty())
        title.clear();
    title.append(QString("Double Logistic with peak - not fully implemented"));
    ui->label_title->setText(title);
    showSliders(8);
    ui->label_1_name->setText("Peak");
    ui->label_1_type->setText("Value");
    ui->label_2_name->setText("Init");
    ui->label_2_type->setText("Value");
    ui->label_3_name->setText("Infl");
    ui->label_3_type->setText("Logit");
    ui->label_4_name->setText("Slope");
    ui->label_4_type->setText("Value");
    ui->label_5_name->setText("Final");
    ui->label_5_type->setText("Logit");
    ui->label_6_name->setText("Infl");
    ui->label_6_type->setText("Logit");
    ui->label_7_name->setText("Slope");
    ui->label_7_type->setText("Value");
    ui->label_8_name->setText("BinWidth");
    ui->label_8_type->setText("Value");

    showBins(true);
    showJoins(3);

    resetChart();

    selSeries->setPen(QPen(QBrush(Qt::red), 3));
    cht->addSeries(selSeries);

    cht->setAxisX(axisXsel, selSeries);
    cht->setAxisY(axisY, selSeries);

    if (equationNum == 8)
        updateDblLogPeak();
    else
        updateDblLogSmooth();
}

void equationDialog::updateDblLogPeak()
{
    float maxSelX = 0;
    float minX = xValList.first();
    float maxX = xValList.last();
    float sel = 0;
    float t1 = 0, t1min, t1max, t1power;
    float t2 = 0, t2min, t2max, t2power;
    float jn1, jn2, jn3;
    float final, upsel, dnsel;
    int binM, i;
    float par1 = ui->doubleSpinBox_1_value->value();
    float par2 = ui->doubleSpinBox_2_value->value();
    float par3 = ui->doubleSpinBox_3_value->value();
    float par4 = ui->doubleSpinBox_4_value->value();
    float par5 = ui->doubleSpinBox_5_value->value();
    float par6 = ui->doubleSpinBox_6_value->value();
    float par7 = ui->doubleSpinBox_7_value->value();
    float par8 = ui->doubleSpinBox_8_value->value();
    ui->doubleSpinBox_1_trans->setValue(par1);
    ui->doubleSpinBox_2_trans->setValue(par2);
    ui->doubleSpinBox_3_trans->setValue(par3);
    ui->doubleSpinBox_4_trans->setValue(par4);
    ui->doubleSpinBox_5_trans->setValue(par5);
    ui->doubleSpinBox_6_trans->setValue(par6);
    ui->doubleSpinBox_7_trans->setValue(par7);
    ui->doubleSpinBox_8_trans->setValue(par8);

    selSeries->clear();
    firstPoints.clear();
//    t1=minL+(1./(1.+mfexp(-sp(3))))*(sp(1)-minL);    // INFL
//    t1min=1./(1.+mfexp(-mfexp(sp(4))*(minL-t1)))*0.9999;  // asc value at minsize
//    t1max=1./(1.+mfexp(-mfexp(sp(4))*(sp(1)-t1)))*1.0001;  // asc value at peak
//    t1power=log(0.5)/log((0.5-t1min)/(t1max-t1min));  // so the parameter will actual correspond to 50% point
    t1 = minX + (logist(par3) * (par1 - minX));
    t1min = 1./(1. + exp(-exp(par4) * (minX - t1))) * 0.9999;
    t1max = 1./(1. + exp(-exp(par4) * (par1 - t1))) * 1.0001;
    t1power = log(0.5)/log((0.5 - t1min)/(t1max - t1min));

//    if(seltype(f,4)==0) {sel_maxL=maxL;} else {sel_maxL=Ave_Size(styr,3,1,nages);}
    if (special == 0)
    {
        maxSelX = maxX;
    }
    else
    {
        maxSelX = aveXvalue(xValList);
    }
//    t2=(sp(1)+sp(8))+(1./(1.+mfexp(-sp(6))))*(sel_maxL-(sp(1)+sp(8)));    // INFL
//    t2min=1./(1.+mfexp(-mfexp(sp(7))*(sp(1)+sp(8)-t2)))*0.9999;  // asc value at peak+
//    t2max=1./(1.+mfexp(-mfexp(sp(7))*(sel_maxL-t2)))*1.0001;  // asc value at maxL
//    t2power=log(0.5)/log((0.5-t2min)/(t2max-t2min));
    t2 = (par1 + par8) + logist(par6) * (maxSelX - (par1 + par8));
    t2min = 1./(1. + exp(-exp(par7) * (par1 + par8 - t2))) * 0.9999;
    t2max = 1./(1. + exp(-exp(par7) * (maxSelX - t2))) * 1.0001;
    t2power = log(0.5) / log((0.5 - t2min)/(t2max - t2min));
//    final=1./(1.+mfexp(-sp(5)));
    final = logist (par5);

/*    for (j=1; j<=nlength; j++)  //calculate the value over length bins
    {   join1=1./(1.+mfexp(10.*(len_bins_m(j)-sp(1))));
        join2=1./(1.+mfexp(10.*(len_bins_m(j)-(sp(1)+sp(8)))));
        join3=1./(1.+mfexp(10.*(len_bins_m(j)-sel_maxL)));
        upselex=sp(2) + (1. - sp(2)) * pow((( 1./(1.+mfexp(-mfexp(sp(4))*(len_bins_m(j)-t1)))-t1min ) / (t1max-t1min)),t1power);
        downselex=(1. + (final - 1.) * pow(fabs(((( 1./(1.+mfexp(-mfexp(sp(7))*(len_bins_m(j)-t2))) -t2min ) / (t2max-t2min) ))),t2power));
        sel(j) = ((((upselex*join1)+1.0*(1.0-join1))*join2) + downselex*(1-join2))*join3 + final*(1-join3);
    }*/
    for (i = 0; i < xValList.count(); i++)
    {
        binM = xValList.at(i);
        jn1 = 1.0 / (1.0 + exp(10.0 * (binM - par1)));
        jn2 = 1.0 / (1.0 + exp(10.0 * (binM - (par1 + par8))));
        jn3 = 1.0 / (1.0 + exp(10.0 * (binM - maxSelX)));
        upsel = par2 + (1.0 - par2) * pow((( 1.0 / (1.0 + exp(-exp(par4) * (binM - t1))) - t1min) / (t1max - t1min)), t1power);
        dnsel = (1.0 + (final - 1.0) * pow(abs((((1.0 / (1.0 + exp(-exp(par7) * (binM - t2))) -t2min) / (t2max - t2min)))), t2power));
        sel = ((((upsel * jn1) + 1.0 * (1.0 - jn1)) * jn2) + dnsel * (1.0 - jn2)) * jn3 + final * (1.0 - jn3);
        selSeries->append(QPointF(xValList.at(i), sel));
    }
    ui->spinBox_steep_join1->setValue(10);
    ui->spinBox_steep_join2->setValue(10);
    ui->spinBox_steep_join3->setValue(10);
}

void equationDialog::dblLogSmooth()
{

}

void equationDialog::updateDblLogSmooth()
{
    float maxSelX = 0;
    float minX = xValList.first();
    float maxX = xValList.last();
    float sel = 0;
    float t1 = 0, t1min, t1max, t1power;
    float t2 = 0, t2min, t2max, t2power;
    float jn1, jn2, jn3;
    float final, upsel, dnsel;
    int binM, i;
    float par1 = ui->doubleSpinBox_1_value->value();
    float par2 = ui->doubleSpinBox_2_value->value();
    float par3 = ui->doubleSpinBox_3_value->value();
    float par4 = ui->doubleSpinBox_4_value->value();
    float par5 = ui->doubleSpinBox_5_value->value();
    float par6 = ui->doubleSpinBox_6_value->value();
    float par7 = ui->doubleSpinBox_7_value->value();
    float par8 = ui->doubleSpinBox_8_value->value();
    ui->doubleSpinBox_1_trans->setValue(par1);
    ui->doubleSpinBox_2_trans->setValue(par2);
    ui->doubleSpinBox_3_trans->setValue(par3);
    ui->doubleSpinBox_4_trans->setValue(par4);
    ui->doubleSpinBox_5_trans->setValue(par5);
    ui->doubleSpinBox_6_trans->setValue(par6);
    ui->doubleSpinBox_7_trans->setValue(par7);
    ui->doubleSpinBox_8_trans->setValue(par8);

    selSeries->clear();
    firstPoints.clear();

//    t1=0.+(1./(1.+mfexp(-sp(3))))*(sp(1)-0.);    // INFL
//    t1min=1./(1.+mfexp(-sp(4)*(0.-t1)))*0.9999;  // asc value at minsize
//    t1max=1./(1.+mfexp(-sp(4)*(sp(1)-t1)))*1.00001;  // asc value at peak
//    t1power=log(0.5)/log((0.5-t1min)/(t1max-t1min));
    t1 = 0.0 + (logist(par3) * (par1 - 0.0));
    t1min = 1.0/(1.0 + exp(-exp(par4) * (0.0 - t1))) * 0.9999;
    t1max = 1.0/(1.0 + exp(-exp(par4) * (par1 - t1))) * 1.0001;
    t1power = log(0.5)/log((0.5 - t1min)/(t1max - t1min));


//    t2=(sp(1)+sp(8))+(1./(1.+mfexp(-sp(6))))*(r_ages(nages)-(sp(1)+sp(8)));    // INFL
//    t2min=1./(1.+mfexp(-sp(7)*(sp(1)+sp(8)-t2)))*0.9999;  // asc value at peak+
//    t2max=1./(1.+mfexp(-sp(7)*(r_ages(nages)-t2)))*1.00001;  // asc value at maxage
//    t2power=log(0.5)/log((0.5-t2min)/(t2max-t2min));
    t2 = (par1 + par8) + logist(par6) * (binMax - (par1 + par8));
    t2min = 1./(1. + exp(-exp(par7) * (par1 + par8 - t2))) * 0.9999;
    t2max = 1./(1. + exp(-exp(par7) * (binMax - t2))) * 1.0001;
    t2power = log(0.5) / log((0.5 - t2min)/(t2max - t2min));
//    final=1./(1.+mfexp(-sp(5)));
    final = logist(par5);

/*    for (a=0; a<=nages; a++)  //calculate the value over ages
    {
        sel_a(y,fs,1,a) =
        (
        (
        (sp(2) + (1.-sp(2)) *
        pow((( 1./(1.+mfexp(-sp(4)*(r_ages(a)-t1)))-t1min)/ (t1max-t1min)),t1power))
        /(1.0+mfexp(30.*(r_ages(a)-sp(1))))  // scale ascending side
        +
        1./(1.+mfexp(-30.*(r_ages(a)-sp(1))))   // flattop, with scaling
        )
        /(1.+mfexp( 30.*(r_ages(a)-(sp(1)+sp(8)))))    // scale combo of ascending and flattop
        +
        (1. + (final - 1.) *
        pow(fabs((( 1./(1.+mfexp(-sp(7)*(r_ages(a)-t2))) -t2min ) / (t2max-t2min) )),t2power))
        /(1.+mfexp( -30.*(r_ages(a)-(sp(1)+sp(8)))))    // scale descending
        );
    }   // end age loop */
    for (i = 0; i < xValList.count(); i++)
    {
        binM = xValList.at(i);
        jn1 = 1.0 / (1.0 + exp(30.0 * (binM - par1)));
        jn2 = 1.0 / (1.0 + exp(30.0 * (binM - (par1 + par8))));
        upsel = par2 + (1.0 - par2) * pow((( 1.0 / (1.0 + exp(-par4 * (binM - t1))) - t1min) / (t1max - t1min)), t1power);
        dnsel = (1.0 + (final - 1.0) * pow(abs((((1.0 / (1.0 + exp(-par7 * (binM - t2))) -t2min) / (t2max - t2min)))), t2power));
        sel = ((((upsel * jn1) + jn1) * jn2) + dnsel * jn2);
        selSeries->append(QPointF(binM, sel));
    }
}

/** Case 22: size selectivity using double normal plateau
 *  (Similar to CASAL)
 *
 * peak2=sp(1)+ (0.99*len_bins(nlength)-sp(1))/(1.+mfexp(-sp(2)));
 * upselex=mfexp(sp(3));
 * downselex=mfexp(sp(4));
 * for (j=1;j<=nlength;j++)
 * {
 *     t1=len_bins_m(j);
 *     if(t1<sp(1))
 *     {
 *         sel(j)=mfexp(-square(t1-sp(1))/upselex);
 *     }
 *     else if (t1<=peak2)
 *     {
 *         sel(j)=1.0;
 *     }
 *     else
 *     {
 *         sel(j)=mfexp(-square(t1-peak2)/downselex);
 *     }
 * }
*/
void equationDialog::dblNormCasal()
{
    if (!title.isEmpty())
        title.clear();
    title.append(QString("Double Normal (like CASAL)"));
    ui->label_title->setText(title);
    showSliders(4);
    ui->label_1_name->setText("Peak");
    ui->label_1_type->setText("Value");
    ui->label_2_name->setText("Top");
    ui->label_2_type->setText("Log");
    ui->label_3_name->setText("Asc-width");
    ui->label_3_type->setText("Exp");
    ui->label_4_name->setText("Dsc-width");
    ui->label_4_type->setText("Exp");

    showBins(true);
    showJoins(0);

    resetChart();

    selSeries->setPen(QPen(QBrush(Qt::red), 3));
    cht->addSeries(selSeries);

    cht->setAxisX(axisXsel, selSeries);
    cht->setAxisY(axisY, selSeries);

    updateDblNormal();
}

void equationDialog::updateDblNormCasal()
{
    float xval, sel, peak2;
    float peak = ui->doubleSpinBox_1_value->value();
    float top = exp(-ui->doubleSpinBox_2_value->value());
    float upsel = exp(ui->doubleSpinBox_3_value->value());
    float dnsel = exp(ui->doubleSpinBox_4_value->value());
    ui->doubleSpinBox_1_trans->setValue(peak);
    ui->doubleSpinBox_2_trans->setValue(top);
    ui->doubleSpinBox_3_trans->setValue(upsel);
    ui->doubleSpinBox_4_trans->setValue(dnsel);

    peak2 = peak + (0.99 * xValList.last() - peak) / (1. + top);

    selSeries->clear();
    for (int i = 0; i < xValList.count(); i++)
    {
        xval = xValList.at(i);
        if (xval < peak)
        {
            sel = exp(-pow((xval - peak),2)/upsel);
        }
        else if (xval <= peak2)
        {
            sel = 1.0;
        }
        else
        {
            sel = exp(-pow((xval - peak2),2)/dnsel);
        }
        selSeries->append(QPointF(xval, sel));
    }
}

void equationDialog::dblNormal()
{
    if (!title.isEmpty())
        title.clear();
    title.append(QString("Double Normal - not fully implemented"));
    ui->label_title->setText(title);
    showSliders(4);
    ui->label_1_name->setText("Peak");
    ui->label_1_type->setText("Value");
    ui->label_2_name->setText("Top");
    ui->label_2_type->setText("Log");
    ui->label_3_name->setText("Asc-width");
    ui->label_3_type->setText("Exp");
    ui->label_4_name->setText("Dsc-width");
    ui->label_4_type->setText("Exp");

    showBins(true);
    showJoins(2);

    resetChart();

    selSeries->setPen(QPen(QBrush(Qt::red), 3));
    cht->addSeries(selSeries);

    cht->setAxisX(axisXsel, selSeries);
    cht->setAxisY(axisY, selSeries);

    updateDblNormal();
}

void equationDialog::updateDblNormal()
{

}

void equationDialog::dblNormEndpts()
{
    if (!title.isEmpty())
        title.clear();
    title.append(QString("Double Normal with defined ends - not fully implemented"));
    ui->label_title->setText(title);
    showSliders(6);
    ui->label_1_name->setText("Peak");
    ui->label_1_type->setText("Value");
    ui->label_2_name->setText("Top");
    ui->label_2_type->setText("Log");
    ui->label_3_name->setText("Asc-width");
    ui->label_3_type->setText("Exp");
    ui->label_4_name->setText("Dsc-width");
    ui->label_4_type->setText("Exp");
    ui->label_5_name->setText("Initial");
    ui->label_5_type->setText("Log");
    ui->label_6_name->setText("Final");
    ui->label_6_type->setText("Log");
    showBins(true);
    showJoins(2);

    resetChart();
    selSeries->setPen(QPen(QBrush(Qt::red), 3));
    cht->addSeries(selSeries);
    cht->setAxisX(axisXsel, selSeries);
    cht->setAxisY(axisY, selSeries);

    updateDblNormEndpts();
}

void equationDialog::updateDblNormEndpts()
{
    float par1 = ui->doubleSpinBox_1_value->value();
    float par2 = ui->doubleSpinBox_2_value->value();
    float par3 = ui->doubleSpinBox_3_value->value();
    float par4 = ui->doubleSpinBox_4_value->value();
    float par5 = ui->doubleSpinBox_5_value->value();
    float par6 = ui->doubleSpinBox_6_value->value();

    float peak = par1;
    float top = logist(par2);
    float asc_wd = exp(par3);
    float dsc_wd = exp(par4);
    float init = logist(par5);
    float final = logist(par6);

    ui->doubleSpinBox_1_trans->setValue(peak);
    ui->doubleSpinBox_2_trans->setValue(top);
    ui->doubleSpinBox_3_trans->setValue(asc_wd);
    ui->doubleSpinBox_4_trans->setValue(dsc_wd);
    ui->doubleSpinBox_5_trans->setValue(init);
    ui->doubleSpinBox_6_trans->setValue(final);

    ui->spinBox_steep_join1->setValue(20);
    ui->spinBox_steep_join2->setValue(20);

    selSeries->clear();


    updateTicks();
}

void equationDialog::expLogistic()
{
    if (!title.isEmpty())
        title.clear();
    title.append(QString("Exponential-Logistic"));
    ui->label_title->setText(title);
    showSliders(3);
    ui->label_1_name->setText("Width");
    ui->label_1_type->setText("Value");
    ui->label_2_name->setText("Peak");
    ui->label_2_type->setText("Scaled");
    ui->label_3_name->setText("Tail Ht");
    ui->label_3_type->setText("Value");
    showBins(true);
    showJoins(0);

    resetChart();

    selSeries->setPen(QPen(QBrush(Qt::red), 3));
    cht->addSeries(selSeries);
    cht->setAxisX(axisXsel, selSeries);
    cht->setAxisY(axisY, selSeries);

    updateExpLogistic();
}

void equationDialog::updateExpLogistic()
{
    int xRange = binMax - binMin;
    float yVal = 0;
    float temp = 0;
    float xVal = 0;
    float par1 = ui->doubleSpinBox_1_value->value();
    float par2 = ui->doubleSpinBox_2_value->value();
    float par3 = ui->doubleSpinBox_3_value->value();
    float peak = binMin + par2 * (xRange);
    ui->doubleSpinBox_1_trans->setValue(par1);
    ui->doubleSpinBox_2_trans->setValue(peak);
    ui->doubleSpinBox_3_trans->setValue(par3);

    selSeries->clear();

    for (int i = 0; i < xValList.count(); i++)
    {
        xVal = xValList.at(i);
        //sel_a(y,fs,1,a) = mfexp(sp(3)*sp(1)*(peak-r_ages(a)))/(1.0-sp(3)*(1.0-mfexp(sp(1)*(peak-r_ages(a)))));
        temp = 1.0 - par3 * (1 - exp(par1 * (peak - xVal)));
        yVal = exp(par3 * par1 * (peak - xVal))/temp;
        selSeries->append(QPointF(xVal, yVal));
    }
}

/** Returns the logistic of the requested value
 * 1/(1 + exp(-value)) */
float equationDialog::logist(double value)
{
    float temp = exp(-value);
    return (1./(1. + temp));
}

void equationDialog::eachAge ()
{
    if (!title.isEmpty())
        title.clear();
    title.append(QString("Value for each age"));
    ui->label_title->setText(title);
/*    binMin = xValList.at(0);
    binMax = binMin + (binWidth * selex->getNumAges());
    ui->spinBox_bins_min->setValue(binMin);
    ui->spinBox_bins_max->setValue(binMax);*/

    showSliders(0);
    showBins(true);
    showJoins(0);

    resetChart();

    selSeries->setPen(QPen(QBrush(Qt::red), 3));
    cht->addSeries(selSeries);
    cht->setAxisX(axisXsel, selSeries);
    cht->setAxisY(axisY, selSeries);

    updateEachAge();
}

void equationDialog::updateEachAge ()
{
    float sel = 0.;
    float parm = 0.;
    float temp, max = 0;
    float age = binMin + binMidWidth * binWidth;
    selSeries->clear();
    firstPoints.clear();
    for (int i = 0; i < parameters->rowCount(); i++)
    {
        temp = QString(parameters->getRowData(i).at(2)).toFloat();
        firstPoints.append(QPointF(age, temp));
        if (temp > max)
            max = temp;
        age += binWidth;
    }
    temp = 9. - max;
    sel = 0.;

    for (int i = 1; i < firstPoints.count(); i++)
    {
        parm = firstPoints.at(i).y();
        if (parm > -999)
        {
            sel = logist(parm + 9. - max);
        }
        selSeries->append(QPointF(firstPoints.at(i).x(), sel));
    }
}

void equationDialog::randomWalk (float scale)
{
    if (!title.isEmpty())
        title.clear();
    title.append(QString("Random Walk"));
    ui->label_title->setText(title);

    showSliders(0);
    showBins(true);
    showJoins(0);

    resetChart();

    selSeries->setPen(QPen(QBrush(Qt::red), 3));
    cht->addSeries(selSeries);
    cht->setAxisX(axisXsel, selSeries);
    cht->setAxisY(axisY, selSeries);
    axisY->setTitleText("Selex (red)");

    firstSeries = new QLineSeries(cht);
    firstSeries->setPen(QPen(QBrush(Qt::blue), 3));
    cht->addSeries(firstSeries);

    axisYalt->setTitleText("Use Parm (blue)");
    cht->addAxis(axisYalt, Qt::AlignRight);
    firstSeries->attachAxis(axisXsel);
    firstSeries->attachAxis(axisYalt);

    updateRandomWalk(scale);
}

void equationDialog::updateRandomWalk (float scale)
{
    float useparm = 0.;
    float sel = 0.;
    float parm = 0.;
    float cumparm = 0;
    float temp = 0, mean = 0, max = 0;
    QList<float> parms;
    int age = binMin;
    int lastage = binMax;
    int lastageIndex;
    int minAge = QString(parameters->getRowData(0).at(2)).toInt();
    int maxAge = QString(parameters->getRowData(1).at(2)).toInt();
    firstSeries->clear();
    selSeries->clear();
    firstPoints.clear();

    if (special != 0)
        lastage = abs(special);
    lastageIndex = xValList.indexOf(lastage);

    parm = QString(parameters->getRowData(0).at(2)).toFloat();
    if (parm == -1000)
    {
        parm = 0.0;
    }
    cumparm = useparm = parm;
    selSeries->append(QPointF(xValList.at(0), parm));

    parms.append(parm);
    firstSeries->append(QPointF(xValList.at(0), useparm));
    firstPoints.append(QPointF(xValList.at(0), cumparm));

    for (int i = 1; i <= lastageIndex; i++)
    {
        parm = QString(parameters->getRowData(i).at(2)).toFloat();
        parms.append(parm);
        if (parms[i] > -999)
            useparm = parms[i];
        if (fabs(useparm) > max)
            max = fabs(useparm);
        firstSeries->append(QPointF(xValList.at(i), useparm));
        cumparm += useparm;
        firstPoints.append(QPointF(xValList.at(i), cumparm));
    }
    axisYalt->setRange(-max, max);

    if (scale == 0)
    {
        temp = maxYvalue(firstPoints);
    }
    else
    {
        int low_bin;
        int high_bin;
        float total = 0;
        if (minAge < 0)
        {
            minAge = 0;
        }
        if (maxAge > binMax)
        {
            maxAge = binMax;
        }
        if (maxAge < minAge)
            maxAge = minAge;
        if (minAge > maxAge)
            minAge = maxAge;

        low_bin = xValList.indexOf(minAge);
        high_bin = xValList.indexOf(maxAge);
//        sp(1) = low_bin;
//        sp(2) = high_bin;
//        temp = mean(tempvec_a(low_bin,high_bin));

        for (int i = low_bin; i <= high_bin; i++)
            total += firstPoints.at(i).y();
        temp = total / (high_bin - low_bin + 1);
    }

    for (int i = 1; i < firstPoints.count(); i++)
    {
        int next = i + 1 + scale;
        if ((next < parms.count()) && (parms.at(next) == -1000))
            sel = 0.0;
        else
            sel = exp(firstPoints.at(i).y() - temp);
        selSeries->append(QPointF(xValList.at(i), sel));
    }

    for (int i = firstPoints.count(); i < xValList.count(); i++)
    {
        if (special > 0)
            selSeries->append(QPointF(xValList.at(i), selSeries->points().at(i-1).y()));
        else
            selSeries->append(QPointF(xValList.at(i), 0.0));
    }
}

void equationDialog::coleGauss ()
{
    if (!title.isEmpty())
        title.clear();
    title.append(QString("Coleraine single Gaussian"));
    ui->label_title->setText(title);

    showSliders(2);
    ui->label_1_name->setText("Value");
    ui->label_1_type->setText("Value");
    ui->label_2_name->setText("Value");
    ui->label_2_type->setText("Value");
    showBins(true);
    showJoins(0);

    resetChart();

    selSeries->setPen(QPen(QBrush(Qt::red), 3));
    cht->addSeries(selSeries);
    cht->setAxisX(axisXsel, selSeries);
    cht->setAxisY(axisY, selSeries);

    updateColeGauss();
}

void equationDialog::updateColeGauss()
{
    float temp = 0;
    float sel = 0;
    float par1 = ui->doubleSpinBox_1_value->value();
    float par2 = ui->doubleSpinBox_2_value->value();
    ui->doubleSpinBox_1_trans->setValue(par1);
    ui->doubleSpinBox_2_trans->setValue(par2);

    selSeries->clear();

    temp = logist(par1) * xValList.count();
    for (int i = 0; i < xValList.count(); i++)
    {
        if (i < temp)
        {
            sel = exp(-1*pow(xValList.at(i) - temp, 2) / exp(par2));
        }
        else
        {
            sel = 1.0;
        }
        selSeries->append(QPointF(xValList.at(i), sel));
    }
}

/** Returns the maximum x-value of a point list */
float equationDialog::maxXvalue(QList<QPointF> &pointlist)
{
    float value = -1000;
    for (int i = 0; i < pointlist.count(); i++)
    {
        if (pointlist.at(i).x() > value)
            value = pointlist.at(i).x();
    }
    return value;
}

/** Returns the maximum y-value of a point list */
float equationDialog::maxYvalue(QList<QPointF> &pointlist)
{
    float value = -1000;
    for (int i = 0; i < pointlist.count(); i++)
    {
        if (pointlist.at(i).y() > value)
            value = pointlist.at(i).y();
    }
    return value;
}

/** Returns the average of the values of a float list */
float equationDialog::aveXvalue(QList<float> &xvals)
{
    float value = 0.;
    int i = 0;
    for (i = 0; i < xvals.count(); i++)
        value += (float)xvals.at(i);
    value /= i;
    return value;
}

/** Returns the average of the y-values of a point list */
float equationDialog::aveYvalue(QList<QPointF> &pointlist)
{
    float value = 0.;
    int i = 0;
    for (i = 0; i < pointlist.count(); i++)
    {
        if (pointlist.at(i).y() > value)
            value += pointlist.at(i).y();
    }
    return (value / i);
}
