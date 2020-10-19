#include "setupmodel.h"
#include "model.h"
#include <cmath>

#include <QMessageBox>

#ifndef min
#define min(a,b) ((a)<(b))?(a):(b)
#endif

setupModel::setupModel(QObject *parent) : QObject(parent)
{
    QStringList vals ("0");
    dataTable = new tablemodel(this);
    dataTable->setRowCount(1);
    dataTable->setRowHeader(0, QString(" "));
    dataTable->setColumnCount(0);
    setData(vals);

    connect (dataTable, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
             SLOT (updateValues()));
}

setupModel::~setupModel()
{
    delete dataTable;
}

void setupModel::setNumValues(int cols)
{
    int num = dataTable->columnCount();
    QStringList setupVals;
    if (cols != num)
    {
        setupVals = dataTable->getRowData(0);
        for (int i = num; i < cols; i++)
        {
            setupVals.append("0");
        }

        while (cols < setupVals.count())
        {
            setupVals.takeLast();
        }
        dataTable->setColumnCount(cols);
        dataTable->setRowData(0, setupVals);
    }
}

void setupModel::setData(QStringList data)
{
    disconnect (dataTable, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
             this, SLOT (updateValues()));
    setNumValues(data.count());
    dataTable->setRowData(0, data);
    updateValues();
    connect (dataTable, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
             SLOT (updateValues()));
}

void setupModel::setHeader(QStringList hdr)
{
    setNumValues(hdr.count());
    dataTable->setHeader(hdr);
}

void setupModel::setValue (int i, int value)
{
    while (valuesList.count() <= i)
        valuesList.append(0);
    if (getValue(i) != value)
    {
        valuesList[i] = value;
        updateTable();
        emit dataChanged(valuesList);
    }
}

int setupModel::getValue (int i)
{
    if (valuesList.count() <= i)
    {
        while(valuesList.count() <= i)
            valuesList.append(0);
        updateTable();
    }
    int value = valuesList.at(i);
    return value;
}

void setupModel::changeValue (int i, int value)
{
    if (getValue(i) != value)
    {
        valuesList[i] = value;
        updateTable();
        emit dataChanged(valuesList);
    }
}

void setupModel::updateTable()
{
    int num = valuesList.count();
    int val = 0;
    QStringList newvals;
    for (int i = 0; i < num; i++)
    {
        val = valuesList.at(i);
        newvals.append(QString::number(val));
    }
    setData(newvals);
}

void setupModel::updateValues()
{
    int num = getNumValues();
    int item = 0;
    int dataItem = 0;
    bool changed = false;
    if (num != valuesList.count())
    {
        while (valuesList.count() < num)
            valuesList.append(0);
        while (valuesList.count() > num)
            valuesList.takeLast();
    }

    QStringList datalist(getData());
    for (int i = 0; i < num; i++)
    {
        item = getValue(i);
        dataItem = QString(datalist.at(i)).toInt();
        if (dataItem != item)
        {
            valuesList[i] = dataItem;
            changed = true;
        }
    }
    if (changed)
    {
        emit dataChanged(valuesList);
    }
}


shortParameterModel::shortParameterModel(QObject *parent) : QObject(parent)
{
    header << "Lo" << "Hi" << "Init" << "Prior" << "P_sd" << "P_type" << "Phase";
    defaultParam << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    paramTable = new tablemodel(this);
    paramData = new tablemodel(this);
    paramTable->setHeader(header);
    paramTable->setRowCount(0);
    paramData->setHeader(header);
    paramData->setRowCount(0);
    paramNum.clear();

    connect (paramTable, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
             SLOT(updateParamData(QModelIndex,QModelIndex,QVector<int>)));

}

shortParameterModel::~shortParameterModel()
{
    delete paramData;
    delete paramTable;
}

void shortParameterModel::setNumParams(int rows)
{
    int currows = getNumParams();
    int i;

    if (currows != rows)
    {
        // set rows in paramTable
        paramTable->setRowCount(rows);
        for (i = currows; i < rows; i++)
        {
            paramTable->setRowData(i, defaultParam);
        }
    }
    // set rows in paramData
    setTotalNumParams(rows);
    // set to use all params - can be changed later
    paramUsed.clear();
    paramNum.clear();
    for (i = 0; i < rows; i++)
    {
        paramUsed.append(1);
        paramNum.append(i);
    }
}

// set the total number of parameters that will be pulled from
// for the gui table
//
void shortParameterModel::setTotalNumParams(int num)
{
    int currows = paramData->rowCount();
    if (currows != num)
    {
        paramData->setRowCount(num);
        for (int i = currows; i < num; i++)
        {
            paramData->setRowData(i, defaultParam);
        }
    }
}

void shortParameterModel::setParamHeader(int row, QString title)
{
    int tblrow = paramNum.indexOf(row);
    if (row >= paramData->rowCount())
        setTotalNumParams(row + 1);
    paramData->setRowHeader(row, title);
//    updateParamHeaders();
    if (tblrow >= 0)
        paramTable->setRowHeader(tblrow, title);
}

void shortParameterModel::setParameter(int row, QStringList &rowstringlist)
{
    if (row >= paramTable->rowCount())
        setNumParams(row);
    int datarow = paramNum.at(row);
    paramTable->setRowData(row, rowstringlist);
    paramData->setRowData(datarow, rowstringlist);
    paramTable->setRowHeader(row, paramData->getRowHeader(datarow));
}

