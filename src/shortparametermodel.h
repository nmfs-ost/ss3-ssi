#ifndef SHORTPARAMETERMODEL_H
#define SHORTPARAMETERMODEL_H

#include "tablemodel.h"

#include <QObject>
#include <QStringList>

class ShortParameterModel : public QObject
{
    Q_OBJECT

public:
    explicit ShortParameterModel(QObject *parent = nullptr);
    ~ShortParameterModel () override;

    void resetToData();

signals:
    void parametersChanged();
    void parameterChanged(int parm, QStringList data);

public slots:
    void setParameterColHeader (QStringList colhdrs);
    void setTotalNumParameters (int rows);
    int getTotalNumParameters ();

    void setParametersUsed (QList<int> data);
    void setParameterUsed (int i, bool flag = true);
    int getNumParameters ();

    void setParameterData(int row, QStringList &rowstringlist);
    void setParameter(int row, QStringList &rowstringlist);
    QStringList getParameter (int row) {return paramTable->getRowData(row);}
    void setParameterHeader (int row, QString title);
    QString getParameterHeader (int row) {return paramTable->getRowHeader(row);}
    tablemodel *getParameterTable () {return paramTable;}

    void updateParameters ();
    void updateParamHeaders ();
    void checkParameterData ();
    void updateParameterData (QModelIndex tl, QModelIndex br, QVector<int> data);

protected:
    QList<bool> paramUsed;
    QList<int> paramNum;
    tablemodel *paramTable;
    QList<QStringList> paramData;
    QStringList colHeader;
    QStringList defaultParam;

};

class ShortParameterMultiModel : public ShortParameterModel
{
    Q_OBJECT

public:
    explicit ShortParameterMultiModel(QObject *parent = nullptr);

public slots:
    void setMultiplier(int newmult);
    int getMultiplier() {return mult;}

private:
    int mult;

};

#endif // SHORTPARAMETERMODEL_H
