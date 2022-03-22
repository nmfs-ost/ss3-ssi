#include "selectivity.h"
#include "selex_equation.h"
#include "fleet.h"
#include "model.h"

selectivity::selectivity(QObject *model, int method)
    : QObject (model)
{
    data_model = static_cast<ss_model *>(model);
    setup = new setupModel();
    QStringList hdr;
    hdr << tr("Pattern") << tr("Discard") << tr("Male") << tr("Special");
    setup->setHeader(hdr);

    parameters = new longParameterModel();
    varParameters = new timeVaryParameterModel(model);

    discardParameters = new longParameterModel();
    discardVarParameters = new timeVaryParameterModel(model);
    retainParameters = new longParameterModel();
    retainVarParameters = new timeVaryParameterModel(model);
    maleParameters = new longParameterModel();
    maleVarParameters = new timeVaryParameterModel(model);
    connectSigs();

    reset();
    setMethod(method);
}

selectivity::selectivity(QObject *model, SelexType stype, int method)
    : QObject (model)
{
    data_model = static_cast<ss_model *>(model);
    type = stype;
    setup = new setupModel();
    QStringList hdr;
    hdr << tr("Pattern") << tr("Discard") << tr("Male") << tr("Special");
    setup->setHeader(hdr);

    parameters = new longParameterModel();
    varParameters = new timeVaryParameterModel(model);

    discardParameters = new longParameterModel();
    discardVarParameters = new timeVaryParameterModel(model);
    retainParameters = new longParameterModel();
    retainVarParameters = new timeVaryParameterModel(model);
    maleParameters = new longParameterModel();
    maleVarParameters = new timeVaryParameterModel(model);
    connectSigs();

    reset();
    setMethod(method);
}

void selectivity::reset()
{
    QStringList values;
    varParameters->setNumVarParams(0);
    discardVarParameters->setNumVarParams(0);
    retainVarParameters->setNumVarParams(0);
    maleVarParameters->setNumVarParams(0);
    values << "0" << "0" << "0" << "0";
    setSetup(values);
    numXvals = 0;
    numAges = 0;
    minSel = 0;
}

void selectivity::connectSigs()
{
    connect (setup, SIGNAL(dataChanged(QList<int>)),
             SLOT(changeSetup(QList<int>)));
    connect (parameters, SIGNAL(paramChanged(int,QStringList)),
             varParameters, SLOT(changeVarParamData(int,QStringList)));
    connect (discardParameters, SIGNAL(paramChanged(int,QStringList)),
             discardVarParameters, SLOT(changeVarParamData(int,QStringList)));
    connect (retainParameters, SIGNAL(paramChanged(int,QStringList)),
             retainVarParameters, SLOT(changeVarParamData(int,QStringList)));
    connect (maleParameters, SIGNAL(paramChanged(int,QStringList)),
             maleVarParameters, SLOT(changeVarParamData(int,QStringList)));
}

void selectivity::disconnectSigs()
{
    disconnect (setup, SIGNAL(dataChanged(QList<int>)),
             this, SLOT(changeSetup(QList<int>)));
    disconnect (parameters, SIGNAL(paramChanged(int,QStringList)),
             varParameters, SLOT(changeVarParamData(int,QStringList)));
    disconnect (discardParameters, SIGNAL(paramChanged(int,QStringList)),
             discardVarParameters, SLOT(changeVarParamData(int,QStringList)));
    disconnect (retainParameters, SIGNAL(paramChanged(int,QStringList)),
             retainVarParameters, SLOT(changeVarParamData(int,QStringList)));
    disconnect (maleParameters, SIGNAL(paramChanged(int,QStringList)),
             maleVarParameters, SLOT(changeVarParamData(int,QStringList)));
}

selectivity::~selectivity()
{
    delete maleVarParameters;
    delete maleParameters;
    delete retainVarParameters;
    delete retainParameters;
    delete discardVarParameters;
    delete discardParameters;
    delete varParameters;
    delete parameters;
    delete setup;
}

void selectivity::setSetup(QString text)
{
    QStringList strList(text.split(' '));
    setSetup(strList);
}
void selectivity::setSetup(QStringList strList)
{
    setSpecial(strList.at(3).toInt());
    setDiscard(strList.at(1).toInt());
    setPattern(strList.at(0).toInt());
    setMale(strList.at(2).toInt());
    if (getNumBins() > 0)
        setDefaultParams();
}
void selectivity::setSetup(QList<int> values)
{
    setSpecial(values.at(3));
    setDiscard(values.at(1));
    setMethod (values.at(0));
    setMale   (values.at(2));
    if (getNumBins() > 0)
        setDefaultParams();
}

void selectivity::setTVautogenerate(int val)
{
    varParameters->setAutoGenerate(val);
    retainVarParameters->setAutoGenerate(val);
    discardVarParameters->setAutoGenerate(val);
    maleVarParameters->setAutoGenerate(val);
}

