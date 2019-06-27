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
    void setParamHeader (int row, QString hdr) {paramTable.setRowHeader (row, hdr);}
    QString getParamHeader (int row) {return paramTable.getRowHeader(row);}
    tablemodel *getParameters () {return &paramTable;}

signals:
    void dataChanged(QModelIndex, QModelIndex, QVector<int>);
    void dataChanged();

protected:
    QStringList header;
    QStringList defaultParam;
    tablemodel paramTable;
};

class longParameterTable : public shortParameterTable
{
    Q_OBJECT

public:
    longParameterTable(QObject *parent = 0);
    ~longParameterTable();

    void setParamCount(int rows);
    void setParameter (int row, QStringList param);

    int envLink (int index);
    int useDev(int index);
    int useBlock (int index);

    void setVarParamCount(int rows);
    int getVarParamCount() {return varParamTable.rowCount();}
    void setVarParameter (int row, QStringList param) {varParamTable.setRowData(row, param);}
    QStringList getVarParameter (int row) {return varParamTable.getRowData(row);}
    void seVartParamHeader (int row, QString hdr) {varParamTable.setRowHeader (row, hdr);}
    QString getVarParamHeader (int row) {return varParamTable.getRowHeader(row);}
    tablemodel *getVarParameters () {return &varParamTable;}

public slots:
    void checkParameters(QModelIndex, QModelIndex, QVector<int> = QVector<int>());
//    void changeVarData (int startRow, int startCol, int endRow, int endCol);


signals:
    void varDataChanged();
    void varDataChanged(QModelIndex, QModelIndex, QVector<int>);

private:
    QList<int> parEnvLink;
    QList<int> parUseDev;
    QList<int> parBlock;
    QList<int> varParamsPerParameter;
    tablemodel varParamTable;

};

#endif // PARAMETERMODEL_H
