#include "parametermodelTV.h"
#include "model.h"
#include <QString>

parameterModelTV::parameterModelTV(ss_model *parent)
    : parameterModel(parent)
{
    parentModel = parent;
    connect (this, SIGNAL(rowsInserted(QModelIndex,int,int)), SLOT(insertData(QModelIndex,int,int)));
    connect (this, SIGNAL(rowsRemoved(QModelIndex,int,int)), SLOT(removeData(QModelIndex,int,int)));
    connect (this, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), SLOT(changeData(QModelIndex,QModelIndex,QVector<int>)));
//    connect (this, SIGNAL(tableChanged()), SLOT(changeData()));

    timeVarianceTable = new shortParameterModel(this);
    blkst = 3;
}

parameterModelTV::~parameterModelTV()
{
    shortParameterModel *spm;
    while (timeVarianceList.count() > 0)
    {
        spm = timeVarianceList.takeLast();
        delete spm;
    }
    delete timeVarianceTable;
    header.clear();
}

void parameterModelTV::setParentModel(ss_model *pModel)
{
    parentModel = pModel;
}

bool parameterModelTV::useBlock(int index)
{
    return blkLinks.at(index) != 0;
}

bool parameterModelTV::useDev(int index)
{
    return devLinks.at(index) != 0;
}

bool parameterModelTV::envLink(int index)
{
    return envLinks.at(index) != 0;
}

void parameterModelTV::updateTimeVarTable()
{
    QString label;
    QStringList ql;
    timeVarianceTable->clear();
    int k = 0;
    for (int i = 0; i < rowCount(); i++)
    {
        ql = getRowData(i);
        label = getRowHeader(i);
        if (blkLinks.at(i) != 0)
        {
            for (int j = 3; j < timeVarianceList.at(i)->rowCount(); j++)
            {
                ql = timeVarianceList.at(i)->getRowData(j);
                timeVarianceTable->setRowData(k, ql);
                timeVarianceTable->setRowHeader(k++, timeVarianceList.at(i)->getRowHeader(j));
            }
        }
        if (devLinks.at(i) != 0)
        {
            ql = timeVarianceList.at(i)->getRowData(1);
            timeVarianceTable->setRowData(k, ql);
            timeVarianceTable->setRowHeader(k++, timeVarianceList.at(i)->getRowHeader(1));
            ql = timeVarianceList.at(i)->getRowData(2);
            timeVarianceTable->setRowData(k, ql);
            timeVarianceTable->setRowHeader(k++, timeVarianceList.at(i)->getRowHeader(2));
        }
        if (envLinks.at(i) != 0)
        {
            ql = timeVarianceList.at(i)->getRowData(0);
            timeVarianceTable->setRowData(k, ql);
            timeVarianceTable->setRowHeader(k++, timeVarianceList.at(i)->getRowHeader(0));
        }
    }
}

void parameterModelTV::modelChanged()
{
    changeData();
}

void parameterModelTV::changeData()
{
    quintptr ptr = 0;
    int rows = rowCount();
    QModelIndex tplt(index(0, 0));// (0, 0, ptr, this);
    QModelIndex btrt(index(rows - 1, 13)); // ((rows - 1), 14, ptr, this);
    changeData(tplt, btrt);
}

void parameterModelTV::changeData(QModelIndex tplt, QModelIndex btrt, QVector<int> ivect)
{
    QString schk;
    int chk;
    QStringList ql;
    bool check, changed = false;
    int start = tplt.row();
    int end = btrt.row();
    if (btrt.column() > 6)
    {
        for (int i = start; i <= end; i++)
        {
            ql = getRowData(i);
            schk = QString(ql.at(7));
            check = (schk.compare("1") == 0);
            if (envLinks.at(i) != check)
            {
                envLinks[i] = check;
                changed = true;
            }
        }
    }
    if (btrt.column() > 7)
    {
        for (int i = start; i <= end; i++)
        {
            ql = getRowData(i);
            schk = QString(ql.at(8));
            check = (schk.compare("1") == 0);
            if (devLinks.at(i) != check)
            {
                devLinks[i] = check;
                changed = true;
            }
        }
    }
    if (btrt.column() > 11)
    {
        for (int i = start; i <= end; i++)
        {
            ql = getRowData(i);
            schk = QString(ql.at(7));
            chk = schk.toInt();
            if (blkLinks.at(i) != chk)
            {
                blkLinks[i] = chk;
                changed = true;
                autoGenBlkParam (i, chk, getRowHeader(i));
            }
        }
    }
    if (changed)
    {
        updateTimeVarTable();
    }
}

