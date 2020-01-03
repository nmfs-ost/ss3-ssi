#include "srequationdialog.h"
#include "ui_srequationdialog.h"

#include <QAbstractButton>
#include <QChart>
#include <QLineSeries>
#include <QValueAxis>
#include <QScatterSeries>
#include <QChartView>
#include <QMessageBox>

QT_CHARTS_USE_NAMESPACE

using namespace tk;

#ifndef PRECISION
#define PRECISION .001
#define HALFPREC .0005
#endif
#ifndef SLIDER_SCALE
#define SLIDER_SCALE 1000
#define VALUE_SCALE .001
#endif

#include <cmath>

#define XMAX   (100)

//static double neglog19 = -1.0 * log(19);

srEquationDialog::srEquationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::srEquationDialog)
{
    ui->setupUi(this);
    name = QString (QString("Spawner-Recruitment Relationship"));
    title = name;

    pop = static_cast<population *>(static_cast<void *>(parent));
    genders = 2;

    equationNum = 1;
    parameters = nullptr;
    numParams = 0;
    numSliders = 0;

    yMax = -10;
    chartview = nullptr;

    setXvals();
    setXvalStrings(QStringList());
    resetChart(true);
    selSeries->append(0, 0);
    selSeries->append(1, 0);
    selSeries->append(2, 0);
    cht->addSeries(selSeries);
    chartview->setChart(cht);

    setSlider1(-5, 5, 8);
    setSlider2(-5, 5, .5);
    setSlider3(-5, 5, 1);
    intVar1Changed(8);

    connect (ui->horizontalSlider_1, SIGNAL(valueChanged(int)), SLOT(slider1Changed(int)));
    connect (ui->doubleSpinBox_1_value, SIGNAL(valueChanged(double)), SLOT(value1Changed(double)));
    connect (ui->doubleSpinBox_1_min, SIGNAL(valueChanged(double)), SLOT(min1Changed(double)));
    connect (ui->doubleSpinBox_1_max, SIGNAL(valueChanged(double)), SLOT(max1Changed(double)));

    connect (ui->horizontalSlider_2, SIGNAL(valueChanged(int)), SLOT(slider2Changed(int)));
    connect (ui->doubleSpinBox_2_value, SIGNAL(valueChanged(double)), SLOT(value2Changed(double)));
    connect (ui->doubleSpinBox_2_min, SIGNAL(valueChanged(double)), SLOT(min2Changed(double)));
    connect (ui->doubleSpinBox_2_max, SIGNAL(valueChanged(double)), SLOT(max2Changed(double)));

    connect (ui->horizontalSlider_3, SIGNAL(valueChanged(int)), SLOT(slider3Changed(int)));
    connect (ui->doubleSpinBox_3_value, SIGNAL(valueChanged(double)), SLOT(value3Changed(double)));
    connect (ui->doubleSpinBox_3_min, SIGNAL(valueChanged(double)), SLOT(min3Changed(double)));
    connect (ui->doubleSpinBox_3_max, SIGNAL(valueChanged(double)), SLOT(max3Changed(double)));

    connect (ui->spinBox_SPR, SIGNAL(valueChanged(int)), SLOT(intVar1Changed(int)));

    connect (this, SIGNAL(numbersUpdated()), SLOT(update()));

    connect (ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), SLOT(buttonClicked(QAbstractButton*)));
    building = false;
    waiting = false;
    updating = false;
    update();
 //   refresh();
}

srEquationDialog::~srEquationDialog()
{
    delete ui;
}



void srEquationDialog::setXvals()
{
    double val;
    xValList.clear();
    for (int i = 0; i <= XMAX; i+=2)
    {
        val = static_cast<double>(i / 100.0);
        xValList.append(val);
    }
}

void srEquationDialog::setXvalStrings(const QStringList &vals)
{
    QList<float> f_vals;

    for (int i = 0; i < vals.count(); i++)
        f_vals.append(QString(vals.at(i)).toFloat());

    setXvals();
}

void srEquationDialog::setEquationNumber(int num)
{
    equationNum = num;
}

void srEquationDialog::changeEquationNumber(int num)
{
    pop->SR()->setMethod(num);
    restoreAll();
}

void srEquationDialog::setParameters(tablemodel *params)
{
    disconnect(parameters, SIGNAL(dataChanged()), this, SLOT(parametersChanged()));
    parameters = params;
    connect(parameters, SIGNAL(dataChanged()), this, SLOT(parametersChanged()));
}

void srEquationDialog::getParameterValues()
{
    QStringList values;
    int switchnum;
    if (nullptr == parameters) {
        switchnum = numSliders = 0;
    }
    else {
        switchnum = numSliders = pop->SR()->getNumFullParameters() - 3;
    }

    disconnect (parameters, SIGNAL(dataChanged()), this, SLOT(parametersChanged()));
    disconnect (this, SIGNAL(numbersUpdated()), this, SLOT(update()));

    switch (switchnum)
    {
     case 3:
        values = parameters->getRowData(2);
        min3Changed(values.at(0).toDouble());
        max3Changed(values.at(1).toDouble());
        value3Changed(values.at(2).toDouble());
        ui->doubleSpinBox_3_input->setValue(val3);
        [[clang::fallthrough]];
    case 2:
        values = parameters->getRowData(1);
        min2Changed(values.at(0).toDouble());
        max2Changed(values.at(1).toDouble());
        value2Changed(values.at(2).toDouble());
        ui->doubleSpinBox_2_input->setValue(val2);
        [[clang::fallthrough]];
    case 1:
        values = parameters->getRowData(0);
        min1Changed(values.at(0).toDouble());
        max1Changed(values.at(1).toDouble());
        value1Changed(values.at(2).toDouble());
        ui->doubleSpinBox_1_input->setValue(val1);
        break;
    default:
    case 0:
        break;
    }
    connect (parameters, SIGNAL(dataChanged()), SLOT(parametersChanged()));
    connect (this, SIGNAL(numbersUpdated()), SLOT(update()));
    showSliders(numSliders);
}

void srEquationDialog::setParameterValues()
{
    QStringList values;
    if (parameters != nullptr)
    {
    disconnect (parameters, SIGNAL(dataChanged()), this, SLOT(parametersChanged()));

    switch (numSliders)
    {
    case 3:
        values = parameters->getRowData(2);
        values[0] = QString::number(min3);
        values[1] = QString::number(max3);
        values[2] = QString::number(val3);
        parameters->setRowData(2, values);
        parameters->setRowHeader(2, ui->label_3_name->text());
        ui->doubleSpinBox_3_input->setValue(val3);
        [[clang::fallthrough]];
    case 2:
        values = parameters->getRowData(1);
        values[0] = QString::number(min2);
        values[1] = QString::number(max2);
        values[2] = QString::number(val2);
        parameters->setRowData(1, values);
        parameters->setRowHeader(1, ui->label_2_name->text());
        ui->doubleSpinBox_2_input->setValue(val2);
        [[clang::fallthrough]];
    case 1:
        values = parameters->getRowData(0);
        values[0] = QString::number(min1);
        values[1] = QString::number(max1);
        values[2] = QString::number(val1);
        parameters->setRowData(0, values);
        parameters->setRowHeader(0, ui->label_1_name->text());
        ui->doubleSpinBox_1_input->setValue(val1);
        [[clang::fallthrough]];
    case 0:
    default:
        ;
    }
    connect (parameters, SIGNAL(dataChanged()), SLOT(parametersChanged()));
    }
}

