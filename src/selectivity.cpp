#include "selectivity.h"
#include "selex_equation.h"

selectivity::selectivity(ss_model *model, int method)
    : QObject ((QObject*)model)
{
//    pattern = 0;
//    discard = 0;
//    male = 0;
//    special = 0;

    numXvals = 0;
    numAges = 0;
//    equation = nullptr;
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

    setMethod(method);
}

void selectivity::reset()
{
    QStringList values;
    values << "0" << "0" << "0" << "0";
    setSetup(values);
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
//    delete equation;
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
    setPattern(strList.at(0).toInt());
    setDiscard(strList.at(1).toInt());
    setMale(strList.at(2).toInt());
/*    setup->setData(strList);*/
//    emit dataChanged();
}
void selectivity::setSetup(QList<int> values)
{
    setSpecial(values.at(3));
    setMale   (values.at(2));
    setDiscard(values.at(1));
    setMethod (values.at(0));
}

void selectivity::setTVautogenerate(int val)
{
    varParameters->setAutoGenerate(val);
//    for (int i = 0; i < getNumParameters(); i++)
//        varParameters->changeVarParamData(i, getParameter(i));
    retainVarParameters->setAutoGenerate(val);
//    for (int i = 0; i < getNumRetainParameters(); i++)
//        retainVarParameters->changeVarParamData(i, getRetainParameter(i));
    discardVarParameters->setAutoGenerate(val);
//    for (int i = 0; i < getNumDiscardParameters(); i++)
//        discardVarParameters->changeVarParamData(i, getDiscardParameter(i));
    maleVarParameters->setAutoGenerate(val);
//    for (int i = 0; i < getNumMaleParameters(); i++)
//        maleVarParameters->changeVarParamData(i, getMaleParameter(i));
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
        setDefaultParams(method, special);
    }
    else
    {
        setPattern (method);
    }
}

void selectivity::setNumXvals(int num)
{
    numXvals = num;
    if (xVals.count() == 0) {
        for (int i = 0; i < num; i++)
            xVals.append(i);
    }

//    setDefaultParams(getPattern(), getSpecial());
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
    int i, j;
    double val;
    double xmin = 1, xmax = 50, peak = 25;
    double x1, x2;
    QString Count, xMin, xMax, Peak;
    QString BinCount;
    QString xLo, xHi;
    QString midbin(QString::number(static_cast<int>(xVals.count()/2)));

    if (parameters == nullptr)
        return;

    int count = xVals.count();
    if (count == 0)
        return;

    xmin = xVals.first();
    xmax = xVals.last();
    peak = (xmin + xmax) / 2.0;

    x1 = (xmin + peak) / 2.0;
    x2 = (peak + xmax) / 2.0;
    Count = QString::number(count);
    xMin = QString::number(xmin);
    xMax = QString::number(xmax);
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
        parm << xMin << xMax << xLo << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm[2] = xHi;
        setParameter(1, parm);
        break;
    }
    case 1:  // case 1 logistic size selectivity
    case 12:
    {
        setNumParameters(2);
        parm << xMin << xMax << Peak << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm[0] = QString(".01");
        parm[1] = QString("60");
        parm[2] = QString("18.9");
        setParameter(1, parm);
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
        parm << "1" << Count << "1" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm[2] = Count;
        setParameter(1, parm);
        break;
    }
    case 6:  // case 6 non-parametric size selex pattern
    {
        if (special < 3)
        {
            special = 3;
            setup->setValue(3, special);
        }
        numparam = 2 + special;
        setNumParameters(numparam);

        parm << xMin << xMax << Peak << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm[2] = QString::number(peak + 10.0);
        setParameter(1, parm);
        parm.clear();
        parm << "-5" << "9" << ".01" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        for (i = 2; i < numparam; i++)
            setParameter(i, parm);
        setSpecial(special);
        break;
    }
    case 2:  // case 2 discontinued; use pattern 8 for double logistic (size)
    case 7:  // case 7 discontinued; use pattern 8 for double logistic
    case 8:  // case 8 double logistic with eight parameters
