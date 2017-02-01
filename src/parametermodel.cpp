#include "parametermodel.h"

shortParameterModel::shortParameterModel(QObject *parent)
    : QObject(parent)
{
    params.setColumnCount(7);
    header << "Lo" << "Hi" << "Init" << "Prior" << "P_type" << "P_sd" << "Phase";
    params.setHeader(header);
    setParamCount(0);

    connect (&params, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
             SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)));
}

shortParameterModel::~shortParameterModel()
{
    header.clear();
}

void shortParameterModel::setHeader(QStringList hdr)
{
    params.setColumnCount(hdr.count());
    params.setHeader(hdr);
}

void shortParameterModel::setParamCount(int rows)
{
    QStringList ql;
    ql << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    for (int i = params.rowCount(); i <= rows; i++)
        params.setRowData(i, ql);
    params.setRowCount(rows);
}

longParameterModel::longParameterModel(QObject *parent)
    : shortParameterModel(parent)
{
    params.setColumnCount(14);
    header << "Lo" << "Hi" << "Init" << "Prior" << "P_type" << "P_sd" << "Phase";
    header << "Env" << "Use_Dev" << "Dev_min" << "Dev_max" << "Dev_sd" << "Use_Blk" << "B_type";
    params.setHeader(header);
    setParamCount(0);

    connect (&params, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
             SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)));
}

longParameterModel::~longParameterModel()
{
    header.clear();
}

void longParameterModel::setParamCount(int rows)
{
    QStringList ql;
    ql << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    for (int i = params.rowCount(); i < rows; i++)
        params.setRowData(i, ql);
    params.setRowCount(rows);
}

bool longParameterModel::envLink(int index)
{
    bool flag = false;
    if (index < params.rowCount())
        if (params.getRowData(index).at(7).toInt() > 0)
            flag = true;
    return (flag);
}

bool longParameterModel::useBlock(int index)
{
    bool flag = false;
    if (index < params.rowCount())
        if (params.getRowData(index).at(12).toInt() > 0)
            flag = true;
    return (flag);
}

bool longParameterModel::useDev(int index)
{
    bool flag = false;
    if (index < params.rowCount())
        if (params.getRowData(index).at(8).toInt() > 0)
            flag = true;
    return (flag);
}

