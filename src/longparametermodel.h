#ifndef LONGPARAMETERMODEL_H
#define LONGPARAMETERMODEL_H

#include "shortparametermodel.h"

#include <QStringList>
#include <QObject>

class LongParameterModel : public QObject
{
    Q_OBJECT
public:
    explicit LongParameterModel(QObject *parent = nullptr);

signals:


private:
    QList<QStringList> paramData;
    tablemodel *paramTable;

    QList<ShortParameterModel *> timevaryParams;
};

#endif // LONGPARAMETERMODEL_H
