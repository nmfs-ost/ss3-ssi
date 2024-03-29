#include "tablemodel.h"

#include <QVector>
#include <QModelIndex>
#include <QStandardItem>

tablemodel::tablemodel(QObject *parent)
  : QStandardItemModel(parent)
{
    setTitle(QString("None"));
    reset();

//    connect (parent, SIGNAL(paramsChanged()), SIGNAL(dataChanged()));
}

tablemodel::~tablemodel()
{
//    reset();
}

void tablemodel::reset()
{
//    QStringList ql("no row string");
//    setRowData(0, ql);
    if (rowCount() > 0)
        clear();
}

void tablemodel::setRowCount(int rows)
{
    QStandardItem *stditem = item(0, 0);
    if (stditem == nullptr)
    {
        QList<QStandardItem *> ql;
        stditem = new QStandardItem("0");
        ql.append(stditem);
        for (int i = 0; i < rows; i++)
            appendRow(ql);
    }
    else
    {
        int chk = rowCount();
        if (chk != rows)
        {
            int cols = columnCount();
            QStandardItemModel::setRowCount(rows);
            if (cols < 1) cols = 1;
            QStringList ql, row;
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
    if (columnCount() < rowstringlist.count())
        setColumnCount(rowstringlist.count());
    if (row >= rowCount())
        setRowCount(row + 1);

    for (int i = 0; i < rowstringlist.count(); i++)
    {
        QStandardItem *pxi = new QStandardItem(rowstringlist.at(i));
        px << pxi;
    }
    insertRow(row, px);
    removeRow(row + 1);
//    emit dataChanged();
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

void tablemodel::addRowData(QStringList &rowstringlist)
{
    int row = rowCount();
    setRowData(row, rowstringlist);
}

void tablemodel::removeHeader()
{
    int cols = columnCount();
    QStringList heads;
    for (int i = 0; i < cols; i++)
        heads << QString("");
    setHeader(heads);
}

void tablemodel::setHeader(QStringList titles)
{
    int cols = titles.count();
    if (cols == 0)
    {
        cols = columnCount();
        for (int i = 0; i < cols; i++)
            titles.append(QString::number(i+1));
    }
    else
    {
        setColumnCount(cols);
    }
    for (int i = 0; i < cols; i++)
        setColumnHeader(i, QString(titles.at(i)));
}

void tablemodel::setColumnHeader(int column, QString title)
{
    setHeaderData(column, Qt::Horizontal, QString(title));
}

QString tablemodel::getColumnHeader(int column)
{
    return headerData(column, Qt::Horizontal).toString();
}

void tablemodel::setRowHeader(int row, QString title)
{
    setHeaderData(row, Qt::Vertical, QString(title));
}

QString tablemodel::getRowHeader(int row)
{
    return headerData(row, Qt::Vertical).toString();
}

void tablemodel::setTitle(QString strtitle)
{
    title = QString(strtitle);
}

QString &tablemodel::getTitle()
{
    return title;
}

