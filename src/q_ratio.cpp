#include "q_ratio.h"
#include "setupmodel.h"
#include "model.h"

#include <QMessageBox>

q_ratio::q_ratio(ss_model *model)
    : QObject(model)
{
    setup = new setupModel(this);
    params = new longParameterModel(this);
    varParams = new timeVaryParameterModel(model);

    QStringList hdr;
    hdr << "Link" << "Link_Info" << "ExtraSD" << "Bias_Adj" << "Float" ;
    setup->setHeader(hdr);
    params->setTotalNumParams(3);
    varParams->setTotalNumVarParams(3);

    reset();

    connect (setup, SIGNAL(dataChanged(QList<int>)), SLOT(changeSetupData(QList<int>)));

    connect (params, SIGNAL(paramChanged(int, QStringList)), SLOT(checkParamValues(int,QStringList)));
//             varParams, SLOT(changeVarParamData(int,QStringList)));
}

q_ratio::~q_ratio()
{
    delete varParams;
    delete params;
    delete setup;
}


void q_ratio::reset()
{
    QStringList ql;
    paramsUsed.clear();
    paramsUsed << 0 << 0 << 0;
    ql << "0" << "0" << "0" << "0" << "0";
    setValues(ql);
    params->setParamsUsed(paramsUsed);
}


void q_ratio::setFleetName(QString name)
{
    fltName = name;
    setParamHdrs();
    setup->setRowHeader(name);
/*    params->setNumParams(3);
    params->setParamHeader(0, QString("%1_LnQ_Base").arg(fltName));
    params->setParamHeader(1, QString ("%1_Power").arg(fltName));
    params->setParamHeader(2, QString ("%1_Extra_SD").arg(fltName));*/

}

void q_ratio::setValues(QStringList values)
{
    setup->setData(values);
}

void q_ratio::setValues (QString str)
{
    QStringList sList (str.split(' ', QString::SkipEmptyParts));
    setValues (sList);
}

void q_ratio::changeSetupData(QList<int> values)
{
    int val = 0;
    bool changed;
    int index = 0;

    val = values.at(0);
    if (val > 0)
    {
        if (paramsUsed.at(0) != 1)
        {
            paramsUsed[0] = 1;
            changed = true;
        }
        BaseIndex = index++;
    }
    else
    {
        if (paramsUsed.at(0) != 0)
        {
            paramsUsed[0] = 0;
            changed = true;
        }
        BaseIndex = -1;
    }

    if (val == 3)
    {
        if (paramsUsed.at(1) != 1)
        {
            paramsUsed[1] = 1;
            changed = true;
        }
        powerIndex = index++;
    }
    else
    {
        if (paramsUsed.at(1) != 0)
        {
            paramsUsed[1] = 0;
            changed = true;
        }
        powerIndex = -1;
    }

    val = values.at(2);
    if (val == 1)
    {
        if (paramsUsed.at(2) != 1)
        {
            paramsUsed[2] = 1;
            changed = true;
        }
        ExtraIndex = index;
    }
    else
    {
        if (paramsUsed.at(2) != 0)
        {
            paramsUsed[2] = 0;
            changed = true;
        }
        ExtraIndex = -1;
    }

    if (changed)
    {
        params->setParamsUsed(paramsUsed);
        varParams->setVarParamsUsed(paramsUsed);
        emit setupChanged();
    }
}

void q_ratio::checkParamValues(int index, QStringList paramValues)
{
    QStringList newValues(paramValues);
    int item, check;
    bool error = false;

    item = QString(paramValues.at(7)).toInt();
    check = varParams->checkEnvVarValue(item);
    if (item != check)
    {
        error = true;
        newValues[7] = QString::number(check);
    }

    item = QString(paramValues.at(8)).toInt();
    check = varParams->checkDevValue(item);
    if (item != check)
    {
        error = true;
        newValues[8] = QString::number(check);
    }

    item = QString(paramValues.at(12)).toInt();
    check = varParams->checkBlockNum(item);
    if (item != check)
    {
        error = true;
        newValues[12] = QString::number(check);
    }

    item = QString(paramValues.at(13)).toInt();
    check = varParams->checkBlockFxn(item);
    if (item != check)
    {
        error = true;
        newValues[13] = QString::number(check);
    }
    if (error)
        setParameterData(index, newValues);

    varParams->changeVarParamData(index, newValues);
}

