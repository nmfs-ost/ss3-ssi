#include "setupmodel.h"
#include "model.h"

#ifndef min
#define min(a,b) ((a)<(b))?(a):(b)
#endif

setupModel::setupModel(ss_model *parent) : QObject((QObject *)parent)
{
    parentModel = parent;

    QStringList vals ("0");
    setup = new tablemodel(this);
    setup->setRowCount(1);
    setNumSetupVals(vals.count());
    setSetupData(vals);

    connect (setup, SIGNAL(dataChanged()), SLOT(updateSetupData()));
}

setupModel::~setupModel()
{
    delete setup;
}

void setupModel::setNumSetupVals(int cols)
{
    int num = setup->columnCount();
    if (cols != num)
    {
        for (int i = num; i < cols; i++)
        {
            setupData.append(0);
        }

        for (int i = cols; i < num; i++)
        {
            setupData.takeLast();
        }
        setup->setColumnCount(cols);
    }
}

void setupModel::setSetupData(QStringList data)
{
    setNumSetupVals(data.count());
    setup->setRowData(0, data);
}

void setupModel::setSetupHeader(QStringList hdr)
{
    setNumSetupVals(hdr.count());
    setup->setHeader(hdr);
}

void setupModel::setSetupValue (int i, int value)
{
    QStringList vals;// (getSetupData());
    QString item (QString::number(value));
    if (i >= setup->columnCount())
        setNumSetupVals(i + 1);

    vals = getSetupData();
    vals.insert(i, item);
    vals.removeAt(i + 1);
    setSetupData(vals);
//    setupData[i] = value;
}

int setupModel::getSetupValue (int i)
{
    QStringList vals (getSetupData());
    int value = QString(vals.at(i)).toInt();
    return value;
}

void setupModel::updateSetupData()
{
    QStringList vals (getSetupData());
    int item = 0;
    int dataItem = 0;
    bool changed = false;

    for (int i = 0; i < vals.count(); i++)
    {
        item = QString(vals.at(i)).toInt();
        dataItem = setupData[i];
        if (dataItem != item)
        {
            setupData[i] = item;
            changed = true;
        }
    }
    if (changed)
        emit setupChanged();
}



setupShortParamModel::setupShortParamModel(ss_model *parent) : setupModel(parent)
{
    mult = 1;
    params = new shortParameterModel(this);
    paramData = new shortParameterModel(this);

    connect (this, SIGNAL(setupChanged()), SLOT(changeParamData()));
    connect (params, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
             SLOT(updateParamData(QModelIndex,QModelIndex,QVector<int>)));

}

setupShortParamModel::~setupShortParamModel()
{
    delete paramData;
    delete params;
}

// set multiplier for number of parameters, typically num of seasons
//
void setupShortParamModel::setMultiplier(int mlt)
{
    int i, j, total;
    int beg, end;
    int row = 0;
    int numparams = getNumSetupVals();
    QStringList parm;
    QStringList param;
    shortParameterModel *newParamData = new shortParameterModel(this);

    if (mlt != mult)
    {
        parm <<"0" <<"0" <<"0" <<"0" <<"0" <<"0" <<"0";
        total = numparams * mlt;
        newParamData->setParamCount(total);
        for (i = 0; i < numparams; i++)
        {
            if (mlt > mult)
            {
                beg = i * mult; end = beg + mult;
                for (j = beg; j < end; j++, row++)
                {
                    param = paramData->getParameter(j);
                    newParamData->setParameter(row, param);
                }
                end = beg + mlt;
                for (; j < end; j++, row++)
                {
                    newParamData->setParameter(row, parm);
                }
            }
            else
            {
                beg = i * mult; end = beg + mlt;
                for (j = beg; j < end; j++, row++)
                {
                    param = paramData->getParameter(j);
                    newParamData->setParameter(row, param);
                }
            }
            newParamData->setParamCount(row);
        }
        delete paramData;
        paramData = newParamData;
/*        {
            params->setParamCount(total);
            // insert parameter lines for each category in setup
            for (i = numparams; i > 0; i--)
            {
                beg = i * mult;
                end = beg + mult;
                for (j = beg; j > end; j++)
                    params->setParameter(row++, paramData->getParameter(j));
                beg = end;
                end = end + mlt;
                for (j = beg; j > end; j++)
                    setParamData(row++, parm);
            }
            paramData->setParamCount(total);
            for (i = 0; i < total; i++)
                paramData->setParameter(i, params->getParameter(i));
        }
        else if (mlt < mult)
        {
            params->setParamCount(total);
            // remove parameter lines for each category in setup
            for (i = numparams; i > 0; i--)
            {
                beg = i * mult;
                end = beg + mlt;
                for (j = beg; j > end; j++)
                    params->setParameter(row++, paramData->getParameter(j));
            }
            paramData->setParamCount(total);
            for (i = 0; i < total; i++)
                paramData->setParameter(i, params->getParameter(i));
        }*/
        mult = mlt;
        setParamHeaders();
        updateParams();
    }
}

