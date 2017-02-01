#include "parametermodelTV.h"
#include "model.h"

#include <QString>

#define min(a,b) ((a)<(b))?(a):(b)

parameterModelTV::parameterModelTV(ss_model *parent)
    : longParameterModel(parent)
{
    parentModel = parent;
    connect (&params, SIGNAL(rowsInserted(QModelIndex,int,int)),
             SLOT(insertData(QModelIndex,int,int)));
    connect (&params, SIGNAL(rowsRemoved(QModelIndex,int,int)),
             SLOT(removeData(QModelIndex,int,int)));
    connect (&params, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
             SLOT(changeData(QModelIndex,QModelIndex,QVector<int>)));
//    connect (this, SIGNAL(tableChanged()), SLOT(changeData()));

//    timeVaryParams = new shortParameterModel(this);
    connect (timeVaryParams.getParameters(), SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
             SLOT(changeTimeVaryData(QModelIndex,QModelIndex,QVector<int>)));
//    blkst = 3;
}

parameterModelTV::~parameterModelTV()
{
    setNumParams(0);
    params.clear();
    paramData.clear();
    timeVaryParams.getParameters()->clear();
    timeVaryParamData.clear();
    header.clear();
}

void parameterModelTV::setParentModel(ss_model *pModel)
{
    parentModel = pModel;
}

void parameterModelTV::setNumParams(int rows)
{
    int numparams = getNumParams();
    shortParameterModel *tvParamData;
    QStringList ql;
    for (int i = 0; i < 14; i++)
        ql.append(QString("0"));
//    ql << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    params.setRowCount(rows);
    for (int i = numparams; i < rows; i++)
    {
        setParameter(i, ql);
        useEnv.append(0);
        useDev.append(0);
        useBlk.append(0);
    }
    ql.clear();
    ql << "0" << "0" << "0" << "0" << "0" << "0" << "0" ;

    for (int i = numparams; i < rows; i++)
    {
        tvParamData = new shortParameterModel(this);
        tvParamData->setParamCount(4);
        timeVaryParamData.append(tvParamData);
    }
    while (timeVaryParamData.count() > rows)
    {
        tvParamData = timeVaryParamData.takeLast();
        delete tvParamData;
    }
    setParamCount(rows);
}

void parameterModelTV::setParameter (int index, QStringList data)
{
    if (index >= getNumParams())
        setNumParams(index + 1);

    params.setRowData(index, data);
}

void parameterModelTV::setParamHeader(int row, QString title)
{
    params.setRowHeader(row, title);
    setTimeVaryHeaders (row, title);
}

bool parameterModelTV::useBlock(int index)
{
    return (useBlk.at(index) != 0);
}

bool parameterModelTV::useDevs(int index)
{
    return (useDev.at(index) != 0);
}

bool parameterModelTV::envLink(int index)
{
    return (useEnv.at(index) != 0);
}