void selectivity::changeTVautogenerate(int val)
{
    varParameters->setAutoGenerate(val);
    for (int i = 0; i < getNumParameters(); i++)
        varParameters->changeVarParamData(i, getParameter(i));
    retainVarParameters->setAutoGenerate(val);
    for (int i = 0; i < getNumRetainParameters(); i++)
        retainVarParameters->changeVarParamData(i, getRetainParameter(i));
    discardVarParameters->setAutoGenerate(val);
    for (int i = 0; i < getNumDiscardParameters(); i++)
        discardVarParameters->changeVarParamData(i, getDiscardParameter(i));
    maleVarParameters->setAutoGenerate(val);
    for (int i = 0; i < getNumMaleParameters(); i++)
        maleVarParameters->changeVarParamData(i, getMaleParameter(i));
}

QString selectivity::getSetupText()
{
    QString text("");
    text.append(QString(" %1 %2 %3 %4").arg(
                QString::number(setup->getValue(0)),
                QString::number(setup->getValue(1)),
                QString::number(setup->getValue(2)),
                QString::number(setup->getValue(3))));
    return text;
}

void selectivity::changeSetup(QList<int> values)
{
    Q_UNUSED(values);
    emit setupChanged(getSetup());
}

void selectivity::setMethod(int method)
{
    int pattn = setup->getValue(0);
    int special = setup->getValue(3);
    if (method == pattn)
    {
        if (method == 0 || method == 10)
            setNumParameters(0);
        else
            setDefaultParams(method, special);
    }
    else
    {
        setPattern (method);
    }
}

void selectivity::setNumBinVals(int num)
{
    if (numXvals != num)
    {
        numXvals = num;
        if (type == Age)
        {
            bins.clear();
            for (int i = 0; i <= numXvals; i++)
                bins.append(QString::number(i));
            setBinVals(bins);
        }
    }
}


void selectivity::setPattern(int value)
{
    if (setup->getValue(0) != value)
    {
        setup->setValue(0, value);
        setMethod(value);
    }
}