// Set headers with multiplier number
//
void setupShortParamModel::setParamHeaders()
{
    int i, j, total;
    int row = 0;
    int numparams = getNumSetupVals();
    QString ColHdr;

    total = numparams * mult;
    for (i = 0; i < numparams; i++)
    {
        ColHdr = getSetupColHeader(i);
        for (j = 0; j < mult; j++, row++)
        {
            paramData->setParamHeader(row, QString("Mult%1 %2").arg(
                                          QString::number(j+1), ColHdr));
        }
    }
}

// change background data, then update gui table
//
void setupShortParamModel::changeParamData()
{
//    int row = 0;
    QStringList param; //QStringList parm;
    int rows = paramData->getParamCount();
    int setupcols = setup->columnCount();
//    parm << "0" << "0" << "0" << "0" << "0" << "0" << "0";

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
//                param[j] = QString("0");
            }
    }
//    setParamHeaders();
    updateParams();
/*
    for (int i = 0; i < setupcols; i++)
    {
        if (useParam(i))
        {
            int beg = i * mult;
            int end = beg + mult;
            for (int j = beg; j < end; j++)
            {
                parm = paramData->getParameter(j);
                params->setParameter(row, parm);
                params->setParamHeader(row++, paramData->getParamHeader(j));
            }
        }
    }
    params->setParamCount(row);*/
}

// Kludge for output routine
// output whole parameter as one string
//
QString setupShortParamModel::getParamText (int row)
{
    QString txt;
    QStringList slist (getParamData(row));
    for (int i = 0; i < slist.count(); i++)
        txt.append(QString(" %1").arg(slist.at(i)));

    return txt;
}


// the background data has changed
// change the gui table to match
//
void setupShortParamModel::updateParams()
{
    QStringList parm;
    int row = 0;
    for (int i = 0; i < setup->columnCount(); i++)
    {
        if (useParam(i))
        {
            int beg = i * mult;
            int end = beg + mult;
            for (int j = beg; j < end; j++)
            {
                parm = paramData->getParameter(j);
                setParamData(row, parm);
                setParamHeader(row++, paramData->getParamHeader(j));
            }
        }
    }
    setNumParams(row);
}

// the gui table has changed
// change all background data to match
//
void setupShortParamModel::updateParamData(QModelIndex tl, QModelIndex br, QVector<int> data)
{
    int row = 0;
    QStringList parm;
    int first = tl.row();
    int last = br.row();

    for (int i = 0; i < setup->columnCount(); i++)
    {
        if (useParam(i))
        {
            int beg = i * mult;
            int end = beg + mult;
            for (int j = beg; j < end; j++)
            {
                if (row >= first && row <= last)
                {
                    parm = params->getParameter(row);
                    paramData->setParameter(i, parm);
                }
                row++;
            }
        }
    }

}



setupLongParamModel::setupLongParamModel(ss_model *parent) : setupModel(parent)
{
    params = new longParameterModel(this);
    paramData = new longParameterModel(this);
    QStringList parm;
    parm << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    parm << "0" << "0" << "0" << "0" << "0" << "0" << "0";

    connect (setup, SIGNAL(dataChanged()), SLOT(changeParamData()));
    connect (params, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
             SLOT(updateParamData(QModelIndex,QModelIndex,QVector<int>)));

}

setupLongParamModel::~setupLongParamModel()
{
    delete paramData;
    delete params;
}