void parameterModelTV::updateTimeVaryData()
{
    QString label;
    QStringList ql;
    timeVaryParams.setParamCount(0);
    int k = 0;
    int i = 0;
    int tvTotal = getNumParams() * 4;
    int var;

    while (tableNum.count() <= tvTotal)
    {
        tableNum.append(-1);
        paramNum.append(-1);
        useBlk.append(0);
        useDev.append(0);
        useEnv.append(0);
    }

    for (i = 0; i < getNumParams(); i++)
    {
        ql = getParameter(i);
        label = getParamHeader(i);
        var = QString(ql.at(12)).toInt();
        if (var != 0)
        {
            useBlk[i] = var;
            for (int j = 3; j < timeVaryParamData.at(i)->getParamCount(); j++)
            {
                tableNum[k] = i;
                paramNum[k] = j;
                ql = timeVaryParamData.at(i)->getParameter(j);
                timeVaryParams.setParameter(k, ql);
                timeVaryParams.setParamHeader(k++, timeVaryParamData.at(i)->getParamHeader(j));
            }
        }
        var = QString(ql.at(8)).toInt();
        if (var != 0)
        {
            useDev[i] = var;
            tableNum[k] = i;
            paramNum[k] = 1;
            ql = timeVaryParamData.at(i)->getParameter(1);
            timeVaryParams.setParameter(k, ql);
            timeVaryParams.setParamHeader(k++, timeVaryParamData.at(i)->getParamHeader(1));
            tableNum[k] = i;
            paramNum[k] = 2;
            ql = timeVaryParamData.at(i)->getParameter(2);
            timeVaryParams.setParameter(k, ql);
            timeVaryParams.setParamHeader(k++, timeVaryParamData.at(i)->getParamHeader(2));
        }
        var = QString(ql.at(7)).toInt();
        if (var != 0)
        {
            useEnv[i] = var;
            tableNum[k] = i;
            paramNum[k] = 0;
            ql = timeVaryParamData.at(i)->getParameter(0);
            timeVaryParams.setParameter(k, ql);
            timeVaryParams.setParamHeader(k++, timeVaryParamData.at(i)->getParamHeader(0));
        }
    }
    while (tableNum.count() > tvTotal)
//    for (; i < tableNum.count(); i++)
    {
        tableNum.takeLast();//[i] = -1;
        paramNum.takeLast();//[i] = -1;
        useBlk.takeLast();//[i] = 0;
        useDev.takeLast();//[i] = 0;
        useEnv.takeLast();//[i] = 0;
    }
}

void parameterModelTV::modelChanged()
{
    changeData();
}

void parameterModelTV::changeData()
{
    int rows = getNumParams();
    changeData(0, 0, rows - 1, 13);
}

void parameterModelTV::changeData(QModelIndex tplt, QModelIndex btrt, QVector<int> ivect)
{
    int startRow = tplt.row();
    int startCol = tplt.column();
    int endRow = btrt.row();
    int endCol = btrt.column();
    changeData (startRow, startCol, endRow, endCol);
}

void parameterModelTV::changeData(int firstRow, int firstCol, int lastRow, int lastCol)
{
    QString schk;
    int chk, chk2, numPatterns;
    QStringList ql;
    bool changed = false;
    for (int i = firstRow; i <= lastRow; i++)
    {
        ql = getParameter(i);
        for (int j = firstCol; j <= lastCol; j++)
            paramData[i][j] = QString(ql.at(j));

        if (lastCol > 6)
        {
        chk = QString(ql.at(7)).toInt();
        if (chk != useEnv.at(i))
        {
            useEnv[i] = chk;
            changed = true;
            if (chk != 0)
                autoGenEnvParam(i, chk, getParamHeader(i));
        }

        chk = QString(ql.at(8)).toInt();
        if (useDev.at(i) != chk)
        {
            useDev[i] = chk;
            changed = true;
            if (chk != 0)
                autoGenDevParam(i, chk, getParamHeader(i));
        }

        chk = QString(ql.at(12)).toInt();
        chk2 = QString(ql.at(13)).toInt();
        numPatterns = parentModel->getNumBlockPatterns();
        if (chk > numPatterns)
        {
            chk = numPatterns;
            ql[12] = QString::number(chk);
            setParameter(i, ql);
        }
        if (useBlk.at(i) != chk)
        {
            useBlk[i] = chk;
            changed = true;
            if (chk != 0)
                autoGenBlkParam (i, chk, getParamHeader(i));
        }
/*            chk2 = QString(ql.at(13)).toInt();
        schk = timeVarianceList.at(i)->getRowData(blkst).at(13);
        if (chk2 != schk.toInt())
        {
            autoGenBlkParam(i, chk, getRowHeader(i));
        }*/
        }
    }
    if (changed)
    {
        updateTimeVaryData();
    }
}

void parameterModelTV::updateParamData()
{
    int numrows = getNumParams();
    QStringList rowdata;
    for (int i = 0; i < numrows; i++)
    {
        rowdata = getParameter(i);
        paramData[i] = rowdata;
    }
}