void selectivity::setDefaultParams(int method, int special)
{
    int numparam;
    QStringList parm;
    int i, j, binmid = 1;
    double val;
    double xmin = 1, xmax = 50, peak = 25;
    double x1, x2;
    QString Count, xMin, xMax, Peak;
    QString BinCount, BinMid;
    QString xLo, xHi;
    QString midcount(QString::number(static_cast<int>(binVals.count()/2)));
    QString Type;

    int count = bins.count();
    if (count == 0)
        return;

    if (method < 0)
        method = getPattern();
    if (special < 0)
        special = getSpecial();

    if (type == Age) {
        Type = QString("Age");
        BinCount = QString::number(count);
        binmid = static_cast<int>(count/2);
        BinMid = QString::number(binmid);
        xmin = bins.first().toDouble();
        xMin = bins.first();
        xmax = bins.last().toDouble();
        xMax = bins.last();
    }
    else {
        Type = QString("Size");
        BinCount = QString::number(count);
        binmid = static_cast<int>(count/2);
        BinMid = QString::number(binmid);
        xmin = bins.first().toDouble();
        xMin = bins.first();
        xmax = bins.last().toDouble();
        xMax = bins.last();
    }
    if (parameters == nullptr)
        return;

    peak = (xmin + xmax) / 2.0;

    x1 = (xmin + peak) / 2.0;
    x2 = (peak + xmax) / 2.0;
    Count = QString::number(numAges);
    Peak = QString::number(peak, 'f', 2);
    xLo = QString::number(x1, 'f', 2);
    xHi = QString::number(x2, 'f', 2);

    disconnectSigs();
    emit startingSetupChanges();
    switch (method)
    {
    case 0:  // case 0 constant size selectivity = 1.0
    case 10: // case 10 Constant age-specific selex for ages 1 to nages
    {
        setNumParameters(0);
        break;
    }
    case 11:  // case 11 Constant age-specific selex for specified age range
    {
        setNumParameters(2);
        parm << "1" << BinCount << BinMid << "0" << "0" << "0" << "-2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm[2] = BinCount;
        setParameter(1, parm);
        setParameterLabel(0, QString("%1Selex LoBin").arg(Type));
        setParameterLabel(1, QString("%1Selex HiBin").arg(Type));
        break;
    }
    case 1:  // case 1 logistic size selectivity
    case 12:
    {
        setNumParameters(2);
        parm << xMin << xMax << Peak << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm[0] = QString(".01");
        parm[1] = QString("60");
        parm[2] = QString("18.9");
        setParameter(1, parm);
        setParameterLabel(0, QString("%1Selex Inflection").arg(Type));
        setParameterLabel(1, QString("%1Selex 95%width").arg(Type));
        break;
    }
    case 3:  // case 3 discontinued
    case 4:  // case 4 discontinued; use pattern 30 to get spawning biomass
    {
        setNumParameters(0);
        break;
    }
    case 5:  // case 5 mirror another fleets size selectivity for specified bin range
    {
        setNumParameters(2);
//        QString bin2(QString::number(xVals.count()));
        parm << "1" << BinCount << "1" << "0" << "0" << "0" << "-99" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        setParameterLabel(0, QString("%1Selex LoBin %2").arg(Type, fisheryName));
        parm[2] = BinCount;
        setParameter(1, parm);
        setParameterLabel(1, QString("%1Selex HiBin %2").arg(Type, fisheryName));
        break;
    }
    case 6:  // case 6 non-parametric size selex pattern
    {
        if (special < 3) {
            special = 3;
        }
        if (special != getSpecial()) {
            setup->setValue(3, special);
        }
        numparam = 2 + special;
        setNumParameters(numparam);

        parm << xMin << xMax << Peak << "0" << "0" << "0" << "-99" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm[2] = QString::number(peak + 10.0);
        setParameter(1, parm);
        parm.clear();
        parm << "-5" << "9" << ".01" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        for (i = 2; i < numparam; i++)
            setParameter(i, parm);
        break;
    }
    case 2:  // case 2 discontinued; use pattern 8 for double logistic (size)
    case 7:  // case 7 discontinued; use pattern 8 for double logistic
    case 8:  // case 8 double logistic with eight parameters
//    case 18: // 18 #age selectivity: double logistic with smooth transition
        // 1=peak, 2=init, 3=infl, 4=slope, 5=final, 6=infl2, 7=slope2 8=binwidth;    Mirror=1===const_above_Linf
    {
        setNumParameters(8);
        parm << xMin << xMax << Peak << "0" << "0" << "0" << "-99" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm.clear();
        parm << "0" << "1" << ".01" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(1, parm);
        parm.clear();
        parm << "0" << "3" << "1.5" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(2, parm);
        parm[2] = QString("2.3");
        setParameter(5, parm);
        parm.clear();
        parm << "-5" << "3" << "-.6" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(3, parm);
        parm[2] = QString("0.06");
        setParameter(6, parm);
        parm.clear();
        parm << "-5" << "9" << "2" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(4, parm);
        parm.clear();
        parm << "1" << "20" << "2" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(7, parm);
        break;
    }
    case 18: // 18 #age selectivity: double logistic with smooth transition
        // 1=peak, 2=init, 3=infl, 4=slope, 5=final, 6=infl2, 7=slope2 8=binwidth;    Mirror=1===const_above_Linf
    {
        setNumParameters(8);
        parm << xMin << xMax << xLo << "0" << "0" << "0" << "-99" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm.clear();
        parm << "0" << "1" << ".01" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(1, parm);
        parm.clear();
        parm << "0" << "3" << "1.3" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(2, parm);
        parm[2] = QString("2.3");
        setParameter(5, parm);
        parm.clear();
        parm << "-5" << "3" << "1.3" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(3, parm);
        setParameter(6, parm);
        parm.clear();
        parm << "-5" << "9" << "2" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(4, parm);
        parm.clear();
        parm << "1" << "20" << "2" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(7, parm);
        break;
    }
    case 9:  // case 9 old double logistic with six parameters
    case 19: // simple double logistic
    {
        setNumParameters(6);
        parm << xMin << xMax << xLo << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
//        parm[0] = QString("0");
        parm[2] = xHi;
        setParameter(2, parm);
/*        parm[0] = QString("1");
        parm[1] = QString::number(xVals.count());
        parm[2] = QString("1");*/
        parm.clear();
        parm << "0" << "9" << "0.3" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(1, parm);
        parm.clear();
        parm << "-1" << "1" << "-0.2" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(3, parm);
        parm.clear();
        if (method == 9) {
            parm << "1" << Count << "2" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        }
        else {
            parm << xMin << xMax << xMin << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        }
        setParameter(4, parm);
        parm.clear();
        parm << "0" << "1" << "0" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(5, parm);
        break;
    }
    case 13:  // double logistic
    {
        setNumParameters(8);
        parm << xMin << xMax << xLo << "0" << "0" << "0" << "-99" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm.clear();
        parm << "0" << "1" << ".01" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(1, parm);
        parm.clear();
        parm << "-5" << "5" << "1" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(2, parm);
        parm.clear();
        parm << "-5" << "3" << "-1" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(3, parm);
        setParameter(6, parm);
        parm.clear();
        parm << "-5" << "9" << "2" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(4, parm);
        setParameter(5, parm);
        parm.clear();
        parm << xMin << xMax << Peak << "0" << "0" << "0" << "-99" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(7, parm);
        break;
    }
    case 14:  // case 14 separate parm for each age
    {
        numparam = numAges + 1;
        setNumParameters(numparam);

        parm << "-5" << "9" << "-5" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm[2] = QString("2");
        for (int i = 1; i < numparam; i++)
            setParameter(i, parm);
        break;
    }
    case 15:  // case 15 mirror another fleets size selectivity for all size bins
    {
        setNumParameters(0);
        break;
    }
    case 16:  // case 16 Coleraine - Gaussian age selectivity
    {
        setNumParameters(2);
        parm << "-5" << "5" << "0" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm.clear();
        parm << "-5" << "10" << "2" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(1, parm);
        break;
    }
    case 17:  // 17 #age selectivity: each age has parameter as random walk
        //    transformation as selex=exp(parm); some special codes
    {
        if (special > 0)
            numparam = special + 1;
        else
            numparam = numAges + 1;
        setNumParameters(numparam);
        parm << "-10" << "5" << "-1000" << "0" << "0" << "0" << "-99" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm.clear();
        parm << "-10" << "10" << ".1" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        for (i = 1; i < numparam; i++)
            setParameter(i, parm);
        break;
    }
    case 21: // case 21 non-parametric size selectivity
        /*  N points; where the first N parameters is vector of sizes for the line segment ends
           and second N parameters is selectivity at that size (no transformations) */
    {
        if (special < 2)
        {
            special = 2;
            setup->setValue(3, special);
        }
        numparam = special * 2;
        setNumParameters(numparam);
        val = (float)numXvals / special;
        parm.clear();
        parm << xMin << xMax << xLo << "0" << "0" << "0" << "-99" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        for (i = 0; i < special; i++)
        {
            parm[2] = QString::number(1.0 + i * val);
            setParameter(i, parm);
        }
        parm.clear();
        parm << "0" << "1" << "1.0" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        for (; i < numparam; i++)
        {
            setParameter(i, parm);
        }
        break;
    }
    case 20: // 20 #age selectivity: double normal with plateau
    case 23: // case 23 size selectivity double_normal_plateau where final value can be greater than 1.0
    case 24: // double normal with plateau and defined ends
    {
        setNumParameters(6);
        parm << xMin << xMax << xLo << "0" << "0" << "0" << "-99" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm.clear();
        parm << "-5" << "3" << "0" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(1, parm);
        parm.clear();
        parm << "-5" << "12" << "3" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(2, parm);
        parm.clear();
        parm << "-2" << "10" << "5" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(3, parm);
        parm.clear();
        parm << "-15" << "5" << "-2" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(4, parm);
        parm.clear();
        parm << "-5" << "5" << "0" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(5, parm);
        break;
    }
    case 22:  // case 22 size selectivity using double_normal_plateau (similar to CASAL)
    {
        setNumParameters(4);
        parm << xMin << xMax << xLo << "0" << "0" << "0" << "-99" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm.clear();
        parm << "-5" << "10" << "1.5" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(1, parm);
        parm.clear();
        parm << "0" << "10" << "4" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(2, parm);
        setParameter(3, parm);
        break;
    }
    case 25: // case 25 size selectivity using exponential-logistic
    case 26: // 26 #age selectivity: exponential logistic
    {
        setNumParameters(3);
        parm << ".02" << "1" << ".5" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm.clear();
        parm << ".001" << ".999" << ".5" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(1, parm);
        parm.clear();
        parm << ".001" << ".5" << ".01" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(2, parm);
        break;
    }
    case 27:  // case 27 size selectivity using cubic spline
    {
        if (special < 3)
        {
            special = 3;
            setSpecial(3);
        }
        else
        {
            numparam = special * 2 + 3;
            setNumParameters(numparam);
            parm << "0" << "2" << "0" << "0" << "0" << "0" << "-99" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(0, parm);
            autogenCubicSpline1();
            autogenCubicSpline2();
        }
        break;
    }
    case 30:  // equal to spawning biomass
    case 31:  // equal to exp(recruitment dev)
    case 32:  // equal to exp(rec dev) * spawn biomass
    case 33:  // expected survey abund = age 0 recruitment
    case 34:  // spawning biomass depletion (B/B0)
    {
        setNumParameters(0);
        break;
    }
    case 41: // case 41 each age has parameter as random walk scaled by average of values at low age through high age
        //    transformation as selex=exp(parm); some special codes
    {
        numparam = numAges + 1 + 2;
        if (parameters->getNumParams() != numparam)
        {
            setNumParameters(numparam);
        }
        parm << "1" << BinCount << BinMid << "0" << "0" << "0" << "-99" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm[2] = BinCount;
        setParameter(1, parm);
        parm.clear();
        parm << "-.001" << "5" << "-1000" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(2, parm);
        parm.clear();
        parm << "-10" << "10" << "0" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        for (i = 1; i < numparam; i++)
            setParameter(i, parm);
        break;
    }
    case 42: // case 42 size selectivity using cubic spline scaled by average of values at low bin through high bin
    {
        if (special < 3)
        {
            special = 3;
            setSpecial(3);
        }
        else
        {
            numparam = special * 2 + 3 + 2;
            setNumParameters(numparam);
            parm << "1" << BinCount  << BinMid << "0" << "0" << "0" << "-99" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(0, parm);
            parm[2] = BinCount;
            setParameter(1, parm);
            parm.clear();
            parm << "0" << "2" << "0" << "0" << "0" << "0" << "-99" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(2, parm);
            autogenCubicSpline1(2);
            autogenCubicSpline2(2);
        }
        break;
    }
    case 43: // case 43 non-parametric size selex scaled by average of values from low bin through high bin
    {
        if (special < 3) {
            special = 3;
            setSpecial(special);
        }
        else {
            numparam = special + 2 + 2;
            setNumParameters(numparam);

            parm.clear();
            parm << "1" << BinCount << BinMid << "0" << "0" << "0" << "-99" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(0, parm);
            parm[2] = BinCount;
            setParameter(1, parm);
            parm.clear();
            parm << xMin << xMax << xLo << "0" << "0" << "0" << "-99" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(2, parm);
            parm[2] = xHi;
            setParameter(3, parm);
            parm.clear();
            parm << "-5" << "9" << ".01" << "0" << "0" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            for (i = 4; i < numparam; i++)
                setParameter(i, parm);
        }
        break;
    }
    case 44: // case 44 like age selex 17 but with separate parameters for males and with revised controls
    {
        if (special < 1)
        {
            special = 1;
            setSpecial(special);
        }
        else {
            int index = 3 + (numGenders>1? 1:0);
            numparam = (special * numGenders) + index;
            setNumParameters(numparam);
            parm << "1" << BinCount << "1" << "0" << "0.5" << "0" << "-99" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(0, parm);
            parm[2] = BinMid;
            setParameter(1, parm);
            parm[2] = QString::number(binmid + 1);
            setParameter(2, parm);
            if (numGenders > 1)  {
                parm.clear();
                parm << "-1" << ".01" << "-.001" << "-.001" << "0.5" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
                setParameter(3, parm);
            }
            parm.clear();
            parm << "-10" << "10" << "3" << "3" << "0.5" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            for (i = index, j = 0; j < special; i++, j++)
                setParameter(i, parm);
            if (numGenders > 1)
            {
                parm.clear();
                parm << "-1000" << "10" << "1" << "-0.9" << "0.5" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
                for (j = 0; j < special; i++, j++)
                    setParameter(i, parm);
            }
        }
        break;
    }
    case 45: // 45 like age selex 14 but with separate parameters for males and with revised controls
    {
        if (special < 1)
        {
            special = 1;
            setSpecial(special);
        }
        else {
            numparam = (special * numGenders) + 4;
            if (parameters->getNumParams() != numparam)
            {
                setNumParameters(numparam);
            }
            parm << "1" << Count << "2" << "0" << "0.5" << "0" << "-99" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(0, parm);
            parm[2] = QString("5");
            setParameter(1, parm);
            setParameter(2, parm);
            if (numGenders > 1) {
                parm.clear();
                parm << "-1" << "2" << "-0.001" << "-0.001" << "0.5" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
                setParameter(3, parm);
            }
            parm.clear();
            parm << "-10" << "10" << "-5.0" << "-5.0" << "0.5" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            for (i = 2+numGenders, j = 0; j < special; i++, j++)
                setParameter(i, parm);
            if (numGenders > 1)
            {
                parm.clear();
                parm << "-1000" << "10" << "-1000" << "-0.9" << "0.5" << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
                for (j = 0; j < special; i++, j++)
                    setParameter(i, parm);
            }
        }
        break;
    }
    default:
    {
        setNumParameters(0);
    }
    }
    connectSigs();
    emit setupChanged(getSetup());

}

