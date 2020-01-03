#include "dialogselectivityequationview.h"

#include <QMessageBox>

using namespace tk;

#ifndef PRECISION
#define PRECISION .0005
#endif
#ifndef SLIDER_SCALE
#define SLIDER_SCALE 1000
#define VALUE_SCALE .001
#endif

#include "ss_math.h"

static double neglog19 = -1.0 * log(19);


DialogSelexEquationView::DialogSelexEquationView(QWidget *parent, SelexType typ)
 : DialogEquationView (parent), slxType(typ)
{
    if (slxType == Age)
        name = QString ("Age Selectivity");
    else // (slxType == Size)
        name = QString ("Size Selectivity");
    title = name;

    selex = nullptr;
    fleet = nullptr;
    genders = 2;

    fleetNum = -1;
    equationNum = 0;
    special = 0;
    male = 0;
    bins.append(0);
    bins.clear();

    setWindowTitle(name);
    parameterView->setName(name + QString(" Parameters"));
    numParams = 0;
    setIntVar1Range(1, 10);
    setIntVar1(2);

    setJoinOne(10);
    setJoinTwo(10);
    setJoinThree(10);

    yMax = 1.0;
    axisY->setRange(0, 1.2);
}

DialogSelexEquationView::~DialogSelexEquationView()
{

}

Fleet *DialogSelexEquationView::getFleet() const {
    return fleet;
}

void DialogSelexEquationView::setFleet(Fleet *flt) {
    fleet = flt;
    fleetNum = flt->getNumber();
}

int DialogSelexEquationView::getSpecial() const {
    return special;
}

void DialogSelexEquationView::setSpecial(int value) {
    special = value;
}

int DialogSelexEquationView::getMale() const {
    return male;
}

void DialogSelexEquationView::setMale(int value) {
    male = value;
}

void DialogSelexEquationView::setOption(int value) {
    setEquationNumber(value);
}

void DialogSelexEquationView::setSelex(selectivity *slx) {
    disconnectAll();
    selex = slx;
    setParameters(selex->getParameterModel());
    restoreAll();
    connectAll();
}

void DialogSelexEquationView::changingSelex() {
//    disconnect(parameters, SIGNAL(dataChanged()), this, SLOT(parametersChanged()));
}

void DialogSelexEquationView::changedSelex(QStringList ql) {
//    connect(parameters, SIGNAL(dataChanged()), this, SLOT(parametersChanged()));
    changeSelex();
}

void DialogSelexEquationView::changeSelex() {
    restoreAll();
}

void DialogSelexEquationView::setXvals(const QList<float> &vals) {
    if (!vals.isEmpty()) {
        double first = vals.first();
        double last = vals.last();
        DialogEquationView::setXvals(first, last, 1.0);
    }
}

void DialogSelexEquationView::setXvalStrings(const QStringList &vals) {
    if (!vals.isEmpty()) {
        double first = vals.first().toDouble();
        double last = vals.last().toDouble();
        DialogEquationView::setXvals(first, last, 1.0);
    }
//    setXvalsConst(vals);
}

void DialogSelexEquationView::setBinValStrings(const QStringList &vals) {
    if (!vals.isEmpty()) {
        int binStep = 2;
        bins.clear();
        for (int i = 0; i < vals.count(); i++)
            bins.append(vals.at(i).toFloat());
        if (bins.count() > 2)
            binStep = static_cast<int> (bins.at(2) - bins.at(1));
        setBinMin(static_cast<int> (bins.first()));
        setBinMax(static_cast<int> (bins.last()));
        setBinStep(binStep);
        setMidBin(binStep/2);
        setXvals(bins);
    }
}

void DialogSelexEquationView::setBinVals(const QList<float> &vals) {
    if (!vals.isEmpty()) {
        int binStep = 2;
        bins.clear();
        for (int i = 0; i < vals.count(); i++)
            bins.append(vals.at(i));
        if (bins.count() > 2)
            binStep = static_cast<int> (bins.at(2) - bins.at(1));
        setBinMin(static_cast<int> (bins.first()));
        setBinMax(static_cast<int> (bins.last()));
        setBinStep(binStep);
        setMidBin(binStep/2);
        setXvals(bins);
    }
}

void DialogSelexEquationView::setParameterHeaders() {
    QString fltNum(QString::number(fleetNum));
    QString fltTyp, selexTyp;
    Fleet::FleetType ft = fleet->getType();
    if     (ft == Fleet::Fishing ||
            ft == Fleet::Bycatch)
        fltTyp = QString("fishery");
    else if (ft == Fleet::Survey)
        fltTyp = QString("survey");
    if (slxType == Age)
        selexTyp = QString("Age");
    else selexTyp = QString("Size");
    for (int i = 0; i < parameters->rowCount(); i++)
    {
        parameters->setRowHeader(i, QString("%1Sel p%2 %3 (%4)").arg(
                     selexTyp, QString::number(i+1), fltTyp, fltNum));
    }
}

void DialogSelexEquationView::resetValues() {
    if (selex != nullptr && fleet != nullptr)
    {
        fleetNum = fleet->getNumber();
        equationNum = selex->getPattern();
        special = selex->getSpecial();
        male = selex->getMale();
        genders = selex->getNumGenders();
        numParams = selex->getNumParameters();
        yMax = 1.2;
    }
}

void DialogSelexEquationView::restoreAll() {
    if (selex != nullptr && fleet != nullptr)
    {
        disconnectAll();
        // get all values from selex
        resetValues();
        // set parameters
        setParameters(selex->getParameterModel());
        connectAll();
        setup();
    }
}

void DialogSelexEquationView::setup() {
    if (fleet == nullptr || selex == nullptr)
        return;

    if (!updating)
    {
        updating = true;

        resetChart();
        // delete any other elements

        if (slxType == Age)
            axisX->setTitleText(QString("Age"));
        else
            axisX->setTitleText(QString("Length"));
        axisX->setRange(xValList.first(), xValList.last());

        axisY->setTitleText(QString("Selectivity"));
        axisY->setRange(0, 1.2);

        axisYalt->setRange(0, 1.2);
        cht->addAxis(axisYalt, Qt::AlignRight);

        valSeries->setName("Selex (red)");
        valSeries->setPen(QPen(QBrush(Qt::red), 5));
        valSeries->attachAxis(axisX);
        valSeries->attachAxis(axisY);

        numParams = parameters->rowCount();
        disconnectAll();

        switch (equationNum)
        {
        case 0:  // case 0: size and age selectivity - constant
            constant();
            break;

        case 1:  // case 1: size selectivity - logistic
            logistic();
            break;

        case 2:  // case 2: discontinued; use pattern 8 for double logistic
            blank (Replaced, equationNum, 8, QString(tr("Use pattern 8 for double logistic.")));
            break;

        case 3:  // case 3: discontinued
            blank (Discontinued, equationNum);
            break;

        case 4:  // case 4: discontinued; use pattern 30 to get spawning biomass
            blank (Discontinued, equationNum, 30, QString(tr("Use pattern 30 to get spawing biomass.")));
            break;

        case 5:  // case 5:  size selectivity - mirror another fleet for specified bin range
            mirror(2);
            break;

        case 6:  // case 6: size selectivity - non-parametric pattern // 43??
            linearSeg(0);
            break;

        case 7:  // case 7: discontinued; use pattern 8 for double logistic
            blank (Replaced, equationNum, 8, QString(tr("Use pattern 8 for double logistic.")));
            break;

        case 8:  // case 8: size selectivity - double logistic with eight parameters
            dblLogPeak();
            break;

        case 9:  // case 9: size selectivity - old double logistic with 6 parameters
            dblLogistic();
            break;

        case 10: // case 10: age selectivity - Constant for ages 1 to nages
        case 11: // case 11: age selectivity - Constant for specified age range (2 params)
            constant();
            break;

        case 12: // case 12: age selectivity - logistic
            logistic();
            break;

        case 13: // case 13: age selectivity - double logistic
            dblLogPeak();
            break;

        case 14: // case 14: age selectivity - separate parm for each age
            eachAge();
            break;

        case 15: // case 15: size and age selectivity - mirror another fleet
            mirror(0);
            break;

        case 16:  // case 16: size selectivity - Coleraine-Gauss pattern
            coleGauss();
            break;

        case 17: // case 17: age selectivity - each age has parameter as random walk
                //        transformation is selex=exp(parm); some special codes
            randomWalk(0);
            break;

        case 18: // case 18: age selectivity - double logistic with smooth transition
            dblLogPeak();
            break;

        case 19: // case 19: age selectivity - old double logistic
            dblLogistic();
            break;

        case 20: // case 20: age selectivity - double normal with plateau
            dblNormEndpts();
            break;

        case 21: // case 21: size selectivity - non-parametric
                // first N params are xvals, next N params are yvals
            linearSeg(1);
            break;

        case 22: // case 22: size selectivity - using double_normal_plateau (similar to CASAL)
            dblNormCasal();
            break;

        case 23: // case 23: size selectivity - double_normal_plateau where final value can be greater than 1.0
        case 24: // case 24: size selectivity - using double_normal_plateau and lots of bells and whistles
            dblNormEndpts();
            break;

        case 25: // case 25: size selectivity - exponential-logistic
        case 26: // case 26: age selectivity - exponential logistic
            expLogistic();
            break;

        case 27: // case 27: age selectivity - cubic spline
            cubicSpline();
            break;

        case 30: // case 30: No equation, Uses spawning biomass/output
            blank (NoEquation, equationNum, 0, QString("Selex = spawning biomass/output (e.g, for an egg and larvae survey)"));
            break;

        case 31: // case 31: No equation, Uses exp(recruit deviation)
            blank (NoEquation, equationNum, 0, QString("Selex = exp(recruit deviation)"));
            break;

        case 32: // case 32: No equation, Uses spawning biomass * exp(recruit deviation)
            blank (NoEquation, equationNum, 0, QString("Selex = spawning biomass * exp(recruit deviation)"));
            break;

        case 33: // case 33: No equation, Uses recruitment, age-0 recruits
            blank (NoEquation, equationNum, 0, QString("Selex = recruitment, age-0 recruits"));
            break;

        case 34: // case 34: No equation, Uses depletion
            blank (NoEquation, equationNum, 0, QString("Selex = depletion (spawning biomass/virgin spawning biomass)"));
            break;

        case 35: // case 35: No equation, Uses survey of a deviation vector
            blank (NoEquation, equationNum, 0, QString("Selex = survey of a deviation vector specified in Q section"));
            break;

        case 41: // case 41: age selectivity - random walk each age scaled by average of values at low age through high age
                //    transformation as selex=exp(parm); some special codes
            randomWalk(2.);
            break;

        case 42: // case 42: age selectivity - cubic spline scaled by average of values at low age through high age
            cubicSpline(2.);
            break;

        case 43: // case 43: size selectivity - non-parametric scaled by average of values at low bin through high bin
            linearScaled();
            break;

        case 44: // case 44: age selectivity - like age selex 17 but with separate parameters for males and with revised controls
            twoSexRandomWalk();
            break;

        case 45: // case 45: age selectivity - like age selex 14 but with separate parameters for males and with revised controls
            twoSexEachAge();
            break;

        default:
            blank(NotUsed, equationNum);
            break;

        }
        updating = false;
    }
    cht->addSeries(valSeries);
    valSeries->attachAxis(axisY);
    updateGrid(cht->rect());
    connectAll();
}

