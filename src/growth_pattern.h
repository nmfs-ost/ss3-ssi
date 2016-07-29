#ifndef GROWTH_PATTERN_H
#define GROWTH_PATTERN_H

#include "growth_morph.h"
#include "parametermodel.h"

class growthPattern
{
public:
    growthPattern();
    growthPattern(const growthPattern &rhs);
    ~growthPattern();

    int getNum_morphs() const;
    void setNum_morphs(int value);

    growth_morph * getMorph(int index) const;
    void setMorph(int index, growth_morph *&value);

    growthPattern &operator =(const growthPattern &rhs);

    void setNumNatMParams (int num) {natMParams->setRowCount(num);}
    int getNumNatMParams () {return natMParams->rowCount();}
    void addNatMParam (QStringList data) {setNatMParam(natMParams->rowCount(), data);}
    void setNatMParam (QStringList data) {setNatMParam(0, data);}
    void setNatMParam (int index, QStringList data);
    QStringList getNatMParam(int index) {return natMParams->getRowData(index);}
    tablemodel *getNatMParams() {return natMParams;}
    void setNatMMaleAgeList (QStringList data) {natMAges->setRowData(1, data);}
    QStringList getNatMMaleAgeList () {return natMAges->getRowData(1);}
    void setNatMFemAgeList (QStringList data) {natMAges->setRowData (0, data);}
    QStringList getNatMFemAgeList () {return natMAges->getRowData(0);}
    tablemodel *getNatMAges () {return natMAges;}

    void setFractionFemaleParam (QStringList data) {fractionFemParams->setRowData (0, data);}
    QStringList getFractionFemaleParam () {return fractionFemParams->getRowData(0);}
    tablemodel *getFractionFemaleParams () {return fractionFemParams;}

    void setNumGrowthParams (int num) {growthParams->setRowCount(num);}
    int getNumGrowthParams () {return growthParams->rowCount();}
    void addGrowthParam (QStringList data) {setGrowthParam(growthParams->rowCount(), data);}
    void setGrowthParam (int index, QStringList data);
    QStringList getGrowthParam(int index) {return growthParams->getRowData(index);}
    tablemodel *getGrowthParams() {return growthParams;}

    void setNumTimeVaryParams (int num) {timeVaryParams->setRowCount(num);}
    int getNumTimeVaryParams () {return timeVaryParams->rowCount();}
    void addTimeVaryParam (QStringList data) {setGrowthParam(timeVaryParams->rowCount(), data);}
    void setTimeVaryParam (int index, QStringList data);
    QStringList getTimeVaryParam(int index) {return timeVaryParams->getRowData(index);}
    tablemodel *getTimeVaryParams() {return timeVaryParams;}

    void setNumCVParams (int num) {cvParams->setRowCount(num);}
    int getNumCVParams () {return cvParams->rowCount();}
    void addCVParam (QStringList data) {setCVParam(cvParams->rowCount(), data);}
    void setCVParam (int index, QStringList data);
    QStringList getCVParam(int index) {return cvParams->getRowData(index);}
    tablemodel *getCVParams() {return cvParams;}

    void setNumDevParams (int num) {devParams->setRowCount(num);}
    int getNumDevParams () {return devParams->rowCount();}
    void addDevParam (QStringList data) {setDevParam(devParams->rowCount(), data);}
    void setDevParam (int index, QStringList data);
    QStringList getDevParam(int index) {return devParams->getRowData(index);}
    tablemodel *getDevParams() {return devParams;}

private:

    int num_morphs;
    QList<growth_morph *> morphs;

    void clear();
    growthPattern& copy (const growthPattern &rhs);

    parametermodel *fractionFemParams;

    parametermodel *devParams;

    parametermodel *growthParams;
    parametermodel *cvParams;
    parametermodel *cohortDevParams;
    parametermodel *maturityParams;

    parametermodel *natMParams;
    tablemodel *natMAges;
    parametermodel *timeVaryParams;
};

#endif // GROWTH_PATTERN_H
