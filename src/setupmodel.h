#ifndef SETUPMODEL_H
#define SETUPMODEL_H

#include <QObject>
#include "parametermodel.h"

class ss_model;

class setupModel : public QObject
{
    Q_OBJECT
public:
    explicit setupModel(ss_model *parent = 0);
    ~setupModel ();

signals:
    void setupChanged();

public slots:
    void setNumSetupVals (int cols);// {table->setColumnCount(cols);}
    int getNumSetupVals () {return setupData.count();}
    void setSetupValue (int i, int value);
    int getSetupValue (int i);
    void setSetupData (QStringList data);// {table->setRowData(0, data);}
    QStringList getSetupData () {return setup->getRowData(0);}
    void setSetupHeader (QStringList hdr);// {table->setHeader(hdr);}
    QString getSetupColHeader (int i) {return setup->getColumnHeader(i);}
    void setSetupRowHeader (QString rhdr) {setup->setRowHeader(0, rhdr);}
    tablemodel *getSetupModel () {return setup;}

    void updateSetupData ();
    bool useParam (int index) {return setupData.at(index);}//QString(table->getRowData(0).at(index)).compare("0");}

protected:
    ss_model *parentModel;

    tablemodel *setup;
    QList<int> setupData;

private slots:
};


class setupShortParamModel : public setupModel
{
    Q_OBJECT
public:
    explicit setupShortParamModel(ss_model *parent = 0);
    ~setupShortParamModel();

signals:
    void paramsChanged();

public slots:
    void setParamModelHeader (QStringList hdr) {params->setHeader(hdr);}

    void setMultiplier (int mlt);
    void setNumParams(int rows) {params->setParamCount(rows);}
    int getNumParams () {return params->getParamCount();}

    void setParamData(int row, QStringList &rowstringlist) {params->setParameter(row, rowstringlist);}
    QStringList getParamData (int row) {return params->getParameter(row);}
    QString getParamText (int row);
    tablemodel *getParamModel () {return params->getParameters();}

    void setParamHeaders();
    void setParamHeader (int row, QString title) {params->setParamHeader(row, title);}
    QString getParamHeader (int row) {return params->getParamHeader(row);}

    void updateParams ();
    void changeParamData();
    void updateParamData(QModelIndex tl, QModelIndex br, QVector<int> data);

protected:
    int mult;
    shortParameterModel *params;
    shortParameterModel *paramData;

private slots:
};


class setupLongParamModel : public setupModel
{
    Q_OBJECT
public:
    explicit setupLongParamModel(ss_model *parent = 0);
    ~setupLongParamModel();

signals:
    void paramsChanged();

public slots:
/*    void setSetupValue (int i, int value);
    void setSetupData (QStringList data);
    void setSetupHeader (QStringList hdr);
    void setSetupRowHeader (QString rhdr);
    void setSetupNumber (int cols);
*/
    void setNumParams(int rows) {params->setParamCount(rows);}
//    void setParamData(int &row, QString rowstring) {params->setParameter(row, rowstring);}
    void setParamData(int row, QStringList &rowstringlist) {params->setParameter(row, rowstringlist);}
//    void setParamData(int row, QVector<double> rowdata) {params->setParameter(row, rowdata);}

    int getNumParams () {return params->getParamCount();}
    QStringList getParamData (int row) {return params->getParameter(row);}
    QString getParamText (int row);
    tablemodel *getParamModel () {return params->getParameters();}

    void setParamHeader (int row, QString title) {params->setParamHeader(row, title);}
    QString getParamHeader (int row) {return params->getParamHeader(row);}

    void updateParams ();
    void changeParamData();
    void updateParamData(QModelIndex tl, QModelIndex br, QVector<int> data);
    bool useEnvVar (int param) {return QString(params->getParameter(param).at(7)).compare("0");}
    bool useDevs (int param) {return QString(params->getParameter(param).at(8)).compare("0");}
    bool useBlks (int param) {return QString(params->getParameter(param).at(12)).compare("0");}

protected:
    longParameterModel *params;
    longParameterModel *paramData;

private slots:
};


class setupParamVarModel : public setupLongParamModel
{
    Q_OBJECT
public:
    explicit setupParamVarModel(ss_model *parent = 0);
    ~setupParamVarModel();

signals:
    void paramVarsChanged();

public slots:
    void setParamVarsCount(int rows);// {paramVars->setParamCount(rows);}
//    void setParamVarData(int &row, QString rowstring) {paramVars->setParameter(row, rowstring);}
    void setParamVarData(int row, QStringList &rowstringlist) {paramVars->setParameter(row, rowstringlist);}
//    void setParamVarData(int row, QVector<double> rowdata) {paramVars->setParameter(row, rowdata);}

    int getNumParamVars () {return params->getParamCount();}
    QStringList getParamVarData (int row) {return paramVars->getParameter(row);}
//    QString getParamVarText (int row) {return paramVars->getRowText(row);}
    tablemodel *getParamVarsModel () {return paramVars->getParameters();}

    void setParamVarHeader (int row, QString title) {paramVars->setParamHeader(row, title);}
    QString getParamVarHeader (int row) {return paramVars->getParamHeader(row);}

    void updateParamVars();
//    void updateParamVars(QModelIndex tl, QModelIndex br, QVector<int> data);
    void changeParamVars();
    void updateParamVarsData(QModelIndex tl, QModelIndex br, QVector<int> data);

    void setNumParamVarTables (int num);
    bool generateParamVarData (int numparams);

    void autoGenEnvVarParam (int parmnum, int value);
    void autoGenDevVarParam (int parmnum, int value);
    void autoGenBlkVarParam (int parmnum, int value, int fxn);

protected:
    QList<int> paramNum;
    QList<int> paramvarNum;
    shortParameterModel *paramVars;
    QList<shortParameterModel *> paramVarData;

    QList<int> blkVal;
    QList<int> blkFxn;
    QList<int> devVal;
    QList<int> envVal;

};

#endif // SETUPMODEL_H
