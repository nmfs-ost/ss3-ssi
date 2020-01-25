#include "dialogspwnrcrequationview.h"

#include <cmath>


DialogSpwnRcrEquationView::DialogSpwnRcrEquationView(QWidget *parent) :
    DialogEquationView (parent)
{
    pop = static_cast<population *>(static_cast<void *>(parent));
    genders = 2;

    populationNum = 0;

//    setTitle(QString("Plotted Curve"));
    setName(QString("Spawner-Recruitment Relationship"));
    setXvals(0.0, 1.0, .02);
    equationNum = 1;
    setIntVar1Range(2, 6);
    setIntVar1(2);
}

DialogSpwnRcrEquationView::~DialogSpwnRcrEquationView()
{
    close();
}


void DialogSpwnRcrEquationView::setOption(int value)
{
    parameterView->setParameterTable(pop->SR()->getFullParameters());
    setEquationNumber(value);
}

int DialogSpwnRcrEquationView::getOption() const
{
    return equationNum;
}

/*void DialogSpwnRcrEquationView::setParameters(tablemodel *params)
{
    disconnect(parameters, SIGNAL(dataChanged()), this, SLOT(parametersChanged()));
    parameters = params;
    connect(parameters, SIGNAL(dataChanged()), this, SLOT(parametersChanged()));
}



void DialogSpwnRcrEquationView::setParameterHeaders()
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
}*/

void DialogSpwnRcrEquationView::setup()
{
    if (pop == nullptr)
        return;

    if (!updating)
    {
        updating = true;

        resetChart();
        axisX->setTitleText(QString("Fraction Unfished"));
        axisY->setTitleText(QString("Recruits (1000)"));
        valSeries->setName("Recruits");

        switch (equationNum)
        {
        case 1: // case 1: null
            blank (Replaced, 1, 6, QString("For Beverton-Holt constrained curve"));
            break;

        case 2:  // case 2: Ricker - 2 parameters: log(R0) and steepness
            ricker ();
            break;

        case 3:  // case 3: Standard Beverton-Holt - 2 parameters
            bevertonHoltStandard ();
            break;

        case 4:  // case 4: Ignore Steepness and no bias adjustment, like CAGEAN - 2 parameters, uses only first one
            constant ();
            break;

        case 5:  // case 5: Hockey stick - 3 prarmenters: log(R0), steepness and Rmin
            hockeyStick ();
            break;

        case 6:  // case 6: Beverton-Holt with flat-top beyond Bzero - 2 parameters
            bevertonHoltBzeroFlat ();
            break;

        case 7: // case 7: Survivorship function - 3 parameters: log(R0), Zfrac, and Beta
            survivorship ();
            break;

        case 8:  // case 8: Shepherd - 3 parameters: log(R0), steepness, and shape C
            shepherd ();
            break;

        case 9: // case 10: Ricker re-parameterization - 3 parameters: log(R0), steepness, and Ricker power Gamma
            rickerReParm ();
            break;

        default:
            blank (NotUsed, equationNum);
            break;

        }
        updating = false;
    }
    updateGrid(cht->rect());
//    QDialog::update();
}

void DialogSpwnRcrEquationView::update()
{
    if (pop == nullptr)
        return;

    if (!updating)
    {
        updating = true;
        setMessageVisible(false);

        switch (equationNum)
        {
//        case 1: // case 1: null
//            blank (Replaced, 1, 6, QString("For Beverton-Holt constrained curve"));
//            break;

        case 2:  // case 2: Ricker - 2 : log(R0) and steepness
            updateRicker ();
            break;

        case 3:  // case 3: Standard Beverton-Holt - 2
            updateBevertonHoltStandard ();
            break;

        case 4:  // case 4: Ignore Steepness and no bias adjustment, like CAGEAN - 2 parameters, uses only first one
            updateConstant ();
            break;

        case 5:  // case 5: Hockey stick - 3 prarmenters: log(R0), steepness and Rmin
            updateHockeyStick ();
            break;

        case 6:  // case 6: Beverton-Holt with flat-top beyond Bzero - 2 parameters
            updateBevertonHoltBzeroFlat ();
            break;

        case 7: // case 7: Survivorship function - 3 parameters: log(R0), Zfrac, and Beta
            updateSurvivorship ();
            break;

        case 8:  // case 8: Shepherd - 3 parameters: log(R0), steepness, and shape C
            updateShepherd ();
            break;

        case 9: // case 10: Ricker re-parameterization - 3 parameters: log(R0), steepness, and Ricker power Gamma
            updateRickerReParm ();
            break;

        }
        updating = false;
    }
    updateGrid(cht->rect());

    QDialog::update();
}