SelexType selectivity::getType() const
{
    return type;
}

void selectivity::setType(const SelexType &value)
{
    type = value;
}

QList<float> selectivity::getBinVals() const
{
    return binVals;
}

void selectivity::setBinVals(const QStringList &value)
{
    binVals.clear();
    for (int i = 0; i < value.count(); i++)
        binVals.append(QString(value.at(i)).toFloat());
}

float selectivity::getBinValue(int index) {
    return binVals[index];
}

void selectivity::setBins (const QStringList &value)
{
    bins = QStringList(value);
    setBinVals(value);
}

void selectivity::setAltBins (const QStringList &value)
{
    altbins = QStringList(value);
}

int selectivity::getNumGenders() const
{
    return numGenders;
}

void selectivity::setNumGenders(int value)
{
    numGenders = value;
//    setDefaultParams(getPattern(), getSpecial());
}

int selectivity::getNumParameters()
{
    return parameters->getNumParams();
}

void selectivity::setNumParameters(int num)
{
    int curr = getNumParameters();
    if (curr != num)
    {
        parameters->setNumParams(num);
    }
    varParameters->setTotalNumVarTables(num);
}

void selectivity::setParameter(int index, QString text)
{
    QStringList strList (text.split(' '));
    setParameter(index, strList);
}