void shortParameterModel::setParamData(int row, QStringList &rowstringlist)
{
    paramData->setRowData(row, rowstringlist);
    updateParams();
}

// Sets the parameter numbers that are used. Data comes from
// setupModel or some other means.
//
void shortParameterModel::setParamsUsed(QList<int> data)
{
    int index, params = data.count();
    bool changed = false;
    Q_UNUSED(index);
    paramNum.clear();
    disconnect (paramTable, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
             this, SLOT(updateParamData(QModelIndex,QModelIndex,QVector<int>)));
    disconnect (paramTable, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
               this, SLOT(updateParams()));
    if (paramUsed.count() != data.count())
    {
        setTotalNumParams(params);
        paramUsed = data;
        changed = true;
    }
    for (int i = 0; i < params; i++)
    {
        if (data.at(i) != 0)
            paramNum.append(i);
        if (data.at(i) != paramUsed.at(i))
        {
            paramUsed[i] = data[i];
            changed = true;
        }
    }
    connect (paramTable, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
             SLOT(updateParamData(QModelIndex,QModelIndex,QVector<int>)));
    connect (paramTable, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
               this, SLOT(updateParams()));
    if (changed)
        updateParams();
}

// Set headers
//
void shortParameterModel::setParamHeaders(QStringList hdr)
{
    int i;//, total;
    int row = 0;
    int numparams = hdr.count();
    QString ColHdr;

//    total = numparams;
    for (i = 0; i < numparams; i++)
    {
        ColHdr = QString(hdr.at(i));
        paramData->setRowHeader(row, ColHdr);
    }
}

// check all parameters for blank values and insert "0",
// change background data, then update gui table
//
void shortParameterModel::checkParamData()
{
    QStringList param;
    int rows = paramData->rowCount();
    int i, j;

    for (i = 0; i < rows; i++)
    {
        param = paramData->getRowData(i);
        for (j = 0; j < param.count(); j++)
        {
            if (QString(param.at(j)).isEmpty())
            {
                param.removeAt(j);
                param.insert(j, QString("0"));
            }
        }
        while (j++ < defaultParam.count())
            param.append(QString("0"));
        paramData->setRowData(i, param);
    }
    updateParams();
}

// Kludge for output routine
// output whole parameter as one string
//
QString shortParameterModel::getParamText (int row)
{
    QString txt;
    QStringList slist (paramTable->getRowData(row));
    for (int i = 0; i < slist.count(); i++)
        txt.append(QString(" %1").arg(slist.at(i)));

    return txt;
}

// the background data has changed
// copy data from there to the gui table
//
void shortParameterModel::updateParams()
{
    QStringList parm;
    QString parmHdr;
    int row = 0;
    int num = 0;
    paramNum.clear();
    paramTable->setRowCount(paramData->rowCount());
    disconnect (paramTable, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
             this, SLOT(updateParamData(QModelIndex,QModelIndex,QVector<int>)));

    for (num = 0; num < paramUsed.count(); num++)
    {
        if (paramUsed.at(num) != 0)
        {
            paramNum.append(num);
            parm = paramData->getRowData(num);
            parmHdr = paramData->getRowHeader(num);
            paramTable->setRowData(row, parm);
            paramTable->setRowHeader(row, parmHdr);
            row++;
        }
    }
    connect (paramTable, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
             SLOT(updateParamData(QModelIndex,QModelIndex,QVector<int>)));
/*    for (row = 0; row < paramNum.count(); row++)
    {
        num = paramNum.at(row);
        parm = paramData->getRowData(num);
        paramTable->setRowData(row, parm);
        paramTable->setRowHeader(row, paramData->getRowHeader(num));
    }*/
    paramTable->setRowCount(row);
}

void shortParameterModel::updateParamHeaders()
{
    QString hdr;
    int num = 0;
    for (int i = 0; i < paramNum.count(); i++)
    {
        num = paramNum.at(i);
        hdr = paramData->getRowHeader(num);
        paramTable->setRowHeader(i, hdr);
    }
}

// the gui table has changed
// change all background data to match
//
void shortParameterModel::updateParamData(QModelIndex tl, QModelIndex br, QVector<int> data)
{
    QStringList parm;
    QStringList parmdat;
    int row = 0;
    int num = 0;
    int first = tl.row();
    int last = br.row();
    bool changed = false;
    Q_UNUSED(data);

    for (row = first; row <= last; row++)
    {
        parm = paramTable->getRowData(row);
        num = paramNum.at(row);
        parmdat = paramData->getRowData(num);
        if (parm != parmdat)
        {
            paramData->setRowData(num, parm);
            emit paramChanged(num, parm);
            changed = true;
        }
    }

    if (changed)
        emit paramsChanged();
}


shortParamMultModel::shortParamMultModel(QObject *parent) : shortParameterModel(parent)
{
    mult = 1;
}

shortParamMultModel::~shortParamMultModel()
{
}

void shortParamMultModel::setNumParams(int rows)
{
    int currows = getNumParams();
    int i;
    int total = rows * mult;
    setTotalNumParams(rows);

    if (currows != total)
    {
        paramNum.clear();
        for (i = 0; i < currows; i++)
            paramNum.append(i);
        while (paramNum.count() > total)
            paramNum.takeLast();
        paramTable->setRowCount(total);
        for (i = currows; i < total; i++)
        {
            paramNum.append(i);
            setParameter(i, defaultParam);
        }
    }
}