void srEquationDialog::setParameterHeaders()
{
    QString parTyp("log(R0)");
    switch (numSliders)
    {
    case 3:
        parameters->setRowHeader(2, ui->label_3_name->text());//QString("SR p3 "));
        [[clang::fallthrough]];
    case 2:
        parameters->setRowHeader(1, ui->label_2_name->text());//QString("SR p2 steepness"));
        [[clang::fallthrough]];
    case 1:
        parameters->setRowHeader(0, ui->label_1_name->text());//QString("SR p1 log(R0)"));
    }
}

void srEquationDialog::refresh()
{
    restoreAll();
}

void srEquationDialog::setup()
{
    if (parameters == nullptr)
        return;
    if (pop == nullptr)
        return;

    while (updating)
        continue;

    {
        updating = true;

        resetChart();

        switch (equationNum)
        {
        case 1: // case 1: null
            numSliders = 0;
            blank (1, 6, QString("For B-H constrained curve"));
            break;

        case 2:  // case 2: Ricker - 2 parameters: log(R0) and steepness
            numSliders = 2;
            ricker ();
            break;

        case 3:  // case 3: Standard Beverton-Holt - 2 parameters
            numSliders = 2;
            bevertonHoltStandard ();
            break;

        case 4:  // case 4: Ignore Steepness and no bias adjustment, like CAGEAN - 2 parameters, uses only first one
            numSliders = 1;
            constant ();
            break;

        case 5:  // case 5: Hockey stick - 3 prarmenters: log(R0), steepness and Rmin
            numSliders = 3;
            hockeyStick ();
            break;

        case 6:  // case 6: Beverton-Holt with flat-top beyond Bzero - 2 parameters
            numSliders = 2;
            bevertonHoltBzeroFlat ();
            break;

        case 7: // case 7: Survivorship function - 3 parameters: log(R0), Zfrac, and Beta
            numSliders = 3;
            survivorship ();
            break;

        case 8:  // case 8: Shepherd - 3 parameters: log(R0), steepness, and shape C
            numSliders = 3;
            shepherd ();
            break;

        case 9:  // case 9: Shepherd re-parameterization - 3 parameters: log(R0), steepness, and shape C
            numSliders = 3;
            notYet();
//            shepherdReParm ();
            break;

        case 10: // case 10: Ricker re-parameterization - 3 parameters: log(R0), steepness, and Ricker power Gamma
            numSliders = 3;
            notYet();
            rickerReParm ();
            break;

        default:
            numSliders = 0;
            blank(0);
            break;

        }
        updating = false;
    }
    updateGrid(cht->rect());
    QDialog::update();
}

void srEquationDialog::update()
{
    if (parameters == nullptr)
        return;
    if (pop == nullptr)
        return;

    while (updating)
        continue;

    {
        updating = true;

        switch (equationNum)
        {
//        case 1: // case 1: null
//            numSliders = 0;
//            blank (1, 6, QString("For B-H constrained curve"));
//            break;

        case 2:  // case 2: Ricker - 2 : log(R0) and steepness
            numSliders = 2;
            updateRicker ();
            break;

        case 3:  // case 3: Standard Beverton-Holt - 2
            numSliders = 2;
            updateBevertonHoltStandard ();
            break;

        case 4:  // case 4: Ignore Steepness and no bias adjustment, like CAGEAN - 2 parameters, uses only first one
            numSliders = 1;
            updateConstant ();
            break;

        case 5:  // case 5: Hockey stick - 3 prarmenters: log(R0), steepness and Rmin
            numSliders = 3;
            updateHockeyStick ();
            break;

        case 6:  // case 6: Beverton-Holt with flat-top beyond Bzero - 2 parameters
            numSliders = 2;
            updateBevertonHoltBzeroFlat ();
            break;

        case 7: // case 7: Survivorship function - 3 parameters: log(R0), Zfrac, and Beta
            numSliders = 3;
            updateSurvivorship ();
            break;

        case 8:  // case 8: Shepherd - 3 parameters: log(R0), steepness, and shape C
            numSliders = 3;
            updateShepherd ();
            break;

        case 9:  // case 9: Shepherd re-parameterization - 3 parameters: log(R0), steepness, and shape C
            numSliders = 3;
//            notYet();
//            updateShepherdReParm ();
            break;

        case 10: // case 10: Ricker re-parameterization - 3 parameters: log(R0), steepness, and Ricker power Gamma
            numSliders = 3;
//            notYet();
            updateRickerReParm ();
            break;

        default:
            numSliders = 0;
            blank(0);
            break;

        }
        updating = false;
    }
    updateGrid(cht->rect());

    QDialog::update();
}

void srEquationDialog::setSlider1(double min, double max, double value)
{
    min1 = min; max1 = max; val1 = value - max;
/*    min1Changed(min);
    max1Changed(max);
    value1Changed(value);*/
    ui->doubleSpinBox_1_max->setValue(max1);
    ui->doubleSpinBox_1_min->setValue(min1);
    ui->horizontalSlider_1->setMaximum(static_cast<int>(max1 * SLIDER_SCALE));
    ui->horizontalSlider_1->setMinimum(static_cast<int>(min1 * SLIDER_SCALE));
//    ui->horizontalSlider_1->setValue(static_cast<int>(val1 * SLIDER_SCALE));
    ui->doubleSpinBox_1_value->setMaximum(max1);
    ui->doubleSpinBox_1_value->setMinimum(min1);
    ui->doubleSpinBox_1_value->setValue(value);
}

void srEquationDialog::slider1Changed(int value)
{
    double val = static_cast<double>(value * VALUE_SCALE);
    ui->doubleSpinBox_1_value->setValue(val);
}

void srEquationDialog::value1Changed (double value)
{
    if (value > (val1 + PRECISION) || value < (val1 - PRECISION))
    {
        val1 = value;
        emit numbersUpdated();
        ui->horizontalSlider_1->setValue(static_cast<int>((val1 + HALFPREC) * SLIDER_SCALE));
    }
}

void srEquationDialog::min1Changed (double value)
{
    if (value > (min1 + PRECISION) || value < (min1 - PRECISION))
    {
        min1 = value;
        if (min1 > max1)
        {
            min1 = max1;
        }
        ui->doubleSpinBox_1_min->setValue(min1);
        ui->doubleSpinBox_1_value->setMinimum(min1);
        ui->horizontalSlider_1->setMinimum(static_cast<int>((min1 + HALFPREC) * SLIDER_SCALE));
    }
}

void srEquationDialog::max1Changed (double value)
{
    if (value > (max1 + PRECISION) || value < (max1 - PRECISION))
    {
        max1 = value;
        if (max1 < min1)
        {
            max1 = min1;
        }
        ui->doubleSpinBox_1_max->setValue(max1);
        ui->doubleSpinBox_1_value->setMaximum(max1);
        ui->horizontalSlider_1->setMaximum(static_cast<int>(max1 * SLIDER_SCALE));
    }
}

