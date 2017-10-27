#include "equationdialog.h"
#include "ui_equationdialog.h"

#include <QAbstractButton>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

#define SLIDER_SPAN  100000

#include <cmath>

equationDialog::equationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::equationDialog)
{
    ui->setupUi(this);

    title = QString("None");

    selex = NULL;
    dataModel = NULL;
    fleet = NULL;

    equationNum = 0;
    parameters = NULL;

    yMax = -10;
    binMin = 0;
    binMax = 20;
    binMidWidth = .5;
    binWidth = 2;

    chartview = new QChartView(this);
    cht = new QChart();
    firstSeries = NULL;
    lastSeries = NULL;
    selSeries = new QLineSeries(cht);
    selSeries->append(0, 0);
    cht->addSeries(selSeries);
    chartview->setChart(cht);
    ui->verticalLayout_graph->addWidget(chartview, 1);
    axisXsel = new QValueAxis();
    axisYalt = new QValueAxis();
    axisY = new QValueAxis();

    ui->horizontalSlider_1->setMaximum(SLIDER_SPAN);
    ui->horizontalSlider_2->setMaximum(SLIDER_SPAN);
    ui->horizontalSlider_3->setMaximum(SLIDER_SPAN);
    ui->horizontalSlider_4->setMaximum(SLIDER_SPAN);
    ui->horizontalSlider_5->setMaximum(SLIDER_SPAN);
    ui->horizontalSlider_6->setMaximum(SLIDER_SPAN);
    ui->horizontalSlider_7->setMaximum(SLIDER_SPAN);
    ui->horizontalSlider_8->setMaximum(SLIDER_SPAN);

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
    connect (this, SIGNAL(linearUpdated()), SLOT(updateLinearExp()));

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

    switch (numParams)
    {
    case 9:
        values = parameters->getRowData(8);
        min = (values.at(0)).toDouble();
        max = (values.at(1)).toDouble();
        val = (values.at(2)).toDouble();
        ui->doubleSpinBox_9_input->setMinimum(min);
        ui->doubleSpinBox_9_input->setMaximum(max);
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
    }

    connect (this, SIGNAL(numbersUpdated()), SLOT(updateSel()));
    emit numbersUpdated();
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
    if (parameters->rowCount() > 2)
    {
        values = parameters->getRowData(2);
        values[0] = QString::number(ui->doubleSpinBox_3_min->value());
        values[1] = QString::number(ui->doubleSpinBox_3_max->value());
        values[2] = QString::number(ui->doubleSpinBox_3_input->value());
        parameters->setRowData(2, values);
    }
    if (parameters->rowCount() > 3)
    {
        values = parameters->getRowData(3);
        values[0] = QString::number(ui->doubleSpinBox_4_min->value());
        values[1] = QString::number(ui->doubleSpinBox_4_max->value());
        values[2] = QString::number(ui->doubleSpinBox_4_input->value());
        parameters->setRowData(3, values);
    }
    if (parameters->rowCount() > 4)
    {
        values = parameters->getRowData(4);
        values[0] = QString::number(ui->doubleSpinBox_5_min->value());
        values[1] = QString::number(ui->doubleSpinBox_5_max->value());
        values[2] = QString::number(ui->doubleSpinBox_5_input->value());
        parameters->setRowData(4, values);
    }
    if (parameters->rowCount() > 5)
    {
        values = parameters->getRowData(5);
        values[0] = QString::number(ui->doubleSpinBox_6_min->value());
        values[1] = QString::number(ui->doubleSpinBox_6_max->value());
        values[2] = QString::number(ui->doubleSpinBox_6_input->value());
        parameters->setRowData(5, values);
    }
    if (parameters->rowCount() > 6)
    {
        values = parameters->getRowData(6);
        values[0] = QString::number(ui->doubleSpinBox_7_min->value());
        values[1] = QString::number(ui->doubleSpinBox_7_max->value());
        values[2] = QString::number(ui->doubleSpinBox_7_input->value());
        parameters->setRowData(6, values);
    }
    if (parameters->rowCount() > 7)
    {
        values = parameters->getRowData(7);
        values[0] = QString::number(ui->doubleSpinBox_8_min->value());
        values[1] = QString::number(ui->doubleSpinBox_8_max->value());
        values[2] = QString::number(ui->doubleSpinBox_8_input->value());
        parameters->setRowData(7, values);
    }
    if (parameters->rowCount() > 8)
    {
        values = parameters->getRowData(8);
        values[0] = QString::number(ui->doubleSpinBox_9_min->value());
        values[1] = QString::number(ui->doubleSpinBox_9_max->value());
        values[2] = QString::number(ui->doubleSpinBox_9_input->value());
        parameters->setRowData(8, values);
    }
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

    building = true;

    switch (equationNum)
    {
    case 0:
    case 10:
        constant(1.0);
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

    case 4:
        blank(0, 30);
        break;

    case 5:
        sliders = 2;
    case 15:
        ui->label_1_type->setText("Value");
        ui->label_2_type->setText("Value");
        msg = QString(QString("Mirror of Fleet(%1)").arg(QString::number(getSpecial())));
        blank(sliders, 0, msg);
        break;

    case 6:
        linear();
        break;

    case 8:
        blank(0);
        break;

    default:
        blank(0);
        break;
    }

    building = false;
}

