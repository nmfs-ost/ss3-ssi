#include "equationdialog.h"
#include "ui_equationdialog.h"

#include <QAbstractButton>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QMessageBox>

QT_CHARTS_USE_NAMESPACE

using namespace tk;

#define SLIDER_SPAN  100000

#define SLIDER_SCALE 10000
#define VALUE_SCALE .0001

#include <cmath>

double neglog19 = -1.0 * log(19);

equationDialog::equationDialog(QWidget *parent, QString *typ) :
    QDialog(parent),
    ui(new Ui::equationDialog)
{
    ui->setupUi(this);

    type = QString (*typ);
    name = QString (QString("%1 Selectivity").arg(type));
    title = name;

    selex = NULL;
    fleet = NULL;
    genders = 2;

    equationNum = -1;
    parameters = NULL;
    numParams = 0;
    numSliders = 0;

    yMax = -10;
    chartview = NULL;

    setXvals(QStringList());
    resetChart(true);
    selSeries->append(0, 0);
    selSeries->append(1, 0);
    selSeries->append(2, 0);
    cht->addSeries(selSeries);
    chartview->setChart(cht);

    setSlider1(-5, 5, 0);
    value1Changed(0);
    setSlider2(-5, 5, 0);
    value2Changed(0);
    setSlider3(-5, 5, 0);
    value3Changed(0);
    setSlider4(-5, 5, 0);
    value4Changed(0);
    setSlider5(-5, 5, 0);
    value5Changed(0);
    setSlider6(-5, 5, 0);
    value6Changed(0);
    setSlider7(-5, 5, 0);
    value7Changed(0);
    setSlider8(-5, 5, 0);
    value8Changed(0);
    setSlider9(-5, 5, 0);
    value9Changed(0);

    join1 = join2 = join3 = 10;
    ui->spinBox_steep_join1->setValue(join1);
    ui->spinBox_steep_join2->setValue(join2);
    ui->spinBox_steep_join3->setValue(join3);

    binMin = 0; binMax = 20; binWidth = 2; binMidWidth = 0.;
    ui->spinBox_bins_min->setValue(binMin);
    ui->spinBox_bins_max->setValue(binMax);
    ui->spinBox_bins_width->setValue(binWidth);
    ui->doubleSpinBox_bins_midbin->setValue(binMidWidth);

//    connect (ui->spinBox_bins_min, SIGNAL(valueChanged(int)), SLOT(binMinChanged(int)));
//    connect (ui->spinBox_bins_max, SIGNAL(valueChanged(int)), SLOT(binMaxChanged(int)));
//    connect (ui->spinBox_bins_width, SIGNAL(valueChanged(int)), SLOT(binWidthChanged(int)));

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

    connect (ui->spinBox_steep_join1, SIGNAL(valueChanged(int)), SLOT(join1Changed(int)));
    connect (ui->spinBox_steep_join2, SIGNAL(valueChanged(int)), SLOT(join2Changed(int)));
    connect (ui->spinBox_steep_join3, SIGNAL(valueChanged(int)), SLOT(join3Changed(int)));

    connect (this, SIGNAL(numbersUpdated()), SLOT(updateSel()));
    connect (this, SIGNAL(linearUpdated(float)), SLOT(updateLinearExp(float)));

    connect (ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), SLOT(buttonClicked(QAbstractButton*)));
    building = false;
    waiting = false;
    updating = false;
}

equationDialog::~equationDialog()
{
    delete ui;
}

void equationDialog::setSelex (selectivity *slx)
{
    if (selex != NULL &&
            selex->getNumXvals() > 0)
    {
        disconnect(selex, SIGNAL(dataChanged()), this, SLOT(changeSelex()));
        disconnect(selex, SIGNAL(startingSetupChanges()), this, SLOT(changingSelex()));
        disconnect(selex, SIGNAL(setupChanged(QStringList)), this, SLOT(changedSelex(QStringList)));
    }
    selex = slx;
    genders = selex->getNumGenders();
    connect(selex, SIGNAL(dataChanged()), this, SLOT(changeSelex()));
    connect(selex, SIGNAL(startingSetupChanges()), this, SLOT(changingSelex()));
    connect(selex, SIGNAL(setupChanged(QStringList)), this, SLOT(changedSelex(QStringList)));
    setParameters(selex->getParameterModel());
    changeSelex();
}

void equationDialog::changingSelex()
{
    disconnect(parameters, SIGNAL(dataChanged()), this, SLOT(parametersChanged()));
}

void equationDialog::changedSelex(QStringList ql)
{
    connect(parameters, SIGNAL(dataChanged()), this, SLOT(parametersChanged()));
    changeSelex();
}

void equationDialog::changeSelex()
{
    restoreAll();
}

/*void equationDialog::setDataModel (ss_model *data)
{
    dataModel = data;
}*/

void equationDialog::setXvals(const QStringList &vals)
{
    xValList.clear();

    if (vals.isEmpty())
    {
        xValList.append(0);
        xValList.append(1);
        xValList.append(2);
    }
    else
    {
        for (int i = 0; i < vals.count(); i++)
            xValList.append(QString(vals.at(i)).toFloat());

        ui->spinBox_bins_min->setValue(xValList.first());
        ui->spinBox_bins_max->setValue(xValList.last());
        ui->spinBox_bins_width->setValue(xValList.at(2) - xValList.at(1));
    }
}

void equationDialog::resetXvals()
{
    setXvals(QStringList());
    update();
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
    disconnect(parameters, SIGNAL(dataChanged()), this, SLOT(parametersChanged()));
    parameters = params;
    connect(parameters, SIGNAL(dataChanged()), this, SLOT(parametersChanged()));
}

void equationDialog::getParameterValues()
{
    QStringList values;
    int switchnum = numParams;
    equationNum = selex->getPattern();
    special = selex->getSpecial();
    genders = selex->getNumGenders();
    parameters = selex->getParameterModel();
    if (NULL == parameters)
        switchnum = numParams = 0;
    else
        numParams = parameters->rowCount();

//    if (parameters != NULL)
    {
//        int oldnum = numParams;
//        numParams = parameters->rowCount();
        if (switchnum != numParams)
            disconnect (parameters, SIGNAL(dataChanged()), this, SLOT(parametersChanged()));


        disconnect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));

        switch (switchnum)
        {
        default:
        case 9:
            values = parameters->getRowData(8);
            min9 = (values.at(0)).toDouble();
            max9 = (values.at(1)).toDouble();
            val9 = (values.at(2)).toDouble();
            ui->doubleSpinBox_9_input->setRange(val9 < min9? val9: min9, max9);
            ui->doubleSpinBox_9_input->setValue(val9);
            setSlider9(min9, max9, val9);
        case 8:
            values = parameters->getRowData(7);
            min8 = (values.at(0)).toDouble();
            max8 = (values.at(1)).toDouble();
            val8 = (values.at(2)).toDouble();
            ui->doubleSpinBox_8_input->setMinimum(min8);
            ui->doubleSpinBox_8_input->setMaximum(max8);
            ui->doubleSpinBox_8_input->setValue(val8);
            setSlider8(min8, max8, val8);
        case 7:
            values = parameters->getRowData(6);
            min7 = (values.at(0)).toDouble();
            max7 = (values.at(1)).toDouble();
            val7 = (values.at(2)).toDouble();
            ui->doubleSpinBox_7_input->setMinimum(min7);
            ui->doubleSpinBox_7_input->setMaximum(max7);
            ui->doubleSpinBox_7_input->setValue(val7);
            setSlider7(min7, max7, val7);
        case 6:
            values = parameters->getRowData(5);
            min6 = (values.at(0)).toDouble();
            max6 = (values.at(1)).toDouble();
            val6 = (values.at(2)).toDouble();
            ui->doubleSpinBox_6_input->setMinimum(min6);
            ui->doubleSpinBox_6_input->setMaximum(max6);
            ui->doubleSpinBox_6_input->setValue(val6);
            setSlider6(min6, max6, val6);
        case 5:
            values = parameters->getRowData(4);
            min5 = (values.at(0)).toDouble();
            max5 = (values.at(1)).toDouble();
            val5 = (values.at(2)).toDouble();
            ui->doubleSpinBox_5_input->setMinimum(min5);
            ui->doubleSpinBox_5_input->setMaximum(max5);
            ui->doubleSpinBox_5_input->setValue(val5);
            setSlider5(min5, max5, val5);
        case 4:
            values = parameters->getRowData(3);
            min4 = (values.at(0)).toDouble();
            max4 = (values.at(1)).toDouble();
            val4 = (values.at(2)).toDouble();
            ui->doubleSpinBox_4_input->setMinimum(min4);
            ui->doubleSpinBox_4_input->setMaximum(max4);
            ui->doubleSpinBox_4_input->setValue(val4);
            setSlider4(min4, max4, val4);
        case 3:
            values = parameters->getRowData(2);
            min3 = (values.at(0)).toDouble();
            max3 = (values.at(1)).toDouble();
            val3 = (values.at(2)).toDouble();
            ui->doubleSpinBox_3_input->setMinimum(min3);
            ui->doubleSpinBox_3_input->setMaximum(max3);
            ui->doubleSpinBox_3_input->setValue(val3);
            setSlider3(min3, max3, val3);
        case 2:
            values = parameters->getRowData(1);
            min2 = (values.at(0)).toDouble();
            max2 = (values.at(1)).toDouble();
            val2 = (values.at(2)).toDouble();
            ui->doubleSpinBox_2_input->setMinimum(min2);
            ui->doubleSpinBox_2_input->setMaximum(max2);
            ui->doubleSpinBox_2_input->setValue(val2);
            setSlider2(min2, max2, val2);
        case 1:
            values = parameters->getRowData(0);
            min1 = (values.at(0)).toDouble();
            max1 = (values.at(1)).toDouble();
            val1 = (values.at(2)).toDouble();
            ui->doubleSpinBox_1_input->setMinimum(min1);
            ui->doubleSpinBox_1_input->setMaximum(max1);
            ui->doubleSpinBox_1_input->setValue(val1);
            setSlider1(min1, max1, val1);
            showSliders(numParams);
            break;
        case 0:
            showSliders(0);
        }
        connect (this, SIGNAL(numbersUpdated()), SLOT(updateSel()));
        if (switchnum != numParams)
            connect (parameters, SIGNAL(dataChanged()), SLOT(parametersChanged()));

//        resetValues();
    }
//    else
//    {
//        numParams = 0;
//    }
}

void equationDialog::setParameterValues()
{
    QStringList values;
    if (parameters != NULL)
    {
    disconnect (parameters, SIGNAL(dataChanged()), this, SLOT(parametersChanged()));

    switch (numSliders)
    {
    case 9:
        values = parameters->getRowData(8);
        values[0] = QString::number(min9);
        values[1] = QString::number(max9);
        values[2] = QString::number(val9);
        parameters->setRowData(8, values);
    case 8:
        values = parameters->getRowData(7);
        values[0] = QString::number(min8);
        values[1] = QString::number(max8);
        values[2] = QString::number(val8);
        parameters->setRowData(7, values);
    case 7:
        values = parameters->getRowData(6);
        values[0] = QString::number(min7);
        values[1] = QString::number(max7);
        values[2] = QString::number(val7);
        parameters->setRowData(6, values);
    case 6:
       values = parameters->getRowData(5);
       values[0] = QString::number(min6);
       values[1] = QString::number(max6);
       values[2] = QString::number(val6);
       parameters->setRowData(5, values);
    case 5:
        values = parameters->getRowData(4);
        values[0] = QString::number(min5);
        values[1] = QString::number(max5);
        values[2] = QString::number(val5);
        parameters->setRowData(4, values);
    case 4:
        values = parameters->getRowData(3);
        values[0] = QString::number(min4);
        values[1] = QString::number(max4);
        values[2] = QString::number(val4);
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
    case 0:
    default:
        ;
    }
    connect (parameters, SIGNAL(dataChanged()), SLOT(parametersChanged()));
    }
}

void equationDialog::setParameterHeaders()
{
    QString fltNum(QString::number(fleetNum));
    QString fltTyp;
    Fleet::FleetType ft = fleet->getType();
    if     (ft == Fleet::Fishing ||
            ft == Fleet::Bycatch)
        fltTyp = QString("fishery");
    else if (ft == Fleet::Survey)
        fltTyp = QString("survey");

    for (int i = 0; i < parameters->rowCount(); i++)
    {
        parameters->setRowHeader(i, QString("%1Sel p%2 %3 (%4)").arg(
                     type, QString::number(i+1), fltTyp, fltNum));
    }
}

void equationDialog::refresh()
{
    ui->doubleSpinBox_bins_midbin->setValue(binMidWidth);
    restoreAll();
}

void equationDialog::update()
{
    if (parameters == NULL)
        return;
    if (selex == NULL)
        return;

/*    if (waiting)
    {
        return;
    }
    if (building)
    {
        waiting = true;
        while (building)
            continue;
        waiting = false;
        update();
    }*/
//    else // if (!building)
    while (updating)
        continue;

    {
        updating = true;
//        building = true;
        disconnect (selex, SIGNAL(dataChanged()), this, SLOT(changeSelex()));

        resetChart();

        switch (equationNum)
        {
        case 0:  // case 0 constant size selectivity
        case 10: // 10 #Constant age-specific selex for ages 1 to nages
            numSliders = 0;
            constant();
            break;

        case 11: // 11 #Constant age-specific selex for specified age range
            numSliders = 2;
            constantRange();
            break;

        case 1:  // case 1 logistic size selectivity
        case 12: // 12 #age selectivity - logistic
            numSliders = 2;
            logistic();
            break;

        case 2:  // case 2 discontinued; use pattern 8 for double logistic
        case 7:  // case 7 discontinued; use pattern 8 for double logistic
            numSliders = 8;
            blank(8, 8);
            break;

        case 3:  // case 3 discontinued
            numSliders = 6;
            blank (6, 0);
            break;

        case 13: // 13 #age selectivity - double logistic
            numSliders = 8;
            dblLogPeak();
            break;

        case 4:  // case 4 discontinued; use pattern 30 to get spawning biomass
            numSliders = 0;
            blank(0, 30);
            break;

        case 14: // 14 #age selectivity - separate parm for each age
            numSliders = 0;
            eachAge();
            break;

        case 5:  // case 5 mirror another fleets size selectivity for specified bin range
            numSliders = 2;
        case 15: // case 15 mirror another fleets size selectivity for all size bins
                 // 15 #age selectivity - mirror selex for lower numbered fleet
            mirror(numSliders);
            break;

        case 6:  // case 6 non-parametric size selex pattern
            numSliders = 2;
            linear(0);
            break;

        case 16: // case 16 Coleraine - Gaussian age selectivity
            numSliders = 2;
            coleGauss();
            break;

        case 17: // 17 #age selectivity: each age has parameter as random walk
            //          transformation is selex=exp(parm); some special codes
            numSliders = 0;
            randomWalk(0);
            break;

        case 8:  // case 8 double logistic with eight parameters
        case 18: // 18 #age selectivity: double logistic with smooth transition
            numSliders = 8;
            dblLogPeak();
            break;

        case 9:  // case 9 old double logistic with 4 parameters
        case 19: // 19 #age selectivity: old double logistic
            numSliders = 6;
            dblLogistic();
            break;

        case 21: // case 21 non-parametric size selectivity
            numSliders = 0;
            linear(1);
            break;

        case 22: // case 22 size selectivity using double_normal_plateau (similar to CASAL)
            numSliders = 4;
            dblNormCasal();
            break;

        case 20: // 20 #age selectivity: double normal with plateau
        case 23: // case 23 size selectivity double_normal_plateau where final value can be greater than 1.0
        case 24: // case 24 size selectivity using double_normal_plateau and lots of bells and whistles
            numSliders = 6;
            dblNormEndpts();
            break;

        case 25: // case 25 size selectivity using exponential-logistic
        case 26: // 26 #age selectivity: exponential logistic
            numSliders = 3;
            expLogistic();
            break;

        case 27: // 27 #age selectivity: cubic spline
            numSliders = 3;
            cubicSpline();
            break;

        case 41: // 41 each age has parameter as random walk scaled by average of values at low age through high age
            //    transformation as selex=exp(parm); some special codes
            numSliders = 2;
            randomWalk(2.);
            break;

        case 42: // 42 cubic spline scaled by average of values at low age through high age
            numSliders = 5;
            cubicSpline(2.);
            break;

        case 43: // 43 non-parametric size selex scaled by average of values at low bin through high bin
            numSliders = 4;
            linearScaled();
            break;

        case 44: // 44 like age selex 17 but with separate parameters for males and with revised controls
            numSliders = genders + 2;
            blank(numSliders, 0, "Not yet implemented");
//            twoSexRandomWalk();
            break;

        case 45: // 45 like age selex 14 but with separate parameters for males and with revised controls
            numSliders = genders + 2;
            twoSexEachAge();
            break;

        default:
            numSliders = 0;
            blank(0);
            break;

        }
        connect (selex, SIGNAL(dataChanged()), this, SLOT(changeSelex()));
//        building = false;
        updating = false;
    }
    QDialog::update();
}

