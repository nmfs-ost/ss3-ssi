#ifndef PARAMETERMODELTV_H
#define PARAMETERMODELTV_H

#include "parametermodel.h"
//#include "model.h"

#include <QStringList>

class ss_model;

class parameterModelTV : public parameterModel
{
    Q_OBJECT
public:
    parameterModelTV(ss_model *parent = 0);
    ~parameterModelTV();

    bool envLink (int index);
    bool useBlock (int index);
    bool useDev (int index);

public slots:
    void setParentModel (ss_model *pModel);

    parameterModel *getParameters () {return this;}
//    int rowCount() {return rowCount();}
//    void setRowCount(int num) {setRowCount(num);}
//    QStringList getRowData (int row) {return getRowData(row);}
//    void setRowData (int row, QStringList data) {setRowData(row, data);}
//    QString getRowText (int row) {return getRowText(row);}
//    void setRowText (int row, QString txt) {params->setRowText(row, txt);}
//    QString getRowHeader (int row) {return getRowHeader(row);}
//    void setRowHeader (int row, QString hdr) {setRowHeader(row, hdr);}

    shortParameterModel *getTimeVarParameters() {return timeVarianceTable;}
    void updateTimeVarTable();
    void modelChanged();
    void changeData();
    void changeData(QModelIndex tplt, QModelIndex btrt, QVector<int> ivect = QVector<int>());
    void insertData(QModelIndex index, int first, int last);
    void removeData(QModelIndex index, int first, int last);

    void setTimeVarBlkParam (int param, int num, QStringList data);
    void setTimeVarDevParam (int param, int num, QStringList data);
    void setTimeVarEnvParam (int param, QStringList data);

    int getNumTimeVarParams ();
    QStringList getTimeVarParam (int row);
    QString getTimeVarParamHdr (int row);

signals:
   // void tableChanged();

private:
    ss_model *parentModel;

    QStringList header;

//    parameterModel *params;
    int blkst;
    QList<int> blkLinks;
    QList<int> devLinks;
    QList<int> envLinks;

    QList<shortParameterModel *> timeVarianceList;
    shortParameterModel *timeVarianceTable;
    QStringList autoGenBlkParam(int param, int block, QString label);
    QStringList autoGenDevParam(int param, int value, QString label);
    QStringList autoGenEnvParam(int param, int value, QString label);
};

#endif // PARAMETERMODELTV_H