void equationDialog::updateSel()
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
        updateLogistic();
        break;

    case 6:
        updateLinear();
        break;
    default:
        blank(0);
        break;
    }
}

void equationDialog::setSlider1(double min, double max, double value)
{
    min1 = min; max1 = max;
    ui->doubleSpinBox_1_min->setValue(min);
    ui->doubleSpinBox_1_max->setValue(max);
    ui->doubleSpinBox_1_value->setMinimum(min1);
    ui->doubleSpinBox_1_value->setMaximum(max1);
    ui->doubleSpinBox_1_value->setValue(value);
    ui->horizontalSlider_1->setMinimum(min1);
    ui->horizontalSlider_1->setMaximum(max1);
    ui->horizontalSlider_1->setValue(value);
}

void equationDialog::slider1Changed(int value)
{
    double val = ui->doubleSpinBox_1_value->value();
    val = value + (val - static_cast<int>(val));
    ui->doubleSpinBox_1_value->setValue(val);
}

void equationDialog::value1Changed (double value)
{
    int val = (int)(value);
    ui->horizontalSlider_1->setValue(val);
    emit numbersUpdated();
//    setTrans1(value);
}

void equationDialog::min1Changed (double value)
{
    min1 = value;
    if (min1 > max1)
    {
        min1 = max1;
    }
    ui->doubleSpinBox_1_min->setValue(min1);
    ui->doubleSpinBox_1_value->setMinimum(min1);
    ui->horizontalSlider_1->setMinimum(min1);
}

void equationDialog::max1Changed (double value)
{
    max1 = value;
    if (max1 < min1)
    {
        max1 = min1;
    }
    ui->doubleSpinBox_1_max->setValue(max1);
    ui->doubleSpinBox_1_input->setMaximum(max1);
    ui->horizontalSlider_1->setMaximum(max1);
}

void equationDialog::setSlider2(float min, float max, float value)
{
    min2 = min; max2 = max;
    ui->doubleSpinBox_2_min->setValue(min);
    ui->doubleSpinBox_2_max->setValue(max);
    ui->doubleSpinBox_2_value->setMinimum(min2);
    ui->doubleSpinBox_2_value->setMaximum(max2);
    ui->doubleSpinBox_2_value->setValue(value);
    ui->horizontalSlider_2->setMinimum(min2);
    ui->horizontalSlider_2->setMaximum(max2);
    ui->horizontalSlider_2->setValue(value);
}

void equationDialog::slider2Changed(int value)
{
    double val = ui->doubleSpinBox_2_value->value();
    val = value + (val - static_cast<int>(val));
    ui->doubleSpinBox_2_value->setValue(val);
}

void equationDialog::value2Changed (double value)
{
    int val = (int)(value);
    ui->horizontalSlider_2->setValue(val);
    emit numbersUpdated();
//    setTrans2(value);
}

void equationDialog::min2Changed (double value)
{
    min2 = value;
    if (min2 > max2)
    {
        min2 = max2;
    }
    ui->doubleSpinBox_2_min->setValue(min2);
    ui->doubleSpinBox_2_value->setMinimum(min2);
    ui->horizontalSlider_2->setMinimum(min2);
}

