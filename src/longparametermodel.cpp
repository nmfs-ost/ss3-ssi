#include "longparametermodel.h"

LongParameterModel::LongParameterModel(QObject *parent) : ShortParameterModel(parent)
{
    defaultParam << "0" << "99" << "0" << "0" << "0" << "0" << "0";
    defaultParam << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    timevaryParamTable = new tablemodel(this);
}

int LongParameterModel::getNumTimeVaryParameters()
{
    return timevaryParamTable->rowCount();
}

void LongParameterModel::setTimeVaryParameter (int i, QStringList data)
{
    timevaryParamTable->setRowData(i, data);
    updateParameters();
}

QStringList LongParameterModel::getTimeVaryParameter (int i)
{
    return timevaryParamTable->getRowData(i);
}

QString LongParameterModel::getTimeVaryParameterHeader(int i)
{
    return timevaryParamTable->getRowHeader(i);
}

void LongParameterModel::setTimeVaryParameterHeader(int i, QString hdr)
{
    timevaryParamTable->setRowHeader(i, hdr);
}

tablemodel *LongParameterModel::getTimeVaryParameterModel ()
{
    return timevaryParamTable;
}

