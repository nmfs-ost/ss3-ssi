/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef PARAMETERMODELTV_H
#define PARAMETERMODELTV_H

#include "parametermodel.h"
//#include "model.h"

#include <QStringList>

class ss_model;

class parameterModelTV : public longParameterTable
{
    Q_OBJECT
public:
    parameterModelTV(ss_model *parent = 0);
    ~parameterModelTV();

    bool envLink (int index);
    bool blkNum (int index);
    bool useDevs (int index);

public slots:
    void setParentModel (ss_model *pModel);

//    tablemodel *getParameters () {return params;}
    void setNumParams(int rows);
    void setParameter (int index, QStringList data);
    QStringList getParameter (int index) {return paramTable.getRowData(index);}
    QString getParamText (int index) {return paramTable.getRowText(index);}
    void setParamHeader (int row, QString title);
    int getNumParams() {return paramTable.rowCount();}
    QString getParamHeader (int row) {return paramTable.getRowHeader(row);}


    void modelChanged();
    void changeData();
    void updateParamData();
    void changeData(QModelIndex tplt, QModelIndex btrt, QVector<int> ivect = QVector<int>());
    void changeData(int firstRow, int firstCol, int lastRow, int lastCol);
    void insertData(QModelIndex index, int first, int last);
    void removeData(QModelIndex index, int first, int last);

    tablemodel *getTimeVaryParams() {return timeVaryParams.getParameters();}
    void updateTimeVaryData();
    void updateTimeVaryModel();
    void changeTimeVaryData();
    void changeTimeVaryData(QModelIndex tplt, QModelIndex btrt, QVector<int> ivect = QVector<int>());
    void changeTimeVaryData(int firstRow, int lastRow);

    void setTimeVaryParam (int index, QStringList data);
    void setTimeVaryBlkParam (int param, int num, QStringList data);
    void setTimeVaryDevParam (int param, int num, QStringList data);
    void setTimeVaryEnvParam (int param, QStringList data);
    void setTimeVaryHeaders (int param = -1, QString label = QString(""));
    void setEnvTimeVaryHeader (int param, int link = 0, QString label = QString(""));
    void setDevTimeVaryHeader (int param, int flag = 0, QString label = QString(""));
    void setBlkTimeVaryHeader (int param, int block = 0, QString label = QString(""));

    int getNumTimeVaryParams ();
    QStringList getTimeVaryParam (int row);
    QString getTimeVaryParamHdr (int row);

signals:
    void paramsChanged();

private:
    ss_model *parentModel;

 //   QStringList header;

//    parameterModel *params;
    QList<QStringList> paramData;
    QList<int> useEnv;
    QList<int> devValue;
    QList<int> useBlk;

    shortParameterTable timeVaryParams;
    QList<shortParameterTable *> timeVaryParamData;
    QList<int> tableNum;
    QList<int> paramNum;

    QStringList autoGenBlkParam(int param, int blkPat, QString label);
    QStringList autoGenDevParam(int param, int devVal, QString label);
    QStringList autoGenEnvParam(int param, int envVal, QString label);
};

#endif // PARAMETERMODELTV_H
