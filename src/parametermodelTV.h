#ifndef PARAMETERMODEL_H
#define PARAMETERMODEL_H

#include "tablemodel.h"
#include "model.h"

#include <QStringList>

class shortparametermodel : public tablemodel
{
public:
    shortparametermodel (QObject *parent = 0);
    ~shortparametermodel ();

private:
    QStringList header;
};

class parametermodel : public tablemodel
{
public:
    parametermodel(QObject *parent = 0);
    ~parametermodel();

    bool envLink (int index);
    bool useBlock (int index);
    bool useDev (int index);

public slots:
    shortparametermodel *getTimeVarModel() {return timeVarianceTable;}
    void updateTimeVarTable();
    void modelChanged();
    void changeData();
    void changeData(QModelIndex tplt, QModelIndex btrt, QVector<int> ivect = QVector<int>());
    void insertData(QModelIndex index, int first, int last);
    void removeData(QModelIndex index, int first, int last);

signals:
    void tableChanged();

private:
    ss_model *parentModel;

    QStringList header;

    QList<int> blkLinks;
    QList<bool> devLinks;
    QList<bool> envLinks;

    QList<shortparametermodel *> timeVarianceList;
    shortparametermodel *timeVarianceTable;
    QStringList autoGenBlkParam(int param, int block, QString label);
    QStringList autoGenDevParam(int param, bool value, QString label);
    QStringList autoGenEnvParam(int param, bool value, QString label);
};

#endif // PARAMETERMODEL_H