// change background data, then update gui table
//
void setupLongParamModel::changeParamData()
{
    int row = 0;
    QStringList parm;
    int rows = paramData->getParamCount();
    int setupcols = setup->columnCount();
    parm << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    parm << "0" << "0" << "0" << "0" << "0" << "0" << "0";

    paramData->setParamCount(setupcols);
/*    for (int i = rows; i < setupcols; i++)
    {
        paramData->setParameter(i, parm);
        paramData->setParamHeader(i, setup->getColumnHeader(i));
    }*/

    for (int i = 0; i < setup->columnCount(); i++)
    {
        parm = paramData->getParameter(i);
        if (useParam(i))
        {
            params->setParameter(row, parm);
            params->setParamHeader(row++, paramData->getParamHeader(i));
        }
    }
    params->setParamCount(row);
}

// Kludge for output routine
// output whole parameter as one string
//
QString setupLongParamModel::getParamText (int row)
{
    QString txt;
    QStringList slist (getParamData(row));
    for (int i = 0; i < slist.count(); i++)
        txt.append(QString(" %1").arg(slist.at(i)));

    return txt;
}


// the background data has changed
// change the gui table to match
//
void setupLongParamModel::updateParams()
{
    QStringList parm;
    int row = 0;
    for (int i = 0; i < setup->columnCount(); i++)
    {
        parm = paramData->getParameter(i);
        if (useParam(i))
        {
            setParamData(row, parm);
            setParamHeader(row++, paramData->getParamHeader(i));
        }
    }
    setNumParams(row);
}

// the gui table has changed
// change all background data to match
//
void setupLongParamModel::updateParamData(QModelIndex tl, QModelIndex br, QVector<int> data)
{
    int row = 0;
    QStringList parm;
    int first = tl.row();
    int last = br.row();

    for (int i = 0; i < setup->columnCount(); i++)
    {
        if (useParam(i))
        {
            if (row >= first && row <= last)
            {
                parm = params->getParameter(row);
                paramData->setParameter(i, parm);
            }
            row++;
        }
    }
}


// Create the setup model that includes parameters
// and associated time-varying parameters
//
setupParamVarModel::setupParamVarModel(ss_model *parent) : setupLongParamModel(parent)
{
    paramVars = new shortParameterModel(this);
    setParamVarsCount(0);

    connect (params, SIGNAL(dataChanged()), SLOT(changeParamVars()));
    connect (paramVars, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
             SLOT(updateParamVarsData(QModelIndex,QModelIndex,QVector<int>)));
}

// Class desctructor
//
setupParamVarModel::~setupParamVarModel()
{
    shortParameterModel *spm;
    setNumParams(0);
    // delete all background tables
    while (paramVarData.count() > 0)
    {
        spm = paramVarData.takeLast();
        delete spm;
    }
    // delete the gui table
    delete paramVars;
}

// set the gui table to correct number of rows
//
void setupParamVarModel::setParamVarsCount(int rows)
{
    paramVars->setParamCount(rows);
    for (int i = paramNum.count(); i < rows; i++)
    {
        paramNum.append(-1);
        paramvarNum.append(-1);
    }
    while (paramNum.count() > rows)
    {
        paramNum.takeLast();
        paramvarNum.takeLast();
    }

}

// set the number of background tables to the number of parameters
// also sets arrays of current values to correct length
//
void setupParamVarModel::setNumParamVarTables(int num)
{
    shortParameterModel *paramVarModel;
    int numtables = paramVarData.count();

    for (int i = numtables; i < num; i++)
    {
        paramVarModel = new shortParameterModel(this);
        paramVarModel->setParamCount(3);
        paramVarData.append(paramVarModel);
        blkVal.append(0);
        blkFxn.append(0);
        devVal.append(0);
        envVal.append(0);
    }
    while (num < paramVarData.count())
    {
        paramVarModel = paramVarData.takeLast();
        delete paramVarModel;
        blkVal.takeLast();
        blkFxn.takeLast();
        devVal.takeLast();
        envVal.takeLast();
    }
}