void DialogSelexEquationView::update() {
    if (fleet == nullptr || selex == nullptr)
        return;

    if (!updating)
    {
        updating = true;
        setMessageVisible(false);

        switch (equationNum)
        {
        case 0:  // case 0 constant size selectivity
            updateConstant();
            break;

        case 1:  // case 1 logistic size selectivity
            updateLogistic();
            break;

        case 5:  // case 5 mirror another fleets size selectivity for specified bin range
            updateMirror(getSpecial());
            break;

        case 6:  // case 6 non-parametric size selex pattern
            updateLinear(0);
            break;

        case 8:  // case 8 double logistic with eight parameters
            updateDblLogPeak();
            break;

        case 9:  // case 9 old double logistic with 6 parameters
            updateDblLogistic();
            break;

        case 10: // 10 #Constant age-specific selex for ages 1 to nages
            updateConstant();
            break;

        case 11: // 11 #Constant age-specific selex for specified age range
            updateConstantRange();
            break;

        case 12: // 12 #age selectivity - logistic
            updateLogistic();
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

        case 18: // 18 #age selectivity: double logistic with smooth transition
            updateDblLogSmooth();
            break;

        case 19: // 19 #age selectivity: old double logistic
            updateDblLogistic();
            break;

        case 20: // 20 #age selectivity: double normal with plateau
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

        case 24: // case 24 size selectivity using double_normal_plateau and lots of bells and whistles
            updateDblNormEndpts();
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
        updating = false;
    }
    updateGrid(cht->rect());
}

// case 0 constant size selectivity
// 10 #Constant age-specific selex for ages 1 to nages
// Size selectivity 0, age selectivity 10 */
void DialogSelexEquationView::constant (double val) {
    QString msg = (QString("Pattern %1: Constant selectivity 1.0").arg(QString::number(equationNum)));
    setLabel(msg);

    showBins(true);
    chartview->setVisible(false);

    if (equationNum == 11) {
        constantRange();
    }
    else  {
        numParams = 0;
        parameterView->setNumParamsShown(numParams);
        updateConstant(val);
    }
    chartview->setVisible(true);
}

// Size selectivity 0, age selectivity 10 - 0 parameters
void DialogSelexEquationView::updateConstant (double val) {
    double binMid = getBinStep() * getMidBin();
    double start = 0;
    double end = 1;

    if (!xValList.isEmpty())
    {
        if (equationNum == 10) {
            start = xValList.at(1);// + binMid;
        }
        else { // equationNum == 0
            start = xValList.at(0);// + binMid;
        }

        end = xValList.last() + binMid;
        updateConstant(val, start, end);
    }
}

// case 11: Constant age-specific selex for specified age range - 2 parameters
void DialogSelexEquationView::constantRange () {
    setLabel(QString("Pattern %1: Constant selectivity 1.0 within age range").arg(equationNum));

    if (parameters->rowCount() == 2) {
        numParams = 2;
        parameterView->setNumParamsShown(numParams);
        parameterView->setName(0, QString("Lo age"));
        parameterView->setType(0, QString("Integr"));
        parameterView->setName(1, QString("Hi age"));
        parameterView->setName(1, QString("Integr"));

        updateConstantRange();
    }
}

// 11 #Constant age-specific selex for specified age range
void DialogSelexEquationView::updateConstantRange (double val) {
    double par1 = parameterView->getInput(0);
    double par2 = parameterView->getInput(1);
//    double binMid = getBinStep() * getMidBin();

    if (par1 < 0)
        par1 = xValList.first();

    if (par2 < 0)
        par2 = xValList.last();
    if (par2 <= par1)
        par2 = par1 + 1;// + binMid;
    double start = par1;// + binMid;
    double end = par2;// + binMid;

    updateConstant(val, start, end);
}

// general constant graph
void DialogSelexEquationView::updateConstant (double val, double first, double last) {
    double start = xValList.first();// + binMid;
    double end = xValList.last();// + binMid;

    valSeries->clear();
    if (start < (first - .001)) {
        valSeries->append(QPointF(start, 0));
        valSeries->append(QPointF(first, 0));
    }
    valSeries->append(QPointF(first, val));

    valSeries->append(QPointF(last, val));
    if ((last + .001) < end) {
        valSeries->append(QPointF(last, 0));
        valSeries->append(QPointF(end, 0));
    }
}

// logistic
// size selectivity: 1
// age selectivity: 12
//    2 parameters
void DialogSelexEquationView::logistic () {
    // Show equation number and descriptor
    setLabel(QString("Pattern %1: Logistic").arg(equationNum));

    if (parameters->rowCount() == 2) {
        // Show num of parameters
        numParams = 2;
        parameterView->setNumParamsShown(numParams);
        // set parameter names (and transforms, if any)
        setParameterHeaders();
        parameterView->setName(0, QString("Value at 50%"));
        parameterView->setType(0, QString("Value"));
        parameterView->setName(1, QString("Diff 95% & 50%"));
        parameterView->setType(1, QString("Value"));
        // add any other items
        showBins(true);
        // update the equation values
        updateLogistic();
    }
}

// logistic
//    sel = 1/(1 + exp(neglog19 * (xval - par1) / par2))

void DialogSelexEquationView::updateLogistic() {
    double yVal = 0;
    double neglog19_par2 = 0, temp = 0;
    double xVal = 0;
    double par1 = parameterView->getInput(0);
    double par2 = parameterView->getInput(1);

    if (par1 < 0)
        par1 = xValList.first();
    if (par2 < -50)
        par2 = -50;

    valSeries->clear();

    // for speed, check for par2 taken out of loop
    // and equation rewritten as 1/(1 + exp(neglog19_par2 * (xval - par1))
    //  where neglog19_par2 = neglog19 / par2
    //
    if (par2 > 0) {
        neglog19_par2 = neglog19 / par2;
    }
    else { // avoid inf and nan
        updateConstant(1.0, xValList.first(), xValList.last());
    }

    for (int i = 0; i < xValList.count(); i++)
    {
        xVal = xValList.at(i);
        temp = neglog19_par2 * (xVal - par1);
        yVal = 1 / (1 + exp(temp));
        valSeries->append(QPointF(xVal, yVal));
    }
}

// Mirror with or without limits
// size selectivity: 5 and 15
// age selectivity: 15
// equation 5 - mirror another fleet's size selectivity for specified bin range
//   2 parameters
// equation 15 - mirror another fleet's selectivity for all size bins
//   no parameters
void DialogSelexEquationView::mirror (int num) {
    int flt = special < 1? 1: special;
    // show equation num and fleet mirrored
    QString msg (QString("Pattern %1: Mirror of Fleet (%2)").arg(
                 QString::number(equationNum),
                 QString::number(flt)));
    setLabel(msg);

    if (parameters->rowCount() == num) {
        // show parameters
        numParams = num;
        parameterView->setNumParamsShown(numParams);
        chartview->setVisible(false);
        if (num == 2)
        {
            msg.append(QString(" between Lo and Hi bins"));
            setLabel(msg);
            parameterView->setName(0, QString("Lo bin"));
            parameterView->setType(0, QString("Integr"));
            parameterView->setName(1, QString("Hi bin"));
            parameterView->setType(1, QString("Integr"));
        }
        // update values (and check fleet number)
        updateMirror(flt);
    }
}

// mirror
// describe low and high bins (if existing)
// check fleet number
//
void DialogSelexEquationView::updateMirror(int flt) {
    QString msg(QString("Mirroring selectivity from Fleet %1 ").arg(QString::number(flt)));
    if (equationNum == 5)
    {
        int start = 1, end = 1;
        int par1 = static_cast<int>(parameterView->getInput(0));
        int par2 = static_cast<int>(parameterView->getInput(1));
        if (par1 < 1 || par1 > selex->getNumXvals()){
            start = 1;
        }
        else {
            start = par1;
        }

        if (par2 < 1 || par2 > bins.count()) {
            end = bins.count();
        }
        else {
            end = par2;
        }
        msg.append(QString("\nFrom Bin %1 (%2)  to Bin %3 (%4)").arg(
                       QString::number(start),
                       QString::number(bins.at(start-1)),
                       QString::number(end),
                       QString::number(bins.at(end-1))));
        if (end < start) {
            msg.append(tr ("\n\nERROR: Hi Bin is less than Lo Bin. "));
        }
    }

    if (fleetNum <= flt) {
        msg.append(tr("\n\nERROR: Mirror must be of previous fleet.\nPlease change Special value."));
    }
    setMessage(msg);
}

// Linear segments
// size selectivity: 6, 21 and 43
//  6 parameters = 2 + special (num nodes): 2 for first/last and then values at nodes
// 21 parameters = 2 * special (num nodes): x-values at nodes followed by y-values
// 43 parameters = 4 + special (num nodes): 2 for scaling, 2 for first/last and then values at nodes
void DialogSelexEquationView::linearSeg (int scale) {
    int numNodes = getSpecial(), num = 0;
    int index = 0;
    // show equation number and number of segments (1 more than nodes)
    QString msg(QString("Pattern %1: Non-parametric, %2 Linear segments").arg(
                    QString::number(equationNum),
                    QString::number(numNodes + 1)));
    setLabel(msg);

    numParams = parameters->rowCount();
    parameterView->setNumParamsShown(numParams);

    if (scale == 1) //  equation 21
    {
        linearPlain ();
        return;
    }
    else if (scale == 2) //  equation 43
    {
        linearScaled();
        return;
    }

    num = parameters->rowCount() - 2;
    if (num < 2 || numNodes != num)  {
        setMessage(tr("ERROR: Incorrect number of parameters. \nShould be Special +2"));
    }
    else {
        setParameterHeaders();
        parameterView->setName(0, QString("length for P1"));
        parameterView->setType(0, QString("Value"));
        parameterView->setName(1, QString("length for P2"));
        parameterView->setType(1, QString("Value"));
        parameterView->setName(2, QString("ln of sel at P1"));
        parameterView->setType(2, QString("Value"));

        for (index = 3; index < numParams - 1; index++)  {
            parameterView->setName(index, QString("ln of sel midpt"));
            parameterView->setType(index, QString("Value"));
        }
        parameterView->setName(index, QString("ln of sel at P2"));
        parameterView->setType(index, QString("Value"));

        showBins(true);

        ascendSeries = new QLineSeries(cht);
        ascendSeries->setPen(QPen(QBrush(Qt::blue), 2));
        ascendSeries->setName(QString("Ln of Selex"));
        cht->addSeries(ascendSeries);

        axisYalt->setTitleText("Ln of Selex");
        cht->addAxis(axisYalt, Qt::AlignRight);
        ascendSeries->attachAxis(axisX);
        ascendSeries->attachAxis(axisYalt);

        cht->legend()->show();
        cht->legend()->setAlignment(Qt::AlignLeft);

        updateLinear(scale);
    }
}

// case 21 non-parametric size selectivity
//  parameters = 2 * special (num nodes)
//    x-values at nodes followed by y-values at nodes
void DialogSelexEquationView::linearPlain() {
    int numNodes = getSpecial(), num = 0;
    QString Min, Max;
    QStringList rowdata;
    QString msg(QString("Pattern %1: Non-parametric, %2 Linear segments").arg(
                    QString::number(equationNum),
                    QString::number(numNodes + 1)));
    setLabel(msg);

    num = numNodes * 2;
    if (parameters->rowCount() < num) {
        setMessage(tr("Incorrect number of parameters. \nShould be (Special * 2)"));
        return;
    }
    else {
        numParams = parameters->rowCount();
        parameterView->setNumParamsShown(numParams);
        showBins(true);

        setParameterHeaders();
        // set parameter min and max
        Min = QString::number(xValList.first());
        Max = QString::number(xValList.last());
        for (int i = 0; i < special; i++)
        {
            rowdata = parameters->getRowData(i);
            rowdata[0] = Min; rowdata[1] = Max;
            parameters->setRowData(i, rowdata);
        }
        updateLinearPlain();
    }
}

// age selectivity 43
void DialogSelexEquationView::linearScaled () {
    int numNodes = getSpecial(), num = 0;
    int index;
    QString msg(QString("Pattern %1: Non-parametric %2 Linear segments").arg(
                    QString::number(equationNum),
                    QString::number(numNodes + 1)));
    setLabel(msg);

    num = parameters->rowCount() - 4;
    if (num < 3 || numNodes != num)  {
        setMessage(tr("ERROR: Incorrect number of parameters. \nShould be Special +4"));
    }
    else {
        numParams = parameters->rowCount();
        parameterView->setNumParamsShown(numParams);
        setParameterHeaders();
        parameterView->setName(0, QString("Scale bin Lo"));
        parameterView->setType(0, QString("Value"));
        parameterView->setName(1, QString("Scale bin Hi"));
        parameterView->setType(1, QString("Value"));
        parameterView->setName(2, QString("length for P1"));
        parameterView->setType(2, QString("Value"));
        parameterView->setName(3, QString("length for P2"));
        parameterView->setType(3, QString("Value"));
        parameterView->setName(4, QString("ln of sel at P1"));
        parameterView->setType(4, QString("Value"));

        for (index = 5; index < num - 1; index++)
        {
            parameterView->setName(index, QString("ln of sel midpt"));
            parameterView->setType(index, QString("Value"));
        }
        parameterView->setName(index, QString("ln of sel at P2"));
        parameterView->setType(index, QString("Value"));

        showBins(true);
        cht->removeSeries(valSeries);

        ascendSeries = new QLineSeries(cht);
        ascendSeries->setPen(QPen(QBrush(Qt::blue), 2));
        ascendSeries->setName(QString("Ln of Selex"));
        cht->addSeries(ascendSeries);

        axisYalt->setTitleText("Ln of Selex");
        cht->addAxis(axisYalt, Qt::AlignRight);

        ascendSeries->attachAxis(axisX);
        ascendSeries->attachAxis(axisYalt);

        cht->legend()->show();
        cht->legend()->setAlignment(Qt::AlignLeft);

        updateLinearScaled();
    }
}

// size selectivity 6
void DialogSelexEquationView::updateLinear(int scale) {
    double y = 0;
    int axisYMax = 0;
    int axisYMin = 0;
    double temp = 0;
    double len = 0;
    int binMax = getBinMax();
    int numNodes = getSpecial();
    QVector<double> xVal;
    QVector<double> yVal;
    int num = parameters->rowCount() - 2;
    int index = 0;

    if (scale == 1)
        updateLinearPlain();
    else if (scale == 2)
        updateLinearScaled();
    else
    {
        valSeries->clear();
        firstPoints.clear();
        ascendSeries->clear();

        for (int i = 0; i <= num+1; i++)
        {
            xVal.append(0.0);
            yVal.append(0.0);
        }
        xVal[0] = xValList.first();
        yVal[0] = -10.0;
        xVal[1] = parameterView->getInput(0);

        for (index = 1; index <= num; index++)
        {
            yVal[index] = parameterView->getInput(index+1);
        }
        xVal[num] = parameterView->getInput(1);

        xVal[num + 1] = xValList.last();
        yVal[num + 1] = yVal[num];

        temp = (xVal[num] - xVal[1]) / (num - 1);
        for (index = 2; index < numNodes; index++)
        {
            xVal[index] = xVal[1] + (index - 1) * temp;
        }

        for (int i = 0; i <= numNodes; i++)
            ascendSeries->append(QPointF(xVal[i], yVal[i]));
        if (xVal[numNodes] > binMax)
            temp = xVal[numNodes] + 2;
        else
            temp = binMax;
        ascendSeries->append(QPointF(temp, yVal[numNodes]));
        axisYMax = static_cast<int>(maxYvalue(ascendSeries->points()) + 1);
        axisYMin = static_cast<int>(minYvalue(ascendSeries->points()) - 2);
        axisYalt->setRange(axisYMin, axisYMax);

//        yMax = yVal[0];
        for (int i = 0; i < xValList.count(); i++)
        {
            len = xValList.at(i);
            for (int j = 1; j <= numNodes; j++)
            {
                if (len >= xVal[j -1] && len < xVal[j])
                {
                    y = evaluateLine(QPointF(xVal[j-1], yVal[j-1]), QPointF(xVal[j], yVal[j]), len);
//                    if (y > yMax)
//                        yMax = y;
//                    if (y < axisYMin)
//                        axisYMin = static_cast<int>(y) - 2;
//                    axisYMax = static_cast<int>(yMax) + 1;
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
        updateLinearExp(scale);
    }
}

// case 21 non-parametric size selectivity
/* This is the code from SS_selex.tpl
 *    while(j<=nlength)
 *    {
 *      if(len_bins(j)<=sp(z)) {
 *        sel(j) = lastsel + (len_bins(j)-lastSelPoint)/(sp(z)-lastSelPoint) * (sp(z+k)-lastsel);
 *        j++;
 *      }
 *      else if(z<=k) {
 *        lastSelPoint=sp(z);
 *        lastsel=sp(z+k);
 *        z++;
 *      }
 *      else {  //  for sizes beyond last point
 *        sel(j)=sp(k+k);
 *        j++;
 *      }
 *    }
 * */
// I've simplified it to just the points given, 0 before nodes and lastSel after nodes
void DialogSelexEquationView::updateLinearPlain() {
    int i = 0;
    double xVal;
    double yVal = 1.0;
    valSeries->clear();
    xVal = parameterView->getInput(0);
    while (xVal > xValList.at(i)) {
        valSeries->append(QPointF(xValList.at(i++), 0.0));
    }
    for (i = 0; i < special; i++) {
        xVal = parameterView->getInput(i);
        yVal = parameterView->getInput(i+special);
        valSeries->append(QPointF(xVal, yVal));
    }
    if (xVal < xValList.last()) {
        valSeries->append(QPointF(xValList.last(), yVal));
    }
}

// age selectivity 43
void DialogSelexEquationView::updateLinearScaled() {
    float y = 0;
    int axisYMax = 0;
    int axisYMin = 0;
    QString msg;
    float temp = 0;
    float len = 0;
    int numNodes = getSpecial();
    int index = 0;
    int binMax = getBinMax();
    QVector<float> xVal;
    QVector<float> yVal;
    float bMin = parameterView->getInput(0);//ui->doubleSpinBox_1_value->value();
    float bMax = parameterView->getInput(1);//ui->doubleSpinBox_2_value->value();
    int num = parameters->rowCount() - 4;

    if (num < 2 || numNodes != num)
    {
        setMessage(tr("ERROR: Incorrect number of parameters. \nShould be Special + 4"));
        return;
    }

    if (bMin < 1)
    {
        setMessage(tr("Minimum for param 1 is first bin. adjusting to first bin..."));
        bMin = 1;
        parameterView->setSlider(0, bMin);
        return;
    }
    if (bMax < bMin)
    {
        setMessage(tr("Maximum is less than Minimum. adjusting to min + 1..."));
        bMax = bMin + 1.0;
        parameterView->setSlider(1, bMax);
        return;
    }
    if (bMax > binMax)
    {
        setMessage(tr("Maximum for param 2 is last bin. adjusting to number of bins..."));
        bMax = bins.count();
        parameterView->setSlider(1, bMax);
        return;
    }
    if (bMax < bMin)
    {
        setMessage(tr("Minimum is greater than Maximum. adjusting to max - 1..."));
        bMin = bMax - 1.0;
        parameterView->setSlider(0, bMin);
        return;
    }

    ascendSeries->clear();
    valSeries->clear();

    for (int i = 0; i < num+1; i++)
    {
        xVal.append(0);
        yVal.append(0);
    }
    xVal[0] = xValList.first();
    yVal[0] = -10.0;
    xVal[1] = parameterView->getInput(3);

    for (index = 1; index <= num; index++)
    {
        yVal[index] = parameterView->getInput(index+4);
    }

    xVal[numNodes] = parameterView->getInput(4);
    xVal[numNodes + 1] = xValList.last();
    yVal[numNodes + 1] = yVal[numNodes];

    temp = (xVal[numNodes] - xVal[1]) / (numNodes - 1);
    for (int i = 2; i < numNodes; i++)
    {
        xVal[i] = xVal[i-1] + temp;
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
    updateLinearExp(2);
    axisYMax = static_cast<int>(yMax + 1);
    axisYMin = static_cast<int>(minYvalue(firstPoints) - 2);
    axisYalt->setRange(axisYMin, axisYMax);
}

// Use the list of points (firstPoints)
// to create the valSeries as firstPoints.x, exp(firstPoints.y - yMax)
// finally, scale by selected bins (if eqn 43)
void DialogSelexEquationView::updateLinearExp(int scale) {
    double yVal;

    yMax = maxYvalue(firstPoints);

    for (int i = 0; i < firstPoints.count(); i++)
    {
        yVal = exp(firstPoints.at(i).y() - yMax);
        valSeries->append(firstPoints.at(i).x(), yVal);
    }
    if (scale == 2)
    {
        int firstBin = static_cast<int>(parameterView->getInput(0)) - 1;
        int lastBin = static_cast<int>(parameterView->getInput(1)) - 1;
        double firstVal;// = xValList.at(firstBin);
        double lastVal;
        double yTotal = 1;
        double yAve = 1;
        int i = 0, yNum = 0;
        if (firstBin <= 0)
            firstBin = 0;
        if (lastBin >= bins.count())
            lastBin = bins.count() - 1;
        firstVal = bins.at(firstBin);
        lastVal = bins.at(lastBin);
        // reuse firstPoints for scaling
        firstPoints = valSeries->points();

        while (firstPoints.at(i++).x() < firstVal)
            continue;
        while (firstPoints.at(i).x() <= lastVal) {
            yNum++;
            yTotal += firstPoints.at(i++).y();
        }

        yAve = yTotal / yNum;
        for (int i = 0; i < firstPoints.count(); i++)
        {
            yVal = firstPoints.at(i).y() / yAve;
            valSeries->append(firstPoints.at(i).x(), yVal);
        }
    }
}

// Double logistic
//  size selex 9: old double logistic with no defined peak
//  age selex 19: old double logistic
//    6 parameters: infl1, slope1, infl2, slope2, first bin, offset
void DialogSelexEquationView::dblLogistic() {
    setLabel(QString("Pattern %1: Simple Double Logistic").arg(
                     QString::number(equationNum)));

    if (parameters->rowCount() == 6) {
        numParams = 6;
        parameterView->setNumParamsShown(numParams);
        parameterView->setName(0, QString("Infl 1"));
        parameterView->setType(0, QString("Value"));
        parameterView->setName(1, QString("Slope 1"));
        parameterView->setType(1, QString("Value"));
        parameterView->setName(2, QString("Infl 2"));
        parameterView->setType(2, QString("Value"));
        parameterView->setName(3, QString("Slope 2"));
        parameterView->setType(3, QString("Value"));
        parameterView->setName(4, QString("First Bin"));
        parameterView->setType(4, QString("Integr"));
        parameterView->setName(5, QString("Offset"));
        parameterView->setType(5, QString("Integr"));
        setParameterHeaders();
        showBins(true);
        cht->removeSeries(valSeries);

        ascendSeries = new QLineSeries(cht);
        ascendSeries->setName(QString("Logistic1"));
        ascendSeries->setPen(QPen(QBrush(Qt::green), 2));
        cht->addSeries(ascendSeries);
        ascendSeries->attachAxis(axisX);
        ascendSeries->attachAxis(axisYalt);

        dscendSeries = new QLineSeries(cht);
        dscendSeries->setName(QString("Logistic2"));
        dscendSeries->setPen(QPen(QBrush(Qt::blue), 2));
        cht->addSeries(dscendSeries);
        dscendSeries->attachAxis(axisX);
        dscendSeries->attachAxis(axisYalt);

        cht->addAxis(axisYalt, Qt::AlignRight);

        cht->legend()->setVisible(true);
        cht->legend()->setAlignment(Qt::AlignLeft);

        updateDblLogistic();
    }
}

/** This is the code from ss_selex.tpl
 *      // size selex
 *      k1 = int(value(sp(5)));
 *      if(k1 > 1)
 *          sel(1,k1-1) = 0.0;
 *      sel(k1,nlength) = elem_prod((1/(1+mfexp(-sp(2)*(len_bins_m(k1,nlength)-sp(1)) ))),
 *                                 (1-1/(1+mfexp(-sp(4)*(len_bins_m(k1,nlength)-(sp(1)*sp(6)+sp(3))) ))) );
 *      sel += 1.0e-6;
 *      sel /= max(sel);
 *
 *      // age selex
 *      k1=int(value(sp(5)));
 *      sel_a(y,fs,1)(k1,nages) =   elem_prod((1./(1.+mfexp(-sp(2)*(r_ages(k1,nages)-sp(1)) ))),
 *                                          (1.-1./(1.+mfexp(-sp(4)*(r_ages(k1,nages)-(sp(1)*sp(6)+sp(3))) ))) );
 *      sel_a(y,fs,1)(k1,nages) /=max(sel_a(y,fs,1)(k1,nages));
 *      if(k1>0) sel_a(y,fs,1)(0,k1-1)=1.0e-6;
 *
 */
void DialogSelexEquationView::updateDblLogistic() {
    double maxSel = 0;
    double minSel = 0;
    double sel = 0;
    double term1 = 0, term2 = 0;
    double term1log, term2log;
    double min, max;
    int i = 0;
    double infl1 = parameterView->getInput(0);
    double slope1 = parameterView->getInput(1);
    double infl2 = parameterView->getInput(2);
    double slope2 = parameterView->getInput(3);
    int limit = static_cast<int>(parameterView->getInput(4));
    double limitVal = 0;
    int offset = static_cast<int>(parameterView->getInput(5));
    if (infl1 < 0) infl1 = 0;
    if (slope1 < 0) slope1 = 0;
    if (infl2 > xValList.last()) infl2 = xValList.last();

    limit = (limit < 0)? 0: limit < bins.count()? limit: bins.count() - 1;
    limitVal = bins.at(limit);

    if (offset < 0.5) {
        parameterView->setInputValue(5, 0);
        offset = 0;
    }
    else {
        parameterView->setInputValue(5, 1);
        offset = 1;
        // calcs this once
        infl2 += infl1;
        parameterView->setInputValue(2, infl2);
    }

    ascendSeries->clear();
    dscendSeries->clear();
    valSeries->clear();
    firstPoints.clear();

    if (xValList.count() > 0)
    {
        // calc acsending, descending
        for (i = 0; i < xValList.count(); i++) {
            term1 = slope1 * (xValList.at(i) - infl1);
            term2 = slope2 * (xValList.at(i) - infl2);
            term1log = logist(term1);
            term2log = logist(term2);
            ascendSeries->append(QPointF(xValList.at(i), term1log));
            dscendSeries->append(QPointF(xValList.at(i), term2log));
            min = fmin(term1log, term2log);
            max = fmax(term1log, term2log);
            if (minSel > min) minSel = min;
            if (maxSel < max) maxSel = max;
        }
        axisYalt->setRange(minSel, (maxSel));
        // calc selex
        for (i = 0; xValList.at(i) < limitVal; i++) {
            firstPoints.append(QPointF(xValList.at(i), 0.0));
        }
        for (; i < xValList.count(); i++) {
            term1log = ascendSeries->at(i).y();
            term2log = dscendSeries->at(i).y();
            sel = term1log * term2log + .000001; //logist(term1) * logist(term2);
            firstPoints.append(QPointF(xValList.at(i), sel));
        }
        maxSel = maxYvalue(firstPoints);
        for (i = 0; i < firstPoints.count(); i++) {
            sel = (firstPoints.at(i).y()) / maxSel;
            valSeries->append(QPointF(xValList.at(i), sel));
        }
    }
}

// Double logistic with define peak
// size selectivity: 8 double logistic with eight parameters
// age selectivity: 13 and 18 double logistic with smooth transition
// 1=peak, 2=init,  3=infl,  4=slope, 5=final, 6=infl2, 7=slope2
// 1=peak, 2=init,  3=infl,  4=slope, 5=final, 6=infl2, 7=slope2 8=binwidth
void DialogSelexEquationView::dblLogPeak() {
    setLabel(QString("Pattern %1: Double Logistic with IF joins ").arg(
                 QString::number(equationNum)));
    if (parameters->rowCount() == 8) {

        setJoinOne(10);
        setJoinTwo(10);
        setJoinThree(10);

        numParams = 8;
        parameterView->setNumParamsShown(numParams);
        parameterView->setName(0, QString("Peak"));
        parameterView->setType(0, QString("Value"));
        parameterView->setName(1, QString("Init"));
        parameterView->setType(1, QString("Value"));
        parameterView->setName(2, QString("Infl-Up"));
        parameterView->setType(2, QString("Logit"));
        parameterView->setName(3, QString("Slope-Up"));
        parameterView->setType(3, QString("Value"));
        parameterView->setName(4, QString("Final"));
        parameterView->setType(4, QString("Logit"));
        parameterView->setName(5, QString("Infl-Dn"));
        parameterView->setType(5, QString("Logit"));
        parameterView->setName(6, QString("Slope-Dn"));
        parameterView->setType(6, QString("Value"));
        parameterView->setName(7, QString("BinWidth"));
        parameterView->setType(7, QString("Value"));

        showBins(true);
        showJoins(3);
        cht->removeSeries(valSeries);

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

        cht->addAxis(axisX, Qt::AlignBottom);
        cht->addAxis(axisY, Qt::AlignLeft);

        ascendSeries->attachAxis(axisX);
        ascendSeries->attachAxis(axisY);
        dscendSeries->attachAxis(axisX);
        dscendSeries->attachAxis(axisY);
        join1Series->attachAxis(axisX);
        join1Series->attachAxis(axisY);
        join2Series->attachAxis(axisX);
        join2Series->attachAxis(axisY);
        join3Series->attachAxis(axisX);
        join3Series->attachAxis(axisY);

        cht->legend()->setVisible(true);
        cht->legend()->setAlignment(Qt::AlignLeft);

        if (equationNum == 8)
            updateDblLogPeak();
        else if (equationNum == 13)
            updateDblLogIf();
        else
            updateDblLogSmooth();
    }
}

// size selectivity 8
void DialogSelexEquationView::updateDblLogPeak() {
    float maxSelX = 0;
    float binMin = getBinMin();
    float binMax = getBinMax();
    float minVal = binMin + (getBinStep() * getMidBin());
    float maxVal = binMax + (getBinStep() * getMidBin());
    float sel = 0;
    float t1 = 0, t1min, t1max, t1power;
    float t2 = 0, t2min, t2max, t2power;
    float t1range, t2range;
    float one_init, final_one;
    float jn1, jn2, jn3;
    float upselex, downselex;
    int binM, i;
    float join1 = getJoinOne();
    float join2 = getJoinTwo();
    float join3 = getJoinThree();

    float peak = parameterView->getInput(0);
    float init = parameterView->getInput(1);
    float infl_up = parameterView->getInput(2);//logist(par3);
    float slope_up = parameterView->getInput(3);
    float final = parameterView->getInput(4);//logist(par5);
    float infl_dn = parameterView->getInput(5);//logist(par6);
    float slope_dn = parameterView->getInput(6);
    float binwid = parameterView->getInput(7);

    valSeries->clear();
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
        binM = xValList.at(i) + getBinStep() * getMidBin();
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
        valSeries->append(QPointF(binM, sel));
    }
}

// 13 #age selectivity - double logistic
/*
*   t1=0.+(1./(1.+mfexp(-sp(3))))*(sp(1)-0.);    // INFL
    t1min=1./(1.+mfexp(-sp(4)*(0.-t1)))*0.9999999;  // asc value at minage
    t1max=1./(1.+mfexp(-sp(4)*(sp(1)-t1)))*1.00001;  // asc value at peak
    t1power=log(0.5)/log((0.5-t1min)/(t1max-t1min));

    t2=(sp(1)+sp(8))+(1./(1.+mfexp(-sp(6))))*(r_ages(nages)-(sp(1)+sp(8)));    // INFL
    t2min=1./(1.+mfexp(-sp(7)*(sp(1)+sp(8)-t2)))*0.9999;  // asc value at peak+
    t2max=1./(1.+mfexp(-sp(7)*(r_ages(nages)-t2)))*1.00001;  // asc value at maxage
    t2power=log(0.5)/log((0.5-t2min)/(t2max-t2min));
    final=1./(1.+mfexp(-sp(5)));
    k1=int(value(sp(1))); k2=int(value(sp(1)+sp(8)));

    for (a=0; a<=nages; a++)  //calculate the value over ages
    {
      if (a < k1) // ascending limb
      {
        sel_a(y,fs,1,a) = sp(2) + (1. - sp(2)) *
        pow((( 1./(1.+mfexp(-sp(4)*(r_ages(a)-t1))) -t1min ) / (t1max-t1min) ),t1power);
      }
      else if (a > k2) // descending limb
      {
        sel_a(y,fs,1,a) = 1. + (final - 1.) *
        pow((( 1./(1.+mfexp(-sp(7)*(r_ages(a)-t2))) -t2min ) / (t2max-t2min) ),t2power);
      }
      else // at the peak
      { sel_a(y,fs,1,a) = 1.0;}
    }   // end age loop
*/
void DialogSelexEquationView::updateDblLogIf() {

    QString msg(tr("This Pattern is not implemented. "));
    msg.append(tr("\n\nIts use is discouraged, use pattern #18\n"));
    setMessage(msg);
    return;

    float maxSelX = 0;
    float minX = xValList.first();
    float maxX = xValList.last();
    float sel = 0;
    float t1 = 0, t1min, t1max, t1power;
    float t2 = 0, t2min, t2max, t2power;
    float jn1, jn2;
    float upsel, dnsel;
    int binM, i;

    float peak = parameterView->getInput(0);
    float init = parameterView->getInput(1);
    float infl_up = parameterView->getInput(2);
    float slope_up = parameterView->getInput(3);
    float final = parameterView->getInput(4);//logist(par5);
    float infl_dn = parameterView->getInput(5);//logist(par6);
    float slope_dn = parameterView->getInput(6);
    float binwid = parameterView->getInput(7);

    valSeries->clear();
    firstPoints.clear();
    ascendSeries->clear();
    dscendSeries->clear();
    join1Series->clear();
    join2Series->clear();
    join3Series->clear();

    float peakBwd = peak + binwid;

}

void DialogSelexEquationView::updateDblLogSmooth() {
    float minX = xValList.first();
    float maxX = xValList.last();
    float sel = 0;
    float t1 = 0, t1min, t1max, t1power;
    float t2 = 0, t2min, t2max, t2power;
    float jn1, jn2;
    float upsel, dnsel;
    int binM, i;
    float binMax = getBinMax();
    float join1 = getJoinOne();
    float join2 = getJoinTwo();

    float peak = parameterView->getInput(0);
    float init = parameterView->getInput(1);
    float infl_up = parameterView->getInput(2);//logist(par3);
    float slope_up = parameterView->getInput(3);
    float final = parameterView->getInput(4);//logist(par5);
    float infl_dn = parameterView->getInput(5);//logist(par6);
    float slope_dn = parameterView->getInput(6);
    float binwid = parameterView->getInput(7);

    valSeries->clear();
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
        valSeries->append(QPointF(binM, sel));
    }
}

// Double normal - CASAL
// size selectivity 22: using double normal plateau
//  (Similar to CASAL)
//     4 parameters

void DialogSelexEquationView::dblNormCasal() {
    setLabel(QString("Pattern %1: Double Normal (like CASAL)").arg(
                 QString::number(equationNum)));
    if (parameters->rowCount() == 4) {
        numParams = 4;
        parameterView->setNumParamsShown(numParams);
        parameterView->setName(0, QString("Peak1"));
        parameterView->setType(0, QString("Value"));
        parameterView->setName(1, QString("Peak2"));
        parameterView->setType(1, QString("Logist"));
        parameterView->setName(2, QString("UpSlope"));
        parameterView->setType(2, QString("Exp"));
        parameterView->setName(3, QString("DownSlope"));
        parameterView->setType(3, QString("Exp"));

        showBins(true);
        cht->removeSeries(valSeries);

        ascendSeries = new QLineSeries(cht);
        ascendSeries->setPen(QPen(QBrush(Qt::green), 2));
        ascendSeries->setName(QString("Ascend"));
        cht->addSeries(ascendSeries);
        ascendSeries->attachAxis(axisX);
        ascendSeries->attachAxis(axisY);

        dscendSeries = new QLineSeries(cht);
        dscendSeries->setPen(QPen(QBrush(Qt::blue), 2));
        dscendSeries->setName(QString("Descend"));
        cht->addSeries(dscendSeries);
        dscendSeries->attachAxis(axisX);
        dscendSeries->attachAxis(axisY);

        cht->legend()->setVisible(true);
        cht->legend()->setAlignment(Qt::AlignLeft);

        updateDblNormCasal();
    }
}
/*  Double normal with plateau
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
void DialogSelexEquationView::updateDblNormCasal() {
    double xval, sel, peak2;
    double peak1 = parameterView->getInput(0);
    double par2  = parameterView->getInput(1); // logist of par2
    double upslope = parameterView->getInput(2); // exp of par3
    double dnslope = parameterView->getInput(3); // exp of par4

//    float peak = par1;
//    float top = exp(-par2);
//    float upsel = exp(par3);
//    float dnsel = exp(par4);

    peak2 = peak1 + (0.99 * xValList.last() - peak1) * par2;

    valSeries->clear();
    ascendSeries->clear();
    dscendSeries->clear();
    for (int i = 0; i < xValList.count(); i++)
    {
        xval = xValList.at(i);
        sel = exp(-pow((xval - peak1),2)/upslope);
        ascendSeries->append(QPointF(xval, sel));

        sel = exp(-pow((xval - peak2),2)/dnslope);
        dscendSeries->append(QPointF(xval, sel));

        if (xval < peak1)
        {
            //  sel = exp(-pow((xval - peak1),2)/upsel);
            sel = ascendSeries->points().at(i).y();
        }
        else if (xval <= peak2)
        {
            sel = 1.0;
        }
        else
        {
            //  sel = exp(-pow((xval - peak2),2)/dnsel);
            sel = dscendSeries->points().at(i).y();
        }
        valSeries->append(QPointF(xval, sel));
    }
}

void DialogSelexEquationView::dblNormal() {
    setLabel(QString("Pattern %1: Double Normal").arg(
                 QString::number(equationNum)));
    if (parameters->rowCount() == 4) {
        numParams = 4;
        parameterView->setNumParamsShown(numParams);
        parameterView->setName(0, QString("Peak"));
        parameterView->setType(0, QString("Value"));
        parameterView->setName(1, QString("Top"));
        parameterView->setType(1, QString("Peak+Log"));
        parameterView->setName(2, QString("Asc-width"));
        parameterView->setType(2, QString("Exp"));
        parameterView->setName(3, QString("Dsc-width"));
        parameterView->setType(3, QString("Exp"));
    //    setParameterHeaders();

        showBins(true);
        cht->removeSeries(valSeries);

        join1Series = new QLineSeries(cht);
        join1Series->setPen(QPen(QBrush(Qt::darkYellow), 2));
        join1Series->setName(QString("Join1"));
        cht->addSeries(join1Series);
        join1Series->attachAxis(axisX);
        join1Series->attachAxis(axisY);

        join2Series = new QLineSeries(cht);
        join2Series->setPen(QPen(QBrush(Qt::yellow), 2));
        join2Series->setName(QString("Join2"));
        cht->addSeries(join2Series);
        join2Series->attachAxis(axisX);
        join2Series->attachAxis(axisY);

        ascendSeries = new QLineSeries(cht);
        ascendSeries->setPen(QPen(QBrush(Qt::green), 3));
        ascendSeries->setName(QString("Asc-Norm"));
        cht->addSeries(ascendSeries);
        ascendSeries->attachAxis(axisX);
        ascendSeries->attachAxis(axisY);

        dscendSeries = new QLineSeries(cht);
        dscendSeries->setPen(QPen(QBrush(Qt::blue), 3));
        dscendSeries->setName(QString("Desc-Norm"));
        cht->addSeries(dscendSeries);
        dscendSeries->attachAxis(axisX);
        dscendSeries->attachAxis(axisY);

        cht->legend()->setVisible(true);
        cht->legend()->setAlignment(Qt::AlignLeft);

        updateDblNormal();
    }
}

void DialogSelexEquationView::updateDblNormal() {
//    float xval, sel, peak2;
    float peak = parameterView->getInput(0);
    float top  = parameterView->getInput(1);
    float upsel = parameterView->getInput(2);
    float dnsel = parameterView->getInput(3);
    float peak2 = peak + (xValList.last() - peak) * top;
}

void DialogSelexEquationView::dblNormPlateau() {
    setLabel(QString("Pattern %1: Double Normal plateau").arg(
                 QString::number(equationNum)));
    if (parameters->rowCount() == 6) {
        numParams = 6;
        parameterView->setNumParamsShown(numParams);
        setParameterHeaders();
        parameterView->setName(0, QString("Peak"));
        parameterView->setType(0, QString("Value"));
        parameterView->setName(1, QString("Top"));
        parameterView->setType(1, QString("Peak+Log"));
        parameterView->setName(2, QString("Asc-width"));
        parameterView->setType(2, QString("Exp"));
        parameterView->setName(3, QString("Dsc-width"));
        parameterView->setType(3, QString("Exp"));
        parameterView->setName(4, QString("Initial"));
        parameterView->setType(4, QString("Log"));
        parameterView->setName(5, QString("Final"));
        if(equationNum == 23)
        {
            parameterView->setSliderRange(5, 0, 1);
            parameterView->setType(5, QString("Value"));
        }
        else
        {
            parameterView->setSliderRange(5, 0, 100);
            parameterView->setType(5, QString("Log"));
        }
        showBins(true);
        cht->removeSeries(valSeries);

        cht->addSeries(join1Series);
        join1Series->attachAxis(axisX);
        join1Series->attachAxis(axisYalt);
        cht->addSeries(join2Series);
        join2Series->attachAxis(axisX);
        join2Series->attachAxis(axisYalt);
        cht->addSeries(ascendSeries);
        ascendSeries->attachAxis(axisX);
        ascendSeries->attachAxis(axisYalt);
        cht->addSeries(dscendSeries);
        dscendSeries->attachAxis(axisX);
        dscendSeries->attachAxis(axisYalt);

        updateDblNormPlateau();
    }
}

// equation 24
void DialogSelexEquationView::updateDblNormPlateau() {
    float peak  = parameterView->getInput(0);
    float top   = parameterView->getInput(1);
    float upsel = parameterView->getInput(2);
    float dnsel = parameterView->getInput(3);
    float par5  = parameterView->getInput(4);
    float par6  = parameterView->getInput(5);
    float init, final;
//    float peak = par1;
    float binWidth = getBinStep();
    float binMax = getBinMax();
    float peak2 = (peak + binWidth) + (.99*binMax - peak - binWidth) * logist(top);
//    float upsel = exp(par3);
//    float dnsel = exp(par4);
//    float init = 0;//logist(par5);
//    float final = (par6);
    float t1, t1min, t1term;
    float t2, t2min, t2term;
    float apical_selex = 1.0;
    int first, last;
    float jn1, jn2;
    int i;
    float xval = 0, asc = 0, des = 0, sel = 0;

    firstPoints.clear();
    ascendSeries->clear();
    dscendSeries->clear();
    join1Series->clear();
    join2Series->clear();
    valSeries->clear();

    if (xValList.count() > 0)
    {
        if (par5 < -1000)
        {
            first = -1000 - static_cast<int>(par5);
            init = 0;
            t1min = 0;
        }
        else if (par5 > -999)
        {
            first = 0;
            init = logist(par5);
            t1min = exp(-1 * (pow((xValList.at(first)-peak), 2)/upsel));
        }
        if (par6 < -1000)
        {
            last = -1000 - static_cast<int>(par6);
            final = 0;
            t2min = 0;
        }
        else if (par6 > -999)
        {
            last = xValList.count() - 1;
            final = logist(par6);
            t2min = exp(-1 * (pow((xValList.at(last) - peak2), 2)/dnsel));
        }
        for (i = 0; i < xValList.count(); i++)
        {
            xval = xValList.at(i);
            t1 = xval - peak;
            t2 = xval - peak2;
            t1term = exp(-t1 * t1 / upsel);
            t2term = exp(-t2 * t2 / dnsel);
            jn1 = logist(getJoinOne() * t1 / (1. + fabs(t1)));
            jn2 = logist(getJoinTwo() * t2 / (1. + fabs(t2)));
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
            valSeries->append(QPointF(xval, sel));
        }
    }
}

// 20 #age selectivity: double normal with plateau
// case 23 size selectivity double_normal_plateau where final value can be greater than 1.0
// case 24 size selectivity using double_normal_plateau and lots of bells and whistles
// selex 20, 23, 24
void DialogSelexEquationView::dblNormEndpts() {
//    disconnect (this, SIGNAL(numbersUpdated()), this, SLOT(update()));
    setLabel(QString("Pattern %1: Double Normal with end points").arg(
                 QString::number(equationNum)));
    if (parameters->rowCount() == 6) {
        numParams = 6;
        parameterView->setNumParamsShown(numParams);
        parameterView->setName(0, QString("Peak"));
        parameterView->setType(0, QString("Value"));
        parameterView->setName(1, QString("Top"));
        parameterView->setType(1, QString("Logist"));
        parameterView->setName(2, QString("Asc-width"));
        parameterView->setType(2, QString("Exp"));
        parameterView->setName(3, QString("Dsc-width"));
        parameterView->setType(3, QString("Exp"));
        parameterView->setName(4, QString("Initial"));
        parameterView->setType(4, QString("Value"));
        parameterView->setName(5, QString("Final"));
        parameterView->setType(5, QString("Value"));
    //    setParameterHeaders();
    /*    if(equationNum == 23)
        {
            setTitle(QString("Pattern %1: Double Normal plateau").arg(
                         QString::number(equationNum)));
            parameterView->setType(5, QString("Value"));
            parameterView->setSliderRange(5, 0, 1);
        }
        else
        {
            setTitle(QString("Pattern %1: Double Normal with end-points").arg(
                         QString::number(equationNum)));
            parameterView->setType(5, QString("Logist"));
        }*/
        setJoinOne(20);
        setJoinTwo(20);

        showBins(true);
        showJoins(2);
        cht->removeSeries(valSeries);

        join1Series = new QLineSeries(cht);
        join1Series->setPen(QPen(QBrush(Qt::darkYellow), 2));
        join1Series->setName(QString("Join1"));
        join1Series->attachAxis(axisX);
        join1Series->attachAxis(axisY);
        cht->addSeries(join1Series);

        join2Series = new QLineSeries(cht);
        join2Series->setPen(QPen(QBrush(Qt::yellow), 2));
        join2Series->setName(QString("Join2"));
        join2Series->attachAxis(axisX);
        join2Series->attachAxis(axisY);
        cht->addSeries(join2Series);

        ascendSeries = new QLineSeries(cht);
        ascendSeries->setPen(QPen(QBrush(Qt::green), 3));
        ascendSeries->setName(QString("Asc-Norm"));
        ascendSeries->attachAxis(axisX);
        ascendSeries->attachAxis(axisY);
        cht->addSeries(ascendSeries);

        dscendSeries = new QLineSeries(cht);
        dscendSeries->setPen(QPen(QBrush(Qt::blue), 3));
        dscendSeries->setName(QString("Desc-Norm"));
        dscendSeries->attachAxis(axisX);
        dscendSeries->attachAxis(axisY);
        cht->addSeries(dscendSeries);

        valSeries->attachAxis(axisY);
        cht->addSeries(valSeries);

        cht->addAxis(axisX, Qt::AlignBottom);
        cht->addAxis(axisY, Qt::AlignLeft);

        cht->legend()->setVisible(true);
        cht->legend()->setAlignment(Qt::AlignLeft);

        if (equationNum == 23)
            updateDblNormPlateau();
        else
            updateDblNormEndpts();
    }
}

// equations 20 and 24
void DialogSelexEquationView::updateDblNormEndpts() {
    float peak  = parameterView->getInput(0);
    float top   = parameterView->getInput(1);
    float asc_wd = parameterView->getInput(2);
    float dsc_wd = parameterView->getInput(3);
    float par5  = parameterView->getInput(4);
    float par6  = parameterView->getInput(5);
    float binMax = getBinMax();
    float binWidth = getBinStep();

//    float peak = par1;
    float peak2 = (peak + binWidth) + (.99*binMax - peak - binWidth) * top;
    parameterView->setInputValue(1, peak2);
//    float asc_wd = exp(par3);
//    float dsc_wd = exp(par4);
    float init = logist(par5);
    float final = logist(par6);
    float valmin, valmax, valminpow, valmaxpow;
    float limit, upsel, dnsel, jn1, jn2;
    int i;
    float xval = 0, sel = 0;

    firstPoints.clear();
    ascendSeries->clear();
    dscendSeries->clear();
    join1Series->clear();
    join2Series->clear();
    valSeries->clear();

    if (xValList.count() > 0)
    {
        limit = (par5 < -1000)? (-1000 - par5): -1;
        valmin = getBinMin() + getBinStep() * getMidBin();
        valmax = getBinMax() + getBinStep() * getMidBin();
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

            jn1 = logist(getJoinOne() *(xval-peak)/(1 + fabs(xval-peak)));
            jn2 = logist(getJoinTwo() *(xval-top)/(1 + fabs(xval-top)));

            if (xval > limit)
                sel = upsel * (1-jn1) + jn1 * ((1-jn2) + (dnsel * jn2));
            else
                sel = 0.000001;

            ascendSeries->append(xval, upsel);
            dscendSeries->append(xval, dnsel);
            join1Series->append(xval, jn1);
            join2Series->append(xval, jn2);
            valSeries->append(xval, sel);
        }
    }
}