// set the total number of parameters that will be pulled from
// for the gui table
//
void shortParamMultModel::setTotalNumParams(int num)
{
    int currows = paramData->rowCount();
    int total = num * mult;
    if (currows != total)
    {
        paramData->setRowCount(total);
        for (int i = currows; i < total; i++)
        {
            setParameter(i, defaultParam);
        }
        while (paramUsed.count() < num)
            paramUsed.append(0);
        while (paramUsed.count() > num)
            paramUsed.takeLast();
    }
}

// set multiplier for number of parameters, typically num of seasons
//
void shortParamMultModel::setMultiplier(int newmult)
{
    int i, j, newtotal;
    int beg, end;
    int row = 0;
    int numparams = paramData->rowCount()/mult;
    QStringList param;
    tablemodel *newParamData;

    if (newmult != mult)
    {
        newParamData = new tablemodel(this);
        newtotal = numparams * newmult;
        newParamData->setRowCount(newtotal);
        if (newmult > mult)
        {
            for (i = 0; i < numparams; i++)
            {
                beg = i * mult; end = beg + mult;
                for (j = beg; j < end; j++, row++)
                {
                    param = paramData->getRowData(j);
                    newParamData->setRowData(row, param);
                }
                end = beg + newmult;
                for (; j < end; j++, row++)
                {
                    newParamData->setRowData(row, defaultParam);
                }
            }
        }
        else // newmult < mult
        {
            for (i = 0; i < numparams; i++)
            {
                beg = i * mult; end = beg + newmult;
                for (j = beg; j < end; j++, row++)
                {
                    param = paramData->getRowData(j);
                    newParamData->setRowData(row, param);
                }
            }
        }
//        newParamData->setRowCount(row);
        delete paramData;
        paramData = newParamData;

        mult = newmult;
        updateParams();
    }
}

void shortParamMultModel::setParamHeader(int row, QString title)
{
    if (row > paramUsed.count())
        setTotalNumParams(row + 1);
    int index = row * mult;
    for (int j = 0; j < mult; j++, index++)
    {
        paramData->setRowHeader(index, QString("%1 %2").arg(
                                      title, QString::number(j+1)));
    }
//    paramData->setRowHeader(row, title);
    updateParams();
}

// Sets the parameter numbers that are used. Data comes from
// setupModel or some other means.
//
void shortParamMultModel::setParamsUsed(QList<int> data)
{
    int index;//, totalparams = data.count() * mult;
    bool changed = false;
    Q_UNUSED(index);
//    paramNum.clear();
//    paramUsed = data;
    if (paramUsed.count() != data.count())
    {
        paramUsed = data;
        changed = true;
        setTotalNumParams(data.count());
    }
    for (int i = 0; i < data.count(); i++)
    {
        if (data.at(i) != paramUsed.at(i))
        {
            paramUsed[i] = data[i];
            changed = true;
/*            if (paramUsed.at(i) == 1)
            for (int j = 0; j < mult; j++)
            {
                index = (i * mult) + j;
                paramNum.append(index);
            }*/
        }
    }
    if (changed)
        updateParams();
}

// Set header without multiplier (must do that separately)
//
void shortParamMultModel::setParamDataHeader(int row, QString title)
{
    if (row > paramData->rowCount())
        setTotalNumParams(row + 1);
    paramData->setRowHeader(row, title);
}


// Set headers with multiplier number
//
void shortParamMultModel::setParamHeaders(QStringList hdr)
{
    int i, j;
    int row = 0;
    int numparams = hdr.count();
    QString ColHdr;

    for (i = 0; i < numparams; i++)
    {
        ColHdr = QString(hdr.at(i));
        for (j = 0; j < mult; j++, row++)
        {
            setParamDataHeader(row, QString("%1 %2").arg(
                                   ColHdr, QString::number(j+1)));
//            paramData->setRowHeader(row, QString("%1 %2").arg(
//                                          ColHdr, QString::number(j+1)));
        }
    }
}



// the background data has changed
// copy the data to the gui table
//
void shortParamMultModel::updateParams()
{
    QStringList parm;
    int row = 0;
    int num = 0;
    int index = 0;
    paramNum.clear();
    for (num = 0; num < paramUsed.count(); num++)
    {
        if (paramUsed.at(num) != 0)
        {
            for (int j = 0; j < mult; j++)
            {
                index = (num * mult) + j;
                paramNum.append(index);
                parm = paramData->getRowData(index);
                paramTable->setRowData(row, parm);
                paramTable->setRowHeader(row, paramData->getRowHeader(index));
                row++;
            }
        }
    }
/*    for (row = 0; row < paramNum.count(); row++)
    {
        num = paramNum.at(row);
        parm = paramData->getRowData(num);
        setParameter(row, parm);
        setParamHeader(row, paramData->getRowHeader(num));
    }*/
    paramTable->setRowCount(row);
//    setNumParams(row);
}

// the gui table has changed
// change all background data to match
//
void shortParamMultModel::updateParamData(QModelIndex tl, QModelIndex br, QVector<int> data)
{
    QStringList parm;
    QStringList parmdat;
    int row = 0;
    int num = 0;
    int first = tl.row();
    int last = br.row();
    bool changed = false;
    Q_UNUSED(data);

    for (row = first; row <= last; row++)
    {
        parm = paramTable->getRowData(row);
        num = paramNum.at(row);
        parmdat = paramData->getRowData(num);
        if (parm != parmdat)
        {
            paramData->setRowData(num, parm);
//            paramData->setRowHeader(num, params->getRowHeader(row));
            emit paramChanged(num, parm);
            changed = true;
        }
    }

    if (changed)
        emit paramsChanged();
}



