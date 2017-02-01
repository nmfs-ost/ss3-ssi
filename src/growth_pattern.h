#ifndef GROWTH_PATTERN_H
#define GROWTH_PATTERN_H

#include "growth_morph.h"
#include "parametermodelTV.h"
#include "setupmodel.h"

class growthPattern
{
public:
    growthPattern(ss_model *parent);
    growthPattern(const growthPattern &rhs);
    ~growthPattern();

    int getNum_morphs() const;
    void setNum_morphs(int value);

    growth_morph * getMorph(int index) const;
    void setMorph(int index, growth_morph *&value);

    growthPattern &operator =(const growthPattern &rhs);

    setupParamVarModel *getGrowthParamsModel() {return growthParams;}

    void setNumNatMParams (int num) {natMParams->setNumParams(num);}
    int getNumNatMParams () {return natMParams->getNumParams();}
    void addNatMParam (QStringList data) {setNatMParam(natMParams->getNumParams(), data);}
    void setNatMParam (QStringList data) {setNatMParam(0, data);}
    void setNatMParam (int index, QStringList data);
    QStringList getNatMParam(int index) {return natMParams->getParamData(index);}
    tablemodel *getNatMParams() {return natMParams->getParamModel();}
    void setNumNatMTVParams (int num) {natMParams->getParamVarsModel()->setRowCount(num);}
    int getNumNatMTVParams () {return natMParams->getNumParams();}
//    void addNatMTVParam (QStringList data) {setNatMParam(natMTVParams->getParamCount(), data);}
    void setNatMTVParam (QStringList data) {setNatMParam(0, data);}
    void setNatMTVParam (int index, QStringList data);
    QStringList getNatMTVParam(int index) {return natMParams->getParamVarData(index);}
    tablemodel *getNatMTVParams() {return natMParams->getParamVarsModel();}
    void setNatMMaleAgeList (QStringList data) {natMAges->setRowData(1, data);}
    QStringList getNatMMaleAgeList () {return natMAges->getRowData(1);}
    void setNatMFemAgeList (QStringList data) {natMAges->setRowData (0, data);}
    QStringList getNatMFemAgeList () {return natMAges->getRowData(0);}
    tablemodel *getNatMAges () {return natMAges;}

    void setFractionFemaleParam (QStringList data) {fractionFemParams->setParameter(0, data);}
    QStringList getFractionFemaleParam () {return fractionFemParams->getParameter(0);}
    tablemodel *getFractionFemaleParams () {return fractionFemParams->getParameters();}//->getParameters();}

    void setNumGrowthParams (int num) {growthParams->setNumParams(num);}
    int getNumGrowthParams () {return growthParams->getNumParams();}
    void addGrowthParam (QStringList data) {setGrowthParam(growthParams->getNumParams(), data);}
    void setGrowthParam (int index, QStringList data);
    QStringList getGrowthParam(int index) {return growthParams->getParamData(index);}
    tablemodel *getGrowthParams() {return growthParams->getParamModel();}
    void setNumGrowthTVParams (int num) {growthParams->getParamVarsModel()->setRowCount(num);}
    int getNumGrowthTVParams () {return growthParams->getNumParamVars();}
    void addGrowthTVParam (QStringList data) {setGrowthTVParam(growthParams->getNumParamVars(), data);}
    void setGrowthTVParam (int index, QStringList data);
    QStringList getGrowthTVParam(int index) {return growthParams->getParamVarData(index);}
    tablemodel *getGrowthTVParams() {return growthParams->getParamVarsModel();}

/*    void setNumTimeVaryParams (int num) {timeVaryParams->setParamCount(num);}
    int getNumTimeVaryParams () {return timeVaryParams->getParamCount();}
    void addTimeVaryParam (QStringList data) {setGrowthParam(timeVaryParams->getParamCount(), data);}
    void setTimeVaryParam (int index, QStringList data);
    void setTimeVaryParamHeader (int index, QString header);
    QStringList getTimeVaryParam(int index) {return timeVaryParams->getParameter(index);}
    QString getTimeVaryParamHeader (int index) {return timeVaryParams->getParamHeader(index);}
    tablemodel *getTimeVaryParams() {return timeVaryParams->getParameters();}*/

    void setNumCVParams (int num) {cvParams->setNumParams(num);}
    int getNumCVParams () {return cvParams->getNumParams();}
    void addCVParam (QStringList data) {setCVParam(cvParams->getNumParams(), data);}
    void setCVParam (int index, QStringList data);
    QStringList getCVParam(int index) {return cvParams->getParamData(index);}
    tablemodel *getCVParams() {return cvParams->getParamModel();}
    void setNumCVTVParams (int num) {cvParams->getParamVarsModel()->setRowCount(num);}
    int getNumCVTVParams () {return cvParams->getNumParamVars();}
    void addCVTVParam (QStringList data) {setCVTVParam(getNumCVTVParams(), data);}
    void setCVTVParam (int index, QStringList data);
    QStringList getCVTVParam(int index) {return cvParams->getParamVarData(index);}
    tablemodel *getCVTVParams() {return cvParams->getParamVarsModel();}

    void setNumDevParams (int num) {devParams->setNumParams(num);}
    int getNumDevParams () {return devParams->getNumParams();}
    void addDevParam (QStringList data) {setDevParam(devParams->getNumParams(), data);}
    void setDevParam (int index, QStringList data);
    QStringList getDevParam(int index) {return devParams->getParamData(index);}
    tablemodel *getDevParams() {return devParams->getParamModel();}

private:
    ss_model *parnt;

    int num_morphs;
    QList<growth_morph *> morphs;

    void clear();
    growthPattern& copy (const growthPattern &rhs);

    longParameterModel *fractionFemParams;

    setupParamVarModel *devParams;
//    parameterModelTV *devParams;

    setupParamVarModel *growthParams;
//    parameterModelTV *growthParams;
//    shortParameterModel *growthTVParams;
    setupParamVarModel *cvParams;
//    parameterModelTV *cvParams;
//    shortParameterModel *cvTVParams;
    setupParamVarModel *cohortDevParams;
//    parameterModelTV *cohortDevParams;
//    shortParameterModel *cohortTVDevParams;
    setupParamVarModel *maturityParams;
//    parameterModelTV *maturityParams;
//    shortParameterModel *maturityTVParams;

    setupParamVarModel *natMParams;
//    parameterModelTV *natMParams;
//    shortParameterModel *natMTVParams;
    tablemodel *natMAges;
//    shortParameterModel *timeVaryParams;
};

#endif // GROWTH_PATTERN_H