// size selectivity 25: exponential-logistic
// age selectivity 26: exponential logistic
//   3 Parameters: width, peak, and tail height
void DialogSelexEquationView::expLogistic() {
    setLabel(QString("Pattern %1: Exponential-Logistic").arg(
                 QString::number(equationNum)));
    if (parameters->rowCount() == 3) {
        numParams = 3;
        setParameterHeaders();
        parameterView->setNumParamsShown(numParams);
        parameterView->setName(0, QString("Width"));
        parameterView->setType(0, QString("Value"));
        parameterView->setName(1, QString("Peak"));
        parameterView->setType(1, QString("Trans"));
        parameterView->setName(2, QString("Tail Ht"));
        parameterView->setType(2, QString("Value"));
        showBins(true);
        showJoins(0);

        if (parameters->rowCount() == 3)
            updateExpLogistic();
    }
}

/* // size selex 25
   peak = len_bins_m(1) + sp(2)*(len_bins_m(nlength)-len_bins_m(1));
   for (j=1;j<=nlength;j++)
   {
     sel(j) = mfexp(sp(3)*sp(1)*(peak-len_bins_m(j)))/
              (1.0-sp(3)*(1.0-mfexp(sp(1)*(peak-len_bins_m(j)))));
   }

   // age selex 26
  peak = sp(2)*r_ages(nages);
  for (a=0;a<=nages;a++)
    {sel_a(y,fs,1,a) = mfexp(sp(3)*sp(1)*(peak-r_ages(a)))/(1.0-sp(3)*(1.0-mfexp(sp(1)*(peak-r_ages(a)))));}
*/
void DialogSelexEquationView::updateExpLogistic() {
    int xRange = static_cast<int>(xValList.last() - xValList.first());
    double yVal = 0;
    double temp = 0;
    double xVal = 0;
    double par1 = parameterView->getValue(0);
    double par2 = parameterView->getInput(1);
    double par3 = parameterView->getInput(2);

    double peak = xValList.first() + par2 * (xRange);
    parameterView->setInputValue(1, peak);

    valSeries->clear();

    for (int i = 0; i < xValList.count(); i++)
    {
        xVal = xValList.at(i);
        //sel_a(y,fs,1,a) = mfexp(sp(3)*sp(1)*(peak-r_ages(a)))/(1.0-sp(3)*(1.0-mfexp(sp(1)*(peak-r_ages(a)))));
        temp = 1.0 - par3 * (1 - exp(par1 * (peak - xVal)));
        yVal = exp(par3 * par1 * (peak - xVal))/temp;
        valSeries->append(QPointF(xVal, yVal));
    }
}