void parameterModelTV::insertData(QModelIndex index, int first, int last)
{
    shortParameterModel *sm;
    QStringList ql;
    for (int i = first; i <= last; i++)
    {
        sm = new shortParameterModel(this);
        sm->setParamCount(4);
        timeVaryParamData.insert(i, sm);
        ql = getParameter(i);
        useBlk.insert(i, (QString(ql.at(12)).toInt()));
//        blkFnxs.insert(i, (QString(ql.at(13)).toInt()));
        autoGenBlkParam (i, useBlk.at(i), getParamHeader(i));
        useDev.insert(i, (QString(ql.at(8)).toInt()));
        autoGenDevParam (i, useDev.at(i), getParamHeader(i));
        useEnv.insert(i, (QString(ql.at(7)).toInt()));
        autoGenEnvParam (i, useEnv.at(i), getParamHeader(i));
    }
//    updateTimeVaryData();
    emit paramsChanged();
}

void parameterModelTV::removeData(QModelIndex index, int first, int last)
{
    shortParameterModel *sm;
    for (int i = last; i >= first; i--)
    {
        useEnv.removeAt(i);
        useDev.removeAt(i);
        useBlk.removeAt(i);
 //       blkFnxs.removeAt(i);
        sm = timeVaryParamData.takeAt(i);
        delete sm;
    }
    updateTimeVaryData();
    emit paramsChanged();
}

void parameterModelTV::updateTimeVaryModel()
{
    changeTimeVaryData();
}

void parameterModelTV::changeTimeVaryData()
{
    int rows = timeVaryParams.getParamCount();
//    QModelIndex tplt(index(0, 0));
//    QModelIndex btrt(index(rows - 1, 6));
    changeTimeVaryData(0, rows - 1);
}

void parameterModelTV::changeTimeVaryData(QModelIndex tplt, QModelIndex btrt, QVector<int> ivect)
{
    int start = tplt.row();
    int end = btrt.row();
    changeTimeVaryData(start, end);
}

void parameterModelTV::changeTimeVaryData(int firstRow, int lastRow)
{
    int param, tvParam;
    QStringList ql;
    shortParameterModel *sm;

    for (int i = firstRow; i <= lastRow; i++)
    {
        param = tableNum.at(i);
        tvParam = paramNum.at(i);
        sm = timeVaryParamData.at(param);
        ql = timeVaryParams.getParameter(i);
        sm->setParameter(tvParam, ql);
    }
}

void parameterModelTV::setTimeVaryParam(int index, QStringList data)
{
    timeVaryParams.setParameter(index, data);
}

void parameterModelTV::setTimeVaryBlkParam (int param, int num, QStringList data)
{
    shortParameterModel *sm = timeVaryParamData.at(param);
    sm->setParameter(num + 3, data);
}

void parameterModelTV::setTimeVaryDevParam (int param, int num, QStringList data)
{
    shortParameterModel *sm = timeVaryParamData.at(param);
    sm->setParameter(num + 1, data);
}

void parameterModelTV::setTimeVaryEnvParam (int param, QStringList data)
{
    shortParameterModel *sm = timeVaryParamData.at(param);
    sm->setParameter(0, data);
}

int  parameterModelTV::getNumTimeVaryParams ()
{
    return timeVaryParams.getParamCount();
}

QStringList parameterModelTV::getTimeVaryParam (int row)
{
    return timeVaryParams.getParameter(row);
}

QString parameterModelTV::getTimeVaryParamHdr (int row)
{
    return timeVaryParams.getParamHeader(row);
}

void parameterModelTV::setTimeVaryHeaders(int param, QString label)
{
    if (param == -1)
    {
        for (param = 0; param < getNumParams(); param++)
        {
            setEnvTimeVaryHeader (param);
            setDevTimeVaryHeader (param);
            setBlkTimeVaryHeader (param);
        }
    }
    else
    {
        setEnvTimeVaryHeader(param);
        setDevTimeVaryHeader(param);
        setBlkTimeVaryHeader(param);
    }
}