void selectivity::setParameter(int index, QStringList strList)
{
    parameters->setParameter(index, strList);
    varParameters->setParameter(index, strList);
}


QString selectivity::getParameterText(int index)
{
    QString text("");
    QStringList strList(parameters->getParameter(index));
    for (int i = 0; i < strList.count(); i++)
        text.append(QString(" %1").arg(strList.at(i)));

    return text;
}

QStringList selectivity::getParameter(int index)
{
    return parameters->getParameter(index);
}

void selectivity::setParameterLabel(int index, QString label)
{
    parameters->setParamHeader(index, label);
    varParameters->setTableTitle(index, label);
}

QString selectivity::getParameterLabel(int index)
{
    return parameters->getParamHeader(index);
}

void selectivity::setDiscard(int value)
{
    int discard = setup->getValue(1);
    if (discard != value)
    {
//        emit startingSetupChanges();
        setup->setValue(1, value);
        switch (value)
        {
        case 1:
            retainParameters->setNumParams(4);
            discardParameters->setNumParams(0);
            for (int i = 0; i < 4; i++)
            {
                setRetainParameterLabel(i, QString("Retain_P%1_%2(%3)").arg(
                                               QString::number(i+1),
                                               fisheryName,
                                               QString::number(fisheryNum)));
            }
            break;
        case 2:
            retainParameters->setNumParams(4);
            discardParameters->setNumParams(4);
            for (int i = 0; i < 4; i++)
            {
                setRetainParameterLabel(i, QString("Retain_P%1_%2(%3)").arg(
                                               QString::number(i+1),
                                               fisheryName,
                                               QString::number(fisheryNum)));
            }
            for (int i = 0; i < 4; i++)
            {
                setDiscardParameterLabel(i, QString("Discard_P%1_%2(%3)").arg(
                                               QString::number(i+1),
                                               fisheryName,
                                               QString::number(fisheryNum)));
            }
            break;
        case 3:
            retainParameters->setNumParams(0);
            discardParameters->setNumParams(0);
            break;
        case 4:
            retainParameters->setNumParams(7);
            discardParameters->setNumParams(4);
            for (int i = 0; i < 7; i++)
            {
                setRetainParameterLabel(i, QString("Retain_P%1_%2(%3)").arg(
                                               QString::number(i+1),
                                               fisheryName,
                                               QString::number(fisheryNum)));
            }
            for (int i = 0; i < 4; i++)
            {
                setDiscardParameterLabel(i, QString("Discard_P%1_%2(%3)").arg(
                                               QString::number(i+1),
                                               fisheryName,
                                               QString::number(fisheryNum)));
            }
            break;
        default:
            retainParameters->setNumParams(0);
            discardParameters->setNumParams(0);
        }
//        emit dataChanged();
    }
}

