#include "seasonaleffectsmodel.h"

seasonalEffectsModel::seasonalEffectsModel(ss_model *parent) : setupShortParamModel(parent)
{
    QStringList headr;
    headr << "FmWtLn1" << "FmWtLn2" ;
    headr << "Matur1" << "Matur2" ;
    headr << "Fecnd1" << "Fecnd2" ;
    headr << "MlWtLn1" << "MlWtLn2" ;
    headr << "L1" << "K";
    setSetupHeader(headr);
    headr.clear();
    for (int i = 0; i < 10; i++)
        headr.append("0");
    setSetupData(headr);
    setMultiplier(1);
}


// set multiplier for number of parameters, typically num of seasons
//
void seasonalEffectsModel::setMultiplier(int mlt)
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
}

void seasonalEffectsModel::setParamHeaders()
{
    int total, numparams = getNumSetupVals();
    int i, j;
    int beg, end, row = 0;
    QString hdrPrm;
    QString hdr;

    total = numparams * mult;
    // set headers for each season, each param type
    for (i = 0; i < numparams; i++)
    {
        hdrPrm = getSetupColHeader(i);
        for (j = 0; j < mult; j++, row++)
        {
            hdr = QString("Seas%1 %2").arg(QString::number(j+1), hdrPrm);
            paramData->setParamHeader(row, hdr);
        }
    }
}

void seasonalEffectsModel::setNumSeasons(int seas)
{
    setMultiplier(seas);
    changeParamData();
}

// Data has been changed programmatically
// Make sure background data is correct
// then update gui
//
void seasonalEffectsModel::changeParamData()
{
    QStringList param;
    int rows = paramData->getParamCount();
    int setupcols = setup->columnCount();

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
    setParamHeaders();
    updateParams();
}

bool seasonalEffectsModel::getFemwtlen1() const
{
    int val = setupData.at(0);
    return (val == 1);
}
void seasonalEffectsModel::setFemwtlen1(bool value)
{
    setSetupValue(0, (value? 1: 0));
}

bool seasonalEffectsModel::getFemwtlen2() const
{
    int val = setupData.at(1);
    return (val == 1);
}
void seasonalEffectsModel::setFemwtlen2(bool value)
{
    setSetupValue(1, (value? 1: 0));
}

bool seasonalEffectsModel::getMat1() const
{
    int val = setupData.at(2);
    return (val == 1);
}
void seasonalEffectsModel::setMat1(bool value)
{
    setSetupValue(2, (value? 1: 0));
}

bool seasonalEffectsModel::getMat2() const
{
    int val = setupData.at(3);
    return (val == 1);
}
void seasonalEffectsModel::setMat2(bool value)
{
    setSetupValue(3, (value? 1: 0));
}

bool seasonalEffectsModel::getFec1() const
{
    int val = setupData.at(4);
    return (val == 1);
}
void seasonalEffectsModel::setFec1(bool value)
{
    setSetupValue(4, (value? 1: 0));
}

bool seasonalEffectsModel::getFec2() const
{
    int val = setupData.at(5);
    return (val == 1);
}
void seasonalEffectsModel::setFec2(bool value)
{
    setSetupValue(5, (value? 1: 0));
}

bool seasonalEffectsModel::getMalewtlen1() const
{
    int val = setupData.at(6);
    return (val == 1);
}
void seasonalEffectsModel::setMalewtlen1(bool value)
{
    setSetupValue(6, (value? 1: 0));
}

bool seasonalEffectsModel::getMalewtlen2() const
{
    int val = setupData.at(7);
    return (val == 1);
}
void seasonalEffectsModel::setMalewtlen2(bool value)
{
    setSetupValue(7, (value? 1: 0));
}

bool seasonalEffectsModel::getL1() const
{
    int val = setupData.at(8);
    return (val == 1);
 }
void seasonalEffectsModel::setL1(bool value)
{
    setSetupValue(8, (value? 1: 0));
}

bool seasonalEffectsModel::getK() const
{
    int val = setupData.at(9);
    return (val == 1);
}
void seasonalEffectsModel::setK(bool value)
{
    setSetupValue(9, (value? 1: 0));
}