//    case 18: // 18 #age selectivity: double logistic with smooth transition
        // 1=peak, 2=init, 3=infl, 4=slope, 5=final, 6=infl2, 7=slope2 8=binwidth;    Mirror=1===const_above_Linf
    {
        setNumParameters(8);
        parm << xMin << xMax << Peak << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm.clear();
        parm << "0" << "1" << ".01" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(1, parm);
        parm.clear();
        parm << "0" << "3" << "1.5" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(2, parm);
        parm[2] = QString("2.3");
        setParameter(5, parm);
        parm.clear();
        parm << "-5" << "3" << "-.6" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(3, parm);
        parm[2] = QString("0.06");
        setParameter(6, parm);
        parm.clear();
        parm << "-5" << "9" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(4, parm);
        parm.clear();
        parm << "1" << "20" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(7, parm);
        break;
    }
    case 18: // 18 #age selectivity: double logistic with smooth transition
        // 1=peak, 2=init, 3=infl, 4=slope, 5=final, 6=infl2, 7=slope2 8=binwidth;    Mirror=1===const_above_Linf
    {
        setNumParameters(8);
        parm << xMin << xMax << xLo << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm.clear();
        parm << "0" << "1" << ".01" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(1, parm);
        parm.clear();
        parm << "0" << "3" << "1.3" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(2, parm);
        parm[2] = QString("2.3");
        setParameter(5, parm);
        parm.clear();
        parm << "-5" << "3" << "1.3" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(3, parm);
        setParameter(6, parm);
        parm.clear();
        parm << "-5" << "9" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(4, parm);
        parm.clear();
        parm << "1" << "20" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(7, parm);
        break;
    }
    case 9:  // case 9 old double logistic with six parameters
    case 19: // simple double logistic
    {
        setNumParameters(6);
        parm << xMin << xMax << xLo << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
//        parm[0] = QString("0");
        parm[2] = Peak;
        setParameter(2, parm);
/*        parm[0] = QString("1");
        parm[1] = QString::number(xVals.count());
        parm[2] = QString("1");*/
        parm.clear();
        parm << "0" << "9" << "1" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(1, parm);
        parm.clear();
        parm << "-1" << "1" << "-.02" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(3, parm);
        parm.clear();
        if (method == 9) {
            parm << "1" << Count << "1" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        }
        else {
            parm << xMin << xMax << xMin << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        }
        setParameter(4, parm);
        parm.clear();
        parm << "0" << "1" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(5, parm);
        break;
    }
    case 13:  // double logistic
    {
        setNumParameters(8);
        parm << xMin << xMax << xLo << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm.clear();
        parm << "0" << "1" << ".01" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(1, parm);
        parm.clear();
        parm << "-5" << "5" << "1" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(2, parm);
        parm.clear();
        parm << "-5" << "3" << "-1" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(3, parm);
        setParameter(6, parm);
        parm.clear();
        parm << "-5" << "9" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(4, parm);
        setParameter(5, parm);
        parm.clear();
        parm << xMin << xMax << Peak << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(7, parm);
        break;
    }
    case 14:  // case 14 separate parm for each age
    {
        numparam = numAges + 1;
        setNumParameters(numparam);

        parm << "-5" << "9" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        for (int i = 0; i < numparam; i++)
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
        parm << "-20" << "20" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm.clear();
        parm << "-5" << "40" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(1, parm);
        break;
    }
    case 17:  // 17 #age selectivity: each age has parameter as random walk
        //    transformation as selex=exp(parm); some special codes
    {
        numparam = numAges + 1;
        setNumParameters(numparam);
        parm << "-1000" << "5" << "-1000" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm.clear();
        parm << "-1000" << "10" << ".1" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
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
        parm << xMin << xMax << xLo << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        for (i = 0; i < special; i++)
        {
            parm[2] = QString::number(1.0 + i * val);
            setParameter(i, parm);
        }
        parm.clear();
        parm << "0" << "1" << "1.0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
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
        parm << xMin << xMax << xLo << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm.clear();
        parm << "-5" << "3" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(1, parm);
        parm.clear();
        parm << "-5" << "12" << "3" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(2, parm);
        parm.clear();
        parm << "-2" << "10" << "5" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(3, parm);
        parm.clear();
        parm << "-15" << "5" << "-2" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(4, parm);
        parm.clear();
        parm << "-5" << "5" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(5, parm);
        break;
    }
    case 22:  // case 22 size selectivity using double_normal_plateau (similar to CASAL)
    {
        setNumParameters(4);
        parm << xMin << xMax << xLo << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm.clear();
        parm << "-5" << "10" << "1.5" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(1, parm);
        parm.clear();
        parm << "0" << "10" << "4" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(2, parm);
        setParameter(3, parm);
        break;
    }
    case 25: // case 25 size selectivity using exponential-logistic
    case 26: // 26 #age selectivity: exponential logistic
    {
        setNumParameters(3);
        parm << ".02" << "2" << ".5" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm.clear();
        parm << ".001" << ".999" << ".3" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(1, parm);
        parm.clear();
        parm << ".001" << ".5" << ".01" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(2, parm);
        break;
    }
    case 27:  // case 27 size selectivity using cubic spline
    {
        int index;
        float val;
        if (special < 3)
        {
            special = 3;
        }
        //setSpecial(special);
        numparam = special * 2 + 3;
        setNumParameters(numparam);
        val = (xmax - xmin) / (float)special;
        setNumParameters(numparam);
        parm << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm.clear();
        parm << "-.001" << "1" << ".13" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(1, parm);
        parm.clear();
        parm << "-1" << ".001" << "-.03" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(2, parm);
        parm.clear();
        parm << xMin << xMax << xLo << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        index = 3;
        for (i = 0; i < special; i++)
        {
            parm[2] = QString::number(xmin + ((float)val * (i + 1)));
            setParameter(index++, parm);
        }
        parm.clear();
        parm << "-9" << "7" << "-3" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        for (i = 0; i < special; i++)
            setParameter(index++, parm);
        setup->setValue(3, special);
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
        parm << "1" << Count << midbin << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm[2] = Count;
        setParameter(1, parm);
        parm.clear();
        parm << "-.001" << "5" << "-1000" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(2, parm);
        parm.clear();
        parm << "-10" << "10" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        for (i = 1; i < numparam; i++)
            setParameter(i, parm);
        break;
    }
    case 42: // case 42 size selectivity using cubic spline scaled by average of values at low bin through high bin
    {
        int index = 0;
        float xval1 = 1, xval20 = 20;
        if (special < 3)
        {
            special = 3;
        }
        numparam = special * 2 + 3 + 2;
        if (xVals.isEmpty()) {
            val = 21.0 / (float)special;
        } else {
            xval1 = xVals.first();
            xval20 = xVals.last();
            numXvals = xVals.count();
            val = xVals.at(numXvals - 2) - xVals.at(1);
            val /= (float)special; //(float)19 / special;
        }
        val = numXvals / 2;
        setNumParameters(numparam);
        parm << "1" << Count  << midbin << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm[2] = Count;
        setParameter(1, parm);
        parm.clear();
        parm << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(2, parm);
        parm.clear();
        parm << "-.001" << "1" << ".13" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(3, parm);
        parm.clear();
        parm << "-1" << ".001" << "-.03" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(4, parm);
        parm.clear();
        val = (xmax-xmin) / (float)special;
        parm << xMin << xMax << xLo << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        index = 5;
        for (i = 0; i < special; i++)
        {
            parm[2] = QString::number(xmin + ((float)val * (i + 1)), 'g', 2);
            setParameter(index++, parm);
        }
        parm.clear();
        parm << "-9" << "7" << "-3" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        for (i = 0; i < special; i++)
            setParameter(index++, parm);
        setup->setValue(3, special);
        break;
    }
    case 43: // case 43 non-parametric size selex scaled by average of values from low bin through high bin
    {
        if (special < 3)
        {
            special = 3;
            setup->setValue(3, special);
        }
        numparam = special + 2 + 2;
        setNumParameters(numparam);

        parm.clear();
        parm << "1" << Count << midbin << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm[2] = Count;
        setParameter(1, parm);
        parm.clear();
        parm << xMin << xMax << xLo << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(2, parm);
        parm[2] = xHi;
        setParameter(3, parm);
        parm.clear();
        parm << "-5" << "9" << ".01" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        for (i = 4; i < numparam; i++)
            setParameter(i, parm);
        break;
    }
    case 44: // case 44 like age selex 17 but with separate parameters for males and with revised controls
    {
        if (special < 1)
        {
            special = 1;
            setup->setValue(3, special);
        }
        numparam = (special * numGenders) + 4;
        setNumParameters(numparam);
        parm << "-1000" << Count << "-1000" << "0" << "0.5" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm[2] = midbin;
        setParameter(1, parm);
        parm[2] = Count;
        setParameter(2, parm);
        parm.clear();
        parm << "-1" << "2" << "-.001" << "-.001" << "0.5" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(3, parm);
        parm.clear();
        parm << "-10" << "10" << "3" << "3" << "0.5" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        for (i = 4, j = 0; j < special; i++, j++)
            setParameter(i, parm);
        if (numGenders > 1)
        {
            parm.clear();
            parm << "-1000" << "10" << "-1000" << "-0.9" << "0.5" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            for (j = 0; j < special; i++, j++)
                setParameter(i, parm);
        }
        break;
    }
    case 45: // 45 like age selex 14 but with separate parameters for males and with revised controls
    {
        if (special < 1)
        {
            special = 1;
        }
        setup->setValue(3, special);
        numparam = (special * numGenders) + 4;
        if (parameters->getNumParams() != numparam)
        {
            setNumParameters(numparam);
        }
        parm << "1" << Count << "2" << "0" << "0.5" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm[2] = QString("5");
        setParameter(1, parm);
        setParameter(2, parm);
        parm.clear();
        parm << "-1" << "2" << "-0.001" << "-0.001" << "0.5" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(3, parm);
        parm.clear();
        parm << "-10" << "10" << "-5.0" << "-5.0" << "0.5" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        for (i = 4, j = 0; j < special; i++, j++)
            setParameter(i, parm);
        if (numGenders > 1)
        {
            parm.clear();
            parm << "-1000" << "10" << "-1000" << "-0.9" << "0.5" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            for (j = 0; j < special; i++, j++)
                setParameter(i, parm);
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

QList<float> selectivity::getXVals() const
{
    return xVals;
}

void selectivity::setXVals(const QStringList &value)
{
    xVals.clear();
    for (int i = 0; i < value.count(); i++)
        xVals.append(QString(value.at(i)).toFloat());
    setNumXvals(value.count());
}

float selectivity::getBinValue(int index) {
    return xVals[index];
}

void selectivity::setAgeBins (const QStringList &value)
{
    abins = QStringList(value);
}

void selectivity::setLenBins (const QStringList &value)
{
    lbins = QStringList(value);
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
        case 4:
            if (getPattern() == 23 || getPattern() == 24)
                num = 5;
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

