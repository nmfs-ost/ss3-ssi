#ifndef PARAMETERMODEL_H
#define PARAMETERMODEL_H

#include "tablemodel.h"

#include <QStringList>

class shortParameterModel : public tablemodel
{
public:
    shortParameterModel (QObject *parent = 0);
    ~shortParameterModel ();

private:
    QStringList header;
};

class parameterModel : public tablemodel
{
public:
    parameterModel(QObject *parent = 0);
    ~parameterModel();

    bool envLink (int index);
    bool useBlock (int index);
    bool useDev (int index);

private:
    QStringList header;
};

#endif // PARAMETERMODEL_H
