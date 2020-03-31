#ifndef DAR1_H
#define DAR1_H

#include <QStringList>

#include "tablemodel.h"

class DAR1
{
public:
    DAR1();
    ~DAR1();

public slots:
    void reset();

    bool getUse();
    void setUse(bool use);

    void setSpec(QStringList data);
    QStringList getSpec();
    tablemodel *getSpecTable();

    void setParam(int row, QStringList data);
    QStringList getParam (int row);
    tablemodel *getParamTable();

private:
    bool active;
    tablemodel *specification;
    tablemodel *parameters;
};

#endif // DAR1_H