/*void DialogSpwnRcrEquationView::setIntVar1(int value)
{
    intvar1 = value;
}

void DialogSpwnRcrEquationView::intVar1Changed(int value)
{
    setIntVar1 (value);
    emit numbersUpdated();
}

void DialogSpwnRcrEquationView::apply()
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

void DialogSpwnRcrEquationView::reset()
{
    disconnect(parameterView, SIGNAL(dataChanged()), this, SLOT(update()));
    parameterView->reset();
    connect(parameterView, SIGNAL(dataChanged()), this, SLOT(update()));
    update();
}*/

void DialogSpwnRcrEquationView::resetValues()
{
    if (pop != nullptr)
    {
        while (equationNum != pop->SR()->getOption())
            continue;
//        equationNum = pop->SR()->getOption();
        genders = pop->gender() > 1? 2: 1;

        building = false;
        waiting = false;
        updating = false;
        yMax = 1.0;
    }
}

void DialogSpwnRcrEquationView::restoreAll()
{
    if (pop != nullptr)
    {
        disconnectAll();
        // get all values from pop->SR()
        resetValues();
        // set parameters
        setParameters(pop->SR()->getFullParameters());
        connectAll();
        setup();
    }
}



population *DialogSpwnRcrEquationView::getPopulation() const
{
    return pop;
}

void DialogSpwnRcrEquationView::setPopulation(population *popultn)
{
    if (pop != nullptr)
    {
        pop = popultn;
        restoreAll();
        connect(parameterView, SIGNAL(inputChanged()), this, SLOT(update()));
    }
}

void DialogSpwnRcrEquationView::changeEquationNumber(int num)
{
    if (equationNum != num) {
        equationNum = num;
        restoreAll();
    }
    else {
        reset();
    }
}

void DialogSpwnRcrEquationView::setParameters(tablemodel *params)
{
    disconnect(parameterView, SIGNAL(inputChanged()), this, SLOT(update()));
    parameterView->setParameterTable(params);
    parameterView->reset();
    connect(parameterView, SIGNAL(inputChanged()), this, SLOT(update()));
}


//  SS_Label_43.3.2  Ricker
//      case 2:  // ricker
//  2 parameters: log(R0) and steepness
void DialogSpwnRcrEquationView::ricker()
{
    setLabel(QString("Option 2: Ricker"));

    chartview->setVisible(false);

    numParams = 2;
    parameterView->setNumParamsShown(numParams);
    parameterView->setName(0, "SR_LN(R0)");
    parameterView->setType(0, "Exp");
    parameterView->setName(1, "SR_Ricker_beta");
    parameterView->setType(1, "Value");

    updateRicker();
    chartview->setVisible(true);

}

//    Recr_virgin_adj = exp(SR_parm_work(1));
//    steepness=SR_parm_work(2);
//    NewRecruits = Recr_virgin_adj*SSB_curr_adj/
//                  SSB_virgin_adj *
//                  mfexp(steepness*(1.-SSB_curr_adj/SSB_virgin_adj));

void DialogSpwnRcrEquationView::updateRicker()
{
    /* R code
    SB = seq(0,1, 0.05)
    h = 0.95
    R0 = 10

    R = R0 * (SB / SB0) * exp(h * (1−SB/SB0))

    plot(SB,R) */

//    double SPRtemp =
    int yMax = 1;
    double SSB_curr_adj, Recruits;
    double Recr_virgin_adj = parameterView->getInput(0);
    double steepness = parameterView->getInput(1);

//    cht->removeSeries(valSeries);
//    cht->update();
    valSeries->clear();

    // assuming SSB_virgin_adj = 1.0, so it factors out
    for (int i = 0; i < xValList.count(); i++)
    {
        SSB_curr_adj = xValList.at(i);
//        Recruits = Recr_virgin_adj * (SSB_curr_adj/SSB_virgin_adj) * (exp(steepness * (1.0 - SSB_curr_adj/SSB_virgin_adj)));
        Recruits = Recr_virgin_adj * (SSB_curr_adj) * (exp(steepness * (1.0 - SSB_curr_adj)));
        valSeries->append(SSB_curr_adj, Recruits);
    }
    yMax = static_cast<int>((maxYvalue(valSeries->points()) + 100) / 100) * 100;
    axisY->setRange(0, yMax);
//    cht->addSeries(valSeries);
//    valSeries->attachAxis(axisY);
}

