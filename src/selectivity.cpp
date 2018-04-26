#include "selectivity.h"
#include "selex_equation.h"

selectivity::selectivity(ss_model *model, int method)
    : QObject ((QObject*)model)
{
//    pattern = 0;
//    discard = 0;
//    male = 0;
//    special = 0;

    numAges = 0;
//    equation = NULL;
    setup = new setupModel();
    QStringList hdr;
    hdr << tr("Pattern") << tr("Discard") << tr("Male") << tr("Special");
    setup->setHeader(hdr);

    parameters = new longParameterModel();
    varParameters = new timeVaryParameterModel(model);

    connect (setup, SIGNAL(dataChanged(QList<int>)),
             SLOT(setSetup(QList<int>)));
    connect (parameters, SIGNAL(paramChanged(int,QStringList)),
             varParameters, SLOT(changeVarParamData(int,QStringList)));

    discardParameters = new shortParameterModel();
    specialParameters = new shortParameterModel();
    retainParameters = new shortParameterModel();
    maleParameters = new shortParameterModel();

    setMethod(method);
}

selectivity::~selectivity()
{
//    delete equation;
    delete maleParameters;
    delete retainParameters;
    delete specialParameters;
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
/*    if (equation != NULL)
        delete equation;
    equation = NULL;*/
    QStringList parm;
    int numparam = getNumParameters();
    int pattn = setup->getValue(0);
    int special = setup->getValue(3);
    if (method == pattn)
    {
        switch (method)
        {
        case 0: // selectivity = 1 for all
        case 10:
            setNumParameters(0);
//            equation = new none();
            break;
        case 1:  // Simple logistic with 2 parameters
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
        case 2:  // discontinued
            setPattern(8);
            break;
        case 3:  // discontinued
            setPattern(0);
            break;
        case 4:  // discontinued
            setPattern(30);
            break;
        case 5:
            if (parameters->getNumParams() != 2)
            {
            setNumParameters(2);
            parm << "-1" << "25" << "-1" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            setParameter(0, parm);
            setParameter(1, parm);
            }
//            equation = new mirror_range();
            break;
        case 6:   // linear segments
            if (special < 2)
            {
                special = 2;
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
        case 7:
            setPattern(8);
            break;
        case 8:
        case 18:
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
        case 9:
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
        case 15:
            setNumParameters(0);
//            equation = new mirror();
            break;
        case 16:
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
        case 23:
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
        case 25:
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
            for (int i = 1; i < numparam; i++)
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
            for (int i = 0; i < special; i++)
                setParameter(index, parm);
            break;
        case 43:
            if (special < 2)
            {
                special = 2;
            }
            setup->setValue(3, special);
            //setSpecial(special);
            numparam = special + 2 + 2;
            if (parameters->getNumParams() != numparam)
            {
            setNumParameters(numparam);
            }
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
            for (int i = 4; i < getNumParameters(); i++)
                setParameter(i, parm);
            break;
        default:
            setPattern(0);
        }
    }
    else
    {
        setPattern (method);
    }
}



void selectivity::setPattern(int value)
{
    if (setup->getValue(0) != value)
    {
        emit startingSetupChanges();
        setup->setValue(0, value);
    //    pattern = value;
    //    setMethod(value);
        emit setupChanged(getSetup());
//        emit dataChanged();
    }
}

void selectivity::setDefaultParams(int method, int num)
{
    int numparam;
    QStringList parm;
    int special = setup->getValue(3);
    switch (method)
    {
    case 0: // selectivity = 1 for all
    case 10:
        setNumParameters(0);
        break;
    case 1:  // Simple logistic with 2 parameters
    case 12:
        parm << "0" << "25" << "10" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm[0] = QString("-50");
        parm[1] = QString("50");
        parm[2] = QString("0");
        setParameter(1, parm);
        break;
    case 5:  // mirror other fleet selectivity with bounds

        if (num != 2)
        {
        setNumParameters(2);
        parm << "-1" << "25" << "-1" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        setParameter(1, parm);
        }
        break;
    case 6:
        if (special < 2)
        {
            special = 2;
            setup->setValue(3, special);
        }
        numparam = 2 + special;
        if (num < numparam)
            setNumParameters(numparam);
        parm << "0" << "25" << "10" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm[2] = QString("20");
        setParameter(1, parm);
        parm.clear();
        parm << "-5" << "9" << ".01" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        for (int i = 2; i < numparam; i++)
            setParameter(i, parm);
        setSpecial(special);
        break;
    case 8:
    case 18: // new double logistic
        if (num != 8)
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
        break;
    case 9:
    case 19:  // simple double logistic
        if (num != 6)
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
        break;
    case 11:  // constant with range
        if (num != 2)
        {
        setNumParameters(2);
        }
        parm << "0" << "25" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm[2] = QString("25");
        setParameter(1, parm);
        break;
    case 13:  // double logistic
        if (num != 8)
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
        break;
    case 14:  // value at each age
        numparam = numAges + 1;
        if (parameters->getNumParams() != numparam)
        {
        setNumParameters(numparam);
        }
        parm << "-5" << "9" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        for (int i = 0; i < numparam; i++)
            setParameter(i, parm);
        break;
    case 15:  // mirror with no bounds
        setNumParameters(0);
        break;
    case 16:  // coleraine
        if (num != 2)
        {
        setNumParameters(2);
        }
        parm << "-5" << "5" << "-1" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm.clear();
        parm << "-5" << "40" << "20" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(1, parm);
        break;
    case 17:  // random walk
        numparam = numAges + 1;
        if (num != 2)
        {
        setNumParameters(numparam);
        }
        parm << "-.001" << "5" << "-1000" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        setParameter(0, parm);
        parm.clear();
        parm << "-10" << "10" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        for (int i = 1; i < numparam; i++)
            setParameter(i, parm);
        break;
    case 20:
    case 23:
    case 24:  // double normal with plateau and defined ends
        if (num != 6)
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
        break;
    case 22:  // double normal with plateau
        if (num != 4)
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
        break;
    case 25:
    case 26:  // exponential-logistic
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
        break;
    case 27:  // cubic spline
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
        parm << "1" << "20" << "2" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        index = 3;
        for (int i = 0; i < special; i++, index++)
            setParameter(index, parm);
        parm.clear();
        parm << "-9" << "7" << "-3" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        for (int i = 0; i < special; i++)
            setParameter(index, parm);
        break;
    case 30:  // equal to spawning biomass
    case 31:  // equal to exp(recruitment dev)
    case 32:  // equal to exp(rec dev) * spawn biomass
    case 33:  // expected survey abund = age 0 recruitment
    case 34:  // spawning biomass depletion (B/B0)
        setNumParameters(0);
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
        for (int i = 1; i < numparam; i++)
            setParameter(i, parm);
        break;
    case 42:
        if (special < 3)
        {
            special = 3;
            setup->setValue(3, special);
        }
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
        for (int i = 0; i < special; i++)
            setParameter(index, parm);
        break;
    case 43:
        if (special < 2)
        {
            special = 2;
            setup->setValue(3, special);
        }
        numparam = special + 2 + 2;
        if (parameters->getNumParams() != numparam)
        {
        setNumParameters(numparam);
        }
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
        for (int i = 4; i < getNumParameters(); i++)
            setParameter(i, parm);
        break;
    default:
        setNumParameters(0);
    }

}

int selectivity::getNumParameters()
{
    return parameters->getNumParams();
}

void selectivity::setNumParameters(int num)
{
    QStringList ql;
    ql << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    int curr = getNumParameters();
    if (curr != num)
    {
        parameters->setNumParams(num);
        for (int i = curr; i < num; i++)
        {
            setParameter(i, ql);
        }
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
        emit startingSetupChanges();
        setup->setValue(1, value);
        switch (discard)
        {
        case 1:
            retainParameters->setNumParams(4);
            discardParameters->setNumParams(0);
            break;
        case 2:
            retainParameters->setNumParams(4);
            discardParameters->setNumParams(4);
            break;
        case 3:
            retainParameters->setNumParams(0);
            discardParameters->setNumParams(0);
            break;
        case 4:
            retainParameters->setNumParams(7);
            discardParameters->setNumParams(4);
            break;
        default:
            retainParameters->setNumParams(0);
            discardParameters->setNumParams(0);
        }
        emit dataChanged();
    }
}

void selectivity::setMale(int value)
{
    int male = setup->getValue(2);
    if (male != value)
    {
        emit startingSetupChanges();
        setup->setValue(2, value);
        switch (male)
        {
        case 1:
            maleParameters->setNumParams(4);
            break;
        default:
            maleParameters->setNumParams(0);
        }
        emit dataChanged();
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
        case 6:
            if (value < 2)
            {
                value = 2;
            }
            setNumParameters(scale + 2 + value);
            break;

        case 42:
            scale = 2;
        case 27:
            if (value < 3)
            {
                value = 3;
            }
            setNumParameters(scale + 3 + value * 2);
            break;
        }
        setup->setValue(3, value);
        setDefaultParams(setup->getValue(0), getNumParameters());
    }
}

