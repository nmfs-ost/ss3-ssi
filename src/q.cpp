#include "q.h"

q_ratio::q_ratio()
{
    QStringList hdr;
    hdr << "fleet" << "link" << "link_info" << "biasadj" << "float" << "fleetname";
    qsetup = new parametermodel();
    qsetup->setColumnCount(6);
    qsetup->setHeader(hdr);


    hdr.clear();
    hdr << "Lo" << "Hi" << "Init" << "Prior" << "P_type" << "P_sd" << "Phase";
    hdr << "env-var" << "use_dev" << "dv_mnyr" << "dv_mxyr" << "dv_stdv" << "Block" << "Blk_Fxn";
    params = new parametermodel();
    params->setHeader(hdr);
    reset();
}

q_ratio::~q_ratio()
{
    reset();
    delete params;
}

void q_ratio::reset()
{
    qsetup->setRowCount(0);
    params->setRowCount(0);
    doPower = 0;
    powerIndex = -1;
    doEnvVar = 0;
    EnvIndex = -1;
    doExtraSD = 0;
    ExtraIndex = -1;
    type = 0;
    typeIndex = -1;
    offset = 0;
    doOffset = false;
}

q_ratio *q_ratio::copy(q_ratio *rhs)
{
    int numprms = rhs->getNumParams();
    QString str;
    QStringList strlst;
    if (rhs->qsetup->rowCount() == 1)
    {
        strlst = rhs->getSetupModel()->getRowData(0);
        qsetup->setRowData(0, strlst);
    }
    for (int i = 0; i < numprms; i++)
    {
        strlst = rhs->getParameter(i);
        setParameter(i, strlst);
        str = rhs->getParamModel()->getRowHeader(i);
        params->setRowHeader(i, str);
    }
    setDoPower  (rhs->getDoPower());
    setDoEnvLink(rhs->getDoEnvLink());
    setDoExtraSD(rhs->getDoExtraSD());
    setType     (rhs->getType());
    setOffset   (rhs->getOffset());
}

void q_ratio::setup(QStringList values)
{
    qsetup->setRowData(0, values);
/*    reset();
    setDoPower(values.at(0).toInt());
    setDoEnvLink(values.at(1).toInt());
    setDoExtraSD(values.at(2).toInt());
    setType(values.at(3).toInt());
    if (values.count() > 4)
    {
        doOffset = true;
        setOffset(values.at(4).toFloat());
    }*/
}

void q_ratio::setup (QString str)
{
    QStringList sList (str.split(' ', QString::SkipEmptyParts));
    setup (sList);
}

QString q_ratio::getSetup()
{
    QStringList sList (qsetup->getRowData(0));
    QString txt("");
    for (int i = 0; i < sList.count(); i++)
        txt.append(QString(" %1").arg(sList.at(i)));
/*    txt.append(QString(" %1").arg(QString::number(doPower)));
    txt.append(QString(" %1").arg(QString::number(doEnvVar)));
    txt.append(QString(" %1").arg(QString::number(doExtraSD)));
    txt.append(QString(" %1").arg(QString::number(type)));
//    if (doOffset)
        txt.append(QString(" %1").arg(QString::number(offset)));*/
    return txt;
}

void q_ratio::setParamHdrs(QString name)
{
    QStringList sList (qsetup->getRowData(0));
    int i = 0;
    if (sList.at(0).compare("0"))//getDoPower() == 1)
        params->setRowHeader(i++, QString("Q_power_") + name);
    if (sList.at(0).compare("1"))//getDoEnvLink() == 1)
        params->setRowHeader(i++, QString("Q_envlink_") + name);
    if (sList.at(0).compare("2"))//getDoExtraSD() == 1)
        params->setRowHeader(i++, QString("Q_extraSD_") + name);
    if (sList.at(0).compare("3"))//getType() > 0)
        params->setRowHeader(i++, QString("lnQ_base_") + name);
}

void q_ratio::setNumParams(int num)
{
    params->setRowCount(num);
}