void equationDialog::max2Changed (double value)
{
    max2 = value;
    if (max2 < min2)
    {
        max2 = min2;
    }
    ui->doubleSpinBox_2_max->setValue(max2);
    ui->doubleSpinBox_2_input->setMaximum(max2);
    ui->horizontalSlider_2->setMaximum(max2);
}

void equationDialog::setSlider3(float min, float max, float value)
{
    min3 = min; max3 = max;
    ui->doubleSpinBox_3_min->setValue(min);
    ui->doubleSpinBox_3_max->setValue(max);
    ui->doubleSpinBox_3_value->setMinimum(min3);
    ui->doubleSpinBox_3_value->setMaximum(max3);
    ui->doubleSpinBox_3_value->setValue(value);
    ui->horizontalSlider_3->setMinimum(min3);
    ui->horizontalSlider_3->setMaximum(max3);
    ui->horizontalSlider_3->setValue(value);
}

void equationDialog::slider3Changed(int value)
{
    double val = ui->doubleSpinBox_3_value->value();
    val = value + (val - static_cast<int>(val));
    ui->doubleSpinBox_3_value->setValue(val);
}

void equationDialog::value3Changed (double value)
{
    int val = (int)(value);
    ui->horizontalSlider_3->setValue(val);
    emit numbersUpdated();
//    setTrans3(value);
}

void equationDialog::min3Changed (double value)
{
    min3 = value;
    if (min3 > max3)
    {
        min3 = max3;
        ui->doubleSpinBox_3_min->setValue(min3);
    }
    else
    {
        ui->doubleSpinBox_3_value->setMinimum(min3);
        ui->horizontalSlider_3->setMinimum(min3);
    }
}

void equationDialog::max3Changed (double value)
{
    max3 = value;
    if (max3 < min3)
    {
        max3 = min3;
        ui->doubleSpinBox_3_max->setValue(max3);
    }
    else
    {
        ui->doubleSpinBox_3_input->setMaximum(max3);
        ui->horizontalSlider_3->setMaximum(max3);
    }
}

void equationDialog::setSlider4(float min, float max, float value)
{
    min4 = min; max4 = max;
    ui->doubleSpinBox_4_min->setValue(min);
    ui->doubleSpinBox_4_max->setValue(max);
    ui->doubleSpinBox_4_value->setMinimum(min4);
    ui->doubleSpinBox_4_value->setMaximum(max4);
    ui->doubleSpinBox_4_value->setValue(value);
    ui->horizontalSlider_4->setMinimum(min4);
    ui->horizontalSlider_4->setMaximum(max4);
    ui->horizontalSlider_4->setValue(value);
}

void equationDialog::slider4Changed(int value)
{
    double val = ui->doubleSpinBox_4_value->value();
    val = value + (val - static_cast<int>(val));
    ui->doubleSpinBox_4_value->setValue(val);
}

void equationDialog::value4Changed (double value)
{
    int val = (int)(value);
    ui->horizontalSlider_4->setValue(val);
    emit numbersUpdated();
//    setTrans4(value);
}

void equationDialog::min4Changed (double value)
{
    min4 = value;
    if (min4 > max4)
    {
        ui->doubleSpinBox_4_min->setValue(max4);
    }
    else
    {
        ui->doubleSpinBox_4_value->setMinimum(min4);
        ui->horizontalSlider_4->setMinimum(min4);
    }
}

void equationDialog::max4Changed (double value)
{
    max4 = value;
    if (max4 < min4)
    {
        ui->doubleSpinBox_4_max->setValue(min4);
    }
    else
    {
        ui->doubleSpinBox_4_input->setMaximum(max4);
        ui->horizontalSlider_4->setMaximum(max4);
    }
}

void equationDialog::setSlider5(float min, float max, float value)
{
    min5 = min; max5 = max;
    ui->doubleSpinBox_5_min->setValue(min);
    ui->doubleSpinBox_5_max->setValue(max);
    ui->doubleSpinBox_5_value->setMinimum(min5);
    ui->doubleSpinBox_5_value->setMaximum(max5);
    ui->doubleSpinBox_5_value->setValue(value);
    ui->horizontalSlider_5->setMinimum(min5);
    ui->horizontalSlider_5->setMaximum(max5);
    ui->horizontalSlider_5->setValue(value);
}

