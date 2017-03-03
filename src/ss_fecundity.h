#ifndef SS_FECUNDITY_H
#define SS_FECUNDITY_H

#include <QStringList>

#include "parametermodel.h"
#include "setupmodel.h"

class ss_model;

class ss_fecundity
{
public:
    ss_fecundity(ss_model *parent = 0);
    ~ss_fecundity();

    void reset();

    int getMethod() const;
    void setMethod(int value);

    void setHermParam (int index, QStringList data);
    QStringList getHermParam(int index) {return hermaphParams->getParameter(index);}
    tablemodel *getHermParams() {return hermaphParams->getParamTable();}

    void setFemaleParam (int index, QStringList data);
    QStringList getFemaleParam(int index) {return femaleParams->getParameter(index);}
    tablemodel *getFemaleParams() {return femaleParams->getParamTable();}

    int getHermIncludeMales() const;
    void setHermIncludeMales(int value);

    int getHermSeason() const;
    void setHermSeason(int value);

    bool getHermaphroditism() const;
    void setHermaphroditism(int value);

private:
    ss_model *parnt;
    int method;
    int hermaphroditism;
    int hermSeason;
    int hermIncludeMales;

    QString pHeader;
    longParameterModel *hermaphParams;
    longParameterModel *femaleParams;
};

#endif // SS_FECUNDITY_H