void srEquationDialog::setSlider2(double min, double max, double value)
{
    min2 = min; max2 = max; val2 = value - max;
    ui->doubleSpinBox_2_max->setValue(max2);
    ui->doubleSpinBox_2_min->setValue(min2);
    ui->horizontalSlider_2->setMaximum(static_cast<int>(max2 * SLIDER_SCALE));
    ui->horizontalSlider_2->setMinimum(static_cast<int>(min2 * SLIDER_SCALE));
//    ui->horizontalSlider_2->setValue(static_cast<int>(value * SLIDER_SCALE));
    ui->doubleSpinBox_2_value->setMaximum(max2);
    ui->doubleSpinBox_2_value->setMinimum(min2);
    ui->doubleSpinBox_2_value->setValue(value);
}

void srEquationDialog::slider2Changed(int value)
{
    double val = static_cast<double>(value * VALUE_SCALE);
    ui->doubleSpinBox_2_value->setValue(val);
}

void srEquationDialog::value2Changed (double value)
{
    if (val2 > (value + PRECISION) || val2 < (value - PRECISION))
    {
        val2 = value;
        ui->horizontalSlider_2->setValue(static_cast<int>((val2 + HALFPREC) * SLIDER_SCALE));
        emit numbersUpdated();
    }
}

void srEquationDialog::min2Changed (double value)
{
    if (min2 > (value + PRECISION) || min2 < (value - PRECISION))
    {
        min2 = value;
        if (min2 > max2)
        {
            min2 = max2;
        }
        ui->doubleSpinBox_2_min->setValue(min2);
        ui->doubleSpinBox_2_value->setMinimum(min2);
        ui->horizontalSlider_2->setMinimum(static_cast<int>(min2 * SLIDER_SCALE));
    }
}

void srEquationDialog::max2Changed (double value)
{
    if (max2 > (value + PRECISION) || max2 < (value - PRECISION))
    {
        max2 = value;
        if (max2 < min2)
        {
            max2 = min2;
        }
        ui->doubleSpinBox_2_max->setValue(max2);
        ui->doubleSpinBox_2_value->setMaximum(max2);
        ui->horizontalSlider_2->setMaximum(static_cast<int>(max2 * SLIDER_SCALE));
    }
}

void srEquationDialog::setSlider3(double min, double max, double value)
{
    min3 = min; max3 = max; val3 = value - max;
    ui->doubleSpinBox_3_max->setValue(max3);
    ui->doubleSpinBox_3_min->setValue(min3);
    ui->horizontalSlider_3->setMaximum(static_cast<int>(max3 * SLIDER_SCALE));
    ui->horizontalSlider_3->setMinimum(static_cast<int>(min3 * SLIDER_SCALE));
    ui->horizontalSlider_3->setValue(static_cast<int>(value * SLIDER_SCALE));
    ui->doubleSpinBox_3_value->setMaximum(max3);
    ui->doubleSpinBox_3_value->setValue(value);
}

void srEquationDialog::slider3Changed(int value)
{
    double val = static_cast<double>(value * VALUE_SCALE);
    ui->doubleSpinBox_3_value->setValue(val);
}

void srEquationDialog::value3Changed (double value)
{
    if (val3 > (value + PRECISION) || val3 < (value - PRECISION))
    {
        val3 = value;
        emit numbersUpdated();
        ui->horizontalSlider_3->setValue(static_cast<int>((val3 + HALFPREC) * SLIDER_SCALE));
    }
}

void srEquationDialog::min3Changed (double value)
{
    if (min3 > (value + PRECISION) || min3 < (value - PRECISION))
    {
        min3 = value;
        if (min3 > max3)
        {
            min3 = max3;
        }
        ui->doubleSpinBox_3_min->setValue(min3);
//        ui->doubleSpinBox_3_value->setMinimum(min3);
        ui->horizontalSlider_3->setMinimum(static_cast<int>(min3 * SLIDER_SCALE));
    }
}

void srEquationDialog::max3Changed (double value)
{
    if (max3 > (value + PRECISION) || max3 < (value - PRECISION))
    {
        max3 = value;
        if (max3 < min3)
        {
            max3 = min3;
        }
        ui->doubleSpinBox_3_max->setValue(max3);
        ui->doubleSpinBox_3_value->setMaximum(max3);
        ui->horizontalSlider_3->setMaximum(static_cast<int>(max3 * SLIDER_SCALE));
    }
}

void srEquationDialog::setIntVar1(int value)
{
    intvar1 = value;
}

void srEquationDialog::intVar1Changed(int value)
{
    setIntVar1 (value);
    emit numbersUpdated();
}

void srEquationDialog::apply()
{
    pop->SR()->setOption(equationNum);
    // set input spinboxes
    ui->doubleSpinBox_1_input->setRange(min1, max1);
    ui->doubleSpinBox_1_input->setValue(val1);
    ui->doubleSpinBox_2_input->setRange(min2, max2);
    ui->doubleSpinBox_2_input->setValue(val2);
    ui->doubleSpinBox_3_input->setRange(min3, max3);
    ui->doubleSpinBox_3_input->setValue(val3);
    // write values to parameters
    setParameterValues();
//    getParameterValues();
}

void srEquationDialog::reset()
{
    resetValues();
    emit numbersUpdated();
}

void srEquationDialog::resetValues()
{
    {
//        disconnect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));

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

 //       connect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));
    }
}

void srEquationDialog::restoreAll()
{
    // get values from parameters
    setEquationNumber(pop->SR()->getMethod());
    getParameterValues();
    setup();
}

void srEquationDialog::close()
{
    emit closed();
    hide();
}

void srEquationDialog::buttonClicked(QAbstractButton *btn)
{
    if      (btn->text().contains("Apply")) {
        apply();
    }
    else if (btn->text().contains("Reset")) {
        restoreAll();
    }
    else if (btn->text().contains("Close")) {
        close();
    }
}

void srEquationDialog::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    emit closed();
}

void srEquationDialog::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    updateGrid(cht->rect());
}

void srEquationDialog::parametersChanged()
{
    getParameterValues();
//    resetValues();
    emit numbersUpdated();
}

void srEquationDialog::setupChanged()
{
    refresh();
}

QString srEquationDialog::getTitle() const
{
    return title;
}

void srEquationDialog::setTitle(const QString &value)
{
    title = value;
    setWindowTitle(QString("%1 -  %2").arg(name, title));
}

void srEquationDialog::setLabel(const QString &value)
{
    ui->label_title->setText(value);
    ui->label_title->show();
}

QString srEquationDialog::getName() const
{
    return name;
}

void srEquationDialog::setName(const QString &value)
{
    name = value;
    setWindowTitle(name);
}

population *srEquationDialog::getPopulation() const
{
    return pop;
}

void srEquationDialog::setPopulation(population *value)
{
    pop = value;
    setEquationNumber(pop->SR()->getMethod());
    setParameters(pop->SR()->getFullParameters());
    refresh();
}

void srEquationDialog::showSliders(int num)
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
        ui->label_1_type->setText("exp");
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
}

void srEquationDialog::updateGrid(QRectF rect)
{
    int xTicks = static_cast<int>(rect.width() / 100);
    int yTicks = static_cast<int>(rect.height() / 60);

    selSeries->detachAxis(axisY);
    int yMax = static_cast<int>((maxYvalue(selSeries->points()) + 100) / 100) * 100;
    axisXsel->setRange(0, (XMAX / 100.0));
    axisY->setRange(0, yMax);
//    selSeries->attachAxis(axisXsel);
    selSeries->attachAxis(axisY);

    xTicks = xTicks < 5? 3: xTicks;
    updateTicks(xTicks, yTicks);
}