void equationDialog::slider5Changed(int value)
{
    double val = ui->doubleSpinBox_5_value->value();
    val = value + (val - static_cast<int>(val));
    ui->doubleSpinBox_5_value->setValue(val);
}

void equationDialog::value5Changed (double value)
{
    int val = (int)(value);
    ui->horizontalSlider_5->setValue(val);
    emit numbersUpdated();
//    setTrans5(value);
}

void equationDialog::min5Changed (double value)
{
    min5 = value;
    if (min5 > max5)
    {
        ui->doubleSpinBox_5_min->setValue(max5);
    }
    else
    {
        ui->doubleSpinBox_5_value->setMinimum(min5);
        ui->horizontalSlider_5->setMinimum(min5);
    }
}

void equationDialog::max5Changed (double value)
{
    max5 = value;
    if (max5 < min5)
    {
        ui->doubleSpinBox_5_max->setValue(min5);
    }
    else
    {
        ui->doubleSpinBox_5_input->setMaximum(max5);
        ui->horizontalSlider_5->setMaximum(max5);
    }
}

void equationDialog::setSlider6(float min, float max, float value)
{
    min6 = min; max6 = max;
    ui->doubleSpinBox_6_min->setValue(min);
    ui->doubleSpinBox_6_max->setValue(max);
    ui->doubleSpinBox_6_value->setMinimum(min6);
    ui->doubleSpinBox_6_value->setMaximum(max6);
    ui->doubleSpinBox_6_value->setValue(value);
    ui->horizontalSlider_6->setMinimum(min6);
    ui->horizontalSlider_6->setMaximum(max6);
    ui->horizontalSlider_6->setValue(value);
}

void equationDialog::slider6Changed(int value)
{
    double val = ui->doubleSpinBox_6_value->value();
    val = value + (val - static_cast<int>(val));
    ui->doubleSpinBox_6_value->setValue(val);
}

void equationDialog::value6Changed (double value)
{
    int val = (int)(value);
    ui->horizontalSlider_6->setValue(val);
    emit numbersUpdated();
//    setTrans6(value);
}

void equationDialog::min6Changed (double value)
{
    min6 = value;
    if (min6 > max6)
    {
        ui->doubleSpinBox_6_min->setValue(max6);
    }
    else
    {
        ui->doubleSpinBox_6_value->setMinimum(min6);
        ui->horizontalSlider_6->setMinimum(min6);
    }
}

void equationDialog::max6Changed (double value)
{
    max6 = value;
    if (max6 < min6)
    {
        ui->doubleSpinBox_6_max->setValue(min6);
    }
    else
    {
        ui->doubleSpinBox_6_input->setMaximum(max6);
        ui->horizontalSlider_6->setMaximum(max6);
    }
}

void equationDialog::setSlider7(float min, float max, float value)
{
    min7 = min; max7 = max;
    ui->doubleSpinBox_7_min->setValue(min);
    ui->doubleSpinBox_7_max->setValue(max);
    ui->doubleSpinBox_7_value->setMinimum(min7);
    ui->doubleSpinBox_7_value->setMaximum(max7);
    ui->doubleSpinBox_7_value->setValue(value);
    ui->horizontalSlider_7->setMinimum(min7);
    ui->horizontalSlider_7->setMaximum(max7);
    ui->horizontalSlider_7->setValue(value);
}

void equationDialog::slider7Changed(int value)
{
    double val = ui->doubleSpinBox_7_value->value();
    val = value + (val - static_cast<int>(val));
    ui->doubleSpinBox_7_value->setValue(val);
}

void equationDialog::value7Changed (double value)
{
    int val = (int)(value);
    ui->horizontalSlider_7->setValue(val);
    emit numbersUpdated();
//    setTrans4(value);
}

void equationDialog::min7Changed (double value)
{
    min7 = value;
    if (min7 > max7)
    {
        ui->doubleSpinBox_7_min->setValue(max7);
    }
    else
    {
        ui->doubleSpinBox_7_value->setMinimum(min7);
        ui->horizontalSlider_7->setMinimum(min7);
    }
}