void selectivity::setTimeVaryParameter (int index, QStringList strList)
{
    varParameters->setVarParameter(index, strList);
//    timeVaryParameters->setParameter(index, strList);
}

void selectivity::setTimeVaryParameterLabel(int index, QString label)
{
    varParameters->setVarParamHeader(index, label);
//    timeVaryParameters->setParamHeader(index, label);
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
/*    int num = 0;
    QStringList data;
    for (int i = 0; i < parameters->rowCount(); i++)
    {
        num += parameters->envLink(i)? 1: 0;
    }
    return num;*/
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

int selectivity::getNumSpecialParameters()
{
    return specialParameters->getNumParams();
}

void selectivity::setSpecialParameter(int index, QStringList strList)
{
    specialParameters->setParameter(index, strList);
}

void selectivity::setSpecialParameterLabel(int index, QString label)
{
    specialParameters->setParamHeader(index, label);
}

QString selectivity::getSpecialParameterText (int index)
{
    QStringList data = specialParameters->getParameter(index);
    QString text (data.at(0));
    for (int i = 1; i < data.count(); i++)
        text.append(QString(" %1").arg(data.at(i)));
    return text;
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
/*
double selectivity::evaluate()
{
    double val = 0;
    return val;
}

double selectivity::evaluate(int f, float m)
{
    double val;
    if (equation == NULL)
        val = 0.0;
    else
        val = equation->evaluate();
    return val;
}*/