void srEquationDialog::updateTicks(int xT, int yT)
{
    int xticks = xT;// % 2? xT: (xT + 1);
    int yticks = yT % 2? yT: (yT + 1);
    axisXsel->setTickCount(xticks);

    axisY->setTickCount(yticks);
    axisYalt->setTickCount(yticks);
}

void srEquationDialog::resetChart(bool create)
{
    ui->label_SPR->hide();
    ui->label_SPR_info->hide();
    ui->spinBox_SPR->hide();
    ui->label_title->hide();
    if (chartview != nullptr)
    {
        chartview->setVisible(false);
        ui->verticalLayout_graph->removeWidget(chartview);
    }
    if (!create)
    {
        chartview->hide();
        cht->hide();
        delete axisXsel;
        delete axisY;
        delete axisYalt;
        if (ascendSeries != nullptr)
            delete ascendSeries;
        if (dscendSeries != nullptr)
            delete dscendSeries;
        if (ptSeries != nullptr)
            delete ptSeries;
        delete selSeries;
        delete cht;
        delete chartview;
    }
    chartview = new QChartView(this);
    chartview->setChart(new QChart());
    cht = chartview->chart();
    ascendSeries = nullptr;
    dscendSeries = nullptr;
    ptSeries = nullptr;
    selSeries = new QLineSeries(cht);
    selSeries->setName(QString("SB"));
    axisXsel = new QValueAxis();
    axisY = new QValueAxis();
    axisY->setTitleText(QString("SB"));
    axisXsel->setTitleText(QString("R"));
    axisYalt = new QValueAxis();
    axisXsel->setRange(0, 1.5);
    selSeries->attachAxis(axisXsel);
    selSeries->attachAxis(axisY);

    firstPoints.clear();

    yMax = 1;
    ui->verticalLayout_graph->addWidget(chartview);
    chartview->show();
    cht->show();
    cht->legend()->setVisible(false);
    int sizew = static_cast<int>(cht->size().width());
    int sizeh = static_cast<int>(cht->size().height());
    resizeEvent(new QResizeEvent(QSize(sizew, sizeh), QSize(sizew, sizeh)));
    repaint();
}

// Not yet implemented
void srEquationDialog::notYet(int opt, int num)
{
    if (opt == 0)
        opt = equationNum;
    if (num == 0)
        num = numSliders;
    QString msg(QString("Option %1 is not yet implemented").arg(
                       QString::number(opt)));
    QString info("\n   NOT YET IMPLEMENTED \n   THIS PAGE INTENTIONALLY LEFT BANK");

    setTitle(msg);

    // INTENTIONALLY LEFT BLANK
    setLabel(info);

    chartview->setVisible(false);
    showSliders(num);
}

// case 1: Null or error
void srEquationDialog::blank (int num, int rep, QString msg)
{
    QString info("\n   THIS PAGE INTENTIONALLY LEFT BLANK");
    QString numstr(QString::number(num > 0? num: equationNum));
    QString repstr(QString::number(rep));
    // If replaced by another option
    if (rep > 0)
    {
        msg.append(QString(", use option %1 instead").arg(repstr));
        info.prepend(QString("   OPTION REPLACED WITH OPTION %1 ").arg(repstr));
    }
    // If discontinued
    else if (msg.isEmpty())
    {
        msg.append(QString("Option %1 is discontinued").arg(numstr));
        info.prepend(QString("   OPTION DISCONTINUED "));
    }
    else
    {
        msg.prepend(QString("Option %1: ").arg(numstr));
    }
    // Window title
    setTitle(msg);

    // INTENTIONALLY LEFT BLANK
    setLabel(info);

    chartview->setVisible(false);
    showSliders(numSliders);
}

//  SS_Label_43.3.2  Ricker
//      case 2:  // ricker
//  2 parameters: log(R0) and steepness
void srEquationDialog::ricker()
{
    setTitle(QString("Option 2: Ricker"));
    setLabel("Ricker");

    chartview->setVisible(false);

    ui->label_1_name->setText("SR_LN(R0)");
    ui->label_1_type->setText("Exp");
    ui->label_2_name->setText("SR_Ricker_beta");
    ui->label_2_type->setText("Value");
    setParameterHeaders();

    numSliders = 2;
    showSliders(numSliders);
    updateRicker();
    chartview->setVisible(true);

    cht->addAxis(axisXsel, Qt::AlignBottom);
    cht->addAxis(axisY, Qt::AlignLeft);
}

//    Recr_virgin_adj = exp(SR_parm_work(1));
//    steepness=SR_parm_work(2);
//    NewRecruits = Recr_virgin_adj*SSB_curr_adj/
//                  SSB_virgin_adj *
//                  mfexp(steepness*(1.-SSB_curr_adj/SSB_virgin_adj));

void srEquationDialog::updateRicker()
{
    /* R code
    SB = seq(0,1, 0.05)
    h = 0.95
    R0 = 10

    R = R0 * (SB / SB0) * exp(h * (1−SB/SB0))

    plot(SB,R) */

//    double SPRtemp =
//    double SB_virgin_adj = 1.0;
    double SSB_curr_adj, Recruits;
    double logRecr_virgin_adj = ui->doubleSpinBox_1_value->value();
    double steepness = ui->doubleSpinBox_2_value->value();

    double Recr_virgin_adj = exp(logRecr_virgin_adj);
    ui->doubleSpinBox_1_trans->setValue(Recr_virgin_adj);
    ui->doubleSpinBox_2_trans->setValue(steepness);

    cht->removeSeries(selSeries);
    cht->update();
    selSeries->clear();

    // SSB_virgin_adj = 1.0, so it factors out
    for (int i = 0; i < xValList.count(); i++)
    {
        SSB_curr_adj = xValList.at(i);
//        Recruits = Recr_virgin_adj * (SSB_curr_adj/SSB_virgin_adj) * (exp(steepness * (1.0 - SSB_curr_adj/SSB_virgin_adj)));
        Recruits = Recr_virgin_adj * (SSB_curr_adj) * (exp(steepness * (1.0 - SSB_curr_adj)));
        selSeries->append(SSB_curr_adj, Recruits);
    }
    cht->addSeries(selSeries);
}

//  SS_Label_43.3.3  Beverton-Holt
//      case 3: // Beverton-Holt
//  2 parameters: log(R0) and steepness
void srEquationDialog::bevertonHoltStandard()
{
    QString msg (QString("Option 3: Standard Beverton-Holt"));
    setTitle(msg);
    setLabel("Standard Beverton-Holt");

    chartview->setVisible(false);

    ui->label_1_name->setText("SR_LN(R0)");
    ui->label_1_type->setText("Exp");
    ui->label_2_name->setText("SR_BH_steep");
    ui->label_2_type->setText("Value");
    setParameterHeaders();

    numSliders = 2;
    showSliders(numSliders);
    updateBevertonHoltStandard();
    chartview->setVisible(true);

    cht->addAxis(axisXsel, Qt::AlignBottom);
    cht->addAxis(axisY, Qt::AlignLeft);
}

