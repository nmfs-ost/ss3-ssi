/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef PARAMETERMODEL_H
#define PARAMETERMODEL_H

#include "tablemodel.h"

#include <QStringList>

class setupTable : public QObject
{
    Q_OBJECT

public:
    setupTable (QObject *parent = 0);
    ~setupTable ();

public slots:
    void setHeaders (QStringList hdrs);
    QStringList getHeaders ();
    void setColHeader (int col, QString hdr);
    QString getColHeader (int col);

    void setDataHeader (QString hdr);
    QString getDataHeader ();

    void setData (QStringList data);
    QStringList getData ();
    void setValues (QList<int> values);
    QList<int> getValues ();

    void setValue (int col, int value);
    int getValue (int col);

    tablemodel *getTable();

signals:
    void valuesChanged(QList<int> values);
    void dataChanged ();

private slots:
    void updateList ();

protected:
    tablemodel dataTable;
    QList<int> valuesList;
};

class shortParameterTable : public QObject
{
    Q_OBJECT

public:
    shortParameterTable (QObject *parent = 0);
    ~shortParameterTable ();

    void setHeader (QStringList hdr);
    QStringList getHeader ();
    void setParamCount(int rows);
    int getParamCount() {return paramTable.rowCount();}
    void setParameter (int row, QStringList param) {paramTable.setRowData(row, param);}
    QStringList getParameter (int row) {return paramTable.getRowData(row);}
    tablemodel *getParameters () {return &paramTable;}
//    QStringList getParameter (int row);
    void setParamHeader (int row, QString hdr) {paramTable.setRowHeader (row, hdr);}
    QString getParamHeader (int row) {return paramTable.getRowHeader(row);}

    tablemodel paramTable;

signals:
    void dataChanged(QModelIndex, QModelIndex, QVector<int>);
    void dataChanged();

protected:
    QStringList header;
    QStringList defaultParam;
};

class longParameterTable : public shortParameterTable
{
    Q_OBJECT

public:
    longParameterTable(QObject *parent = 0);
    ~longParameterTable();

    void setParamCount(int rows);

    bool envLink (int index);
    bool useBlock (int index);
    bool useDev (int index);

private:
};

#endif // PARAMETERMODEL_H
