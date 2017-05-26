#include "selectivity.h"
#include "selex_equation.h"

selectivity::selectivity(ss_model *model, int method)
    : QObject ((QObject*)model)
{
    pattern = 0;
    discard = 0;
    male = 0;
    special = 0;

    numAges = 0;
//    equation = NULL;
    setup = new setupModel();
    setup->setNumValues(4);
    setup->setColHeader(0, "Pattern");
    setup->setColHeader(1, "Discard");
    setup->setColHeader(2, "Male");
    setup->setColHeader(3, "Special");
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
    pattern = strList.at(0).toInt();
    discard = strList.at(1).toInt();
    male = strList.at(2).toInt();
    special = strList.at(3).toInt();
}
void selectivity::setSetup(QList<int> values)
{
    setMethod (values.at(0));
    setDiscard(values.at(1));
    setMale (values.at(2));
    setSpecial(values.at(3));
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
    int pattn = setup->getValue(0);
    if (method == pattern)
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
            setNumParameters(2);
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
            setNumParameters(2);
//            equation = new mirror_range();
            break;
        case 6:
            setNumParameters(2 + special);
//            equation = new linear_segments();
            break;
        case 7:
            setPattern(8);
            break;
        case 8:
        case 18:
            setNumParameters(8);
//            equation = new double_logistic();
            break;
        case 9:
        case 19:
            setNumParameters(6);
//            equation = new simple_double_logistic();
            break;
        case 11:
            setNumParameters(2);
//            equation = new constant_for_range();
            break;
        case 13:
            setNumParameters(8);
//            equation = new double_logistic();
            break;
        case 14:
            setNumParameters(numAges+1);
//            equation = new each_age();
            break;
        case 15:
            setNumParameters(0);
//            equation = new mirror();
            break;
        case 16:
            setNumParameters(2);
//            equation = new coleraine();
            break;
        case 17:
            setNumParameters(numAges+1);
//            equation = new random_walk();
            break;
        case 20:
        case 23:
        case 24:
            setNumParameters(6);
//            equation = new double_normal_plateau_ends();
            break;
        case 22:
            setNumParameters(4);
//            equation = new double_normal_plateau();
            break;
        case 25:
        case 26:
            setNumParameters(3);
//            equation = new exponent_logistic();
            break;
        case 27:
            setNumParameters(3 + (2 * special));
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
    setup->setValue(0, value);
    pattern = value;
    setMethod(value);
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
    setup->setValue(1, value);
    if (discard != value)
    {
        discard = value;
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
    }
}

void selectivity::setMale(int value)
{
    setup->setValue(2, value);
    if (male != value)
    {
        male = value;
        switch (male)
        {
        case 1:
            maleParameters->setNumParams(4);
            break;
        default:
            maleParameters->setNumParams(0);
        }
    }
}

void selectivity::setSpecial(int value)
{
    setup->setValue(3, value);
    special = value;
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