//    Recr_virgin_adj = exp(SR_parm_work(1));
//    steepness=SR_parm_work(2);
//    alpha = 4.0 * steepness*Recr_virgin / (5.*steepness-1.);
//    beta = (SSB_virgin_adj*(1.-steepness)) / (5.*steepness-1.);
//    NewRecruits =  (4.*steepness*Recr_virgin_adj*SSB_curr_adj) /
//                   (SSB_virgin_adj*(1.-steepness)+(5.*steepness-1.)*SSB_curr_adj);

void srEquationDialog::updateBevertonHoltStandard()
{
    /* R code from C. Wetzel
    SB0 = 1
    SB = seq(0,1, 0.05)
    h = 0.95
    R0 = 10

    R = R0*(4*h*SB)/(SB0*(1-h)+SB*(5*h-1))

    plot(SB,R) */

//    double SSB_virgin_adj = 1.0;
    double SSB_curr_adj, Recruits;
    double logRecr_virgin_adj = val1;// ui->doubleSpinBox_1_value->value();
    double steep = val2; // ui->doubleSpinBox_2_value->value();

    double Recr_virgin_adj = exp(logRecr_virgin_adj);
    ui->doubleSpinBox_1_trans->setValue(Recr_virgin_adj);
    ui->doubleSpinBox_2_trans->setValue(steep);

    cht->removeSeries(selSeries);
    cht->update();
    selSeries->clear();

    // SSB_virgin_adj = 1.0
    for (int i = 0; i < xValList.count(); i++)
    {
        SSB_curr_adj = xValList.at(i);
        Recruits = (4.0 * Recr_virgin_adj * steep * SSB_curr_adj) /
                ((1.0-steep) + SSB_curr_adj * (5.0*steep - 1.0));
        selSeries->append(SSB_curr_adj, Recruits);
    }
    cht->addSeries(selSeries);
}


//  SS_Label_43.3.4  constant expected recruitment
//      case 4:  // none
// case 4: CAGEAN-like
//  1 parameter:
//  Ignore Steepness and no bias adjustment. Use this in
//  conjunction with very low emphasis on recruitment deviations
//  to get CAGEAN-like unconstrained recruitment estimates,
void srEquationDialog::constant()
{
    QString msg(QString("Option 4: Ignore steepness and no bias adjustment"));
    setTitle(msg);
    setLabel("Ignore steepness and no bias adjustment - only first parameter used.");

    chartview->setVisible(false);

    ui->label_1_name->setText("SR_LN(R0)");
    ui->label_1_type->setText("Exp");
    ui->label_2_name->setText("SR_steepness");
    ui->label_2_type->setText("Value");
    setParameterHeaders();

    numSliders = 2;
    showSliders(numSliders);
    updateConstant();
    chartview->setVisible(true);

    cht->addAxis(axisXsel, Qt::AlignBottom);
    cht->addAxis(axisY, Qt::AlignLeft);
}

//    NewRecruits=Recr_virgin_adj;

void srEquationDialog::updateConstant()
{
    double SSB_curr_adj, Recruits;
    double logRecr_virgin_adj = ui->doubleSpinBox_1_value->value();

    double Recr_virgin_adj = exp(logRecr_virgin_adj);
    ui->doubleSpinBox_1_trans->setValue(Recr_virgin_adj);

    cht->removeSeries(selSeries);
    cht->update();
    selSeries->clear();

    for (int i = 0; i < xValList.count(); i++)
    {
        SSB_curr_adj = xValList.at(i);
        Recruits = Recr_virgin_adj;
        selSeries->append(SSB_curr_adj, Recruits);
    }
    cht->addSeries(selSeries);
    updateGrid(cht->rect());
}

//  SS_Label_43.3.5  Hockey stick
//      case 5:  // hockey stick  where "steepness" is now the fraction of B0 below which recruitment declines linearly
//               //  the 3rd parameter allows for a minimum recruitment level
// case 5: Hockey stick
//  3 parameters: log(R0), steepness, and Rmin for ln(R0),
//  fraction of virgin SSB at which inflection occurs,
//  and the Recruits level at SSB=0.0.
void srEquationDialog::hockeyStick()
{
    QString msg (QString("Option 5: Hockey Stick"));
    setTitle(msg);
    setLabel("Hockey Stick");

    chartview->setVisible(false);

    ui->label_1_name->setText("SR_LN(R0)");
    ui->label_1_type->setText("Exp");
    ui->label_2_name->setText("SR_hockey_infl");
    ui->label_2_type->setText("Value");
    ui->label_3_name->setText("SR_hockey_min_R");
    ui->label_3_type->setText("Value");
    setParameterHeaders();

    numSliders = 3;
    showSliders(numSliders);
    updateHockeyStick();
    chartview->setVisible(true);

    cht->addAxis(axisXsel, Qt::AlignBottom);
    cht->addAxis(axisY, Qt::AlignLeft);
}

//    Recr_virgin_adj = exp(SR_parm_work(1));
//    steepness = SR_parm_work(2);
      //  linear decrease below steepness * SSB_virgin_adj
//    temp = SR_parm_work(3) * Recr_virgin_adj +
//           SSB_curr_adj / (steepness * SSB_virgin_adj) *
//           (Recr_virgin_adj - SR_parm_work(3) * Recr_virgin_adj);
//    NewRecruits = Join_Fxn(0.0 * SSB_virgin_adj, SSB_virgin_adj, steepness * SSB_virgin_adj, SSB_curr_adj, temp, Recr_virgin_adj);

void srEquationDialog::updateHockeyStick()
{
    double SSB_virgin_adj = 1.0;
    double temp, join, SSB_curr_adj, Recruits;
    double Recr_virgin_adj = exp(ui->doubleSpinBox_1_value->value());
    double inflection = ui->doubleSpinBox_2_value->value();
    double Rmin = ui->doubleSpinBox_3_value->value();
    Rmin = Rmin < 0? 0: Rmin;
    Rmin = Rmin > 1? 1: Rmin;

    ui->doubleSpinBox_1_trans->setValue(Recr_virgin_adj);
    ui->doubleSpinBox_2_trans->setValue(inflection);
    ui->doubleSpinBox_3_trans->setValue(Rmin);

    cht->removeSeries(selSeries);
    cht->update();
    selSeries->clear();

    // SSB_virgin_adj = 1.0, so it factors out
    for (int i = 0; i < xValList.count(); i++)
    {
        SSB_curr_adj = xValList.at(i);
//        temp = Rmin + Recr_virgin_adj * (1.0 - (inflection - SSB_curr_adj));
//        join = 1.0 / (1.0 + exp(1000.0 * (SSB_curr_adj - inflection)/(Recr_virgin_adj-Rmin)));
//        Recruits = temp * join + Recr_virgin_adj * (1.0 - join);
        temp = Rmin * Recr_virgin_adj +
              SSB_curr_adj / (inflection * SSB_virgin_adj) *
              (Recr_virgin_adj - Rmin * Recr_virgin_adj);
        Recruits = joinFunction(0.0 * SSB_virgin_adj, SSB_virgin_adj, inflection * SSB_virgin_adj, SSB_curr_adj, temp, Recr_virgin_adj);
        selSeries->append(SSB_curr_adj, Recruits);
    }
    cht->addSeries(selSeries);
}