int q_ratio::getNumParams()
{
    return params->rowCount();
}

void q_ratio::setParameter(int index, QStringList values)
{
    if (index < params->rowCount())
        params->setRowCount(index + 1);
    params->setRowData(index, values);
}

void q_ratio::setParameter(int index, QString text)
{
    QStringList values(text.split(' ', QString::SkipEmptyParts));
    setParameter (index, values);
}

QStringList q_ratio::getParameter(int index)
{
    return params->getRowData(index);
}

int q_ratio::getTypeIndex() const
{
    return typeIndex;
}

void q_ratio::setTypeIndex(int value)
{
    typeIndex = value;
}

int q_ratio::getDoPower() const
{
    return qsetup->getRowData(0).at(0).compare("0");// doPower;
}

void q_ratio::setDoPower(int value)
{
    QStringList sList (qsetup->getRowData(0));

    doPower = value;
    if (value == 1)
        powerIndex = 0;
    else
        powerIndex = -1;
    setDoEnvLink(doEnvVar);
}

int q_ratio::getDoEnvLink() const
{
    return doEnvVar;
}

void q_ratio::setDoEnvLink(int value)
{
    doEnvVar = value;
    if (value == 1)
        EnvIndex = powerIndex + 1;
    else
        EnvIndex = -1;
    setDoExtraSD(doExtraSD);
    setType(type);
}

int q_ratio::getDoExtraSD() const
{
    return doExtraSD;
}

void q_ratio::setDoExtraSD(int value)
{
    doExtraSD = value;
    if (value == 1)
    {
        ExtraIndex = (EnvIndex != -1)? EnvIndex + 1: powerIndex + 1;
    }
    else
        ExtraIndex = -1;
    setType(type);
}

int q_ratio::getType() const
{
    return type;
}

void q_ratio::setType(int value)
{
    type = value;
    if (type > 1)
    {
        typeIndex = powerIndex + 1;
        if (typeIndex == EnvIndex) typeIndex += 1;
        if (typeIndex == ExtraIndex) typeIndex += 1;
    }
    else
        typeIndex = -1;
}

float q_ratio::getOffset() const
{
    return offset;
}

void q_ratio::setOffset(float value)
{
    doOffset = value > 0;
    offset = value;
}

QString q_ratio::getPower() const
{
    QString txt("");
    if (powerIndex == 0)
    {
        QStringList values(params->getRowData(powerIndex));
        for (int j = 0; j < values.count(); j++)
            txt.append(QString(" %1").arg(values.at(j)));
    }
    return txt;
}

void q_ratio::setPower(QStringList values)
{
    if (powerIndex == 0)
        params->setRowData(powerIndex, values);
}

QString q_ratio::getVariable() const
{
    QString txt("");
    if (EnvIndex > -1)
    {
        QStringList values(params->getRowData(EnvIndex));
        for (int j = 0; j < values.count(); j++)
            txt.append(QString(" %1").arg(values.at(j)));
    }
    return txt;
}

void q_ratio::setVariable(QStringList values)
{
    if (EnvIndex > -1)
    {
        params->setRowData(EnvIndex, values);
    }
}

QString q_ratio::getExtra() const
{
    QString txt("");
    if (ExtraIndex > -1)
    {
        QStringList values(params->getRowData(ExtraIndex));
        for (int j = 0; j < values.count(); j++)
            txt.append(QString(" %1").arg(values.at(j)));
    }
    return txt;
}

void q_ratio::setExtra(QStringList values)
{
    if (ExtraIndex > -1)
    {
        params->setRowData(ExtraIndex, values);
    }
}

QString q_ratio::getBase() const
{
    QString txt("");
    if (typeIndex > -1)
    {
        QStringList values(params->getRowData(typeIndex));
        for (int j = 0; j < values.count(); j++)
            txt.append(QString(" %1").arg(values.at(j)));
    }
    return txt;
}

void q_ratio::setBase(QStringList values)
{
    if (typeIndex > -1)
    {
        params->setRowData(typeIndex, values);
    }
}