QString q_ratio::getSetup()
{
    QStringList sList (setup->getData());
    QString txt("");
    for (int i = 0; i < sList.count(); i++)
        txt.append(QString("      %1").arg(sList.at(i)));
    return txt;
}

void q_ratio::changeParamData()
{
}



void q_ratio::setParamHdrs(int errType)
{
//    params->setTotalNumParams(3);
    QString hdr;
    if (errType > 0)
    {
        hdr = QString("%1_Q_Base").arg(fltName);
        params->setParamHeader(0, hdr);
        varParams->setTableTitle(0, hdr);
    }
    else
    {
        hdr = QString("%1_LnQ_Base").arg(fltName);
        params->setParamHeader(0, hdr);
        varParams->setTableTitle(0, hdr);
    }

    hdr = QString ("%1_Power").arg(fltName);
    params->setParamHeader(1, hdr);
    varParams->setTableTitle(1, hdr);

    hdr = QString ("%1_Extra_SD").arg(fltName);
    params->setParamHeader(2, hdr);
    varParams->setTableTitle(2, hdr);

    params->updateParamHeaders();
}

/*
void q_ratio::setNumParams(int num)
{
    params->setNumParams(num);
}

int q_ratio::getNumParams()
{
    return params->getNumParams();
}

void q_ratio::setParameter(int index, QStringList values)
{
    if (index < params->getNumParams())
        params->setNumParams(index + 1);
    params->setParameter(index, values);
}

void q_ratio::setParameter(int index, QString text)
{
    QStringList values(text.split(' ', QString::SkipEmptyParts));
    setParameter (index, values);
}

QStringList q_ratio::getParameter(int index)
{
    return params->getParameter(index);
}

int q_ratio::getTypeIndex() const
{
    return typeIndex;
}*/


bool q_ratio::getDoPower()
{
    int val = setup->getValue(0);
    return (val == 3);
}

void q_ratio::setDoPower(bool value)
{
    int curVal = setup->getValue(0);
    if (value)
    {
        if (curVal != 3)
        {
            setup->setValue(0, 3);
            setup->setValue(1, 0);
        }
    }
    doPower = value;
}


bool q_ratio::getDoExtraSD()
{
    int val = setup->getValue(2);
    return (val > 0);
}

void q_ratio::setDoExtraSD(bool value)
{
    if (getDoExtraSD() != value)
    {
        if (value)
            setup->setValue(2, 1);
        else
            setup->setValue(2, 0);
    }
}



QString q_ratio::getPower()
{
    QString txt("");
    if (powerIndex == 0)
    {
        QStringList values(params->getParameter(powerIndex));
        for (int j = 0; j < values.count(); j++)
            txt.append(QString("  %1 ").arg(values.at(j)));
    }
    return txt;
}

void q_ratio::setPower(QStringList values)
{
    if (powerIndex > 0)
    {
        params->setParameter(powerIndex, values);
        params->setParamHeader(powerIndex, QString("Q_Power"));
    }
}



QString q_ratio::getExtra()
{
    QString txt("");
    if (ExtraIndex > -1)
    {
        QStringList values(params->getParameter(ExtraIndex));
        for (int j = 0; j < values.count(); j++)
            txt.append(QString("  %1 ").arg(values.at(j)));
    }
    return txt;
}

void q_ratio::setExtra(QStringList values)
{
    if (ExtraIndex > -1)
    {
        while (values.count() < 14)
            values.append("0");
        params->setParameter(ExtraIndex, values);
    }
}

QString q_ratio::getBase()
{
    QString txt("");
    if (typeIndex > -1)
    {
        QStringList values(params->getParameter(typeIndex));
        for (int j = 0; j < values.count(); j++)
            txt.append(QString("  %1 ").arg(values.at(j)));
    }
    return txt;
}

void q_ratio::setBase(QStringList values)
{
    typeIndex = 0;
    params->setParameter(typeIndex, values);
}

void q_ratio::changeSetup(int errType)
{
    setParamHdrs(errType);
}

void q_ratio::changeSetup()
{

}