//  SS_Label_43.3.3  Beverton-Holt
//      case 3: // Beverton-Holt
//  2 parameters: log(R0) and steepness
void DialogSpwnRcrEquationView::bevertonHoltStandard()
{
    setLabel(QString("Option 3: Standard Beverton-Holt"));

    chartview->setVisible(false);

    numParams = 2;
    parameterView->setNumParamsShown(numParams);
    parameterView->setName(0, "SR_LN(R0)");
    parameterView->setType(0, "Exp");
    parameterView->setName(1, "SR_BH_steep");
    parameterView->setType(1, "Value");
//    parameterView->convertToInput(0);
//    parameterView->convertToInput(1);

    updateBevertonHoltStandard();
    chartview->setVisible(true);

//    cht->addAxis(axisXsel, Qt::AlignBottom);
//    cht->addAxis(axisY, Qt::AlignLeft);
}

//    Recr_virgin_adj = exp(SR_parm_work(1));
//    steepness=SR_parm_work(2);
//    alpha = 4.0 * steepness*Recr_virgin / (5.*steepness-1.);
//    beta = (SSB_virgin_adj*(1.-steepness)) / (5.*steepness-1.);
//    NewRecruits =  (4.*steepness*Recr_virgin_adj*SSB_curr_adj) /
//                   (SSB_virgin_adj*(1.-steepness)+(5.*steepness-1.)*SSB_curr_adj);

void DialogSpwnRcrEquationView::updateBevertonHoltStandard()
{
    /* R code from C. Wetzel
    SB0 = 1
    SB = seq(0,1, 0.05)
    h = 0.95
    R0 = 10

    R = R0*(4*h*SB)/(SB0*(1-h)+SB*(5*h-1))

    plot(SB,R) */

//    double SSB_virgin_adj = 1.0;
    int yMax;
    double SSB_curr_adj, Recruits;
 //   double logRecr_virgin_adj = val1;// ui->doubleSpinBox_1_value->value();
    double steep = parameterView->getInput(1);//val2;

    double Recr_virgin_adj = parameterView->getInput(0);//exp(logRecr_virgin_adj);

//    cht->removeSeries(valSeries);
//    cht->update();
    valSeries->clear();

    // SSB_virgin_adj = 1.0
    for (int i = 0; i < xValList.count(); i++)
    {
        SSB_curr_adj = xValList.at(i);
        Recruits = (4.0 * Recr_virgin_adj * steep * SSB_curr_adj) /
                ((1.0-steep) + SSB_curr_adj * (5.0*steep - 1.0));
        valSeries->append(SSB_curr_adj, Recruits);
    }
    yMax = static_cast<int>((maxYvalue(valSeries->points()) + 100) / 100) * 100;
    axisY->setRange(0, yMax);
//    cht->addSeries(valSeries);
//    valSeries->attachAxis(axisY);
}


//  SS_Label_43.3.4  constant expected recruitment
//      case 4:  // none
// case 4: CAGEAN-like
//  1 parameter:
//  Ignore Steepness and no bias adjustment. Use this in
//  conjunction with very low emphasis on recruitment deviations
//  to get CAGEAN-like unconstrained recruitment estimates,
void DialogSpwnRcrEquationView::constant()
{
    setLabel(QString("Option 4: Ignore steepness and no bias adjustment"));

    chartview->setVisible(false);

    numParams = 1;
    parameterView->setNumParamsShown(numParams);
    parameterView->setName(0, "SR_LN(R0)");
    parameterView->setType(0, "Exp");
    parameterView->setName(1, "SR_steepness");
    parameterView->setType(1, "Value");

    updateConstant();
    chartview->setVisible(true);

//    cht->addAxis(axisX, Qt::AlignBottom);
//    cht->addAxis(axisY, Qt::AlignLeft);
}

//    NewRecruits=Recr_virgin_adj;

