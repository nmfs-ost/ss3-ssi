#include "dialogselectivityequationview.h"

#include <QMessageBox>
#include <QSplineSeries>

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

//    setJoinOne(10);
//    setJoinTwo(10);
//    setJoinThree(10);

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
//    disconnectAll();
//    disconnect (selex, SIGNAL(setupChanged(QStringList)), this,
//                SLOT(changedSelex(QStringList)));
    selex = slx;
//    setParameters(selex->getParameterModel());
    restoreAll();
//    connect (selex, SIGNAL(setupChanged(QStringList)), this,
//                 SLOT(changedSelex(QStringList)));
//    connectAll();
}

void DialogSelexEquationView::changingSelex() {
//    disconnect(parameters, SIGNAL(dataChanged()), this, SLOT(parametersChanged()));
}

void DialogSelexEquationView::changedSelex(QStringList &ql) {
    Q_UNUSED(ql);
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

void DialogSelexEquationView::setParameterHeaders(int scale) {
    QString fltNum(QString::number(fleetNum));
    QString fltTyp, selexTyp;
    Fleet::FleetType ft = fleet->getType();
    int i = scale == 2? 2: 0;
    if     (ft == Fleet::Fishing ||
            ft == Fleet::Bycatch)
        fltTyp = QString("fishery");
    else if (ft == Fleet::Survey)
        fltTyp = QString("survey");
    if (slxType == Age)
        selexTyp = QString("Age");
    else selexTyp = QString("Size");

    if (i == 2) {
        parameterView->setSliderRange(0, 1, bins.count());
        parameters->setRowHeader(0, QString("%1Sel_Scale%1BinLo").arg(selexTyp));
        parameterView->setSliderRange(1, 1, bins.count());
        parameters->setRowHeader(1, QString("%1Sel_Scale%1BinHi").arg(selexTyp));
    }
    for (; i < parameters->rowCount(); i++)
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
    if (fleet == nullptr || selex == nullptr || bins.isEmpty())
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

        axisY->setTitleText(QString("Selectivity (red)"));
        axisY->setRange(0, 1.2);

        axisYalt->setRange(0, 1.2);

        valSeries->setName("Selex (red)");
        valSeries->setPen(QPen(QBrush(Qt::red), 5));


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
            if (special >= 3)
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
            if (special >= 3)
                cubicSpline(2.);
            break;

        case 43: // case 43: size selectivity - non-parametric scaled by average of values at low bin through high bin
//            blank (NotYet, 43, 0, QString("This equation is in work."));
            linearScaled();
            break;

        case 44: // case 44: age selectivity - like age selex 17 but with separate parameters for males and with revised controls
//            blank (NotYet, 44, 0, QString("IN WORK."));
            twoSexRandomWalk();
            break;

        case 45: // case 45: age selectivity - like age selex 14 but with separate parameters for males and with revised controls
//            blank (NotYet, 44, 0, QString("IN WORK."));
            twoSexEachAge();
            break;

        default:
            blank(NotUsed, equationNum);
            break;

        }
        updating = false;
    }
//    cht->addSeries(valSeries);
//    valSeries->attachAxis(axisY);
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
            updateLinearScaled(0);
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
            updateLinearScaled(2.);
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
//    chartview->setVisible(false);

    if (equationNum == 11) {
        constantRange();
    }
    else  {
        numParams = 0;
        parameterView->setNumParamsShown(numParams);
        updateConstant(val);
    }
//    chartview->setVisible(true);
}

// Size selectivity 0, age selectivity 10 - 0 parameters
void DialogSelexEquationView::updateConstant (double val) {
    double start = 0; // equation 0
    double end = 2;

    if (!bins.isEmpty())
    {
        if (equationNum == 10) {
            start = bins.at(0);
        }
        else {
        }
        end = bins.last();
        updateConstant(val, start, end);
    }
}

// case 11: Constant age-specific selex for specified age range - 2 parameters
void DialogSelexEquationView::constantRange () {
    QString type;
    if (selex->getType() == Age)
        type = QString("age");
    else
        type = QString("size");

    setLabel(QString("Pattern %1: Constant selectivity 1.0 within range").arg(equationNum));

    if (parameters->rowCount() == 2) {
        numParams = 2;
        parameterView->setNumParamsShown(numParams);
        parameterView->setName(0, QString("Lo %1").arg(type));
        parameterView->setType(0, QString("Integr"));
        parameterView->setName(1, QString("Hi %1").arg(type));
        parameterView->setType(1, QString("Integr"));

        updateConstantRange();
    }
}

// 11 #Constant age-specific selex for specified age range
void DialogSelexEquationView::updateConstantRange (double val) {
    double loBin = parameterView->getInput(0);
    double hiBin = parameterView->getInput(1);
    int first, last;
//    double start; = bins.at(first);
//    double end; = bins.at(last);
    int lastbin = bins.count();
    if (lastbin > 1) {
        if(checkScaleSliders(0, 1, bins, loBin, hiBin)) {
            first = static_cast<int>(loBin);
            last  = static_cast<int>(hiBin);

            updateConstant(val, bins.at(first-1), bins.at(last-1));
        }
    }
}