void equationDialog::max7Changed (double value)
{
    max7 = value;
    if (max7 < min7)
    {
        ui->doubleSpinBox_7_max->setValue(min7);
    }
    else
    {
        ui->doubleSpinBox_7_input->setMaximum(max7);
        ui->horizontalSlider_7->setMaximum(max7);
    }
}

void equationDialog::setSlider8(float min, float max, float value)
{
    min8 = min; max8 = max;
    ui->doubleSpinBox_8_min->setValue(min);
    ui->doubleSpinBox_8_max->setValue(max);
    ui->doubleSpinBox_8_value->setMinimum(min8);
    ui->doubleSpinBox_8_value->setMaximum(max8);
    ui->doubleSpinBox_8_value->setValue(value);
    ui->horizontalSlider_8->setMinimum(min8);
    ui->horizontalSlider_8->setMaximum(max8);
    ui->horizontalSlider_8->setValue(value);
}

void equationDialog::slider8Changed(int value)
{
    double val = ui->doubleSpinBox_8_value->value();
    val = value + (val - static_cast<int>(val));
    ui->doubleSpinBox_8_value->setValue(val);
}

void equationDialog::value8Changed (double value)
{
    int val = (int)(value);
    ui->horizontalSlider_8->setValue(val);
    emit numbersUpdated();
//    setTrans4(value);
}

void equationDialog::min8Changed (double value)
{
    min8 = value;
    if (min8 > max8)
    {
        ui->doubleSpinBox_8_min->setValue(max8);
    }
    else
    {
        ui->doubleSpinBox_8_value->setMinimum(min8);
        ui->horizontalSlider_8->setMinimum(min8);
    }
}

void equationDialog::max8Changed (double value)
{
    max8 = value;
    if (max8 < min8)
    {
        ui->doubleSpinBox_8_max->setValue(min8);
    }
    else
    {
        ui->doubleSpinBox_8_input->setMaximum(max8);
        ui->horizontalSlider_8->setMaximum(max8);
    }
}

void equationDialog::setSlider9(float min, float max, float value)
{
    min9 = min; max9 = max;
    ui->doubleSpinBox_9_min->setValue(min);
    ui->doubleSpinBox_9_max->setValue(max);
    ui->doubleSpinBox_9_value->setMinimum(min9);
    ui->doubleSpinBox_9_value->setMaximum(max9);
    ui->doubleSpinBox_9_value->setValue(value);
    ui->horizontalSlider_9->setMinimum(min9);
    ui->horizontalSlider_9->setMaximum(max9);
    ui->horizontalSlider_9->setValue(value);
}

void equationDialog::slider9Changed(int value)
{
    double val = ui->doubleSpinBox_9_value->value();
    val = value + (val - static_cast<int>(val));
    ui->doubleSpinBox_9_value->setValue(val);
}

void equationDialog::value9Changed (double value)
{
    int val = (int)(value);
    ui->horizontalSlider_9->setValue(val);
    emit numbersUpdated();
//    setTrans4(value);
}

void equationDialog::min9Changed (double value)
{
    min9 = value;
    if (min9 > max9)
    {
        ui->doubleSpinBox_9_min->setValue(max9);
    }
    else
    {
        ui->doubleSpinBox_9_value->setMinimum(min9);
        ui->horizontalSlider_9->setMinimum(min9);
    }
}