void DialogSpwnRcrEquationView::updateConstant()
{
    int yMax = 1;
    double SSB_curr_adj, Recruits;
//    double logRecr_virgin_adj = ui->doubleSpinBox_1_value->value();

    double Recr_virgin_adj = parameterView->getInput(0);//exp(logRecr_virgin_adj);

//    cht->removeSeries(valSeries);
//    cht->update();
    valSeries->clear();

/*    for (int i = 0; i < xValList.count(); i++)
    {
        SSB_curr_adj = xValList.at(i);
        Recruits = Recr_virgin_adj;
        valSeries->append(SSB_curr_adj, Recruits);
    }*/
    Recruits = Recr_virgin_adj;
    valSeries->append(xValList.first(), Recruits);
    valSeries->append(xValList.last(), Recruits);

    updateGrid(cht->rect());
    yMax = static_cast<int>((maxYvalue(valSeries->points()) + 200) / 100) * 100;
    axisY->setRange(0, yMax);
//    cht->addSeries(valSeries);
//    valSeries->attachAxis(axisY);
}

//  SS_Label_43.3.5  Hockey stick
//      case 5:  // hockey stick  where "steepness" is now the fraction of B0 below which recruitment declines linearly
//               //  the 3rd parameter allows for a minimum recruitment level
// case 5: Hockey stick
//  3 parameters: log(R0), steepness, and Rmin for ln(R0),
//  fraction of virgin SSB at which inflection occurs,
//  and the Recruits level at SSB=0.0.
void DialogSpwnRcrEquationView::hockeyStick()
{
    setLabel(QString("Option 5: Hockey Stick"));

    chartview->setVisible(false);

    numParams = 3;
    parameterView->setNumParamsShown(numParams);
    parameterView->setName(0, "SR_LN(R0)");
    parameterView->setType(0, "Exp");
    parameterView->setName(1, "SR_hockey_infl");
    parameterView->setType(1, "Value");
    parameterView->setName(2, "SR_hockey_min_R");
    parameterView->setType(2, "Value");

    updateHockeyStick();
    chartview->setVisible(true);

//    cht->addAxis(axisX, Qt::AlignBottom);
//    cht->addAxis(axisY, Qt::AlignLeft);
}

//    Recr_virgin_adj = exp(SR_parm_work(1));
//    steepness = SR_parm_work(2);
      //  linear decrease below steepness * SSB_virgin_adj
//    temp = SR_parm_work(3) * Recr_virgin_adj +
//           SSB_curr_adj / (steepness * SSB_virgin_adj) *
//           (Recr_virgin_adj - SR_parm_work(3) * Recr_virgin_adj);
//    NewRecruits = Join_Fxn(0.0 * SSB_virgin_adj, SSB_virgin_adj, steepness * SSB_virgin_adj, SSB_curr_adj, temp, Recr_virgin_adj);

void DialogSpwnRcrEquationView::updateHockeyStick()
{
    int yMax = 1;
    double SSB_virgin_adj = 1.0;
    double temp, join, SSB_curr_adj, Recruits;
    double Recr_virgin_adj = parameterView->getInput(0);//exp(ui->doubleSpinBox_1_value->value());
    double inflection = parameterView->getInput(1);//ui->doubleSpinBox_2_value->value();
    double Rmin = parameterView->getInput(2);//ui->doubleSpinBox_3_value->value();
    Rmin = Rmin < 0? 0: Rmin;
    Rmin = Rmin > 1? 1: Rmin;

//    cht->removeSeries(valSeries);
//    cht->update();
    valSeries->clear();

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
        valSeries->append(SSB_curr_adj, Recruits);
    }
    yMax = static_cast<int>((maxYvalue(valSeries->points()) + 100) / 100) * 100;
    axisY->setRange(0, yMax);
//    cht->addSeries(valSeries);
//    valSeries->attachAxis(axisY);
}

//  SS_Label_43.3.6  Beverton-Holt, with constraint to have constant R about Bzero
//      case 6: //Beverton-Holt constrained
//  2 parameters: log(R0) and steepness.
void DialogSpwnRcrEquationView::bevertonHoltBzeroFlat()
{
    setLabel(QString("Option 6: Beverton-Holt with constraint"));

    chartview->setVisible(false);

    numParams = 2;
    parameterView->setNumParamsShown(numParams);
    parameterView->setName(0, "SR_LN(R0)");
    parameterView->setType(0, "Exp");
    parameterView->setName(1, "SR_steepness");
    parameterView->setType(1, "Value");

    updateBevertonHoltBzeroFlat();
    chartview->setVisible(true);

//    cht->addAxis(axisX, Qt::AlignBottom);
//    cht->addAxis(axisY, Qt::AlignLeft);
}