void selectivity::setMale(int value)
{
    int num = 0;
    int male = setup->getValue(2);
    QString gender("Male");
    if (male != value)
    {
//        emit startingSetupChanges();
        setup->setValue(2, value);
        switch (value)
        {
        case 1:
            num = 4;
            break;
        case 2:
            num = 4;
            gender = QString("Fem");
            break;
        case 3:
            if (getPattern() == 1)
                num = 1;
            if (getPattern() == 20 || getPattern() == 24)
                num = 5;
            break;
        case 4:
            if (getPattern() == 1)
                num = 1;
            if (getPattern() == 20 || getPattern() == 24)
                num = 5;
            gender = QString("Fem");
            break;
        default:
            num = 0;
        }
        maleParameters->setNumParams(num);
        for (int i = 0; i < num; i++)
        {
            setMaleParameterLabel(i, QString("%1_P%2_%3(%4)").arg(
                                           gender,
                                           QString::number(i+1),
                                           fisheryName,
                                           QString::number(fisheryNum)));
        }
//        emit dataChanged();
    }
}

void selectivity::setSpecial(int value)
{
    int special = setup->getValue(3);
    int scale = 0;
    if (special != value)
    {
        emit startingSetupChanges();
        switch(getPattern())
        {
        case 43:
            scale = 2;
            [[clang::fallthrough]];
        case 6:
            if (value < 2)
            {
                value = 2;
            }
            setNumParameters(scale + 2 + value);
            break;

        case 42:
            scale = 2;
            [[clang::fallthrough]];
        case 27:
            if (value < 3)
            {
                value = 3;
            }
            setNumParameters(scale + 3 + value * 2);
            break;
        }
        setup->setValue(3, value);
        setDefaultParams(getPattern(), getSpecial());
    }
}

void selectivity::setTimeVaryParameter (int index, QStringList strList)
{
    varParameters->setVarParameter(index, strList);
}

void selectivity::setTimeVaryParameterLabel(int index, QString label)
{
    varParameters->setVarParamHeader(index, label);
}

QString selectivity::getTimeVaryParameterText (int index)
{
    QString text("");
    QStringList ql (varParameters->getVarParameter(index));
    for (int i = 0; i < 7; i++)
        text.append(QString (" %1").arg(ql[i]));
    return text;
}

int selectivity::getNumTimeVaryParameters ()
{
    return varParameters->getNumVarParams();
}

int selectivity::getNumDiscardParameters()
{
    return discardParameters->getNumParams();
}