/* Returns the logistic of the requested value
 * 1/(1 + exp(-value))
float DialogSelexEquationView::logist(double value)
{
    if (value < -1000)
        return 0.0;
    if (value > 1000)
        return 1.0;
    float temp = exp(-value);
    return (1./(1. + temp));
}*/

// 14 #age selectivity - separate parm for each age
void DialogSelexEquationView::eachAge () {
    setLabel(QString("Pattern %1: Revise Age").arg(
                 QString::number(equationNum)));
/*    binMin = xValList.at(0);
    binMax = binMin + (binWidth * selex->getNumAges());
    ui->spinBox_bins_min->setValue(binMin);
    ui->spinBox_bins_max->setValue(binMax);*/

    if (parameters->rowCount() >= bins.count()) {
        numParams = bins.count() + 1; //parameters->rowCount();
        parameterView->setNumParamsShown(numParams);
        showBins(true);
        showJoins(0);
        cht->removeSeries(valSeries);

        setParameterHeaders();

        axisYalt->setTitleText("Value at age (blue)");
        cht->addAxis(axisYalt, Qt::AlignRight);

        ascendSeries->setPen(QPen(QBrush(Qt::blue), 2));
        cht->addSeries(ascendSeries);
        ascendSeries->attachAxis(axisX);
        ascendSeries->attachAxis(axisYalt);

        if (parameters->rowCount() > xValList.count())
            updateEachAge();
    }
}