void equationDialog::updateSel()
{
    if (parameters == NULL)
        return;
    if (selex == NULL)
        return;

    if (waiting)
    {
        return;
    }
    if (building)
    {
        waiting = true;
        while (building)
            continue;
        waiting = false;
    }
//    if (!building)
    {
        building = true;
        disconnect (parameters, SIGNAL(dataChanged()), this, SLOT(parametersChanged()));
        disconnect (selex, SIGNAL(dataChanged()), this, SLOT(changeSelex()));
        switch (equationNum)
        {
        case 0:  // case 0 constant size selectivity
        case 10: // 10 #Constant age-specific selex for ages 1 to nages
            updateConstant();
            break;

        case 11: // 11 #Constant age-specific selex for specified age range
            updateConstantRange();
            break;

        case 1:  // case 1 logistic size selectivity
        case 12: // 12 #age selectivity - logistic
            updateLogistic();
            break;

        case 5:  // case 5 mirror another fleets size selectivity for specified bin range
            updateMirror(2.);
            break;

        case 6:  // case 6 non-parametric size selex pattern
            updateLinear(0);
            break;

        case 8:  // case 8 double logistic with eight parameters
            updateDblLogPeak();
            break;

        case 18: // 18 #age selectivity: double logistic with smooth transition
            updateDblLogSmooth();
            break;

        case 9:  // case 9 old double logistic with 4 parameters
        case 19: // 19 #age selectivity: old double logistic
            updateDblLogistic();
            break;

        case 14: // 14 #age selectivity - separate parm for each age
            updateEachAge();
            break;

        case 16: // case 16 Coleraine - Gaussian age selectivity
            updateColeGauss();
            break;

        case 17: // 17 #age selectivity: each age has parameter as random walk
            //          transformation is selex=exp(parm); some special codes
            updateRandomWalk();
            break;

        case 20: // 20 #age selectivity: double normal with plateau
        case 24: // case 24 size selectivity using double_normal_plateau and lots of bells and whistles
            updateDblNormEndpts();
            break;

        case 21: // case 21 non-parametric size selectivity
            updateLinearPlain();
            break;

        case 22: // case 22 size selectivity using double_normal_plateau (similar to CASAL)
            updateDblNormCasal();
            break;

        case 23: // case 23 size selectivity double_normal_plateau where final value can be greater than 1.0
            updateDblNormPlateau();
            break;

        case 25: // case 25 size selectivity using exponential-logistic
        case 26: // 26 #age selectivity: exponential logistic
            updateExpLogistic();
            break;

        case 27: // 27 #age selectivity: cubic spline
            updateCubicSpline();
            break;

        case 41: // 41 each age has parameter as random walk scaled by average of values at low age through high age
            //    transformation as selex=exp(parm); some special codes
            updateRandomWalk(2.);
            break;

        case 42: // 42 cubic spline scaled by average of values at low age through high age
            updateCubicSpline(2.);
            break;

        case 43: // 43 non-parametric size selex scaled by average of values at low bin through high bin
            updateLinear(2.);
            break;

        case 44: // 44 like age selex 17 but with separate parameters for males and with revised controls
            updateTwoSexRandom();
            break;

        case 45: // 45 like age selex 14 but with separate parameters for males and with revised controls
            updateTwoSexEachAge();
            break;

        default:
            break;
        }
        connect (parameters, SIGNAL(dataChanged()), this, SLOT(parametersChanged()));
        connect (selex, SIGNAL(dataChanged()), this, SLOT(changeSelex()));
        building = false;
    }
}

void equationDialog::setSlider1(double min, double max, double value)
{
    min1 = min; max1 = max;
    ui->doubleSpinBox_1_max->setValue(max1);
    ui->doubleSpinBox_1_min->setValue(min1);
    ui->horizontalSlider_1->setMaximum(max1 * SLIDER_SCALE);
    ui->horizontalSlider_1->setMinimum(min1 * SLIDER_SCALE);
    ui->horizontalSlider_1->setValue(value * SLIDER_SCALE);
//    ui->doubleSpinBox_1_value->setMinimum(min1);
    ui->doubleSpinBox_1_value->setMaximum(max1);
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
//        ui->doubleSpinBox_1_value->setMinimum(min1);
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
        ui->doubleSpinBox_1_value->setMaximum(max1);
        ui->horizontalSlider_1->setMaximum(max1 * SLIDER_SCALE);
    }
}

void equationDialog::setSlider2(float min, float max, float value)
{
    min2 = min; max2 = max;
    ui->doubleSpinBox_2_max->setValue(max2);
    ui->doubleSpinBox_2_min->setValue(min2);
    ui->horizontalSlider_2->setMaximum(max2 * SLIDER_SCALE);
    ui->horizontalSlider_2->setMinimum(min2 * SLIDER_SCALE);
    ui->horizontalSlider_2->setValue(value * SLIDER_SCALE);
    ui->doubleSpinBox_2_value->setMaximum(max2);
//    ui->doubleSpinBox_2_value->setMinimum(min2);
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
//        ui->doubleSpinBox_2_value->setMinimum(min2);
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
        ui->doubleSpinBox_2_value->setMaximum(max2);
        ui->horizontalSlider_2->setMaximum(max2 * SLIDER_SCALE);
    }
}

void equationDialog::setSlider3(float min, float max, float value)
{
    min3 = min; max3 = max;
    ui->doubleSpinBox_3_max->setValue(max3);
    ui->doubleSpinBox_3_min->setValue(min3);
    ui->horizontalSlider_3->setMaximum(max3 * SLIDER_SCALE);
    ui->horizontalSlider_3->setMinimum(min3 * SLIDER_SCALE);
    ui->horizontalSlider_3->setValue(value * SLIDER_SCALE);
    ui->doubleSpinBox_3_value->setMaximum(max3);
//    ui->doubleSpinBox_3_value->setMinimum(min3);
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
//        ui->doubleSpinBox_3_value->setMinimum(min3);
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
    ui->horizontalSlider_4->setMaximum(max4 * SLIDER_SCALE);
    ui->horizontalSlider_4->setMinimum(min4 * SLIDER_SCALE);
    ui->horizontalSlider_4->setValue(value * SLIDER_SCALE);
    ui->doubleSpinBox_4_value->setMaximum(max4);
//    ui->doubleSpinBox_4_value->setMinimum(min4);
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
//        ui->doubleSpinBox_4_value->setMinimum(min4);
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
    ui->horizontalSlider_5->setMaximum(max5 * SLIDER_SCALE);
    ui->horizontalSlider_5->setMinimum(min5 * SLIDER_SCALE);
    ui->horizontalSlider_5->setValue(value * SLIDER_SCALE);
    ui->doubleSpinBox_5_value->setMaximum(max5);
//    ui->doubleSpinBox_5_value->setMinimum(min5);
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
//        ui->doubleSpinBox_5_value->setMinimum(min5);
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
    ui->horizontalSlider_6->setMaximum(max6 * SLIDER_SCALE);
    ui->horizontalSlider_6->setMinimum(min6 * SLIDER_SCALE);
    ui->horizontalSlider_6->setValue(value * SLIDER_SCALE);
    ui->doubleSpinBox_6_value->setMaximum(max6);
//    ui->doubleSpinBox_6_value->setMinimum(min6);
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
        if (min6 > max6)
        {
            min6 = max6;
        }
        ui->doubleSpinBox_6_min->setValue(min6);
//        ui->doubleSpinBox_6_value->setMinimum(min6);
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
    ui->horizontalSlider_7->setMaximum(max7 * SLIDER_SCALE);
    ui->horizontalSlider_7->setMinimum(min7 * SLIDER_SCALE);
    ui->horizontalSlider_7->setValue(value * SLIDER_SCALE);
    ui->doubleSpinBox_7_value->setMaximum(max7);
//    ui->doubleSpinBox_7_value->setMinimum(min7);
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
//        ui->doubleSpinBox_7_value->setMinimum(min7);
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
    ui->horizontalSlider_8->setMaximum(max8 * SLIDER_SCALE);
    ui->horizontalSlider_8->setMinimum(min8 * SLIDER_SCALE);
    ui->horizontalSlider_8->setValue(value * SLIDER_SCALE);
    ui->doubleSpinBox_8_value->setMaximum(max8);
//    ui->doubleSpinBox_8_value->setMinimum(min8);
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
//        ui->doubleSpinBox_8_value->setMinimum(min8);
        ui->horizontalSlider_8->setMinimum(min8 * SLIDER_SCALE);
    }
}

void equationDialog::max8Changed (double value)
{
    if (max8 != value)
    {
        max8 = value;
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
    ui->horizontalSlider_9->setMaximum(max9 * SLIDER_SCALE);
    ui->horizontalSlider_9->setMinimum(min9 * SLIDER_SCALE);
    ui->horizontalSlider_9->setValue(value * SLIDER_SCALE);
    ui->doubleSpinBox_9_value->setMaximum(max9);
//    ui->doubleSpinBox_9_value->setMinimum(min9);
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
//        ui->doubleSpinBox_9_value->setMinimum(min9);
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
    emit numbersUpdated();
}

void equationDialog::join2Changed (int value)
{
    join2 = value;
    emit numbersUpdated();
}

void equationDialog::join3Changed (int value)
{
    join3 = value;
    emit numbersUpdated();
}

void equationDialog::apply()
{
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
//    getParameterValues();
}

void equationDialog::reset()
{
    resetValues();
    emit numbersUpdated();
}

void equationDialog::resetValues()
{
//    if (!building)
    {
        disconnect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));
//        building = true;
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
//        building = false;
        connect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));
    }
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

    equationNum = selex->getPattern();
    special = selex->getSpecial();
    male = selex->getMale();
    // get values from parameters
    getParameterValues();

    update();
}

void equationDialog::close()
{
    emit closed();
    hide();
}

void equationDialog::buttonClicked(QAbstractButton *btn)
{
    if      (btn->text().contains("Apply")) {
        apply();
    }
    else if (btn->text().contains("Reset")){
        reset();
    }
    else if (btn->text().contains("Defaults")){
        selex->setDefaultParams(equationNum, special);
        restoreAll();
    }
    else if (btn->text().contains("Close")){
        close();
    }
}

void equationDialog::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    emit closed();
}

void equationDialog::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    updateTicks(cht->rect());
}

void equationDialog::parametersChanged()
{
    getParameterValues();
//    resetValues();
    emit numbersUpdated();
}

void equationDialog::setupChanged()
{
    refresh();
}

QString equationDialog::getTitle() const
{
    return title;
}

void equationDialog::setTitle(const QString &value)
{
    title = value;
    setWindowTitle(QString("%1 -  %2").arg(name, title));
}

QString equationDialog::getName() const
{
    return name;
}

void equationDialog::setName(const QString &value)
{
    name = value;
    setWindowTitle(name);
}

Fleet *equationDialog::getFleet() const
{
    return fleet;
}

void equationDialog::setFleet(Fleet *value)
{
    fleet = value;
    fleetNum = fleet->getNumber();
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
    ui->label_title->hide();
    if (chartview != NULL)
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
        if (ascendSeries != NULL)
            delete ascendSeries;
        if (dscendSeries != NULL)
            delete dscendSeries;
        if (join1Series != NULL)
            delete join1Series;
        if (join2Series != NULL)
            delete join2Series;
        if (join3Series != NULL)
            delete join3Series;
        if (ptSeries != NULL)
            delete ptSeries;
        delete selSeries;
        delete cht;
        delete chartview;
    }
    chartview = new QChartView(this);
    chartview->setChart(new QChart());
    cht = chartview->chart();
    ascendSeries = NULL;
    dscendSeries = NULL;
    join1Series = NULL;
    join2Series = NULL;
    join3Series = NULL;
    ptSeries = NULL;
    selSeries = new QLineSeries(cht);
    selSeries->setName(QString("Selex"));
    axisXsel = new QValueAxis();
    axisY = new QValueAxis();
    axisY->setTitleText(QString("Selex"));
    axisYalt = new QValueAxis();

    firstPoints.clear();
    disconnect (parameters, SIGNAL(dataChanged()), this, SLOT(parametersChanged()));

    yMax = 0;
    ui->verticalLayout_graph->addWidget(chartview);
    chartview->show();
    cht->show();
    cht->legend()->setVisible(false);
    int sizew = cht->size().width();
    int sizeh = cht->size().height();
    resizeEvent(new QResizeEvent(QSize(sizew, sizeh), QSize(sizew, sizeh)));
    repaint();
}

/* size selectivity 2, 3, 4, 7, */
void equationDialog::blank (int num, int rep, QString msg)
{
//    resetChart();
    // If discontinued
    if (msg.isEmpty())
        msg.append(QString("Pattern %1 is discontinued").arg(
                       QString::number(equationNum)));
    else
        msg.prepend(QString("Pattern %1: ").arg(
                        QString::number(equationNum)));
    // If replaced by another pattern
    if (rep > 0)
        msg.append(QString(", use pattern %1 instead").arg(
                       QString::number(rep)));

    setTitle(msg);

    // INTENTIONALLY LEFT BLANK
    msg = QString("\n\n\n THIS PAGE INTENTIONALLY LEFT BANK");
    ui->label_title->setText(msg);
    ui->label_title->show();

    chartview->setVisible(false);
    showSliders(0);
    showBins (false);
    showJoins(0);
}

// case 0 constant size selectivity
// 10 #Constant age-specific selex for ages 1 to nages
// Size selectivity 0, age selectivity 10 */
void equationDialog::constant (float val)
{
    setTitle(QString("Pattern %1: Constant selectivity 1.0").arg(QString::number(equationNum)));
    parameters->setRowCount(0);
    showSliders(0);
    showBins(true);
    showJoins(0);

//    resetChart();

    selSeries->setPen(QPen(QBrush(Qt::red), 3));
    cht->addSeries(selSeries);

    cht->setAxisX(axisXsel, selSeries);
    cht->setAxisY(axisY, selSeries);

    updateConstant(val);
}

// Size selectivity 0, age selectivity 10
void equationDialog::updateConstant (float val)
{
    float binMid = binWidth * binMidWidth;
    float start;
    float end;

    if (!xValList.isEmpty())
    {
        if (equationNum == 0)
            start = xValList.at(0) + binMid;
        else
            start = xValList.at(1) + binMid;
    }
    end = xValList.last() + binMid;
    updateConstant(val, start, end);
}

// general constant graph
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

