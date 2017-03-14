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
    void setHermTVParam (int index, QStringList data);
    QStringList getHermTVParam (int index) {return hermaphVarParams->getVarParameter(index);}
    tablemodel *getHermTVParams () {return hermaphVarParams->getVarParamTable();}

    void setFemaleParam (int index, QStringList data);
    QStringList getFemaleParam(int index) {return femaleParams->getParameter(index);}
    tablemodel *getFemaleParams() {return femaleParams->getParamTable();}
    void setFemaleTVParam (int index, QStringList data);
    QStringList getFemaleTVParam(int index) {return femaleVarParams->getVarParameter(index);}
    tablemodel *getFemaleTVParams() {return femaleVarParams->getVarParamTable();}

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
    timeVaryParameterModel *hermaphVarParams;
    longParameterModel *femaleParams;
    timeVaryParameterModel *femaleVarParams;
};

#endif // SS_FECUNDITY_H