// generate any and all parameter time varying parameters
// and compare with the underlying data.
// if any have changed, return true, else return false
//
bool setupParamVarModel::generateParamVarData(int numparams)
{
    int paramNum = 0;
    int varParamNum = 0;
    int numvarparams;
    int varValue;
    int fxnValue;
    QStringList parmdata, parmvardata, newvardata;
    shortParameterModel *parmVarModel;
    bool varsChanged = false;

    // get param and paramVar table
    // generate a time-varying parameter and compare to existing parameter
    // if different, replace existing and set varsChanged to true.
    for (paramNum = 0; paramNum < numparams; paramNum++)
    {
        parmdata = params->getParameter(paramNum);
        parmVarModel = paramVarData.at(paramNum);
        numvarparams = parmVarModel->getParamCount();
        // Environmental variable time-varying
        parmvardata = parmVarModel->getParameter(varParamNum++);
        varValue = QString(parmdata.at(7)).toInt();
        if (envVal.at(paramNum) != varValue)
        {
            envVal[paramNum] = varValue;
            autoGenEnvVarParam (paramNum, varValue);
            varsChanged = true;
        }

        // Deviation time-varying
        parmvardata = parmVarModel->getParameter(varParamNum++);
        varValue = QString(parmdata.at(8)).toInt();
        if (devVal.at(paramNum) != varValue)
        {
            devVal[paramNum] = varValue;
            autoGenDevVarParam (paramNum, varValue);
            varsChanged = true;
        }

        // Block time-varying
        parmvardata = parmVarModel->getParameter(varParamNum++);
        varValue = QString(parmdata.at(12)).toInt();
        fxnValue = QString(parmdata.at(13)).toInt();
        if (blkVal.at(paramNum) != varValue ||
            blkFxn.at(paramNum) != fxnValue)
        {
            devVal[paramNum] = varValue;
            blkFxn[paramNum] = fxnValue;
            autoGenBlkVarParam (paramNum, varValue, fxnValue);
            varsChanged = true;
        }
    }
    // has anything really changed?
    return varsChanged;
}

// model above has been changed
// change background data and update gui table
//
void setupParamVarModel::changeParamVars()
{
    int paramrows = paramData->getParamCount();
    bool varsChanged = false;

    // set correct number of data tables for parameters
    setNumParamVarTables (paramrows);

    // if any change to var data in parameters (col 8-14),
    //    generate paramvarsdata, and
    //    regenerate gui table
    varsChanged = generateParamVarData(paramrows);

    if (varsChanged)
    {
        updateParamVars ();
        emit paramVarsChanged();
    }
}

// main gui table has been changed
// change all background data to match
//
void setupParamVarModel::updateParamVarsData(QModelIndex tl, QModelIndex br, QVector<int> data)
{
    QStringList param;
    QStringList paramvar;
    int first = tl.row();
    int last = br.row();
    shortParameterModel *parmvartable;
    int parm, parmvar;
    bool changed = false;

    for (int i = first; i <= last; i++)
    {
        param = paramVars->getParameter(i);
        parm = paramNum.at(i);
        parmvar = paramvarNum.at(i);

        parmvartable = paramVarData.at(parm);
        paramvar = parmvartable->getParameter(parmvar);

        if (paramvar != param)
        {
            parmvartable->setParameter(parmvar, param);
            changed = true;
        }
    }
    if (changed) {emit paramVarsChanged();}
}

// background data is changed
// update gui table to match
//
void setupParamVarModel::updateParamVars()
{
    int numparams = getNumSetupVals();
    setNumParams(0);
    int row = 0;
    int numrows;
    shortParameterModel *parmVars;
    QStringList varParam;
    QString varParamHdr;

    for (int i = 0; i < numparams; i++)
    {
        if (useParam(i))
        {
            parmVars = paramVarData.at(i);

            if (useBlks(i))
            {
                numrows = parmVars->getParamCount();
                for (int j = 3; j < numrows; j++)
                {
                    varParam = parmVars->getParameter(j);
                    varParamHdr = parmVars->getParamHeader(j);
                    setParamVarData (row, varParam);
                    setParamVarHeader(row, varParamHdr);
                    row++;
                }
            }
            if (useDevs(i))
            {
                numrows = 2;
                for (int j = 1; j < 3; j++)
                {
                    varParam = parmVars->getParameter(j);
                    varParamHdr = parmVars->getParamHeader(j);
                    setParamVarData (row, varParam);
                    setParamVarHeader(row, varParamHdr);
                    row++;
                }
            }
            if (useEnvVar(i))
            {
                varParam = parmVars->getParameter(0);
                varParamHdr = parmVars->getParamHeader(0);
                setParamVarData (row, varParam);
                setParamVarHeader(row, varParamHdr);
                row++;
            }
        }
    }
    paramVars->setParamCount(row);
}