// general constant graph
void DialogSelexEquationView::updateConstant (double val, double first, double last) {
    double start = 0;
    double end = bins.isEmpty()? xValList.last(): bins.last();


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
    bool okay = true;
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
            double lo = parameterView->getInput(0);
            double hi = parameterView->getInput(1);
            okay = checkScaleSliders(0, 1, bins, lo, hi);

            parameterView->setSliderRange(0, 1, bins.count());
            parameterView->setSliderRange(1, 1, bins.count());
            msg.append(QString(" between Lo and Hi bins"));
            setLabel(msg);
            parameterView->setName(0, QString("Lo bin"));
            parameterView->setType(0, QString("Integr"));
            parameterView->setName(1, QString("Hi bin"));
            parameterView->setType(1, QString("Integr"));
        }
        if (okay)
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
        if (par1 < 1 || par1 > bins.count()){
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
        cht->removeSeries(valSeries);

        axisYalt->setTitleText("Ln of Selex");
        cht->addAxis(axisYalt, Qt::AlignRight);

//        ascendSeries = new QLineSeries(cht);
        ascendSeries->setPen(QPen(QBrush(Qt::blue), 2));
        ascendSeries->setName(QString("Ln of Selex"));
        cht->addSeries(ascendSeries);
        ascendSeries->attachAxis(axisX);
        ascendSeries->attachAxis(axisYalt);

        cht->addSeries(valSeries);
        valSeries->attachAxis(axisX);
        valSeries->attachAxis(axisY);

        cht->legend()->show();
        cht->legend()->setAlignment(Qt::AlignLeft);

        updateLinearScaled(scale);
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
            parameterView->setSliderRange(i, bins.first(), bins.last());
            parameterView->setName(i, QString("Node %1").arg(QString::number(i+1)));
        }
        for (int i = 0; i < special; i++) {
            int row = special + i;
            parameterView->setName(row, QString("Node Value %1").arg(QString::number(row+1)));
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

    numParams = parameters->rowCount();
    parameterView->setNumParamsShown(numParams);

    num = parameters->rowCount() - 4;
    if (num < 3 || numNodes != num)  {
        setMessage(tr("ERROR: Incorrect number of parameters. \nShould be Special +4"));
    }
    else {
        setParameterHeaders();
        parameterView->setName(0, QString("Scale Low Bin"));
        parameterView->setType(0, QString("Integr"));
        parameterView->setName(1, QString("Scale High Bin"));
        parameterView->setType(1, QString("Integr"));
        parameterView->setName(2, QString("length for P1"));
        parameterView->setType(2, QString("Value"));
        parameterView->setName(3, QString("length for P2"));
        parameterView->setType(3, QString("Value"));
        parameterView->setName(4, QString("ln of sel at P1"));
        parameterView->setType(4, QString("Value"));

        for (index = 5; index < numParams - 1; index++)
        {
            parameterView->setName(index, QString("ln of sel midpt"));
            parameterView->setType(index, QString("Value"));
        }
        parameterView->setName(index, QString("ln of sel at P2"));
        parameterView->setType(index, QString("Value"));

        showBins(true);
        cht->removeSeries(valSeries);

        axisYalt->setTitleText("Ln of Selex");
        cht->addAxis(axisYalt, Qt::AlignRight);

        setupLimits();
        limit1->setPen(QPen(QBrush(Qt::darkGreen), 2));
        limit1->setName(QString("ScaleLoBin"));
        limit2->setPen(QPen(QBrush(Qt::darkBlue), 2));
        limit2->setName(QString("ScaleHiBin"));

        ascendSeries->setPen(QPen(QBrush(Qt::blue), 2));
        ascendSeries->setName(QString("Ln of Selex"));
        cht->addSeries(ascendSeries);
        ascendSeries->attachAxis(axisX);
        ascendSeries->attachAxis(axisYalt);

        cht->addSeries(valSeries);
        valSeries->attachAxis(axisX);
        valSeries->attachAxis(axisY);

        cht->legend()->show();
        cht->legend()->setAlignment(Qt::AlignLeft);

        updateLinearScaled(2);
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
        updateLinearScaled(2);
    else
    {
        valSeries->clear();
        firstPoints.clear();
        ascendSeries->clear();

        for (int i = 0; i <= num+1; i++)  {
            xVal.append(0.0);
            yVal.append(0.0);
        }
    // first point
        xVal[0] = xValList.first();
        yVal[0] = -10.0;
        xVal[1] = parameterView->getInput(0);

        for (index = 1; index <= num; index++) {
            yVal[index] = parameterView->getInput(index+1);
        }
        xVal[num] = parameterView->getInput(1);

        xVal[num + 1] = xValList.last();
        yVal[num + 1] = yVal[num];

        temp = (xVal[num] - xVal[1]) / (num - 1);
        for (index = 2; index < numNodes; index++) {
            xVal[index] = xVal[1] + (index - 1) * temp;
        }

        for (int i = 0; i <= numNodes; i++)
            ascendSeries->append(QPointF(xVal[i], yVal[i]));
        if (xVal[numNodes] > binMax)
            temp = xVal[numNodes] + 2;
        else
            temp = binMax;
        ascendSeries->append(QPointF(temp, yVal[numNodes]));
        axisYMax = static_cast<int>(maxYvalue(ascendSeries->points()));
        axisYMin = static_cast<int>(minYvalue(ascendSeries->points()));
        axisYalt->setRange(axisYMin, axisYMax);

        for (int i = 0; i < xValList.count(); i++)
        {
            len = xValList.at(i);
            for (int j = 1; j <= numNodes; j++)
            {
                if (len >= xVal[j -1] && len < xVal[j])
                {
                    y = evaluateLine(QPointF(xVal[j-1], yVal[j-1]), QPointF(xVal[j], yVal[j]), len);
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
    firstPoints.clear();
    valSeries->clear();
//    xVal = parameterView->getInput(0);
//    while (xVal > xValList.at(i)) {
 //       valSeries->append(QPointF(xValList.at(i++), 0.0));
//    }
    xVal = 0;
    firstPoints.append(QPointF(xVal, 0.0));
    for (i = 0; i < special; i++) {
        xVal = parameterView->getInput(i);
        yVal = parameterView->getInput(i+special);
        firstPoints.append(QPointF(xVal, yVal));
    }
    if (xVal < xValList.last()) {
        firstPoints.append(QPointF(xValList.last(), yVal));
    }
    float temp = maxYvalue(firstPoints);
    if (temp > 1) {
        for (i = 0; i < firstPoints.count(); i++) {
            valSeries->append(QPointF(firstPoints.at(i).x(), firstPoints.at(i).y()/temp));
        }
    }
}

// size selectivity 43
void DialogSelexEquationView::updateLinearScaled(int scale) {
//    int scale = 2;
    bool okay = true;
    double loBin = 1, hiBin = 2;
    double altYmin = 0, altYmax = 1;
    double lastsel=-10.0;  // log(selex) for first bin;
    double lastSelPoint=bins.first();    //  first size
    double finalSelPoint=parameterView->getInput(scale + 1); // size beyond which selex is constant
    double SelPoint=parameterView->getInput(scale);   //  first size that will get a parameter.  Value will get incremented by step interval (temp1)
    int index=scale+2;  // next parameter
    double temp1 = (finalSelPoint-SelPoint)/(special-1.0);  // step interval
    QList<QPointF> secndPoints;

    axisY->setRange(0.0, 1.2);

    if (scale == 1)
        updateLinearPlain();
    else if (scale == 2) {
        loBin = parameterView->getInput(0);
        hiBin = parameterView->getInput(1);
        okay = checkScaleSliders(0, 1, bins, loBin, hiBin);
        setLimits(loBin, hiBin);
    }

    if (okay) {
        firstPoints.clear();
        ascendSeries->clear();
        valSeries->clear();

        firstPoints.append(QPointF(lastSelPoint, lastsel));
        for (; index < parameters->rowCount(); index++)
        {
            lastsel = parameterView->getInput(index);
            firstPoints.append(QPointF(SelPoint, lastsel));
            SelPoint += temp1;
        }
        SelPoint = bins.last();
        firstPoints.append(QPointF(SelPoint, lastsel));
        altYmin = minYvalue(firstPoints);
        altYmax = maxYvalue(firstPoints);
        axisYalt->setRange(altYmin, altYmax);

        ascendSeries->append(firstPoints);

        if (scale == 2) {
            temp1 = aveYvalue(firstPoints, loBin-1, hiBin-1);
            setLimits(bins.at(loBin-1), bins.at(hiBin-1));
        }
        else {
            temp1 = altYmax;
        }

        secndPoints.clear();
        for (double a = bins.first(); a < bins.last(); a+=1.0) {
            secndPoints.append(QPointF(a, evaluatePoints(firstPoints, a)));
        }
        secndPoints.append(firstPoints.last());

        for (int i = 0; i < secndPoints.count(); i++)
            valSeries->append(QPointF(secndPoints.at(i).x(), exp(secndPoints.at(i).y() - temp1)));

        yMax = maxYvalue(valSeries->points());
        if (yMax > 1.0)
            axisY->setRange(0.0, yMax * 1.2);

    }
    /*   double x = 0, y = 0;
    float range, increment;
    double altYMax = 0;
    double altYMin = 0;
    QString msg;
    float temp = 0;
    double len = 0;
    int numNodes = getSpecial();
    int index = 0;
    int binCount = bins.count();
    QVector<double> xVal;
    QVector<double> yVal;
    int lobin = static_cast<int>(parameterView->getInput(0));
    int hibin = static_cast<int>(parameterView->getInput(1));
    int num = parameters->rowCount() - 4;
    int numPoints = num + 1;
    bool okay;

    if (num < 3 || numNodes != num)
    {
        setMessage(tr("ERROR: Incorrect number of parameters. \nShould be Special + 4"));
        return;
    }

//    if (intScale == 2)
//    {
        lobin = static_cast<int>(parameterView->getInput(0));
        hibin = static_cast<int>(parameterView->getInput(1));
        okay = checkScaleSliders(bins, lobin, hibin);
//    }

    if (okay) {
        valSeries->clear();
        firstPoints.clear();
        ascendSeries->clear();

        for (int i = 0; i < numPoints; i++)
        {
            xVal.append(0);
            yVal.append(0);
        }
        xVal[0] = bins.first();
        yVal[0] = -10.0;
        xVal[1] = parameterView->getInput(2);
        xVal[num] = parameterView->getInput(3);
        if (xVal[num] < (xVal[1] + num))
            xVal[num] = xVal[1] + num;
        range = xVal[num] - xVal[1];
        increment = range / (num - 1);

        yVal[1] = parameterView->getInput(4);
        for (index = 2; index < numPoints; index++)
        {
            xVal[index] = xVal[index - 1] + increment;
            yVal[index] = parameterView->getInput(index+3);
        }


        for (index = 0; index <= numNodes; index++)
            ascendSeries->append(QPointF(xVal[index], yVal[index]));
        if (xVal[numNodes] < bins.last()) {
            ascendSeries->append(bins.last(), yVal[numNodes]);
        }


        for (int i = 0; i < xValList.count(); i++)
        {
            len = xValList.at(i);
            for (int j = 1; j <= numNodes; j++)
            {
                if (len >= xVal[j -1] && len < xVal[j])
                {
                    y = evaluateLine(QPointF(xVal[j-1], yVal[j-1]), QPointF(xVal[j], yVal[j]), len);
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
        altYMin = minYvalue(firstPoints);
        altYMax = maxYvalue(firstPoints);
        limit1->clear();
        limit2->clear();
        limit1->append(QPointF(bins.at(lobin-1)-.01, altYMin));
        limit1->append(QPointF(bins.at(lobin-1), altYMax));
        limit2->append(QPointF(bins.at(hibin-1), altYMin));
        limit2->append(QPointF(bins.at(hibin-1)+.01, altYMax));

        updateLinearExp(2);

    }*/
}

// Use the list of points (firstPoints)
// to create the valSeries as firstPoints.x, exp(firstPoints.y - yMax)
// finally, scale by selected bins (if eqn 43)
void DialogSelexEquationView::updateLinearExp(int scale) {
    double yVal;
    double altYmin, altYmax;

    altYmin = minYvalue(firstPoints);
    altYmax = maxYvalue(firstPoints);
    axisYalt->setRange(altYmin, altYmax);

    valSeries->clear();
    if (scale == 2)
    {
        double lobin = parameterView->getInput(0);
        double hibin = parameterView->getInput(1);
        if (checkScaleSliders(0, 1, bins, lobin, hibin)) {
            int firstBin = static_cast<int>(lobin) - 1;
            int lastBin = static_cast<int>(hibin) - 1;

            double yAve = aveYvalue(firstPoints, firstBin, lastBin);

            for (int i = 0; i < firstPoints.count(); i++)
            {
                yVal = exp(firstPoints.at(i).y() - yAve);
                valSeries->append(firstPoints.at(i).x(), yVal);
            }
        }
    }
    else {
        for (int i = 0; i < firstPoints.count(); i++) {
            yVal = exp(firstPoints.at(i).y() - yMax);
            valSeries->append(firstPoints.at(i).x(), yVal);
        }
    }
    yVal = maxYvalue(valSeries->points());
    if (yVal > 1.0) {
        axisY->setRange(0, yVal * 1.2);
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
        cht->addAxis(axisYalt, Qt::AlignRight);

//        ascendSeries = new QLineSeries(cht);
        ascendSeries->setName(QString("Logistic1"));
        ascendSeries->setPen(QPen(QBrush(Qt::green), 2));
        cht->addSeries(ascendSeries);
        ascendSeries->attachAxis(axisX);
        ascendSeries->attachAxis(axisYalt);

//        dscendSeries = new QLineSeries(cht);
        dscendSeries->setName(QString("Logistic2"));
        dscendSeries->setPen(QPen(QBrush(Qt::blue), 2));
        cht->addSeries(dscendSeries);
        dscendSeries->attachAxis(axisX);
        dscendSeries->attachAxis(axisYalt);

        cht->addSeries(valSeries);
        valSeries->attachAxis(axisX);
        valSeries->attachAxis(axisY);

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
        parameterView->setType(2, QString("Logist"));
        parameterView->setName(3, QString("Slope-Up"));
        parameterView->setType(3, QString("Value"));
        parameterView->setName(4, QString("Final"));
        parameterView->setType(4, QString("Logist"));
        parameterView->setName(5, QString("Infl-Dn"));
        parameterView->setType(5, QString("Logist"));
        parameterView->setName(6, QString("Slope-Dn"));
        parameterView->setType(6, QString("Value"));
        parameterView->setName(7, QString("BinWidth"));
        parameterView->setType(7, QString("Value"));
        setParameterHeaders();

        showBins(true);
        showJoins(0);
        cht->removeSeries(valSeries);

        join1Series->setPen(QPen(QBrush(Qt::darkGreen), 1));
        join1Series->setName(QString("Join1"));
        cht->addSeries(join1Series);
        join1Series->attachAxis(axisX);
        join1Series->attachAxis(axisY);

        join2Series->setPen(QPen(QBrush(Qt::darkCyan), 1));
        join2Series->setName(QString("Join2"));
        cht->addSeries(join2Series);
        join2Series->attachAxis(axisX);
        join2Series->attachAxis(axisY);

        join3Series->setPen(QPen(QBrush(Qt::darkYellow), 1));
        join3Series->setName(QString("Join3"));
        cht->addSeries(join3Series);
        join3Series->attachAxis(axisX);
        join3Series->attachAxis(axisY);

        ascendSeries->setPen(QPen(QBrush(Qt::green), 3));
        ascendSeries->setName(QString("Logist1"));
        cht->addSeries(ascendSeries);
        ascendSeries->attachAxis(axisX);
        ascendSeries->attachAxis(axisY);

        dscendSeries->setPen(QPen(QBrush(Qt::blue), 3));
        dscendSeries->setName(QString("Logist2"));
        cht->addSeries(dscendSeries);
        dscendSeries->attachAxis(axisX);
        dscendSeries->attachAxis(axisY);

        cht->addSeries(valSeries);
        valSeries->attachAxis(axisX);
        valSeries->attachAxis(axisY);

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
//    float binMin = getBinMin();
//    float binMax = getBinMax();
    float minVal = xValList.first();// binMin + (getBinStep() * getMidBin());
    float maxVal = xValList.last(); // binMax + (getBinStep() * getMidBin());
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
    chartview->setVisible(false);
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

//        ascendSeries = new QLineSeries(cht);
        ascendSeries->setPen(QPen(QBrush(Qt::green), 2));
        ascendSeries->setName(QString("Ascend"));
        cht->addSeries(ascendSeries);
        ascendSeries->attachAxis(axisX);
        ascendSeries->attachAxis(axisY);

//        dscendSeries = new QLineSeries(cht);
        dscendSeries->setPen(QPen(QBrush(Qt::blue), 2));
        dscendSeries->setName(QString("Descend"));
        cht->addSeries(dscendSeries);
        dscendSeries->attachAxis(axisX);
        dscendSeries->attachAxis(axisY);

        cht->addSeries(valSeries);
        valSeries->attachAxis(axisX);
        valSeries->attachAxis(axisY);

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

//        join1Series = new QLineSeries(cht);
        join1Series->setPen(QPen(QBrush(Qt::darkYellow), 2));
        join1Series->setName(QString("Join1"));
        cht->addSeries(join1Series);
        join1Series->attachAxis(axisX);
        join1Series->attachAxis(axisY);

//        join2Series = new QLineSeries(cht);
        join2Series->setPen(QPen(QBrush(Qt::yellow), 2));
        join2Series->setName(QString("Join2"));
        cht->addSeries(join2Series);
        join2Series->attachAxis(axisX);
        join2Series->attachAxis(axisY);

//        ascendSeries = new QLineSeries(cht);
        ascendSeries->setPen(QPen(QBrush(Qt::green), 3));
        ascendSeries->setName(QString("Asc-Norm"));
        cht->addSeries(ascendSeries);
        ascendSeries->attachAxis(axisX);
        ascendSeries->attachAxis(axisY);

//        dscendSeries = new QLineSeries(cht);
        dscendSeries->setPen(QPen(QBrush(Qt::blue), 3));
        dscendSeries->setName(QString("Desc-Norm"));
        cht->addSeries(dscendSeries);
        dscendSeries->attachAxis(axisX);
        dscendSeries->attachAxis(axisY);

        cht->addSeries(valSeries);
        valSeries->attachAxis(axisX);
        valSeries->attachAxis(axisY);

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
        parameterView->setType(1, QString("Trans"));
        parameterView->setName(2, QString("Asc-width"));
        parameterView->setType(2, QString("Exp"));
        parameterView->setName(3, QString("Dsc-width"));
        parameterView->setType(3, QString("Exp"));
        parameterView->setName(4, QString("Initial"));
        parameterView->setType(4, QString("Logist"));
        parameterView->setName(5, QString("Final"));
        parameterView->setType(5, QString("Logist"));
        setParameterHeaders();
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
        cht->removeSeries(valSeries);

//        join1Series = new QLineSeries(cht);
        join1Series->setPen(QPen(QBrush(Qt::darkYellow), 2));
        join1Series->setName(QString("Join1"));
        cht->addSeries(join1Series);
        join1Series->attachAxis(axisX);
        join1Series->attachAxis(axisY);

//        join2Series = new QLineSeries(cht);
        join2Series->setPen(QPen(QBrush(Qt::yellow), 2));
        join2Series->setName(QString("Join2"));
        cht->addSeries(join2Series);
        join2Series->attachAxis(axisX);
        join2Series->attachAxis(axisY);

//        ascendSeries = new QLineSeries(cht);
        ascendSeries->setPen(QPen(QBrush(Qt::green), 3));
        ascendSeries->setName(QString("Asc-Norm"));
        cht->addSeries(ascendSeries);
        ascendSeries->attachAxis(axisX);
        ascendSeries->attachAxis(axisY);

//        dscendSeries = new QLineSeries(cht);
        dscendSeries->setPen(QPen(QBrush(Qt::blue), 3));
        dscendSeries->setName(QString("Desc-Norm"));
        cht->addSeries(dscendSeries);
        dscendSeries->attachAxis(axisX);
        dscendSeries->attachAxis(axisY);

        cht->addSeries(valSeries);
        valSeries->attachAxis(axisX);
        valSeries->attachAxis(axisY);

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
    float top   = logist(parameterView->getValue(1));
    float asc_wd = parameterView->getInput(2);
    float dsc_wd = parameterView->getInput(3);
    float par5  = parameterView->getValue(4);
    float par6  = parameterView->getValue(5);
    float binMax = getBinMax();
    float binWidth = getBinStep();

//    float peak = par1;
    float peak2 = (peak + binWidth) + (.99*binMax - peak - binWidth) * top;
    parameterView->setInputValue(1, peak2);
//    float asc_wd = exp(par3);
//    float dsc_wd = exp(par4);
    float init = logist(par5);
    parameterView->setInputValue(4, init);
    float final = logist(par6);
    parameterView->setInputValue(5, final);
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
        valmin = getBinMin();// + getBinStep() * getMidBin();
        valmax = getBinMax();// + getBinStep() * getMidBin();
        valminpow = pow((valmin - peak), 2);
        valmaxpow = pow((valmax - peak2), 2);
        valmin = exp(-1 * valminpow/asc_wd);
        valmax = exp(-1 * valmaxpow/dsc_wd);

        for (i = 0; i < xValList.count(); i++)
        {
            xval = xValList.at(i);
            upsel = exp(-1*(pow(xval - peak, 2))/asc_wd);
            if (par5 > -999)
                upsel = init + (1 - init) * (upsel - valmin)/(1-valmin);
            dnsel = exp(-1*(pow(xval - peak2, 2))/dsc_wd);
            if (par6 > -999)
                dnsel = (1 + (final - 1) * (dnsel - 1)/(valmax - 1));

            jn1 = logist(getJoinOne() *(xval-peak)/(1 + fabs(xval-peak)));
            jn2 = logist(getJoinTwo() *(xval-peak2)/(1 + fabs(xval-peak2)));

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
    double par2 = parameterView->getValue(1);
    double par3 = parameterView->getInput(2);

    double peak;
    if (slxType == Size) {
        peak = bins.first() + par2 * (bins.last() - bins.first());
    }
    else {
        peak = par2 * bins.last();
    }
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
    QString msg;
    setLabel(QString("Pattern %1: Revise Age").arg(
                 QString::number(equationNum)));

    if (parameters->rowCount() >= bins.count()) {
        numParams = bins.count() + 1;
        parameterView->setNumParamsShown(numParams);

        for (int i = 0; i < numParams; i++)
            parameterView->setName(i, QString("Value at age %1").arg(QString::number(i)));
        setParameterHeaders();

        showBins(true);
        showJoins(0);
        cht->removeSeries(valSeries);

        axisYalt->setTitleText(QString("Value at age (blue)"));
        cht->addAxis(axisYalt, Qt::AlignRight);
        cht->legend()->show();
        cht->legend()->setAlignment(Qt::AlignLeft);

        ascendSeries->setName(QString("Input value (blue)"));
        ascendSeries->setPen(QPen(QBrush(Qt::blue), 2));
        cht->addSeries(ascendSeries);
        ascendSeries->attachAxis(axisX);
        ascendSeries->attachAxis(axisYalt);

        cht->addSeries(valSeries);
        valSeries->attachAxis(axisX);
        valSeries->attachAxis(axisY);

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
            updateEachAge();
        }
    }
}

/*
  // find max of input parameters
  temp=9.-max(sp(1,nages+1));  // this forces at least one age to have selex weight equal to 9
  for (a=0; a<=nages; a++)
  {
    if (sp(a+1)>-999) // normal
    {
      // logist of (parm(a+1) + temp)
      sel_a(y,fs,1,a) = 1./(1.+mfexp(-(sp(a+1)+temp)));
    }
    else
    {
      // previous value
      sel_a(y,fs,1,a) = sel_a(y,fs,1,a-1);
    }
  }
*/
void DialogSelexEquationView::updateEachAge () {
    float parm = 0., lastParm = 0.;
    float asc = 0.;
    float sel = 0.;
    float temp;
    int  max = 0, min = 0;
    float binWidth = getBinStep();
    float age = getBinMin() + getMidBin() * binWidth;
    QString msg("");

    firstPoints.clear();
    ascendSeries->clear();
    valSeries->clear();

    age = 0;
    firstPoints.append(QPointF(0, parameterView->getInput(0)));
    for (int i = 1; i < numParams; i++)
    {
        age = bins.at(i-1);
        parm = parameterView->getInput(i);
        if (parm > -999) {
            if (parm < -5) parm = -5.;
            if (parm > 9) parm = 9.;
            lastParm = parm;
        }
        else {
            parm = lastParm;
        }
        firstPoints.append(QPointF(age, parm));
    }
    temp = 9. - maxYvalue(firstPoints);

    ascendSeries->append(firstPoints);
    for (int a = 0; a < numParams; a++)
    {
        asc = (firstPoints.at(a).y() + temp);
        sel = logist(asc);
        valSeries->append(QPointF(firstPoints.at(a).x(), sel));
    }
    min = static_cast<int> (minYvalue(ascendSeries->points()));
    max = static_cast<int> (maxYvalue(ascendSeries->points())) ;

    axisYalt->setRange(min, max);
//    ascendSeries->append(QPointF(firstPoints.at(bins.count()).x(), asc));
//    valSeries->append(QPointF(firstPoints.at(bins.count()).x(), sel));
}

// 17 #age selectivity: each age has parameter as random walk
//    transformation is selex=exp(parm); some special codes
void DialogSelexEquationView::randomWalk (float scale) {
    int intScale = static_cast<int>(scale);
    setLabel(QString("Pattern %1: Random Walk").arg(
                 QString::number(equationNum)));
    if (parameters->rowCount() > bins.count()) {
        numParams = bins.count() + 1;
        parameterView->setNumParamsShown(numParams + scale);
        showBins(true);
        showJoins(0);
        cht->removeSeries(valSeries);

        if (intScale > 0) {
            parameterView->setName(0, QString("Scale Low Bin"));
            parameterView->setName(1, QString("Scale High Bin"));
        }
        for (int i = intScale; i < numParams; i++)
            parameterView->setName(i, QString("Value at age %1").arg(QString::number(i-intScale)));
        setParameterHeaders(intScale);

        axisYalt->setTitleText("Param value (blue)");
        cht->addAxis(axisYalt, Qt::AlignRight);

        if (scale > 1) {
            setupLimits();
            limit1->setPen(QPen(QBrush(Qt::darkGreen), 2));
            limit1->setName(QString("ScaleLoBin"));
            limit2->setPen(QPen(QBrush(Qt::darkBlue), 2));
            limit2->setName(QString("ScaleHiBin"));
        }

        ascendSeries->setName(QString("Param value (blue)"));
        ascendSeries->setPen(QPen(QBrush(Qt::blue), 3));
        cht->addSeries(ascendSeries);
        ascendSeries->attachAxis(axisX);
        ascendSeries->attachAxis(axisYalt);

        cht->addSeries(valSeries);
        valSeries->attachAxis(axisX);
        valSeries->attachAxis(axisY);

        updateRandomWalk(scale);
    }
}
/*
  lastsel=0.0;  //  value is the change in log(selex);  this is the reference value for age 0
  tempvec_a=-999.;
  tempvec_a(0)=0.0;   //  so do not try to estimate the first value
  int lastage;
  if(seltype(f,4)==0)
  {
    lastage=nages;
  }
  else
  {
    lastage=abs(seltype(f,4));
  }

  for (a=1; a<=lastage; a++)
  {
    //  with use of -999, lastsel stays constant until changed, so could create a linear change in ln(selex)
                                          // use of (a+1) is because the first element, sp(1), is for age zero
    if(sp(a+1+scaling_offset) > -999.)
    {
      lastsel=sp(a+1+scaling_offset);
    }
    tempvec_a(a)=tempvec_a(a-1)+lastsel;   // cumulative log(selex)
  }
  if (scaling_offset == 0)
  {
      temp=max(tempvec_a);   //  find max so at least one age will have selex=1.
  }
  else
  {
      int low_bin  = int(value(sp(1)));
      int high_bin = int(value(sp(2)));
      if (low_bin < 0)
      {
        low_bin = 0;
        N_warn++; warning<<" selex pattern 41; value for low bin is less than 0, so set to 0 "<<endl;
      }
      if (high_bin > nages)
      {
        high_bin = nages;
        N_warn++; warning<<" selex pattern 41; value for high bin is greater than "<<nages<<", so set to "<<nages<<" "<<endl;
      }
      if (high_bin < low_bin)
        high_bin = low_bin;
      if (low_bin > high_bin)
        low_bin = high_bin;
      sp(1) = low_bin;
      sp(2) = high_bin;
      temp=mean(tempvec_a(low_bin,high_bin));
  }
  sel_a(y,fs,1)=mfexp(tempvec_a-temp);
  a=0;
  while(sp(a+1+scaling_offset) == -1000)  //  reset range of young ages to selex=0.0
  {
    sel_a(y,fs,1,a)=0.0;
    a++;
  }
  scaling_offset = 0;     // reset scaling offset
  if(lastage<nages)
  {
    for (a=lastage+1; a<=nages; a++)
    {
      if(seltype(f,4) > 0)
      {
        sel_a(y,fs,1,a)=sel_a(y,fs,1,a-1);
      }
      else
      {
        sel_a(y,fs,1,a)=0.0;
      }
    }
  }
*/
void DialogSelexEquationView::updateRandomWalk (float scale) {
    double firstBin, lastBin;
    double parmval;
    double lastSel = 0.0;
    double temp = -999;
    QList<float> parms;
    float binMax = getBinMax();
    int scaleOffset = static_cast<int>(scale);
    int firstage = 0;
    int lastage = bins.count();
    int i = 0;
    int loBin = 1;// scaleLo = bins.first();
    int hiBin = bins.count();// scaleHi = bins.last();

    ascendSeries->clear();
    valSeries->clear();
    firstPoints.clear();

    // set last age = special or last bin
    if (special != 0)
        lastage = abs(special);
    if (lastage > bins.count())
        lastage = bins.count();


    // tempvec_a
    firstPoints.append(QPointF(0.0, 0.0));

    for (int a = 1; a < numParams; a++) {
        //  with use of -999, lastsel stays constant until changed, so could create a linear change in ln(selex)
        parmval = parameterView->getValue(scaleOffset + a);
        if (parmval > -999.) {
            lastSel = parmval;
        }
        else {
            parameterView->setInputValue(scaleOffset + a, lastSel);
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
        firstBin = parameterView->getInput(0);
        lastBin = parameterView->getInput(1);
        checkScaleSliders(0, 1, bins, firstBin, lastBin);
        loBin = static_cast<int>(firstBin);
        hiBin = static_cast<int>(lastBin);

        setLimits(bins.at(loBin-1), bins.at(hiBin-1));

        temp = aveYvalue(firstPoints, loBin - 1, hiBin - 1);
    }

    // find first age bin
    i = 0;
    firstage = 1;
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
    yMax = maxYvalue(valSeries->points());
    if (yMax > 1.0001)
        axisY->setRange(0, yMax * 1.2);

}



// case 16 Coleraine - Gaussian age selectivity
void DialogSelexEquationView::coleGauss () {
    setLabel(QString("Pattern %1: Coleraine single Gaussian").arg(
                 QString::number(equationNum)));

    if (parameters->rowCount() == 2) {
    numParams = 2;
    parameterView->setNumParamsShown(numParams);
    setParameterHeaders();

    parameterView->setName(0, QString("Age limit"));
    parameterView->setType(0, QString("Logist"));
    parameterView->setName(1, QString("Decline scaling"));
    parameterView->setType(1, QString("Value"));

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
    double temp = 0;
    double sel = 0;
    double par1 = parameterView->getInput(0);
    double par2 = parameterView->getInput(1);

    valSeries->clear();

    if (slxType == Size)
        temp = xValList.first() + par1 * (xValList.last() - xValList.first());
    else
        temp = par1 * xValList.last();

    for (i = 0; xValList.at(i) < temp; i++)
    {
        sel = exp(-1*pow(xValList.at(i) - temp, 2) / exp(par2));
        valSeries->append(QPointF(xValList.at(i), sel));
//        sel = exp(-1*(pow(xValList.at(i) -temp, 2) / par2));
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
    int numNodes = getSpecial();
    int index = 0;
    int intScale = static_cast<int>(scale + .5);
    int numParams = intScale + 3 + (numNodes * 2);
    QString typestr (selex->getType() == Age? "Age": "Length");
    parameterView->setNumParamsShown(numParams);
    setParameterHeaders(intScale);

    if (numNodes >= 3) {
        flag = static_cast<int>(parameterView->getInput(intScale) + .5);
        if (flag) {
            setMessage(QString("%1 parameters were autogenerated.").arg(flag == 1? "Node": "All Cubic"));
            selex->autogenParameters();
        }

        int num = 2*numNodes + 3 + intScale;
        if (parameters->rowCount() == num) {
            numParams = parameters->rowCount();
            parameterView->setNumParamsShown(numParams);
            if (intScale == 2)
            {
                parameterView->setName(index, QString("ScaleLo"));
                parameterView->setType(index++, QString("Integr"));
                parameterView->setName(index, QString("ScaleHi"));
                parameterView->setType(index++, QString("Integr"));
            }
            parameterView->setName(index, QString("Setup"));
            parameterView->setType(index++, QString("Integr"));
            parameterView->setName(index, QString("GradLo"));
            parameterView->setType(index++, QString("Value"));
            parameterView->setName(index, QString("GradHi"));
            parameterView->setType(index++, QString("Value"));

            for (int i = 0; i < numNodes; i++) {
                parameterView->setName(index + i, QString("Knot %1 %2").arg(QString::number(i+1), typestr));
                parameterView->setType(index + i, QString("Value"));
            }
            index += numNodes; //= static_cast<int>(scale + .5) + 3 + numNodes;
            for (int i = 0; i < numNodes; i++) {
                parameterView->setName(index + i, QString("Knot %1 Value").arg(QString::number(i+1)));
                parameterView->setType(index + i, QString("Value"));
            }

            setLabel(QString("Pattern %1: Cubic Spline %2 nodes").arg(
                         QString::number(equationNum),
                         QString::number(special)));

            showBins(true);
            cht->removeSeries(valSeries);

            axisYalt->setTitleText("Cubic Nodes (blue)");
            cht->addAxis(axisYalt, Qt::AlignRight);

            if (scale > 0) {
                setupLimits();
                limit1->setPen(QPen(QBrush(Qt::darkGreen), 2));
                limit1->setName(QString("ScaleLoBin"));
                limit2->setPen(QPen(QBrush(Qt::darkBlue), 2));
                limit2->setName(QString("ScaleHiBin"));
            }

            ascendSeries->setPen(QPen(QBrush(Qt::blue), 3));
            cht->addSeries(ascendSeries);
            ascendSeries->attachAxis(axisX);
            ascendSeries->attachAxis(axisYalt);

            ptSeries->setPen(QPen(QBrush(Qt::blue), 3));
            cht->addSeries(ptSeries);
            ptSeries->attachAxis(axisX);
            ptSeries->attachAxis(axisYalt);

            cht->addSeries(valSeries);
            valSeries->attachAxis(axisX);
            valSeries->attachAxis(axisY);

            if (numNodes > 5 && (numNodes % 2) == 0)
            {
                updateCubicSpline(scale);
            }
        }
    }
}

void DialogSelexEquationView::updateCubicSpline(float scale) {
    double xval = 0, yval = 0;
    int intScale = static_cast<int>(scale + 0.5);
    int start = intScale + 3;
    int num = parameters->rowCount() - start;
    int size, end = intScale;
    double firstBin, lastBin;
    int loBin = -1, hiBin = -1;
    int setup = 0;
    double gradLo = 0;
    double gradHi = 0;
    double temp1 = 1;
    double temp2 = 2;
    double min, max, val;
    int i, one, two;
    bool okay = true;
    QString msg;
    QStringList data;
    double altMaxVal, altMinVal, altMax, altMin;
    QList<QPointF> secndPoints;


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
        if (intScale == 2)
        {
            firstBin = parameterView->getInput(0);
            lastBin = parameterView->getInput(1);
            checkScaleSliders(0, 1, bins, firstBin, lastBin);
            loBin = static_cast<int>(firstBin);
            hiBin = static_cast<int>(lastBin);
        }

        setup = static_cast<int>(parameterView->getInput(intScale));
        if (setup != flag) {
            if (setup)
                selex->autogenParameters(setup);
            flag = setup;

            switch(setup) {
            case 1:
                end = intScale + 3 + special;
                break;
            case 2:
                start = intScale;
                end = parameters->rowCount();
                break;
            }
            for (int i = start; i < end; i++) {
                data = parameters->getRowData(i);
                min = data.at(0).toDouble();
                max = data.at(1).toDouble();
                val = data.at(2).toDouble();
                parameterView->setSliderRange(i, min, max);
                parameterView->setSliderValue(i, val);
            }
        }
        start = intScale + 1;

        gradLo = parameterView->getInput(start++);
        gradHi = parameterView->getInput(start++);

        // check x-vals
        if (parameterView->getInput(start) < bins.first()) {
            parameterView->setSliderValue(start, bins.first());
        }
        for (int k = 0; k < size - 1; k++) {
            one = start + k; two = one + 1;
            temp1 = parameterView->getInput(one);
            temp2 = parameterView->getInput(two);
            if (temp2 <= temp1) {
                parameterView->setSliderValue(two, parameterView->getInput(one) + .5);
            }
        }

        if (okay) {

        for (i = 0; i < size; i++)
            X[i] = parameterView->getInput(start++);
        for (i = 0; i < size; i++)
            Y[i] = parameterView->getInput(start++);

        ptSeries->clear();

        for (i = 0; i < size; i++)
            ptSeries->append(X[i], Y[i]);


        // currently it is required that X is already sorted
        cubicspl.set_boundary(tk::spline::first_deriv, gradLo,
                              tk::spline::first_deriv, gradHi);
        cubicspl.set_points(X, Y);

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
        altMax = altMinVal + ((altMaxVal - altMinVal) * 1.2);
        axisYalt->setRange(altMinVal, altMaxVal);

        secndPoints.clear();
        if (scale > 0)
        {
            setLimits(bins.at(loBin-1), bins.at(hiBin-1));

            temp1 = aveYvalue(firstPoints, loBin-1, hiBin-1);
            for (i = 0; i < firstPoints.count(); i++) {
                temp2 = firstPoints.at(i).y();
                secndPoints.append(QPointF(firstPoints.at(i)));
                secndPoints[i].setY(exp(temp2 - temp1));
            }
        }
        else
        {
            temp1 = maxYvalue(firstPoints);
//            if (temp1 > 10) temp1 = 10;
            for (i = 0; i < firstPoints.count(); i++) {
                temp2 = firstPoints.at(i).y();
//                if (temp2 > temp1) temp2 = temp1 - .0000001;
                secndPoints.append(firstPoints.at(i));
                secndPoints[i].setY(exp(temp2 - temp1));
            }
        }
        yMax = maxYvalue(secndPoints);
        if (yMax > 1.0)
            axisY->setRange(0, yMax * 1.2);

        valSeries->append(secndPoints);
        }
    }
}

// 44 like age selex 17 but with separate parameters for males and with revised controls
//  param(0) is first age with non-zero selectivity; can be age 0
//  special is number of selectivity change parameters beginning at age=first_age+1
//  param(1) is first age for which mean selex=1
//  param(2) is last age  for which mean selex=1
//  param(3) is male mean selex relative to female mean.
//  -999 code means to keep the change unchanged from previous age (so keeps same rate of change)
// -1000 code is only for males and sets the male change to be same as female change
//  xxx is the index for sex, where 1=female, 2=male
void DialogSelexEquationView::twoSexRandomWalk() {
    setLabel(QString("Pattern %1: Random Walk - female/male").arg(
                 QString::number(equationNum)));
    int index = 3 + (genders > 1? 1:0);
    int numChanges = special;
    int num = 2 * numChanges + index;
    if (parameters->rowCount() == num) {
        numParams = num;
        parameterView->setNumParamsShown(numParams);
        showBins(true);
        setParameterHeaders();

        parameterView->setName(0, QString("First Age Bin"));
        parameterView->setType(0, QString("Integr"));
        parameterView->setName(1, QString("Scale Low Bin"));
        parameterView->setType(1, QString("Integr"));
        parameterView->setName(2, QString("Scale High Bin"));
        parameterView->setType(2, QString("Integr"));
        parameterView->setName(3, QString("Male ln(ratio)"));
        parameterView->setType(3, QString("Exp"));

        for (int i = 0; i < numChanges; i++) {
            parameterView->setName(index+i, QString("Fem ln(slx) change %1").arg(QString::number(i+1)));
            parameterView->setType(index+i, QString("Value"));
        }
        cht->removeSeries(valSeries);

        axisYalt->setTitleText("Param Value(blue)");
        axisYalt->setRange(0, 1.0);
        cht->addAxis(axisYalt, Qt::AlignRight);
        cht->legend()->show();
        cht->legend()->setAlignment(Qt::AlignLeft);

        setupLimits();

        // female ln(selex)
        ascendSeries->setPen(QPen(QBrush(Qt::blue), 2));
        ascendSeries->setName(QString("Fem ln(slx)"));
        cht->addSeries(ascendSeries);
        ascendSeries->attachAxis(axisX);
        ascendSeries->attachAxis(axisYalt);

        // female selex
        valSeries->setName(QString("Fem slx (red)"));
        cht->addSeries(valSeries);
        valSeries->attachAxis(axisX);
        valSeries->attachAxis(axisY);

        if (genders > 1) {
            index = 4 + numChanges;
            for (int i = 0; i < numChanges; i++) {
                parameterView->setName(index+i, QString("Male ln(slx) change %1").arg(QString::number(i+1)));
                parameterView->setType(index+i, QString("Value"));
            }

            // male ln(selex)
            dscendSeries->setPen(QPen(QBrush(Qt::green), 2));
            dscendSeries->setName(QString("Mal ln(slx)"));
            cht->addSeries(dscendSeries);
            dscendSeries->attachAxis(axisX);
            dscendSeries->attachAxis(axisYalt);

            // male selex
            join3Series->setPen(QPen(QBrush(Qt::magenta), 5));
            join3Series->setName(QString("Mal slx (mag)"));
            cht->addSeries(join3Series);
            join3Series->attachAxis(axisX);
            join3Series->attachAxis(axisY);
        }
        updateTwoSexRandom();
    }
}

// code from SS_selex.tpl
//	sel_a(y,fs,gg).initialize();
//  dvariable seldelta=0.0;  //  value is the change in log(selex)
//  tempvec_a.initialize(); //  null value for vector
//
//  int first_age=int(value(sp(1)));
//  tempvec_a(first_age)=0.0;   //  start value for random walk across ages
//  int last_age=first_age+seltype(f,4);  //  because seltype(f,4) contains the number of changes
//  if(gg==1)
//      {scaling_offset=2+gender;}
//  else
//      {scaling_offset=2+gender+seltype(f,4);}  // to get male vs female starting point for parameters
//  j=scaling_offset;
//
//  for (a=first_age+1;a<=last_age;a++)
//  {
//      j++;
//      //  with use of -999, lastsel stays constant until changed, so could create a linear change in ln(selex)
//      if(sp(j)>-999.)
//          {seldelta=sp(j);}  // so allows for seldelta to remain unchanged
//      else if(gg==2 && sp(j)==-1000.)  //  more options for male selectivity
//          {seldelta=sp(j-seltype(f,4));}  //  use female delta for the male delta at this age
//      tempvec_a(a)=tempvec_a(a-1)+seldelta;   // cumulative log(selex)
//  }
//  int low_bin  = int(value(sp(2)));
//  int high_bin = int(value(sp(3)));
//
//  temp=mean(tempvec_a(low_bin,high_bin));
//
//  sel_a(y,fs,gg)(first_age,last_age)=mfexp(tempvec_a(first_age,last_age)-temp);
//
//  if(gg==2)
//      sel_a(y,fs,gg)(first_age,last_age)*=mfexp(sp(4)); //  apply male ratio
//  if (last_age<nages)
//      {sel_a(y,fs,gg)(last_age+1,nages)=sel_a(y,fs,gg,last_age);}
void DialogSelexEquationView::updateTwoSexRandom() {
    int index = 0;
    double altYmin = 0, altYmax = 1;
    double parm = 0.;
    double lastSel = 0;
    double temp = 0;
    double templimit = 0;
    QList<QPointF> secndPoints;

    int numChanges = special;
    int firstAge = static_cast<int>(parameterView->getInput(0));
    int lastAge = firstAge + numChanges;
    double firstBin = parameterView->getInput(1);
    double lastBin = parameterView->getInput(2);
    double maleMeanRatio = parameterView->getInput(3);
    checkScaleSliders(1, 2, bins, firstBin, lastBin);
    int loBin = static_cast<int>(firstBin);
    int hiBin = static_cast<int>(lastBin);
    firstPoints.clear();
    ascendSeries->clear();
    valSeries->clear();
    secndPoints.clear();
    dscendSeries->clear();
    join3Series->clear();

    if (lastAge > bins.count()) {
        firstAge = bins.count() - numChanges;
        parameterView->setSliderValue(0, firstAge);
    }

    // Female ln(selex)
    // random walk - ascendSeries
    firstPoints.append(QPointF(0, lastSel));
    firstPoints.append(QPointF(bins.at(firstAge-1), lastSel));
    index = 2 + (genders > 1? 2: 1);
    for (int i = 0; i < numChanges; i++) {
        parm = parameterView->getInput(index+i);
        if (parm < -998) {
            parm = parameterView->getInput(index+i-1);
            parameterView->setSliderValue(index+i, parm);
        }
        lastSel += parm;
        firstPoints.append(QPointF(bins.at(firstAge+i), lastSel));
    }
    firstPoints.append(QPointF(bins.last(), lastSel));
    altYmin = minYvalue(firstPoints);
    altYmax = maxYvalue(firstPoints);

    for (int i = 0; i < bins.count(); i++) {
        parm = evaluatePoints(firstPoints, bins.at(i));
        ascendSeries->append(bins.at(i), parm);
    }
    temp = aveYvalue(ascendSeries->points(), (loBin-1), (hiBin-1));

    // valSeries = exp(ascendSeries)
    valSeries->append(0, 0);
    for (int i = 0; i < xValList.count(); i++) {
        parm = evaluatePoints(firstPoints, xValList.at(i));
        valSeries->append(xValList.at(i), exp(parm - temp));
    }
    yMax = maxYvalue(valSeries->points());
    if (yMax > 1.0001)
        axisY->setRange(0, yMax * 1.2);

    if (genders > 1) {
        // Male ln(selex)
        // random walk - dscendSeries
        lastSel = 0.0;
        secndPoints.append(QPointF(0, lastSel));
        secndPoints.append(QPointF(bins.at(firstAge-1), lastSel));
        index = 4 + numChanges;
        for (int i = 0; i < numChanges; i++) {
            parm = parameterView->getInput(index+i);
            if (parm < -998) {
                if (parm < -999.5)
                    parm = parameterView->getInput(index+i-numChanges);
                else
                    parm = parameterView->getInput(index+i-1);
                parameterView->setSliderValue(index+i, parm);
            }
            lastSel += parm;
            secndPoints.append(QPointF(bins.at(firstAge+i), lastSel));
        }
        secndPoints.append(QPointF(bins.last(), lastSel));
        templimit = minYvalue(secndPoints);
        if (templimit < altYmin)
            altYmin = templimit;
        templimit = maxYvalue(secndPoints);
        if (templimit > altYmax)
            altYmax = templimit;

        for (int i = 0; i < bins.count(); i++) {
            parm = evaluatePoints(secndPoints, bins.at(i));
            dscendSeries->append(bins.at(i), parm);
        }
        temp = aveYvalue(dscendSeries->points(), (loBin-1), (hiBin-1));

        // join3Series = exp(dscendSeries-temp) * exp(maleMeanRatio)
        join3Series->append(0, 0);
        for (int i = 0; i < xValList.count(); i++) {
            parm = evaluatePoints(secndPoints, xValList.at(i));
            join3Series->append(xValList.at(i), exp((parm - temp))*(maleMeanRatio));
        }
    }
    axisYalt->setRange(altYmin, altYmax);
    setLimits(bins.at(loBin-1), bins.at(hiBin-1));
}

// 45 //  like age selex 14 but with separate parameters for males and with revised controls
  //  parameter value is logit(selectivity)
  //  peak selex in logit space is penalized towards value of 8.0, which gives selex near 1.0
  //  sp(1) is first age with non-zero selectivity; can be age 0
  //  seltype(f,4) is number of selectivity parameters beginning at age=first_age
  //  sp(2) is first age in mean for peak selex
  //  sp(3) is last age  in mean for peak selex
  //  sp(4) is male mean selex relative to female mean.
  //  -999 code means to keep the selex same as previous age
  // -1000 code is only for males and sets the male selex to be same as female selex
  //  gg is the index for sex, where 1=female, 2=male
void DialogSelexEquationView::twoSexEachAge() {
    setLabel(QString("Pattern %1: Revise Age - female/male").arg(
                 QString::number(equationNum)));
    int index = 3 + (genders > 1? 1:0);
    int numChanges = special;
    int num = 2 * numChanges + index;
    if (parameters->rowCount() == num) {
        numParams = num;
        parameterView->setNumParamsShown(numParams);
        showBins(true);
        setParameterHeaders();

        parameterView->setName(0, QString("First Age Bin"));
        parameterView->setType(0, QString("Integr"));
        parameterView->setName(1, QString("Scale Low Bin"));
        parameterView->setType(1, QString("Integr"));
        parameterView->setName(2, QString("Scale High Bin"));
        parameterView->setType(2, QString("Integr"));
        parameterView->setName(3, QString("Male ln(ratio)"));
        parameterView->setType(3, QString("Exp"));

        for (int i = 0; i < numChanges; i++) {
            parameterView->setName(index+i, QString("Fem ln(slx) change %1").arg(QString::number(i+1)));
            parameterView->setType(index+i, QString("Value"));
        }
        cht->removeSeries(valSeries);

        axisYalt->setTitleText("Param Value(blue)");
        axisYalt->setRange(0, 1.0);
        cht->addAxis(axisYalt, Qt::AlignRight);
        cht->legend()->show();
        cht->legend()->setAlignment(Qt::AlignLeft);

        setupLimits();

        // female ln(selex)
        ascendSeries->setPen(QPen(QBrush(Qt::blue), 2));
        ascendSeries->setName(QString("Fem ln(slx)"));
        cht->addSeries(ascendSeries);
        ascendSeries->attachAxis(axisX);
        ascendSeries->attachAxis(axisYalt);

        // female selex
        valSeries->setName(QString("Fem slx (red)"));
        cht->addSeries(valSeries);
        valSeries->attachAxis(axisX);
        valSeries->attachAxis(axisY);

        if (genders > 1) {
            index = 4 + numChanges;
            for (int i = 0; i < numChanges; i++) {
                parameterView->setName(index+i, QString("Male ln(slx) change %1").arg(QString::number(i+1)));
                parameterView->setType(index+i, QString("Value"));
            }

            // male ln(selex)
            dscendSeries->setPen(QPen(QBrush(Qt::green), 2));
            dscendSeries->setName(QString("Mal ln(slx)"));
            cht->addSeries(dscendSeries);
            dscendSeries->attachAxis(axisX);
            dscendSeries->attachAxis(axisYalt);

            // male selex
            join3Series->setPen(QPen(QBrush(Qt::magenta), 5));
            join3Series->setName(QString("Mal slx (mag)"));
            cht->addSeries(join3Series);
            join3Series->attachAxis(axisX);
            join3Series->attachAxis(axisY);
        }
        updateTwoSexEachAge();
    }
}

//	sel_a(y,fs,gg).initialize();
//	tempvec_a.initialize(); //  null value for vector
//	int first_age=int(value(sp(1)));
//	int last_age=first_age+seltype(f,4)-1;  //  because seltype(f,4) contains the number of ages with selex parameter
//	if(gg==1)
//	{  // to get male vs female starting point for parameters
//		scaling_offset=2+gender;
//  } else {
//      scaling_offset=2+gender+seltype(f,4);
//	}
//	j=scaling_offset;
//	for (a=first_age;a<=last_age;a++)
//	{
//		j++;
//		if(sp(j)>-999)
//		{
//			tempvec_a(a) = sp(j);
//		}
//		else if(gg==2 && sp(j)==-1000.)  //  set male selectivity raw value same as female
//		{
//			tempvec_a(a)=sp(j-seltype(f,4));  //  use female parameter for males
//		}
//		else  //  so value is -999 so set to next younger age
//		{
//			tempvec_a(a) = tempvec_a(a-1);
//		}
//	}
//	int low_bin  = int(value(sp(2)));
//	int high_bin = int(value(sp(3)));
//	temp=8.-mean(tempvec_a(low_bin,high_bin));
//	sel_a(y,fs,gg)(first_age,last_age) = 1./(1.+mfexp(-(tempvec_a(first_age,last_age)+temp)));
//	if(gg==2)
//	{ //  apply male ratio
//		sel_a(y,fs,gg)(first_age,last_age)*=mfexp(sp(4));
//	}
//	if (last_age<nages)
//	{
//		sel_a(y,fs,gg)(last_age+1,nages)=sel_a(y,fs,gg,last_age);
//	}
void DialogSelexEquationView::updateTwoSexEachAge() {
    int index = 0;
    double altYmin = 0, altYmax = 1;
    double parm = 0.;
    double lastSel = 0;
    double temp = 0;
    double templimit = 0;
    QList<QPointF> secndPoints;
    double yVal;
    int numChanges = special;
    int firstAge = static_cast<int>(parameterView->getInput(0));
    int lastAge = firstAge + numChanges;
    double firstBin = parameterView->getInput(1);
    double lastBin = parameterView->getInput(2);
    double maleMeanRatio = parameterView->getInput(3);
    checkScaleSliders(1, 2, bins, firstBin, lastBin);
    int loBin = static_cast<int>(firstBin);
    int hiBin = static_cast<int>(lastBin);
    firstPoints.clear();
    ascendSeries->clear();
    valSeries->clear();
    secndPoints.clear();
    dscendSeries->clear();
    join3Series->clear();

    if (lastAge > bins.count()) {
        firstAge = bins.count() - numChanges;
        parameterView->setSliderValue(0, firstAge);
    }

    // Female ln(selex)
    // parameter values
    firstPoints.append(QPointF(0, 0));
    firstPoints.append(QPointF(bins.at(firstAge-1), 0));
    index = 2 + (genders > 1? 2: 1);
    for (int i = 0; i < numChanges; i++) {
        parm = parameterView->getInput(index+i);
        if (parm < -998) {
            parm = parameterView->getInput(index+i-1);
            parameterView->setSliderValue(index+i, parm);
        }
        firstPoints.append(QPointF(bins.at(firstAge+i), parm));
    }
    firstPoints.append(QPointF(bins.last(), parm));
    altYmin = minYvalue(firstPoints);
    altYmax = maxYvalue(firstPoints);

    for (int i = 0; i < bins.count(); i++) {
        parm = evaluatePoints(firstPoints, bins.at(i));
        ascendSeries->append(bins.at(i), parm);
    }
    temp = 8.0 - aveYvalue(ascendSeries->points(), (loBin-1), (hiBin-1));

    // valSeries = exp(ascendSeries)
    valSeries->append(0, 0);
    for (int i = 0; i < xValList.count(); i++) {
        if (xValList.at(i) < bins.at(firstAge)) {
            yVal = 0.0;
        }
        else {
            parm = evaluatePoints(firstPoints, xValList.at(i));
            yVal = logist(parm + temp);
        }
        valSeries->append(xValList.at(i), yVal);
    }
    yMax = maxYvalue(valSeries->points());
    if (yMax > 1.0001)
        axisY->setRange(0, yMax * 1.2);

    if (genders > 1) {
        // Male ln(selex)
        // parameter values
        lastSel = 0.0;
        secndPoints.append(QPointF(0, 0));
        secndPoints.append(QPointF(bins.at(firstAge-1), 0));
        index = 4 + numChanges;
        for (int i = 0; i < numChanges; i++) {
            parm = parameterView->getInput(index+i);
            if (parm < -998) {
                if (parm < -999.5)
                    parm = parameterView->getInput(index+i-numChanges);
                else
                    parm = parameterView->getInput(index+i-1);
                parameterView->setSliderValue(index+i, parm);
            }
            secndPoints.append(QPointF(bins.at(firstAge+i), parm));
        }
        secndPoints.append(QPointF(bins.last(), parm));
        templimit = minYvalue(secndPoints);
        if (templimit < altYmin)
            altYmin = templimit;
        templimit = maxYvalue(secndPoints);
        if (templimit > altYmax)
            altYmax = templimit;

        for (int i = 0; i < bins.count(); i++) {
            parm = evaluatePoints(secndPoints, bins.at(i));
            dscendSeries->append(bins.at(i), parm);
        }
        temp = 8.0 - aveYvalue(dscendSeries->points(), (loBin-1), (hiBin-1));

        // join3Series = exp(dscendSeries-temp) * maleMeanRatio // already exp'd
        join3Series->append(0, 0);
        for (int i = 0; i < xValList.count(); i++) {
            if (xValList.at(i) < bins.at(firstAge))  {
                yVal = 0.0;
            }
            else {
                parm = evaluatePoints(secndPoints, xValList.at(i));
                yVal = logist(parm + temp)*(maleMeanRatio);
            }
            join3Series->append(xValList.at(i), yVal);
        }
    }
    axisYalt->setRange(altYmin, altYmax);
    setLimits(bins.at(loBin-1), bins.at(hiBin-1));
}

// check scale bin limits and adjust as necessary
bool DialogSelexEquationView::checkScaleSliders(int first, int secnd, QList<float> bins, double &binLo, double &binHi) {
    bool okay = true;
    int lo = static_cast<int>(binLo);
    int hi = static_cast<int>(binHi);

    if (lo <= -1) {  // legitimate input
        lo = 1;
        binLo = 1;
    }
    if (hi <= -1) {  // legitimate input
        hi = bins.count();
        binHi = hi;
    }
    if (lo < 1) {
        okay = false;
        setMessage(tr("Minimum for param 1 is first bin, adjusting to first bin..."));
        binLo = 1;
        parameterView->setSliderValue(first, 1);
    }
    else if (hi < lo) {
        okay = false;
        setMessage(tr("Maximum is less than Minimum, adjusting to minimum..."));
        binHi = lo;
        parameterView->setSliderValue(secnd, lo);
    }
    else if (hi > bins.count()) {
        okay = false;
        setMessage(tr("Maximum for param 2 is last bin, adjusting to number of bins..."));
        binHi = bins.count();
        parameterView->setSliderValue(secnd, bins.count());
    }
    else if (lo > hi) {
        okay = false;
        setMessage(tr("Minimum is greater than Maximum, adjusting to maximum..."));
        binLo = hi;
        parameterView->setSliderValue(first, hi);
    }

    return okay;
}


double DialogSelexEquationView::aveYvalue(const QList<QPointF> &pointlist, int lowBin, int highBin)
{
    int start = 0, stop = pointlist.count() - 1;
    double xval1 = bins.at(lowBin)-.000001;
    double xval2 = bins.at(highBin)+.000001;

    while (pointlist.at(start).x() < xval1)
        start++;
//    start -= 1;
    while (pointlist.at(stop).x() > xval2)
        stop--;
//    stop += 1;

    return DialogEquationView::aveYvalue(pointlist, start, stop);
}
