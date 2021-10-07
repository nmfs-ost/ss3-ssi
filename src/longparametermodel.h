#ifndef LONGPARAMETERMODEL_H
#define LONGPARAMETERMODEL_H

#include "shortparametermodel.h"

#include <QStringList>
#include <QObject>

class LongParameterModel : public ShortParameterModel
{
    Q_OBJECT
public:
    explicit LongParameterModel(QObject *parent = nullptr);

public slots:
    int getNumTimeVaryParameters();
    void setTimeVaryParameter (int i, QStringList data);
    QStringList getTimeVaryParameter (int i);
    QString getTimeVaryParameterHeader(int i);
    void setTimeVaryParameterHeader(int i, QString hdr);
    tablemodel *getTimeVaryParameterModel ();

signals:


private:
//    QList<QStringList> paramData;
//    tablemodel *paramTable;

    QList<ShortParameterModel *> timevaryParamData;
    tablemodel *timevaryParamTable;
};

#endif // LONGPARAMETERMODEL_H