void DialogSelexEquationView::updateEachAge () {
    float parm = 0.;
    float asc = 0.;
    float sel = 0.;
    float temp, max = 0, min = 0;
    float binWidth = getBinStep();
    float age = getBinMin() + getMidBin() * binWidth;
    QString msg("");

    valSeries->clear();
    firstPoints.clear();
    if (numParams < (bins.count() + 1))
    {
        msg = QString("Fewer parameters than number of ages +1.");
        setMessage(msg);
    }
    else if (numParams > (bins.count() + 1))
    {
        msg = QString("More parameters than number of ages +1.");
        setMessage(msg);
    }
    else
    {
        age = 0;
        firstPoints.append(QPointF(0, parameterView->getInput(0)));
        for (int i = 1; i < parameters->rowCount(); i++)
        {
            age = bins.at(i-1);
            parm = parameterView->getInput(i);
            if (parm < -5) parm = -5.;
            if (parm > 9) parm = 9.;
            firstPoints.append(QPointF(age, parm));
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
            valSeries->append(QPointF(firstPoints.at(i).x(), sel));
        }
        min = minYvalue(ascendSeries->points());
        max = maxYvalue(ascendSeries->points());
        axisYalt->setRange(min, max);
    }
}

// 17 #age selectivity: each age has parameter as random walk
//    transformation is selex=exp(parm); some special codes
void DialogSelexEquationView::randomWalk (float scale) {
    setLabel(QString("Pattern %1: Random Walk").arg(
                 QString::number(equationNum)));
    if (parameters->rowCount() > bins.count()) {
        numParams = bins.count() + 1;
        parameterView->setNumParamsShown(numParams);
        showBins(true);
        showJoins(0);
        cht->removeSeries(valSeries);

        setParameterHeaders();

        axisYalt->setTitleText("Use Parm (blue)");
        cht->addAxis(axisYalt, Qt::AlignRight);

        ascendSeries->setPen(QPen(QBrush(Qt::blue), 3));
        cht->addSeries(ascendSeries);
        ascendSeries->attachAxis(axisX);
        ascendSeries->attachAxis(axisYalt);

        updateRandomWalk(scale);
    }
}