longParameterModel::longParameterModel(QObject *parent) : shortParameterModel(parent)
{
    header << "Env" << "Use_Dev" << "Dev_min" << "Dev_max" << "Dev_sd" << "Use_Blk" << "B_type";
    defaultParam << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    paramTable->setHeader(header);
    paramData->setHeader(header);

}

longParameterModel::~longParameterModel()
{
}


// the background data has changed
// change the gui table to match
//
void longParameterModel::updateParams()
{
    QStringList parm;
    int row = 0;
    for (row = 0; row < paramNum.count(); row++)//paramData->rowCount(); i++)
    {
        parm = paramData->getRowData(paramNum.at(row));
        paramTable->setRowData(row, parm);
        paramTable->setRowHeader(row, paramData->getRowHeader(paramNum.at(row)));
    }
    paramTable->setRowCount(row);
}

// the gui table has changed
// change all background data to match
//
void longParameterModel::updateParamData(QModelIndex tl, QModelIndex br, QVector<int> data)
{
//    int row = 0;
    QStringList parm;
    QStringList parmdat;
    int first = tl.row();
    int last = br.row();
    int parmIndex = 0;
    bool changed = false;
    Q_UNUSED(data);

    for (int i = first; i <= last; i++)
    {
        parm = paramTable->getRowData(i);
        parmIndex = paramNum.at(i);
        parmdat = paramData->getRowData(parmIndex);
        if (parm != parmdat)
        {
            paramData->setRowData(parmIndex, parm);
            emit paramChanged(parmIndex, parm);
            changed = true;
        }
    }
    if (changed)
        emit paramsChanged();
}


// Create the setup model that includes parameters
// and associated time-varying parameters
//
timeVaryParameterModel::timeVaryParameterModel(QObject *parent) : QObject(parent)
{
    model_data = static_cast<ss_model *>(parent);
    header << "Lo" << "Hi" << "Init" << "Prior" << "P_sd" << "P_type" << "Phase";
    defaultParam << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    varyParamTable = new tablemodel(this);
    varyParamTable->setHeader(header);
    varyParamTable->setRowCount(0);
    autoGenerate = 0;

    setTotalNumVarTables(0);
    setNumVarParams(0);

    connect (varyParamTable, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
             SLOT(updateVarParamData(QModelIndex,QModelIndex,QVector<int>)));
}

// Class desctructor
//
timeVaryParameterModel::~timeVaryParameterModel()
{
    tablemodel *spm;
    // delete all background tables
    while (varyParamDataTables.count() > 0)
    {
        spm = varyParamDataTables.takeLast();
        delete spm;
    }
    // delete the gui table
    delete varyParamTable;
}

// set the gui table to correct number of rows
//
void timeVaryParameterModel::setNumVarParams(int rows)
{
    varyParamTable->setRowCount(rows);
}

void timeVaryParameterModel::setVarParamHeaders(QStringList hdr)
{
    for (int i = 0; i < hdr.count(); i++)
    {
        tableHeaders[i] = hdr[i];
    }
    updateVarParamHdrs();
}

// set the number of background tables to the number of parameters
// also sets arrays of current values to correct length
//
void timeVaryParameterModel::setTotalNumVarTables(int num)
{
    tablemodel *paramVarModel;
    int numtables = varyParamDataTables.count();

    for (int i = numtables; i < num; i++)
    {
        paramVarModel = newVaryParamTable();
        varyParamDataTables.append(paramVarModel);
        tableUsed.append(false);
        blkVal.append(0);
        blkFxn.append(0);
        devVal.append(0);
        envVal.append(0);
    }
    while (num < varyParamDataTables.count())
    {
        paramVarModel = varyParamDataTables.takeLast();
        delete paramVarModel;
        tableUsed.takeLast();
        blkVal.takeLast();
        blkFxn.takeLast();
        devVal.takeLast();
        envVal.takeLast();
    }
}

// Set the tables (parameters) that are used in the gui table
//
void timeVaryParameterModel::setVarParamsUsed(QList<int> data)
{
    int num = data.count();
    bool changed = false;
    if (num != tableUsed.count())
    {
        setTotalNumVarTables(num);
        changed = true;
    }
    else
    {
        for (int i = 0; i < num; i++)
        {
            if ((tableUsed.at(i) && data.at(i) == 0) ||
                (!tableUsed.at(i) && data.at(i) != 0))
            {
                tableUsed[i] = (data[i] != 0);
                changed = true;
            }
        }
    }
    if (changed)
        updateVarParams();
}

void timeVaryParameterModel::setVarParamHeader(int row, QString title)
{
//    int num = tableNum.at(row);
    varyParamTable->setRowHeader(row, title);
}

void timeVaryParameterModel::setVarParameter(int row, QStringList &rowstringlist)
{
//    int num = tableNum.at(row);
    varyParamTable->setRowData(row, rowstringlist);
}