// case 11: Constant age-specific selex for specified age range
void equationDialog::constantRange ()
{
    setTitle(QString("Pattern %1: Constant selectivity 1.0 within age range").arg(equationNum));
    parameters->setRowCount(2);
    disconnect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));
    max1Changed(xValList.last()); max2Changed(xValList.last());
    min1Changed(xValList.first());min2Changed(xValList.first());
    connect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));
    showSliders(numSliders);
    ui->label_1_name->setText("Lo age");
    ui->label_1_type->setText("Value");
    ui->label_2_name->setText("Hi age");
    ui->label_2_type->setText("Value");
    setParameterHeaders();
    showBins(true);
    showJoins(0);

//    resetChart();

    selSeries->setPen(QPen(QBrush(Qt::red), 3));
    cht->addSeries(selSeries);
    cht->setAxisX(axisXsel, selSeries);
    cht->setAxisY(axisY, selSeries);

    if (parameters->rowCount() == 2)
        updateConstantRange();
}

// 11 #Constant age-specific selex for specified age range
void equationDialog::updateConstantRange (float val)
{
    float par1 = ui->doubleSpinBox_1_value->value();
    float par2 = ui->doubleSpinBox_2_value->value();
    float binMid = binWidth * binMidWidth;
    if (numParams < 2)
        return;
    if (par1 < xValList.first())
        par1 = xValList.first();
    if (par1 >= par2)
        par1 = par2 - .001;
    ui->doubleSpinBox_1_trans->setValue(par1);
    if (par2 < xValList.first())
        par2 = xValList.last();
    if (par2 <= par1)
        par2 = par1 + .001;
    ui->doubleSpinBox_2_trans->setValue(par2);
    float start = par1 + binMid;
    float end = par2 + binMid;

    updateConstant(val, start, end);
}

// case 1 logistic size selectivity
// 12 #age selectivity - logistic
// Size selectivity 1 and age selectivity 12
void equationDialog::logistic ()
{
    setTitle(QString("Pattern %1: Logistic").arg(equationNum));
    disconnect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));
    max1Changed(xValList.last());
    min1Changed(0);
    max2Changed(50);
    min2Changed(-50);
    connect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));
    ui->label_1_name->setText("Value at 50%");
    ui->label_1_type->setText("Value");
    ui->label_2_name->setText("Diff 95% & 50%");
    ui->label_2_type->setText("Value");
    showSliders(2);
    setParameterHeaders();
    showBins(true);
    showJoins(0);

//    resetChart();

    selSeries->setPen(QPen(QBrush(Qt::red), 3));
    cht->addSeries(selSeries);

    cht->setAxisX(axisXsel, selSeries);
    cht->setAxisY(axisY, selSeries);

    if (parameters->rowCount() == 2)
        updateLogistic();
}

// Size selectivity 1 and age selectivity 12
void equationDialog::updateLogistic()
{
    float yVal = 0;
    float temp = 0;
    float len = 0;
    float par1 = ui->doubleSpinBox_1_value->value();
    float par2 = ui->doubleSpinBox_2_value->value();
    if (numParams < 2)
        return;
    if (par1 < 0)
        par1 = xValList.first();
    if (par2 < -50)
        par2 = -50;
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

// case 5 mirror another fleets size selectivity for specified bin range
//   parm1 = lo bin; parm2 = hi bin
// case 15 mirror another fleets size selectivity for all size bins
// 15 #age selectivity - mirror selex for lower numbered fleet
void equationDialog::mirror (int sliders)
{
    int flt = special;
    QString msg (QString("Pattern %1: Mirror of Fleet (%2)").arg(
                 QString::number(equationNum),
                 QString::number(flt)));
    if (flt >= fleet->getNumber())
    {
        msg.append(QString("\nMust use fleet (special) previously defined!"));
    }
    else
    {
//    resetChart();
    chartview->setVisible(false);
    if (sliders == 2)
    {
        if (numParams < 2)
            return;
        int num = xValList.count();
        disconnect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));
        max1Changed(num); max2Changed(num);
        min1Changed(-1.0); min2Changed(-1.0);
        msg.append(QString(" between Lo and Hi bins"));
        connect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));
        ui->label_1_name->setText("Lo Bin");
        ui->label_1_type->setText("Value");
        ui->label_2_name->setText("Hi Bin");
        ui->label_2_type->setText("Value");
        setParameterHeaders();
    }
    }
    setTitle(msg);
    showSliders(sliders);
    showBins (false);
    showJoins(0);

    updateMirror(sliders);
}

// size selectivity 5 and size and age selectivity 15
void equationDialog::updateMirror(int sliders)
{
    int flt = special < 1? 1: special;
    if (fleetNum <= flt)
    {
        QString msg (tr("Mirror must be of previous fleet.\n Change Special value."));
        ui->label_title->setText(msg);
        ui->label_title->setVisible(true);
        return;
    }
    if (sliders == 2)
    {
        QString msg;
        int start = 1, end = 1;
        float par1 = static_cast<int>(ui->doubleSpinBox_1_value->value());
        float par2 = static_cast<int>(ui->doubleSpinBox_2_value->value());
        if (numParams < 2)
            return;
        if (par1 < 1)
        {
            start = 1;
        }
        else
        {
            start = par1;
        }
        if (par2 < 1 || par2 > xValList.count())
        {
            end = xValList.count();
        }
        else
        {
            end = par2;
        }
        ui->doubleSpinBox_1_trans->setValue(float(start));
        ui->doubleSpinBox_2_trans->setValue(float(end));
        if (end < start)
        {
            msg = QString ("Hi Bin is less than Lo Bin. adjusting Hi Bin ...");
            QMessageBox::information(this, tr("Error in Value"), tr(msg.toUtf8()));
            setSlider2(-1, xValList.count(), par1);
            return;
        }
        msg = QString (QString("Mirroring selectivity from Fleet %1 \nFrom Bin %2  to Bin %3").arg(
                           QString::number(getSpecial()),
                           QString::number(start),
                           QString::number(end)));
        ui->label_title->setText(msg);
    }
}

// size selectivity 6 and age selectivity 43
// case 21 non-parametric size selectivity
void equationDialog::linear (float scale)
{
    int numNodes = getSpecial(), num = 0;
    QString msg(QString("Pattern %1: Non-parametric, %2 Linear segments").arg(
                    QString::number(equationNum),
                    QString::number(numNodes + 1)));
    setTitle(msg);

    if (scale == 1)
    {
        linearPlain ();
        return;
    }
    else if (scale == 2)
    {
        linearScaled();
        return;
    }

    num = parameters->rowCount() - 2;
    if (num < 2 || numNodes != num)
    {
        ui->label_title->setText(tr("Incorrect number of parameters. \nShould be Special +2"));
        ui->label_title->setVisible(true);
        return;
    }
    disconnect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));
    max1Changed(xValList.last()); min1Changed(xValList.first());
    max2Changed(xValList.last()); min2Changed(xValList.first());
    max3Changed(9); min3Changed(-5);
    max4Changed(9); min4Changed(-5);
    max5Changed(9); min5Changed(-5);
    max6Changed(9); min6Changed(-5);
    max7Changed(9); min7Changed(-5);
    max8Changed(9); min8Changed(-5);
    max9Changed(9); min9Changed(-5);
    connect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));

    ui->label_1_name->setText("length for P1");
    ui->label_1_type->setText("Value");
    ui->label_2_name->setText("length for P2");
    ui->label_2_type->setText("Value");
    setParameterHeaders();
    if (numNodes > 7)
    {
        msg = QString("Non-parametric - too many segments(%1) to display!").arg(
                    QString::number(numNodes));
        blank(2, 0, msg);
        return;
    }
    else
    {
        showSliders(numNodes + 2);
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
        ui->label_9_name->setText("ln of sel at P2");
        ui->label_9_type->setText("Value");
//        setParameterHeaders();

        if (numNodes == 2)
        {
            ui->label_4_name->setText("ln of sel at P2");
        }
        else if (numNodes == 3)
        {
            ui->label_5_name->setText("ln of sel at P2");
        }
        else if (numNodes == 4)
        {
            ui->label_6_name->setText("ln of sel at P2");
        }
        else if (numNodes == 5)
        {
            ui->label_7_name->setText("ln of sel at P2");
        }
        else if (numNodes == 6)
        {
            ui->label_8_name->setText("ln of sel at P2");
        }
        showBins(true);
        showJoins(0);

//        resetChart();

        axisY->setTitleText("Selex");
        cht->addAxis(axisY, Qt::AlignLeft);
        cht->addAxis(axisXsel, Qt::AlignBottom);

        ascendSeries = new QLineSeries(cht);
        ascendSeries->setPen(QPen(QBrush(Qt::blue), 2));
        ascendSeries->setName(QString("Ln of Selex"));
        cht->addSeries(ascendSeries);

        selSeries->setPen(QPen(QBrush(Qt::red), 3));
        cht->addSeries(selSeries);
        selSeries->attachAxis(axisXsel);
        selSeries->attachAxis(axisY);

        axisYalt->setTitleText("Ln of Selex");

        cht->addAxis(axisYalt, Qt::AlignRight);
        ascendSeries->attachAxis(axisXsel);
        ascendSeries->attachAxis(axisYalt);
        cht->legend()->show();
        cht->legend()->setAlignment(Qt::AlignLeft);
        updateLinear(scale);
    }
}
/* This is the code from SS_selex.tpl
 *    while(j<=nlength)
 *    {
 *      if(len_bins(j)<=sp(z))
 *      {
 *        sel(j) = lastsel + (len_bins(j)-lastSelPoint)/(sp(z)-lastSelPoint) * (sp(z+k)-lastsel);
 *        j++;
 *      }
 *      else if(z<=k)
 *      {
 *        lastSelPoint=sp(z);
 *        lastsel=sp(z+k);
 *        z++;
 *      }
 *      else  //  for sizes beyond last point
 *      {
 *        sel(j)=sp(k+k);
 *        j++;
 *      }
 *    }
 * I've simplified it to just the points given */
// case 21 non-parametric size selectivity
void equationDialog::linearPlain()
{
    int numNodes = getSpecial(), num = 0;
    float min, max;
    QStringList rowdata;
    QString msg(QString("Pattern %1: Non-parametric %2 Linear segments").arg(
                    QString::number(equationNum),
                    QString::number(numNodes + 1)));
    setTitle(msg);

    num = numNodes * 2;
    if (parameters->rowCount() < num)
    {
        QMessageBox::information(this, tr("Parameter error"), tr("Incorrect number of parameters. \nShould be (Special * 2)"));
        return;
    }
    showSliders(0);
    showBins(true);
    showJoins(0);
//    resetChart();

    axisY->setTitleText("Selex");
    cht->addAxis(axisY, Qt::AlignLeft);
    cht->addAxis(axisXsel, Qt::AlignBottom);

    selSeries->setPen(QPen(QBrush(Qt::red), 3));
    cht->addSeries(selSeries);
    selSeries->attachAxis(axisXsel);
    selSeries->attachAxis(axisY);

    setParameterHeaders();
    // set parameter min and max
    min = xValList.first();
    max = xValList.last();
    for (int i = 0; i < special; i++)
    {
        rowdata = parameters->getRowData(i);
        rowdata[0] = min; rowdata[1] = max;
        parameters->setRowData(i, rowdata);
    }
    updateLinearPlain();
}

// age selectivity 43
void equationDialog::linearScaled ()
{
    int numNodes = getSpecial(), num = 0;
    QString msg(QString("Pattern %1: Non-parametric %2 Linear segments").arg(
                    QString::number(equationNum),
                    QString::number(numNodes + 1)));
    setTitle(msg);

    num = parameters->rowCount() - 4;
    if (num < 3 || numNodes != num)
    {
        QMessageBox::information(this, tr("Parameter error"), tr("Incorrect number of parameters. \nShould be Special +4"));
        return;
    }
    disconnect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));
    max1Changed(xValList.count()); min1Changed(1);
    max2Changed(xValList.count()); min2Changed(1);
    max3Changed(xValList.last()); min3Changed(xValList.first());
    max4Changed(xValList.last()); min4Changed(xValList.first());
    max5Changed(9); min5Changed(-5);
    max6Changed(9); min6Changed(-5);
    max7Changed(9); min7Changed(-5);
    max8Changed(9); min8Changed(-5);
    max9Changed(9); min9Changed(-5);
    connect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));

    ui->label_1_name->setText("Scale bin Lo");
    ui->label_1_type->setText("Value");
    ui->label_2_name->setText("Scale bin Hi");
    ui->label_2_type->setText("Value");
    ui->label_3_name->setText("length for P1");
    ui->label_3_type->setText("Value");
    ui->label_4_name->setText("length for P2");
    ui->label_4_type->setText("Value");
    setParameterHeaders();
    if (numNodes > 5)
    {
        msg.append(" - too many segments to display!");
        blank(4, 0, msg);
    }
    else
    {
        showSliders(numNodes + 4);
        ui->label_5_name->setText("ln of sel at P1");
        ui->label_5_type->setText("Value");
        ui->label_6_name->setText("ln of sel midpt");
        ui->label_6_type->setText("Value");
        ui->label_7_name->setText("ln of sel midpt");
        ui->label_7_type->setText("Value");
        ui->label_8_name->setText("ln of sel midpt");
        ui->label_8_type->setText("Value");
        ui->label_9_type->setText("Value");
        setParameterHeaders();
        if (numNodes == 2)
        {
            ui->label_6_name->setText("ln of sel at P2");
        }
        else if (numNodes == 3)
        {
            ui->label_7_name->setText("ln of sel at P2");
        }
        else if (numNodes == 4)
        {
            ui->label_8_name->setText("ln of sel at P2");
        }
        else if (numNodes == 5)
        {
            ui->label_9_name->setText("ln of sel at P2");
        }
        showBins(true);
        showJoins(0);

//        resetChart();

        axisY->setTitleText("Selex");
        cht->addAxis(axisY, Qt::AlignLeft);
        cht->addAxis(axisXsel, Qt::AlignBottom);

        ascendSeries = new QLineSeries(cht);
        ascendSeries->setPen(QPen(QBrush(Qt::blue), 2));
        ascendSeries->setName(QString("Ln of Selex"));
        cht->addSeries(ascendSeries);

        selSeries->setPen(QPen(QBrush(Qt::red), 3));
        cht->addSeries(selSeries);
        selSeries->attachAxis(axisXsel);
        selSeries->attachAxis(axisY);

        axisYalt->setTitleText("Ln of Selex");

        cht->addAxis(axisYalt, Qt::AlignRight);
        ascendSeries->attachAxis(axisXsel);
        ascendSeries->attachAxis(axisYalt);
        cht->legend()->show();
        cht->legend()->setAlignment(Qt::AlignLeft);
        updateLinearScaled();
    }
}

