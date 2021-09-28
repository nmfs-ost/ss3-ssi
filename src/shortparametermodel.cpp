#include "shortparametermodel.h"

ShortParameterModel::ShortParameterModel(QObject *parent) : QObject(parent)
{
    QList<bool> paramUsed;
    QList<int> paramNum;
    tablemodel *paramTable = new tablemodel(this);
    QList<QStringList> paramData;
    QStringList colHeader;
    QStringList defaultParam;

}

ShortParameterModel::~ShortParameterModel()
{
    delete paramTable;
}

void ShortParameterModel::resetToDefault()
{
    int num = paramData.count();

    colHeader.clear();
    colHeader << "Lo" << "Hi" << "Val" << "";

}

void ShortParameterModel::setParameterColHeader (QStringList colhdrs)
{}
void ShortParameterModel::setTotalNumParameters (int rows)
{}
int ShortParameterModel::getTotalNumParameters ()
{
    return 0;
}

void ShortParameterModel::setParametersUsed (QList<int> data)
{}
void ShortParameterModel::setParameterUsed (int i, bool flag)
{}
int ShortParameterModel::getNumParameters ()
{
return 0;
}

void ShortParameterModel::setParameterData(int row, QStringList &rowstringlist)
{}
void ShortParameterModel::setParameter(int row, QStringList &rowstringlist)
{}
void ShortParameterModel::setParameterHeader (int row, QString title)
{}

void ShortParameterModel::updateParameters ()
{}
void ShortParameterModel::updateParamHeaders ()
{}
void ShortParameterModel::checkParameterData ()
{}
void ShortParameterModel::updateParameterData (QModelIndex tl, QModelIndex br, QVector<int> data)
{}
