#include "parametermodelTV.h"
#include "model.h"
#include <QString>

parameterModelTV::parameterModelTV(QObject *parent)
    : parameterModel(parent)
{
    connect (this, SIGNAL(rowsInserted(QModelIndex,int,int)), SLOT(insertData(QModelIndex,int,int)));
    connect (this, SIGNAL(rowsRemoved(QModelIndex,int,int)), SLOT(removeData(QModelIndex,int,int)));
    connect (this, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), SLOT(changeData(QModelIndex,QModelIndex,QVector<int>)));
    connect (this, SIGNAL(tableChanged()), SLOT(changeData()));

//    timeVarianceModel = new shortparametermodel(this);
}

parameterModelTV::~parameterModelTV()
{
    shortParameterModel *spm;
    while (timeVarianceList.count() > 0)
    {
        spm = timeVarianceList.takeLast();
        delete spm;
    }
//    delete timeVarianceModel;
    header.clear();
}

bool parameterModelTV::envLink(int index)
{
    return envLinks.at(index);
/*    bool flag = false;
    if (index < rowCount())
        if (getRowData(index).at(7).toInt() > 0)
            flag = true;
    return (flag);*/
}

bool parameterModelTV::useBlock(int index)
{
    return blkLinks.at(index) != 0;
/*    bool flag = false;
    if (index < rowCount())
        if (getRowData(index).at(12).toInt() > 0)
            flag = true;
    return (flag);*/
}

bool parameterModelTV::useDev(int index)
{
    return devLinks.at(index);
/*    bool flag = false;
    if (index < rowCount())
        if (getRowData(index).at(8).toInt() > 0)
            flag = true;
    return (flag);*/
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
        if (devLinks.at(i))
        {
            ql = timeVarianceList.at(i)->getRowData(1);
            timeVarianceTable->setRowData(k, ql);
            timeVarianceTable->setRowHeader(k++, timeVarianceList.at(i)->getRowHeader(1));
            ql = timeVarianceList.at(i)->getRowData(2);
            timeVarianceTable->setRowData(k, ql);
            timeVarianceTable->setRowHeader(k++, timeVarianceList.at(i)->getRowHeader(2));
        }
        if (envLinks.at(i))
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

QStringList parameterModelTV::autoGenEnvParam(int param, bool value, QString label)
{
    QStringList ql;

    timeVarianceList.at(param)->setRowData(0, ql);
    timeVarianceList.at(param)->setRowHeader(0, label + QString("_env"));
    return timeVarianceList.at(param)->getRowData(0);
}

QStringList parameterModelTV::autoGenDevParam(int param, bool value, QString label)
{
    QStringList ql;

    timeVarianceList.at(param)->setRowData(1, ql);
    timeVarianceList.at(param)->setRowHeader(1, label + QString("_se"));
    timeVarianceList.at(param)->setRowData(2, ql);
    timeVarianceList.at(param)->setRowHeader(2, label + QString("_rho"));
    return timeVarianceList.at(param)->getRowData(1);
}

QStringList parameterModelTV::autoGenBlkParam(int param, int block, QString label)
{
    QStringList ql;
    BlockPattern *bp = parentModel->getBlockPattern(block);
    int numBlocks = bp->getNumBlocks();
    int beg = 0;
    int end = 0;
    int k = 0;

    for (int i = 0; i < numBlocks; i++)
    {
        beg = bp->getBlockBegin(i);
        end = bp->getBlockEnd(i);
        timeVarianceList.at(param)->setRowData(k, ql);
        timeVarianceList.at(param)->setRowHeader(k++, label + QString("_block_") + QString::number(i));
    }

    return timeVarianceList.at(param)->getRowData(3);
}
