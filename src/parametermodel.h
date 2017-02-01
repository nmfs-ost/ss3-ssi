#ifndef PARAMETERMODEL_H
#define PARAMETERMODEL_H

#include "tablemodel.h"

#include <QStringList>

class shortParameterModel : public QObject
{
    Q_OBJECT

public:
    shortParameterModel (QObject *parent = 0);
    ~shortParameterModel ();

    void setHeader (QStringList hdr);
    QStringList getHeader ();
    void setParamCount(int rows);
    int getParamCount() {return params.rowCount();}
    void setParameter (int row, QStringList param) {params.setRowData(row, param);}
    QStringList getParameter (int row) {return params.getRowData(row);}
    tablemodel *getParameters () {return &params;}
//    QStringList getParameter (int row);
    void setParamHeader (int row, QString hdr) {params.setRowHeader (row, hdr);}
    QString getParamHeader (int row) {return params.getRowHeader(row);}

    tablemodel params;

signals:
    void dataChanged(QModelIndex, QModelIndex, QVector<int>);
    void dataChanged();

protected:
    QStringList header;
};

class longParameterModel : public shortParameterModel
{
    Q_OBJECT

public:
    longParameterModel(QObject *parent = 0);
    ~longParameterModel();

    void setParamCount(int rows);

    bool envLink (int index);
    bool useBlock (int index);
    bool useDev (int index);

private:
};

#endif // PARAMETERMODEL_H
