#include "q_ratio.h"
#include "setupmodel.h"

q_ratio::q_ratio(ss_model *model)
    : setupParamVarModel(model)
{
    QStringList hdr;
    hdr << "Link" << "Link_Info" << "ExtraSD" << "Bias_Adj" << "Float" ;
    setSetupHeader(hdr);
    setNumParams(0);
/*    setParamHeader(0, "LnQ_Base");

    hdr.clear();
    hdr << "Lo" << "Hi" << "Init" << "Prior" << "P_type" << "P_sd" << "Phase";
    hdr << "env-var" << "use_dev" << "dv_mnyr" << "dv_mxyr" << "dv_stdv" << "Block" << "Blk_Fxn";
    params = new parameterModelTV(model);
    params->setColumnCount(14);
    params->setHeader(hdr);
    params->setNumParams(1);
    params->setParamHeader(0, "LnQ_Base");
    reset();*/
}

q_ratio::~q_ratio()
{
/*    reset();
    delete params;
    delete qsetup;*/
}


void q_ratio::reset()
{
//    qsetup->setParamCount(0);
//    params->setNumParams(0);
    setNumParams(0);
    setDoPower(0);
    powerIndex = -1;
    setDoEnvLink(0);
    EnvIndex = -1;
    setDoExtraSD(0);
    ExtraIndex = -1;
    setType(0);
    typeIndex = -1;
    setOffset(0);
    doOffset = false;
}

/*q_ratio *q_ratio::copy(q_ratio *rhs)
{
    int numprms = rhs->getNumParams();
    QString str;
    QStringList strlst;
    if (rhs->qsetup->getParamCount() == 1)
    {
        strlst = rhs->getSetupModel()->getParameter(0);
        qsetup->setRowData(0, strlst);
    }
    for (int i = 0; i < numprms; i++)
    {
        strlst = rhs->getParameter(i);
        setParameter(i, strlst);
        str = rhs->getParamModel()->getParamHeader(i);
        params->setParamHeader(i, str);
    }
    setDoPower  (rhs->getDoPower());
    setDoEnvLink(rhs->getDoEnvLink());
    setDoExtraSD(rhs->getDoExtraSD());
    setType     (rhs->getType());
    setOffset   (rhs->getOffset());
    return this;
}*/

void q_ratio::setup(QStringList values)
{
    setSetupData(values);
//    qsetup->setRowData(0, values);
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
//    setupChanged();
}

void q_ratio::setup (QString str)
{
    QStringList sList (str.split(' ', QString::SkipEmptyParts));
    setup (sList);
}

QString q_ratio::getSetup()
{
    QStringList sList (getSetupData());
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
/*
void q_ratio::setParamHdrs(QString name)
{
    QStringList sList (qsetup->getParameter(0));
    int i = 0;
    if (getDoPower() == 1)
        params->setParamHeader(i++, QString("Q_power_") + name);
    if (getDoExtraSD() == 1)
        params->setParamHeader(i++, QString("Q_extraSD_") + name);
    if (sList.at(0).compare("0"))//getType() > 0)
        params->setParamHeader(i++, QString("lnQ_base_") + name);
}

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

void q_ratio::setTypeIndex(int value)
{
    typeIndex = value;
}

int q_ratio::getDoPower()
{
    int val = getSetupValue(0);//qsetup->getParameter(0).at(0).toInt();
    if (val == 3)
        val = 1;
    else
        val = 0;
    return val;
}

void q_ratio::setDoPower(int value)
{
    int curVal = getSetupValue(0);
    if (doPower != value)
    {
//        QStringList sList (qsetup->getParameter(0));
        if (curVal != 3)//sList.at(0).toInt() != 3)
        {
//            if (value)
            setSetupValue(0, 3);
            setSetupValue(1, 0);
/*            sList.removeAt(0);
            sList.insert(0, "3");
            sList.removeAt(1);
            sList.insert(1, "0");*/
            doPower = value;
        }
//        setupChanged();
    }
}

int q_ratio::getDoEnvLink() // obsolete
{
    return 0;// qsetup->getRowData(0).at(1).toInt();// doEnvVar;
}

