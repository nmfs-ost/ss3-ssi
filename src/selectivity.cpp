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
             SIGNAL(setupChanged(QStringList)));
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
             this, SIGNAL(setupChanged(QStringList)));
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
    setup->setData(strList);
/*    pattern = strList.at(0).toInt();
    discard = strList.at(1).toInt();
    male = strList.at(2).toInt();
    special = strList.at(3).toInt();*/
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

void selectivity::setMethod(int method)
{
    QStringList parm;
//    int numparam = getNumParameters();
    int pattn = setup->getValue(0);
    int special = setup->getValue(3);
//    int i, j;
    if (method == pattn)
    {
        setDefaultParams(method, special);
/*        emit startingSetupChanges();
        switch (method)
        {
        case 0: // case 0 constant size selectivity = 1.0
        case 10:
            setNumParameters(0);
//            equation = new none();
            break;
        case 1:  // case 1 logistic size selectivity
        case 12:
            if (parameters->getNumParams() != 2)
            {
            setNumParameters(2);
            parm << "0" << "25" << "10" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(0, parm);
            parm[0] = QString("-50");
            parm[1] = QString("50");
            parm[2] = QString("0");
            setParameter(1, parm);
            }
//            equation = new simple_logistic();
            break;
        case 2:  // case 2 discontinued; use pattern 8 for double logistic (size)
            setPattern(8);
            break;
        case 3:  // case 3 discontinued
            setPattern(0);
            break;
        case 4:  // case 4 discontinued; use pattern 30 to get spawning biomass
            setPattern(30);
            break;
        case 5: // case 5 mirror another fleet size selectivity for specified bin range
            setNumParameters(2);
            parm << "-1" << "25" << "-1" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(0, parm);
            setParameter(1, parm);
            break;
        case 6: // case 6 non-parametric size selex pattern
            if (special < 3)
            {
                special = 3;
                setup->setValue(3, special);
            }
//            numparam = getNumParameters();
            numparam = 2 + special;
            if (getNumParameters() < numparam)
                setNumParameters(numparam);
            parm << "0" << "25" << "10" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(0, parm);
            parm[2] = QString("20");
            setParameter(1, parm);
            parm.clear();
            parm << "-5" << "9" << ".01" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            for (int i = 2; i < numparam; i++)
                setParameter(i, parm);
//            equation = new linear_segments();
            break;
        case 7:  // case 7 discontinued; use pattern 8 for double logistic
            setPattern(8);
            break;
        case 8:
        case 18: // 18 #age selectivity: double logistic with smooth transition
            if (parameters->getNumParams() != 8)
            {
            setNumParameters(8);
            }
            parm << "0" << "25" << "1" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
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
            parm << "1" << "20" << "10" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(7, parm);
//            equation = new double_logistic();
            break;
        case 9:  // case 9 old double logistic with 4 parameters
        case 19:
            if (parameters->getNumParams() != 6)
            {
            setNumParameters(6);
            }
            parm << "0" << "25" << "1" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(0, parm);
            setParameter(2, parm);
            setParameter(4, parm);
            parm.clear();
            parm << "0" << "1" << ".01" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(1, parm);
            setParameter(5, parm);
            parm.clear();
            parm << "-1" << "0" << "-.01" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(3, parm);
//            equation = new simple_double_logistic();
            break;
        case 11:
            if (parameters->getNumParams() != 2)
            {
            setNumParameters(2);
            }
            parm << "0" << "25" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(0, parm);
            parm[2] = QString("25");
            setParameter(1, parm);
//            equation = new constant_for_range();
            break;
        case 13:
            if (parameters->getNumParams() != 8)
            {
            setNumParameters(8);
            }
            parm << "0" << "25" << "1" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
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
            parm << "1" << "20" << "10" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(7, parm);
//            equation = new double_logistic();
            break;
        case 14:
            numparam = numAges + 1;
            if (parameters->getNumParams() != numparam)
            {
            setNumParameters(numparam);
            }
            parm << "-5" << "9" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            for (int i = 0; i < numparam; i++)
                setParameter(i, parm);
//            equation = new each_age();
            break;
        case 15: // case 15 mirror another fleet size selectivity for all size bins
            setNumParameters(0);
//            equation = new mirror();
            break;
        case 16: // 16 #age selectivity: Coleraine - Gaussian
            if (parameters->getNumParams() != 2)
            {
            setNumParameters(2);
            }
            parm << "-5" << "5" << "-1" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(0, parm);
            parm.clear();
            parm << "-5" << "40" << "20" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(1, parm);
//            equation = new coleraine();
            break;
        case 17:
            numparam = numAges + 1;
            if (parameters->getNumParams() != 2)
            {
            setNumParameters(numparam);
            }
            parm << "-.001" << "5" << "-1000" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(0, parm);
            parm.clear();
            parm << "-10" << "10" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            for (int i = 1; i < numparam; i++)
                setParameter(i, parm);
//            equation = new random_walk();
            break;
        case 20:
        case 23: // case 23 size selectivity double_normal_plateau where final value can be greater than 1.0
        case 24:
            if (parameters->getNumParams() != 6)
            {
            setNumParameters(6);
            }
            parm << "0" << "25" << "1" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(0, parm);
            parm.clear();
            parm << "-5" << "3" << "-.01" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(1, parm);
            parm.clear();
            parm << "-5" << "12" << "1" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(2, parm);
            parm.clear();
            parm << "-2" << "10" << "1" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(3, parm);
            parm.clear();
            parm << "-15" << "5" << "-2" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(4, parm);
            parm.clear();
            parm << "-5" << "5" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(5, parm);
//            equation = new double_normal_plateau_ends();
            break;
        case 22:
            if (parameters->getNumParams() != 4)
            {
            setNumParameters(4);
            }
            parm << "0" << "25" << "1" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(0, parm);
            parm.clear();
            parm << "0" << "1" << ".01" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(1, parm);
            parm.clear();
            parm << "0" << "10" << "1" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(2, parm);
            setParameter(3, parm);
//            equation = new double_normal_plateau();
            break;
        case 25: // case 25 size selectivity using exponential-logistic
        case 26:
            if (parameters->getNumParams() != 3)
            {
            setNumParameters(3);
            }
            parm << ".02" << "2" << "1" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(0, parm);
            parm.clear();
            parm << ".001" << ".999" << ".5" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(1, parm);
            parm.clear();
            parm << ".001" << ".5" << ".1" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(2, parm);
//            equation = new exponent_logistic();
            break;
        case 27:
            int index;
            if (special < 3)
            {
                special = 3;
                setup->setValue(3, special);
            }
            //setSpecial(special);
            numparam = special * 2 + 3;
            if (parameters->getNumParams() != numparam)
            {
            setNumParameters(numparam);
            }
            parm << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(0, parm);
            parm.clear();
            parm << "-.001" << "1" << ".13" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(1, parm);
            parm.clear();
            parm << "-1" << ".001" << "-.03" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(2, parm);
            parm.clear();
            parm << "1" << "90" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            index = 3;
            for (int i = 0; i < special; i++, index++)
                setParameter(index, parm);
            parm.clear();
            parm << "-9" << "7" << "-3" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            for (int i = 0; i < special; i++)
                setParameter(index, parm);
//            equation = new cubic_spline();
            break;
        case 30:  // equal to spawning biomass
        case 31:  // equal to exp(recruitment dev)
        case 32:  // equal to exp(rec dev) * spawn biomass
        case 33:  // expected survey abund = age 0 recruitment
        case 34:  // spawning biomass depletion (B/B0)
            setNumParameters(0);
//            equation = new none();
            break;
        case 41:
            numparam = numAges + 1 + 2;
            if (parameters->getNumParams() != numparam)
            {
            setNumParameters(numparam);
            }
            parm << "1" << QString::number(numAges) << "10" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(0, parm);
            parm[2] = QString("10");
            setParameter(1, parm);
            parm.clear();
            parm << "-.001" << "5" << "-1000" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(2, parm);
            parm.clear();
            parm << "-10" << "10" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            for (i = 1; i < numparam; i++)
                setParameter(i, parm);
            break;
        case 42:
            if (special < 3)
            {
                special = 3;
            }
            setup->setValue(3, special);
            //setSpecial(special);
            numparam = special * 2 + 3 + 2;
            if (parameters->getNumParams() != numparam)
            {
            setNumParameters(numparam);
            }
            parm << "1" << QString::number(numAges) << "10" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(0, parm);
            parm[2] = QString("10");
            setParameter(1, parm);
            parm << "0" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(2, parm);
            parm.clear();
            parm << "-.001" << "1" << ".13" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(3, parm);
            parm.clear();
            parm << "-1" << ".001" << "-.03" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(4, parm);
            parm.clear();
            parm << "1" << "20" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            index = 5;
            for (int i = 0; i < special; i++, index++)
                setParameter(index, parm);
            parm.clear();
            parm << "-9" << "7" << "-3" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            for (i = 0; i < special; i++, index++)
                setParameter(index, parm);
            break;
        case 43: // case 43 non-parametric size selex scaled by average of values from low bin through high bin
            if (special < 2)
            {
                special = 2;
                setup->setValue(3, special);
            }
            //setSpecial(special);
            numparam = special + 2 + 2;
            setNumParameters(numparam);
            parm << "1" << QString::number(numAges) << "10" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(0, parm);
            parm[2] = QString("10");
            setParameter(1, parm);
            parm << "0" << "25" << "10" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(2, parm);
            parm[2] = QString("20");
            setParameter(3, parm);
            parm.clear();
            parm << "-5" << "9" << ".01" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            for (i = 4; i < numparam; i++)
                setParameter(i, parm);
            break;
        case 44:
            if (special < 1)
            {
                special = 1;
                setup->setValue(3, special);
            }
            numparam = (special * numGenders) + 4;
            setNumParameters(numparam);
            parm << "0" << QString::number(numAges) << "0" << "0" << "0.5" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(0, parm);
            parm[2] = QString("2");
            setParameter(1, parm);
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
        case 45:
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
            parm << "1" << QString::number(numAges) << "2" << "0" << "0.5" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
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
        default:
            setPattern(0);
        }
        emit setupChanged(setup->getData());*/
    }
    else
    {
        setPattern (method);
    }
}