// generate any and all parameter time varying parameters
// for one parameter and compare with the underlying data.
// if any have changed, return true, else return false
//
void timeVaryParameterModel::generateVarParamData(int parmNum, QStringList data)
{
    // get param and paramVar table
    // generate a time-varying parameter and replace existing.

    // Environmental variable time-varying
    if (envVal.at(parmNum) != 0)
    {
        autoGenEnvVarParam (parmNum, envVal.at(parmNum));
    }

    // Deviation time-varying
    if (devVal.at(parmNum) != 0)
    {
        autoGenDevVarParam (parmNum, devVal.at(parmNum));
    }

    // Block time-varying
    if (blkVal.at(parmNum) != 0)
    {
        autoGenBlkVarParam (parmNum, blkVal.at(parmNum), blkFxn.at(parmNum), data);
    }
}

// a parameter in the model above has been changed
// change background data and update gui table
//
void timeVaryParameterModel::changeVarParamData(int parm, QStringList data)
{
    bool varsChanged = false;
    if (data.count() > parm) {
        int item = QString(data.at(0)).toInt();
        bool autogen = (autoGenerate == 0) || (autoGenerate == 2 && item == -12345);

        if (getNumParams() <= parm)
            setNumParams(parm + 1);

        item = QString(data.at(7)).toInt();
        if (envVal[parm] != item)
        {
            envVal[parm] = item;
            if (autogen)
                autoGenEnvVarParam(parm, item);
            varsChanged = true;
        }
        item = QString(data.at(8)).toInt();
        if (devVal[parm] != item)
        {
            devVal[parm] = item;
            if (autogen)
                autoGenDevVarParam(parm, item);
            varsChanged = true;
        }
        item = QString(data.at(12)).toInt();
        if (blkVal[parm] != item)
        {
            blkVal[parm] = item;
            if (autogen)
                autoGenBlkVarParam(parm, item, QString(data.at(13)).toInt(), data);
            varsChanged = true;
        }
        item = QString(data.at(13)).toInt();
        if (blkFxn[parm] != item)
        {
            blkFxn[parm] = item;
            if (autogen)
                autoGenBlkVarParam(parm, QString(data.at(12)).toInt(), item, data);
            varsChanged = true;
        }
        tableUsed[parm] = (envVal[parm] != 0) ||
                (devVal[parm] != 0) ||
                (blkVal[parm] != 0);
    }
    if (varsChanged)
    {
//        item = QString(data.at(0)).toInt();
//        if (autoGenerate == 0 ||
  //         (autoGenerate == 2 && item == -12345))
    //        generateVarParamData(parm, data);
        updateVarParams ();
        emit varParamsChanged();
    }
}

// model above has been changed
// change background data and update gui table
//
/*void timeVaryParameterModel::changeParamVars()
{
    int paramrows = paramData->getParamCount();
    bool varsChanged = false;

    if (paramVarData.count() != paramrows)
        varsChanged = true;

    // set correct number of data tables for parameters
    setNumParamVarTables (paramrows);

    // if any change to var data in parameters (col 8-14),
    //    generate paramvarsdata, and
    //    regenerate gui table
    for (int i = 0; i < paramrows; i++)
        varsChanged = (varsChanged || generateParamVarData(i));//paramrows));

    if (varsChanged)
    {
        updateParamVars ();
        emit paramVarsChanged();
    }
}*/

void timeVaryParameterModel::changeVarParamHeader(int parm, QString title)
{
    if (tableHeaders.count() <= parm)
    {
        while (tableHeaders.count() <= parm)
        {
            tableHeaders.append(title);
        }
    }
    else
    {
        tableHeaders[parm] = title;
    }
}

// main gui table has been changed
// change all background data to match
//
void timeVaryParameterModel::updateVarParamData(QModelIndex tl, QModelIndex br, QVector<int> data)
{
    QStringList param;
    QStringList paramvar;
    int first = tl.row();
    int last = br.row();
    tablemodel *parmvartable;
    int parm, parmvar;
    bool changed = false;
    Q_UNUSED(data);

    for (int i = first; i <= last; i++)
    {
        param = varyParamTable->getRowData(i);
        parm = tableNum.at(i);
        parmvar = tableParNum.at(i);

        parmvartable = varyParamDataTables.at(parm);
        paramvar = parmvartable->getRowData(parmvar);

        if (paramvar != param)
        {
            parmvartable->setRowData(parmvar, param);
            changed = true;
        }
    }
    if (changed) {emit varParamsChanged();}
}

void timeVaryParameterModel::updateVarParamData (int first, int last)
{
    QStringList param;
    QStringList paramvar;
    tablemodel *parmvartable;
    int parm, parmvar;
    bool changed = false;

    for (int i = first; i <= last; i++)
    {
        param = varyParamTable->getRowData(i);
        parm = tableNum.at(i);
        parmvar = tableParNum.at(i);

        parmvartable = varyParamDataTables.at(parm);
        paramvar = parmvartable->getRowData(parmvar);

        if (paramvar != param)
        {
            parmvartable->setRowData(parmvar, param);
            changed = true;
        }
    }
    if (changed) {emit varParamsChanged();}
}

