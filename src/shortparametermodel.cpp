#include "shortparametermodel.h"

ShortParameterModel::ShortParameterModel(QObject *parent) : QObject(parent)
{
    paramTable = new tablemodel(this);
    colHeader << "LO" << "HI" << "INIT" << "PRIOR" << "PR_SD" << "PR_type" << "PHASE";
    defaultParam << "-1" << "1" << "0" << "0" << "0" << "0" << "0";
}

ShortParameterModel::~ShortParameterModel()
{
    delete paramTable;
}

void ShortParameterModel::resetToData()
{
    QStringList data;
    int num = paramData.count();
    paramTable->setRowCount(num);

    for (int i = 0; i < num; i++)
    {
        paramTable->setRowData(i, paramData[i]);
    }
}

void ShortParameterModel::setParameterColHeader (QStringList colhdrs)
{
    colHeader = colhdrs;
    paramTable->setHeader(colhdrs);
}

void ShortParameterModel::setTotalNumParameters (int rows)
{
    while (paramData.count() < rows)
        paramData.append(defaultParam);
    while (paramData.count() > rows)
        paramData.takeLast();
    paramTable->setRowCount(rows);
}

int ShortParameterModel::getTotalNumParameters ()
{
    return paramData.count();
}

void ShortParameterModel::setParametersUsed (QList<int> data)
{}
void ShortParameterModel::setParameterUsed (int i, bool flag)
{}
int ShortParameterModel::getNumParameters ()
{
    return paramTable->rowCount();
}

void ShortParameterModel::setParameterData(int row, QStringList &rowstringlist)
{
    paramData[row] = rowstringlist;
}

void ShortParameterModel::setParameter(int row, QStringList &rowstringlist)
{
    paramTable->setRowData(row, rowstringlist);
}

void ShortParameterModel::setParameterHeader (int row, QString title)
{
    paramTable->setRowHeader(row, title);
}

void ShortParameterModel::updateParameters ()
{}
void ShortParameterModel::updateParamHeaders ()
{}
void ShortParameterModel::checkParameterData ()
{}
void ShortParameterModel::updateParameterData (QModelIndex tl, QModelIndex br, QVector<int> data)
{}


void ShortParameterMultiModel::setMultiplier(int newmult)
{
    mult = newmult;
}