void DialogSelexEquationView::updateRandomWalk (float scale) {
    float parmval;
    float lastSel = 0.0;
    float temp = -999;
    QList<float> parms;
    float binMax = getBinMax();
    int scaleOffset = static_cast<int>(scale);
    int firstage = 0;
    int lastage = bins.count();
    int i = 0;

    ascendSeries->clear();
    valSeries->clear();
    firstPoints.clear();

    // set last age, special or last bin
    if (special != 0)
        lastage = abs(special);
    if (lastage > bins.count())
        lastage = bins.count();

    // tempvec_a
    firstPoints.append(QPointF(0.0, 0.0));

    for (int a = 1; a < numParams; a++) {
        //  with use of -999, lastsel stays constant until changed, so could create a linear change in ln(selex)
        parmval = parameterView->getValue(a + scaleOffset);
        if (parmval > -999.) {
            lastSel = parmval;
        }
        else {
            parameterView->setInputValue(a, lastSel);
        }
        firstPoints.append(QPointF(bins.at(a-1), firstPoints.at(a-1).y() + lastSel));
    }
    axisYalt->setRange(minYvalue(firstPoints), maxYvalue(firstPoints));
    ascendSeries->append(firstPoints);
    cht->addSeries(ascendSeries);
    ascendSeries->attachAxis(axisX);
    ascendSeries->attachAxis(axisYalt);

    // if no scaling
    if (scaleOffset == 0) {
        temp = maxYvalue(firstPoints);
    }
    else {
        int lowBin = parameterView->getInput(0);
        int hiBin = parameterView->getInput(1);

        if (lowBin < 0) {
            lowBin = 0;
            parameterView->setSlider(0, 0);
        }
        if (hiBin > bins.count()) {
            hiBin = bins.count();
            parameterView->setSlider(1, hiBin);
        }
        if (hiBin < lowBin) {
            hiBin = lowBin;
            parameterView->setSlider(1, lowBin);
        }
        if (lowBin > hiBin) {
            lowBin = hiBin;
            parameterView->setSlider(0, hiBin);
        }

        temp = aveYvalue(firstPoints, lowBin, hiBin);
    }

    // find first age bin
    i = 0;
    if (firstPoints.at(i).y() == -1000)
    {
        while (parameterView->getInput(i) == -1000)
        {
            firstage = i;
            i++;
//            valSeries->append(firstPoints.at(i++).x(), 0.0);
        }
    }

    valSeries->append(firstPoints.at(0).x(), 0.0);

    for (i = 1; i < firstage; i++)
        valSeries->append(firstPoints.at(i).x(), 0.0);

    for (; i <= lastage; i++)
        valSeries->append(firstPoints.at(i).x(), exp(firstPoints.at(i).y()-temp));


    if (special > 0) {
        for (int i = lastage + 1; i < firstPoints.count(); i++)
            valSeries->append(firstPoints.at(i).x(), valSeries->points().at(lastage).y());
    }
    else {
        for (int i = lastage + 1; i < firstPoints.count(); i++)
            valSeries->append(firstPoints.at(i).x(), 0.0);
    }
}