//  SS_Label_43.3.6  Beverton-Holt, with constraint to have constant R about Bzero
//      case 6: //Beverton-Holt constrained
//  2 parameters: log(R0) and steepness.
void srEquationDialog::bevertonHoltBzeroFlat()
{
    QString msg (QString("Option 6: Beverton-Holt with constraint"));
    setTitle(msg);
    setLabel("Beverton-Holt with constant around Bzero");

    chartview->setVisible(false);

    ui->label_1_name->setText("SR_LN(R0)");
    ui->label_1_type->setText("Exp");
    ui->label_2_name->setText("SR_BH_flat_steep");
    ui->label_2_type->setText("Value");
    setParameterHeaders();

    numSliders = 2;
    showSliders(numSliders);
    updateBevertonHoltBzeroFlat();
    chartview->setVisible(true);

    cht->addAxis(axisXsel, Qt::AlignBottom);
    cht->addAxis(axisY, Qt::AlignLeft);
}

//    Recr_virgin_adj = exp(SR_parm_work(1));
//    steepness = SR_parm_work(2);
//    alpha = 4.0 * steepness*Recr_virgin / (5.*steepness-1.);
//    beta = (SSB_virgin_adj*(1.-steepness)) / (5.*steepness-1.);
//    if(SSB_curr_adj>SSB_virgin_adj) {SSB_BH1=SSB_virgin_adj;} else {SSB_BH1=SSB_curr_adj;}
//    NewRecruits=(4.*steepness*Recr_virgin_adj*SSB_BH1) / (SSB_virgin_adj*(1.-steepness)+(5.*steepness-1.)*SSB_BH1);

// case 6: Beverton-Holt with flat top beyond Bzero
void srEquationDialog::updateBevertonHoltBzeroFlat()
{
    double SSB_virgin_adj = 0.8;
    double SSB_curr_adj, Recruits, SSB_BH1;
    double logRecr_virgin_adj = ui->doubleSpinBox_1_value->value();
    double Recr_virgin_adj = exp(logRecr_virgin_adj);
    double steep = ui->doubleSpinBox_2_value->value();
    double alpha, beta;

    ui->doubleSpinBox_1_trans->setValue(Recr_virgin_adj);
    ui->doubleSpinBox_2_trans->setValue(steep);

    cht->removeSeries(selSeries);
    cht->update();
    selSeries->clear();

    // SSB_virgin_adj = 1.0, so it factors out
    for (int i = 0; i < xValList.count(); i++)
    {
        SSB_curr_adj = xValList.at(i);
//        alpha = 4.0 * steep * Recr_virgin_adj / (.5 * steep - 1);
//        beta = (SSB_virgin_adj * (1 - steep)) / (.5 * steep - 1);
        if (SSB_curr_adj > SSB_virgin_adj)
            SSB_BH1 = SSB_virgin_adj;
        else
            SSB_BH1 = SSB_curr_adj;
        Recruits = (4.0 * Recr_virgin_adj * steep * SSB_BH1) /
                ((1.0 - steep) + SSB_BH1 * (5.0*steep - 1.0));
        selSeries->append(SSB_curr_adj, Recruits);
    }
    cht->addSeries(selSeries);
}


//  SS_Label_44.1.7  3 parameter survival based
//      case 7:  // survival
//  3 parameters: log(R0), zfrac, and Beta.
//  Suitable for sharks and low fecundity stocks to assure
//  recruits are <= population production.
void srEquationDialog::survivorship()
{
    setTitle(QString("Option 7: Survivorship"));
    setLabel("Survivorship");
    ui->label_SPR->show();
    ui->label_SPR_info->show();
    ui->spinBox_SPR->show();

    chartview->setVisible(false);

    ui->label_1_name->setText("SR_LN(R0)");
    ui->label_1_type->setText("Exp");
    ui->label_2_name->setText("SR_surv_Zfrac");
    ui->label_2_type->setText("Value");
    ui->label_3_name->setText("SR_surv_Beta");
    ui->label_3_type->setText("Value");
    setParameterHeaders();

    numSliders = 3;
    showSliders(numSliders);
    updateSurvivorship();
    chartview->setVisible(true);

    cht->addAxis(axisXsel, Qt::AlignBottom);
    cht->addAxis(axisY, Qt::AlignLeft);
    cht->addAxis(axisYalt, Qt::AlignRight);
    cht->addSeries(selSeries);
}

//    Recr_virgin_adj = exp(SR_parm_work(1));
//    Zfrac = SR_parm_work(2);
//    Beta = SR_parm_work(3);
//    SRZ_0=log(1.0/(SSB_virgin_adj/Recr_virgin_adj));
//    srz_min=SRZ_0*(1.0-Zfrac);
//    SRZ_surv=mfexp((1.-pow((SSB_curr_adj/SSB_virgin_adj),Beta) )*(srz_min-SRZ_0)+SRZ_0);  //  survival
//    NewRecruits=SSB_curr_adj*SRZ_surv;

//Recr_virgin_adj = exp(ln_R0)
//SPB_virgin_adj = Recr_virgin_adj * SPR_0
//SRZ_0 = ln(1.0/(SPB_virgin_adj/Recr_virgin_adj))
//SRZ_min = SRZ_0 * (1.0 - ZFrac)

//surv = exp((1.0-xval^shape) * (SRZ_min-SRZ_0) + SRZ_0)
//recr = surv * xval * SPB_virgin_adj

// Case 7: Suvivorship function
void srEquationDialog::updateSurvivorship()
{
    double SPR = intvar1;
    double SSB_curr_adj, Recruits;
    double ln_R0 = ui->doubleSpinBox_1_value->value();
    double Recr_virgin_adj = exp(ln_R0);
    double Zfrac = ui->doubleSpinBox_2_value->value();
    double Beta = ui->doubleSpinBox_3_value->value();
    ui->doubleSpinBox_1_trans->setValue(Recr_virgin_adj);
    ui->doubleSpinBox_2_trans->setValue(Zfrac);
    ui->doubleSpinBox_3_trans->setValue(Beta);

    double SPB_virgin_adj = Recr_virgin_adj * SPR;

    double SRZ_0 = log(1.0/SPR); // 1.0/(SB_virgin_adj/Recr_virgin_adj)
    double SRZ_min = SRZ_0 * (1.0 - Zfrac);
    double SRZ_surv = 0;

    cht->removeSeries(selSeries);
    cht->update();
    selSeries->clear();

    for (int i = 0; i < xValList.count(); i++)
    {
        SSB_curr_adj = xValList.at(i);
        SRZ_surv = exp((1.0 - pow(SSB_curr_adj, Beta)) * (SRZ_min - SRZ_0) + SRZ_0); // survival
        Recruits = SRZ_surv * SSB_curr_adj * SPB_virgin_adj;
        selSeries->append(SSB_curr_adj, Recruits);
    }
    cht->addSeries(selSeries);
//    int maxY = static_cast<int>(maxYvalue(selSeries->points()) + 1);
//    axisY->setRange(0, maxY);
}

//  SS_Label_43.3.8  Shepherd
//      case 8:  // Shepherd 3-parameter SRR. per Punt & Cope 2017
void srEquationDialog::shepherd()
{
    QString msg (QString("Option 8: Shepherd"));
    setTitle(msg);
    setLabel("Shepherd");

    chartview->setVisible(false);

    ui->label_1_name->setText("SR_LN(R0)");
    ui->label_1_type->setText("Exp");
    ui->label_2_name->setText("SR_steepness");
    ui->label_2_type->setText("Value");
    ui->label_3_name->setText("SR_Shepherd_c");
    ui->label_3_type->setText("Value");
    setParameterHeaders();

    numSliders = 3;
    showSliders(numSliders);
    updateShepherd();
    chartview->setVisible(true);

    cht->addAxis(axisXsel, Qt::AlignBottom);
    cht->addAxis(axisY, Qt::AlignLeft);
}