// size selectivity 6
void equationDialog::updateLinear(float scale)
{
    float y = 0;
    int axisYMax = 0;
    int axisYMin = 0;
    float temp = 0;
    float len = 0;
    int numNodes = getSpecial();
    float xVal[10];
    float yVal[10];
    int num = parameters->rowCount() - 2;

    if (scale > 0)
    {
        updateLinearScaled();
        return;
    }

    ascendSeries->clear();

    if (num < 2 || numNodes != num)
    {
        QMessageBox::information(this, tr("Parameter error"), tr("Incorrect number of parameters. \nShould be Special +2"));
        return;
    }
    if (numNodes > 7)
    {
        QString msg(QString("Pattern %1: %2 Linear segments - too many segments to display!").arg(
                        QString::number(equationNum),
                        QString::number(numNodes)));
        blank(2, 0, msg);
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

    xVal[numNodes] = ui->doubleSpinBox_2_value->value();
    ui->doubleSpinBox_2_trans->setValue(xVal[numNodes]);
    xVal[numNodes + 1] = xValList.last();
    yVal[numNodes + 1] = yVal[numNodes];

    temp = (xVal[numNodes] - xVal[1]) / (numNodes - 1);
    for (int i = 2; i < numNodes; i++)
    {
        xVal[i] = xVal[1] + (i - 1) * temp;
    }

    for (int i = 0; i <= numNodes; i++)
        ascendSeries->append(QPointF(xVal[i], yVal[i]));
    if (xVal[numNodes] > binMax)
        temp = xVal[numNodes] + 2;
    else
        temp = binMax;
    ascendSeries->append(QPointF(temp, yVal[numNodes]));

    firstPoints.clear();
    yMax = yVal[0];
    for (int i = 0; i < xValList.count(); i++)
    {
        len = xValList.at(i);
        for (int j = 1; j <= numNodes; j++)
        {
            if (len >= xVal[j -1] && len < xVal[j])
            {
                y = evaluateLine(QPointF(xVal[j-1], yVal[j-1]), QPointF(xVal[j], yVal[j]), len);
                if (y > yMax)
                    yMax = y;
                if (y < axisYMin)
                    axisYMin = abs(y) - 2;
                axisYMax = abs(yMax) + 1;
                firstPoints.append(QPointF(len, y));
                break;
            }
        }
        if (len >= xVal[numNodes])
        {
            y = yVal[numNodes];
            firstPoints.append(QPointF(len, y));
        }
    }
    axisYalt->setRange(axisYMin, axisYMax);
    updateLinearExp(scale);
}

// case 21 non-parametric size selectivity
void equationDialog::updateLinearPlain()
{
    int i = 0;
    float xVal;
    float yVal;
    selSeries->clear();
    xVal = QString(parameters->getRowData(0).at(2)).toFloat();
    while (xVal > xValList.at(i))
    {
        selSeries->append(QPointF(xValList.at(i++), 0.0));
    }
    for (i = 0; i < special; i++)
    {
        xVal = QString(parameters->getRowData(i).at(2)).toFloat();
        yVal = QString(parameters->getRowData(i + special).at(2)).toFloat();
        selSeries->append(QPointF(xVal, yVal));
    }
    if (xVal < xValList.last())
        selSeries->append(QPointF(xValList.last(), yVal));
}

// age selectivity 43
void equationDialog::updateLinearScaled()
{
    float y = 0;
    int axisYMax = 0;
    int axisYMin = 0;
    QString msg;
    float temp = 0;
    float len = 0;
    int numNodes = getSpecial();
    float xVal[10];
    float yVal[10];
    float xMin = ui->doubleSpinBox_1_value->value();
    float xMax = ui->doubleSpinBox_2_value->value();
    int num = parameters->rowCount() - 4;

    if (num < 2 || numNodes != num)
    {
        QMessageBox::information(this, tr("Parameter error"), tr("Incorrect number of parameters. \nShould be Special +4"));
        return;
    }

    if (xMin < 1)
    {
        xMin = 1;
//        msg = QString("Minimum for param 1 is first bin. adjusting ...");
//        QMessageBox::information(this, tr("Error in Value"), tr(msg.toUtf8()));
        value1Changed(xMin);
        return;
    }
    if (xMax < xMin)
    {
        xMax = xMin + 1.0;
//        msg = QString("Maximum is greater than Minimum. adjusting ...");
//        QMessageBox::information(this, tr("Error in Value"), tr(msg.toUtf8()));
        value2Changed(xMax);
        return;
    }
    if (xMax > xValList.count())
    {
        xMax = xValList.count();
//        msg = QString("Maximum for param 2 is last bin. adjusting ...");
//        QMessageBox::information(this, tr("Error in Value"), tr(msg.toUtf8()));
        value2Changed(xMax);
        return;
    }
    if (xMax < xMin)
    {
        xMin = xMax;
//        msg = QString("Minimum is greater than Maximum. adjusting ...");
//        QMessageBox::information(this, tr("Error in Value"), tr(msg.toUtf8()));
        value1Changed(xMin);
        return;
    }
    ui->doubleSpinBox_1_trans->setValue(xMin);
    ui->doubleSpinBox_2_trans->setValue(xMax);

    ascendSeries->clear();

    if (numNodes > 5)
    {
        QString msg(QString("Pattern %1: %2 Linear segments - too many segments to display!").arg(
                        QString::number(equationNum),
                        QString::number(numNodes)));
        blank(5, 0, msg);
        return;
    }

    for (int i = 0; i < 10; i++)
    {
        xVal[i] = 0.0;
        yVal[i] = 0.0;
    }
    xVal[0] = xValList.first();
    yVal[0] = -10.0;
    xVal[1] = ui->doubleSpinBox_3_value->value();
    ui->doubleSpinBox_1_trans->setValue(xVal[1]);
    yVal[1] = ui->doubleSpinBox_5_value->value();
    ui->doubleSpinBox_3_trans->setValue(yVal[1]);
    yVal[2] = ui->doubleSpinBox_6_value->value();
    ui->doubleSpinBox_4_trans->setValue(yVal[2]);
    yVal[3] = ui->doubleSpinBox_7_value->value();
    ui->doubleSpinBox_5_trans->setValue(yVal[3]);
    yVal[4] = ui->doubleSpinBox_8_value->value();
    ui->doubleSpinBox_6_trans->setValue(yVal[4]);
    yVal[5] = ui->doubleSpinBox_9_value->value();
    ui->doubleSpinBox_7_trans->setValue(yVal[5]);

    xVal[numNodes] = ui->doubleSpinBox_4_value->value();
    ui->doubleSpinBox_4_trans->setValue(xVal[numNodes]);
    xVal[numNodes + 1] = xValList.last();
    yVal[numNodes + 1] = yVal[numNodes];

    temp = (xVal[numNodes] - xVal[1]) / (numNodes - 1);
    for (int i = 2; i < numNodes; i++)
    {
        xVal[i] = xVal[1] + (i - 1) * temp;
    }

    for (int i = 0; i <= numNodes; i++)
        ascendSeries->append(QPointF(xVal[i], yVal[i]));
    if (xVal[numNodes] > binMax)
        temp = xVal[numNodes] + 2;
    else
        temp = binMax;
    ascendSeries->append(QPointF(temp, yVal[numNodes]));

    firstPoints.clear();
    yMax = yVal[0];
    for (int i = 0; i < xValList.count(); i++)
    {
        len = xValList.at(i);
        for (int j = 1; j <= numNodes; j++)
        {
            if (len >= xVal[j -1] && len < xVal[j])
            {
                y = evaluateLine(QPointF(xVal[j-1], yVal[j-1]), QPointF(xVal[j], yVal[j]), len);
                firstPoints.append(QPointF(len, y));
                if (y > yMax)
                    yMax = y;
                if (y < axisYMin)
                    axisYMin = (int)y - 2;
                break;
            }
        }
        if (len >= xVal[numNodes])
        {
            y = yVal[numNodes];
            firstPoints.append(QPointF(len, y));
        }
    }
    axisYMax = (int)yMax + 1;
    axisYalt->setRange(axisYMin, axisYMax);
    updateLinearExp(2);
}

void equationDialog::updateLinearExp(float scale)
{
    float yVal;
    int first = ui->doubleSpinBox_1_trans->value();
    int last = ui->doubleSpinBox_2_trans->value();
    float yTotal = 1;
    float yAve = 1;

    selSeries->clear();

    if (scale == 0)
    {
        for (int i = 0; i < firstPoints.count(); i++)
        {
            yVal = exp(firstPoints.at(i).y() - yMax);
            selSeries->append(firstPoints.at(i).x(), yVal);
        }
    }
    else
    {
        for (int i = 0; i < firstPoints.count(); i++)
        {
            yVal = exp(firstPoints.at(i).y() - yMax);
            firstPoints[i].setY(yVal);
        }
        for (int i = first-1; i < last; i++)
        {
            yTotal += firstPoints.at(i).y();
        }
        yAve = yTotal / (last - first + 1);
        for (int i = 0; i < firstPoints.count(); i++)
        {
            yVal = firstPoints.at(i).y() / yAve;
            selSeries->append(firstPoints.at(i).x(), yVal);
        }
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
// case 9 old double logistic with 4 parameters
// 19 #age selectivity: old double logistic
void equationDialog::dblLogistic()
{
    setTitle(QString("Pattern %1: Simple Double Logistic").arg(
                     QString::number(equationNum)));
    disconnect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));
    max1Changed(xValList.last()); min1Changed(0);
    max2Changed(1); min2Changed(0);
    max3Changed(xValList.last()); min3Changed(xValList.first());
    max4Changed(0); min4Changed(-1);
    max5Changed(xValList.count()); min5Changed(1);
    max6Changed(1); min6Changed(0);
    connect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));
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
    setParameterHeaders();
    showBins(true);
    showJoins(0);

//    resetChart();

    ascendSeries = new QLineSeries(cht);
    ascendSeries->setName(QString("Logist1"));
    ascendSeries->setPen(QPen(QBrush(Qt::green), 2));
    cht->addSeries(ascendSeries);

    dscendSeries = new QLineSeries(cht);
    dscendSeries->setName(QString("Logist2"));
    dscendSeries->setPen(QPen(QBrush(Qt::blue), 2));
    cht->addSeries(dscendSeries);

    selSeries->setPen(QPen(QBrush(Qt::red), 3));
    cht->addSeries(selSeries);

    cht->setAxisX(axisXsel, selSeries);
    cht->setAxisY(axisY, selSeries);
//    cht->addAxis(axisYalt, Qt::AlignRight);

    ascendSeries->attachAxis(axisXsel);
    ascendSeries->attachAxis(axisY);

    dscendSeries->attachAxis(axisXsel);
    dscendSeries->attachAxis(axisY);
//    cht->addAxis(axisYalt, Qt::AlignRight);

    cht->legend()->setVisible(true);
    cht->legend()->setAlignment(Qt::AlignLeft);

    if (parameters->rowCount() == 6)
        updateDblLogistic();
}

void equationDialog::updateDblLogistic()
{
    float maxSel = 0;
    float sel = 0;
    float term1 = 0, term2 = 0;
    float term1log, term2log;
    int limit = (xValList.count() - 1), i = 0, offset;
    float par1 = ui->doubleSpinBox_1_value->value();
    float par2 = ui->doubleSpinBox_2_value->value();
    float par3 = ui->doubleSpinBox_3_value->value();
    float par4 = ui->doubleSpinBox_4_value->value();
    int par5 = static_cast<int>(ui->doubleSpinBox_5_value->value());
    float par6 = (ui->doubleSpinBox_6_value->value());
    if (par1 < 0) par1 = 0;
    if (par2 < 0) par2 = 0;
    if (par3 < xValList.first()) par3 = xValList.first();
    if (par4 < -1) par4 = -1;

    limit = (par5 < 1)? 0: ((par5 < xValList.count())? (par5 - 1): limit);
    offset = (par6 < 0.5) ? 0: 1;
    ui->doubleSpinBox_1_trans->setValue(par1);
    ui->doubleSpinBox_2_trans->setValue(par2);
    ui->doubleSpinBox_3_trans->setValue(par3);
    ui->doubleSpinBox_4_trans->setValue(par4);
    ui->doubleSpinBox_5_trans->setValue(limit);
    ui->doubleSpinBox_6_trans->setValue(offset);

    ascendSeries->clear();
    dscendSeries->clear();
    selSeries->clear();
    firstPoints.clear();

    if (xValList.count() > 0)
    {
    for (i = 0; i < limit; i++)
    {
        term1 = par2 * (xValList.at(i) - par1);
        term2 = par4 * (xValList.at(i) - (par1 * offset + par3));
        term1log = logist(term1);
        term2log = logist(term2);
        ascendSeries->append(QPointF(xValList.at(i), term1log));
        dscendSeries->append(QPointF(xValList.at(i), term2log));
        firstPoints.append(QPointF(xValList.at(i), 0.0));
    }
    for (i = limit; i < xValList.count(); i++)
    {
        term1 = par2 * (xValList.at(i) - par1);
        term2 = par4 * (xValList.at(i) - (par1 * offset + par3));
        term1log = logist(term1);
        term2log = logist(term2);
        ascendSeries->append(QPointF(xValList.at(i), term1log));
        dscendSeries->append(QPointF(xValList.at(i), term2log));
        sel = term1log * term2log; //logist(term1) * logist(term2);
        firstPoints.append(QPointF(xValList.at(i), sel));
    }
    maxSel = maxYvalue(firstPoints);
    for (i = 0; i < firstPoints.count(); i++)
    {
        sel = (firstPoints.at(i).y() + .000001) / maxSel;
        selSeries->append(QPointF(xValList.at(i), sel));
    }
    maxSel = maxYvalue(dscendSeries->points());
    axisYalt->setRange(0, (maxSel * 1.2));
    }
}

// case 8 double logistic with eight parameters
// 13 #age selectivity - double logistic
// 18 #age selectivity: double logistic with smooth transition
// 1=peak, 2=init,  3=infl,  4=slope, 5=final, 6=infl2, 7=slope2
// 1=peak, 2=init,  3=infl,  4=slope, 5=final, 6=infl2, 7=slope2 8=binwidth
void equationDialog::dblLogPeak()
{
    setTitle(QString("Pattern %1: Double Logistic with IF joins ").arg(
                 QString::number(equationNum)));
    disconnect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));
    max1Changed(xValList.last()); min1Changed(xValList.first());
    max2Changed(1); min2Changed(0);
    max3Changed(5); min3Changed(-5);
    max4Changed(3); min4Changed(-5);
    max5Changed(9); min5Changed(-5);
    max6Changed(9); min6Changed(-5);
    max7Changed(3); min7Changed(-5);
    max8Changed(20); min8Changed(1);
    connect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));
    if (equationNum == 13)
        ui->label_title->setText(QString("This pattern is discouraged, use pattern #18"));
    showSliders(8);
    ui->label_1_name->setText("Peak");
    ui->label_1_type->setText("Value");
    ui->label_2_name->setText("Init");
    ui->label_2_type->setText("Value");
    ui->label_3_name->setText("Infl-Up");
    ui->label_3_type->setText("Logit");
    ui->label_4_name->setText("Slope-Up");
    ui->label_4_type->setText("Value");
    ui->label_5_name->setText("Final");
    ui->label_5_type->setText("Logit");
    ui->label_6_name->setText("Infl-Dn");
    ui->label_6_type->setText("Logit");
    ui->label_7_name->setText("Slope-Dn");
    ui->label_7_type->setText("Value");
    ui->label_8_name->setText("BinWidth");
    ui->label_8_type->setText("Value");
    setParameterHeaders();

    showBins(true);
    showJoins(3);

//    resetChart();

    join1Series = new QLineSeries(cht);
    join1Series->setPen(QPen(QBrush(Qt::darkGreen), 1));
    join1Series->setName(QString("Join1"));
    cht->addSeries(join1Series);

    join2Series = new QLineSeries(cht);
    join2Series->setPen(QPen(QBrush(Qt::darkCyan), 1));
    join2Series->setName(QString("Join2"));
    cht->addSeries(join2Series);

    join3Series = new QLineSeries(cht);
    join3Series->setPen(QPen(QBrush(Qt::darkYellow), 1));
    join3Series->setName(QString("Join3"));
    cht->addSeries(join3Series);

    ascendSeries = new QLineSeries(cht);
    ascendSeries->setPen(QPen(QBrush(Qt::green), 3));
    ascendSeries->setName(QString("Logist1"));
    cht->addSeries(ascendSeries);

    dscendSeries = new QLineSeries(cht);
    dscendSeries->setPen(QPen(QBrush(Qt::blue), 3));
    dscendSeries->setName(QString("Logist2"));
    cht->addSeries(dscendSeries);

    selSeries->setPen(QPen(QBrush(Qt::red), 3));
    cht->addSeries(selSeries);

    cht->setAxisX(axisXsel, selSeries);
    cht->setAxisY(axisY, selSeries);

    ascendSeries->attachAxis(axisXsel);
    ascendSeries->attachAxis(axisY);
    dscendSeries->attachAxis(axisXsel);
    dscendSeries->attachAxis(axisY);
    join1Series->attachAxis(axisXsel);
    join1Series->attachAxis(axisY);
    join2Series->attachAxis(axisXsel);
    join2Series->attachAxis(axisY);
    join3Series->attachAxis(axisXsel);
    join3Series->attachAxis(axisY);

    cht->legend()->setVisible(true);
    cht->legend()->setAlignment(Qt::AlignLeft);

    if (parameters->rowCount() == 8)
    {
        if (equationNum == 8)
            updateDblLogPeak();
        else if (equationNum == 13)
            updateDblLogIf();
        else
            updateDblLogSmooth();
    }
}