void q_ratio::setDoEnvLink(int value)
{
    QStringList sList (getParamData(0));

    if (sList.at(7).toInt() != value)
    {
        sList.removeAt(7);
        if (value)
            sList.insert(7, QString::number(value));//"1");
        else
            sList.insert(7, QString::number(0));
    }
    setParamData(0, sList);

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

int q_ratio::getDoExtraSD()
{
    int val = getSetupValue(2);
    return val;
//    return qsetup->getParameter(0).at(2).toInt();//doExtraSD;
}

void q_ratio::setDoExtraSD(int value)
{
//    QStringList sList (qsetup->getParameter(0));
    if (getDoExtraSD() != value)
    {
        setSetupValue(2, value);
//        sList.removeAt(2);
  //      sList.insert(2, QString::number(value));
    //    doExtraSD = value;
/*        setupChanged();
        if (value == 1)
        {
            ExtraIndex = (EnvIndex != -1)? EnvIndex + 1: powerIndex + 1;
        }
        else
            ExtraIndex = -1;
        setType(type);*/
    }
}

int q_ratio::getType() // obsolete
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

float q_ratio::getOffset()
{
    return getSetupValue(4);
//    return qsetup->getParameter(0).at(4).toInt();//offset;
}

void q_ratio::setOffset(float value)
{
    QStringList sList (getSetupData());
    if (getOffset() != value)//sList.at(4).toInt() != value)
    {
        setSetupValue(4, value);
//        sList.removeAt(4);
 //       sList.insert(4, QString::number(value));
        doOffset = value > 0;
        offset = value;
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

QString q_ratio::getVariable()
{
    QString txt("");
    if (EnvIndex > -1)
    {
        QStringList values(params->getParameter(EnvIndex));
        for (int j = 0; j < values.count(); j++)
            txt.append(QString("  %1 ").arg(values.at(j)));
    }
    return txt;
}

void q_ratio::setEnvLink(QStringList values)
{
    if (EnvIndex > -1)
    {
        params->setParameter(EnvIndex, values);
        params->setParamHeader(EnvIndex, QString("Q_env"));
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
        params->setParamHeader(ExtraIndex, QString("Q_extra_SD"));
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
    setParamData(typeIndex, values);
    setParamHeader(typeIndex, QString("LnQ_Base"));
}

void q_ratio::changeSetup(int errType)
{
    if (getNumParams() < 1)
        setNumParams(1);
    setParamHeader(0, QString("LnQ_Base"));
    if (errType > 0)
        setParamHeader(0, QString("Q_Base"));
//    changeSetup();
}

void q_ratio::changeSetup()
{

}

/*
void q_ratio::setupChanged(int error_type)
{
}

void q_ratio::setupChanged()
{
    int id = 1;
    int option = 0;
    QStringList sList(getSetupData());

    typeIndex = 0;
    option = sList.at(0).toInt();
    if (type != option)
    {
        type = option;
        if (type == 3)
        {
            if (doPower != 1)
            {
                doPower = 1;
                powerIndex = id++;
                params->insertRow(powerIndex);
            }
        }
        else if (doPower == 1)
        {
            doPower = 0;
            params->removeRow(powerIndex);
            powerIndex = -1;
        }
    }

    option = sList.at(2).toInt() > 0? 1: 0;
    if (option != doExtraSD)
    {
        doExtraSD = option;
        if (doExtraSD)
        {
            ExtraIndex = id++;
            params->insertRow(ExtraIndex);
        }
        else
        {
            params->removeRow(ExtraIndex);
            ExtraIndex = -1;
        }
    }
    params->setNumParams(id);
    if (powerIndex > 0)
        params->setParamHeader(powerIndex, QString("Q_Power"));
    if (ExtraIndex > 0)
        params->setParamHeader(ExtraIndex, QString("Q_ExtraSD"));
*/
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
//}
/*
void q_ratio::setTVBlkParam(int index, int row, QStringList data)
{
    params->setTimeVaryBlkParam (index, row, data);
}

void q_ratio::setTVDevParam(int index, int row, QStringList data)
{
    params->setTimeVaryDevParam (index, row, data);
}

void q_ratio::setTVEnvParam(int index, QStringList data)
{
    params->setTimeVaryEnvParam (index, data);
}
*/