// background data is changed
// update gui table to match
//
void timeVaryParameterModel::updateVarParams()
{
    int numparams = varyParamDataTables.count();
    setNumVarParams(0);
    int row = 0;
    int numrows;
    tablemodel *parmVars;
    QStringList varParam;
    QString varParamHdr;

    tableNum.clear();
    tableParNum.clear();

    for (int i = 0; i < numparams; i++)
    {
        if (tableUsed.at(i))
        {
            parmVars = varyParamDataTables.at(i);

            if (blkVal.at(i) != 0)
            {
                numrows = parmVars->rowCount();
                for (int j = 3; j < numrows; j++)
                {
                    tableNum.insert(row, i);
                    tableParNum.insert(row, j);
                    varParam = parmVars->getRowData(j);
                    varParamHdr = parmVars->getRowHeader(j);
                    setVarParameter (row, varParam);
                    setVarParamHeader(row, varParamHdr);
                    row++;
                }
            }
            if (devVal.at(i) != 0)
            {
                numrows = 2;
                for (int j = 1; j < 3; j++)
                {
                    tableNum.insert(row, i);
                    tableParNum.insert(row, j);
                    varParam = parmVars->getRowData(j);
                    varParamHdr = parmVars->getRowHeader(j);
                    setVarParameter (row, varParam);
                    setVarParamHeader(row, varParamHdr);
                    row++;
                }
            }
            if (envVal.at(i) != 0)
            {
                tableNum.insert(row, i);
                tableParNum.insert(row, 0);
                varParam = parmVars->getRowData(0);
                varParamHdr = parmVars->getRowHeader(0);
                setVarParameter (row, varParam);
                setVarParamHeader(row, varParamHdr);
                row++;
            }
        }
    }
    while (tableNum.count() > row)
    {
        tableNum.takeLast();
        tableParNum.takeLast();
    }
    varyParamTable->setRowCount(row);
}

void timeVaryParameterModel::updateVarParamHdrs()
{
    int numparams = varyParamDataTables.count();
    for (int i = 0; i < numparams; i++)
    {
        updateTableParamHdrs(i);
    }
}

// Check the Environment Link value
int timeVaryParameterModel::checkEnvVarValue(int value)
{
    QString title, text;
    int check = abs(value);
    int numEnvVar = static_cast<ss_model *>(parent())->getNumEnvironVars();
    int fxn = check/100;
    int var = check - (fxn * 100);

    if (value != 0)
    {
    if ((var < 0 || var > numEnvVar) ||
        (fxn < 1 || fxn > 4))
    {
        title = QString("Incorrect Environmental Linkage Value");
        if (fxn < 1)
            fxn = 1;
        if (fxn > 4)
            fxn = 4;
        if (numEnvVar == 0)
        {
            text = QString("There are no Environmental Variables defined.");
            check = 0;
        }
        else
        {
            text = QString( QString("Value is of the form xyy where x is 1 to 4 and yy is 01 to 0%1.").arg(QString::number(numEnvVar)));
            if (var == 0)
                check = 0;
            else if (var < numEnvVar)
                check = (fxn * 100) + var;
            else
                check = (fxn * 100) + numEnvVar;
        }
        QMessageBox::information(0, title, text);
    }
    }
    return check;
}

// Check the Deviation Link value
int timeVaryParameterModel::checkDevValue(int value)
{
    QString title, text;
    int check = value;
    if (value != 0)
    {
    if (value < 1 || value > 2)
    {
        if (value < 1)
            check = 0;
        else if (value > 2)
            check = 2;
        title = QString("Incorrect Deviation Link");
        text = QString("Value may be 0 - none, 1 - multiplicative, or 2 - additive.");
        QMessageBox::information(0, title, text);
    }
    }
    return check;
}

// Check the Block Pattern number
int timeVaryParameterModel::checkBlockNum(int value)
{
    QString title, text;
    int numBlks = static_cast<ss_model *>(parent())->getNumBlockPatterns();
    int check = value;
    if (value != 0)
    {
    if (value > numBlks)
    {
        title = QString("Incorrect Block Pattern/Trend Number");
        if (numBlks == 0)
        {
            text = QString("There are no Block Patterns defined.");
            check = 0;
        }
        else
        {
            text = QString( QString("Value may be 0 - none or 1 to %1.").arg(QString::number(numBlks)));
            check = numBlks;
        }
        QMessageBox::information(0, title, text);
    }
    }
    return check;
}

// Check the Block Function value
int timeVaryParameterModel::checkBlockFxn(int value)
{
    QString title, text;
    int check = value;
    if (value < 0 || value > 3)
    {
        title = QString("Incorrect Block Function Number");
        text = QString("Value may be 0 - multiplicative, 1 - additive, 2 - replacement, or 3 - random walk.");
        if (value < 0)
            check = 0;
        else if (value > 3)
            check = 3;
        QMessageBox::information(0, title, text);
    }
    return check;
}

// new environment value
// auto generate a time-varying parameter
//
void timeVaryParameterModel::autoGenEnvVarParam(int parmnum, int value)
{
    tablemodel * varparms = varyParamDataTables.at(parmnum);
    QString parmhdr = varparms->getTitle();
    QStringList varparam;
    int link = value / 100;
//    int var = value - (link * 100);

    switch (link)
    {
    case 0:
        varparam << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        varparms->setRowData(0, varparam);
        varparms->setRowHeader(0, QString("%1_ENV_none").arg(parmhdr));
        break;
    case 1:
        varparam << "-10" << "10" << "1" << "1" << "0.5" << "6" << "4";
        varparms->setRowData(0, varparam);
        varparms->setRowHeader(0, QString("%1_ENV_add").arg(parmhdr));
        break;
    case 2:
        varparam << "-10" << "10" << "1" << "1" << "0.5" << "6" << "4";
        varparms->setRowData(0, varparam);
        varparms->setRowHeader(0, QString("%1_ENV_mult").arg(parmhdr));
        break;
    case 3:
        varparam << "-0.9" << "0.9" << "0" << "0" << "0.5" << "6" << "4";
        varparms->setRowData(0, varparam);
        varparms->setRowHeader(0, QString("%1_ENV_offset").arg(parmhdr));
        break;
    case 4:
        varparam << "-0.9" << "0.9" << "0" << "0" << "0.5" << "6" << "4";
        varparms->setRowData(0, varparam);
        varparms->setRowHeader(0, QString("%1_ENV_lgst_slope").arg(parmhdr));
        break;
    }
}