// size selectivity 8
void equationDialog::updateDblLogPeak()
{
    float maxSelX = 0;
    float minVal = binMin + (binWidth * binMidWidth);
    float maxVal = binMax + (binWidth * binMidWidth);
    float sel = 0;
    float t1 = 0, t1min, t1max, t1power;
    float t2 = 0, t2min, t2max, t2power;
    float t1range, t2range;
    float one_init, final_one;
    float jn1, jn2, jn3;
    float upselex, downselex;
    int binM, i;
    float par1 = ui->doubleSpinBox_1_value->value();
    float par2 = ui->doubleSpinBox_2_value->value();
    float par3 = ui->doubleSpinBox_3_value->value();
    float par4 = ui->doubleSpinBox_4_value->value();
    float par5 = ui->doubleSpinBox_5_value->value();
    float par6 = ui->doubleSpinBox_6_value->value();
    float par7 = ui->doubleSpinBox_7_value->value();
    float par8 = ui->doubleSpinBox_8_value->value();
    if (par1 < min1) par1 = min1;
    if (par2 < min2) par2 = min2;
    if (par3 < min3) par3 = min3;
    if (par4 < min4) par4 = min4;
    if (par5 < min5) par5 = min5;
    if (par6 < min6) par6 = min6;
    if (par7 < min7) par7 = min7;
    if (par8 < min8) par8 = min8;
    float peak = par1;
    float init = par2;
    float infl_up = logist(par3);
    float slope_up = par4;
    float final = logist(par5);
    float infl_dn = logist(par6);
    float slope_dn = par7;
    float binwid = par8;
    ui->doubleSpinBox_1_trans->setValue(peak);
    ui->doubleSpinBox_2_trans->setValue(init);
    ui->doubleSpinBox_3_trans->setValue(infl_up);
    ui->doubleSpinBox_4_trans->setValue(slope_up);
    ui->doubleSpinBox_5_trans->setValue(final);
    ui->doubleSpinBox_6_trans->setValue(infl_dn);
    ui->doubleSpinBox_7_trans->setValue(slope_dn);
    ui->doubleSpinBox_8_trans->setValue(binwid);

    selSeries->clear();
    firstPoints.clear();
    ascendSeries->clear();
    dscendSeries->clear();
    join1Series->clear();
    join2Series->clear();
    join3Series->clear();
    float peakBwd = peak + binwid;
//    t1=minL+(1./(1.+mfexp(-sp(3))))*(sp(1)-minL);    // INFL
//    t1min=1./(1.+mfexp(-mfexp(sp(4))*(minL-t1)))*0.9999;  // asc value at minsize
//    t1max=1./(1.+mfexp(-mfexp(sp(4))*(sp(1)-t1)))*1.0001;  // asc value at peak
//    t1power=log(0.5)/log((0.5-t1min)/(t1max-t1min));  // so the parameter will actual correspond to 50% point
    t1 = minVal + (infl_up * (peak - minVal));
    t1min = logist (exp(slope_up) * (minVal - t1)) * 0.9999;// asc value at minsize
    t1max = logist (exp(slope_up) * (peak - t1)) * 1.0001;  // asc value at peak
    t1power = log(0.5)/log((0.5 - t1min)/(t1max - t1min));  // so the parameter will actual correspond to 50% point

//    if(seltype(f,4)==0) {sel_maxL=maxL;} else {sel_maxL=Ave_Size(styr,3,1,nages);}
    if (special == 0)
    {
        maxSelX = maxVal;
    }
    else
    {
        maxSelX = aveXvalue(xValList);
    }
//    t2=(sp(1)+sp(8))+(1./(1.+mfexp(-sp(6))))*(sel_maxL-(sp(1)+sp(8)));    // INFL
//    t2min=1./(1.+mfexp(-mfexp(sp(7))*(sp(1)+sp(8)-t2)))*0.9999;  // asc value at peak+
//    t2max=1./(1.+mfexp(-mfexp(sp(7))*(sel_maxL-t2)))*1.0001;  // asc value at maxL
//    t2power=log(0.5)/log((0.5-t2min)/(t2max-t2min));
    t2 = peakBwd + infl_dn * (maxSelX - peakBwd);    // INFL
    t2min = logist(exp(slope_dn) * (peakBwd - t2)) * 0.9999;// asc value at peak+
    t2max = logist(exp(slope_dn) * (maxSelX - t2)) * 1.0001;// asc value at maxL
    t2power = log(0.5) / log((0.5 - t2min)/(t2max - t2min));

/*    for (j=1; j<=nlength; j++)  //calculate the value over length bins
    {   join1=1./(1.+mfexp(10.*(len_bins_m(j)-sp(1))));
        join2=1./(1.+mfexp(10.*(len_bins_m(j)-(sp(1)+sp(8)))));
        join3=1./(1.+mfexp(10.*(len_bins_m(j)-sel_maxL)));
        upselex=sp(2) + (1. - sp(2)) * pow((( 1./(1.+mfexp(-mfexp(sp(4))*(len_bins_m(j)-t1)))-t1min ) / (t1max-t1min)),t1power);
        downselex=(1. + (final - 1.) * pow(fabs(((( 1./(1.+mfexp(-mfexp(sp(7))*(len_bins_m(j)-t2))) -t2min ) / (t2max-t2min) ))),t2power));
        sel(j) = ((((upselex*join1)+1.0*(1.0-join1))*join2) + downselex*(1-join2))*join3 + final*(1-join3);
    }*/
    one_init = 1.0 - init;
    final_one = final - 1.0;
    t1range = t1max - t1min;
    t2range = t2max - t2min;

    for (i = 0; i < xValList.count(); i++)
    {
        binM = xValList.at(i) + binWidth * binMidWidth;
        jn1 = logist(-join1 * (binM - peak));
        jn2 = logist(-join2 * (binM - peakBwd));
        jn3 = logist(-join3 * (binM - maxVal));
//        jn1 = 1.0 / (1.0 + exp(10.0 * (binM - peak)));
//        jn2 = 1.0 / (1.0 + exp(10.0 * (binM - peakBwd)));
//        jn3 = 1.0 / (1.0 + exp(10.0 * (binM - maxSelX)));
        upselex = init + (one_init) * pow(((logist(exp(slope_up) * (binM - t1)) - t1min) / (t1range)), t1power);
        downselex = (1.0 + (final_one) * pow(fabs((((logist(exp(slope_dn) * (binM - t2)) -t2min) / (t2range)))), t2power));
//        upselex = init + (1.0 - init) * pow((( 1.0 / (1.0 + exp(-exp(slope_up) * (binM - t1))) - t1min) / (t1max - t1min)), t1power);
//        downselex = (1.0 + (final - 1.0) * pow(abs((((1.0 / (1.0 + exp(-exp(slope_dn) * (binM - t2))) -t2min) / (t2max - t2min)))), t2power));
        sel = ((((upselex * jn1) + (1.0 - jn1)) * jn2) + downselex * (1.0 - jn2)) * jn3 + final * (1.0 - jn3);
//        sel = ((upselex * jn1) + (jn2 - jn1) + (downselex * (jn3 - jn2)) + (final * (1.0 - jn3)));
        join1Series->append(QPointF(binM, jn1));
        join2Series->append(QPointF(binM, jn2));
        join3Series->append(QPointF(binM, jn3));
        ascendSeries->append(QPointF(binM, upselex));
        dscendSeries->append(QPointF(binM, downselex));
        selSeries->append(QPointF(binM, sel));
    }
//    ui->spinBox_steep_join1->setValue(10);
//    ui->spinBox_steep_join2->setValue(10);
//    ui->spinBox_steep_join3->setValue(10);
}

// 13 #age selectivity - double logistic
void equationDialog::updateDblLogIf()
{
    float maxSelX = 0;
    float minX = xValList.first();
    float maxX = xValList.last();
    float sel = 0;
    float t1 = 0, t1min, t1max, t1power;
    float t2 = 0, t2min, t2max, t2power;
    float jn1, jn2;
    float upsel, dnsel;
    int binM, i;
    float par1 = ui->doubleSpinBox_1_value->value();
    float par2 = ui->doubleSpinBox_2_value->value();
    float par3 = ui->doubleSpinBox_3_value->value();
    float par4 = ui->doubleSpinBox_4_value->value();
    float par5 = ui->doubleSpinBox_5_value->value();
    float par6 = ui->doubleSpinBox_6_value->value();
    float par7 = ui->doubleSpinBox_7_value->value();
    float par8 = ui->doubleSpinBox_8_value->value();
    if (par1 < min1) par1 = min1;
    if (par2 < min2) par2 = min2;
    if (par3 < min3) par3 = min3;
    if (par4 < min4) par4 = min4;
    if (par5 < min5) par5 = min5;
    if (par6 < min6) par6 = min6;
    if (par7 < min7) par7 = min7;
    if (par8 < min8) par8 = min8;
    float peak = par1;
    float init = par2;
    float infl_up = logist(par3);
    float slope_up = par4;
    float final = logist(par5);
    float infl_dn = logist(par6);
    float slope_dn = par7;
    float binwid = par8;
    ui->doubleSpinBox_1_trans->setValue(peak);
    ui->doubleSpinBox_2_trans->setValue(init);
    ui->doubleSpinBox_3_trans->setValue(infl_up);
    ui->doubleSpinBox_4_trans->setValue(slope_up);
    ui->doubleSpinBox_5_trans->setValue(final);
    ui->doubleSpinBox_6_trans->setValue(infl_dn);
    ui->doubleSpinBox_7_trans->setValue(slope_dn);
    ui->doubleSpinBox_8_trans->setValue(binwid);

    selSeries->clear();
    firstPoints.clear();
    ascendSeries->clear();
    dscendSeries->clear();
    join1Series->clear();
    join2Series->clear();
    join3Series->clear();

    float peakBwd = peak + binwid;

}

void equationDialog::updateDblLogSmooth()
{
    float maxSelX = 0;
    float minX = xValList.first();
    float maxX = xValList.last();
    float sel = 0;
    float t1 = 0, t1min, t1max, t1power;
    float t2 = 0, t2min, t2max, t2power;
    float jn1, jn2;
    float upsel, dnsel;
    int binM, i;
    float par1 = ui->doubleSpinBox_1_value->value();
    float par2 = ui->doubleSpinBox_2_value->value();
    float par3 = ui->doubleSpinBox_3_value->value();
    float par4 = ui->doubleSpinBox_4_value->value();
    float par5 = ui->doubleSpinBox_5_value->value();
    float par6 = ui->doubleSpinBox_6_value->value();
    float par7 = ui->doubleSpinBox_7_value->value();
    float par8 = ui->doubleSpinBox_8_value->value();
    if (par1 < min1) par1 = min1;
    if (par2 < min2) par2 = min2;
    if (par3 < min3) par3 = min3;
    if (par4 < min4) par4 = min4;
    if (par5 < min5) par5 = min5;
    if (par6 < min6) par6 = min6;
    if (par7 < min7) par7 = min7;
    if (par8 < min8) par8 = min8;
    float peak = par1;
    float init = par2;
    float infl_up = logist(par3);
    float slope_up = par4;
    float final = logist(par5);
    float infl_dn = logist(par6);
    float slope_dn = par7;
    float binwid = par8;
    ui->doubleSpinBox_1_trans->setValue(peak);
    ui->doubleSpinBox_2_trans->setValue(init);
    ui->doubleSpinBox_3_trans->setValue(infl_up);
    ui->doubleSpinBox_4_trans->setValue(slope_up);
    ui->doubleSpinBox_5_trans->setValue(final);
    ui->doubleSpinBox_6_trans->setValue(infl_dn);
    ui->doubleSpinBox_7_trans->setValue(slope_dn);
    ui->doubleSpinBox_8_trans->setValue(binwid);

    selSeries->clear();
    firstPoints.clear();
    ascendSeries->clear();
    dscendSeries->clear();
    join1Series->clear();
    join2Series->clear();
    join3Series->clear();

    float peakBwd = peak + binwid;
//    t1=0.+(1./(1.+mfexp(-sp(3))))*(sp(1)-0.);    // INFL
//    t1min=1./(1.+mfexp(-sp(4)*(0.-t1)))*0.9999;  // asc value at minsize
//    t1max=1./(1.+mfexp(-sp(4)*(sp(1)-t1)))*1.00001;  // asc value at peak
//    t1power=log(0.5)/log((0.5-t1min)/(t1max-t1min));
    t1 = 0.0 + (infl_up * (peak - 0.0));
    t1min = logist(exp(slope_up) * (0.0 - t1)) * 0.9999;
    t1max = logist(exp(slope_up) * (peak - t1)) * 1.0001;
//    t1min = 1.0/(1.0 + exp(-exp(par4) * (0.0 - t1))) * 0.9999;
//    t1max = 1.0/(1.0 + exp(-exp(par4) * (par1 - t1))) * 1.0001;
    t1power = log(0.5)/log((0.5 - t1min)/(t1max - t1min));


//    t2=(sp(1)+sp(8))+(1./(1.+mfexp(-sp(6))))*(r_ages(nages)-(sp(1)+sp(8)));    // INFL
//    t2min=1./(1.+mfexp(-sp(7)*(sp(1)+sp(8)-t2)))*0.9999;  // asc value at peak+
//    t2max=1./(1.+mfexp(-sp(7)*(r_ages(nages)-t2)))*1.00001;  // asc value at maxage
//    t2power=log(0.5)/log((0.5-t2min)/(t2max-t2min));
    t2 = (peakBwd) + infl_dn * (binMax - (peakBwd));
    t2min = logist(exp(slope_dn) * (peakBwd - t2)) * 0.9999;
    t2max = logist(exp(slope_dn) * (binMax - t2)) * 1.0001;
 //   t2min = 1./(1. + exp(-exp(par7) * (peak + par8 - t2))) * 0.9999;
//    t2max = 1./(1. + exp(-exp(par7) * (binMax - t2))) * 1.0001;
    t2power = log(0.5) / log((0.5 - t2min)/(t2max - t2min));
//    final=1./(1.+mfexp(-sp(5)));
//    final = logist(par5); // done above

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
        jn1 = logist (-join1 * (binM - peak));
        jn2 = logist (-join2 * (binM - peakBwd));
//        jn1 = 1.0 / (1.0 + exp(30.0 * (binM - peak)));
  //      jn2 = 1.0 / (1.0 + exp(30.0 * (binM - peakBwd)));
        upsel = init + (1.0 - init) * pow((( 1.0 / (1.0 + exp(-slope_up * (binM - t1))) - t1min) / (t1max - t1min)), t1power);
        dnsel = (1.0 + (final - 1.0) * pow(abs((((1.0 / (1.0 + exp(-slope_dn * (binM - t2))) -t2min) / (t2max - t2min)))), t2power));
        join1Series->append(QPoint(binM, jn1));
        join2Series->append(QPointF(binM, jn2));
//        join3Series->append(QPointF(binM, jn3));
        ascendSeries->append(QPointF(binM, upsel));
        dscendSeries->append(QPointF(binM, dnsel));
        sel = ((((upsel * jn1) + jn1) * jn2) + dnsel * jn2);
        sel = (upsel * jn1) + (jn2 - jn1) + (dnsel * (1.0 - jn2));
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
    setTitle(QString("Pattern %1: Double Normal (like CASAL)").arg(
                 QString::number(equationNum)));
    disconnect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));
    max1Changed(xValList.last()); min1Changed(xValList.first());
    max2Changed(1); min2Changed(0);
    max3Changed(10); min3Changed(0);
    max4Changed(10); min4Changed(0);
    connect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));
    showSliders(4);
    ui->label_1_name->setText("Peak");
    ui->label_1_type->setText("Value");
    ui->label_2_name->setText("Top");
    ui->label_2_type->setText("Log");
    ui->label_3_name->setText("Asc-width");
    ui->label_3_type->setText("Exp");
    ui->label_4_name->setText("Dsc-width");
    ui->label_4_type->setText("Exp");
    setParameterHeaders();

    showBins(true);
    showJoins(0);

