#include "tablemodel.h"

#include <QVector>
#include <QModelIndex>
#include <QStandardItem>

tablemodel::tablemodel(QObject *parent)
  : QStandardItemModel(parent)
{
    reset();
}

tablemodel::~tablemodel()
{
//    reset();
}


void tablemodel::setRowCount(int rows)
{
    int chk = rowCount();
    if (rows != chk)
    {
        int cols = columnCount();
        QStringList ql, row;
        QStandardItemModel::setRowCount(rows);
        for (int i = 0; i < cols; i++)
            ql << QString("0");
        for (int i = chk; i < rows; i++)
        {
            row = getRowData(i);
            if (row.count() < cols)
                setRowData(i, ql);
        }
        emit dataChanged();
    }
}

void tablemodel::setRowData(int row, QVector<double> rowdata)
{
    QStringList ql;
    for (int i = 0; i < rowdata.count(); i++)
        ql << QString::number(rowdata.at(i));
    setRowData(row, ql);
    /*
    QList<QStandardItem *> px;
    if (row >= rowCount())
        setRowCount(row + 1);

    for (int i = 0; i < rowdata.count(); i++)
    {
        QStandardItem *pxi = new QStandardItem(QString::number(rowdata.at(i),'g',6));
//        setItem(row, i, pxi);
        px << pxi;
    }
    insertRow(row, px);
    removeRow(row + 1);
    emit dataChanged();*/
}

void tablemodel::setRowData(int row, QStringList &rowstringlist)
{
    QList<QStandardItem *> px;
    if (row >= rowCount())
        setRowCount(row + 1);

    for (int i = 0; i < rowstringlist.count(); i++)
    {
        QStandardItem *pxi = new QStandardItem(rowstringlist.at(i));
        px << pxi;
    }
    insertRow(row, px);
    removeRow(row + 1);
    emit dataChanged();
}

void tablemodel::setRowData(int &row, QString rowstring)
{
    QStringList datalist (rowstring.split(' ', QString::SkipEmptyParts));
    setRowData(row, datalist);
}

QStringList tablemodel::getRowData(int row)
{
    QVariant qdata;
    QStringList *newstring = new QStringList();

    for (int i = 0; i < columnCount(); i++)
    {
        qdata = data (index(row, i), Qt::EditRole);
        newstring->append(qdata.toString());
    }
    return *newstring;
}

QString tablemodel::getRowText(int row)
{
    QStringList list = getRowData(row);
    QString text("");
    for (int i = 0; i < list.count(); i++)
        text.append(QString(" %1").arg(list.at(i)));
    return text;
}

void tablemodel::setHeader(QStringList &titles)
{
    int cols = titles.count();
    setColumnCount(cols);
    for (int i = 0; i < cols; i++)
        setHeaderData(i, Qt::Horizontal, titles.at(i));
}

void tablemodel::setColumnHeader(int column, QString title)
{
    setHeaderData(column, Qt::Horizontal, title);
}

QString tablemodel::getColumnHeader(int column)
{
    return headerData(column, Qt::Horizontal).toString();
}

void tablemodel::setRowHeader(int row, QString title)
{
    setHeaderData(row, Qt::Vertical, title);
}

QString tablemodel::getRowHeader(int row)
{
    return headerData(row, Qt::Vertical).toString();
}

void tablemodel::setTitle(QString strtitle)
{
    title = strtitle;
}

QString tablemodel::getTitle()
{
    return title;
}