void selectivity::setDiscardParameter(int index, QStringList strList)
{
    discardParameters->setParameter(index, strList);
}

void selectivity::setDiscardParameterLabel(int index, QString label)
{
    discardParameters->setParamHeader(index, label);
}

QString selectivity::getDiscardParameterText (int index)
{
    QStringList data = discardParameters->getParameter(index);
    QString text (data.at(0));
    for (int i = 1; i < data.count(); i++)
        text.append(QString(" %1").arg(data.at(i)));
    return text;
}

void selectivity::setDiscardTimeVaryParameter (int index, QStringList strList)
{
    discardVarParameters->setVarParameter(index, strList);
}

void selectivity::setDiscardTimeVaryParameterLabel(int index, QString label)
{
    discardVarParameters->setVarParamHeader(index, label);
}

QString selectivity::getDiscardTimeVaryParameterText (int index)
{
    QString text("");
    QStringList ql (discardVarParameters->getVarParameter(index));
    for (int i = 0; i < 7; i++)
        text.append(QString (" %1").arg(ql[i]));
    return text;
}

int selectivity::getNumDiscardTimeVaryParameters ()
{
    return discardVarParameters->getNumVarParams();
}

void selectivity::setRetainParameter(int index, QStringList strList)
{
    retainParameters->setParameter(index, strList);
}

void selectivity::setRetainParameterLabel(int index, QString label)
{
    retainParameters->setParamHeader(index, label);
}

QString selectivity::getRetainParameterText (int index)
{
    QStringList data = retainParameters->getParameter(index);
    QString text (data.at(0));
    for (int i = 1; i < data.count(); i++)
        text.append(QString(" %1").arg(data.at(i)));
    return text;
}

void selectivity::setRetainTimeVaryParameter (int index, QStringList strList)
{
    retainVarParameters->setVarParameter(index, strList);
}

void selectivity::setRetainTimeVaryParameterLabel(int index, QString label)
{
    retainVarParameters->setVarParamHeader(index, label);
}

QString selectivity::getRetainTimeVaryParameterText (int index)
{
    QString text("");
    QStringList ql (retainVarParameters->getVarParameter(index));
    for (int i = 0; i < 7; i++)
        text.append(QString (" %1").arg(ql[i]));
    return text;
}

int selectivity::getNumRetainTimeVaryParameters ()
{
    return retainVarParameters->getNumVarParams();
}

int selectivity::getNumMaleParameters()
{
    return maleParameters->getNumParams();
}

void selectivity::setMaleParameter(int index, QStringList strList)
{
    maleParameters->setParameter(index, strList);
}

void selectivity::setMaleParameterLabel(int index, QString label)
{
    maleParameters->setParamHeader(index, label);
}

QString selectivity::getMaleParameterText (int index)
{
    QStringList data = maleParameters->getParameter(index);
    QString text (data.at(0));
    for (int i = 1; i < data.count(); i++)
        text.append(QString(" %1").arg(data.at(i)));
    return text;
}

void selectivity::setMaleTimeVaryParameter (int index, QStringList strList)
{
    maleVarParameters->setVarParameter(index, strList);
}

void selectivity::setMaleTimeVaryParameterLabel(int index, QString label)
{
    maleVarParameters->setVarParamHeader(index, label);
}

QString selectivity::getMaleTimeVaryParameterText (int index)
{
    QString text("");
    QStringList ql (maleVarParameters->getVarParameter(index));
    for (int i = 0; i < 7; i++)
        text.append(QString (" %1").arg(ql[i]));
    return text;
}

int selectivity::getNumMaleTimeVaryParameters ()
{
    return maleVarParameters->getNumVarParams();
}

/*
double selectivity::evaluate()
{
    double val = 0;
    return val;
}

double selectivity::evaluate(int f, float m)
{
    double val;
    if (equation == nullptr)
        val = 0.0;
    else
        val = equation->evaluate();
    return val;
}*/

// autogenerate Parameters if setup parameter = 1, 2, 11, or 12
void selectivity::autogenParameters(int flg) {
//    int pat = getPattern();
    int scale = 0;
    int setup = flg;

    switch (setup) {
    case 1:
    case 11:
        autogenCubicSpline1(scale);
        break;
    case 2:
    case 12:
        autogenCubicSpline1(scale);
        autogenCubicSpline2(scale);
        break;
    }
}

int selectivity::getMinSel() const
{
    return minSel;
}

void selectivity::setMinSel(int value)
{
    minSel = value;
}