void equationDialog::max9Changed (double value)
{
    max9 = value;
    if (max9 < min9)
    {
        ui->doubleSpinBox_9_max->setValue(min9);
    }
    else
    {
        ui->doubleSpinBox_9_input->setMaximum(max9);
        ui->horizontalSlider_9->setMaximum(max9);
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
    // set input spinboxes
    ui->doubleSpinBox_1_input->setValue(ui->doubleSpinBox_1_value->value());
    ui->doubleSpinBox_2_input->setValue(ui->doubleSpinBox_2_value->value());
    ui->doubleSpinBox_3_input->setValue(ui->doubleSpinBox_3_value->value());
    ui->doubleSpinBox_4_input->setValue(ui->doubleSpinBox_4_value->value());
    ui->doubleSpinBox_5_input->setValue(ui->doubleSpinBox_5_value->value());
    ui->doubleSpinBox_6_input->setValue(ui->doubleSpinBox_6_value->value());
    ui->doubleSpinBox_7_input->setValue(ui->doubleSpinBox_7_value->value());
    ui->doubleSpinBox_8_input->setValue(ui->doubleSpinBox_8_value->value());
    ui->doubleSpinBox_9_input->setValue(ui->doubleSpinBox_9_value->value());
    // write values to parameters
    setParameterValues();

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
    emit numbersUpdated();
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
    /*            templist = parameters->getRowData(0);
                min = QString(templist.at(0)).toFloat();
                max = QString(templist.at(1)).toFloat();
                init = QString(templist.at(2)).toFloat();
                setSlider1(min, max, init);
                templist = parameters->getRowData(1);
                min = QString(templist.at(0)).toFloat();
                max = QString(templist.at(1)).toFloat();
                init = QString(templist.at(2)).toFloat();
                setSlider2(min, max, init);*/
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


void equationDialog::updateTicks(QRectF rect)
{
    int xTicks = rect.width() / 100;
    int yTicks = rect.height() / 60;

    axisXsel->setRange((binMin), (binMax + binWidth));
    axisXsel->setTickCount(xTicks % 2? xTicks: xTicks + 1);

    axisY->setRange(0, 1.2);
    axisY->setTickCount(yTicks % 2? yTicks: yTicks + 1);
}

void equationDialog::blank (int num, int rep, QString msg)
{
    // If replaced by another pattern
    if (rep > 0)
    {
        msg.clear();
        msg.append(QString("Discontinued: use pattern %1 instead.").arg(QString::number(rep)));
    }
    // This is for equations not yet implemented.
    // INTENTIONALLY LEFT BLANK
    if (msg.isEmpty())
        msg.append ("This equation is not yet implemented.\n\n THIS PAGE INTENTIONALLY LEFT BANK");

    ui->label_title->setText(msg);
    showSliders(num);
    showBins (num > 0);
    showJoins(0);
    chartview->hide();
}

/* Size selex equation 0, age selex equation 10 */
void equationDialog::constant (float val)
{
    int xTicks = 9;
    int yTicks = 7;
    int xRange = binMax - binMin;
    float binMid = binWidth * binMidWidth;

    if (!title.isEmpty())
        title.clear();
    title.append(QString("Constant Value %1").arg(QString::number(val,'g',2)));
    ui->label_title->setText(title);
    showSliders(0);
    showBins(true);
    showJoins(0);

    delete axisXsel;
    delete axisY;
    delete selSeries;
    delete cht;
    delete chartview;
    chartview = new QChartView(this);
    ui->verticalLayout_graph->addWidget(chartview);
    chartview->setChart(new QChart());
    cht = chartview->chart();
    cht->legend()->setVisible(false);

    selSeries = new QLineSeries(cht);
    selSeries->append(QPointF(binMin + binMid, val));
    selSeries->append(QPointF(binMax + binMid, val));
    selSeries->setPen(QPen(QBrush(Qt::red), 3));
    cht->addSeries(selSeries);

    axisXsel = new QValueAxis();
    axisXsel->setRange((binMin), (binMax + binWidth));
    axisXsel->setTickCount(xTicks % 2? xTicks: xTicks + 1);
    axisY = new QValueAxis();
    axisY->setRange(0, (val + .2));
    axisY->setTickCount(yTicks % 2? yTicks: yTicks + 1);
//    axisX->applyNiceNumbers();
//    axisY->applyNiceNumbers();
    cht->setAxisX(axisXsel, selSeries);
    cht->setAxisY(axisY, selSeries);
    updateTicks(cht->rect());
}

/* Size selex equation 1, age selex equation 12 */
void equationDialog::logistic ()
{
    int xRange = binMax - binMin;
    int xTicks = 9;
    int yTicks = 7;
    float yVal = 0;
    float temp = 0;
    float len = 0;
    float par1 = ui->doubleSpinBox_1_trans->value();
    float par2 = ui->doubleSpinBox_2_trans->value();

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

    delete axisXsel;
    delete axisY;
    delete selSeries;
    delete cht;
    delete chartview;
    chartview = new QChartView(this);
    ui->verticalLayout_graph->addWidget(chartview);
    cht = new QChart();
    chartview->setChart(cht);

    cht->legend()->setVisible(false);
    xTicks = cht->rect().width() / 100;
    yTicks = cht->rect().height() / 60;

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

    axisXsel = new QValueAxis();
    axisXsel->setRange((binMin), (binMax + binWidth));
    axisXsel->setTickCount(xTicks % 2? xTicks: xTicks + 1);
    axisY = new QValueAxis();
    axisY->setRange(0, (1 + .2));
    axisY->setTickCount(yTicks % 2? yTicks: yTicks + 1);
//    axisXsel->applyNiceNumbers();
//    axisY->applyNiceNumbers();
    cht->setAxisX(axisXsel, selSeries);
    cht->setAxisY(axisY, selSeries);
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

    delete selSeries;

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

    cht->setAxisX(axisXsel, selSeries);
    cht->setAxisY(axisY, selSeries);
}

/* Size selex equation 6, age selex equation 16 */
void equationDialog::linear ()
{
    float y = 0;
    float temp = 0;
    float len = 0;
    int num = getSpecial();
    float xVal[10];
    float yVal[10];
    int xTicks = 9;
    int yTicks = 5;

    if (num > 7)
    {
        QString msg("Too many points for this display!");
        blank(9, 0, msg);
    }


    if (!title.isEmpty())
        title.clear();
    title.append(QString("%1 Linear segments").arg(QString::number(num)));
    ui->label_title->setText(title);
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

    chartview->setChart(new QChart());
    delete cht;
    cht = chartview->chart();
    cht->legend()->setVisible(false);
    xTicks = cht->rect().width() / 100;
    yTicks = cht->rect().height() / 50;

    axisXsel = new QValueAxis();
    axisXsel->setRange((binMin), (binMax + binWidth));
    axisXsel->setTickCount(xTicks % 2? xTicks: xTicks + 1);
    axisY = new QValueAxis();
    axisY->setRange(0, (1 + .2));
    axisY->setTickCount(yTicks % 2? yTicks: yTicks + 1);
    axisY->setTitleText("Selex (red)");

    updateLinear();
}

void equationDialog::updateLinear()
{
    float y = 0;
    int axisYMax = 0;
    int axisYMin = 0;
    float temp = 0;
    float len = 0;
    int num = getSpecial();
    float xVal[10];
    float yVal[10];

    if (axisYalt != NULL)
        delete axisYalt;
    if (firstSeries != NULL)
        delete firstSeries;
    if (lastSeries != NULL)
        delete lastSeries;

    if (num > 7)
    {
        QString msg("Too many points for this display!");
        blank(7, 0, msg);
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

    firstSeries = new QLineSeries(cht);
//    selSeries = new QLineSeries(cht);
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
    firstSeries->setPen(QPen(QBrush(Qt::blue), 2));
    cht->addSeries(firstSeries);

    axisYalt = new QValueAxis();
    axisYalt->setRange(axisYMin, axisYMax);
    axisYalt->setTitleText("Ln of Selex (blue)");

    cht->addAxis(axisYalt, Qt::AlignRight);
    firstSeries->attachAxis(axisXsel);
    firstSeries->attachAxis(axisYalt);
//    cht->setAxisX(axisXsel, firstSeries);
//    cht->setAxisY(axisYalt, firstSeries);
    emit linearUpdated();
}

void equationDialog::updateLinearExp()
{
    float yVal;
    QList<QPointF> points = firstSeries->points();

    delete selSeries;
    selSeries = new QLineSeries(cht);


    for (int i = 0; i < firstPoints.count(); i++)
    {
        yVal = exp(firstPoints.at(i).y() - yMax);
        selSeries->append(firstPoints.at(i).x(), yVal);
    }
    selSeries->setPen(QPen(QBrush(Qt::red), 3));
    cht->addSeries(selSeries);

    cht->addAxis(axisY, Qt::AlignLeft);
    cht->addAxis(axisXsel, Qt::AlignBottom);
    selSeries->attachAxis(axisXsel);
    selSeries->attachAxis(axisY);
//    cht->setAxisX(axisXsel, selSeries);
//    cht->setAxisY(axisY, selSeries);
}


float equationDialog::evaluateLine(QPointF pt1, QPointF pt2, float x)
{
    float slope = (pt2.y() - pt1.y()) / (pt2.x() - pt1.x());
    float y = -1 * (slope * (pt2.x() - x) - pt2.y());
    return y;
}