// new deviation value
// auto generate a time-varying parameter
//
void timeVaryParameterModel::autoGenDevVarParam(int parmnum, int value)
{
    tablemodel * varparms = varyParamDataTables.at(parmnum);
    QString paramhdr = varparms->getTitle();
    QStringList varparam;
    int k = 1;
    switch (value)
    {
    case 1:
        varparam << "0.0001" << "2" << "0.05" << "0.05" << "0.5" << "6" << "-5";
        varparms->setRowData(k, varparam);
        varparms->setRowHeader(k++, QString("%1_dev_se").arg(paramhdr));
        varparam.clear();
        varparam << "-0.99" << "0.99" << "0" << "0" << "0.5" << "6" << "-6";
        varparms->setRowData(k, varparam);
        varparms->setRowHeader(k++, QString("%1_dev_autocorr").arg(paramhdr));
        break;
    case 2:
        varparam << "0.0001" << "2" << "0.05" << "0.05" << "0.5" << "6" << "-5";
        varparms->setRowData(k, varparam);
        varparms->setRowHeader(k++, QString("%1_dev_se").arg(paramhdr));
        varparam.clear();
        varparam << "-0.99" << "0.99" << "0" << "0" << "0.5" << "6" << "-6";
        varparms->setRowData(k, varparam);
        varparms->setRowHeader(k++, QString("%1_dev_autocorr").arg(paramhdr));
        break;
    }
}

// new block or block function value
// auto generate a time-varying parameter
//
void timeVaryParameterModel::autoGenBlkVarParam(int parmnum, int value, int fxn, QStringList parmdata)
{
    tablemodel * varparms = varyParamDataTables.at(parmnum);
    QString paramhdr = varparms->getTitle();
    QStringList varparam;
    BlockPattern *bp;
    int numBlocks = 0;
    int beg = 0;
    int end = 0;
    int k = 3;
    varparam << "0" << "0" << "0" << "0" << "0" << "6" << "4";

    if (value == 0)
    {
        varparms->setRowData(k, varparam);
        varparms->setRowHeader(k++, paramhdr + QString("_BLK0none"));
    }
    else if (value > 0) // blocks
    {
        float p_lo = QString(parmdata[0]).toFloat();
        float p_hi = QString(parmdata[1]).toFloat();
        float p_init = QString(parmdata[2]).toFloat();
        float lo;    // max negative change
        float hi;    // max positive change
        float pr_tp; // sd of normal prior

        if (fxn == 0) // mult
        {
            lo = labs(p_lo / p_init);
            if (p_lo < 0)
                lo = 1.0 / lo;
            if (lo > 0.0) lo = log (lo);     // max negative change
            hi = labs(p_hi / p_init);
            if (hi > 0.0) hi = log (hi);     // max positive change
//            lo = log(p_lo/p_init);
//            hi = log(p_hi/p_init);
            pr_tp = min(labs(lo), hi);
            pr_tp = 0.5 * pr_tp;  // sd of normal prior
            varparam[0] = QString::number(lo);
            varparam[1] = QString::number(hi);
            varparam[4] = QString::number(pr_tp);
        }
        else if (fxn == 2) // repl
        {
            for (int j = 0; j < 7; j++)
                varparam[j] = parmdata.at(j);
        }
        else // fxn == 1 (add) or fxn == 3 (delta)
        {
            lo = p_lo - p_init;              // max negative change
            hi = p_hi - p_init;              // max positive change
            pr_tp = min(labs(lo), hi);
            pr_tp = 0.5 * pr_tp;  // sd of normal prior
            varparam[0] = QString::number(lo);
            varparam[1] = QString::number(hi);
            varparam[4] = QString::number(pr_tp);
        }

        bp = model_data->getBlockPattern(value-1);
        numBlocks = bp->getNumBlocks();
        for (int i = 0; i < numBlocks; i++)
        {
            beg = bp->getBlockBegin(i);
            if (fxn == 0) // mult
            {
                varparms->setRowData(k, varparam);
                varparms->setRowHeader(k++, paramhdr + QString("_BLK%2mult_%3").arg(QString::number(i+1), QString::number(beg)));
            }
            else if (fxn == 1) // add
            {
                varparms->setRowData(k, varparam);
                varparms->setRowHeader(k++, paramhdr + QString("_BLK%2add_%3").arg(QString::number(i+1), QString::number(beg)));
            }
            else if (fxn == 2) // replace
            {
                varparms->setRowData(k, varparam);
                varparms->setRowHeader(k++, paramhdr + QString("_BLK%2repl_%3").arg(QString::number(i+1), QString::number(beg)));
            }
            else if (fxn == 3) // delta
            {
                varparms->setRowData(k, varparam);
                varparms->setRowHeader(k++, paramhdr + QString("_BLK%2delta_%3").arg(QString::number(i+1), QString::number(beg)));
            }
        }
    }
    else if (value == -1) // trend - offset
    {
        varparam.clear();
        varparam << "-4" << "4" << "0" << "0" << "0.5" << "6" << "40";
        varparms->setRowData(k, varparam);
        varparms->setRowHeader(k++, paramhdr + QString("_TrendFinal_LogstOffset"));
        varparms->setRowData(k, varparam);
        varparms->setRowHeader(k++, paramhdr + QString("_TrendInfl_LogstOffset"));
        varparam.clear();
        varparam << "1" << "20" << "3" << "3" << "3" << "6" << "4" ;
        varparms->setRowData(k, varparam);
        varparms->setRowHeader(k++, paramhdr + QString("_TrendWidth_yrs"));
    }
    else if (value == -2) // trend - direct
    {
        beg = model_data->get_start_year();
        end = model_data->get_end_year();
        int infl = (beg + end) / 2;
        varparam.clear();
        varparam << "0.05" << "0.25" << "0.1" << "0.1" << "0.8" << "0" << "-3";
        varparms->setRowData(k, varparam);
        varparms->setRowHeader(k++, paramhdr + QString("_TrendFinal_direct"));
        varparam.clear();
        varparam << QString::number(beg) << QString::number(end) << QString::number(infl);
        varparam << QString::number(infl) << "0.5" << "6" << "4";
        varparms->setRowData(k, varparam);
        varparms->setRowHeader(k++, paramhdr + QString("_TrendInfl_yr"));
        varparam.clear();
        varparam << "1" << "20" << "3" << "3" << "3" << "6" << "4" ;
        varparms->setRowData(k, varparam);
        varparms->setRowHeader(k++, paramhdr + QString("_TrendWidth_yr"));
    }
    else if (value == -3) // trend - fraction
    {
        varparam.clear();
        varparam << "0.0001" << "0.999" << "0.5" << "0.5" << "0.5" << "6" << "4";
        varparms->setRowData(k, varparam);
        varparms->setRowHeader(k++, paramhdr + QString("_TrendFinal_frac"));
        varparms->setRowData(k, varparam);
        varparms->setRowHeader(k++, paramhdr + QString("_TrendInfl_frac"));
        varparam.clear();
        varparam << "1" << "20" << "3" << "3" << "3" << "6" << "4" ;
        varparms->setRowData(k, varparam);
        varparms->setRowHeader(k++, paramhdr + QString("_TrendWidth_yr"));
    }
    else if (value <= -4) // trend - seasonal
    {
        int seas = model_data->get_num_seasons();
        for (int i = 0; i < seas; i++)
        {
            varparam.clear();
            varparam << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            varparms->setRowData(k, defaultParam);
            varparms->setRowHeader(k++, paramhdr + QString("_TrendFinal_seas%1").arg(QString::number(i+1)));
        }
    }
    if (k < varparms->rowCount())
        varparms->setRowCount(k);
}
/*
QStringList *timeVaryParameterModel::autoGenTndSeasVarParam(int parmnum, int blk, QStringList parmdate, int varparmnum)
{
    int seas = model_data->get_num_seasons();
    QStringList *varparm = new QStringList;
    varparm << "0";

}*/

