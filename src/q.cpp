#include "q.h"

q_ratio::q_ratio()
{
    QStringList hdr;
    hdr << "Link" << "Link_Info" << "ExtraSD" << "Bias_Adj" << "Float" ;
    qsetup = new parametermodel();
    qsetup->setColumnCount(5);
    qsetup->setHeader(hdr);

    hdr.clear();
    hdr << "Lo" << "Hi" << "Init" << "Prior" << "P_type" << "P_sd" << "Phase";
    hdr << "env-var" << "use_dev" << "dv_mnyr" << "dv_mxyr" << "dv_stdv" << "Block" << "Blk_Fxn";
    params = new parametermodel();
    params->setColumnCount(14);
    params->setHeader(hdr);
    params->setRowCount(1);
    params->setRowHeader(0, "LnQ_Base");
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
    setupChanged();
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
        txt.append(QString("      %1").arg(sList.at(i)));
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
    if (getDoPower() == 1)
        params->setRowHeader(i++, QString("Q_power_") + name);
    if (getDoExtraSD() == 1)
        params->setRowHeader(i++, QString("Q_extraSD_") + name);
    if (sList.at(0).compare("0"))//getType() > 0)
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
    int val = qsetup->getRowData(0).at(0).toInt();
    if (val == 3)
        val = 1;
    else
        val = 0;
    return val;
}

void q_ratio::setDoPower(int value)
{
    if (doPower != value)
    {
        QStringList sList (qsetup->getRowData(0));
        if (sList.at(0).toInt() != 3)
        {
            if (value)

            sList.removeAt(0);
            sList.insert(0, "3");
            sList.removeAt(1);
            sList.insert(1, "0");
            doPower = value;
        }
        setupChanged();
    }
}

int q_ratio::getDoEnvLink() const // obsolete
{
    return 0;// qsetup->getRowData(0).at(1).toInt();// doEnvVar;
}

void q_ratio::setDoEnvLink(int value)
{
    QStringList sList (params->getRowData(0));
    while (sList.count() < 14)
    {
        sList.append("0");
    }
    if (sList.at(7).toInt() != value)
    {
        sList.removeAt(7);
        if (value)
            sList.insert(7, "1");
        else
            sList.insert(7, "0");
    }

    /*if (sList.at(1).toInt() != value)
     {
        sList.removeAt(1);
        sList.insert(1, QString::number(value));
        doEnvVar = value;
        if (value == 1)
            EnvIndex = powerIndex + 1;
        else
            EnvIndex = -1;
        setDoExtraSD(doExtraSD);
        setType(type);
    }*/
}

int q_ratio::getDoExtraSD() const
{
    return qsetup->getRowData(0).at(2).toInt();//doExtraSD;
}

void q_ratio::setDoExtraSD(int value)
{
    QStringList sList (qsetup->getRowData(0));
    if (sList.at(2).toInt() != value)
    {
        sList.removeAt(2);
        sList.insert(2, QString::number(value));
        doExtraSD = value;
        setupChanged();
/*        if (value == 1)
        {
            ExtraIndex = (EnvIndex != -1)? EnvIndex + 1: powerIndex + 1;
        }
        else
            ExtraIndex = -1;
        setType(type);*/
    }
}

int q_ratio::getType() const // obsolete
{
    return type;
}

void q_ratio::setType(int value)
{
    type = value;
    // assumes setup of "1 0 0 0 0"
/*    QStringList sList (qsetup->getRowData(0));
    if (value < 0) // mirror (1 parameter)
    {
        sList.removeAt(0);
        sList.insert(0, "2");
        sList.removeAt(1);
        sList.insert(0, QString::number(abs(value)));
    }
    switch (value)
    {
    case 0:  // float
        sList.removeAt(4);
        sList.insert(4, "1");
        break;
    case 5:  // bias adj
        sList.removeAt(3);
        sList.insert(3, "1");
    }

    if (value > 0 && value < 5)
    {
        params->setRowHeader(0, "LnQ_Base");
    }
    else
    {
        params->setRowHeader(0, "Q_Base");
    }
    setupChanged();
    if (sList.at(3).toInt() != value)
    {
        sList.removeAt(3);
        sList.insert(3, QString::number(value));
        type = value;
        if (type > 1)
        {
            typeIndex = powerIndex + 1;
            if (typeIndex == EnvIndex) typeIndex += 1;
            if (typeIndex == ExtraIndex) typeIndex += 1;
        }
        else
            typeIndex = -1;
    }*/
}

float q_ratio::getOffset() const
{
    return qsetup->getRowData(0).at(4).toInt();//offset;
}

void q_ratio::setOffset(float value)
{
    QStringList sList (qsetup->getRowData(0));
    if (sList.at(4).toInt() != value)
    {
        sList.removeAt(4);
        sList.insert(4, QString::number(value));
        doOffset = value > 0;
        offset = value;
    }
}

QString q_ratio::getPower() const
{
    QString txt("");
    if (powerIndex == 0)
    {
        QStringList values(params->getRowData(powerIndex));
        for (int j = 0; j < values.count(); j++)
            txt.append(QString("  %1 ").arg(values.at(j)));
    }
    return txt;
}

void q_ratio::setPower(QStringList values)
{
    if (powerIndex > 0)
        params->setRowData(powerIndex, values);
}

QString q_ratio::getVariable() const
{
    QString txt("");
    if (EnvIndex > -1)
    {
        QStringList values(params->getRowData(EnvIndex));
        for (int j = 0; j < values.count(); j++)
            txt.append(QString("  %1 ").arg(values.at(j)));
    }
    return txt;
}

void q_ratio::setEnvLink(QStringList values)
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
            txt.append(QString("  %1 ").arg(values.at(j)));
    }
    return txt;
}

void q_ratio::setBase(QStringList values)
{
    typeIndex = 0;
    params->setRowData(typeIndex, values);
}

void q_ratio::setupChanged()
{
    int id = 1;
    int option = 0;
    QStringList sList(getSetupModel()->getRowData(0));
    QString hdr;

    typeIndex = 0;
    option = sList.at(0).toInt();
    params->setRowHeader(0, QString("LnQ_Base"));
    if (type != option)
    {
        if (type == 3)
        {
            if (doPower != 1)
            {
                doPower = 1;
                powerIndex = id++;
                params->insertRow(powerIndex);
                params->setRowHeader(powerIndex, QString("Q_Power"));
            }
        }
        else if (doPower == 1)
        {
            doPower = 0;
            params->removeRow(powerIndex);
            powerIndex = -1;
        }
    }

    option = sList.at(2).toInt();
    if (option != doExtraSD)
    {
        doExtraSD = option;
        if (doExtraSD)
        {
            ExtraIndex = id++;
            params->insertRow(ExtraIndex);
            params->setRowHeader(ExtraIndex, QString("Q_ExtraSD"));
        }
        else
        {
            params->removeRow(ExtraIndex);
            ExtraIndex = -1;
        }
    }

/*    option = sList.at(1).toInt();
    if (option != doEnvVar)
    {
        doEnvVar = option;
        if (doEnvVar)
        {
            EnvIndex = id++;
            params->insertRow(EnvIndex);
            params->setRowHeader(EnvIndex, QString("Q_Envlink"));
        }
        else
        {
            params->removeRow(EnvIndex);
            EnvIndex = -1;
        }
    }*/
}







