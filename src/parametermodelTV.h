#ifndef PARAMETERMODELTV_H
#define PARAMETERMODELTV_H

#include "parametermodel.h"
//#include "model.h"

#include <QStringList>

class ss_model;

class parameterModelTV : public parameterModel
{
public:
    parameterModelTV(QObject *parent = 0);
    ~parameterModelTV();

    bool envLink (int index);
    bool useBlock (int index);
    bool useDev (int index);

public slots:
    shortParameterModel *getTimeVarModel() {return timeVarianceTable;}
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
    void tableChanged();

private:
    ss_model *parentModel;

    QStringList header;

    QList<int> blkLinks;
    QList<bool> devLinks;
    QList<bool> envLinks;

    QList<shortParameterModel *> timeVarianceList;
    shortParameterModel *timeVarianceTable;
    QStringList autoGenBlkParam(int param, int block, QString label);
    QStringList autoGenDevParam(int param, bool value, QString label);
    QStringList autoGenEnvParam(int param, bool value, QString label);
};

#endif // PARAMETERMODELTV_H
