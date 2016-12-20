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
    void setNumNatMTVParams (int num) {natMTVParams->setRowCount(num);}
    int getNumNatMTVParams () {return natMTVParams->rowCount();}
    void addNatMTVParam (QStringList data) {setNatMParam(natMTVParams->rowCount(), data);}
    void setNatMTVParam (QStringList data) {setNatMParam(0, data);}
    void setNatMTVParam (int index, QStringList data);
    QStringList getNatMTVParam(int index) {return natMTVParams->getRowData(index);}
    tablemodel *getNatMTVParams() {return natMTVParams;}
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
    void setNumGrowthTVParams (int num) {growthTVParams->setRowCount(num);}
    int getNumGrowthTVParams () {return growthTVParams->rowCount();}
    void addGrowthTVParam (QStringList data) {setGrowthParam(growthTVParams->rowCount(), data);}
    void setGrowthTVParam (int index, QStringList data);
    QStringList getGrowthTVParam(int index) {return growthTVParams->getRowData(index);}
    tablemodel *getGrowthTVParams() {return growthTVParams;}

    void setNumTimeVaryParams (int num) {timeVaryParams->setRowCount(num);}
    int getNumTimeVaryParams () {return timeVaryParams->rowCount();}
    void addTimeVaryParam (QStringList data) {setGrowthParam(timeVaryParams->rowCount(), data);}
    void setTimeVaryParam (int index, QStringList data);
    void setTimeVaryParamHeader (int index, QString header);
    QStringList getTimeVaryParam(int index) {return timeVaryParams->getRowData(index);}
    QString getTimeVaryParamHeader (int index) {return timeVaryParams->getRowHeader(index);}
    tablemodel *getTimeVaryParams() {return timeVaryParams;}

    void setNumCVParams (int num) {cvParams->setRowCount(num);}
    int getNumCVParams () {return cvParams->rowCount();}
    void addCVParam (QStringList data) {setCVParam(cvParams->rowCount(), data);}
    void setCVParam (int index, QStringList data);
    QStringList getCVParam(int index) {return cvParams->getRowData(index);}
    tablemodel *getCVParams() {return cvParams;}
    void setNumCVTVParams (int num) {cvTVParams->setRowCount(num);}
    int getNumCVTVParams () {return cvTVParams->rowCount();}
    void addCVTVParam (QStringList data) {setCVParam(cvTVParams->rowCount(), data);}
    void setCVTVParam (int index, QStringList data);
    QStringList getCVTVParam(int index) {return cvTVParams->getRowData(index);}
    tablemodel *getCVTVParams() {return cvTVParams;}

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

    parameterModelTV *fractionFemParams;

    parameterModelTV *devParams;

    parameterModelTV *growthParams;
    parameterModelTV *growthTVParams;
    parameterModelTV *cvParams;
    parameterModelTV *cvTVParams;
    parameterModelTV *cohortDevParams;
    parameterModelTV *cohortTVDevParams;
    parameterModelTV *maturityParams;
    parameterModelTV *maturityTVParams;

    parameterModelTV *natMParams;
    parameterModelTV *natMTVParams;
    tablemodel *natMAges;
    parameterModelTV *timeVaryParams;
};

#endif // GROWTH_PATTERN_H