//    resetChart();

    ascendSeries = new QLineSeries(cht);
    ascendSeries->setPen(QPen(QBrush(Qt::green), 2));
    ascendSeries->setName(QString("Ascend"));
    cht->addSeries(ascendSeries);
    ascendSeries->attachAxis(axisXsel);

    dscendSeries = new QLineSeries(cht);
    dscendSeries->setPen(QPen(QBrush(Qt::blue), 2));
    dscendSeries->setName(QString("Descend"));
    cht->addSeries(dscendSeries);
    dscendSeries->attachAxis(axisXsel);

    selSeries->setPen(QPen(QBrush(Qt::red), 3));
    cht->addSeries(selSeries);

    cht->setAxisX(axisXsel, selSeries);
    cht->setAxisY(axisY, selSeries);

    ascendSeries->attachAxis(axisXsel);
    ascendSeries->attachAxis(axisY);
    dscendSeries->attachAxis(axisXsel);
    dscendSeries->attachAxis(axisY);

    cht->legend()->setVisible(true);
    cht->legend()->setAlignment(Qt::AlignLeft);

    if (parameters->rowCount() == 4)
        updateDblNormCasal();
}

void equationDialog::updateDblNormCasal()
{
    float xval, sel, peak2;
    float par1 = ui->doubleSpinBox_1_value->value();
    float par2 = ui->doubleSpinBox_2_value->value();
    float par3 = ui->doubleSpinBox_3_value->value();
    float par4 = ui->doubleSpinBox_4_value->value();
    if (par1 < min1) par1 = min1;
    if (par2 < min2) par2 = min2;
    if (par3 < min3) par3 = min3;
    if (par4 < min4) par4 = min4;
    float peak = par1;
    float top = exp(-par2);
    float upsel = exp(par3);
    float dnsel = exp(par4);
    ui->doubleSpinBox_1_trans->setValue(peak);
    ui->doubleSpinBox_2_trans->setValue(top);
    ui->doubleSpinBox_3_trans->setValue(upsel);
    ui->doubleSpinBox_4_trans->setValue(dnsel);

    peak2 = peak + (0.99 * xValList.last() - peak) / (1. + top);

    selSeries->clear();
    ascendSeries->clear();
    dscendSeries->clear();
    for (int i = 0; i < xValList.count(); i++)
    {
        xval = xValList.at(i);
        sel = exp(-pow((xval - peak),2)/upsel);
        ascendSeries->append(QPointF(xval, sel));

        sel = exp(-pow((xval - peak2),2)/dnsel);
        dscendSeries->append(QPointF(xval, sel));
    }
    for (int i = 0; i < xValList.count(); i++)
    {
        xval = xValList.at(i);
        if (xval < peak)
        {
            sel = ascendSeries->points().at(i).y();
//            sel = exp(-pow((xval - peak),2)/upsel);
        }
        else if (xval <= peak2)
        {
            sel = 1.0;
        }
        else
        {
            sel = dscendSeries->points().at(i).y();
//            sel = exp(-pow((xval - peak2),2)/dnsel);
        }
        selSeries->append(QPointF(xval, sel));
    }
}

void equationDialog::dblNormal()
{
    setTitle(QString("Pattern %1: Double Normal").arg(
                 QString::number(equationNum)));
    disconnect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));
    max1Changed(xValList.last()); min1Changed(xValList.first());
    max2Changed(1); min2Changed(0);
    max3Changed(10); min3Changed(0);
    max4Changed(10); min4Changed(0);
    connect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));
    showSliders(4);
    ui->label_1_name->setText("Peak");
    ui->label_1_type->setText("Value");
    ui->label_2_name->setText("Top");
    ui->label_2_type->setText("Peak+Log");
    ui->label_3_name->setText("Asc-width");
    ui->label_3_type->setText("Exp");
    ui->label_4_name->setText("Dsc-width");
    ui->label_4_type->setText("Exp");
    setParameterHeaders();

    showBins(true);
    showJoins(2);

//    resetChart();

    join1Series = new QLineSeries(cht);
    join1Series->setPen(QPen(QBrush(Qt::darkYellow), 2));
    join1Series->setName(QString("Join1"));
    cht->addSeries(join1Series);

    join2Series = new QLineSeries(cht);
    join2Series->setPen(QPen(QBrush(Qt::yellow), 2));
    join2Series->setName(QString("Join2"));
    cht->addSeries(join2Series);

    ascendSeries = new QLineSeries(cht);
    ascendSeries->setPen(QPen(QBrush(Qt::green), 3));
    ascendSeries->setName(QString("Asc-Norm"));
    cht->addSeries(ascendSeries);

    dscendSeries = new QLineSeries(cht);
    dscendSeries->setPen(QPen(QBrush(Qt::blue), 3));
    dscendSeries->setName(QString("Desc-Norm"));
    cht->addSeries(dscendSeries);

    selSeries->setPen(QPen(QBrush(Qt::red), 3));
    cht->addSeries(selSeries);

    cht->setAxisX(axisXsel, selSeries);
    cht->setAxisY(axisY, selSeries);

    ascendSeries->attachAxis(axisXsel);
    ascendSeries->attachAxis(axisY);
    dscendSeries->attachAxis(axisXsel);
    dscendSeries->attachAxis(axisY);
    join1Series->attachAxis(axisXsel);
    join1Series->attachAxis(axisY);
    join2Series->attachAxis(axisXsel);
    join2Series->attachAxis(axisY);

    cht->legend()->setVisible(true);
    cht->legend()->setAlignment(Qt::AlignLeft);

    if (parameters->rowCount() == 4)
        updateDblNormal();
}

void equationDialog::updateDblNormal()
{
    float xval, sel, peak2;
    float par1 = ui->doubleSpinBox_1_value->value();
    float par2 = ui->doubleSpinBox_2_value->value();
    float par3 = ui->doubleSpinBox_3_value->value();
    float par4 = ui->doubleSpinBox_4_value->value();
    if (par1 < min1) par1 = min1;
    if (par2 < min2) par2 = min2;
    if (par3 < min3) par3 = min3;
    if (par4 < min4) par4 = min4;
    float peak = par1;
    float top = exp(-par2);
    float upsel = exp(par3);
    float dnsel = exp(par4);
}

void equationDialog::dblNormPlateau()
{
    setTitle(QString("Pattern %1: Double Normal plateau").arg(
                 QString::number(equationNum)));
    disconnect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));
    max1Changed(xValList.last()); min1Changed(xValList.first());
    max2Changed(1); min2Changed(0);
    max3Changed(10); min3Changed(0);
    max4Changed(10); min4Changed(0);
    max5Changed(100); min5Changed(0);
    showSliders(6);
    ui->label_1_name->setText("Peak");
    ui->label_1_type->setText("Value");
    ui->label_2_name->setText("Top");
    ui->label_2_type->setText("Peak+Log");
    ui->label_3_name->setText("Asc-width");
    ui->label_3_type->setText("Exp");
    ui->label_4_name->setText("Dsc-width");
    ui->label_4_type->setText("Exp");
    ui->label_5_name->setText("Initial");
    ui->label_5_type->setText("Log");
    ui->label_6_name->setText("Final");
    if(equationNum == 23)
    {
        max6Changed(1); min6Changed(0);
        ui->label_6_type->setText("Value");
    }
    else
    {
        max6Changed(100); min6Changed(0);
        ui->label_6_type->setText("Log");
    }
    connect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));
    setParameterHeaders();

    showBins(true);
    showJoins(2);

//    resetChart();

    updateDblNormPlateau();
}

void equationDialog::updateDblNormPlateau()
{
    float par1 = ui->doubleSpinBox_1_value->value();
    float par2 = ui->doubleSpinBox_2_value->value();
    float par3 = ui->doubleSpinBox_3_value->value();
    float par4 = ui->doubleSpinBox_4_value->value();
    float par5 = ui->doubleSpinBox_5_value->value();
    float par6 = ui->doubleSpinBox_6_value->value();
    if (par1 < min1) par1 = min1;
    if (par2 < min2) par2 = min2;
    if (par3 < min3) par3 = min3;
    if (par4 < min4) par4 = min4;
    if (par5 < min5) par5 = min5;
    if (par6 < min6) par6 = min6;

    float peak = par1;
    float peak2 = peak+binWidth + (.99*binMax - peak - binWidth) * logist(par2);
    float upsel = exp(par3);
    float dnsel = exp(par4);
    float init = 0;//logist(par5);
    float final = (par6);
    float t1, t1min, t1term;
    float t2, t2min, t2term;
    float apical_selex = 1.0;
    int first, last;
    float jn1, jn2;
    int i;
    float xval = 0, asc = 0, des = 0, sel = 0;

    ui->doubleSpinBox_1_trans->setValue(peak);
    ui->doubleSpinBox_2_trans->setValue(peak2);
    ui->doubleSpinBox_3_trans->setValue(upsel);
    ui->doubleSpinBox_4_trans->setValue(dnsel);
    ui->doubleSpinBox_5_trans->setValue(init);
    ui->doubleSpinBox_6_trans->setValue(final);

    firstPoints.clear();
    ascendSeries->clear();
    dscendSeries->clear();
    join1Series->clear();
    join2Series->clear();
    selSeries->clear();

    if (xValList.count() > 0)
    {
        if (par5 < -1000)
        {
            first = -1000 - static_cast<int>(par5);
            init = 0;
            ui->doubleSpinBox_5_trans->setValue(first);
            t1min = 0;
        }
        else if (par5 > -999)
        {
            first = 0;
            init = logist(par5);
            ui->doubleSpinBox_5_trans->setValue(init);
            t1min = exp(-1 * (pow((xValList.at(first)-peak), 2)/upsel));
        }
        if (par6 < -1000)
        {
            last = -1000 - static_cast<int>(par6);
            final = 0;
            ui->doubleSpinBox_6_trans->setValue(last);
            t2min = 0;
        }
        else if (par6 > -999)
        {
            last = xValList.count() - 1;
            final = logist(par6);
            ui->doubleSpinBox_6_trans->setValue(final);
            t2min = exp(-1 * (pow((xValList.at(last) - peak2), 2)/dnsel));
        }
        for (i = 0; i < xValList.count(); i++)
        {
            xval = xValList.at(i);
            t1 = xval - peak;
            t2 = xval - peak2;
            t1term = exp(-t1 * t1 / upsel);
            t2term = exp(-t2 * t2 / dnsel);
            jn1 = logist(join1 * t1 / (1. + fabs(t1)));
            jn2 = logist(join2 * t2 / (1. + fabs(t2)));
            if (par5 > -999)
            {
                asc = init +
                        (apical_selex-init) * (t1term - t1min)
                        / (1.0 - t1min);
            }
            else
            {
                asc = apical_selex * t1term;
            }
            if (par6 > -999)
            {
                des = apical_selex +
                        (final-apical_selex) * (t2term - 1.0)
                        / (t2min - 1.0);
            }
            else
            {
                des = apical_selex * t2term;
            }
            join1Series->append(QPointF(xval, jn1));
            join2Series->append(QPointF(xval, jn2));
            ascendSeries->append(QPointF(xval, asc));
            dscendSeries->append(QPointF(xval, des));
            if (i < first)
                sel = 0.000001;
            else if (i <= last)
                sel = asc * (1.0-jn1) + jn1 * (apical_selex*(1.-jn2) + des * jn2);
            selSeries->append(QPointF(xval, sel));
        }
    }
}

// 20 #age selectivity: double normal with plateau
// case 23 size selectivity double_normal_plateau where final value can be greater than 1.0
// case 24 size selectivity using double_normal_plateau and lots of bells and whistles
// selex 20, 23, 24
void equationDialog::dblNormEndpts()
{
    disconnect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));
    max1Changed(xValList.last()); min1Changed(xValList.first());
    max2Changed(3); min2Changed(-5);
    max3Changed(12); min3Changed(-5);
    max4Changed(10); min4Changed(-2);
    max5Changed(5); min5Changed(-15);
    max6Changed(5); min6Changed(-5);
    showSliders(6);
    ui->label_1_name->setText("Peak");
    ui->label_1_type->setText("Value");
    ui->label_2_name->setText("Top");
    ui->label_2_type->setText("Peak+Log");
    ui->label_3_name->setText("Asc-width");
    ui->label_3_type->setText("Exp");
    ui->label_4_name->setText("Dsc-width");
    ui->label_4_type->setText("Exp");
    ui->label_5_name->setText("Initial");
    ui->label_5_type->setText("Log");
    ui->label_6_name->setText("Final");
    ui->label_6_type->setText("Log");
    setParameterHeaders();
    if(equationNum == 23)
    {
        setTitle(QString("Pattern %1: Double Normal plateau").arg(
                     QString::number(equationNum)));
        ui->label_6_type->setText("Value");
        max6Changed(1); min6Changed(0);
    }
    else
    {
        setTitle(QString("Pattern %1: Double Normal with end-points").arg(
                     QString::number(equationNum)));
        ui->label_6_type->setText("Log");
    }
    ui->spinBox_steep_join1->setValue(20);
    ui->spinBox_steep_join2->setValue(20);

    connect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));
    showBins(true);
    showJoins(2);

//    resetChart();

    join1Series = new QLineSeries(cht);
    join1Series->setPen(QPen(QBrush(Qt::darkYellow), 2));
    join1Series->setName(QString("Join1"));
    cht->addSeries(join1Series);

    join2Series = new QLineSeries(cht);
    join2Series->setPen(QPen(QBrush(Qt::yellow), 2));
    join2Series->setName(QString("Join2"));
    cht->addSeries(join2Series);

    ascendSeries = new QLineSeries(cht);
    ascendSeries->setPen(QPen(QBrush(Qt::green), 3));
    ascendSeries->setName(QString("Asc-Norm"));
    cht->addSeries(ascendSeries);

    dscendSeries = new QLineSeries(cht);
    dscendSeries->setPen(QPen(QBrush(Qt::blue), 3));
    dscendSeries->setName(QString("Desc-Norm"));
    cht->addSeries(dscendSeries);

    selSeries->setPen(QPen(QBrush(Qt::red), 3));
    cht->addSeries(selSeries);

    cht->setAxisX(axisXsel, selSeries);
    cht->setAxisY(axisY, selSeries);

    ascendSeries->attachAxis(axisXsel);
    ascendSeries->attachAxis(axisY);
    dscendSeries->attachAxis(axisXsel);
    dscendSeries->attachAxis(axisY);
    join1Series->attachAxis(axisXsel);
    join1Series->attachAxis(axisY);
    join2Series->attachAxis(axisXsel);
    join2Series->attachAxis(axisY);

    cht->legend()->setVisible(true);
    cht->legend()->setAlignment(Qt::AlignLeft);

    if (parameters->rowCount() == 6)
    {
        if (equationNum == 23)
            updateDblNormPlateau();
        else
            updateDblNormEndpts();
    }
}