void parameterModelTV::setEnvTimeVaryHeader(int param, int link, QString label)
{
    QStringList ql;// (getRowData(param));
//    QString hdr (getRowHeader(param));
    QString tvHdr;
    shortParameterModel *sm = timeVaryParamData.at(param);
    int flag;// = QString(ql.at(7)).toInt();

    if (link == 0)
    {
        ql = getParameter(param);
        flag = QString(ql.at(7)).toInt();
        link = flag / 100;
    }
    if (label.isEmpty())
        label = getParamHeader(param);

    tvHdr = QString("%1_ENV_").arg(label);//
    switch (link)
    {
    case 1:
        tvHdr.append("add");
        break;
    case 2:
        tvHdr.append("mult");
        break;
    case 3:
        tvHdr.append("offset");
        break;
    case 4:
        tvHdr.append("lgst_slope");
        break;
    }
    sm->setParamHeader(0, tvHdr);
}

void parameterModelTV::setDevTimeVaryHeader(int param, int flag, QString label)
{
    QStringList ql (getParameter(param));
//    QString hdr (getRowHeader(param));
    QString tvHdr;
    shortParameterModel *sm = timeVaryParamData.at(param);

    if (label.isEmpty())
        label = getParamHeader(param);

    tvHdr = QString("%1_dev_se").arg(label);
    sm->setParamHeader(1, tvHdr);
    tvHdr = QString("%1_dev_autocorr").arg(label);
    sm->setParamHeader(2, tvHdr);
}

void parameterModelTV::setBlkTimeVaryHeader(int param, int block, QString label)
{
    QStringList ql (getParameter(param));
    QString hdr;
    shortParameterModel *sm = timeVaryParamData.at(param);
    int func = QString(ql.at(13)).toInt();
    BlockPattern *bp;
    int k = 3;//blkst;
    int numBlocks, beg, end;

    if (label.isEmpty())
        label = getParamHeader(param);

    if (block > 0) // block
    {
        bp = parentModel->getBlockPattern(block-1);
        numBlocks = bp->getNumBlocks();
        for (int i = 0; i < numBlocks; i++)
        {
            hdr = QString("%1_BLK%2").arg(label, QString::number(i));
            beg = bp->getBlockBegin(i);
            end = bp->getBlockEnd(i);
            if (func == 0)
            {
                hdr.append(QString("mult_%1").arg(QString::number(beg)));
            }
            else if (func == 1)
            {
                hdr.append(QString("add_%1").arg(QString::number(beg)));
            }
            else if (func == 2)
            {
                hdr.append(QString("repl_%1").arg(QString::number(beg)));
            }
            else if (func == 3)
            {
                hdr.append(QString("delta_%1").arg(QString::number(beg)));
            }
            sm->setParamHeader(k++, hdr);
        }
    }
    else if (block == -1) // trends
    {
        sm->setParamHeader(k++, label + QString("_TrendFinal_LogstOffset"));
        sm->setParamHeader(k++, label + QString("_TrendInfl_LogstOffset"));
        sm->setParamHeader(k++, label + QString("_TrendWidth_yrs"));
    }
    else if (block == -2)
    {
        beg = parentModel->get_start_year();
        end = parentModel->get_end_year();
        int infl = (beg + end) / 2;
        sm->setParamHeader(k++, label + QString("_TrendFinal_direct"));
        sm->setParamHeader(k++, label + QString("_TrendInfl_yr"));
        sm->setParamHeader(k++, label + QString("_TrendWidth_yr"));
    }
    else if (block == -3)
    {
        sm->setParamHeader(k++, label + QString("_TrendFinal_frac"));
        sm->setParamHeader(k++, label + QString("_TrendInfl_frac"));
        sm->setParamHeader(k++, label + QString("_TrendWidth_yr"));
    }
    else if (block <= -4)
    {
        int seas = parentModel->get_num_seasons();
        for (int i = 0; i < seas; i++)
        {
            sm->setParamHeader(k++, label + QString("_TrendFinal_seas%1").arg(QString::number(i)));
        }
    }
}

