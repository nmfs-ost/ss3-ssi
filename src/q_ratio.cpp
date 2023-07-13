#include "q_ratio.h"
#include "setupmodel.h"
#include "model.h"

#include <QMessageBox>

q_ratio::q_ratio(ss_model *model)
    : QObject(model)
{
    setup = new setupModel(this);
    params = new longParameterModel(model);
    varParams = new timeVaryParameterModel(model);

    QStringList hdr;
    hdr << "Link" << "Link_Info" << "ExtraSD" << "Bias_Adj" << "Float" ;
    setup->setHeader(hdr);
    params->setTotalNumParams(3);
    varParams->setTotalNumVarTables(3);
    paramsUsed << 0 << 0 << 0;

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
    QList <int> vals;
    ql << "Link" << "Link_Info" << "ExtraSD" << "Bias_Adj" << "Float" ;
    setup->setHeader(ql);
    ql.clear();
    ql << "0" << "0" << "0" << "0" << "0";
    setup->setData(ql);
}


void q_ratio::setFleetName(QString name)
{
    fltName = name;
    setParamHdrs();
//    setup->setRowHeader(name);
}

void q_ratio::setSetup(QStringList values)
{
    reset();
    while (values.count() < 5)
        values.append(QString("0"));
    setup->setData(values);
}

void q_ratio::setSetup (QString str)
{
    QStringList sList (str.split(' ', QString::SkipEmptyParts));
    setSetup (sList);
}

void q_ratio::changeSetupData(QList<int> values)
{
    int val = 0;
    bool changed = false;

    val = values.at(0);
    if (val > 0)
    {
        if (paramsUsed.at(0) != 1)
        {
            paramsUsed[0] = 1;
            varParams->changeVarParamData(0, params->getParameter(0));
            changed = true;
        }
    }
    else
    {
        if (paramsUsed.at(0) != 0)
        {
            paramsUsed[0] = 0;
            changed = true;
        }
    }

    if (val == 3)
    {
        if (paramsUsed.at(1) != 1)
        {
            paramsUsed[1] = 1;
            varParams->changeVarParamData(1, params->getParameter(1));
            changed = true;
        }
    }
    else
    {
        if (paramsUsed.at(1) != 0)
        {
            paramsUsed[1] = 0;
            changed = true;
        }
    }

    val = values.at(2);
    if (val > 0)
    {
        if (paramsUsed.at(2) != 1)
        {
            paramsUsed[2] = 1;
            varParams->changeVarParamData(2, params->getParameter(2));
            changed = true;
        }
    }
    else
    {
        if (paramsUsed.at(2) != 0)
        {
            paramsUsed[2] = 0;
            changed = true;
        }
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
    params->setTotalNumParams(3);
    QStringList varHdrs;
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
    varHdrs.append(hdr);

    hdr = QString ("%1_Power").arg(fltName);
    params->setParamHeader(1, hdr);
    varParams->setTableTitle(1, hdr);
    varHdrs.append(hdr);

    hdr = QString ("%1_Extra_SD").arg(fltName);
    params->setParamHeader(2, hdr);
    varParams->setTableTitle(2, hdr);
    varHdrs.append(hdr);

    params->updateParamHeaders();
    varParams->updateVarParamHdrs();
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


int q_ratio::getLinkType ()
{
    int val = setup->getValue(0);
    return (val);
}

int q_ratio::getLinkVal ()
{
    int val = setup->getValue(1);
    return (val);
}

bool q_ratio::getDoMirror()
{
    int val = getLinkType();
    return (val == 2);
}

bool q_ratio::getDoPower()
{
    int val = getLinkType();
    return (val == 3);
}

bool q_ratio::getDoMirOffset()
{
    int val = getLinkType();
    return (val == 4);
}

bool q_ratio::getDoExtraSD()
{
    int val = setup->getValue(2);
    return (val > 0);
}

bool q_ratio::getDoBiasAdj ()
{
    int val = setup->getValue(3);
    return (val > 0);
}

bool q_ratio::getDoFloat()
{
    int val = setup->getValue(4);
    return (val > 0);
}


QString q_ratio::getMirOffset()
{
    QString txt("");
    if (getDoMirOffset())
    {
        QStringList values(params->getParameter(1));
        for (int j = 0; j < values.count(); j++)
            txt.append(QString("  %1 ").arg(values.at(j)));
    }
    return txt;
}
QStringList q_ratio::getMirOffsetParam()
{
    return params->getParameter(1);
}

void q_ratio::setMirOffset(QStringList values)
{
    params->setParamData(1, values);
    varParams->setParameter(1, values);
}

QString q_ratio::getPower()
{
    QString txt("");
    if (getDoPower())
    {
        QStringList values(params->getParameter(1));
        for (int j = 0; j < values.count(); j++)
            txt.append(QString("  %1 ").arg(values.at(j)));
    }
    return txt;
}

QStringList q_ratio::getPowerParam()
{
    return params->getParameter(1);
}

void q_ratio::setPower(QStringList values)
{
    params->setParamData(1, values);
    varParams->setParameter(1, values);
}



QString q_ratio::getExtra()
{
    QString txt("");
    int index = 1;
    if (getDoExtraSD())
    {
        if (getDoPower() || getDoMirOffset())
        {
            index = 2;
        }
        QStringList values(params->getParameter(index));
        for (int j = 0; j < values.count(); j++)
            txt.append(QString("  %1 ").arg(values.at(j)));
    }
    return txt;
}

QStringList q_ratio::getExtraParam()
{
    int index = 1;
    if (getDoPower() || getDoMirOffset())
    {
        index = 2;
    }
    return params->getParameter(index);
}

void q_ratio::setExtra(QStringList values)
{
    params->setParamData(2, values);
    varParams->setParameter(2, values);
}

QString q_ratio::getLink()
{
    QString txt("");
    if (setup->getValue(0) > 0)
    {
        QStringList values(params->getParameter(0));
        for (int j = 0; j < values.count(); j++)
            txt.append(QString("  %1 ").arg(values.at(j)));
    }
    return txt;
}

QStringList q_ratio::getLinkParam()
{
    return params->getParameter(0);
}

void q_ratio::setLink(QStringList values)
{
    params->setParamData(0, values);
    varParams->setParameter(0, values);
}

void q_ratio::changeSetup(int errType)
{
    setParamHdrs(errType);
}

/*void q_ratio::changeSetup()
{

}*/