void selectivity::setNumXvals(int num)
{
    numXvals = num;
    setDefaultParams(getPattern(), getSpecial());
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
    float val;
    float xmin = 1, xmax = 50, peak = 25;
    float x1, x2;
    QString xMin, xMax, Peak;
    QString xLo, xHi;
    if (xVals.count() > 0)
    {
        xmin = xVals.first();
        xmax = xVals.last();
        peak = (xmax-xmin) / 2.0;
    }
    x1 = (peak-xmin) / 2.0;
    x2 = (xmax-peak) / 2.0;
    xMin = QString::number(xmin);
    xMax = QString::number(xmax);
    Peak = QString::number(peak, 'g', 2);
    xLo = QString::number(x1, 'g', 2);
    xHi = QString::number(x2, 'g', 2);

    emit startingSetupChanges();
    switch (method)
    {
    case 0:  // case 0 constant size selectivity = 1.0
    case 10: // case 10 Constant age-specific selex for ages 1 to nages
    {
        setNumParameters(0);
        break;
    }
    case 1:  // case 1 logistic size selectivity
    case 12:
    {
        setNumParameters(2);
        parm << xMin << xMax << Peak << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm[0] = QString("-50");
        parm[1] = QString("50");
        parm[2] = QString("0");
        setParameter(1, parm);
        break;
    }
    case 2:  // case 2 discontinued; use pattern 8 for double logistic (size)
    {
        setPattern(8);
        break;
    }
    case 3:  // case 3 discontinued
    {
        setPattern(0);
        break;
    }
    case 4:  // case 4 discontinued; use pattern 30 to get spawning biomass
    {
        setPattern(30);
        break;
    }
    case 5:  // case 5 mirror another fleets size selectivity for specified bin range
    {
        setNumParameters(2);
        parm << "-1" << "25" << "-1" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
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
    case 7:  // case 7 discontinued; use pattern 8 for double logistic
    {
        setPattern(8);
        break;
    }
    case 8:  // case 8 double logistic with eight parameters
    case 18: // 18 #age selectivity: double logistic with smooth transition
        // 1=peak, 2=init, 3=infl, 4=slope, 5=final, 6=infl2, 7=slope2 8=binwidth;    Mirror=1===const_above_Linf
    {
        setNumParameters(8);
        parm << xMin << xMax << Peak << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm.clear();
        parm << "0" << "1" << ".01" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(1, parm);
        parm.clear();
        parm << xMin << xMax << xLo << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(2, parm);
        parm[2] = xHi;
        setParameter(5, parm);
        parm.clear();
        parm << "-5" << "3" << ".001" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(3, parm);
        parm[2] = QString("-.001");
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
        parm << xMin << xMax << Peak << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        setParameter(2, parm);
        setParameter(4, parm);
        parm.clear();
        parm << "0" << "1" << ".01" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(1, parm);
        setParameter(5, parm);
        parm.clear();
        parm << "-1" << "0" << "-.01" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(3, parm);
        break;
    }
    case 11:  // case 11 Constant age-specific selex for specified age range
    {
        setNumParameters(2);
        parm << xMin << xMax << xMin << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm[2] = xMax;
        setParameter(1, parm);
        break;
    }
    case 13:  // double logistic
    {
        setNumParameters(8);
        parm << xMin << xMax << xMin << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
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
        parm << "-5" << "5" << "-1" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm.clear();
        parm << "-5" << "40" << "20" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(1, parm);
        break;
    }
    case 17:  // 17 #age selectivity: each age has parameter as random walk
        //    transformation as selex=exp(parm); some special codes
    {
        numparam = numAges + 1;
        setNumParameters(numparam);
        parm << "-.001" << "5" << "-1000" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm.clear();
        parm << "-10" << "10" << ".001" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
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
        parm << xMin << xMax << Peak << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
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
        parm << xMin << xMax << xMin << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm.clear();
        parm << "-5" << "3" << "-.01" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(1, parm);
        parm.clear();
        parm << "-5" << "12" << "1" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(2, parm);
        parm.clear();
        parm << "-2" << "10" << "1" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
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
        parm << "0" << "1" << ".01" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(1, parm);
        parm.clear();
        parm << "0" << "10" << "1" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(2, parm);
        setParameter(3, parm);
        break;
    }
    case 25: // case 25 size selectivity using exponential-logistic
    case 26: // 26 #age selectivity: exponential logistic
    {
        setNumParameters(3);
        parm << ".02" << "2" << "1" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm.clear();
        parm << ".001" << ".999" << ".5" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(1, parm);
        parm.clear();
        parm << ".001" << ".5" << ".1" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(2, parm);
        break;
    }
    case 27:  // case 27 size selectivity using cubic spline
    {
        int index;
        float val;
  //      float xval1 = 1;
  //      float xval20 = 20;
        if (special < 3)
        {
            special = 3;
        }
        //setSpecial(special);
        numparam = special * 2 + 3;
  /*      if (xVals.isEmpty()) {
            val = 21.0 / (float)special;
        } else {
            xval1 = xVals.first();
            xval20 = xVals.last();
            numXvals = xVals.count();
            val = xVals.at(numXvals - 2) - xVals.at(1);
            val /= (float)special; //(float)19 / special;
        }*/
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
        numparam = numXvals + 1 + 2;
        if (parameters->getNumParams() != numparam)
        {
            setNumParameters(numparam);
        }
        parm << "1" << QString::number(numXvals) << "10" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm[2] = QString("10");
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
        parm << "1" << QString::number((int)val) << "1" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm[2] = QString::number((int)(val+1));
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

        parm << "1" << QString::number(numXvals) << QString::number((int)(numXvals/2)) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm[2] = QString::number((int)(numXvals/2) + 1);
        setParameter(1, parm);
        parm << xMin << xMax << xLo << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(2, parm);
        parm[2] = xHi;
        setParameter(3, parm);
        parm.clear();
        parm << "-5" << "9" << ".01" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        for (i = 4; i < getNumParameters(); i++)
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
        parm << "0" << QString::number(numXvals) << "1" << "0" << "0.5" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm[2] = QString("2");
        setParameter(1, parm);
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
        parm << "1" << QString::number(numXvals) << "2" << "0" << "0.5" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
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
    //    QStringList ql;
//    ql << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    int curr = getNumParameters();
    if (curr != num)
    {
        parameters->setNumParams(num);
//        for (int i = curr; i < num; i++)
//        {
//            setParameter(i, ql);
//        }
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
    int male = setup->getValue(2);
    if (male != value)
    {
//        emit startingSetupChanges();
        setup->setValue(2, value);
        switch (value)
        {
        case 1:
            maleParameters->setNumParams(4);
            for (int i = 0; i < 4; i++)
            {
                setMaleParameterLabel(i, QString("Male_P%1_%2(%3)").arg(
                                               QString::number(i+1),
                                               fisheryName,
                                               QString::number(fisheryNum)));
            }
            break;
        default:
            maleParameters->setNumParams(0);
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