//    Recr_virgin_adj = exp(SR_parm_work(1));
//    steepness = SR_parm_work(2);
//    alpha = 4.0 * steepness*Recr_virgin / (5.*steepness-1.);
//    beta = (SSB_virgin_adj*(1.-steepness)) / (5.*steepness-1.);
//    if(SSB_curr_adj>SSB_virgin_adj) {SSB_BH1=SSB_virgin_adj;} else {SSB_BH1=SSB_curr_adj;}
//    NewRecruits=(4.*steepness*Recr_virgin_adj*SSB_BH1) / (SSB_virgin_adj*(1.-steepness)+(5.*steepness-1.)*SSB_BH1);

// case 6: Beverton-Holt with flat top beyond Bzero
void DialogSpwnRcrEquationView::updateBevertonHoltBzeroFlat()
{
    int yMax = 1;
    double SSB_virgin_adj = 0.8;
    double SSB_curr_adj, Recruits, SSB_BH1;
    double Recr_virgin_adj = parameterView->getInput(0);//exp(logRecr_virgin_adj);
    double steep = parameterView->getInput(1);//ui->doubleSpinBox_2_value->value();
    double alpha, beta;

//    cht->removeSeries(valSeries);
//    cht->update();
    valSeries->clear();

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
        valSeries->append(SSB_curr_adj, Recruits);
    }
    yMax = static_cast<int>((maxYvalue(valSeries->points()) + 100) / 100) * 100;
    axisY->setRange(0, yMax);
//    cht->addSeries(valSeries);
//    valSeries->attachAxis(axisY);
}


