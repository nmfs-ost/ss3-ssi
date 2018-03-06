#include "seasonaleffectsmodel.h"

seasonalEffectsModel::seasonalEffectsModel(QObject *parent) : QObject(parent)
{
    QStringList headr;
    setup = new setupModel(parent);
    params = new shortParamMultModel(parent);

    connect (setup, SIGNAL(dataChanged(QList<int>)), SLOT(changeParameters(QList<int>)));
    connect (params, SIGNAL(paramsChanged()), SIGNAL(parametersChanged()));

    headr << "FmWtLn1" << "FmWtLn2" ;
    headr << "Matur1" << "Matur2" ;
    headr << "Fecnd1" << "Fecnd2" ;
    headr << "MlWtLn1" << "MlWtLn2" ;
    headr << "L1" << "K";
    setup->setHeader(headr);
    headr.clear();
    headr << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    setup->setData(headr);
    setNumSeasons(1);
//    setParamHeaders();
}

seasonalEffectsModel::~seasonalEffectsModel()
{
    delete params;
    delete setup;
}

// set multiplier for number of parameters, typically num of seasons
//
/*void seasonalEffectsModel::setMultiplier(int mlt)
{
    int i, j, total;
    int beg, end;
    int row = 0;
    int numparams = getNumSetupVals();
    QStringList parm;
    QString hdrPrm;
    QString hdr;
    parm << "0" << "0" << "0" << "0" << "0" << "0" << "0";

    total = numparams * mlt;
    if (mlt > mult)
    {
        params->setParamCount(total);
        // insert parameter lines for each category in setup
        for (i = 0; i < numparams; i++)
        {
            hdrPrm = getSetupColHeader(i);
            beg = i * mult;
            end = beg + mult;
            for (j = beg, row = 0; j < end; j++, row++)
            {
                params->setParameter(row, paramData->getParameter(j));
                setParamHeader(row, paramData->getParamHeader(j));
            }

            end = beg + mlt;
            for (; j < end; j++, row++)
            {
                hdr = QString("Seas%1 %2").arg(QString::number(j+1), hdrPrm);
                setParamData(row, parm);
                setParamHeader(row, hdr);
            }
        }
        paramData->setParamCount(total);
        for (i = 0; i < total; i++)
        {
            paramData->setParameter(i, getParamData(i));
            paramData->setParamHeader(i, getParamHeader(i));
        }
    }
    else if (mlt < mult)
    {
        params->setParamCount(total);
        // remove parameter lines for each category in setup
        for (i = 0; i < numparams; i++)
        {
            beg = i * mult;
            end = beg + mlt;
            for (j = beg, row = 0; j > end; j++, row++)
            {
                params->setParameter(row, paramData->getParameter(j));
                setParamHeader(row, paramData->getParamHeader(j));
            }
        }
        paramData->setParamCount(total);
        for (i = 0; i < total; i++)
        {
            paramData->setParameter(i, getParamData(i));
            paramData->setParamHeader(i, getParamHeader(i));
        }
    }
    if (mult != mlt)
    {
        mult = mlt;
        setParamHeaders();
        updateParams();
    }
}*/

void seasonalEffectsModel::changeParameters(QList<int> setupvals)
{
    params->setParamsUsed(setupvals);
    setParamHeaders();
}

void seasonalEffectsModel::setParamHeaders()
{
    int total, numparams = setup->getData().count();
    int mult = params->getMultiplier();
    int i, j;
    int row = 0;
    QString hdrPrm;
    QString hdr;

    total = numparams * mult;
    // set headers for each season, each param type
    params->setTotalNumParams(numparams);
    for (i = 0; i < numparams; i++)
    {
        hdrPrm = setup->getColHeader(i);
        for (j = 0; j < mult; j++, row++)
        {
            hdr = QString("Seas%1 %2").arg(QString::number(j+1), hdrPrm);
            params->setParamDataHeader(row, hdr);
        }
    }
}

void seasonalEffectsModel::setNumSeasons(int seas)
{
    int check = params->getMultiplier();
    if (check != seas)
    {
        params->setMultiplier(seas);
        params->checkParamData();
        setParamHeaders();
        params->updateParams();
    }
}

// Data has been changed programmatically
// Make sure background data is correct
// then update gui
//
/*void seasonalEffectsModel::changeParamData()
{
    QStringList param;
    int rows = params->getNumParams();
    int setupcols = setup->getNumSetupVals();

    rows = setupcols * mult;
    paramData->setParamCount(rows);
    for (int i = 0; i < rows; i++)
    {
        param = paramData->getParameter(i);
        for (int j = 0; j < param.count(); j++)
            if (QString(param.at(j)).isEmpty())
            {
                param.removeAt(j);
                param.insert(j, QString("0"));
            }
    }
//    setParamHeaders();
//    updateParams();
}*/

bool seasonalEffectsModel::getFemwtlen1() const
{
    int val = setup->getValue(0);
    return (val == 1);
}
void seasonalEffectsModel::setFemwtlen1(bool value)
{
    setup->setValue(0, (value? 1: 0));
}

bool seasonalEffectsModel::getFemwtlen2() const
{
    int val = setup->getValue(1);
    return (val == 1);
}
void seasonalEffectsModel::setFemwtlen2(bool value)
{
    setup->setValue(1, (value? 1: 0));
}

bool seasonalEffectsModel::getMat1() const
{
    int val = setup->getValue(2);
    return (val == 1);
}
void seasonalEffectsModel::setMat1(bool value)
{
    setup->setValue(2, (value? 1: 0));
}

bool seasonalEffectsModel::getMat2() const
{
    int val = setup->getValue(3);
    return (val == 1);
}
void seasonalEffectsModel::setMat2(bool value)
{
    setup->setValue(3, (value? 1: 0));
}

bool seasonalEffectsModel::getFec1() const
{
    int val = setup->getValue(4);
    return (val == 1);
}
void seasonalEffectsModel::setFec1(bool value)
{
    setup->setValue(4, (value? 1: 0));
}

bool seasonalEffectsModel::getFec2() const
{
    int val = setup->getValue(5);
    return (val == 1);
}
void seasonalEffectsModel::setFec2(bool value)
{
    setup->setValue(5, (value? 1: 0));
}

bool seasonalEffectsModel::getMalewtlen1() const
{
    int val = setup->getValue(6);
    return (val == 1);
}
void seasonalEffectsModel::setMalewtlen1(bool value)
{
    setup->setValue(6, (value? 1: 0));
}

bool seasonalEffectsModel::getMalewtlen2() const
{
    int val = setup->getValue(7);
    return (val == 1);
}
void seasonalEffectsModel::setMalewtlen2(bool value)
{
    setup->setValue(7, (value? 1: 0));
}

bool seasonalEffectsModel::getL1() const
{
    int val = setup->getValue(8);
    return (val == 1);
 }
void seasonalEffectsModel::setL1(bool value)
{
    setup->setValue(8, (value? 1: 0));
}

bool seasonalEffectsModel::getK() const
{
    int val = setup->getValue(9);
    return (val == 1);
}
void seasonalEffectsModel::setK(bool value)
{
    setup->setValue(9, (value? 1: 0));
}