void parameterModelTV::insertData(QModelIndex index, int first, int last)
{
    shortParameterModel *sm;
    QStringList ql;
    for (int i = first; i <= last; i++)
    {
        sm = new shortParameterModel(this);
        sm->setRowCount(4);
        timeVarianceList.insert(i, sm);
        ql = getRowData(i);
        blkLinks.insert(i, (QString(ql.at(12)).toInt()));
        autoGenBlkParam (i, blkLinks.at(i), getRowHeader(i));
        devLinks.insert(i, (ql.at(8).compare("1") == 0));
        autoGenDevParam (i, devLinks.at(i), getRowHeader(i));
        envLinks.insert(i, (ql.at(7).compare("1") == 0));
        autoGenEnvParam (i, envLinks.at(i), getRowHeader(i));
    }
    updateTimeVarTable();
}

void parameterModelTV::removeData(QModelIndex index, int first, int last)
{
    shortParameterModel *sm;
    for (int i = last; i >= first; i--)
    {
        envLinks.removeAt(i);
        devLinks.removeAt(i);
        blkLinks.removeAt(i);
        sm = timeVarianceList.takeAt(i);
        delete sm;
    }
    updateTimeVarTable();
}

void parameterModelTV::setTimeVarBlkParam (int param, int num, QStringList data)
{
    shortParameterModel *sm = timeVarianceList.at(param);
    sm->setRowData(num + blkst, data);
}

void parameterModelTV::setTimeVarDevParam (int param, int num, QStringList data)
{
    shortParameterModel *sm = timeVarianceList.at(param);
    sm->setRowData(num + 1, data);

}

void parameterModelTV::setTimeVarEnvParam (int param, QStringList data)
{
    shortParameterModel *sm = timeVarianceList.at(param);
    sm->setRowData(0, data);
}

int  parameterModelTV::getNumTimeVarParams ()
{
    return timeVarianceTable->rowCount();
}

QStringList parameterModelTV::getTimeVarParam (int row)
{
    return timeVarianceTable->getRowData(row);
}

QString parameterModelTV::getTimeVarParamHdr (int row)
{
    return timeVarianceTable->getRowHeader(row);
}

QStringList parameterModelTV::autoGenEnvParam(int param, int value, QString label)
{
    QStringList ql;
    int link = value / 100;
    int var = value - (link * 100);

    switch (link)
    {
    case 1:
        ql << "-10" << "10" << "1" << "1" << "0.5" << "6" << "4";
        timeVarianceList.at(param)->setRowData(0, ql);
        timeVarianceList.at(param)->setRowHeader(0, label + QString("_ENV_add"));
        break;
    case 2:
        ql << "-10" << "10" << "1" << "1" << "0.5" << "6" << "4";
        timeVarianceList.at(param)->setRowData(0, ql);
        timeVarianceList.at(param)->setRowHeader(0, label + QString("_ENV_mult"));
        break;
    case 3:
        ql << "-0.9" << "0.9" << "0" << "0" << "0.5" << "6" << "4";
        timeVarianceList.at(param)->setRowData(0, ql);
        timeVarianceList.at(param)->setRowHeader(0, label + QString("_ENV_offset"));
        break;
    case 4:
        ql << "-0.9" << "0.9" << "0" << "0" << "0.5" << "6" << "4";
        timeVarianceList.at(param)->setRowData(0, ql);
        timeVarianceList.at(param)->setRowHeader(0, label + QString("_ENV_lgst_slope"));
        break;
    }

    return timeVarianceList.at(param)->getRowData(0);
}

QStringList parameterModelTV::autoGenDevParam(int param, int value, QString label)
{
    QStringList ql;
    int k = 1;
    switch (value)
    {
    case 1:
        ql << "0.0001" << "2" << "0.05" << "0.05" << "0.5" << "6" << "-5";
        timeVarianceList.at(param)->setRowData(k, ql);
        timeVarianceList.at(param)->setRowHeader(k++, label + QString("_dev_se"));
        ql.clear();
        ql << "-0.99" << "0.99" << "0" << "0" << "0.5" << "6" << "-6";
        timeVarianceList.at(param)->setRowData(k, ql);
        timeVarianceList.at(param)->setRowHeader(k++, label + QString("_dev_autocorr"));
        break;
    case 2:
        ql << "0.0001" << "2" << "0.05" << "0.05" << "0.5" << "6" << "-5";
        timeVarianceList.at(param)->setRowData(k, ql);
        timeVarianceList.at(param)->setRowHeader(k++, label + QString("_dev_se"));
        ql.clear();
        ql << "-0.99" << "0.99" << "0" << "0" << "0.5" << "6" << "-6";
        timeVarianceList.at(param)->setRowData(k, ql);
        timeVarianceList.at(param)->setRowHeader(k++, label + QString("_dev_autocorr"));
        break;
    }
    blkst = k;

    return timeVarianceList.at(param)->getRowData(1);
}

