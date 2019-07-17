#include "parametermodel.h"

setupTable::setupTable (QObject *parent) : QObject(parent)
{
    dataTable.setRowCount(1);
    dataTable.setColumnCount(0);
    setDataHeader("");

    connect (&dataTable, SIGNAL(dataChanged()), SLOT(updateList()));
}

setupTable::~setupTable ()
{ }

void setupTable::setHeaders (QStringList hdrs)
{
    dataTable.setHeader(hdrs);
}

QStringList setupTable::getHeaders ()
{
    QStringList hdr (getColHeader(0));
    for (int i = 1; i < dataTable.columnCount(); i++)
        hdr.append(getColHeader(i));
    return hdr;
}

void setupTable::setColHeader (int col, QString hdr)
{
    dataTable.setColumnHeader(col, hdr);
}

QString setupTable::getColHeader (int col)
{
    return dataTable.getColumnHeader(col);
}

void setupTable::setDataHeader (QString hdr)
{
    dataTable.setRowHeader(0, hdr);
}

QString setupTable::getDataHeader ()
{
    return dataTable.getRowHeader(0);
}

void setupTable::setData (QStringList data)
{
    dataTable.setRowData(0, data);
}

QStringList setupTable::getData ()
{
    return dataTable.getRowData(0);
}

void setupTable::setValues (QList<int> values)
{
    QStringList dlist;
    for (int i = 0; i < values.count(); i++)
        dlist.append(QString::number(values.at(i)));
    setData(dlist);
}

QList<int> setupTable::getValues ()
{
    QStringList dlist (getData());
    valuesList.clear();
    for (int i = 0; i < dlist.count(); i++)
        valuesList.append(QString(dlist.at(i)).toInt());
    return valuesList;
}

void setupTable::setValue (int col, int value)
{
    getValues();
    while (valuesList.count() < col)
        valuesList.append(0);
    valuesList[col] = value;
    setValues(valuesList);
    emit valuesChanged(valuesList);
}

int setupTable::getValue (int col)
{
    int val;
    getValues();
    val = valuesList.at(col);
    return val;
}

tablemodel *setupTable::getTable()
{
    return &dataTable;
}

void setupTable::updateList()
{
    QStringList data(getData());
    int item;
    bool changed = false;
    for (int i = 0; i < data.count(); i++)
    {
        item = QString(data.at(i)).toInt();
        if (item != valuesList.at(i))
        {
            valuesList[i] = item;
            changed = true;
        }
    }
    if (changed)
    {
        emit valuesChanged(valuesList);
        emit dataChanged();
    }
}


shortParameterTable::shortParameterTable(QObject *parent)
    : QObject(parent)
{
    header << "Lo" << "Hi" << "Init" << "Prior" << "P_sd" << "P_type" << "Phase";
    defaultParam << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    paramTable.setHeader(header);
    setParamCount(0);

    connect (&paramTable, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
             SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)));
}

shortParameterTable::~shortParameterTable()
{
    header.clear();
}

void shortParameterTable::setHeader(QStringList hdr)
{
    paramTable.setColumnCount(hdr.count());
    paramTable.setHeader(hdr);
}

void shortParameterTable::setParamCount(int rows)
{
    for (int i = paramTable.rowCount(); i <= rows; i++)
        paramTable.setRowData(i, defaultParam);
    paramTable.setRowCount(rows);
}

longParameterTable::longParameterTable(QObject *parent)
    : shortParameterTable(parent)
{
    header << "Env" << "Use_Dev" << "Dev_min" << "Dev_max" << "Dev_sd" << "Use_Blk" << "B_type";
    defaultParam << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    paramTable.setHeader(header);
    setParamCount(0);

//    disconnect (&paramTable, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this,
//             SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)));
    connect (&paramTable, SIGNAL(dataChanged(QModelIndex, QModelIndex, QVector<int>)), this,
             SLOT(checkParameters(QModelIndex, QModelInes, QVector<int>)));
}

longParameterTable::~longParameterTable()
{
    header.clear();
}

void longParameterTable::setParamCount(int rows)
{
    while (parEnvLink.count() < rows)
    {
        parEnvLink.append(0);
        parUseDev.append(0);
        parBlock.append(0);
    }
    for (int i = paramTable.rowCount(); i < rows; i++)
    {
        paramTable.setRowData(i, defaultParam);
    }
    while (parEnvLink.count() > rows)
    {
        parEnvLink.removeLast();
        parUseDev.removeLast();
        parBlock.removeLast();
    }
    paramTable.setRowCount(rows);
}

void longParameterTable::setParameter(int row, QStringList param)
{
    paramTable.setRowData(row, param);
    parEnvLink[row] = envLink(row);
    parUseDev[row] = useDev(row);
    parBlock[row] = useBlock(row);
}

int longParameterTable::envLink(int index)
{
    return paramTable.getRowData(index).at(7).toInt();
}

int longParameterTable::useDev(int index)
{
    return paramTable.getRowData(index).at(8).toInt();
}

int longParameterTable::useBlock(int index)
{
    return paramTable.getRowData(index).at(12).toInt();
}

void longParameterTable::setVarParamCount(int rows)
{
    varParamTable.setRowCount(rows);
}

void longParameterTable::checkParameters(QModelIndex tplt, QModelIndex btrt, QVector<int> ivect)
{
    int startRow = tplt.row();
//    int startCol = tplt.column();
    int endRow = btrt.row();
    int endCol = btrt.column();
    bool varChanged = false;
    int lnk, dev, blk;

    Q_UNUSED(ivect);

    if (endCol > 7)
    {
     //   changeVarData (startRow, startCol, endRow, endCol);
        for (int i = startRow; i <= endRow; i++)
        {
            lnk = envLink(i);
            dev = useDev(i);
            blk = useBlock(i);
            if (parEnvLink[i] != lnk)
            {
                varChanged = true;
                parEnvLink[i] = lnk;
            }
            if (parUseDev[i] != dev)
            {
                varChanged = true;
                parUseDev[i] = dev;
            }
            if (parBlock[i] != blk)
            {
                varChanged = true;
                parBlock[i] = blk;
            }

        }
        if (varChanged)
        {

            emit varDataChanged();
        }
    }
}