//    Recr_virgin_adj = exp(SR_parm_work(1));
//    Shepherd_c = SR_parm_work(3);
//    Shepherd_c2 = pow(0.2, SR_parm_work(3));
//    Hupper = 1.0 / (5.0 * Shepherd_c2);
//    steepness = 0.2 + (SR_parm_work(2) - 0.2) / (0.8) * (Hupper-0.2);
//    temp = (SSB_curr_adj) / (SSB_virgin_adj);
//    NewRecruits = (5. * steepness * Recr_virgin_adj * (1. - Shepherd_c2) * temp) /
//    (1.0 - 5.0 * steepness * Shepherd_c2 + (5. * steepness - 1.) * pow(temp, Shepherd_c));

void srEquationDialog::updateShepherd()
{
//    double SSB_virgin_adj = 1.0;
    double SSB_curr_adj, Recruits;
    double logRecr_virgin_adj = ui->doubleSpinBox_1_value->value();
    double SRparm2 = ui->doubleSpinBox_2_value->value();
    double shepherd_c = ui->doubleSpinBox_3_value->value();
    if (shepherd_c == 0.0)
        shepherd_c = .0000001;
    double shepherd_c2 = pow(.2, shepherd_c);
    double Hupper = 1.0 / (5.0 * shepherd_c2);
    double steep = 0.2 + (SRparm2 - 0.2) / (0.8) * (Hupper - 0.2);
    double temp;

    double Recr_virgin_adj = exp(logRecr_virgin_adj);
    ui->doubleSpinBox_1_trans->setValue(Recr_virgin_adj);
    ui->doubleSpinBox_2_trans->setValue(SRparm2);
    ui->doubleSpinBox_3_trans->setValue(shepherd_c);

    cht->removeSeries(selSeries);
    cht->update();
    selSeries->clear();

    // SSB_virgin_adj = 1.0, so it factors out
    for (int i = 0; i < xValList.count(); i++)
    {
        SSB_curr_adj = xValList.at(i);
//        temp = SSB_curr_adj;
        Recruits = (5.0 * steep * (1.0 - shepherd_c2) * SSB_curr_adj) /
                (1.0 - 5.0 * steep * shepherd_c2 + (5.0 * steep - 1.0) * pow (SSB_curr_adj, shepherd_c));
        selSeries->append(SSB_curr_adj, Recruits * Recr_virgin_adj);
    }
    cht->addSeries(selSeries);
}

// case 9: Shepherd re-parameterization (beta) - not yet implemented
//  3 parameters: log(R0), steepness, and shape parameter, c.
void srEquationDialog::shepherdReParm()
{
    QString msg (QString("Option 8: Shepherd"));
    setTitle(msg);
    setLabel("Shepherd");

    chartview->setVisible(false);

    ui->label_1_name->setText("SR_LN(R0)");
    ui->label_1_type->setText("Exp");
    ui->label_2_name->setText("SR_ShpReParm_steep");
    ui->label_2_type->setText("Value");
    ui->label_3_name->setText("SR_ShpReParm_c");
    ui->label_3_type->setText("Value");
    setParameterHeaders();

    numSliders = 3;
    showSliders(numSliders);
    updateShepherdReParm();
    chartview->setVisible(true);

    cht->addAxis(axisXsel, Qt::AlignBottom);
    cht->addAxis(axisY, Qt::AlignLeft);
}

//  Andre's FORTRAN
//        TOP = 5*Steep*(1-0.2**POWER)*SPR/SPRF0-(1-5*Steep*0.2**POWER)
//      BOT = (5*Steep-1)
//       REC = (TOP/BOT)**(1.0/POWER)*SPRF0/SPR
// Power = exp(logC);
// Hupper = 1.0/(5.0 * pow(0.2,Power));
//dvariable Shep_top;
//dvariable Shep_bot;
//dvariable Hupper;
//dvariable Shep_top2;
//Shepherd_c=exp(SRparm3);
//Shepherd_c2=pow(0.2,Shepherd_c);
//Hupper=1.0/(5.0*Shepherd_c2);
//steepness=0.20001+((0.8)/(1.0+exp(-SRparm2))-0.2)/(0.8)*(Hupper-0.2);
//        steep2=0.20001+(steepness-0.2)/(0.8)*(Hupper-0.2);
//Shep_top=5.0*steepness*(1.0-Shepherd_c2)*(SPR_temp*Recr_virgin)/SSB_virgin-(1.0-5.0*steepness*Shepherd_c2);
//Shep_bot=5.0*steepness-1.0;
//Shep_top2=posfun(Shep_top,0.001,temp);
//R_equil=(SSB_virgin/SPR_temp) * pow((Shep_top2/Shep_bot),(1.0/Shepherd_c));
//B_equil=R_equil*SPR_temp;

void srEquationDialog::updateShepherdReParm()
{
    double SPRtemp = pop->SR()->getSPR();
    double logRecr_virgin_adj = ui->doubleSpinBox_1_value->value();
    double SRparm2 = ui->doubleSpinBox_2_value->value();
    double SRparm3 = ui->doubleSpinBox_3_value->value();
    double Recr_virgin_adj = exp(logRecr_virgin_adj);
    ui->doubleSpinBox_1_trans->setValue(Recr_virgin_adj);
    ui->doubleSpinBox_2_trans->setValue(SRparm2);
    ui->doubleSpinBox_3_trans->setValue(SRparm3);
    double Shep_top, penalty, SSB_virgin, Recr_virgin = 1;
    double Shep_bot;
    double Shep_top2;
    double Shepherd_c=exp(SRparm3);
    double Shepherd_c2=pow(0.2,Shepherd_c);
    double Hupper=1.0/(5.0*Shepherd_c2);
    double steepness=0.20001+((0.8)/(1.0+exp(-SRparm2))-0.2)/(0.8)*(Hupper-0.2);
    for (int i = 0; i < xValList.count(); i++)
    {
        SSB_virgin = xValList.at(i);
        Shep_top=5.0*steepness*(1.0-Shepherd_c2)*(SPRtemp*Recr_virgin)/SSB_virgin-(1.0-5.0*steepness*Shepherd_c2);
        Shep_bot=5.0*steepness-1.0;
        Shep_top2=posfun(Shep_top,0.001,penalty);
        double R_equil=(SSB_virgin/SPRtemp) * pow((Shep_top2/Shep_bot),(1.0/Shepherd_c));
        double B_equil=R_equil*SPRtemp;
    }
}