// autogenerate Cubic Spline nodes locations
void selectivity::autogenCubicSpline1(int scale) {
    int numNodes = getSpecial();
    int bin = 0;
    int numObs = 1;
    QList<int> binTotals;
//    int total = 0;
//    int pctl025 = 0;
//    int pctl975 = 2;
    float term = .5, interm = 1;
    float nodeFirst = 0, nodeLast = 0, nodeRange = 0;
    float valFirst = bins.first().toFloat();
    float valLast = bins.last().toFloat();
    float valRange = valLast - valFirst;
    int numBins = 0;
    QStringList data;


    if (type == Size && fleet->getLengthNumObs() > 0) {
        numObs = fleet->getLengthNumObs();
        numBins = fleet->getLengthObservation(0).count() - 7;
        for (bin = 0; bin < numBins; bin++) {
            binTotals.append(0);
        }

        for (int row = 0; row < numObs; row++) {
            for (bin = 0; bin < numBins; bin++) {
                binTotals[bin] += fleet->getLengthObservation(row).at(bin+7).toInt();
            }
        }
        getPctTileBins(data_model->get_length_composition()->getBins(), binTotals, nodeFirst, nodeLast);
    }
    else if (type == Age && fleet->getAgeNumObs() > 0) {
        numObs = fleet->getAgeNumObs();
        numBins = fleet->getAgeObservation(0).count() - 9;
        for (bin = 0; bin < numBins; bin++) {
            binTotals.append(0);
        }

        for (int row = 0; row < numObs; row++) {
            for (bin = 0; bin < numBins; bin++) {
                binTotals[bin] += fleet->getAgeObservation(row).at(bin+9).toInt();
            }
        }
        getPctTileBins(data_model->get_age_composition()->getBins(), binTotals, nodeFirst, nodeLast);
    }
    if (nodeLast < 1)
    {
        float val = 0;
        val = bins.at(1).toFloat();
        nodeFirst = valRange * .025 + valFirst;
        if (nodeFirst < val) nodeFirst = val;
        nodeLast  = valRange * .975 + valFirst;
        val = bins.at(bins.count() - 2).toFloat();
        if (nodeLast > val) nodeLast = val;
    }
    nodeRange = nodeLast - nodeFirst;
    interm = nodeRange / (numNodes - 1);
    term = nodeFirst;
    int index = scale + 3;
    for (int i = 0; i < numNodes; i++) {
        data  = getParameter(index + i);
        data[0] = QString::number(valFirst);
        data[1] = QString::number(valLast);
        data[2] = QString::number(term);
        data[6] = QString::number(-99);
        setParameter(index +i, data);
        term += interm;
    }
}

void selectivity::autogenCubicSpline2(int scale) {
    int numNodes = getSpecial();
    int index = scale + 3 + numNodes;
    QStringList data;

    // scaling bins
    if (scale > 0)
    {
        data = parameters->getParameter(0);
        data[0] = QString::number(1);
        data[1] = QString::number(bins.count());
        data[2] = QString::number(bins.count()/2);
        data[6] = QString::number(2);
        parameters->setParameter(0, data);
        data = parameters->getParameter(1);
        data[0] = QString::number(1);
        data[1] = QString::number(bins.count());
        data[2] = QString::number(bins.count()/2 + 5);
        data[6] = QString::number(2);
        parameters->setParameter(1, data);
    }

    // grad lo and hi
    data = parameters->getParameter(scale + 1);
    data[0] = QString::number(-0.001);
    data[1] = QString::number(1.0);
    data[2] = QString::number(0.03);
    data[6] = QString::number(2);
    parameters->setParameter(scale + 1, data);
    data = parameters->getParameter(scale + 2);
    data[0] = QString::number(-1);
    data[1] = QString::number(0.001);
    data[2] = QString::number(-0.01);
    data[6] = QString::number(2);
    parameters->setParameter(scale + 2, data);

    // min, max, init for node values
    for (int i = 0; i < numNodes; i++) {
        data = parameters->getParameter(index + i);
        data[0] = QString::number(-9);
        data[1] = QString::number(7);
        data[2] = QString::number(-1);
        data[6] = QString::number(2);
        parameters->setParameter(index + i, data);
    }
    data = parameters->getParameter(index);
    data[2] = QString::number(-3);
    parameters->setParameter(index, data);
    data = parameters->getParameter(index+numNodes-1);
    data[2] = QString::number(-0.78);
    parameters->setParameter(index+numNodes-1, data);
}


void selectivity::getPctTileBins(const QStringList &bins, QList<int> &binTotals, float &nodeFirst, float &nodeLast)
{
    int bin = 0;
    int total = 0;
    int numBins = binTotals.count();

    for (bin = 0; bin < numBins; bin++) {
        total += binTotals.at(bin);
    }
    if (total > 0) {
        int pct025 = static_cast<int>(total * 0.025 + .5);
        int pct975 = static_cast<int>(total * 0.975 + .5);

        total = 0;
        for (bin = 0; bin < numBins-1; bin++) {
            total += binTotals.at(bin);
            if (total < pct025)
                nodeFirst = bins.at(bin+1).toFloat();
            else if (total < pct975)
                nodeLast = bins.at(bin+1).toFloat();
        }
        if (nodeLast >= bins.at(bins.count() - 1).toFloat())
        {
            nodeLast = bins.at(bins.count() - 2).toFloat();
        }
    }
    else
    {
        nodeFirst = nodeLast = 0;
    }
}