QStringList parameterModelTV::autoGenEnvParam(int param, int envVal, QString label)
{
    QStringList ql;
    int link = envVal / 100;
    int var = envVal - (link * 100);

    switch (link)
    {
    case 1:
    case 2:
        ql << "-10" << "10" << "1" << "1" << "0.5" << "6" << "4";
        timeVaryParamData.at(param)->setParameter(0, ql);
        break;
    case 3:
    case 4:
        ql << "-0.9" << "0.9" << "0" << "0" << "0.5" << "6" << "4";
        timeVaryParamData.at(param)->setParameter(0, ql);
        break;
    }
    setEnvTimeVaryHeader(param, link);

    return ql;//timeVaryParamData.at(param)->getRowData(0);
}

QStringList parameterModelTV::autoGenDevParam(int param, int devVal, QString label)
{
    QStringList ql;
    int k = 1;
    switch (devVal)
    {
    case 1:
        ql << "0.0001" << "2" << "0.05" << "0.05" << "0.5" << "6" << "-5";
        timeVaryParamData.at(param)->setParameter(k++, ql);
        ql.clear();
        ql << "-0.99" << "0.99" << "0" << "0" << "0.5" << "6" << "-6";
        timeVaryParamData.at(param)->setParameter(k++, ql);
        break;
    case 2:
        ql << "0.0001" << "2" << "0.05" << "0.05" << "0.5" << "6" << "-5";
        timeVaryParamData.at(param)->setParameter(k++, ql);
        ql.clear();
        ql << "-0.99" << "0.99" << "0" << "0" << "0.5" << "6" << "-6";
        timeVaryParamData.at(param)->setParameter(k++, ql);
        break;
    }
//    blkst = k;

    setDevTimeVaryHeader(param);

    return ql;//timeVaryParamData.at(param)->getRowData(1);
}