/*
    float useparm = 0.;
    float sel = 0.;
    float parm = 0.;
    float cumparm = 0;
    float rateofchange = 0;
    int lastage = static_cast<int>(binMax + .5);
    int lastageIndex = xValList.count() - 1;

    for (int i = 0; i < xValList.count(); i++) {
        if (xValList.at(i) > lastage) {
            lastageIndex = i - 1;
            break;
        }
    }

    parm = parameterView->getInput(0);
    if (parm == -1000)
    {
        parm = 0.0;
    }
    cumparm = useparm = parm;
    if (xValList.count() > 0)
    {
    valSeries->append(QPointF(xValList.at(0), parm));

    parms.append(parm);
    ascendSeries->append(QPointF(xValList.at(0), useparm));
    firstPoints.append(QPointF(xValList.at(0), cumparm));

    for (int i = 1; i <= lastageIndex; i++)
    {
        parm = parameterView->getInput(i);
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

    if (scale < 2)
    {
        temp = maxYvalue(firstPoints);
    }
    else
    {
        int low_bin;
        int high_bin;
        float total = 0;
        if (minBin < 0)
        {
            minBin = 0;
        }
        if (maxBin > binMax)
        {
            maxBin = static_cast<int>(binMax);
        }
        if (maxBin < minBin)
            maxBin = minBin;
        if (minBin > maxBin)
            minBin = maxBin;

        low_bin = xValList.indexOf(minBin);
        high_bin = xValList.indexOf(maxBin);
//        sp(1) = low_bin;
//        sp(2) = high_bin;
//        temp = mean(tempvec_a(low_bin,high_bin));

        temp = aveYvalue(firstPoints, 1, maxBin);
    }

    for (int i = 1; i < firstPoints.count(); i++)
    {
        int next = i + 1 + scale;
        if ((next < parms.count()) && (parms.at(next) == -1000))
            sel = 0.0;
        else
            sel = exp(firstPoints.at(i).y() - temp);
        valSeries->append(QPointF(xValList.at(i), sel));
    }

    for (int i = firstPoints.count(); i < xValList.count(); i++)
    {
        if (special > 0)
            valSeries->append(QPointF(xValList.at(i), valSeries->points().at(i-1).y()));
        else
            valSeries->append(QPointF(xValList.at(i), 0.0));
    }
//    updateLinearExp(scale);
    }/
}*/

