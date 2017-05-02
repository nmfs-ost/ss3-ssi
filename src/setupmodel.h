#ifndef SETUPMODEL_H
#define SETUPMODEL_H

#include <QObject>
//#include "tablemodel.h"
#include "parametermodel.h"

class ss_model;

/** This class holds a single row table of integer values. It may be
   used alone or to control the number of parameters in an attached
   table.
*/
class setupModel : public QObject
{
    Q_OBJECT
public:
    explicit setupModel(QObject *parent = 0);
    ~setupModel ();

signals:
    void dataChanged(QList<int> data);

public slots:
    void setNumValues (int cols);
    int getNumValues () {return dataTable->columnCount();}
    void setValue (int i, int value);
    int getValue (int i);
    void setData (QStringList data);
    QStringList getData () {return dataTable->getRowData(0);}
    void setHeader (QStringList hdr);
    void setColHeader (int i, QString hdr) {dataTable->setColumnHeader(i, hdr);}
    QString getColHeader (int i) {return dataTable->getColumnHeader(i);}
    void setRowHeader (QString rhdr) {dataTable->setRowHeader(0, rhdr);}
    tablemodel *getTable () {return dataTable;}
    setupModel *getModel () {return this;}

    void updateTable ();
    void updateData ();
    bool useParam (int index) {return valuesList.at(index);}

protected:
//    ss_model *parentModel;

    tablemodel *dataTable;
    QList<int> valuesList;

private slots:
};


class shortParameterModel : public QObject
{
    Q_OBJECT
public:
    explicit shortParameterModel(QObject *parent = 0);
    ~shortParameterModel();

signals:
    void paramsChanged();
    void paramChanged(int parm, QStringList data);

public slots:
    void setParamModelHeader (QStringList colhdrs)
        {paramTable->setHeader(colhdrs);}

    void setNumParams(int rows);
    int getNumParams () {return paramTable->rowCount();}

    void setTotalNumParams (int rows);
    void setParamHeaders(QStringList hdr);

    void setParamsUsed (QList<int> data);

    void setParamHeader (int row, QString title);
    QString getParamHeader (int row) {return paramData->getRowHeader(row);}
    void setParameter(int row, QStringList &rowstringlist);
    void setParamData(int row, QStringList &rowstringlist);
    QStringList getParameter (int row) {return paramTable->getRowData(row);}
    QString getParamText (int row);
    tablemodel *getParamTable () {return paramTable;}
    shortParameterModel *getParamModel() {return this;}

    void updateParams ();
    void updateParamHeaders ();
    void checkParamData();
    void updateParamData(QModelIndex tl, QModelIndex br, QVector<int> data);

protected:
    QList<int> paramUsed;
    QList<int> paramNum;
    tablemodel *paramTable;
    tablemodel *paramData;
    QStringList header;
    QStringList defaultParam;

private slots:
};


class shortParamMultModel : public shortParameterModel
{
    Q_OBJECT
public:
    explicit shortParamMultModel(QObject *parent = 0);
    ~shortParamMultModel();

signals:

public slots:
    void setMultiplier (int newmult);
    int getMultiplier () {return mult;}
    void setNumParams(int rows);

    void setTotalNumParams (int rows);
    void setParamHeaders(QStringList hdr);
    void setParamDataHeader (int row, QString hdr);

    void setParamsUsed (QList<int> data);

    void setParamHeader (int row, QString title);
    QString getParamHeader (int row) {return paramTable->getRowHeader(row);}
    void setParameter(int row, QStringList &rowstringlist) {paramTable->setRowData(row, rowstringlist);}
    QStringList getParameter (int row) {return paramTable->getRowData(row);}
//    QString getParamText (int row);

    tablemodel *getParamTable () {return paramTable;}
    shortParamMultModel *getParamModel() {return this;}

    void updateParams ();
//    void checkParamData();
    void updateParamData(QModelIndex tl, QModelIndex br, QVector<int> data);

protected:
    int mult;

private slots:
};


class longParameterModel : public shortParameterModel
{
    Q_OBJECT
public:
    explicit longParameterModel(QObject *parent = 0);
    ~longParameterModel();

signals:

public slots:
    bool useEnvVar (int param) {return envLink(param) != 0;}
    bool useDevs (int param) {return devValue(param) != 0;}
    bool useBlks (int param) {return blkNum(param) != 0;}

    int envLink (int param) {return QString(paramData->getRowData(param).at(7)).toInt();}
    int devValue (int param) {return QString(paramData->getRowData(param).at(8)).toInt();}
    int blkNum (int param) {return QString(paramData->getRowData(param).at(12)).toInt();}
    int BlkFnc (int param) {return QString(paramData->getRowData(param).at(13)).toInt();}

protected:

private slots:
    void updateParams ();
    void updateParamData(QModelIndex tl, QModelIndex br, QVector<int> data);

};


class timeVaryParameterModel : public QObject
{
    Q_OBJECT
public:
    explicit timeVaryParameterModel(ss_model *parent = 0);
    ~timeVaryParameterModel();

signals:
    void varParamChanged(int parm, QStringList data);
    void varParamsChanged();

public slots:
    int getAutoGenerate() const;
    void setAutoGenerate(int value);

    void setNumParams (int tables) {setTotalNumVarTables(tables);}
    int getNumParams () {return getTotalNumVarTables();}
    void setParameter (int table, QStringList data)
        {changeVarParamData(table, data);}

    void setNumVarParams(int rows);
    int getNumVarParams () {return varyParamTable->rowCount();}

    void setTotalNumVarTables (int rows);
    int getTotalNumVarTables () {return varyParamDataTables.count();}
    void setVarParamHeaders (QStringList hdr);

    void setVarParamsUsed (QList<int> data);

    void setVarParamHeader (int row, QString title);// {varyParams->setParamHeader(row, title);}
    QString getVarParamHeader (int row) {return varyParamTable->getRowHeader(row);}
    void setVarParameter(int row, QStringList &rowstringlist);// {varyParams->setParameter(row, rowstringlist);}
    QStringList getVarParameter (int row) {return varyParamTable->getRowData(row);}
    tablemodel *getVarParamTable () {return varyParamTable;}
    timeVaryParameterModel *getVarParamModel() {return this;}

    void updateVarParams ();
    void updateVarParamHdrs ();
    void changeVarParamData(int parm, QStringList data);
    void changeVarParamHeader(int parm, QString title);
    void updateVarParamData(QModelIndex tl, QModelIndex br, QVector<int> data);

    int checkEnvVarValue (int value);
    int checkDevValue (int value);
    int checkBlockNum (int value);
    int checkBlockFxn (int value);

    bool generateVarParamData (int parmNum, QStringList data);

    void autoGenEnvVarParam (int parmnum, int value);
    void autoGenDevVarParam (int parmnum, int value);
    void autoGenBlkVarParam (int parmnum, int value, int fxn, QStringList parmdata);

    tablemodel * newVaryParamTable ();
    void setTableTitle (int tbl, QString name);
    void updateTableParamHdrs (int tbl);

protected:
    ss_model *model_data;
    QStringList header;
    QStringList defaultParam;
    int autoGenerate;

    tablemodel *varyParamTable;
    QList<int> tableNum;
    QList<int> tableParNum;

    QList<tablemodel *> varyParamDataTables;
    QStringList tableHeaders;
    QList<int> tableUsed;
    QList<int> blkVal;
    QList<int> blkFxn;
    QList<int> devVal;
    QList<int> envVal;

};

#endif // SETUPMODEL_H