QStringList parameterModelTV::autoGenBlkParam(int param, int blkPat, QString label)
{
    QStringList ql = getParameter(param);
    BlockPattern *bp;
    int numBlocks = 0;
    int beg = 0;
    int end = 0;
    int k = 3;
    int func = QString(getParameter(param).at(13)).toInt();

    if (blkPat == 0)
    {
        ql.clear();
        ql << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        timeVaryParamData.at(param)->setParameter(k, ql);
        timeVaryParamData.at(param)->setParamHeader(k++, label + QString("_BLK0none"));
    }
    else if (blkPat > 0) // block
    {
        QString p_lo = QString(ql[0]);
        QString p_hi = QString(ql[1]);
        QString p_init = QString(ql[2]);
        float lo = p_lo.toFloat() - p_init.toFloat(); // max negative change
        float hi = p_hi.toFloat() - p_init.toFloat(); // max positive change
        float pr_tp = 0.5 * min(abs(lo), hi); // sd of normal prior
        QString b_lo = QString::number(lo);
        QString b_hi = QString::number(hi);
        QString b_pr_type = QString::number(pr_tp);

        ql.clear();
        ql << b_lo << b_hi << "0." << "0." << b_pr_type << "6" << "4";

        bp = parentModel->getBlockPattern(blkPat-1);
        numBlocks = bp->getNumBlocks();
        for (int i = 0; i < numBlocks; i++)
        {
            beg = bp->getBlockBegin(i);
            end = bp->getBlockEnd(i);
            if (func == 0)
            {
//                ql << b_lo << b_hi << "1" << "1" << "0.05" << "6" << "4";
                timeVaryParamData.at(param)->setParameter(k, ql);
                timeVaryParamData.at(param)->setParamHeader(k++, label + QString("_BLK1mult_") + QString::number(beg));
            }
            else if (func == 1)
            {
//                ql << "-0.05" << "0.05" << "0" << "0" << "0.025" << "6" << "4";
                timeVaryParamData.at(param)->setParameter(k, ql);
                timeVaryParamData.at(param)->setParamHeader(k++, label + QString("_BLK1add_") + QString::number(beg));
            }
            else if (func == 2)
            {
//                ql << "1" << "45" << "0" << "36" << "10" << "0" << "-3";
                timeVaryParamData.at(param)->setParameter(k, ql);
                timeVaryParamData.at(param)->setParamHeader(k++, label + QString("_BLK1repl_") + QString::number(beg));
            }
            else if (func == 3)
            {
//                ql << "-29.5361" << "20.4639" << "0" << "0" << "10.2319" << "6" << "4";
                timeVaryParamData.at(param)->setParameter(k, ql);
                timeVaryParamData.at(param)->setParamHeader(k++, label + QString("_BLK1delta_") + QString::number(beg));
            }
        }
    }
    else if (blkPat == -1) // trends
    {
        ql.clear();
        ql << "-4" << "4" << "0" << "0" << "0.5" << "6" << "40";
        timeVaryParamData.at(param)->setParameter(k, ql);
        timeVaryParamData.at(param)->setParamHeader(k++, label + QString("_TrendFinal_LogstOffset"));
        timeVaryParamData.at(param)->setParameter(k, ql);
        timeVaryParamData.at(param)->setParamHeader(k++, label + QString("_TrendInfl_LogstOffset"));
        ql.clear();
        ql << "1" << "20" << "3" << "3" << "3" << "6" << "4" ;
        timeVaryParamData.at(param)->setParameter(k, ql);
        timeVaryParamData.at(param)->setParamHeader(k++, label + QString("_TrendWidth_yrs"));
    }
    else if (blkPat == -2)
    {
        beg = parentModel->get_start_year();
        end = parentModel->get_end_year();
        int infl = (beg + end) / 2;
        ql.clear();
        ql << "0.05" << "0.25" << "0.1" << "0.1" << "0.8" << "0" << "-3";
        timeVaryParamData.at(param)->setParameter(k, ql);
        timeVaryParamData.at(param)->setParamHeader(k++, label + QString("_TrendFinal_direct"));
        ql.clear();
        ql << QString::number(beg) << QString::number(end) << QString::number(infl);
        ql << QString::number(infl) << "0.5" << "6" << "4";
        timeVaryParamData.at(param)->setParameter(k, ql);
        timeVaryParamData.at(param)->setParamHeader(k++, label + QString("_TrendInfl_yr"));
        ql.clear();
        ql << "1" << "20" << "3" << "3" << "3" << "6" << "4" ;
        timeVaryParamData.at(param)->setParameter(k, ql);
        timeVaryParamData.at(param)->setParamHeader(k++, label + QString("_TrendWidth_yr"));
    }
    else if (blkPat == -3)
    {
        ql.clear();
        ql << "0.0001" << "0.999" << "0.5" << "0.5" << "0.5" << "6" << "4";
        timeVaryParamData.at(param)->setParameter(k, ql);
        timeVaryParamData.at(param)->setParamHeader(k++, label + QString("_TrendFinal_frac"));
        timeVaryParamData.at(param)->setParameter(k, ql);
        timeVaryParamData.at(param)->setParamHeader(k++, label + QString("_TrendInfl_frac"));
        ql.clear();
        ql << "1" << "20" << "3" << "3" << "3" << "6" << "4" ;
        timeVaryParamData.at(param)->setParameter(k, ql);
        timeVaryParamData.at(param)->setParamHeader(k++, label + QString("_TrendWidth_yr"));
    }
    else if (blkPat <= -4)
    {
        int seas = parentModel->get_num_seasons();
        for (int i = 0; i < seas; i++)
        {
            ql.clear();
            ql << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            timeVaryParamData.at(param)->setParameter(k, ql);
            timeVaryParamData.at(param)->setParamHeader(k++, label + QString("_TrendFinal_seas%1").arg(QString::number(i)));
        }
    }
    timeVaryParamData.at(param)->setParamCount(k);
//    while (k < timeVaryParamData.at(param)->getParamCount())
//        timeVaryParamData.at(param)->takeRow(k);

    return ql;//timeVaryParamData.at(param)->getRowData(blkst);
}