//  SS_Label_44.1.7  3 parameter survival based
//      case 7:  // survival
//  3 parameters: log(R0), zfrac, and Beta. Also SPR value which is not saved.
//  Suitable for sharks and low fecundity stocks to assure
//  recruits are <= population production.
void DialogSpwnRcrEquationView::survivorship()
{
    setLabel(QString("Option 7: Survivorship"));
    showInt1 (true, QString("Spawners per recruit on fish conditions"),
              QString("(this value will not be saved)"));

    chartview->setVisible(false);

    numParams = 3;
    parameterView->setNumParamsShown(numParams);
    parameterView->setName(0, "SR_LN(R0)");
    parameterView->setType(0, "Exp");
    parameterView->setName(1, "SR_surv_Zfrac");
    parameterView->setType(1, "Value");
    parameterView->setName(2, "SR_surv_Beta");
    parameterView->setType(2, "Value");

    updateSurvivorship();
    chartview->setVisible(true);

//    cht->addAxis(axisX, Qt::AlignBottom);
//    cht->addAxis(axisY, Qt::AlignLeft);
    cht->addAxis(axisYalt, Qt::AlignRight);
//    cht->addSeries(valSeries);
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
void DialogSpwnRcrEquationView::updateSurvivorship()
{
    int yMax = 1;
    double SPR = intvar1;
    double SSB_curr_adj, Recruits;
    double Recr_virgin_adj = parameterView->getInput(0);
    double Zfrac = parameterView->getInput(1);
    double Beta = parameterView->getInput(2);

    double SPB_virgin_adj = Recr_virgin_adj * SPR;

    double SRZ_0 = log(1.0/SPR); // 1.0/(SPB_virgin_adj/Recr_virgin_adj)
    double SRZ_min = SRZ_0 * (1.0 - Zfrac);
    double SRZ_surv = 0;

//    cht->removeSeries(valSeries);
//    cht->update();
    valSeries->clear();

    for (int i = 0; i < xValList.count(); i++)
    {
        SSB_curr_adj = xValList.at(i);
        SRZ_surv = exp((1.0 - pow(SSB_curr_adj, Beta)) * (SRZ_min - SRZ_0) + SRZ_0); // survival
        Recruits = SRZ_surv * SSB_curr_adj * SPB_virgin_adj;
        valSeries->append(SSB_curr_adj, Recruits);
    }
    yMax = static_cast<int>((maxYvalue(valSeries->points()) + 100) / 100) * 100;
    axisY->setRange(0, yMax);
//    cht->addSeries(valSeries);
//    valSeries->attachAxis(axisY);
}

//  SS_Label_43.3.8  Shepherd
//      case 8:  // Shepherd 3-parameter SRR. per Punt & Cope 2017
void DialogSpwnRcrEquationView::shepherd()
{
    setLabel(QString("Option 8: Shepherd"));

    chartview->setVisible(false);

    numParams = 3;
    parameterView->setNumParamsShown(numParams);
    parameterView->setName(0, "SR_LN(R0)");
    parameterView->setType(0, "Exp");
    parameterView->setName(1, "SR_steepness");
    parameterView->setType(1, "Value");
    parameterView->setName(2, "SR_Shepherd_c");
    parameterView->setType(2, "Value");

    updateShepherd();
    chartview->setVisible(true);

//    cht->addAxis(axisX, Qt::AlignBottom);
//    cht->addAxis(axisY, Qt::AlignLeft);
}

//    Recr_virgin_adj = exp(SR_parm_work(1));
//    Shepherd_c = SR_parm_work(3);
//    Shepherd_c2 = pow(0.2, SR_parm_work(3));
//    Hupper = 1.0 / (5.0 * Shepherd_c2);
//    steepness = 0.2 + (SR_parm_work(2) - 0.2) / (0.8) * (Hupper-0.2);
//    temp = (SSB_curr_adj) / (SSB_virgin_adj);
//    NewRecruits = (5. * steepness * Recr_virgin_adj * (1. - Shepherd_c2) * temp) /
//    (1.0 - 5.0 * steepness * Shepherd_c2 + (5. * steepness - 1.) * pow(temp, Shepherd_c));

void DialogSpwnRcrEquationView::updateShepherd()
{
    int yMax = 1;
//    double SSB_virgin_adj = 1.0;
    double SSB_curr_adj, Recruits;
    double Recr_virgin_adj = parameterView->getInput(0);//ui->doubleSpinBox_1_value->value();
    double SRparm2 = parameterView->getInput(1);//ui->doubleSpinBox_2_value->value();
    double shepherd_c = parameterView->getInput(2);//ui->doubleSpinBox_3_value->value();
    if (shepherd_c == 0.0)
        shepherd_c = .0000001;
    double shepherd_c2 = pow(.2, shepherd_c);
    double Hupper = 1.0 / (5.0 * shepherd_c2);
    double steep = 0.2 + (SRparm2 - 0.2) / (0.8) * (Hupper - 0.2);
    double temp;

//    cht->removeSeries(valSeries);
//    cht->update();
    valSeries->clear();

    // SSB_virgin_adj = 1.0, so it factors out
    for (int i = 0; i < xValList.count(); i++)
    {
        SSB_curr_adj = xValList.at(i);
//        temp = SSB_curr_adj;
        Recruits = (5.0 * steep * (1.0 - shepherd_c2) * SSB_curr_adj) /
                (1.0 - 5.0 * steep * shepherd_c2 + (5.0 * steep - 1.0) * pow (SSB_curr_adj, shepherd_c));
        valSeries->append(SSB_curr_adj, Recruits * Recr_virgin_adj);
    }
    yMax = static_cast<int>((maxYvalue(valSeries->points()) + 100) / 100) * 100;
    axisY->setRange(0, yMax);
//    cht->addSeries(valSeries);
//    valSeries->attachAxis(axisY);
}
/*
// case 9: Shepherd re-parameterization (beta) - not yet implemented
//  3 parameters: log(R0), steepness, and shape parameter, c.
void DialogSpwnRcrEquationView::shepherdReParm()
{
    setLabel(QString("Option 19: Shepherd Re-Parameterization"));

    numParams = 3;
    parameterView->setNumParamsShown(numParams);
    parameterView->setName(0, "SR_LN(R0)");
    parameterView->setType(0, "Exp");
    parameterView->setName(1, "SR_ShpReParm_steep");
    parameterView->setType(1, "Value");
    parameterView->setName(2, "SR_ShpReParm_c");
    parameterView->setType(2, "Value");

    updateShepherdReParm();
    chartview->setVisible(true);

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

void DialogSpwnRcrEquationView::updateShepherdReParm()
{
    int yMax = 1;
    double SPRtemp = pop->SR()->getSPR();
    double Recr_virgin_adj = parameterView->getInput(0);
    double SRparm2 = parameterView->getInput(1);
    double SRparm3 = parameterView->getInput(2);

    double Shep_top, penalty, SSB_virgin, Recr_virgin = 1;
    double Shep_bot;
    double Shep_top2;
    double Shepherd_c=exp(SRparm3);
    double Shepherd_c2=pow(0.2,Shepherd_c);
    double Hupper=1.0/(5.0*Shepherd_c2);
    double steepness=0.20001+((0.8)/(1.0+exp(-SRparm2))-0.2)/(0.8)*(Hupper-0.2);

 //   QString msg("This is a work in progress - this is a beta version only.");
//    msg.append("\n   It is based on Andre's FORTRAN.");
//    setMessage(msg);

    valSeries->clear();
    valSeries->append(0, 0);
    for (int i = 0; i < xValList.count(); i++)
    {
        SSB_virgin = xValList.at(i);
        Shep_top=5.0*steepness*(1.0-Shepherd_c2)*(SPRtemp*Recr_virgin)/SSB_virgin-(1.0-5.0*steepness*Shepherd_c2);
        Shep_bot=5.0*steepness-1.0;
        Shep_top2=posfun(Shep_top,0.001,penalty);
        double R_equil=(SSB_virgin/SPRtemp) * pow((Shep_top2/Shep_bot),(1.0/Shepherd_c));
        double B_equil=R_equil*SPRtemp;
        valSeries->append(SSB_virgin, B_equil * Recr_virgin_adj);
    }
    yMax = static_cast<int>((maxYvalue(valSeries->points()) + 100) / 100) * 100;
    axisY->setRange(0, yMax);
//    valSeries->attachAxis(axisY);
}*/

//  9: Ricker re-parameterization (beta) - power
//  3 Parameters: log(R0), steepness, and Ricker power, gamma.
void DialogSpwnRcrEquationView::rickerReParm()
{
    setLabel(QString("Option 9: Ricker Re-Parameterization"));

    chartview->setVisible(false);

    numParams = 3;
    parameterView->setNumParamsShown(numParams);
    parameterView->setName(0, "SR_LN(R0)");
    parameterView->setType(0, "Exp");
    parameterView->setName(1, "SR_RkrPwr_steep");
    parameterView->setType(1, "Value");
    parameterView->setName(2, "SR_RkrPwr_gamma");
    parameterView->setType(2, "Value");

    updateRickerReParm();
    chartview->setVisible(true);

//    cht->addAxis(axisX, Qt::AlignBottom);
//    cht->addAxis(axisY, Qt::AlignLeft);
}

// steepness = SR_parm_work(2);
// dvariable RkrPower=SR_parm_work(3);
// temp=SSB_curr_adj/SSB_virgin_adj;
// temp2 = posfun(1.0-temp,0.0000001,temp3);
// temp=1.0-temp2;  //  Rick's new line to stabilize recruitment at R0 if B>B0
// dvariable RkrTop =  log(5.0*steepness) * pow(temp2,RkrPower) / pow(0.8,RkrPower);
// NewRecruits = Recr_virgin_adj * temp * mfexp(RkrTop);

void DialogSpwnRcrEquationView::updateRickerReParm()
{
    int yMax = 1;
//    double SB_virgin_adj = 1.0;
    double SB_curr_adj, Recruits;
    double Recr_virgin_adj = parameterView->getInput(0);
    double steepness = parameterView->getInput(1);
    double rkrPower = parameterView->getInput(2);
    double rkrTop = 1.0;
    double temp, temp2, penalty;

    valSeries->clear();

    // SSB_virgin_adj = 1.0, so it factors out
    for (int i = 0; i < xValList.count(); i++)
    {
        SB_curr_adj = xValList.at(i);
        temp = SB_curr_adj; // / SB_virgin_adj;
        temp2 = posfun ((1.0 - temp), 0.0000001, penalty);
        temp = 1.0 - temp2;  // Rick's new line to stabilize recruitment at R0 if B> R0
        rkrTop = log(5.0 * steepness) * pow(temp2, rkrPower) / pow(0.8, rkrPower);
        Recruits = Recr_virgin_adj * temp * exp (rkrTop);

        valSeries->append(SB_curr_adj, Recruits);
    }
    yMax = static_cast<int>((maxYvalue(valSeries->points()) + 100) / 100) * 100;
    axisY->setRange(0, yMax);
//    cht->addSeries(valSeries);
//    valSeries->attachAxis(axisY);
}