tablemodel *timeVaryParameterModel::newVaryParamTable()
{
    tablemodel *newtable = new tablemodel(this);
    newtable->setHeader(header);
    newtable->setRowCount(3);
    for (int i = 0; i < 3; i++)
        newtable->setRowData(i, defaultParam);
    return newtable;
}

void timeVaryParameterModel::setTableTitle(int tbl, QString name)
{
    tablemodel *table = varyParamDataTables.at(tbl);
    table->setTitle(name);
    updateTableParamHdrs(tbl);
}

void timeVaryParameterModel::updateTableParamHdrs(int tbl)
{
    tablemodel *parmVars = varyParamDataTables.at(tbl);
    int numrows = parmVars->rowCount();
    QString title (parmVars->getTitle());
    QString env, dev, blk;

    env = QString (parmVars->getRowHeader(0));
    if (env.contains("ENV"))
        env = QString (parmVars->getRowHeader(0).split("ENV").at(1));
    parmVars->setRowHeader(0, QString("%1_ENV%2").arg(title, env));

    dev = QString (parmVars->getRowHeader(1));
    if (dev.contains("dev"))
        dev = QString (parmVars->getRowHeader(1).split("dev").at(1));
    parmVars->setRowHeader(1, QString("%1_dev%2").arg(title, dev));
    dev = QString (parmVars->getRowHeader(2));
    if (dev.contains("dev"))
        dev = QString (parmVars->getRowHeader(2).split("dev").at(1));
    parmVars->setRowHeader(2, QString("%1_dev%2").arg(title, dev));


    for (int j = 3; j < numrows; j++)
    {
        blk = QString (parmVars->getRowHeader(j));
        if (blk.contains("BLK"))
        {
            blk = QString (parmVars->getRowHeader(j).split("BLK").at(1));
            parmVars->setRowHeader(j, QString("%1_BLK%2").arg(title, blk));
        }
        else if (blk.contains("Trend"))
        {
            blk = QString (parmVars->getRowHeader(j).split("Trend").at(1));
            parmVars->setRowHeader(j, QString("%1_Trend%2").arg(title, blk));
        }
        else
        {
            parmVars->setRowHeader(j, QString("%1_BLK").arg(title));
        }
    }
    updateVarParams();
}

int timeVaryParameterModel::getAutoGenerate() const
{
    return autoGenerate;
}

void timeVaryParameterModel::setAutoGenerate(int value)
{
    if (value != autoGenerate) {
        autoGenerate = value;
        updateVarParams();
    }
}