QStringList parameterModelTV::autoGenBlkParam(int param, int block, QString label)
{
    QStringList ql;
    BlockPattern *bp;
    int numBlocks = 0;
    int beg = 0;
    int end = 0;
    int k = blkst;
    int func = QString(getRowData(param).at(13)).toInt();

    if (block > 0) // block
    {
        bp = parentModel->getBlockPattern(block-1);
        numBlocks = bp->getNumBlocks();
        for (int i = 0; i < numBlocks; i++)
        {
            beg = bp->getBlockBegin(i);
            end = bp->getBlockEnd(i);
            if (func == 0)
            {
                ql << "-1" << "1" << "1" << "1" << "0.05" << "6" << "4";
                timeVarianceList.at(param)->setRowData(k, ql);
                timeVarianceList.at(param)->setRowHeader(k++, label + QString("_BLK1add_") + QString::number(beg));
            }
            else if (func == 1)
            {
                ql << "-0.05" << "0.05" << "0" << "0" << "0.025" << "6" << "4";
                timeVarianceList.at(param)->setRowData(k, ql);
                timeVarianceList.at(param)->setRowHeader(k++, label + QString("_BLK1add_") + QString::number(beg));
            }
            else if (func == 2)
            {
                ql << "1" << "45" << "0" << "36" << "10" << "0" << "-3";
                timeVarianceList.at(param)->setRowData(k, ql);
                timeVarianceList.at(param)->setRowHeader(k++, label + QString("_BLK1repl_") + QString::number(beg));
            }
            else if (func == 3)
            {
                ql << "-29.5361" << "20.4639" << "0" << "0" << "10.2319" << "6" << "4";
                timeVarianceList.at(param)->setRowData(k, ql);
                timeVarianceList.at(param)->setRowHeader(k++, label + QString("_BLK1delta_") + QString::number(beg));
            }
            ql.clear();
        }
    }
    else if (block == -1) // trends
    {
        ql << "-4" << "4" << "0" << "0" << "0.5" << "6" << "40";
        timeVarianceList.at(param)->setRowData(k, ql);
        timeVarianceList.at(param)->setRowHeader(k++, label + QString("_TrendFinal_LogstOffset"));
        timeVarianceList.at(param)->setRowData(k, ql);
        timeVarianceList.at(param)->setRowHeader(k++, label + QString("_TrendInfl_LogstOffset"));
        ql.clear();
        ql << "1" << "20" << "3" << "3" << "3" << "6" << "4" ;
        timeVarianceList.at(param)->setRowData(k, ql);
        timeVarianceList.at(param)->setRowHeader(k++, label + QString("_TrendWidth_yrs"));
        ql.clear();
    }
    else if (block == -2)
    {
        beg = parentModel->get_start_year();
        end = parentModel->get_end_year();
        int infl = (beg + end) / 2;
        ql << "0.05" << "0.25" << "0.1" << "0.1" << "0.8" << "0" << "-3";
        timeVarianceList.at(param)->setRowData(k, ql);
        timeVarianceList.at(param)->setRowHeader(k++, label + QString("_TrendFinal_direct"));
        ql.clear();
        ql << QString::number(beg) << QString::number(end) << QString::number(infl);
        ql << QString::number(infl) << "0.5" << "6" << "4";
        timeVarianceList.at(param)->setRowData(k, ql);
        timeVarianceList.at(param)->setRowHeader(k++, label + QString("_TrendInfl_yr"));
        ql.clear();
        ql << "1" << "20" << "3" << "3" << "3" << "6" << "4" ;
        timeVarianceList.at(param)->setRowData(k, ql);
        timeVarianceList.at(param)->setRowHeader(k++, label + QString("_TrendWidth_yr"));
        ql.clear();

    }
    else if (block == -3)
    {
        ql << "0.0001" << "0.999" << "0.5" << "0.5" << "0.5" << "6" << "4";
        timeVarianceList.at(param)->setRowData(k, ql);
        timeVarianceList.at(param)->setRowHeader(k++, label + QString("_TrendFinal_frac"));
        timeVarianceList.at(param)->setRowData(k, ql);
        timeVarianceList.at(param)->setRowHeader(k++, label + QString("_TrendInfl_frac"));
        ql.clear();
        ql << "1" << "20" << "3" << "3" << "3" << "6" << "4" ;
        timeVarianceList.at(param)->setRowData(k, ql);
        timeVarianceList.at(param)->setRowHeader(k++, label + QString("_TrendWidth_yr"));
        ql.clear();

    }
    else if (block <= -4)
    {
        int seas = parentModel->get_num_seasons();
        for (int i = 0; i < seas; i++)
        {
            ql << "0" << "0" << "0" << "0" << "0" << "0" << "0";
            timeVarianceList.at(param)->setRowData(k, ql);
            timeVarianceList.at(param)->setRowHeader(k++, label + QString("_TrendFinal_seas%1").arg(QString::number(i)));
            ql.clear();
        }
    }
    while (k < timeVarianceList.at(param)->rowCount())
        timeVarianceList.at(param)->takeRow(k);

    return timeVarianceList.at(param)->getRowData(blkst);
}