// 10: Ricker re-parameterization (beta) - power
//  3 Parameters: log(R0), steepness, and Ricker power, gamma.
void srEquationDialog::rickerReParm()
{
    QString msg (QString("Option 10: Ricker Power - Beta"));
    setTitle(msg);
    setLabel("Shepherd");

    chartview->setVisible(false);

    ui->label_1_name->setText("SR_LN(R0)");
    ui->label_1_type->setText("Exp");
    ui->label_2_name->setText("SR_RkrPwr_steep");
    ui->label_2_type->setText("Value");
    ui->label_3_name->setText("SR_RkrPwr_gamma");
    ui->label_3_type->setText("Value");
    setParameterHeaders();

    numSliders = 3;
    showSliders(numSliders);
    updateRickerReParm();
    chartview->setVisible(true);

    cht->addAxis(axisXsel, Qt::AlignBottom);
    cht->addAxis(axisY, Qt::AlignLeft);
}

// steepness = SR_parm_work(2);
// dvariable RkrPower=SR_parm_work(3);
// temp=SSB_curr_adj/SSB_virgin_adj;
// temp2 = posfun(1.0-temp,0.0000001,temp3);
// temp=1.0-temp2;  //  Rick's new line to stabilize recruitment at R0 if B>B0
// dvariable RkrTop =  log(5.0*steepness) * pow(temp2,RkrPower) / pow(0.8,RkrPower);
// NewRecruits = Recr_virgin_adj * temp * mfexp(RkrTop);

void srEquationDialog::updateRickerReParm()
{
//    double SB_virgin_adj = 1.0;
    double SB_curr_adj, Recruits;
    double logRecr_virgin_adj = ui->doubleSpinBox_1_value->value();
    double steepness = ui->doubleSpinBox_2_value->value();
    double rkrPower = ui->doubleSpinBox_3_value->value();
    double rkrTop = 1.0;
    double temp, temp2, penalty;

    double Recr_virgin_adj = exp(logRecr_virgin_adj);
    ui->doubleSpinBox_1_trans->setValue(Recr_virgin_adj);
    ui->doubleSpinBox_2_trans->setValue(steepness);
    ui->doubleSpinBox_3_trans->setValue(rkrPower);

    cht->removeSeries(selSeries);
    cht->update();
    selSeries->clear();

    // SSB_virgin_adj = 1.0, so it factors out
    for (int i = 0; i < xValList.count(); i++)
    {
        SB_curr_adj = xValList.at(i);
        temp = SB_curr_adj; // / SB_virgin_adj;
        temp2 = posfun ((1.0 - temp), 0.0000001, penalty);
        temp = 1.0 - temp2;  // Rick's new line to stabilize recruitment at R0 if B> R0
        rkrTop = log(5.0 * steepness) * pow(temp2, rkrPower) / pow(0.8, rkrPower);
        Recruits = Recr_virgin_adj * temp * exp (rkrTop);

        selSeries->append(SB_curr_adj, Recruits);
    }
    cht->addSeries(selSeries);
}

// JoinFunction from tpl code:
//FUNCTION dvariable Join_Fxn(const prevariable& MinPoss, const prevariable& MaxPoss, const prevariable& Inflec, const prevariable& Xvar, const prevariable& Y1, const prevariable& Y2)
//  {
//    RETURN_ARRAYS_INCREMENT();
//  dvariable Yresult;
//  dvariable join;
//  join=1.000/(1.000+mfexp(1000.0*(Xvar-Inflec)/(MaxPoss-MinPoss)));  //  steep joiner at the inflection
//  Yresult=Y1*(join)+Y2*(1.000-join);
//    RETURN_ARRAYS_DECREMENT();
//  return Yresult;
//  }
double srEquationDialog::joinFunction(double minPoss, double maxPoss, double inflec, double xVar, double y1, double y2)
{
    double result = 0.0;
    double join;
    join = 1.0 / (1.0 + exp(1000.0 * (xVar - inflec) / (maxPoss - minPoss))); // steep joiner at the inflection
    result = y1 * (join) + y2 * (1.0 - join);
    return result;
}

// Evaluate a line between two points at a specific x value
double srEquationDialog::evaluateLine(QPointF pt1, QPointF pt2, double x)
{
    double slope = (pt2.y() - pt1.y()) / (pt2.x() - pt1.x());
    double y = slope * (x - pt2.x()) + pt2.y();
    return y;
}


/** Returns the minimum x-value of a point list */
double srEquationDialog::minXvalue(const QList<QPointF> &pointlist)
{
    double value = 1000.0;
    for (int i = 0; i < pointlist.count(); i++)
    {
        if (pointlist.at(i).x() < value)
            value = pointlist.at(i).x();
    }
    return value;
}

/** Returns the maximum x-value of a point list */
double srEquationDialog::maxXvalue(const QList<QPointF> &pointlist)
{
    double value = -1000.0;
    for (int i = 0; i < pointlist.count(); i++)
    {
        if (pointlist.at(i).x() > value)
            value = pointlist.at(i).x();
    }
    return value;
}

/** Returns the minimum y-value of a point list */
double srEquationDialog::minYvalue(const QList<QPointF> &pointlist)
{
    double value = 1000.0;
    for (int i = 0; i < pointlist.count(); i++)
    {
        if (pointlist.at(i).y() < value)
            value = pointlist.at(i).y();
    }
    return value;
}

/** Returns the maximum y-value of a point list */
double srEquationDialog::maxYvalue(const QList<QPointF> &pointlist)
{
    double value = -1000;
    for (int i = 0; i < pointlist.count(); i++)
    {
        if (pointlist.at(i).y() > value)
            value = pointlist.at(i).y();
    }
    return value;
}

/** Returns the average of the values of a float list */
double srEquationDialog::aveXvalue(const QList<float> &xvals)
{
    double value = 0.;
    int i = 0;
    for (i = 0; i < xvals.count(); i++)
        value += static_cast<double>(xvals.at(i));
    value /= i;
    return value;
}

/** Returns the average of the y-values of a point list */
double srEquationDialog::aveYvalue(const QList<QPointF> &pointlist, int start, int stop)
{
    double value = 0.;
    double count = 0.0;
    int i = 0, end = 2;
    int listend = pointlist.count() - 1;
    if (start > listend)
        start = listend;
    if (start < 0)
        start = 0;
    if (stop > listend)
        stop = listend;
    if (stop < 0)
        stop = 0;

    i = start;
    end = stop;

    for (; i <= end; i++)
    {
        value += pointlist.at(i).y();
        count += 1.0;
    }
    return (value / count);
}

void srEquationDialog::fillValues(const QList<QPointF> fewpoints, QList<double> xvals, QList<QPointF> &fullpoints)
{
    int i = 0, j =0;
    double yVal = 0;
    int fplimit = fewpoints.count() - 1;

    while (xvals.at(j) < fewpoints.at(0).x())
    {
        yVal = 0.0;
        fullpoints.append(QPointF(xvals.at(j++), yVal));
    }
    for (i = 0; i < fplimit; i++)
    {
        while (j < xvals.count() && xvals.at(j) < fewpoints.at(i+1).x())
        {
            yVal = evaluateLine(fewpoints.at(i), fewpoints.at(i+1), xvals.at(j));
            fullpoints.append(QPointF(xvals.at(j++), yVal));
        }
    }
    yVal = fewpoints.last().x();
    for (; j < xvals.count(); j++)
    {
        fullpoints.append(QPointF(xvals.at(j), yVal));
    }
}

// posfun from ADMB source (more or less)
double srEquationDialog::posfun(const double &x, const double eps, double& pen)
{
  if (x >= eps)
  {
    return x;
  }
  else
  {
    pen += .01 * (x-eps * x-eps);
    return eps / (2-x/eps);
  }
}