void equationDialog::updateDblNormEndpts()
{
    float par1 = ui->doubleSpinBox_1_value->value();
    float par2 = ui->doubleSpinBox_2_value->value();
    float par3 = ui->doubleSpinBox_3_value->value();
    float par4 = ui->doubleSpinBox_4_value->value();
    float par5 = ui->doubleSpinBox_5_value->value();
    float par6 = ui->doubleSpinBox_6_value->value();
    if (par1 < min1) par1 = min1;
    if (par2 < min2) par2 = min2;
    if (par3 < min3) par3 = min3;
    if (par4 < min4) par4 = min4;
    if (par5 < min5) par5 = min5;
    if (par6 < min6) par6 = min6;

    float peak = par1;
    float top = peak+binWidth + (.99*binMax - peak - binWidth) * logist(par2);
    float asc_wd = exp(par3);
    float dsc_wd = exp(par4);
    float init = logist(par5);
    float final = logist(par6);
    float valmin, valmax, valminpow, valmaxpow;
    float limit, upsel, dnsel, jn1, jn2;
    int i;
    float xval = 0, sel = 0;

    ui->doubleSpinBox_1_trans->setValue(peak);
    ui->doubleSpinBox_2_trans->setValue(top);
    ui->doubleSpinBox_3_trans->setValue(asc_wd);
    ui->doubleSpinBox_4_trans->setValue(dsc_wd);
    ui->doubleSpinBox_5_trans->setValue(init);
    ui->doubleSpinBox_6_trans->setValue(final);

    firstPoints.clear();
    ascendSeries->clear();
    dscendSeries->clear();
    join1Series->clear();
    join2Series->clear();
    selSeries->clear();

    if (xValList.count() > 0)
    {
        limit = (par5 < -1000)? (-1000 - par5): -1;
        valmin = binMin + binWidth * binMidWidth;
        valmax = binMax + binWidth * binMidWidth;
        valminpow = pow((valmin - peak), 2);
        valmaxpow = pow((valmax - top), 2);
        valmin = exp(-1 * valminpow/asc_wd);
        valmax = exp(-1 * valmaxpow/dsc_wd);

        for (i = 0; i < xValList.count(); i++)
        {
            xval = xValList.at(i);
            upsel = exp(-1*(pow(xval - peak, 2))/asc_wd);
            if (par5 > -999)
                upsel = init + (1 - init) * (upsel - valmin)/(1-valmin);
            dnsel = exp(-1*(pow(xval - top, 2))/dsc_wd);
            if (par6 > -999)
                dnsel = (1 + (final - 1) * (dnsel - 1)/(valmax - 1));

            jn1 = logist(join1 *(xval-peak)/(1 + fabs(xval-peak)));
            jn2 = logist(join2 *(xval-top)/(1 + fabs(xval-top)));

            if (xval > limit)
                sel = upsel * (1-jn1) + jn1 * ((1-jn2) + (dnsel * jn2));
            else
                sel = 0.000001;

            ascendSeries->append(xval, upsel);
            dscendSeries->append(xval, dnsel);
            join1Series->append(xval, jn1);
            join2Series->append(xval, jn2);
            selSeries->append(xval, sel);
        }
    }
}

// case 25 size selectivity using exponential-logistic
// 26 #age selectivity: exponential logistic
/* peak = len_bins_m(1) + sp(2)*(len_bins_m(nlength)-len_bins_m(1));
   for (j=1;j<=nlength;j++)
   {
     sel(j) = mfexp(sp(3)*sp(1)*(peak-len_bins_m(j)))/
              (1.0-sp(3)*(1.0-mfexp(sp(1)*(peak-len_bins_m(j)))));
   }
*/
void equationDialog::expLogistic()
{
    setTitle(QString("Pattern %1: Exponential-Logistic").arg(
                 QString::number(equationNum)));
    disconnect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));
    max1Changed(2); min1Changed(.02);
    max2Changed(.999); min2Changed(.001);
    max3Changed(.5); min3Changed(.001);
    connect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));
    showSliders(3);
    ui->label_1_name->setText("Width");
    ui->label_1_type->setText("Value");
    ui->label_2_name->setText("Peak");
    ui->label_2_type->setText("Scaled");
    ui->label_3_name->setText("Tail Ht");
    ui->label_3_type->setText("Value");
    setParameterHeaders();
    showBins(true);
    showJoins(0);

//    resetChart();

    selSeries->setPen(QPen(QBrush(Qt::red), 3));
    cht->addSeries(selSeries);
    cht->setAxisX(axisXsel, selSeries);
    cht->setAxisY(axisY, selSeries);

    if (parameters->rowCount() == 3)
        updateExpLogistic();
}

void equationDialog::updateExpLogistic()
{
    int xRange = xValList.last() - xValList.first();
    float yVal = 0;
    float temp = 0;
    float xVal = 0;
    float par1 = ui->doubleSpinBox_1_value->value();
    float par2 = ui->doubleSpinBox_2_value->value();
    float par3 = ui->doubleSpinBox_3_value->value();
    if (par1 < min1) par1 = min1;
    if (par2 < min2) par2 = min2;
    if (par3 < min3) par3 = min3;

    float peak = xValList.first() + par2 * (xRange);
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

// 14 #age selectivity - separate parm for each age
void equationDialog::eachAge ()
{
    setTitle(QString("Pattern %1: Revise Age").arg(
                 QString::number(equationNum)));
/*    binMin = xValList.at(0);
    binMax = binMin + (binWidth * selex->getNumAges());
    ui->spinBox_bins_min->setValue(binMin);
    ui->spinBox_bins_max->setValue(binMax);*/

    showSliders(0);
    showBins(true);
    showJoins(0);
    setParameterHeaders();

//    resetChart();
    connect (parameters, SIGNAL(dataChanged()), this, SLOT(parametersChanged()));

    ascendSeries = new QLineSeries(cht);
    ascendSeries->setPen(QPen(QBrush(Qt::blue), 3));
    cht->addSeries(ascendSeries);

    selSeries->setPen(QPen(QBrush(Qt::red), 3));
    cht->addSeries(selSeries);
    cht->setAxisX(axisXsel, selSeries);
    cht->setAxisY(axisY, selSeries);

    axisYalt->setTitleText("Value at age (blue)");
    cht->addAxis(axisYalt, Qt::AlignRight);
    ascendSeries->attachAxis(axisXsel);
    ascendSeries->attachAxis(axisYalt);

    if (parameters->rowCount() > xValList.count())
        updateEachAge();
}

void equationDialog::updateEachAge ()
{
    float parm = 0.;
    float asc = 0.;
    float sel = 0.;
    float temp, max = 0, min = 0;
    float age = binMin + binMidWidth * binWidth;
    QString msg("");

    selSeries->clear();
    firstPoints.clear();
    if (numParams < xValList.count())
    {
        msg = QString("Fewer parameters than number of ages +1.");
        ui->label_title->setVisible(true);
    }
    else if (numParams > xValList.count())
    {
        msg = QString("More parameters than number of ages +1.");
        ui->label_title->setVisible(true);
    }
    ui->label_title->setText(msg);

    for (int i = 0; i < parameters->rowCount() || i < xValList.count(); i++)
    {
        parm = QString(parameters->getRowData(i).at(2)).toFloat();
        if (parm < -5) parm = -5.;
        if (parm > 9) parm = 9.;
        firstPoints.append(QPointF(age, parm));
        age += binWidth;
    }
    temp = 9. - maxYvalue(firstPoints);
    sel = 0.;

    for (int i = 0; i < firstPoints.count(); i++)
    {
        if (i < firstPoints.count() - 1)
        {
            parm = firstPoints.at(i + 1).y();
        }
        else
        {
            parm = firstPoints.at(i).y();
        }
        if (parm > -999)
        {
            asc = (parm + temp);
            sel = logist(asc);
        }
        ascendSeries->append(QPointF(firstPoints.at(i).x(), asc));
        selSeries->append(QPointF(firstPoints.at(i).x(), sel));
    }
    min = minYvalue(ascendSeries->points());
    max = maxYvalue(ascendSeries->points());
    axisYalt->setRange(min, max);
}

// 17 #age selectivity: each age has parameter as random walk
//    transformation is selex=exp(parm); some special codes
void equationDialog::randomWalk (float scale)
{
    setTitle(QString("Pattern %1: Random Walk").arg(
                 QString::number(equationNum)));

    showSliders(0);
    showBins(true);
    showJoins(0);
    setParameterHeaders();

//    resetChart();
    connect (parameters, SIGNAL(dataChanged()), this, SLOT(parametersChanged()));

    ascendSeries = new QLineSeries(cht);
    ascendSeries->setPen(QPen(QBrush(Qt::blue), 3));
    cht->addSeries(ascendSeries);

    selSeries->setPen(QPen(QBrush(Qt::red), 3));
    cht->addSeries(selSeries);
    cht->setAxisX(axisXsel, selSeries);
    cht->setAxisY(axisY, selSeries);
    axisY->setTitleText("Selex (red)");

    axisYalt->setTitleText("Use Parm (blue)");
    cht->addAxis(axisYalt, Qt::AlignRight);
    ascendSeries->attachAxis(axisXsel);
    ascendSeries->attachAxis(axisYalt);

    if (parameters->rowCount() > xValList.count())
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
//    int age = binMin;
    int lastage = binMax;
    int lastageIndex = xValList.count() - 1;
    int minAge = QString(parameters->getRowData(0).at(2)).toInt();
    int maxAge = QString(parameters->getRowData(1).at(2)).toInt();
    ascendSeries->clear();
    selSeries->clear();
    firstPoints.clear();

    if (special != 0)
        lastage = abs(special);
    for (int i = 0; i < xValList.count(); i++) {
        if (xValList.at(i) > lastage) {
            lastageIndex = i - 1;
            break;
        }
    }

    parm = QString(parameters->getRowData(0).at(2)).toFloat();
    if (parm == -1000)
    {
        parm = 0.0;
    }
    cumparm = useparm = parm;
    if (xValList.count() > 0)
    {
    selSeries->append(QPointF(xValList.at(0), parm));

    parms.append(parm);
    ascendSeries->append(QPointF(xValList.at(0), useparm));
    firstPoints.append(QPointF(xValList.at(0), cumparm));

    for (int i = 1; i <= lastageIndex; i++)
    {
        parm = QString(parameters->getRowData(i).at(2)).toFloat();
        parms.append(parm);
        if (parms[i] > -999)
            useparm = parms[i];
        if (fabs(useparm) > max)
            max = fabs(useparm);
        ascendSeries->append(QPointF(xValList.at(i), useparm));
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
}

// case 16 Coleraine - Gaussian age selectivity
void equationDialog::coleGauss ()
{
    setTitle(QString("Pattern %1: Coleraine single Gaussian").arg(
                 QString::number(equationNum)));

    disconnect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));
    max1Changed(5); min1Changed(-5);
    max2Changed(40); min2Changed(-5);
    connect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));
    showSliders(2);
    ui->label_1_name->setText("Age limit");
    ui->label_1_type->setText("Log");
    ui->label_2_name->setText("Decline scaling");
    ui->label_2_type->setText("Value");
    setParameterHeaders();
    showBins(true);
    showJoins(0);

//    resetChart();

    selSeries->setPen(QPen(QBrush(Qt::red), 3));
    cht->addSeries(selSeries);
    cht->setAxisX(axisXsel, selSeries);
    cht->setAxisY(axisY, selSeries);

    if (parameters->rowCount() == 2)
        updateColeGauss();
}

// Coleraine - Gaussian
/*  t1 = 1/(1+mfexp(-sp(1)))*nages;
    for (a=0;a<=nages;a++)
    {
      if(a<t1)
      {
        sel_a(y,fs,1,a) = mfexp(-square(r_ages(a)-t1)/mfexp(sp(2)));
      }
      else
      {
        sel_a(y,fs,1,a)=1.0;
      }
    }
 */
void equationDialog::updateColeGauss()
{
    int i;
    float temp = 0;
    float sel = 0;
    float par1 = ui->doubleSpinBox_1_value->value();
    float par2 = ui->doubleSpinBox_2_value->value();
    if (par1 < min1) par1 = min1;
    if (par2 < min2) par2 = min2;
    ui->doubleSpinBox_2_trans->setValue(par2);

    selSeries->clear();

    temp = static_cast<int>(logist(par1) * xValList.count());
    ui->doubleSpinBox_1_trans->setValue(xValList.at(temp));

    for (i = 0; i < temp; i++)
    {
        sel = exp(-1*pow(xValList.at(i) - temp, 2) / exp(par2));
        selSeries->append(QPointF(xValList.at(i), sel));
    }
    for (; i < xValList.count(); i++)
    {
        selSeries->append(QPointF(xValList.at(i), 1.0));
    }
}

// case 27 size selectivity using cubic spline
//  first N parameters are the spline knots; second N parameters are ln(selex) at the knot
//  scaled by max(raw vector) to achieve scale to 1.0
// case 42 size and selectivity using cubic spline
//  scaled by average of values at low bin through high bin
void equationDialog::cubicSpline(float scale)
{
    int num = parameters->rowCount();

    if (scale > 0)
    {
        num -= 5;
        showSliders(5);
/*        max1Changed(xValList.count()); min1Changed(2);
        max2Changed(xValList.count()); min2Changed(1);
        max3Changed(2); min3Changed(0);
        max4Changed(1); min4Changed(-.001);
        max5Changed(.001); min5Changed(-1.0);*/
        ui->label_1_name->setText("ScaleLo");
        ui->label_1_type->setText("Value");
        ui->label_2_name->setText("ScaleHi");
        ui->label_2_type->setText("Value");
        ui->label_3_name->setText("Setup");
        ui->label_3_type->setText("Value");
        ui->label_4_name->setText("GradLo");
        ui->label_4_type->setText("Value");
        ui->label_5_name->setText("GradHi");
        ui->label_5_type->setText("Value");
    }
    else
    {
        num -= 3;
        showSliders(3);
/*        max1Changed(2); min1Changed(0);
        max2Changed(1); min2Changed(-.001);
        max3Changed(.001); min3Changed(-1.0);*/
        ui->label_1_name->setText("Setup");
        ui->label_1_type->setText("Value");
        ui->label_2_name->setText("GradLo");
        ui->label_2_type->setText("Value");
        ui->label_3_name->setText("GradHi");
        ui->label_3_type->setText("Value");
    }

    setTitle(QString("Pattern %1: Cubic Spline %2 nodes").arg(
                 QString::number(equationNum),
                 QString::number(num/2)));
    setParameterHeaders();
    showBins(true);
    showJoins(0);

 //   resetChart();
    connect (parameters, SIGNAL(dataChanged()), this, SLOT(parametersChanged()));

    selSeries->setPen(QPen(QBrush(Qt::red), 3));
    cht->addSeries(selSeries);
    cht->setAxisX(axisXsel, selSeries);
    cht->setAxisY(axisY, selSeries);
    axisY->setTitleText("Selex (red)");

    ptSeries = new QScatterSeries(cht);
    ptSeries->setPen(QPen(QBrush(Qt::blue), 3));
    cht->addSeries(ptSeries);

    axisYalt->setTitleText("Cubic Nodes (blue)");
    cht->addAxis(axisYalt, Qt::AlignRight);
    ptSeries->attachAxis(axisXsel);
    ptSeries->attachAxis(axisYalt);

    if (num > 5 && (num % 2) == 0)
    {
        updateCubicSpline(scale);
    }
}