// new environment value
// auto generate a time-varying parameter
//
void setupParamVarModel::autoGenEnvVarParam(int parmnum, int value)
{
    QStringList param = params->getParameter(parmnum);
    QString parmhdr = params->getParamHeader(parmnum);
    shortParameterModel * varparms = paramVarData.at(parmnum);
    QStringList varparam;
    int link = value / 100;
//    int var = value - (link * 100);

    switch (link)
    {
    case 0:
        varparam << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        varparms->setParameter(0, varparam);
        varparms->setParamHeader(0, QString("%1_ENV_none").arg(parmhdr));
        break;
    case 1:
        varparam << "-10" << "10" << "1" << "1" << "0.5" << "6" << "4";
        varparms->setParameter(0, varparam);
        varparms->setParamHeader(0, QString("%1_ENV_add").arg(parmhdr));
        break;
    case 2:
        varparam << "-10" << "10" << "1" << "1" << "0.5" << "6" << "4";
        varparms->setParameter(0, varparam);
        varparms->setParamHeader(0, QString("%1_ENV_mult").arg(parmhdr));
        break;
    case 3:
        varparam << "-0.9" << "0.9" << "0" << "0" << "0.5" << "6" << "4";
        varparms->setParameter(0, varparam);
        varparms->setParamHeader(0, QString("%1_ENV_offset").arg(parmhdr));
        break;
    case 4:
        varparam << "-0.9" << "0.9" << "0" << "0" << "0.5" << "6" << "4";
        varparms->setParameter(0, varparam);
        varparms->setParamHeader(0, QString("%1_ENV_lgst_slope").arg(parmhdr));
        break;
    }
}

// new deviation value
// auto generate a time-varying parameter
//
void setupParamVarModel::autoGenDevVarParam(int parmnum, int value)
{
    QString paramhdr = params->getParamHeader(parmnum);
    shortParameterModel * varparms = paramVarData.at(parmnum);
    QStringList varparam;
    int k = 1;
    switch (value)
    {
    case 1:
        varparam << "0.0001" << "2" << "0.05" << "0.05" << "0.5" << "6" << "-5";
        varparms->setParameter(k, varparam);
        varparms->setParamHeader(k++, QString("%1_dev_se").arg(paramhdr));
        varparam.clear();
        varparam << "-0.99" << "0.99" << "0" << "0" << "0.5" << "6" << "-6";
        varparms->setParameter(k, varparam);
        varparms->setParamHeader(k++, QString("%1_dev_autocorr").arg(paramhdr));
        break;
    case 2:
        varparam << "0.0001" << "2" << "0.05" << "0.05" << "0.5" << "6" << "-5";
        varparms->setParameter(k, varparam);
        varparms->setParamHeader(k++, QString("%1_dev_se").arg(paramhdr));
        varparam.clear();
        varparam << "-0.99" << "0.99" << "0" << "0" << "0.5" << "6" << "-6";
        varparms->setParameter(k, varparam);
        varparms->setParamHeader(k++, QString("%1_dev_autocorr").arg(paramhdr));
        break;
    }
}