// case 16 Coleraine - Gaussian age selectivity
void DialogSelexEquationView::coleGauss () {
    setLabel(QString("Pattern %1: Coleraine single Gaussian").arg(
                 QString::number(equationNum)));

    if (parameters->rowCount() == 2) {
    numParams = 2;
    parameterView->setNumParamsShown(numParams);
//    disconnect (this, SIGNAL(numbersUpdated()), this, SLOT(update()));
//    parameterView->setSliderRange(0, -5, 5);
//    parameterView->setSliderRange(0, -5, 40);
//    connect (this, SIGNAL(numbersUpdated()), this, SLOT(update()));

    parameterView->setName(0, QString("Age limit"));
    parameterView->setType(0, QString("Logist"));
    parameterView->setName(1, QString("Decline scaling"));
    parameterView->setType(1, QString("Value"));
    setParameterHeaders();
    showBins(true);

    updateColeGauss();
    }
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
void DialogSelexEquationView::updateColeGauss() {
    int i;
    float temp = 0;
    float sel = 0;
    float par1 = parameterView->getInput(0);
    float par2 = parameterView->getInput(1);

    valSeries->clear();

    temp = static_cast<int>(par1 * xValList.count());

    for (i = 0; i < temp; i++)
    {
        sel = exp(-1*pow(xValList.at(i) - temp, 2) / exp(par2));
        valSeries->append(QPointF(xValList.at(i), sel));
    }
    for (; i < xValList.count(); i++)
    {
        valSeries->append(QPointF(xValList.at(i), 1.0));
    }
}

// case 27 size selectivity using cubic spline
//  first N parameters are the spline knots; second N parameters are ln(selex) at the knot
//  scaled by max(raw vector) to achieve scale to 1.0
// case 42 size and selectivity using cubic spline
//  scaled by average of values at low bin through high bin
void DialogSelexEquationView::cubicSpline(float scale) {
    int num = 2*special + 3 + scale;
    if (parameters->rowCount() == num) {
        numParams = parameters->rowCount();
        parameterView->setNumParamsShown(numParams);
        setParameterHeaders();
        if (scale > 0)
        {
            num -= 5;
            parameterView->setName(0, QString("ScaleLo"));
            parameterView->setType(0, QString("Value"));
            parameterView->setName(1, QString("ScaleHi"));
            parameterView->setType(1, QString("Value"));
            parameterView->setName(2, QString("Setup"));
            parameterView->setType(2, QString("Integr"));
            parameterView->setName(3, QString("GradLo"));
            parameterView->setType(3, QString("Value"));
            parameterView->setName(4, QString("GradHi"));
            parameterView->setType(4, QString("Value"));
        }
        else
        {
            num -= 3;
            parameterView->setName(0, QString("Setup"));
            parameterView->setType(0, QString("Integr"));
            parameterView->setName(1, QString("GradLo"));
            parameterView->setType(1, QString("Value"));
            parameterView->setName(2, QString("GradHi"));
            parameterView->setType(2, QString("Value"));
        }

        setLabel(QString("Pattern %1: Cubic Spline %2 nodes").arg(
                     QString::number(equationNum),
                     QString::number(special)));
//        setParameterHeaders();
        showBins(true);
        cht->removeSeries(valSeries);

//        ascendSeries = new QLineSeries(cht);
        ascendSeries->setPen(QPen(QBrush(Qt::blue), 3));
        cht->addSeries(ascendSeries);

//        ptSeries = new QScatterSeries(cht);
        ptSeries->setPen(QPen(QBrush(Qt::blue), 3));
        cht->addSeries(ptSeries);

        axisYalt->setTitleText("Cubic Nodes (blue)");
        cht->addAxis(axisYalt, Qt::AlignRight);
        ptSeries->attachAxis(axisX);
        ptSeries->attachAxis(axisYalt);
        ascendSeries->attachAxis(axisX);
        ascendSeries->attachAxis(axisYalt);

        if (num > 5 && (num % 2) == 0)
        {
            updateCubicSpline(scale);
        }
    }
}

void DialogSelexEquationView::updateCubicSpline(float scale) {
    double xval = 0, yval = 0;
    int start = 3 + static_cast<int>(scale);
    int num = parameters->rowCount() - start;
    int size;
    float scaleLo = -1, scaleHi = -1;
    int lobin = -1, hibin = -1;
    int setup = 0;
    double gradLo = 0;
    double gradHi = 0;
    double temp = 1;
    int i;
    QString msg;
    // QMap automatically sorts on key.
//    QMap<float, float> pts;
    double maxVal, minVal, max, min;
    double altMaxVal, altMinVal, altMax, altMin;

    if ((num % 2) == 1 || num < 6)
    {
        msg = QString(tr("Incorrect number of parameters. Check manual.\n"));
        msg.append(QString("It needs to be odd and at least 6 + 3 [+ 2(if scaled)]."));
        setMessage(msg);
    }
    else
    {
        size = static_cast<int>(num / 2);
        std::vector<double> X(size), Y(size);
        tk::spline cubicspl;

        i = 0;
        if (scale > 1)
        {
            lobin = static_cast<int>(parameterView->getInput(i++));
            hibin = static_cast<int>(parameterView->getInput(i++));
            if (lobin < 1)
                lobin = 1;
            if (hibin > bins.count())
                hibin = bins.count();
            if (hibin < lobin)
                hibin = lobin;
            if (lobin > hibin)
                lobin = hibin;
            parameterView->setInputValue(0, lobin);
            parameterView->setInputValue(1, hibin);
            scaleLo = bins.at(lobin-1);
            scaleHi = bins.at(hibin-1);
        }
        setup  = static_cast<int>(parameterView->getInput(i++));
        gradLo = parameterView->getInput(i++);
        gradHi = parameterView->getInput(i++);

//        start = i;

        switch (setup)
        {
        case 0:
            for (i = 0; i < size; i++)
                X[i] = parameterView->getInput(start++);
            break;
        case 1:
        case 2:
            float x0 = bins.first();
            float xn = bins.last();
            int size_1 = size - 1;
            float dist = xn - x0;
            float incr = dist / (size_1);
            X[0] = x0 + (2.5 * dist);
//            X[1] = (x0 + (2.5 * dist) / 100);
            X[size_1] = (x0 + (97.5 * dist));// / 100);
            incr = (X[size_1] - X[0]) / size_1;
            for (i = 1; i < size_1; i++)
                X[i] = (X[i-1] + incr);
//            for (i = 0; i < num; i++)
//                parameters->setItem(start++, 2, new QStandardItem(QString::number(X[i])));
        }

        for (i = 0; i < size; i++)
        {
            Y[i] = parameterView->getInput(start++);
        }
//        for (i = 0; i < size; i++)
//            pts.insert(X[i], Y[i]);

        ptSeries->clear();
//        QMapIterator<float, float> mi(pts);
//        i = 0;
//        while (mi.hasNext())
 //       {
 //           mi.next();
 //           X[i] = mi.key();
 //           Y[i] = mi.value();
        for (i = 0; i < size; i++)
            ptSeries->append(X[i], Y[i]);
 //           i++;
 //       }


        // currently it is required that X is already sorted
        cubicspl.set_boundary(tk::spline::first_deriv, gradLo,
                              tk::spline::first_deriv, gradHi);
        cubicspl.set_points(X, Y);

//        cht->removeSeries(valSeries);
        firstPoints.clear();
        valSeries->clear();
        ascendSeries->clear();

        for (i = 0; i < xValList.count(); i++)
        {
            xval = xValList.at(i);
            if (xval > X[size - 1])
                break;
            yval = cubicspl(xval);
            firstPoints.append(QPointF(xval, yval));
        }
        for ( ; i < xValList.count(); i++)
        {
            xval = xValList.at(i);
            firstPoints.append(QPointF(xval, yval));
        }
        ascendSeries->append(firstPoints);

        altMinVal = minYvalue (firstPoints);
        altMaxVal = maxYvalue (firstPoints);
//        range = maxVal - minVal;
//        if (range < 1.0)
//            minVal = maxVal - 1.0;
        altMax = altMinVal + ((altMaxVal - altMinVal) * 1.2);
        axisYalt->setRange(altMinVal, altMaxVal);
        altMaxVal -= fabs(altMinVal);

/*        for (i = 0; i < firstPoints.count(); i++)
        {
            yval = firstPoints.at(i).y();
            firstPoints[i].setY(yval - altMinVal);
        }
        maxVal = maxYvalue(firstPoints);*/

        if (scale > 0)
        {
            temp = fabs(aveYvalue(firstPoints, scaleLo, scaleHi));
        }
        else
        {
            temp = maxYvalue(firstPoints);
        }

        for (i = 0; i < firstPoints.count(); i++)
            firstPoints[i].setY(exp(firstPoints.at(i).y() - temp));
/*        for (i = 0; i < firstPoints.count(); i++)
        {
            yval = firstPoints[i].y();
            if (yval > 1.0)
                firstPoints[i].setY(1.0);
        }*/
//        maxVal = maxYvalue (firstPoints);
//        max = maxVal * 1.2;
//        axisY->setRange(0.0, max);
        valSeries->append(firstPoints);
//        cht->addSeries(valSeries);
//        valSeries->attachAxis(axisY);
//        if (scale > 1)
//            axisYalt->setRange(altMinVal, (altMinVal + max));
    }
}

// 44 like age selex 17 but with separate parameters for males and with revised controls
void DialogSelexEquationView::twoSexRandomWalk() {
    setLabel(QString("Pattern %1: Random Walk - female/male").arg(
                 QString::number(equationNum)));

    int num = xValList.count() + genders + 2;
    if (parameters->rowCount() == num) {
        numParams = num;
        parameterView->setNumParamsShown(numParams);
        showBins(true);
        cht->removeSeries(valSeries);

        setParameterHeaders();

        resetChart();
        axisY->setRange(0, 1.2);

        cht->addAxis(axisX, Qt::AlignBottom);
        cht->addAxis(axisY, Qt::AlignLeft);

        axisYalt->setTitleText("Use Parm (blue)");
        axisYalt->setRange(0, 1.0);
        cht->addAxis(axisYalt, Qt::AlignRight);

        // female ln(selex)
        ascendSeries = new QLineSeries(cht);
        ascendSeries->setPen(QPen(QBrush(Qt::blue), 2));
        ascendSeries->setName(QString("Fem ln(slx)"));
        cht->addSeries(ascendSeries);
        ascendSeries->attachAxis(axisX);
        ascendSeries->attachAxis(axisYalt);

        // female selex
        valSeries->setName(QString("Fem slx"));

        // male ln(selex)
        dscendSeries = new QLineSeries(cht);
        dscendSeries->setPen(QPen(QBrush(Qt::green), 2));
        dscendSeries->setName(QString("Mal ln(slx)"));
        cht->addSeries(dscendSeries);
        dscendSeries->attachAxis(axisX);
        dscendSeries->attachAxis(axisYalt);

        // male selex
        join3Series = new QLineSeries(cht);
        join3Series->setPen(QPen(QBrush(Qt::magenta), 5));
        join3Series->setName(QString("Mal slx"));
        cht->addSeries(join3Series);
        join3Series->attachAxis(axisX);
        join3Series->attachAxis(axisY);

        ascendSeries->attachAxis(axisX);
        ascendSeries->attachAxis(axisYalt);

        updateTwoSexRandom();
    }
}

void DialogSelexEquationView::updateTwoSexRandom() {
    double useparm = 0.;
    double sel = 0.;
    double parm = 0.;
    double cumparm = 0;
    double temp = 0, mean = 0, max = 0;
    QList<double> parms;
//    int age = binMin;
    int lastage = getBinMax();
    int lastageIndex = xValList.count() - 1;
    int minAge = static_cast<int>(parameterView->getInput(0));
    int maxAge = static_cast<int>(parameterView->getInput(1));
    ascendSeries->clear();
    dscendSeries->clear();
    valSeries->clear();
    join3Series->clear();
    firstPoints.clear();

    if (special != 0)
        lastage = abs(special);
    for (int i = 0; i < xValList.count(); i++) {
        if (xValList.at(i) > lastage) {
            lastageIndex = i - 1;
            break;
        }
    }

    parm = parameterView->getInput(0);
    if (static_cast<int>(parm) == -1000)
    {
        parm = 0.0;
    }
    cumparm = useparm = parm;
    if (xValList.count() > 0)
    {
    valSeries->append(QPointF(xValList.at(0), parm));

    parms.append(parm);
    ascendSeries->append(QPointF(xValList.at(0), useparm));
    firstPoints.append(QPointF(xValList.at(0), cumparm));

    for (int i = 1; i <= lastageIndex; i++)
    {
        parm = parameterView->getInput(i);
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
        double total = 0;
        if (minAge < 0)
        {
            minAge = 0;
        }
        if (maxAge > getBinMax())
        {
            maxAge = getBinMax();
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
        if ((next < parms.count()) && (static_cast<int>(parms.at(next)) == -1000))
            sel = 0.0;
        else
            sel = exp(firstPoints.at(i).y() - temp);
        valSeries->append(QPointF(xValList.at(i), sel));
    }

    for (int i = firstPoints.count(); i < xValList.count(); i++)
    {
        if (special > 0)
            valSeries->append(QPointF(xValList.at(i), valSeries->points().at(i-1).y()));
        else
            valSeries->append(QPointF(xValList.at(i), 0.0));
    }
    }
}

// 45 similar to selex 14 but with separate parameters for males and with revised controls
void DialogSelexEquationView::twoSexEachAge() {
    setLabel(QString("Pattern %1: Revise Age - female/male").arg(
                 QString::number(equationNum)));
    int num = bins.count() + genders + 2;
    if (parameters->rowCount() == num) {

    numParams = num;
    parameterView->setNumParamsShown(numParams);
//    disconnect (this, SIGNAL(numbersUpdated()), this, SLOT(update()));
    parameterView->setSliderRange(0, xValList.first(), xValList.last());
    parameterView->setSliderRange(1, xValList.first(), xValList.last());
    parameterView->setSliderRange(2, xValList.first(), xValList.last());
    parameterView->setSliderRange(3, -1.0, 2.0);
//    connect (this, SIGNAL(numbersUpdated()), this, SLOT(update()));
    parameterView->setName(0, QString("First age non-zero"));
    parameterView->setType(0, QString("Value"));
    parameterView->setName(1, QString("First age peak"));
    parameterView->setType(1, QString("Value"));
    parameterView->setName(2, QString("Last age peak"));
    parameterView->setType(2, QString("Value"));
    parameterView->setName(3, QString("Male ln(ratio)"));
    parameterView->setType(3, QString("Value"));

    showBins(true);
    showJoins(0);
    setParameterHeaders();

//    connect (parameters, SIGNAL(dataChanged()), this, SLOT(parametersChanged()));
    cht->removeSeries(valSeries);

    // female ln(selex)
    ascendSeries = new QLineSeries(cht);
    ascendSeries->setPen(QPen(QBrush(Qt::blue), 2));
    ascendSeries->attachAxis(axisX);
    ascendSeries->attachAxis(axisYalt);
    cht->addSeries(ascendSeries);

    // male ln(selex)
    dscendSeries = new QLineSeries(cht);
    dscendSeries->setPen(QPen(QBrush(Qt::green), 2));
    cht->addSeries(dscendSeries);

    // male selex
    join3Series = new QLineSeries(cht);
    join3Series->setPen(QPen(QBrush(Qt::magenta), 3));
    cht->addAxis(axisX, Qt::AlignBottom);
    cht->addAxis(axisY, Qt::AlignLeft);


    axisYalt->setTitleText("Parm val (blue)");
    cht->addAxis(axisYalt, Qt::AlignRight);

    updateTwoSexEachAge();

    // female selex
    cht->addSeries(valSeries);
    // male selex
    cht->addSeries(join3Series);
    }
}

void DialogSelexEquationView::updateTwoSexEachAge() {
    double parmfm = 0.;
    double parmml = 0.;
    double temp = 0.;
    double sel = 0.;
    double max = 0, min = 0;
    double binMin = getBinMin();
    float binMax = getBinMax();
    double binWidth = getBinStep();
    double binMidWidth = getMidBin();
    double age = binMin + binMidWidth * binWidth;
    int firstage = 0, lastage = static_cast<int>(xValList.last());
    int i, j, minbin, maxbin;
    int offset = genders + 2;
    int maleoffset = offset + special;
    int endoffset = maleoffset + special;
    double par;
    int par1 = static_cast<int>(parameterView->getInput(0));
    int par2 = static_cast<int>(parameterView->getInput(1));
    int par3 = static_cast<int>(parameterView->getInput(2));
    double mlratio = 0;
    if (genders > 1)
        mlratio = parameterView->getInput(3);
    QString msg("");

    firstPoints.clear();
    ascendSeries->clear();
    valSeries->clear();
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
        par = parameterView->getInput(j);
        if (par > -999 || par < -999)
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
//    ascendSeries->append(firstPoints);

    temp = 8. - aveYvalue(firstPoints, minbin, maxbin);
    sel = 0.;

    for (int i = 0; i < firstPoints.count(); i++)
    {
        sel = logist(firstPoints.at(i).y() + temp);
        valSeries->append(QPointF(firstPoints.at(i).x(), sel));
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
            par = QString(parameters->getRowData(j + special).at(2)).toDouble();
            if (static_cast<int>(par) == -1000)
                parmml = QPointF(ascendSeries->points().at(i)).x();
            else if (static_cast<int>(par) != -999)
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
        valSeries->append(firstPoints);

        temp = 8. - aveYvalue(firstPoints, minbin, maxbin);
        sel = 0.;

        for (int i = 0; i < firstPoints.count(); i++)
        {
            sel = logist(firstPoints.at(i).y() + temp);
            join3Series->append(QPointF(firstPoints.at(i).x(), sel));
        }
    }
}