void equationDialog::updateCubicSpline(float scale)
{
    float xval = 0, yval = 0;
    int start = 3 + scale;
    int num = parameters->rowCount() - start;
    int scaleLo = -1, scaleHi = -1;
    int setup = 0;
    float gradLo = 0;
    float gradHi = 0;
    float divisor = 1;
    int i;
    bool okay;
    QString msg;
    float maxVal, minVal, max, min;

    if ((num % 2) == 1 || num < 6)
    {
        msg = QString(tr("Incorrect number of parameters."));
        ui->label_title->setText(msg);
    }
    else
    {
        num /= 2;
        std::vector<double> X(num), Y(num);
        tk::spline cubicspl;

        if (scale < 2)
        {
            setup = ui->doubleSpinBox_1_value->value();
            gradLo = ui->doubleSpinBox_2_value->value();
            gradHi = ui->doubleSpinBox_3_value->value();
            ui->doubleSpinBox_1_trans->setValue(setup);
            ui->doubleSpinBox_2_trans->setValue(gradLo);
            ui->doubleSpinBox_3_trans->setValue(gradHi);
        }
        else
        {
            start = 5;
            scaleLo = ui->doubleSpinBox_1_value->value();
            scaleHi = ui->doubleSpinBox_2_value->value();
            setup = ui->doubleSpinBox_3_value->value();
            gradLo = ui->doubleSpinBox_4_value->value();
            gradHi = ui->doubleSpinBox_5_value->value();
            ui->doubleSpinBox_1_trans->setValue(scaleLo);
            ui->doubleSpinBox_2_trans->setValue(scaleHi);
            ui->doubleSpinBox_3_trans->setValue(setup);
            ui->doubleSpinBox_4_trans->setValue(gradLo);
            ui->doubleSpinBox_5_trans->setValue(gradHi);
        }

        switch (setup)
        {
        case 0:
            for (i = 0; i < num; i++)
                X[i] = QString(parameters->getRowData(start++).at(2)).toFloat();
            break;
        case 1:
        case 2:
            int x0 = xValList.first();
            int xn = xValList.last();
            int num_1 = num - 1;
            float dist = xn - x0;
            float incr = dist / (num_1);
            X[0]     = x0 + (2.5 * dist) / 100;
            X[num_1] = x0 + (97.5 * dist) / 100;
            for (i = 1; i < num_1; i++)
                X[i] = X[i-1] + incr;
            for (i = 0; i < num; i++)
                parameters->setItem(start++, 2, new QStandardItem(QString::number(X[i])));
        }

        for (i = 0; i < num; i++)
        {
            Y[i] = QString(parameters->getRowData(start++).at(2)).toFloat();
        }

        i = num;
        while (i > 1)
        {
            i--;
            for (int j = 0; j < i; j++)
            {
                float x, y;
                if (X[j+1] < X[j])
                {
                    x = X[j];
                    y = Y[j];
                    X[j] = X[j+1];
                    Y[j] = Y[j+1];
                    X[j+1] = x;
                    Y[j+1] = y;
                }
            }
        }
        // currently it is required that X is already sorted
        cubicspl.set_boundary(tk::spline::first_deriv, gradLo,
                              tk::spline::first_deriv, gradHi);
        cubicspl.set_points(X, Y);

        ptSeries->clear();
        for (i = 0; i < num; i++)
            ptSeries->append(QPointF(X[i], Y[i]));

        firstPoints.clear();
        selSeries->clear();
        for (i = 0; i < xValList.count(); i++)
        {
            xval = xValList.at(i);
            yval = cubicspl(xval);
            firstPoints.append(QPointF(xval, yval));
        }
        minVal = minYvalue (firstPoints);
        maxVal = maxYvalue (firstPoints);
        max = minVal + ((maxVal - minVal) * 1.2);
        axisYalt->setRange(minVal, max);
        maxVal -= minVal;

        for (i = 0; i < firstPoints.count(); i++)
        {
            yval = firstPoints.at(i).y();
            firstPoints[i].setY(yval - minVal);
        }

        if (scale > 0)
        {
            divisor = aveYvalue(firstPoints, scaleLo, scaleHi);
        }
        else
        {
            divisor = maxVal;
        }
        for (i = 0; i < firstPoints.count(); i++)
            firstPoints[i].setY(firstPoints.at(i).y() / divisor);

        selSeries->append(firstPoints);
    }
}

// 44 like age selex 17 but with separate parameters for males and with revised controls
void equationDialog::twoSexRandomWalk()
{
    setTitle(QString("Pattern %1: Random Walk - female/male").arg(
                 QString::number(equationNum)));

    showSliders(genders + 2);
    showBins(true);
    showJoins(0);
    setParameterHeaders();

//    resetChart();
    connect (parameters, SIGNAL(dataChanged()), this, SLOT(parametersChanged()));

    // female ln(selex)
    ascendSeries = new QLineSeries(cht);
    ascendSeries->setPen(QPen(QBrush(Qt::blue), 3));
    cht->addSeries(ascendSeries);

    // male ln(selex)
    dscendSeries = new QLineSeries(cht);
    dscendSeries->setPen(QPen(QBrush(Qt::green), 3));
    cht->addSeries(dscendSeries);

    // female selex
    selSeries->setPen(QPen(QBrush(Qt::red), 3));
    cht->addSeries(selSeries);
    cht->setAxisX(axisXsel, selSeries);
    cht->setAxisY(axisY, selSeries);

    // male selex
    join3Series = new QLineSeries(cht);
    join3Series->setPen(QPen(QBrush(Qt::magenta), 3));
    cht->addSeries(join3Series);
    cht->setAxisX(axisXsel, join3Series);
    cht->setAxisY(axisY, join3Series);

    axisY->setTitleText("Selex (red)");

    axisYalt->setTitleText("Use Parm (blue)");
    cht->addAxis(axisYalt, Qt::AlignRight);
    ascendSeries->attachAxis(axisXsel);
    ascendSeries->attachAxis(axisYalt);

    updateTwoSexRandom();
}

void equationDialog::updateTwoSexRandom()
{
    float useparm = 0.;
    float sel = 0.;
    float parm = 0.;
    float cumparm = 0;
    float temp = 0, mean = 0, max = 0;
    QList<float> parms;
//    int age = binMin;
    int lastage = binMax;
    int lastageIndex = xValList.count() - 1;
    int minAge = QString(parameters->getRowData(0).at(2)).toInt();
    int maxAge = QString(parameters->getRowData(1).at(2)).toInt();
    ascendSeries->clear();
    selSeries->clear();
    firstPoints.clear();

    if (special != 0)
        lastage = abs(special);
    for (int i = 0; i < xValList.count(); i++) {
        if (xValList.at(i) > lastage) {
            lastageIndex = i - 1;
            break;
        }
    }

    parm = QString(parameters->getRowData(0).at(2)).toFloat();
    if (parm == -1000)
    {
        parm = 0.0;
    }
    cumparm = useparm = parm;
    if (xValList.count() > 0)
    {
    selSeries->append(QPointF(xValList.at(0), parm));

    parms.append(parm);
    ascendSeries->append(QPointF(xValList.at(0), useparm));
    firstPoints.append(QPointF(xValList.at(0), cumparm));

    for (int i = 1; i <= lastageIndex; i++)
    {
        parm = QString(parameters->getRowData(i).at(2)).toFloat();
        parms.append(parm);
        if (parms[i] > -999)
            useparm = parms[i];
        if (fabs(useparm) > max)
            max = fabs(useparm);
        ascendSeries->append(QPointF(xValList.at(i), useparm));
        cumparm += useparm;
        firstPoints.append(QPointF(xValList.at(i), cumparm));
    }
    axisYalt->setRange(-max, max);

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
        int next = i + 1;
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
}

// 45 similar to selex 14 but with separate parameters for males and with revised controls
void equationDialog::twoSexEachAge()
{
    setTitle(QString("Pattern %1: Revise Age - female/male").arg(
                 QString::number(equationNum)));
    disconnect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));
    max1Changed(xValList.last());
    min1Changed(xValList.first());
    max2Changed(xValList.last());
    min2Changed(xValList.first());
    max3Changed(xValList.last());
    min3Changed(xValList.first());
    max4Changed(2.0);
    min4Changed(-1.0);
    connect (this, SIGNAL(numbersUpdated()), this, SLOT(updateSel()));
    ui->label_1_name->setText("First age non-zero");
    ui->label_1_type->setText("Value");
    ui->label_2_name->setText("First age peak");
    ui->label_2_type->setText("Value");
    ui->label_3_name->setText("Last age peak");
    ui->label_3_type->setText("Value");
    ui->label_4_name->setText("Male ln(ratio)");
    ui->label_4_type->setText("Value");
    showSliders(genders + 2);
    showBins(true);
    showJoins(0);
    setParameterHeaders();

//    resetChart();
    connect (parameters, SIGNAL(dataChanged()), this, SLOT(parametersChanged()));

    // female ln(selex)
    ascendSeries = new QLineSeries(cht);
    ascendSeries->setPen(QPen(QBrush(Qt::blue), 3));
    cht->addSeries(ascendSeries);

    // female selex
    selSeries->setPen(QPen(QBrush(Qt::red), 3));
    cht->addSeries(selSeries);
    cht->setAxisX(axisXsel, selSeries);
    cht->setAxisY(axisY, selSeries);

    // male ln(selex)
    dscendSeries = new QLineSeries(cht);
    dscendSeries->setPen(QPen(QBrush(Qt::green), 3));
    cht->addSeries(dscendSeries);

    // male selex
    join3Series = new QLineSeries(cht);
    join3Series->setPen(QPen(QBrush(Qt::magenta), 3));
    cht->addSeries(join3Series);
    cht->setAxisX(axisXsel, join3Series);
    cht->setAxisY(axisY, join3Series);

    axisY->setTitleText("Selex (red)");

    axisYalt->setTitleText("Parm val (blue)");
    cht->addAxis(axisYalt, Qt::AlignRight);
    ascendSeries->attachAxis(axisXsel);
    ascendSeries->attachAxis(axisYalt);

    updateTwoSexEachAge();
}

void equationDialog::updateTwoSexEachAge()
{
    float parmfm = 0.;
    float parmml = 0.;
    float temp = 0.;
    float sel = 0.;
    float max = 0, min = 0;
    float age = binMin + binMidWidth * binWidth;
    int firstage = 0, lastage = xValList.last();
    int i, j, minbin, maxbin;
    int offset = genders + 2;
    int maleoffset = offset + special;
    int endoffset = maleoffset + special;
    float par;
    float par1 = ui->doubleSpinBox_1_value->value();
    float par2 = ui->doubleSpinBox_2_value->value();
    float par3 = ui->doubleSpinBox_3_value->value();
    float mlratio = 0;
    if (genders > 1)
        mlratio = ui->doubleSpinBox_4_value->value();
    QString msg("");

    firstPoints.clear();
    ascendSeries->clear();
    selSeries->clear();
    dscendSeries->clear();
    join3Series->clear();

    firstage = par1;
    if (par2 > par3)
    {
        minbin = par3;
        maxbin = par2;
    }
    else
    {
        minbin = par2;
        maxbin = par3;
    }
    if (firstage > 0)
    {
        ascendSeries->append(QPointF(xValList.first(), 0.0));
    }
    if (firstage > 1)
    {
        ascendSeries->append(QPointF(xValList.at(firstage - 1), 0.0));
    }
    for (i = firstage, j = offset; j < maleoffset; i++, j++)
    {
        par = QString(parameters->getRowData(j).at(2)).toFloat();
        if (par != -999)
            parmfm = par;
        ascendSeries->append(QPointF(xValList.at(i), parmfm));
    }
    for (; i < xValList.count(); i++)
    {
        ascendSeries->append(QPointF(xValList.at(i), parmfm));
    }
    min = minYvalue(ascendSeries->points());
    max = maxYvalue(ascendSeries->points());
    axisYalt->setRange(min, max);

    // female selex
    firstPoints.clear();
    fillValues(ascendSeries->points(), xValList, firstPoints);
    selSeries->append(firstPoints);

    temp = 8. - aveYvalue(firstPoints, minbin, maxbin);
    sel = 0.;

    for (int i = 0; i < firstPoints.count(); i++)
    {
        sel = logist(firstPoints.at(i).y() + temp);
        selSeries->append(QPointF(firstPoints.at(i).x(), sel));
    }
    if (genders > 1)
    {
        if (firstage > 0)
        {
            dscendSeries->append(QPointF(xValList.first(), 0.0));
        }
        if (firstage > 1)
        {
            dscendSeries->append(QPointF(xValList.at(firstage - 1), 0.0));
        }
        for (i = firstage, j = offset; j < maleoffset; i++, j++)
        {
            par = QString(parameters->getRowData(j + special).at(2)).toFloat();
            if (par == -1000)
                parmml = QPointF(ascendSeries->points().at(i)).x();
            else if (par != -999)
                parmml = par;
            dscendSeries->append(QPointF(xValList.at(i), parmml));
        }
        for (; i < xValList.count(); i++)
        {
            dscendSeries->append(QPointF(xValList.at(i), parmml));
        }

        // male selex
        firstPoints.clear();
        fillValues(dscendSeries->points(), xValList, firstPoints);
        selSeries->append(firstPoints);

        temp = 8. - aveYvalue(firstPoints, minbin, maxbin);
        sel = 0.;

        for (int i = 0; i < firstPoints.count(); i++)
        {
            sel = logist(firstPoints.at(i).y() + temp);
            join3Series->append(QPointF(firstPoints.at(i).x(), sel));
        }
    }
}

/** Returns the minimum x-value of a point list */
float equationDialog::minXvalue(const QList<QPointF> &pointlist)
{
    float value = 1000;
    for (int i = 0; i < pointlist.count(); i++)
    {
        if (pointlist.at(i).x() < value)
            value = pointlist.at(i).x();
    }
    return value;
}

/** Returns the maximum x-value of a point list */
float equationDialog::maxXvalue(const QList<QPointF> &pointlist)
{
    float value = -1000;
    for (int i = 0; i < pointlist.count(); i++)
    {
        if (pointlist.at(i).x() > value)
            value = pointlist.at(i).x();
    }
    return value;
}

/** Returns the minimum y-value of a point list */
float equationDialog::minYvalue(const QList<QPointF> &pointlist)
{
    float value = 1000;
    for (int i = 0; i < pointlist.count(); i++)
    {
        if (pointlist.at(i).y() < value)
            value = pointlist.at(i).y();
    }
    return value;
}

/** Returns the maximum y-value of a point list */
float equationDialog::maxYvalue(const QList<QPointF> &pointlist)
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
float equationDialog::aveXvalue(const QList<float> &xvals)
{
    float value = 0.;
    int i = 0;
    for (i = 0; i < xvals.count(); i++)
        value += (float)xvals.at(i);
    value /= i;
    return value;
}

/** Returns the average of the y-values of a point list */
float equationDialog::aveYvalue(const QList<QPointF> &pointlist, int start, int stop)
{
    float value = 0.;
    int i = (start == -1)? 0: start;
    int end = (stop == -1)? pointlist.count() - 1: stop;

    for (; i <= end; i++)
    {
        value += pointlist.at(i).y();
    }
    return (value / i);
}

void equationDialog::fillValues(const QList<QPointF> fewpoints, QList<float> xvals, QList<QPointF> &fullpoints)
{
    int i = 0, j =0;
    float yVal = 0;

    while (xvals.at(j) < fewpoints.at(0).x())
    {
        yVal = 0.0;
        fullpoints.append(QPointF(xvals.at(j++), yVal));
    }
    for (i = 0; i < fewpoints.count(); i++)
    {
        while (j < xvals.count() && xvals.at(j) < fewpoints.at(i+1).x())
        {
            yVal = evaluateLine(fewpoints.at(i), fewpoints.at(i+1), xvals.at(j));
            fullpoints.append(QPointF(xvals.at(j++), yVal));
        }
    }
    for (; j < xvals.count(); j++)
    {
        fullpoints.append(QPointF(xvals.at(j), yVal));
    }
}