// new block or block function value
// auto generate a time-varying parameter
//
void setupParamVarModel::autoGenBlkVarParam(int parmnum, int value, int fxn)
{
    QStringList param = params->getParameter(parmnum);
    QString paramhdr = params->getParamHeader(parmnum);
    shortParameterModel * varparms = paramVarData.at(parmnum);
    QStringList varparam;
    BlockPattern *bp;
    int numBlocks = 0;
    int beg = 0;
    int end = 0;
    int k = 3;

    if (value == 0)
    {
        varparam.clear();
        varparam << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        varparms->setParameter(k, varparam);
        varparms->setParamHeader(k++, paramhdr + QString("_BLK0none"));
    }
    else if (value > 0) // block
    {
        QString p_lo = QString(varparam[0]);
        QString p_hi = QString(varparam[1]);
        QString p_init = QString(varparam[2]);
        float lo = p_lo.toFloat() - p_init.toFloat(); // max negative change
        float hi = p_hi.toFloat() - p_init.toFloat(); // max positive change
        float pr_tp = 0.5 * min(abs(lo), hi); // sd of normal prior
        QString b_lo = QString::number(lo);
        QString b_hi = QString::number(hi);
        QString b_pr_type = QString::number(pr_tp);

        varparam.clear();
        varparam << b_lo << b_hi << "0." << "0." << b_pr_type << "6" << "4";

        bp = parentModel->getBlockPattern(value-1);
        numBlocks = bp->getNumBlocks();
        for (int i = 0; i < numBlocks; i++)
        {
            beg = bp->getBlockBegin(i);
            end = bp->getBlockEnd(i);
            if (fxn == 0) // mult
            {
//                varparam << b_lo << b_hi << "1" << "1" << "0.05" << "6" << "4";
                varparms->setParameter(k, varparam);
                varparms->setParamHeader(k++, paramhdr + QString("_BLK%2mult_%3").arg(QString::number(i+1), QString::number(beg)));
            }
            else if (fxn == 1) // add
            {
//                varparam << "-0.05" << "0.05" << "0" << "0" << "0.025" << "6" << "4";
                varparms->setParameter(k, varparam);
                varparms->setParamHeader(k++, paramhdr + QString("_BLK%2add_%3").arg(QString::number(i+1), QString::number(beg)));
            }
            else if (fxn == 2) // replace
            {
//                varparam << "1" << "45" << "0" << "36" << "10" << "0" << "-3";
                varparms->setParameter(k, varparam);
                varparms->setParamHeader(k++, paramhdr + QString("_BLK%2repl_%3").arg(QString::number(i+1), QString::number(beg)));
            }
            else if (fxn == 3) // delta
            {
//                varparam << "-29.5361" << "20.4639" << "0" << "0" << "10.2319" << "6" << "4";
                varparms->setParameter(k, varparam);
                varparms->setParamHeader(k++, paramhdr + QString("_BLK%2delta_%3").arg(QString::number(i+1), QString::number(beg)));
            }
        }
    }
    else if (value == -1) // trend - offset
    {
        varparam.clear();
        varparam << "-4" << "4" << "0" << "0" << "0.5" << "6" << "40";
        varparms->setParameter(k, varparam);
        varparms->setParamHeader(k++, paramhdr + QString("_TrendFinal_LogstOffset"));
        varparms->setParameter(k, varparam);
        varparms->setParamHeader(k++, paramhdr + QString("_TrendInfl_LogstOffset"));
        varparam.clear();
        varparam << "1" << "20" << "3" << "3" << "3" << "6" << "4" ;
        varparms->setParameter(k, varparam);
        varparms->setParamHeader(k++, paramhdr + QString("_TrendWidth_yrs"));
    }
    else if (value == -2) // trend - direct
    {
        beg = parentModel->get_start_year();
        end = parentModel->get_end_year();
        int infl = (beg + end) / 2;
        varparam.clear();
        varparam << "0.05" << "0.25" << "0.1" << "0.1" << "0.8" << "0" << "-3";
        varparms->setParameter(k, varparam);
        varparms->setParamHeader(k++, paramhdr + QString("_TrendFinal_direct"));
        varparam.clear();
        varparam << QString::number(beg) << QString::number(end) << QString::number(infl);
        varparam << QString::number(infl) << "0.5" << "6" << "4";
        varparms->setParameter(k, varparam);
        varparms->setParamHeader(k++, paramhdr + QString("_TrendInfl_yr"));
        varparam.clear();
        varparam << "1" << "20" << "3" << "3" << "3" << "6" << "4" ;
        varparms->setParameter(k, varparam);
        varparms->setParamHeader(k++, paramhdr + QString("_TrendWidth_yr"));
    }
    else if (value == -3) // trend - fraction
    {
        varparam.clear();
        varparam << "0.0001" << "0.999" << "0.5" << "0.5" << "0.5" << "6" << "4";
        varparms->setParameter(k, varparam);
        varparms->setParamHeader(k++, paramhdr + QString("_TrendFinal_frac"));
        varparms->setParameter(k, varparam);
        varparms->setParamHeader(k++, paramhdr + QString("_TrendInfl_frac"));
        varparam.clear();
        varparam << "1" << "20" << "3" << "3" << "3" << "6" << "4" ;
        varparms->setParameter(k, varparam);
        varparms->setParamHeader(k++, paramhdr + QString("_TrendWidth_yr"));
    }
    else if (value <= -4) // trend - seasonal
    {
        int seas = parentModel->get_num_seasons();
        for (int i = 0; i < seas; i++)
        {
            varparam.clear();
            varparam << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            varparms->setParameter(k, varparam);
            varparms->setParamHeader(k++, paramhdr + QString("_TrendFinal_seas%1").arg(QString::number(i+1)));
        }
    }
    if (k < varparms->getParamCount())
        varparms->setParamCount(k);
}
